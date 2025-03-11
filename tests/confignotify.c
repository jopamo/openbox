// confignotify.c for the Openbox window manager

#include <stdio.h>
#include <unistd.h>
#include <X11/Xlib.h>

int main () {
    Display *display;
    Window win;
    XEvent report;
    int x = 10, y = 10, h = 100, w = 100;

    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "couldn't connect to X server :0\n");
        return 1;  // Return failure if the display cannot be opened
    }

    win = XCreateWindow(display, RootWindow(display, 0),
                        x, y, w, h, 0, CopyFromParent, CopyFromParent,
                        CopyFromParent, 0, NULL);

    XSetWindowBackground(display, win, WhitePixel(display, 0));

    XSelectInput(display, win, (ExposureMask | StructureNotifyMask | GravityNotify));

    XMapWindow(display, win);
    XFlush(display);

    sleep(1);
    XResizeWindow(display, win, w + 5, h + 5);
    XMoveWindow(display, win, x, y);

    while (1) {
        XNextEvent(display, &report);

        switch (report.type) {
        case MapNotify:
            printf("map notify\n");
            break;
        case Expose:
            printf("exposed\n");
            break;
        case GravityNotify:
            printf("gravity notify event 0x%x window 0x%x x %d y %d\n",
                   report.xgravity.event, report.xgravity.window,
                   report.xgravity.x, report.xgravity.y);
            break;
        case ConfigureNotify: {
            int se = report.xconfigure.send_event;
            int event = report.xconfigure.event;
            int window = report.xconfigure.window;
            int x = report.xconfigure.x;
            int y = report.xconfigure.y;
            int w = report.xconfigure.width;
            int h = report.xconfigure.height;
            int bw = report.xconfigure.border_width;
            int above = report.xconfigure.above;
            int or = report.xconfigure.override_redirect;
            printf("confignotify send %d ev 0x%x win 0x%x %i,%i-%ix%i bw %i\n"
                   "             above 0x%x ovrd %d\n",
                   se, event, window, x, y, w, h, bw, above, or);
            break;
        }
        }

    }

    XDestroyWindow(display, win);  // Destroy the window
    XCloseDisplay(display);        // Close the display connection

    return 0;
}
