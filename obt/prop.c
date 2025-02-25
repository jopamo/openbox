/* -*- indent-tabs-mode: nil; tab-width: 4; c-basic-offset: 4; -*-

   obt/prop.c for the Openbox window manager
   Copyright (c) 2006        Mikael Magnusson
   Copyright (c) 2003-2007   Dana Jansens

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   See the COPYING file for a copy of the GNU General Public License.
*/

#include "obt/prop.h"
#include "obt/display.h"

#include <X11/Xatom.h>
#ifdef HAVE_STRING_H
#  include <string.h>
#endif

Atom prop_atoms[OBT_PROP_NUM_ATOMS];
gboolean prop_started = FALSE;

#define CREATE_NAME(var, name) (prop_atoms[OBT_PROP_##var] = \
                                XInternAtom((obt_display), (name), FALSE))
#define CREATE(var) CREATE_NAME(var, #var)
#define CREATE_(var) CREATE_NAME(var, "_" #var)

void obt_prop_startup(void)
{
    if (prop_started) return;
    prop_started = TRUE;

    g_assert(obt_display);

    CREATE(CARDINAL);
    CREATE(WINDOW);
    CREATE(PIXMAP);
    CREATE(ATOM);
    CREATE(STRING);
    CREATE(COMPOUND_TEXT);
    CREATE(UTF8_STRING);

    CREATE(MANAGER);

    CREATE(WM_COLORMAP_WINDOWS);
    CREATE(WM_PROTOCOLS);
    CREATE(WM_STATE);
    CREATE(WM_CHANGE_STATE);
    CREATE(WM_DELETE_WINDOW);
    CREATE(WM_TAKE_FOCUS);
    CREATE(WM_NAME);
    CREATE(WM_ICON_NAME);
    CREATE(WM_CLASS);
    CREATE(WM_WINDOW_ROLE);
    CREATE(WM_CLIENT_MACHINE);
    CREATE(WM_COMMAND);
    CREATE(WM_CLIENT_LEADER);
    CREATE(WM_TRANSIENT_FOR);
    CREATE_(MOTIF_WM_HINTS);
    CREATE_(MOTIF_WM_INFO);

    CREATE(SM_CLIENT_ID);

    CREATE_(NET_WM_FULL_PLACEMENT);

    CREATE_(NET_SUPPORTED);
    CREATE_(NET_CLIENT_LIST);
    CREATE_(NET_CLIENT_LIST_STACKING);
    CREATE_(NET_NUMBER_OF_DESKTOPS);
    CREATE_(NET_DESKTOP_GEOMETRY);
    CREATE_(NET_DESKTOP_VIEWPORT);
    CREATE_(NET_CURRENT_DESKTOP);
    CREATE_(NET_DESKTOP_NAMES);
    CREATE_(NET_ACTIVE_WINDOW);
/*    CREATE_(NET_RESTACK_WINDOW);*/
    CREATE_(NET_WORKAREA);
    CREATE_(NET_SUPPORTING_WM_CHECK);
    CREATE_(NET_DESKTOP_LAYOUT);
    CREATE_(NET_SHOWING_DESKTOP);

    CREATE_(NET_CLOSE_WINDOW);
    CREATE_(NET_WM_MOVERESIZE);
    CREATE_(NET_MOVERESIZE_WINDOW);
    CREATE_(NET_REQUEST_FRAME_EXTENTS);
    CREATE_(NET_RESTACK_WINDOW);

    CREATE_(NET_STARTUP_ID);

    CREATE_(NET_WM_NAME);
    CREATE_(NET_WM_VISIBLE_NAME);
    CREATE_(NET_WM_ICON_NAME);
    CREATE_(NET_WM_VISIBLE_ICON_NAME);
    CREATE_(NET_WM_DESKTOP);
    CREATE_(NET_WM_WINDOW_TYPE);
    CREATE_(NET_WM_STATE);
    CREATE_(NET_WM_STRUT);
    CREATE_(NET_WM_STRUT_PARTIAL);
    CREATE_(NET_WM_ICON);
    CREATE_(NET_WM_ICON_GEOMETRY);
    CREATE_(NET_WM_PID);
    CREATE_(NET_WM_ALLOWED_ACTIONS);
    CREATE_(NET_WM_WINDOW_OPACITY);
    CREATE_(NET_WM_USER_TIME);
/*  CREATE_(NET_WM_USER_TIME_WINDOW); */
    CREATE_(KDE_NET_WM_FRAME_STRUT);
    CREATE_(NET_FRAME_EXTENTS);

    CREATE_(NET_WM_PING);
#ifdef SYNC
    CREATE_(NET_WM_SYNC_REQUEST);
    CREATE_(NET_WM_SYNC_REQUEST_COUNTER);
#endif

    CREATE_(NET_WM_WINDOW_TYPE_DESKTOP);
    CREATE_(NET_WM_WINDOW_TYPE_DOCK);
    CREATE_(NET_WM_WINDOW_TYPE_TOOLBAR);
    CREATE_(NET_WM_WINDOW_TYPE_MENU);
    CREATE_(NET_WM_WINDOW_TYPE_UTILITY);
    CREATE_(NET_WM_WINDOW_TYPE_SPLASH);
    CREATE_(NET_WM_WINDOW_TYPE_DIALOG);
    CREATE_(NET_WM_WINDOW_TYPE_NORMAL);
    CREATE_(NET_WM_WINDOW_TYPE_POPUP_MENU);

    prop_atoms[OBT_PROP_NET_WM_MOVERESIZE_SIZE_TOPLEFT] = 0;
    prop_atoms[OBT_PROP_NET_WM_MOVERESIZE_SIZE_TOP] = 1;
    prop_atoms[OBT_PROP_NET_WM_MOVERESIZE_SIZE_TOPRIGHT] = 2;
    prop_atoms[OBT_PROP_NET_WM_MOVERESIZE_SIZE_RIGHT] = 3;
    prop_atoms[OBT_PROP_NET_WM_MOVERESIZE_SIZE_BOTTOMRIGHT] = 4;
    prop_atoms[OBT_PROP_NET_WM_MOVERESIZE_SIZE_BOTTOM] = 5;
    prop_atoms[OBT_PROP_NET_WM_MOVERESIZE_SIZE_BOTTOMLEFT] = 6;
    prop_atoms[OBT_PROP_NET_WM_MOVERESIZE_SIZE_LEFT] = 7;
    prop_atoms[OBT_PROP_NET_WM_MOVERESIZE_MOVE] = 8;
    prop_atoms[OBT_PROP_NET_WM_MOVERESIZE_SIZE_KEYBOARD] = 9;
    prop_atoms[OBT_PROP_NET_WM_MOVERESIZE_MOVE_KEYBOARD] = 10;
    prop_atoms[OBT_PROP_NET_WM_MOVERESIZE_CANCEL] = 11;

    CREATE_(NET_WM_ACTION_MOVE);
    CREATE_(NET_WM_ACTION_RESIZE);
    CREATE_(NET_WM_ACTION_MINIMIZE);
    CREATE_(NET_WM_ACTION_SHADE);
    CREATE_(NET_WM_ACTION_MAXIMIZE_HORZ);
    CREATE_(NET_WM_ACTION_MAXIMIZE_VERT);
    CREATE_(NET_WM_ACTION_FULLSCREEN);
    CREATE_(NET_WM_ACTION_CHANGE_DESKTOP);
    CREATE_(NET_WM_ACTION_CLOSE);
    CREATE_(NET_WM_ACTION_ABOVE);
    CREATE_(NET_WM_ACTION_BELOW);

    CREATE_(NET_WM_STATE_MODAL);
/*    CREATE_(NET_WM_STATE_STICKY);*/
    CREATE_(NET_WM_STATE_MAXIMIZED_VERT);
    CREATE_(NET_WM_STATE_MAXIMIZED_HORZ);
    CREATE_(NET_WM_STATE_SHADED);
    CREATE_(NET_WM_STATE_SKIP_TASKBAR);
    CREATE_(NET_WM_STATE_SKIP_PAGER);
    CREATE_(NET_WM_STATE_HIDDEN);
    CREATE_(NET_WM_STATE_FULLSCREEN);
    CREATE_(NET_WM_STATE_ABOVE);
    CREATE_(NET_WM_STATE_BELOW);
    CREATE_(NET_WM_STATE_DEMANDS_ATTENTION);

    prop_atoms[OBT_PROP_NET_WM_STATE_ADD] = 1;
    prop_atoms[OBT_PROP_NET_WM_STATE_REMOVE] = 0;
    prop_atoms[OBT_PROP_NET_WM_STATE_TOGGLE] = 2;

    prop_atoms[OBT_PROP_NET_WM_ORIENTATION_HORZ] = 0;
    prop_atoms[OBT_PROP_NET_WM_ORIENTATION_VERT] = 1;
    prop_atoms[OBT_PROP_NET_WM_TOPLEFT] = 0;
    prop_atoms[OBT_PROP_NET_WM_TOPRIGHT] = 1;
    prop_atoms[OBT_PROP_NET_WM_BOTTOMRIGHT] = 2;
    prop_atoms[OBT_PROP_NET_WM_BOTTOMLEFT] = 3;

    CREATE_(KDE_WM_CHANGE_STATE);
    CREATE_(KDE_NET_WM_WINDOW_TYPE_OVERRIDE);

/*
    CREATE_NAME(ROOTPMAPId, "_XROOTPMAP_ID");
    CREATE_NAME(ESETROOTId, "ESETROOT_PMAP_ID");
*/

    CREATE_(OPENBOX_PID);
    CREATE_(OB_THEME);
    CREATE_(OB_CONFIG_FILE);
    CREATE_(OB_WM_ACTION_UNDECORATE);
    CREATE_(OB_WM_STATE_UNDECORATED);
    CREATE_(OB_CONTROL);
    CREATE_(OB_VERSION);
    CREATE_(OB_APP_ROLE);
    CREATE_(OB_APP_TITLE);
    CREATE_(OB_APP_NAME);
    CREATE_(OB_APP_CLASS);
    CREATE_(OB_APP_GROUP_NAME);
    CREATE_(OB_APP_GROUP_CLASS);
    CREATE_(OB_APP_TYPE);
}

