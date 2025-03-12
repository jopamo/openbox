/* wmhints.c for the Openbox window manager */

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
  Hints hints;
  Atom prop;

  // Open the X display
  display = XOpenDisplay(NULL);
  if (display == NULL) {
    fprintf(stderr, "couldn't connect to X server :0\n");
    return 1;  // Return 1 to indicate failure if unable to connect to the X server
  }

  // Create the window
  win = XCreateWindow(display, RootWindow(display, 0), x, y, w, h, 0, CopyFromParent, CopyFromParent, CopyFromParent, 0,
                      NULL);
  XSetWindowBackground(display, win, WhitePixel(display, 0));

  // Set the MOTIF WM hints (disable decorations)
  hints.flags = 2;
  hints.decorations = 0;
  prop = XInternAtom(display, "_MOTIF_WM_HINTS", False);
  XChangeProperty(display, win, prop, prop, 32, PropModeReplace, (unsigned char*)&hints, 5);

  // Select input events to listen for
  XSelectInput(display, win, StructureNotifyMask | ButtonPressMask);

  // Map the window and flush the display
  XMapWindow(display, win);
  XFlush(display);

  // Simulate changing the window decorations (enable border and title)
  sleep(1);
  hints.flags = 2;
  hints.decorations = (1 << 3) || (1 << 1);  // Set border and title
  prop = XInternAtom(display, "_MOTIF_WM_HINTS", False);
  XChangeProperty(display, win, prop, prop, 32, PropModeReplace, (unsigned char*)&hints, 5);

  XMapWindow(display, win);
  XFlush(display);

  // Process events
  int event_processed = 0;
  while (!event_processed) {
    XNextEvent(display, &report);

    switch (report.type) {
      case ButtonPress:
        XUnmapWindow(display, win);
        event_processed = 1;
        break;
      case ConfigureNotify:
        x = report.xconfigure.x;
        y = report.xconfigure.y;
        w = report.xconfigure.width;
        h = report.xconfigure.height;
        s = report.xconfigure.send_event;
        printf("confignotify %i,%i-%ix%i (send: %d)\n", x, y, w, h, s);
        event_processed = 1;
        break;
    }

    // Exit after handling the first event to avoid infinite loops
  }

  // Clean up and close the display connection
  XDestroyWindow(display, win);
  XCloseDisplay(display);

  return 0;  // Return 0 to indicate success
}
