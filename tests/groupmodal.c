// groupmodal.c for the Openbox window manager

#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

int main () {
  Display   *display;
  Window     one, two, group;
  XEvent     report;
  XWMHints  *wmhints;
  Atom       state, modal;

  display = XOpenDisplay(NULL);

  if (display == NULL) {
    fprintf(stderr, "couldn't connect to X server :0\n");
    return 0;
  }

  // Intern Atoms
  state = XInternAtom(display, "_NET_WM_STATE", True);
  modal = XInternAtom(display, "_NET_WM_STATE_MODAL", True);

  // Check if atoms are valid
  if (state == None || modal == None) {
    fprintf(stderr, "Failed to intern atoms\n");
    return 1;
  }
  printf("Atoms successfully interned: _NET_WM_STATE = %lu, _NET_WM_STATE_MODAL = %lu\n", state, modal);

  // Create windows
  group = XCreateWindow(display, RootWindow(display, 0),
                        0,0,1,1, 10, CopyFromParent, CopyFromParent,
                        CopyFromParent, 0, 0);

  one = XCreateWindow(display, RootWindow(display, 0),
                      0,0,300,300, 10, CopyFromParent, CopyFromParent,
                      CopyFromParent, 0, 0);
  two = XCreateWindow(display, RootWindow(display, 0),
                      0,0,100,100, 10, CopyFromParent, CopyFromParent,
                      CopyFromParent, 0, 0);

  // Set window background colors
  XSetWindowBackground(display, one, WhitePixel(display, 0));
  XSetWindowBackground(display, two, BlackPixel(display, 0));

  // Set transient and modal state
  XSetTransientForHint(display, two, RootWindow(display, 0));
  XChangeProperty(display, two, state, XA_ATOM, 32,
                  PropModeReplace, (unsigned char*)&modal, 1);

  // Set window group for both windows
  wmhints = XAllocWMHints();
  wmhints->flags = WindowGroupHint;
  wmhints->window_group = group;

  XSetWMHints(display, one, wmhints);
  XSetWMHints(display, two, wmhints);
  XFree(wmhints);

  // Map the windows
  XMapWindow(display, one);
  XFlush(display);
  sleep(1);
  XMapWindow(display, two);
  XFlush(display);

  // Event loop
  while (1) {
    XNextEvent(display, &report);
  }

  return 1;
}
