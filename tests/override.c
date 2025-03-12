/* override.c for the Openbox window manager */

#include <stdio.h>
#include <X11/Xlib.h>
#include <unistd.h>

int main() {
  XSetWindowAttributes xswa;
  unsigned long xswamask;
  Display* display;
  Window win;
  XEvent report;
  int x = 10, y = 10, h = 100, w = 400;

  display = XOpenDisplay(NULL);
  if (display == NULL) {
    fprintf(stderr, "couldn't connect to X server :0\n");
    return 1;
  }

  // Set override_redirect to True
  xswa.override_redirect = True;
  xswamask = CWOverrideRedirect;

  // Create a window with override_redirect flag set
  win = XCreateWindow(display, RootWindow(display, 0), x, y, w, h, 10, CopyFromParent, CopyFromParent, CopyFromParent,
                      xswamask, &xswa);
  XSetWindowBackground(display, win, WhitePixel(display, 0));

  // Map the window and flush the display
  XMapWindow(display, win);
  XFlush(display);  // Correct usage of XFlush

  sleep(1);  // Simulate a delay for the window to appear

  // Unmap and remap the window to simulate window state change
  XUnmapWindow(display, win);
  XFlush(display);  // Correct usage of XFlush
  sleep(1);
  XMapWindow(display, win);
  XFlush(display);  // Correct usage of XFlush

  // Listen for events
  XSelectInput(display, win, ExposureMask | StructureNotifyMask);

  // Event loop
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

    // Exit the loop after handling an event
    if (report.type == Expose && report.xexpose.count == 0) {
      printf("Test completed. Closing the program.\n");
      break;
    }
  }

  // Clean up and close the display connection
  XDestroyWindow(display, win);
  XCloseDisplay(display);

  return 0;
}
