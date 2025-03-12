/* modal3.c for the Openbox window manager */

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

  display = XOpenDisplay(NULL);
  if (display == NULL) {
    fprintf(stderr, "couldn't connect to X server :0\n");
    return 1;
  }

  // Get the necessary atoms
  state = XInternAtom(display, "_NET_WM_STATE", True);
  modal = XInternAtom(display, "_NET_WM_STATE_MODAL", True);

  // Create parent and child windows
  parent = XCreateWindow(display, RootWindow(display, 0), x, y, w, h, 10, CopyFromParent, CopyFromParent,
                         CopyFromParent, 0, 0);
  child = XCreateWindow(display, RootWindow(display, 0), x, y, w / 2, h / 2, 10, CopyFromParent, CopyFromParent,
                        CopyFromParent, 0, 0);

  // Set backgrounds for parent and child windows
  XSetWindowBackground(display, parent, WhitePixel(display, 0));
  XSetWindowBackground(display, child, BlackPixel(display, 0));

  // Set the child window as transient for the parent window
  XSetTransientForHint(display, child, parent);

  // Map the windows to make them visible
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

  // Deactivate modal state
  ce.xclient.data.l[0] = 0;  // 0 to deactivate modal
  XSendEvent(display, RootWindow(display, DefaultScreen(display)), False,
             SubstructureNotifyMask | SubstructureRedirectMask, &ce);

  // Event loop (wait for events to be processed)
  while (1) {
    XNextEvent(display, &report);
    // Process events here (e.g., handle modal or expose events)
    if (report.type == Expose) {
      printf("Expose event\n");
    }
  }

  // Cleanup and close the display connection
  XDestroyWindow(display, parent);
  XDestroyWindow(display, child);
  XCloseDisplay(display);

  return 0;
}
