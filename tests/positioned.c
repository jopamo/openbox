/* positioned.c for the Openbox window manager */

#include <string.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

int main(int argc, char** argv) {
  Display* display;
  Window win;
  XEvent report;
  int x = 200, y = 200, h = 100, w = 400, s;
  XSizeHints* size;

  // Open X display
  display = XOpenDisplay(NULL);
  if (display == NULL) {
    fprintf(stderr, "couldn't connect to X server :0\n");
    return 0;
  }

  // Create the window
  win = XCreateWindow(display, RootWindow(display, 0), x, y, w, h, 0, CopyFromParent, CopyFromParent, CopyFromParent, 0,
                      NULL);
  XSetWindowBackground(display, win, WhitePixel(display, 0));

  // Set size hints for the window
  size = XAllocSizeHints();
  size->flags = PPosition;
  XSetWMNormalHints(display, win, size);
  XFree(size);

  // Map the window and flush the display
  XFlush(display);
  XMapWindow(display, win);

  // Select events to listen for
  XSelectInput(display, win, StructureNotifyMask | ButtonPressMask);

  // Simulate an event loop, exit after handling events
  while (1) {
    XNextEvent(display, &report);

    switch (report.type) {
      case ButtonPress:
        // Simulate button press event to unmap the window
        XUnmapWindow(display, win);
        break;

      case ConfigureNotify:
        // Handle window configuration changes
        x = report.xconfigure.x;
        y = report.xconfigure.y;
        w = report.xconfigure.width;
        h = report.xconfigure.height;
        s = report.xconfigure.send_event;
        printf("confignotify %i,%i-%ix%i (send: %d)\n", x, y, w, h, s);
        break;
    }

    // Exit the loop after handling an event
    if (report.type == ButtonPress) {
      printf("Test completed. Closing the program.\n");
      break;
    }
  }

  // Cleanup and close the display connection
  XDestroyWindow(display, win);
  XCloseDisplay(display);

  return 0;
}
