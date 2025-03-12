/* modal.c for the Openbox window manager */

#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

int main() {
  Display* display;
  Window parent, child;
  XEvent report;
  Atom state, modal;
  int x = 10, y = 10, h = 400, w = 400;

  // Open the X display
  display = XOpenDisplay(NULL);
  if (display == NULL) {
    fprintf(stderr, "couldn't connect to X server :0\n");
    return 0;
  }

  // Intern atoms
  state = XInternAtom(display, "_NET_WM_STATE", True);
  modal = XInternAtom(display, "_NET_WM_STATE_MODAL", True);

  // Check if atoms are valid
  if (state == None || modal == None) {
    fprintf(stderr, "Failed to intern atoms: _NET_WM_STATE or _NET_WM_STATE_MODAL\n");
    return 1;
  }

  printf("Atoms successfully interned: _NET_WM_STATE = %lu, _NET_WM_STATE_MODAL = %lu\n", state, modal);

  // Create windows
  parent = XCreateWindow(display, RootWindow(display, 0), x, y, w, h, 10, CopyFromParent, CopyFromParent,
                         CopyFromParent, 0, 0);
  child = XCreateWindow(display, RootWindow(display, 0), x, y, w / 2, h / 2, 10, CopyFromParent, CopyFromParent,
                        CopyFromParent, 0, 0);

  XSetWindowBackground(display, parent, WhitePixel(display, 0));
  XSetWindowBackground(display, child, BlackPixel(display, 0));

  // Set transient window hint
  XSetTransientForHint(display, child, parent);

  // Set modal state for the child window
  XChangeProperty(display, child, state, XA_ATOM, 32, PropModeReplace, (unsigned char*)&modal, 1);

  // Map windows
  XMapWindow(display, parent);
  XMapWindow(display, child);
  XFlush(display);

  // Listen for events (simulate handling)
  XSelectInput(display, child, ExposureMask | StructureNotifyMask);

  while (1) {
    XNextEvent(display, &report);

    // Handle Expose and ConfigureNotify events
    switch (report.type) {
      case Expose:
        printf("Expose event received\n");
        break;
      case ConfigureNotify:
        printf("ConfigureNotify: window %lu moved to (%d, %d) and resized to %dx%d\n", report.xconfigure.window,
               report.xconfigure.x, report.xconfigure.y, report.xconfigure.width, report.xconfigure.height);
        break;
    }

    // Exit after handling the first event to avoid an infinite loop in CI
    if (report.type == Expose && report.xexpose.count == 0) {
      printf("Test completed. Closing the program.\n");
      break;
    }
  }

  // Clean up and close the display connection
  XDestroyWindow(display, parent);
  XDestroyWindow(display, child);
  XCloseDisplay(display);

  return 0;
}
