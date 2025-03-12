/* grouptrancircular2.c for the Openbox window manager */

#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

int main() {
  Display* display;
  Window one, two, three, group;
  XEvent report;
  XWMHints* wmhints;

  // Open the X display
  display = XOpenDisplay(NULL);
  if (display == NULL) {
    fprintf(stderr, "couldn't connect to X server :0\n");
    return 1;  // Return 1 to indicate failure if unable to connect to the X server
  }

  // Create the group window (invisible)
  group = XCreateWindow(display, RootWindow(display, 0), 0, 0, 1, 1, 10, CopyFromParent, CopyFromParent, CopyFromParent,
                        0, 0);

  // Create the main and child windows
  one = XCreateWindow(display, RootWindow(display, 0), 0, 0, 100, 100, 10, CopyFromParent, CopyFromParent,
                      CopyFromParent, 0, 0);
  two = XCreateWindow(display, RootWindow(display, 0), 0, 0, 100, 100, 10, CopyFromParent, CopyFromParent,
                      CopyFromParent, 0, 0);
  three = XCreateWindow(display, RootWindow(display, 0), 0, 0, 100, 100, 10, CopyFromParent, CopyFromParent,
                        CopyFromParent, 0, 0);

  // Set window backgrounds
  XSetWindowBackground(display, one, WhitePixel(display, 0));
  XSetWindowBackground(display, two, BlackPixel(display, 0));

  // Set transient window hints
  XSetTransientForHint(display, one, RootWindow(display, 0));
  XSetTransientForHint(display, two, one);
  XSetTransientForHint(display, three, two);

  // Set window group hints
  wmhints = XAllocWMHints();
  wmhints->flags = WindowGroupHint;
  wmhints->window_group = group;

  XSetWMHints(display, one, wmhints);
  XSetWMHints(display, two, wmhints);
  XSetWMHints(display, three, wmhints);

  XFree(wmhints);

  // Map the windows and flush the display
  XMapWindow(display, one);
  XMapWindow(display, two);
  XMapWindow(display, three);
  XFlush(display);

  // Select input events for the windows
  XSelectInput(display, one, ExposureMask | StructureNotifyMask);
  XSelectInput(display, two, ExposureMask | StructureNotifyMask);
  XSelectInput(display, three, ExposureMask | StructureNotifyMask);

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
  XDestroyWindow(display, one);
  XDestroyWindow(display, two);
  XDestroyWindow(display, three);
  XDestroyWindow(display, group);
  XCloseDisplay(display);

  return 0;  // Return 0 to indicate success
}
