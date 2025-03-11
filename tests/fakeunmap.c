/* fakeunmap.c for Openbox window manager */

#include <stdio.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdlib.h>

void test_window_state(Display *display, Window win, int expected_mapped) {
    XWindowAttributes attr;
    XGetWindowAttributes(display, win, &attr);

    if ((attr.map_state == expected_mapped) ? 1 : 0) {
        printf("Test passed: Window is in the expected mapped state\n");
    } else {
        printf("Test failed: Expected window to be %s, but it is %s\n",
               (expected_mapped == IsViewable) ? "mapped" : "unmapped",
               (attr.map_state == IsViewable) ? "mapped" : "unmapped");
    }
}

int main () {
    Display   *display;
    Window     win;
    XEvent     report;
    XEvent     msg;
    int        x = 50, y = 50, h = 100, w = 400;

    display = XOpenDisplay(NULL);

    if (display == NULL) {
        fprintf(stderr, "couldn't connect to X server :0\n");
        return 1;
    }

    win = XCreateWindow(display, RootWindow(display, 0),
                        x, y, w, h, 10, CopyFromParent, CopyFromParent,
                        CopyFromParent, 0, NULL);
    XSetWindowBackground(display, win, WhitePixel(display, 0));

    XMapWindow(display, win);
    XFlush(display);
    usleep(10000);  // Wait for the window to be mapped

    // Verify that the window is mapped
    test_window_state(display, win, IsViewable);

    // Simulate UnmapNotify event
    msg.type = UnmapNotify;
    msg.xunmap.display = display;
    msg.xunmap.event = RootWindow(display, DefaultScreen(display));
    msg.xunmap.window = win;
    msg.xunmap.from_configure = False;
    XSendEvent(display, RootWindow(display, DefaultScreen(display)), False,
               SubstructureRedirectMask | SubstructureNotifyMask, &msg);

    usleep(10000);  // Wait after sending the UnmapNotify event

    // Unmap the window
    XUnmapWindow(display, win);
    XSync(display, False);

    // Verify that the window is unmapped
    test_window_state(display, win, IsUnmapped);

    XCloseDisplay(display);  // Close the display properly

    return 0;
}
