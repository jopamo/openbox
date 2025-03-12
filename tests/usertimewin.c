/* usertimewin.c for the Openbox window manager */

#include <stdio.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

int main() {
  Display* display;
  Window win, twin;
  XEvent report;
  Atom atime, atimewin;
  int x = 10, y = 10, h = 400, w = 400;
  Time num;

  // Open the X display
  display = XOpenDisplay(NULL);
  if (display == NULL) {
    fprintf(stderr, "couldn't connect to X server :0\n");
    return 1;  // Return 1 to indicate failure if unable to connect to the X server
  }

  // Intern atoms for window time properties
  atime = XInternAtom(display, "_NET_WM_USER_TIME", True);
  atimewin = XInternAtom(display, "_NET_WM_USER_TIME_WINDOW", True);

  // Create two windows
  win = XCreateWindow(display, RootWindow(display, 0), x, y, w, h, 10, CopyFromParent, CopyFromParent, CopyFromParent,
                      0, 0);
  twin = XCreateWindow(display, RootWindow(display, 0), x, y, w / 2, h / 2, 10, CopyFromParent, CopyFromParent,
                       CopyFromParent, 0, 0);

  // Set the background color for the window
  XSetWindowBackground(display, win, WhitePixel(display, 0));

  // Map the window and flush the display
  XMapWindow(display, win);
  XFlush(display);

  // Simulate some delay
  sleep(2);

  // Set the _NET_WM_USER_TIME_WINDOW property on the first window
  printf("Setting time window\n");
  XChangeProperty(display, win, atimewin, XA_WINDOW, 32, PropModeReplace, (unsigned char*)&twin, 1);
  XFlush(display);

  // Simulate some delay
  sleep(1);

  // Set the _NET_WM_USER_TIME property on the second window
  num = 100;
  printf("Setting time stamp on time window\n");
  XChangeProperty(display, twin, atime, XA_CARDINAL, 32, PropModeReplace, (unsigned char*)&num, 1);
  XFlush(display);

  // Select input events to listen for
  XSelectInput(display, win, ExposureMask | StructureNotifyMask);
  XSelectInput(display, twin, ExposureMask | StructureNotifyMask);

  // Event loop to process events
  while (1) {
    XNextEvent(display, &report);

    switch (report.type) {
      case Expose:
        printf("exposed\n");
        break;
      case ConfigureNotify:
        printf("confignotify %i,%i-%ix%i\n", report.xconfigure.x, report.xconfigure.y, report.xconfigure.width,
               report.xconfigure.height);
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
  XDestroyWindow(display, twin);
  XCloseDisplay(display);

  return 0;  // Return 0 to indicate success
}
