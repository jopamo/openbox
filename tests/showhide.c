/*

   showhide.c for the Openbox window manager

*/

#include <stdio.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

int main() {
  Display* display;
  Window win;
  XEvent report;
  int x = 50, y = 50, h = 100, w = 400;

  // Open connection to X server
  display = XOpenDisplay(NULL);

  if (display == NULL) {
    fprintf(stderr, "couldn't connect to X server :0\n");
    return 1;  // Return 1 if unable to connect to X server
  }

  // Create the window
  win = XCreateWindow(display, RootWindow(display, 0), x, y, w, h, 10, CopyFromParent, CopyFromParent, CopyFromParent,
                      0, NULL);

  if (win == 0) {
    fprintf(stderr, "Failed to create window\n");
    return 1;  // Return 1 if window creation fails
  }

  XSetWindowBackground(display, win, WhitePixel(display, 0));
  XMapWindow(display, win);  // Make the window visible
  XFlush(display);           // Ensure the window gets displayed

  // Sleep to keep the window visible for 2 seconds
  sleep(2);

  // Destroy the window after the delay
  XDestroyWindow(display, win);
  XSync(display, False);  // Flush requests to the server

  return 0;
}
