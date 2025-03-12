/* duplicatesession.c for the Openbox window manager */

#include <string.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

int main(int argc, char** argv) {
  Display* display;
  Window win1, win2;
  XEvent report;
  int x = 10, y = 10, h = 100, w = 400;
  XSizeHints size;
  XTextProperty name;
  Atom sm_id, enc;

  // Open the X display
  display = XOpenDisplay(NULL);
  if (display == NULL) {
    fprintf(stderr, "couldn't connect to X server :0\n");
    return 0;
  }

  // Intern atoms
  sm_id = XInternAtom(display, "SM_CLIENT_ID", False);
  enc = XInternAtom(display, "STRING", False);

  // Create two windows
  win1 = XCreateWindow(display, RootWindow(display, 0), x, y, w, h, 10, CopyFromParent, CopyFromParent, CopyFromParent,
                       0, NULL);
  win2 = XCreateWindow(display, RootWindow(display, 0), x, y, w, h, 10, CopyFromParent, CopyFromParent, CopyFromParent,
                       0, NULL);

  // Set background colors for the windows
  XSetWindowBackground(display, win1, WhitePixel(display, 0));
  XSetWindowBackground(display, win2, BlackPixel(display, 0));

  // Change the properties for the windows
  XChangeProperty(display, win1, sm_id, enc, 8, PropModeAppend, "abcdefg", strlen("abcdefg"));
  XChangeProperty(display, win2, sm_id, enc, 8, PropModeAppend, "abcdefg", strlen("abcdefg"));

  // Flush the display and map the windows
  XFlush(display);
  XMapWindow(display, win1);
  XMapWindow(display, win2);

  // Select the events to listen for
  XSelectInput(display, win1, ExposureMask | StructureNotifyMask);
  XSelectInput(display, win2, ExposureMask | StructureNotifyMask);

  // Event loop to process events
  while (1) {
    XNextEvent(display, &report);

    switch (report.type) {
      case Expose:
        printf("Expose event received\n");
        break;
      case ConfigureNotify:
        printf("ConfigureNotify: window %lu moved to (%d, %d) and resized to %dx%d\n", report.xconfigure.window,
               report.xconfigure.x, report.xconfigure.y, report.xconfigure.width, report.xconfigure.height);
        break;
    }

    // Exit the loop after handling the first event to avoid infinite loops in CI
    if (report.type == Expose && report.xexpose.count == 0) {
      printf("Test completed. Closing the program.\n");
      break;
    }
  }

  // Cleanup and close the display connection
  XDestroyWindow(display, win1);
  XDestroyWindow(display, win2);
  XCloseDisplay(display);

  return 0;
}
