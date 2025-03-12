/* modal2.c for the Openbox window manager */

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
  XEvent ce;

  // Open the X display
  display = XOpenDisplay(NULL);
  if (display == NULL) {
    fprintf(stderr, "couldn't connect to X server :0\n");
    return 0;
  }

  // Intern atoms for window state
  state = XInternAtom(display, "_NET_WM_STATE", True);
  modal = XInternAtom(display, "_NET_WM_STATE_MODAL", True);

  // Check if atoms are valid
  if (state == None || modal == None) {
    fprintf(stderr, "Failed to intern atoms: _NET_WM_STATE or _NET_WM_STATE_MODAL\n");
    return 1;
  }

  printf("Atoms successfully interned: _NET_WM_STATE = %lu, _NET_WM_STATE_MODAL = %lu\n", state, modal);

  // Create parent and child windows
  parent = XCreateWindow(display, RootWindow(display, 0), x, y, w, h, 10, CopyFromParent, CopyFromParent,
                         CopyFromParent, 0, 0);
  child = XCreateWindow(display, RootWindow(display, 0), x, y, w / 2, h / 2, 10, CopyFromParent, CopyFromParent,
                        CopyFromParent, 0, 0);

  // Set background colors
  XSetWindowBackground(display, parent, WhitePixel(display, 0));
  XSetWindowBackground(display, child, BlackPixel(display, 0));

  // Set the child window as transient for the parent window
  XSetTransientForHint(display, child, parent);

  // Map the windows (make them visible)
  XMapWindow(display, parent);
  XMapWindow(display, child);
  XFlush(display);

  // Send the modal state to the child window
  ce.xclient.type = ClientMessage;
  ce.xclient.message_type = state;
  ce.xclient.display = display;
  ce.xclient.window = child;
  ce.xclient.format = 32;
  ce.xclient.data.l[0] = 1;  // 1 to activate modal
  ce.xclient.data.l[1] = modal;
  ce.xclient.data.l[2] = 0;
  XSendEvent(display, RootWindow(display, DefaultScreen(display)), False,
             SubstructureNotifyMask | SubstructureRedirectMask, &ce);

  // Wait for events (simulate handling)
  XSelectInput(display, child, ExposureMask | StructureNotifyMask);

  // Event loop
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

    // Exit the loop after handling the first event to avoid infinite loops in CI
    if (report.type == Expose && report.xexpose.count == 0) {
      printf("Test completed. Closing the program.\n");
      break;
    }
  }

  // Cleanup and close the display connection
  XDestroyWindow(display, parent);
  XDestroyWindow(display, child);
  XCloseDisplay(display);

  return 0;
}
