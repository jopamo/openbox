// skiptaskbar2.c for the Openbox window manager

#include <stdio.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

int main() {
  Display* display;
  Window win;
  XEvent report, ce;
  Atom state, skip;
  int x = 10, y = 10, h = 400, w = 400;

  display = XOpenDisplay(NULL);

  if (display == NULL) {
    fprintf(stderr, "couldn't connect to X server :0\n");
    return 0;
  }

  state = XInternAtom(display, "_NET_WM_STATE", True);
  skip = XInternAtom(display, "_NET_WM_STATE_SKIP_TASKBAR", True);

  win = XCreateWindow(display, RootWindow(display, 0), x, y, w, h, 10, CopyFromParent, CopyFromParent, CopyFromParent,
                      0, 0);

  XSetWindowBackground(display, win, WhitePixel(display, 0));

  XMapWindow(display, win);
  XFlush(display);

  sleep(1);

  ce.xclient.type = ClientMessage;
  ce.xclient.message_type = state;
  ce.xclient.display = display;
  ce.xclient.window = win;
  ce.xclient.format = 32;
  ce.xclient.data.l[0] = 1;
  ce.xclient.data.l[1] = skip;
  ce.xclient.data.l[2] = 0;
  XSendEvent(display, RootWindow(display, DefaultScreen(display)), False,
             SubstructureNotifyMask | SubstructureRedirectMask, &ce);

  while (1) {
    XNextEvent(display, &report);
  }

  return 1;
}
