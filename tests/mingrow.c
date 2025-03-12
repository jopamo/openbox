/* mingrow.c for the Openbox window manager */

#include <stdio.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

int main() {
  XSetWindowAttributes xswa;
  unsigned long xswamask;
  Display* display;
  Window win;
  XEvent report;
  int x = 10, y = 10, h = 100, w = 100;
  XSizeHints size;

  // Open the X display
  display = XOpenDisplay(NULL);
  if (display == NULL) {
    fprintf(stderr, "couldn't connect to X server :0\n");
    return 0;
  }

  // Set window gravity
  xswa.win_gravity = StaticGravity;
  xswamask = CWWinGravity;

  // Create the window
  win = XCreateWindow(display, RootWindow(display, 0), x, y, w, h, 0, CopyFromParent, CopyFromParent, CopyFromParent,
                      xswamask, &xswa);
  XSetWindowBackground(display, win, WhitePixel(display, 0));

  // Set window size hints
  size.flags = PMinSize | PMaxSize;
  size.max_width = 0;
  size.min_width = w;
  size.max_height = 0;
  size.min_height = h;
  XSetWMNormalHints(display, win, &size);

  // Map the window and flush the display
  XSelectInput(display, win, ExposureMask | StructureNotifyMask);
  XMapWindow(display, win);
  XFlush(display);

  // Simulate waiting for some time (for CI purposes, we'll wait 4 seconds before resizing)
  printf("sleeping 4..");
  fflush(stdout);
  sleep(1);
  printf("3..");
  fflush(stdout);
  sleep(1);
  printf("2..");
  fflush(stdout);
  sleep(1);
  printf("1..");
  fflush(stdout);
  sleep(1);
  printf("\n");

  // Resize the window by changing the size hints
  size.flags = PMinSize | PMaxSize;
  size.max_width = 0;
  size.min_width = w * 2;  // Change the min width
  size.max_height = 0;
  size.min_height = h * 2;  // Change the min height
  XSetWMNormalHints(display, win, &size);

  // Event loop to handle window events
  while (1) {
    XNextEvent(display, &report);

    switch (report.type) {
      case Expose:
        printf("exposed\n");
        break;
      case ConfigureNotify:
        // Handle window size changes
        x = report.xconfigure.x;
        y = report.xconfigure.y;
        w = report.xconfigure.width;
        h = report.xconfigure.height;
        printf("confignotify %i,%i-%ix%i\n", x, y, w, h);
        break;
    }

    // Exit the loop after handling the first event to avoid infinite loops in CI
    if (report.type == Expose && report.xexpose.count == 0) {
      printf("Test completed. Closing the program.\n");
      break;
    }
  }

  // Cleanup and close the display connection
  XDestroyWindow(display, win);
  XCloseDisplay(display);

  return 0;
}