Atom obt_prop_atom(ObtPropAtom a)
{
    g_assert(prop_started);
    g_assert(a < OBT_PROP_NUM_ATOMS);
    return prop_atoms[a];
}

static gboolean get_prealloc(Window win, Atom prop, Atom type, gint size,
                             guchar *data, gulong num)
{
    gboolean ret = FALSE;
    gint res;
    guchar *xdata = NULL;
    Atom ret_type;
    gint ret_size;
    gulong ret_items, bytes_left;
    glong num32 = 32 / size * num; /* num in 32-bit elements */

    res = XGetWindowProperty(obt_display, win, prop, 0l, num32,
                             FALSE, type, &ret_type, &ret_size,
                             &ret_items, &bytes_left, &xdata);
    if (res == Success && ret_items && xdata) {
        if (ret_size == size && ret_items >= num) {
            guint i;
            for (i = 0; i < num; ++i)
                switch (size) {
                case 8:
                    data[i] = xdata[i];
                    break;
                case 16:
                    ((guint16*)data)[i] = ((gushort*)xdata)[i];
                    break;
                case 32:
                    ((guint32*)data)[i] = ((gulong*)xdata)[i];
                    break;
                default:
                    g_assert_not_reached(); /* unhandled size */
                }
            ret = TRUE;
        }
        XFree(xdata);
    }
    return ret;
}

