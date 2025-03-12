/* grouptran2.c for the Openbox window manager */

#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

int main() {
  Display* display;
  Window main, grouptran, child, group;
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
  main = XCreateWindow(display, RootWindow(display, 0), 0, 0, 100, 100, 10, CopyFromParent, CopyFromParent,
                       CopyFromParent, 0, 0);
  grouptran = XCreateWindow(display, RootWindow(display, 0), 10, 10, 80, 180, 10, CopyFromParent, CopyFromParent,
                            CopyFromParent, 0, 0);
  child = XCreateWindow(display, RootWindow(display, 0), 20, 20, 60, 60, 10, CopyFromParent, CopyFromParent,
                        CopyFromParent, 0, 0);

  // Set window backgrounds
  XSetWindowBackground(display, main, WhitePixel(display, 0));
  XSetWindowBackground(display, grouptran, BlackPixel(display, 0));
  XSetWindowBackground(display, child, WhitePixel(display, 0));

  // Set transient window hints
  XSetTransientForHint(display, grouptran, RootWindow(display, 0));
  XSetTransientForHint(display, child, grouptran);

  // Set window group hints
  wmhints = XAllocWMHints();
  wmhints->flags = WindowGroupHint;
  wmhints->window_group = group;

  XSetWMHints(display, main, wmhints);
  XSetWMHints(display, grouptran, wmhints);
  XSetWMHints(display, child, wmhints);

  XFree(wmhints);

  // Map the windows and flush the display
  XMapWindow(display, main);
  XMapWindow(display, grouptran);
  XMapWindow(display, child);
  XFlush(display);

  // Listen for events
  XSelectInput(display, main, ExposureMask | StructureNotifyMask);
  XSelectInput(display, grouptran, ExposureMask | StructureNotifyMask);
  XSelectInput(display, child, ExposureMask | StructureNotifyMask);

  // Event loop
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
  XDestroyWindow(display, main);
  XDestroyWindow(display, grouptran);
  XDestroyWindow(display, child);
  XDestroyWindow(display, group);
  XCloseDisplay(display);

  return 0;  // Return 0 to indicate success
}
