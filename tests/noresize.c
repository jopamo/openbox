/* noresize.c for the Openbox window manager */

#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

int main() {
  XSetWindowAttributes xswa;
  unsigned long xswamask;
  Display* display;
  Window win;
  XEvent report;
  int x = 10, y = 10, h = 100, w = 400;
  XSizeHints size;

  display = XOpenDisplay(NULL);
  if (display == NULL) {
    fprintf(stderr, "couldn't connect to X server :0\n");
    return 1;
  }

  // Set window gravity
  xswa.win_gravity = StaticGravity;
  xswamask = CWWinGravity;

  // Create the window
  win = XCreateWindow(display, RootWindow(display, 0), x, y, w, h, 0, CopyFromParent, CopyFromParent, CopyFromParent,
                      xswamask, &xswa);
  XSetWindowBackground(display, win, WhitePixel(display, 0));

  // Set size hints to prevent resizing
  size.flags = PMinSize | PMaxSize;
  size.max_width = 0;
  size.min_width = w;
  size.max_height = 0;
  size.min_height = h;
  XSetWMNormalHints(display, win, &size);

  XSelectInput(display, win, ExposureMask | StructureNotifyMask);

  XMapWindow(display, win);
  XFlush(display);

  // Move the window
  XMoveWindow(display, win, 10, 10);
  XMoveWindow(display, win, 10, 10);

  // Event loop to handle Expose and ConfigureNotify events
  while (1) {
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

    // Exit after handling a single event to avoid infinite loop
    if (report.type == Expose && report.xexpose.count == 0) {
      printf("Test completed. Closing the program.\n");
      break;
    }
  }

  // Cleanup
  XDestroyWindow(display, win);
  XCloseDisplay(display);

  return 0;
}
