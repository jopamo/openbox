/* overrideinputonly.c for the Openbox window manager */

#include <stdio.h>
#include <X11/Xlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
  XSetWindowAttributes xswa;
  unsigned long xswamask;
  Display* display;
  Window win;
  XEvent report;
  int i, x = 0, y = 0, h = 1, w = 1;

  // Parse the command-line arguments for geometry
  for (i = 0; i < argc; i++) {
    if (!strcmp(argv[i], "-g") || !strcmp(argv[i], "-geometry")) {
      XParseGeometry(argv[++i], &x, &y, &w, &h);
    }
  }

  // Open the X display
  display = XOpenDisplay(NULL);
  if (display == NULL) {
    fprintf(stderr, "couldn't connect to X server :0\n");
    return 0;
  }

  // Set the window to be override_redirect
  xswa.override_redirect = True;
  xswamask = CWOverrideRedirect;

  // Create an InputOnly window
  win = XCreateWindow(display, RootWindow(display, 0), x, y, w, h, 0, 0, InputOnly, CopyFromParent, xswamask, &xswa);

  // Map the window (make it visible)
  XMapWindow(display, win);
  XFlush(display);

  // Simulate event handling
  while (1) {
    XNextEvent(display, &report);
    // Exit after handling the first event to avoid an infinite loop in CI
    if (report.type == Expose) {
      printf("Expose event handled\n");
      break;
    }
  }

  // Clean up and close the display connection
  XDestroyWindow(display, win);
  XCloseDisplay(display);

  return 0;
}
