/* FlyWM is written by Christopher Dumas <christopherdumas@gmail.com> in 2015
 *
 * This software is under the GPL v3 license, and is provided with ABSULUTLY NO WARRENTY
 */

#include <X11/Xlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>

#define nil (0x0)
#define ever (;;)

int main () {
  char *applications[] = {"firefox", "xterm", "chromium-browser", "gnome-terminal"};
  int applist_size = 4;
  int seli = 0;

  Display *display = XOpenDisplay(nil);
  assert(display);
  XEvent event;

  int black_color = BlackPixel(display, DefaultScreen(display));
  int white_color = WhitePixel(display, DefaultScreen(display));
  Window w = XCreateSimpleWindow(display, DefaultRootWindow(display), 0, 0,
      150, 300, 2, black_color, white_color);
  XMapWindow(display, w);

  GC gc = XCreateGC(display, w, 0, nil);
  XSetForeground(display, gc, black_color);
  XSetBackground(display, gc, white_color);

  XGrabButton(display, 1, 0, w, True,
      ButtonPressMask|ButtonReleaseMask, GrabModeAsync,
      GrabModeAsync, None, None);

  // Arrow keys and enter:
  XGrabKey(display, XKeysymToKeycode(display, XStringToKeysym("Up")), 0, w, True, GrabModeAsync,
             GrabModeAsync); // Up
  XGrabKey(display, XKeysymToKeycode(display, XStringToKeysym("Down")), 0, w, True, GrabModeAsync,
             GrabModeAsync); // Down
  XGrabKey(display, XKeysymToKeycode(display, XStringToKeysym("Return")), 0, w, True, GrabModeAsync,
             GrabModeAsync); // Enter
  for ever {
    XClearWindow(display, w);
    int i = 0;
    for (i; i < applist_size; i++) {
      if (i == seli) {
        XFillRectangle(display, w, gc, 0, 15*i, 300, 19);
        XSetForeground(display, gc, white_color);
        XDrawString(display, w, gc, (150/8), 15*(i+1), applications[i], strlen(applications[i]));
        XSetForeground(display, gc, black_color);
      } else {
        XSetForeground(display, gc, black_color);
        XDrawString(display, w, gc, (150/8), 15*(i+1), applications[i], strlen(applications[i]));
      }
    }
    XFlush(display);

    XNextEvent(display, &event);
    if (event.type == KeyRelease) {
      if (event.xkey.keycode == XKeysymToKeycode(display, XStringToKeysym("Down"))) {
        seli = (seli + 1) % applist_size;
      } else if (event.xkey.keycode == XKeysymToKeycode(display, XStringToKeysym("Up"))) {
        seli = (seli - 1) % applist_size;
      } else if (event.xkey.keycode == XKeysymToKeycode(display, XStringToKeysym("Return"))) {
        system(applications[seli]);
      }
    }
  }

  return 0;
}
