/* skiptaskbar.c for the Openbox window manager */

#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

int main() {
  Display* display;
  Window win;
  XEvent report;
  Atom state, skip;
  XClassHint classhint;
  int x = 10, y = 10, h = 400, w = 400;

  display = XOpenDisplay(NULL);

  if (display == NULL) {
    fprintf(stderr, "couldn't connect to X server :0\n");
    return 1;  // Return 1 if there is an error with opening the display
  }

  state = XInternAtom(display, "_NET_WM_STATE", True);
  skip = XInternAtom(display, "_NET_WM_STATE_SKIP_TASKBAR", True);

  if (state == None || skip == None) {
    fprintf(stderr, "Failed to intern atoms\n");
    return 1;  // Return 1 if atom intern failed
  }

  win = XCreateWindow(display, RootWindow(display, 0), x, y, w, h, 10, CopyFromParent, CopyFromParent, CopyFromParent,
                      0, NULL);

  if (win == None) {
    fprintf(stderr, "Failed to create window\n");
    return 1;  // Return 1 if window creation fails
  }

  XSetWindowBackground(display, win, WhitePixel(display, 0));

  // Set the _NET_WM_STATE_SKIP_TASKBAR property
  XChangeProperty(display, win, state, XA_ATOM, 32, PropModeReplace, (unsigned char*)&skip, 1);

  // Set window class hints
  classhint.res_name = "test";
  classhint.res_class = "Test";
  XSetClassHint(display, win, &classhint);

  // Map the window and flush the output
  XMapWindow(display, win);
  XFlush(display);

  // Event loop (keeps the window alive)
  while (1) {
    XNextEvent(display, &report);
  }

  return 0;
}
