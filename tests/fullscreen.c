/* fullscreen.c for the Openbox window manager */

#include <stdio.h>
#include <unistd.h>
#include <X11/Xlib.h>

int main() {
  Display* display;
  Window win;
  XEvent report;
  Atom _net_fs, _net_state;
  XEvent msg;
  int x = 10, y = 10, h = 100, w = 400;

  // Open the X display
  display = XOpenDisplay(NULL);
  if (display == NULL) {
    fprintf(stderr, "couldn't connect to X server :0\n");
    return 1;  // Return 1 if the display can't be opened
  }

  // Intern atoms for window state
  _net_state = XInternAtom(display, "_NET_WM_STATE", False);
  _net_fs = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", False);

  // Create a window
  win = XCreateWindow(display, RootWindow(display, 0), x, y, w, h, 10, CopyFromParent, CopyFromParent, CopyFromParent,
                      0, NULL);

  // Set the background color for the window
  XSetWindowBackground(display, win, WhitePixel(display, 0));

  // Map the window and flush the display
  XMapWindow(display, win);
  XFlush(display);
  sleep(2);

  // Send a message to toggle fullscreen state
  printf("fullscreen\n");
  msg.xclient.type = ClientMessage;
  msg.xclient.message_type = _net_state;
  msg.xclient.display = display;
  msg.xclient.window = win;
  msg.xclient.format = 32;
  msg.xclient.data.l[0] = 2;  // toggle
  msg.xclient.data.l[1] = _net_fs;
  msg.xclient.data.l[2] = 0l;
  msg.xclient.data.l[3] = 0l;
  msg.xclient.data.l[4] = 0l;
  XSendEvent(display, RootWindow(display, 0), False, SubstructureNotifyMask | SubstructureRedirectMask, &msg);
  XFlush(display);
  sleep(2);

  // Send a message to restore the window
  printf("restore\n");
  msg.xclient.type = ClientMessage;
  msg.xclient.message_type = _net_state;
  msg.xclient.display = display;
  msg.xclient.window = win;
  msg.xclient.format = 32;
  msg.xclient.data.l[0] = 2;  // toggle
  msg.xclient.data.l[1] = _net_fs;
  msg.xclient.data.l[2] = 0l;
  msg.xclient.data.l[3] = 0l;
  msg.xclient.data.l[4] = 0l;
  XSendEvent(display, RootWindow(display, 0), False, SubstructureNotifyMask | SubstructureRedirectMask, &msg);

  // Select input events to listen for
  XSelectInput(display, win, ExposureMask | StructureNotifyMask);

  // Event loop to process events
  while (1) {
    XNextEvent(display, &report);

    switch (report.type) {
      case Expose:
        printf("exposed\n");
        break;
      case ConfigureNotify:
        x = report.xconfigure.x;
        y = report.xconfigure.y;
        w = report.xconfigure.width;
        h = report.xconfigure.height;
        printf("confignotify %i,%i-%ix%i\n", x, y, w, h);
        break;
    }

    // Exit after processing the first event to avoid infinite loops in CI
    if (report.type == Expose && report.xexpose.count == 0) {
      printf("Test completed. Closing the program.\n");
      break;
    }
  }

  // Clean up and close the display connection
  XDestroyWindow(display, win);
  XCloseDisplay(display);

  return 0;  // Return 0 to indicate success
}