static gboolean get_all(Window win, Atom prop, Atom type, gint size,
                        guchar **data, guint *num)
{
    gboolean ret = FALSE;
    gint res;
    guchar *xdata = NULL;
    Atom ret_type;
    gint ret_size;
    gulong ret_items, bytes_left;

    res = XGetWindowProperty(obt_display, win, prop, 0l, G_MAXLONG,
                             FALSE, type, &ret_type, &ret_size,
                             &ret_items, &bytes_left, &xdata);
    if (res == Success) {
        if (ret_size == size && ret_items > 0) {
            guint i;

            *data = g_malloc(ret_items * (size / 8));
            for (i = 0; i < ret_items; ++i)
                switch (size) {
                case 8:
                    (*data)[i] = xdata[i];
                    break;
                case 16:
                    ((guint16*)*data)[i] = ((gushort*)xdata)[i];
                    break;
                case 32:
                    ((guint32*)*data)[i] = ((gulong*)xdata)[i];
                    break;
                default:
                    g_assert_not_reached(); /* unhandled size */
                }
            *num = ret_items;
            ret = TRUE;
        }
        XFree(xdata);
    }
    return ret;
}

/*! Get a text property from a window, and fill out the XTextProperty with it.
  @param win The window to read the property from.
  @param prop The atom of the property to read off the window.
  @param tprop The XTextProperty to fill out.
  @param type 0 to get text of any type, or a value from
    ObtPropTextType to restrict the value to a specific type.
  @return TRUE if the text was read and validated against the @type, and FALSE
    otherwise.
*/
static gboolean get_text_property(Window win, Atom prop,
                                  XTextProperty *tprop, ObtPropTextType type)
{
    if (!(XGetTextProperty(obt_display, win, tprop, prop) && tprop->nitems))
        return FALSE;
    if (!type)
        return TRUE; /* no type checking */
    switch (type) {
    case OBT_PROP_TEXT_STRING:
    case OBT_PROP_TEXT_STRING_XPCS:
    case OBT_PROP_TEXT_STRING_NO_CC:
        return tprop->encoding == OBT_PROP_ATOM(STRING);
    case OBT_PROP_TEXT_COMPOUND_TEXT:
        return tprop->encoding == OBT_PROP_ATOM(COMPOUND_TEXT);
    case OBT_PROP_TEXT_UTF8_STRING:
        return tprop->encoding == OBT_PROP_ATOM(UTF8_STRING);
    default:
        g_assert_not_reached();
        return FALSE;
    }
}

