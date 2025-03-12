/* strut.c for Openbox window manager */

#include <stdio.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

void test_strut_property(Display* display, Window win, Atom _net_strut) {
  int s[4];

  // Set top strut
  s[0] = 0;
  s[1] = 0;
  s[2] = 20;
  s[3] = 0;
  XChangeProperty(display, win, _net_strut, XA_CARDINAL, 32, PropModeReplace, (unsigned char*)s, 4);
  XFlush(display);
  sleep(2);

  // Remove strut
  XDeleteProperty(display, win, _net_strut);
  XFlush(display);
  sleep(2);
}

void handle_events(Display* display, Window win) {
  XEvent report;
  while (1) {
    XNextEvent(display, &report);

    switch (report.type) {
      case Expose:
        printf("Window exposed\n");
        break;
      case ConfigureNotify:
        printf("Window configuration changed: position (%i, %i), size (%ix%i)\n", report.xconfigure.x,
               report.xconfigure.y, report.xconfigure.width, report.xconfigure.height);
        break;
    }

    if (report.type == Expose && report.xexpose.count == 0) {
      printf("All events processed. Closing the program.\n");
      return;
    }
  }
}

int main() {
  Display* display;
  Window win;
  Atom _net_strut;
  int x = 10, y = 10, h = 100, w = 400;

  // Connect to X server
  display = XOpenDisplay(NULL);
  if (display == NULL) {
    fprintf(stderr, "couldn't connect to X server :0\n");
    return 1;
  }

  _net_strut = XInternAtom(display, "_NET_WM_STRUT", False);

  // Create window
  win = XCreateWindow(display, RootWindow(display, 0), x, y, w, h, 10, CopyFromParent, CopyFromParent, CopyFromParent,
                      0, NULL);
  XSetWindowBackground(display, win, WhitePixel(display, 0));
  XMapWindow(display, win);
  XFlush(display);

  // Test strut property
  test_strut_property(display, win, _net_strut);

  // Listen for events
  XSelectInput(display, win, ExposureMask | StructureNotifyMask);
  handle_events(display, win);

  // Clean up
  XDestroyWindow(display, win);
  XCloseDisplay(display);

  return 0;
}
