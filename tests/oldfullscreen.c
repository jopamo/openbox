/* oldfullscreen.c for the Openbox window manager */

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

typedef struct {
  unsigned long flags;
  unsigned long functions;
  unsigned long decorations;
  long inputMode;
  unsigned long status;
} Hints;

int main(int argc, char** argv) {
  Display* display;
  Window win;
  XEvent report;
  int x = 200, y = 200, h = 100, w = 400, s;
  XSizeHints* size;
  Hints hints;
  Atom prop;

  // Open X display
  display = XOpenDisplay(NULL);
  if (display == NULL) {
    fprintf(stderr, "couldn't connect to X server :0\n");
    return 1;
  }

  // Get geometry of root window (default screen)
  XGetGeometry(display, RootWindow(display, DefaultScreen(display)), &win, &x, &y, &w, &h, &s, &s);

  // Create a new window
  win = XCreateWindow(display, RootWindow(display, 0), x, y, w, h, 0, CopyFromParent, CopyFromParent, CopyFromParent, 0,
                      NULL);
  XSetWindowBackground(display, win, WhitePixel(display, 0));

  // Set window size hints
  size = XAllocSizeHints();
  size->flags = PPosition;
  XSetWMNormalHints(display, win, size);
  XFree(size);

  // Set _MOTIF_WM_HINTS property
  hints.flags = 2;
  hints.decorations = 0;
  prop = XInternAtom(display, "_MOTIF_WM_HINTS", False);
  XChangeProperty(display, win, prop, prop, 32, PropModeReplace, (unsigned char*)&hints, 5);

  XFlush(display);
  XMapWindow(display, win);

  // Listen for events
  XSelectInput(display, win, StructureNotifyMask | ButtonPressMask);

  // Simulate resizing and moving the window
  sleep(1);
  XResizeWindow(display, win, w + 5, h + 5);
  XMoveWindow(display, win, x, y);

  // Event loop to handle ButtonPress and ConfigureNotify events
  while (1) {
    XNextEvent(display, &report);

    switch (report.type) {
      case ButtonPress:
        // Simulate button press (window will be unmapped)
        XUnmapWindow(display, win);
        printf("Window unmapped due to ButtonPress\n");
        break;

      case ConfigureNotify:
        x = report.xconfigure.x;
        y = report.xconfigure.y;
        w = report.xconfigure.width;
        h = report.xconfigure.height;
        s = report.xconfigure.send_event;
        printf("confignotify %i,%i-%ix%i (send: %d)\n", x, y, w, h, s);
        break;
    }

    // Exit after handling events
    if (report.type == ButtonPress) {
      printf("Test completed. Closing the program.\n");
      break;
    }
  }

  // Clean up and close the display connection
  XDestroyWindow(display, win);
  XCloseDisplay(display);

  return 0;
}
