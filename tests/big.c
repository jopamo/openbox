/* big.c for Openbox window manager */

#include <stdio.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <stdlib.h>

void test_window(Display* display, Window win, int expected_x, int expected_y, int expected_w, int expected_h) {
  XWindowAttributes attr;
  XGetWindowAttributes(display, win, &attr);

  if (attr.x == expected_x && attr.y == expected_y && attr.width == expected_w && attr.height == expected_h) {
    printf("Test passed: Window position and size are correct\n");
  }
  else {
    printf("Test failed: Expected position (%d, %d) and size (%dx%d), but got position (%d, %d) and size (%dx%d)\n",
           expected_x, expected_y, expected_w, expected_h, attr.x, attr.y, attr.width, attr.height);
  }
}

int main() {
  Display* display;
  Window win;
  XEvent report;
  int x = 10, y = 10, h = 2000, w = 2000;

  display = XOpenDisplay(NULL);

  if (display == NULL) {
    fprintf(stderr, "couldn't connect to X server :0\n");
    return 1;
  }

  win = XCreateWindow(display, RootWindow(display, 0), x, y, w, h, 10, CopyFromParent, CopyFromParent, CopyFromParent,
                      0, NULL);

  XSetWindowBackground(display, win, WhitePixel(display, 0));

  XMapWindow(display, win);
  XFlush(display);
  sleep(2);

  XSelectInput(display, win, ExposureMask | StructureNotifyMask);

  // Simulate a few events for testing
  for (int i = 0; i < 5; i++) {
    // Simulate Expose event
    report.type = Expose;
    printf("Simulating Expose event...\n");
    XNextEvent(display, &report);
    printf("exposed\n");

    // Simulate ConfigureNotify event with random position and size
    report.type = ConfigureNotify;
    report.xconfigure.x = rand() % 500;
    report.xconfigure.y = rand() % 500;
    report.xconfigure.width = 100 + rand() % 400;
    report.xconfigure.height = 100 + rand() % 400;
    XNextEvent(display, &report);

    // Print new window size after ConfigureNotify
    printf("confignotify %i,%i-%ix%i\n", report.xconfigure.x, report.xconfigure.y, report.xconfigure.width,
           report.xconfigure.height);

    // Test the window's new attributes
    test_window(display, win, report.xconfigure.x, report.xconfigure.y, report.xconfigure.width,
                report.xconfigure.height);

    // Sleep to simulate time between events
    usleep(500000);  // Sleep for 0.5 seconds
  }

  // Destroy the window and close the display connection
  XDestroyWindow(display, win);
  XCloseDisplay(display);  // Ensure the display is closed properly

  return 0;
}
