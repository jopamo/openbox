// title.c for the Openbox window manager

#include <string.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

int main(int argc, char** argv) {
  Display* display;
  Window win;
  XEvent report;
  int x = 10, y = 10, h = 100, w = 400;
  XSizeHints size;
  XTextProperty name;
  Atom nameprop, nameenc;

  if (argc < 2) {
    fprintf(stderr, "Usage: %s <window_title> [nameprop] [encoding]\n", argv[0]);
    return 1;
  }

  // Open connection to the X server
  display = XOpenDisplay(NULL);

  if (display == NULL) {
    fprintf(stderr, "couldn't connect to X server :0\n");
    return 1;
  }

  // Use the provided arguments for window property names and encoding
  if (argc > 2)
    nameprop = XInternAtom(display, argv[2], False);
  else
    nameprop = XInternAtom(display, "WM_NAME", False);

  if (argc > 3)
    nameenc = XInternAtom(display, argv[3], False);
  else
    nameenc = XInternAtom(display, "STRING", False);

  // Create a simple window
  win = XCreateWindow(display, RootWindow(display, 0), x, y, w, h, 10, CopyFromParent, CopyFromParent, CopyFromParent,
                      0, NULL);

  // Set the window background color to white
  XSetWindowBackground(display, win, WhitePixel(display, 0));

  // Set the window title using the provided argument (argv[1])
  XStringListToTextProperty(&argv[1], 1, &name);
  XSetWMName(display, win, &name);

  // Alternatively, you can set the window property directly (for example with XChangeProperty)
  XChangeProperty(display, win, nameprop, nameenc, 8, PropModeReplace, (unsigned char*)argv[1], strlen(argv[1]));

  // Flush the display to ensure changes take effect
  XFlush(display);

  // Map the window (make it visible)
  XMapWindow(display, win);

  // Set the event mask to handle exposure and configuration changes
  XSelectInput(display, win, ExposureMask | StructureNotifyMask);

  // Event loop
  while (1) {
    XNextEvent(display, &report);

    switch (report.type) {
      case Expose:
        // The window is exposed (redrawn)
        printf("Window exposed\n");
        break;

      case ConfigureNotify:
        // The window has been resized or moved
        x = report.xconfigure.x;
        y = report.xconfigure.y;
        w = report.xconfigure.width;
        h = report.xconfigure.height;
        printf("Window configured: %i,%i-%ix%i\n", x, y, w, h);
        break;
    }
  }

  return 0;
}