/*!
  Returns one or more UTF-8 encoded strings from the text property.

  @param tprop The XTextProperty to convert into UTF-8 string(s).
  @param type  The type which specifies format constraints (or 0 for “anything convertible”).
  @param max   The maximum number of strings to return; -1 to return them all.

  @return
   - If max == 1, returns a single `gchar*` (heap-allocated UTF-8 string).
   - Otherwise, returns a `gchar**` (null-terminated list of up to `max` strings).
   - On error, returns NULL.
*/
static void *
convert_text_property(XTextProperty *tprop, ObtPropTextType type, gint max)
{
    gboolean return_single = (max == 1);

    // Temporary array from either XmbTextPropertyToTextList or manual parsing:
    gchar **strlist = NULL;
    // Did we get strlist via XmbTextPropertyToTextList (so must call XFreeStringList later)?
    gboolean use_xfree = FALSE;

    gint n_strs = 0;

    // Final results:
    gchar *single_str = NULL;  // the single string if return_single==TRUE
    gchar **retlist   = NULL;  // array of strings if return_single==FALSE

    // Distinguish internal encodings
    enum {
        ENCODING_LATIN1,
        ENCODING_UTF8,
        ENCODING_LOCALE
    } encoding;

    // Quick sanity check:
    if (!tprop || tprop->nitems == 0 || tprop->value == NULL)
        return NULL;

    // Identify the encoding and build `strlist`
    if (tprop->encoding == OBT_PROP_ATOM(COMPOUND_TEXT))
    {
        //  COMPOUND_TEXT -> call XmbTextPropertyToTextList
        encoding = ENCODING_LOCALE;
        Status status = XmbTextPropertyToTextList(obt_display, tprop, &strlist, &n_strs);
        if (status != Success || n_strs <= 0 || !strlist)
            goto fail;
        use_xfree = TRUE;  // Must free via XFreeStringList
    }
    else if (tprop->encoding == OBT_PROP_ATOM(UTF8_STRING) ||
             tprop->encoding == OBT_PROP_ATOM(STRING))
    {
        //  UTF8_STRING or STRING -> manually parse the null-terminated substrings
        encoding = (tprop->encoding == OBT_PROP_ATOM(STRING))
                   ? ENCODING_LATIN1
                   : ENCODING_UTF8;

        gchar *p = (gchar *) tprop->value;
        gchar *end = p + tprop->nitems;

        // Count how many strings are in the property
        while (p < end) {
            p += strlen(p) + 1;  // Move past this string’s trailing '\0'
            n_strs++;
        }

        if (n_strs <= 0)
            goto fail;

        // Allocate array of pointers (but not the strings themselves)
        strlist = g_new0(gchar*, n_strs + 1);
        // We will free this later with g_free() (NOT XFreeStringList) => use_xfree=FALSE
        p = (gchar *) tprop->value;
        for (int i = 0; i < n_strs; i++) {
            strlist[i] = p;
            p += strlen(p) + 1;
        }
    }
    else {
        // Unsupported encoding
        goto fail;
    }

    // If caller gave us a maximum number, enforce it:
    if (max >= 0 && n_strs > max)
        n_strs = max;
    if (n_strs == 0)
        goto fail;

    // If returning multiple strings, allocate retlist for them.
    if (!return_single)
        retlist = g_new0(gchar*, n_strs + 1);  // +1 for NULL-termination

    // Convert each string in `strlist` to UTF-8
    for (int i = 0; i < n_strs; i++)
    {
        gchar *original = strlist[i];
        if (!original)
            goto fail;  // Shouldn’t happen, but just in case

        gchar *conv = NULL;

        if (encoding == ENCODING_UTF8)
        {
            // Validate or truncate invalid UTF-8
            const gchar *endp;
            g_utf8_validate(original, -1, &endp);
            conv = g_strndup(original, endp - original);
        }
        else if (encoding == ENCODING_LOCALE)
        {
            // Convert from locale codeset to UTF-8
            gsize nvalid = 0;
            conv = g_locale_to_utf8(original, -1, &nvalid, NULL, NULL);
            if (!conv) {
                // If it failed part-way, try partial conversion
                conv = g_locale_to_utf8(original, nvalid, NULL, NULL, NULL);
                if (!conv)
                    goto fail;
            }
        }
        else  // ENCODING_LATIN1
        {
            // Check for or exclude invalid control chars etc.
            gsize nvalid = 0;
            for (gchar *p = original; *p; p++, nvalid++) {
                const guchar c = (guchar)*p;
                // Allowed controls: TAB=9, NEWLINE=10
                if ((c < 32 && c != 9 && c != 10) || (c >= 127 && c <= 160))
                    break;
                if (type == OBT_PROP_TEXT_STRING_NO_CC && c < 32)
                    break;
                if (type == OBT_PROP_TEXT_STRING_XPCS) {
                    gboolean valid = ((c >= 32 && c < 128) || c == 9 || c == 10);
                    if (!valid)
                        break;
                }
            }
            conv = g_convert(original, nvalid, "UTF-8", "ISO-8859-1", NULL, NULL, NULL);
            if (!conv) {
                // Try one more time with the partial length
                conv = g_convert(original, nvalid, "UTF-8", "ISO-8859-1", NULL, NULL, NULL);
                if (!conv)
                    goto fail;
            }
        }

        if (return_single) {
            // We only care about the first string for max == 1
            single_str = conv;
            break;
        } else {
            // Place into the array
            retlist[i] = conv;
        }
    }

    // Clean up the strlist array (depends on how we got it)
    if (strlist) {
        if (use_xfree)
            XFreeStringList(strlist);  // Freed all X-allocated memory
        else
            g_free(strlist);           // Freed our g_new0() array
    }
    strlist = NULL;

    // Return results
    if (return_single) {
        return single_str;   // A single heap-allocated string
    } else {
        return retlist;      // A null-terminated array of strings
    }

    // On failure, free everything properly:
fail:
    if (strlist) {
        // Free the array with the matching method
        if (use_xfree)
            XFreeStringList(strlist);
        else
            g_free(strlist);
    }
    if (retlist) {
        for (int j = 0; j < n_strs; j++)
            g_free(retlist[j]);  // free each converted string
        g_free(retlist);
    }
    g_free(single_str);

    return NULL;
}


