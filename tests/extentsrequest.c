/* extentsrequest.c for the Openbox window manager */

#include <stdio.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <stdlib.h>

void request(Display* display, Atom _request, Atom _extents, Window win) {
  XEvent msg;
  msg.xclient.type = ClientMessage;
  msg.xclient.message_type = _request;
  msg.xclient.display = display;
  msg.xclient.window = win;
  msg.xclient.format = 32;
  msg.xclient.data.l[0] = 0l;
  msg.xclient.data.l[1] = 0l;
  msg.xclient.data.l[2] = 0l;
  msg.xclient.data.l[3] = 0l;
  msg.xclient.data.l[4] = 0l;
  XSendEvent(display, RootWindow(display, 0), False, SubstructureNotifyMask | SubstructureRedirectMask, &msg);
  XFlush(display);
}

void reply(Display* display, Atom _extents) {
  printf("  waiting for extents\n");
  while (1) {
    XEvent report;
    XNextEvent(display, &report);

    if (report.type == PropertyNotify && report.xproperty.atom == _extents) {
      Atom ret_type;
      int ret_format;
      unsigned long ret_items, ret_bytesleft;
      unsigned long* prop_return;
      XGetWindowProperty(display, report.xproperty.window, _extents, 0, 4, False, XA_CARDINAL, &ret_type, &ret_format,
                         &ret_items, &ret_bytesleft, (unsigned char**)&prop_return);
      if (ret_type == XA_CARDINAL && ret_format == 32 && ret_items == 4) {
        printf("  got new extents %lu, %lu, %lu, %lu\n", prop_return[0], prop_return[1], prop_return[2],
               prop_return[3]);
      }
      break;
    }
  }
}

void test_request(Display* display,
                  Window win,
                  Atom _request,
                  Atom _extents,
                  Atom _type,
                  Atom _state,
                  Atom state_value,
                  const char* state_name) {
  printf("requesting for type %s\n", state_name);
  XChangeProperty(display, win, _type, XA_ATOM, 32, PropModeReplace, (unsigned char*)&_type, 1);
  XChangeProperty(display, win, _state, XA_ATOM, 32, PropModeReplace, (unsigned char*)&state_value, 1);
  request(display, _request, _extents, win);
  reply(display, _extents);
}

int main() {
  Display* display;
  Window win;
  Atom _request, _extents, _type, _normal, _desktop, _state;
  Atom _state_fs, _state_mh, _state_mv;
  int x = 10, y = 10, h = 100, w = 400;

  display = XOpenDisplay(NULL);

  if (display == NULL) {
    fprintf(stderr, "couldn't connect to X server :0\n");
    return 1;
  }

  _type = XInternAtom(display, "_NET_WM_WINDOW_TYPE", False);
  _normal = XInternAtom(display, "_NET_WM_WINDOW_TYPE_NORMAL", False);
  _desktop = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DESKTOP", False);
  _request = XInternAtom(display, "_NET_REQUEST_FRAME_EXTENTS", False);
  _extents = XInternAtom(display, "_NET_FRAME_EXTENTS", False);
  _state = XInternAtom(display, "_NET_WM_STATE", False);
  _state_fs = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", False);
  _state_mh = XInternAtom(display, "_NET_WM_STATE_MAXIMIZED_HORZ", False);
  _state_mv = XInternAtom(display, "_NET_WM_STATE_MAXIMIZED_VERT", False);

  win = XCreateWindow(display, RootWindow(display, 0), x, y, w, h, 10, CopyFromParent, CopyFromParent, CopyFromParent,
                      0, NULL);
  XSelectInput(display, win, PropertyChangeMask);

  // Test all the states
  test_request(display, win, _request, _extents, _normal, _state, _state_fs, "normal+fullscreen");
  test_request(display, win, _request, _extents, _normal, _state, _state_mv, "normal+maximized_vert");
  test_request(display, win, _request, _extents, _normal, _state, _state_mh, "normal+maximized_horz");
  test_request(display, win, _request, _extents, _normal, _state, _desktop, "desktop");

  XCloseDisplay(display);  // Close the display properly

  return 0;
}
