// window.c for the Openbox window manager

#include "window.h"
#include "menuframe.h"
#include "config.h"
#include "dock.h"
#include "client.h"
#include "frame.h"
#include "openbox.h"
#include "prompt.h"
#include "debug.h"
#include "grab.h"
#include "obt/prop.h"
#include "obt/xqueue.h"

static GHashTable *window_map;

static guint window_hash(Window *w) { return *w; }
static gboolean window_comp(Window *w1, Window *w2) { return *w1 == *w2; }

void window_startup(gboolean reconfig)
{
    if (reconfig) return;

    window_map = g_hash_table_new((GHashFunc)window_hash,
                                  (GEqualFunc)window_comp);
}

void window_shutdown(gboolean reconfig)
{
    if (reconfig) return;

    g_hash_table_destroy(window_map);
}

Window window_top(ObWindow *self)
{
    if (self == NULL) {
        ob_debug("Attempted to access a NULL window in window_top");
        return None;
    }

    switch (self->type) {
    case OB_WINDOW_CLASS_MENUFRAME:
        return WINDOW_AS_MENUFRAME(self)->window;
    case OB_WINDOW_CLASS_DOCK:
        return WINDOW_AS_DOCK(self)->frame;
    case OB_WINDOW_CLASS_CLIENT:
        return WINDOW_AS_CLIENT(self)->frame->window;
    case OB_WINDOW_CLASS_INTERNAL:
        return WINDOW_AS_INTERNAL(self)->window;
    case OB_WINDOW_CLASS_PROMPT:
        return WINDOW_AS_PROMPT(self)->super.window;
    default:
        ob_debug("Unknown window type: %d", self->type);
        g_assert_not_reached();
        return None;
    }
}

ObStackingLayer window_layer(ObWindow *self)
{
    if (self == NULL) {
        ob_debug("Attempted to access a NULL window in window_layer");
        return None;
    }

    ob_debug("Accessing window %p in window_layer", self);

    switch (self->type) {
    case OB_WINDOW_CLASS_DOCK:
        return config_dock_layer;

    case OB_WINDOW_CLASS_CLIENT:
        if (self) {
            ObClient *client = (ObClient*)self;
            ob_debug("Accessing client %p in window_layer", client);
            return client->layer;
        }
        ob_debug("Client is NULL in window_layer");
        return None;

    case OB_WINDOW_CLASS_MENUFRAME:
    case OB_WINDOW_CLASS_INTERNAL:
        return OB_STACKING_LAYER_INTERNAL;

    case OB_WINDOW_CLASS_PROMPT:
        /* Prompts are managed as clients, not directly for stacking */
        g_assert_not_reached();
        break;
    }

    g_assert_not_reached();
    return None;
}

ObWindow* window_find(Window xwin)
{
    return g_hash_table_lookup(window_map, &xwin);
}

void window_add(Window *xwin, ObWindow *win)
{
    g_assert(xwin != NULL);
    g_assert(win != NULL);

    /* Add to window map */
    g_hash_table_insert(window_map, xwin, win);
}

void window_remove(Window xwin)
{
    g_assert(xwin != None);

    /* Remove from window map */
    g_hash_table_remove(window_map, &xwin);
}

void window_manage_all(void)
{
    guint i, j, nchild;
    Window w, *children;
    XWMHints *wmhints;
    XWindowAttributes attrib;

    if (!XQueryTree(obt_display, RootWindow(obt_display, ob_screen),
                    &w, &w, &children, &nchild)) {
        ob_debug("XQueryTree failed in window_manage_all");
        nchild = 0;
    }

    /* Remove all icon windows from the list */
    for (i = 0; i < nchild; i++) {
        if (children[i] == None) continue;
        wmhints = XGetWMHints(obt_display, children[i]);
        if (wmhints) {
            if ((wmhints->flags & IconWindowHint) &&
                (wmhints->icon_window != children[i]))
                for (j = 0; j < nchild; j++)
                    if (children[j] == wmhints->icon_window) {
                        /* XXX Watch the window */
                        children[j] = None;
                        break;
                    }
            XFree(wmhints);
        }
    }

    /* Manage non-client windows */
    for (i = 0; i < nchild; ++i) {
        if (children[i] == None) continue;
        if (window_find(children[i])) continue; /* Skip our own windows */
        if (XGetWindowAttributes(obt_display, children[i], &attrib)) {
            if (attrib.map_state != IsUnmapped)
                window_manage(children[i]);
        }
    }

    if (children) XFree(children);
}

static gboolean check_unmap(XEvent *e, gpointer data)
{
    const Window win = *(Window*)data;
    return ((e->type == DestroyNotify && e->xdestroywindow.window == win) ||
            (e->type == UnmapNotify && e->xunmap.window == win));
}

void window_manage(Window win)
{
    XWindowAttributes attrib;
    gboolean no_manage = FALSE;
    gboolean is_dockapp = FALSE;
    Window icon_win = None;

    grab_server(TRUE);

    /* Check if the window is unmapped by the time we start mapping */
    if (xqueue_exists_local(check_unmap, &win)) {
        ob_debug("Trying to manage unmapped window. Aborting that.");
        no_manage = TRUE;
    } else if (!XGetWindowAttributes(obt_display, win, &attrib)) {
        no_manage = TRUE;
    } else {
        XWMHints *wmhints;

        /* Check if the window is a docking app */
        is_dockapp = FALSE;
        if ((wmhints = XGetWMHints(obt_display, win))) {
            if ((wmhints->flags & StateHint) &&
                wmhints->initial_state == WithdrawnState) {
                if (wmhints->flags & IconWindowHint)
                    icon_win = wmhints->icon_window;
                is_dockapp = TRUE;
            }
            XFree(wmhints);
        }

        /* Dock app check through WM_CLASS property */
        if (!is_dockapp) {
            gchar **ss;
            if (OBT_PROP_GETSS_TYPE(win, WM_CLASS, STRING_NO_CC, &ss)) {
                if (ss[0] && ss[1] && strcmp(ss[1], "DockApp") == 0)
                    is_dockapp = TRUE;
                g_strfreev(ss);
            }
        }
    }

    if (!no_manage) {
        if (attrib.override_redirect) {
            ob_debug("Not managing override redirect window 0x%x", win);
            grab_server(FALSE);
        } else if (is_dockapp) {
            if (!icon_win)
                icon_win = win;
            dock_manage(icon_win, win);
        } else {
            client_manage(win, NULL);
        }
    } else {
        grab_server(FALSE);
        ob_debug("FAILED to manage window 0x%x", win);
    }
}

void window_unmanage_all(void)
{
    dock_unmanage_all();
    client_unmanage_all();
}