gboolean obt_prop_get32(Window win, Atom prop, Atom type, guint32 *ret)
{
    return get_prealloc(win, prop, type, 32, (guchar*)ret, 1);
}

gboolean obt_prop_get_array32(Window win, Atom prop, Atom type, guint32 **ret,
                              guint *nret)
{
    return get_all(win, prop, type, 32, (guchar**)ret, nret);
}

gboolean obt_prop_get_text(Window win, Atom prop, ObtPropTextType type,
                           gchar **ret_string)
{
    XTextProperty tprop;
    gchar *str;
    gboolean ret = FALSE;

    if (get_text_property(win, prop, &tprop, type)) {
        str = (gchar*)convert_text_property(&tprop, type, 1);

        if (str) {
            *ret_string = str;
            ret = TRUE;
        }
    }
    XFree(tprop.value);
    return ret;
}

gboolean obt_prop_get_array_text(Window win, Atom prop,
                                 ObtPropTextType type,
                                 gchar ***ret_strings)
{
    XTextProperty tprop;
    gchar **strs;
    gboolean ret = FALSE;

    if (get_text_property(win, prop, &tprop, type)) {
        strs = (gchar**)convert_text_property(&tprop, type, -1);

        if (strs) {
            *ret_strings = strs;
            ret = TRUE;
        }
    }
    XFree(tprop.value);
    return ret;
}

