/* urgent.c for the Openbox window manager */

#include <stdio.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

int main() {
  Display* display;
  Window win;
  XEvent report;
  Atom _net_fs, _net_state;
  XEvent msg;
  int x = 50, y = 50, h = 100, w = 400;
  XWMHints hint;

  // Open the X display
  display = XOpenDisplay(NULL);
  if (display == NULL) {
    fprintf(stderr, "couldn't connect to X server :0\n");
    return 1;  // Return 1 to indicate failure if unable to connect to the X server
  }

  // Intern atoms for window state
  _net_state = XInternAtom(display, "_NET_WM_STATE", False);
  _net_fs = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", False);

  // Create the window
  win = XCreateWindow(display, RootWindow(display, 0), x, y, w, h, 10, CopyFromParent, CopyFromParent, CopyFromParent,
                      0, NULL);

  // Set the background color for the window
  XSetWindowBackground(display, win, WhitePixel(display, 0));

  // Map the window and flush the display
  XMapWindow(display, win);
  XFlush(display);

  // Simulate a small delay
  sleep(1);

  // Set the urgency hint on the window
  printf("urgent on\n");
  hint.flags = XUrgencyHint;
  XSetWMHints(display, win, &hint);
  XFlush(display);

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
