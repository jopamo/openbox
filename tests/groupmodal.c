// groupmodal.c for the Openbox window manager

#include <stdio.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

int main() {
  Display* display;
  Window one, two, group;
  XEvent report;
  XWMHints* wmhints;
  Atom state, modal;

  // Open a connection to the X server
  display = XOpenDisplay(NULL);
  if (display == NULL) {
    fprintf(stderr, "Couldn't connect to X server :0\n");
    return 1;  // Return failure if X server connection fails
  }

  // Intern Atoms
  state = XInternAtom(display, "_NET_WM_STATE", True);
  modal = XInternAtom(display, "_NET_WM_STATE_MODAL", True);

  // Check if atoms are valid
  if (state == None || modal == None) {
    fprintf(stderr, "Failed to intern atoms\n");
    XCloseDisplay(display);  // Clean up and close display
    return 1;                // Return failure if atoms cannot be interned
  }
  printf("Atoms successfully interned: _NET_WM_STATE = %lu, _NET_WM_STATE_MODAL = %lu\n", state, modal);

  // Create windows
  group = XCreateWindow(display, RootWindow(display, 0), 0, 0, 1, 1, 10, CopyFromParent, CopyFromParent, CopyFromParent,
                        0, 0);
  if (group == 0) {
    fprintf(stderr, "Failed to create group window\n");
    XCloseDisplay(display);  // Clean up and close display
    return 1;                // Return failure if window creation fails
  }

  one = XCreateWindow(display, RootWindow(display, 0), 0, 0, 300, 300, 10, CopyFromParent, CopyFromParent,
                      CopyFromParent, 0, 0);
  if (one == 0) {
    fprintf(stderr, "Failed to create window one\n");
    XCloseDisplay(display);  // Clean up and close display
    return 1;                // Return failure if window creation fails
  }

  two = XCreateWindow(display, RootWindow(display, 0), 0, 0, 100, 100, 10, CopyFromParent, CopyFromParent,
                      CopyFromParent, 0, 0);
  if (two == 0) {
    fprintf(stderr, "Failed to create window two\n");
    XCloseDisplay(display);  // Clean up and close display
    return 1;                // Return failure if window creation fails
  }

  // Set window background colors
  XSetWindowBackground(display, one, WhitePixel(display, 0));
  XSetWindowBackground(display, two, BlackPixel(display, 0));

  // Set transient and modal state
  XSetTransientForHint(display, two, RootWindow(display, 0));
  XChangeProperty(display, two, state, XA_ATOM, 32, PropModeReplace, (unsigned char*)&modal, 1);

  // Set window group for both windows
  wmhints = XAllocWMHints();
  if (wmhints == NULL) {
    fprintf(stderr, "Failed to allocate memory for window hints\n");
    XCloseDisplay(display);  // Clean up and close display
    return 1;                // Return failure if allocation fails
  }

  wmhints->flags = WindowGroupHint;
  wmhints->window_group = group;

  XSetWMHints(display, one, wmhints);
  XSetWMHints(display, two, wmhints);
  XFree(wmhints);  // Free memory after use

  // Map the windows
  XMapWindow(display, one);
  XFlush(display);
  sleep(1);  // Sleep to ensure the first window is mapped before the second
  XMapWindow(display, two);
  XFlush(display);

  // Event loop
  while (1) {
    XNextEvent(display, &report);
    // Process events (currently does nothing, but could be extended)
  }

  // Clean up and close display connection when done
  XCloseDisplay(display);

  return 0;  // Return success
}
