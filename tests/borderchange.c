/* borderchange.c for Openbox window manager */

#include <stdio.h>
#include <unistd.h>
#include <X11/Xlib.h>

int main() {
  Display* display;
  Window win;
  XEvent report;
  int x = 10, y = 10, h = 200, w = 200;

  display = XOpenDisplay(NULL);
  if (display == NULL) {
    fprintf(stderr, "couldn't connect to X server :0\n");
    return 0;
  }

  win = XCreateWindow(display, RootWindow(display, 0), x, y, w, h, 10, CopyFromParent, CopyFromParent, CopyFromParent,
                      0, NULL);
  XSetWindowBackground(display, win, WhitePixel(display, 0));
  XSelectInput(display, win, ExposureMask | StructureNotifyMask);

  XMapWindow(display, win);
  XFlush(display);

  // Initial window events
  while (XPending(display)) {
    XNextEvent(display, &report);
    switch (report.type) {
      case Expose:
        printf("exposed\n");
        break;
      case ConfigureNotify:
        x = report.xconfigure.x;
        y = report.xconfigure.y;
        w = report.xconfigure.width;
        h = report.xconfigure.height;
        printf("confignotify %i,%i-%ix%i\n", x, y, w, h);
        break;
    }
  }

  sleep(2);

  // Set border width to 0
  printf("setting border to 0\n");
  XSetWindowBorderWidth(display, win, 0);
  XFlush(display);

  // Events after border width change
  while (XPending(display)) {
    XNextEvent(display, &report);
    switch (report.type) {
      case Expose:
        printf("exposed\n");
        break;
      case ConfigureNotify:
        x = report.xconfigure.x;
        y = report.xconfigure.y;
        w = report.xconfigure.width;
        h = report.xconfigure.height;
        printf("confignotify %i,%i-%ix%i\n", x, y, w, h);
        break;
    }
  }

  sleep(2);

  // Set border width to 50
  printf("setting border to 50\n");
  XSetWindowBorderWidth(display, win, 50);
  XFlush(display);

  // Events after border width change
  while (XPending(display)) {
    XNextEvent(display, &report);
    switch (report.type) {
      case Expose:
        printf("exposed\n");
        break;
      case ConfigureNotify:
        x = report.xconfigure.x;
        y = report.xconfigure.y;
        w = report.xconfigure.width;
        h = report.xconfigure.height;
        printf("confignotify %i,%i-%ix%i\n", x, y, w, h);
        break;
    }
  }

  return 0;
}
