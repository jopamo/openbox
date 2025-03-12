/* focusout.c for the Openbox window manager */

#include <stdio.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

int main() {
  Display* display;
  Window win, child;
  XEvent report;
  Atom _net_fs, _net_state;
  XEvent msg;
  int x = 50, y = 50, h = 100, w = 400;
  XWMHints hint;

  // Open the X display
  display = XOpenDisplay(NULL);
  if (display == NULL) {
    fprintf(stderr, "couldn't connect to X server :0\n");
    return 0;
  }

  // Create windows
  win = XCreateWindow(display, RootWindow(display, 0), x, y, w, h, 10, CopyFromParent, CopyFromParent, CopyFromParent,
                      0, NULL);
  child =
      XCreateWindow(display, win, 10, 10, w - 20, h - 20, 0, CopyFromParent, CopyFromParent, CopyFromParent, 0, NULL);

  // Set background colors for the windows
  XSetWindowBackground(display, win, WhitePixel(display, 0));
  XSetWindowBackground(display, child, BlackPixel(display, 0));

  // Select input events
  XSelectInput(display, win, FocusChangeMask | EnterWindowMask | LeaveWindowMask);
  XMapWindow(display, win);
  XMapWindow(display, child);

  // Event loop to process events
  while (1) {
    const char *mode, *detail;

    // Wait for the next event
    XNextEvent(display, &report);

    switch (report.type) {
      case ButtonPress:
        printf("button press\n");
        break;
      case MotionNotify:
        printf("motion\n");
        break;
      case ButtonRelease:
        break;
      case FocusIn:
        switch (report.xfocus.mode) {
          case NotifyNormal:
            mode = "NotifyNormal";
            break;
          case NotifyGrab:
            mode = "NotifyGrab";
            break;
          case NotifyUngrab:
            mode = "NotifyUngrab";
            break;
        }

        switch (report.xfocus.detail) {
          case NotifyAncestor:
            detail = "NotifyAncestor";
            break;
          case NotifyVirtual:
            detail = "NotifyVirtual";
            break;
          case NotifyInferior:
            detail = "NotifyInferior";
            break;
          case NotifyNonlinear:
            detail = "NotifyNonlinear";
            break;
          case NotifyNonlinearVirtual:
            detail = "NotifyNonlinearVirtual";
            break;
          case NotifyPointer:
            detail = "NotifyPointer";
            break;
          case NotifyPointerRoot:
            detail = "NotifyPointerRoot";
            break;
          case NotifyDetailNone:
            detail = "NotifyDetailNone";
            break;
        }
        printf("focusin\n");
        break;
      case FocusOut:
        switch (report.xfocus.mode) {
          case NotifyNormal:
            mode = "NotifyNormal";
            break;
          case NotifyGrab:
            mode = "NotifyGrab";
            break;
          case NotifyUngrab:
            mode = "NotifyUngrab";
            break;
        }

        switch (report.xfocus.detail) {
          case NotifyAncestor:
            detail = "NotifyAncestor";
            break;
          case NotifyVirtual:
            detail = "NotifyVirtual";
            break;
          case NotifyInferior:
            detail = "NotifyInferior";
            break;
          case NotifyNonlinear:
            detail = "NotifyNonlinear";
            break;
          case NotifyNonlinearVirtual:
            detail = "NotifyNonlinearVirtual";
            break;
          case NotifyPointer:
            detail = "NotifyPointer";
            break;
          case NotifyPointerRoot:
            detail = "NotifyPointerRoot";
            break;
          case NotifyDetailNone:
            detail = "NotifyDetailNone";
            break;
        }
        printf("focusout\n");
        break;
      case EnterNotify:
        switch (report.xcrossing.mode) {
          case NotifyNormal:
            mode = "NotifyNormal";
            break;
          case NotifyGrab:
            mode = "NotifyGrab";
            break;
          case NotifyUngrab:
            mode = "NotifyUngrab";
            break;
        }

        switch (report.xcrossing.detail) {
          case NotifyAncestor:
            detail = "NotifyAncestor";
            break;
          case NotifyVirtual:
            detail = "NotifyVirtual";
            break;
          case NotifyInferior:
            detail = "NotifyInferior";
            break;
          case NotifyNonlinear:
            detail = "NotifyNonlinear";
            break;
          case NotifyNonlinearVirtual:
            detail = "NotifyNonlinearVirtual";
            break;
          case NotifyPointer:
            detail = "NotifyPointer";
            break;
          case NotifyPointerRoot:
            detail = "NotifyPointerRoot";
            break;
          case NotifyDetailNone:
            detail = "NotifyDetailNone";
            break;
        }
        printf("enternotify\n");
        break;
      case LeaveNotify:
        switch (report.xcrossing.mode) {
          case NotifyNormal:
            mode = "NotifyNormal";
            break;
          case NotifyGrab:
            mode = "NotifyGrab";
            break;
          case NotifyUngrab:
            mode = "NotifyUngrab";
            break;
        }

        switch (report.xcrossing.detail) {
          case NotifyAncestor:
            detail = "NotifyAncestor";
            break;
          case NotifyVirtual:
            detail = "NotifyVirtual";
            break;
          case NotifyInferior:
            detail = "NotifyInferior";
            break;
          case NotifyNonlinear:
            detail = "NotifyNonlinear";
            break;
          case NotifyNonlinearVirtual:
            detail = "NotifyNonlinearVirtual";
            break;
          case NotifyPointer:
            detail = "NotifyPointer";
            break;
          case NotifyPointerRoot:
            detail = "NotifyPointerRoot";
            break;
          case NotifyDetailNone:
            detail = "NotifyDetailNone";
            break;
        }
        printf("leavenotify\n");
        break;
    }

    // Exit after handling the first event to avoid infinite loops in CI
    if (report.type == FocusOut) {
      printf("Test completed. Closing the program.\n");
      break;
    }
  }

  // Clean up and close the display connection
  XDestroyWindow(display, win);
  XDestroyWindow(display, child);
  XCloseDisplay(display);

  return 1;
}
