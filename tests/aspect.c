/* aspect.c for Openbox window manager */

#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#include <unistd.h>

#define TOLERANCE 10  // Tolerance for position check

// Function to test the window position and size
void test_window(Display* display, Window win, int expected_x, int expected_y, int expected_w, int expected_h) {
  XWindowAttributes attr;
  XGetWindowAttributes(display, win, &attr);

  // Allow small tolerance for position check
  if (abs(attr.x - expected_x) <= TOLERANCE && abs(attr.y - expected_y) <= TOLERANCE && attr.width == expected_w &&
      attr.height == expected_h) {
    printf("Test passed: Window position and size are correct\n");
  }
  else {
    printf("Test failed: Expected position (%d, %d) and size (%dx%d), but got position (%d, %d) and size (%dx%d)\n",
           expected_x, expected_y, expected_w, expected_h, attr.x, attr.y, attr.width, attr.height);
  }
}

int main() {
  // Initialize X11 display and window
  Display* display;
  Window win;
  XEvent report;
  int x = 10, y = 10, w = 400, h = 100;
  XSizeHints size;

  // Connect to the X server
  display = XOpenDisplay(NULL);
  if (display == NULL) {
    fprintf(stderr, "couldn't connect to X server :0\n");
    return 1;
  }

  // Set window attributes
  XSetWindowAttributes xswa;
  xswa.win_gravity = StaticGravity;
  unsigned long xswamask = CWWinGravity;

  // Create a window
  win = XCreateWindow(display, RootWindow(display, 0), x, y, w, h, 10, CopyFromParent, CopyFromParent, CopyFromParent,
                      xswamask, &xswa);
  XSetWindowBackground(display, win, WhitePixel(display, 0));

  // Set aspect ratio hints
  size.flags = PAspect;
  size.min_aspect.x = 3;
  size.min_aspect.y = 3;
  size.max_aspect.x = 3;
  size.max_aspect.y = 3;
  XSetWMNormalHints(display, win, &size);

  // Map the window
  XMapWindow(display, win);
  XFlush(display);

  // Wait for the MapNotify event (window fully mapped)
  XSelectInput(display, win, ExposureMask | StructureNotifyMask | MapNotify);
  XNextEvent(display, &report);  // Wait for MapNotify event

  if (report.type == MapNotify) {
    // After the window is mapped, explicitly set position to (0, 0)
    XMoveWindow(display, win, 0, 0);
    XFlush(display);
    test_window(display, win, 0, 0, w, h);  // Test with expected position and size
  }

  // Simulate some events (e.g., ConfigureNotify)
  for (int i = 0; i < 3; i++) {
    // Simulate a ConfigureNotify event with a random position
    report.type = ConfigureNotify;
    report.xconfigure.x = rand() % 500;
    report.xconfigure.y = rand() % 500;
    report.xconfigure.width = w;
    report.xconfigure.height = h;
    XNextEvent(display, &report);  // Handle the event
    printf("confignotify %i,%i-%ix%i\n", report.xconfigure.x, report.xconfigure.y, report.xconfigure.width,
           report.xconfigure.height);

    // Check window attributes after resize
    test_window(display, win, report.xconfigure.x, report.xconfigure.y, w, h);
    usleep(500000);  // Sleep for 0.5 seconds to simulate time between events
  }

  // Clean up
  XDestroyWindow(display, win);
  XCloseDisplay(display);

  return 0;  // Success
}