void obt_prop_set32(Window win, Atom prop, Atom type, gulong val)
{
    XChangeProperty(obt_display, win, prop, type, 32, PropModeReplace,
                    (guchar*)&val, 1);
}

void obt_prop_set_array32(Window win, Atom prop, Atom type, gulong *val,
                      guint num)
{
    XChangeProperty(obt_display, win, prop, type, 32, PropModeReplace,
                    (guchar*)val, num);
}

void obt_prop_set_text(Window win, Atom prop, const gchar *val)
{
    XChangeProperty(obt_display, win, prop, OBT_PROP_ATOM(UTF8_STRING), 8,
                    PropModeReplace, (const guchar*)val, strlen(val));
}

void obt_prop_set_array_text(Window win, Atom prop, const gchar *const *strs)
{
    GString *str;
    gchar const *const *s;

    str = g_string_sized_new(0);
    for (s = strs; *s; ++s) {
        str = g_string_append(str, *s);
        str = g_string_append_c(str, '\0');
    }
    XChangeProperty(obt_display, win, prop, OBT_PROP_ATOM(UTF8_STRING), 8,
                    PropModeReplace, (guchar*)str->str, str->len);
    g_string_free(str, TRUE);
}

void obt_prop_erase(Window win, Atom prop)
{
    XDeleteProperty(obt_display, win, prop);
}

void obt_prop_message(gint screen, Window about, Atom messagetype,
                      glong data0, glong data1, glong data2, glong data3,
                      glong data4, glong mask)
{
    obt_prop_message_to(obt_root(screen), about, messagetype,
                        data0, data1, data2, data3, data4, mask);
}

void obt_prop_message_to(Window to, Window about,
                         Atom messagetype,
                         glong data0, glong data1, glong data2, glong data3,
                         glong data4, glong mask)
{
    XEvent ce;
    ce.xclient.type = ClientMessage;
    ce.xclient.message_type = messagetype;
    ce.xclient.display = obt_display;
    ce.xclient.window = about;
    ce.xclient.format = 32;
    ce.xclient.data.l[0] = data0;
    ce.xclient.data.l[1] = data1;
    ce.xclient.data.l[2] = data2;
    ce.xclient.data.l[3] = data3;
    ce.xclient.data.l[4] = data4;
    XSendEvent(obt_display, to, FALSE, mask, &ce);
}
