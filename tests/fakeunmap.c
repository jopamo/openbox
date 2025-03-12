/*
 * fakeunmap.c for Openbox window manager
 *
 * Demonstrates creating a window, mapping it, verifying its state,
 * sending a synthetic UnmapNotify, and then unmapping the window
 * with proper checks and event synchronization.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

static void fail_and_exit(const char* msg) {
  fprintf(stderr, "%s\n", msg);
  exit(EXIT_FAILURE);
}

static void wait_for_event(Display* display, Window win, int event_type) {
  /* Wait for a specific event type on a given window */
  XEvent e;
  for (;;) {
    XNextEvent(display, &e);
    if (e.type == event_type && e.xany.window == win) {
      break;
    }
  }
}

/* Helper function to check whether a window is mapped or unmapped */
static void test_window_state(Display* display, Window win, int expected) {
  XWindowAttributes attr;

  if (!XGetWindowAttributes(display, win, &attr)) {
    fail_and_exit("XGetWindowAttributes failed");
  }

  if (attr.map_state == expected) {
    printf("Test passed: Window is in the expected %s state\n", (expected == IsViewable) ? "mapped" : "unmapped");
  }
  else {
    printf("Test failed: Expected window to be %s, but it is %s\n", (expected == IsViewable) ? "mapped" : "unmapped",
           (attr.map_state == IsViewable) ? "mapped" : "unmapped");
  }
}

int main(void) {
  Display* display;
  Window root, win;
  XSetWindowAttributes xswa;
  XEvent msg;
  int screen_num;
  int x = 50, y = 50, width = 400, height = 100;

  /* Open the display */
  display = XOpenDisplay(NULL);
  if (display == NULL) {
    fprintf(stderr, "Could not connect to X server.\n");
    return EXIT_FAILURE;
  }

  screen_num = DefaultScreen(display);
  root = RootWindow(display, screen_num);

  /* Create a simple window */
  xswa.background_pixel = WhitePixel(display, screen_num);
  xswa.border_pixel = BlackPixel(display, screen_num);
  xswa.event_mask = StructureNotifyMask; /* We want MapNotify/UnmapNotify */

  win = XCreateWindow(display, root, x, y, width, height, 1, /* border width */
                      CopyFromParent,                        /* depth */
                      InputOutput,                           /* class */
                      CopyFromParent,                        /* visual */
                      CWBackPixel | CWBorderPixel | CWEventMask, &xswa);
  if (!win) {
    fail_and_exit("Failed to create X Window");
  }

  /* Map the window (make it visible) */
  XMapWindow(display, win);
  XFlush(display);

  /* Wait for MapNotify to ensure window is actually mapped */
  wait_for_event(display, win, MapNotify);

  /* Verify that the window is mapped */
  test_window_state(display, win, IsViewable);

  /* -----------------------------------------------------
     Simulate UnmapNotify event (synthetic). In a real X
     environment, the server usually sends this event.
     ----------------------------------------------------- */
  memset(&msg, 0, sizeof(msg));
  msg.type = UnmapNotify;
  msg.xunmap.event = root;
  msg.xunmap.window = win;
  msg.xunmap.from_configure = False;

  /* Send the synthetic UnmapNotify event to the root window */
  XSendEvent(display, root, False, SubstructureRedirectMask | SubstructureNotifyMask, &msg);

  /* -----------------------------------------------------
     Now actually unmap the window. This will trigger a
     real UnmapNotify event from the server.
     ----------------------------------------------------- */
  XUnmapWindow(display, win);
  XFlush(display);

  /* Wait for the actual UnmapNotify event */
  wait_for_event(display, win, UnmapNotify);

  /* Verify that the window is unmapped */
  test_window_state(display, win, IsUnmapped);

  /* Clean up and exit */
  XDestroyWindow(display, win);
  XCloseDisplay(display);

  return EXIT_SUCCESS;
}
