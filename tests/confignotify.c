// confignotify.c for the Openbox window manager

#include <stdio.h>
#include <unistd.h>
#include <X11/Xlib.h>

int main() {
  Display* display;
  Window win;
  XEvent report;
  int x = 10, y = 10, h = 100, w = 100;

  display = XOpenDisplay(NULL);
  if (display == NULL) {
    fprintf(stderr, "couldn't connect to X server :0\n");
    return 1;  // Return failure if the display cannot be opened
  }

  win = XCreateWindow(display, RootWindow(display, 0), x, y, w, h, 0, CopyFromParent, CopyFromParent, CopyFromParent, 0,
                      NULL);

  XSetWindowBackground(display, win, WhitePixel(display, 0));

  XSelectInput(display, win, (ExposureMask | StructureNotifyMask | GravityNotify));

  XMapWindow(display, win);
  XFlush(display);

  sleep(1);
  XResizeWindow(display, win, w + 5, h + 5);
  XMoveWindow(display, win, x, y);

  while (1) {
    XNextEvent(display, &report);

    switch (report.type) {
      case MapNotify:
        printf("map notify\n");
        break;
      case Expose:
        printf("exposed\n");
        break;
      case GravityNotify:
        printf("gravity notify event 0x%lx window 0x%lx x %d y %d\n", (unsigned long)report.xgravity.event,
               (unsigned long)report.xgravity.window, report.xgravity.x, report.xgravity.y);
        break;

      case ConfigureNotify: {
        int se = report.xconfigure.send_event;
        /* 'Window' is long unsigned int, so cast for printing */
        unsigned long ev = (unsigned long)report.xconfigure.event;
        unsigned long win = (unsigned long)report.xconfigure.window;
        int x = report.xconfigure.x;
        int y = report.xconfigure.y;
        int w = report.xconfigure.width;
        int h = report.xconfigure.height;
        int bw = report.xconfigure.border_width;
        unsigned long above = (unsigned long)report.xconfigure.above;
        int or = report.xconfigure.override_redirect;

        printf(
            "confignotify send %d ev 0x%lx win 0x%lx %i,%i-%ix%i bw %i\n"
            "             above 0x%lx ovrd %d\n",
            se, ev, win, x, y, w, h, bw, above, or);
        break;
      }
    }
  }

  XDestroyWindow(display, win);  // Destroy the window
  XCloseDisplay(display);        // Close the display connection

  return 0;
}
