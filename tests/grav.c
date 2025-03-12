/* grav.c for the Openbox window manager */

#include <stdio.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

int main() {
  Display* display;
  Window win;
  XEvent report;
  int x = 10, y = 10, h = 100, w = 400, b = 10;
  XSizeHints* hints;

  // Open the X display
  display = XOpenDisplay(NULL);
  if (display == NULL) {
    fprintf(stderr, "couldn't connect to X server :0\n");
    return 1;  // Return 1 to indicate failure if unable to connect to the X server
  }

  // Create a window
  win = XCreateWindow(display, RootWindow(display, 0), x, y, w, h, b, CopyFromParent, CopyFromParent, CopyFromParent, 0,
                      NULL);

  // Set the window gravity to SouthEastGravity
  hints = XAllocSizeHints();
  hints->flags = PWinGravity;
  hints->win_gravity = SouthEastGravity;
  XSetWMNormalHints(display, win, hints);
  XFree(hints);

  // Set the window background color
  XSetWindowBackground(display, win, WhitePixel(display, 0));

  // Map the window and flush the display
  XMapWindow(display, win);
  XFlush(display);

  // Resize and move the window
  w = 600;
  h = 160;
  XMoveResizeWindow(display, win, 1172 - w - b * 2, 668 - h - b * 2, w, h);
  XFlush(display);
  sleep(1);
  XResizeWindow(display, win, 900, 275);

  // Select input events to listen for
  XSelectInput(display, win, ExposureMask | StructureNotifyMask);

  // Event loop to process events
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

    // Exit after processing the first event to avoid infinite loops in CI
    if (report.type == Expose && report.xexpose.count == 0) {
      printf("Test completed. Closing the program.\n");
      break;
    }
  }

  // Clean up and close the display connection
  XDestroyWindow(display, win);
  XCloseDisplay(display);

  return 0;  // Return 0 to indicate success
}
