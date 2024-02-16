#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#ifdef _WIN32
#include <windows.h>
#include <winuser.h>
#define SLEEP(milliseconds) Sleep(milliseconds)
#define MOUSE_MOVE(x, y) SetCursorPos(x, y)
#elif __APPLE__
#include <ApplicationServices/ApplicationServices.h>
#include <unistd.h>
#define SLEEP(milliseconds) usleep(milliseconds * 1000)
#define MOUSE_MOVE(x, y) moveMouse(x, y)
void moveMouse(int x, int y) {
    CGEventSourceRef source = CGEventSourceCreate(kCGEventSourceStateHIDSystemState);
    CGEventRef move = CGEventCreateMouseEvent(source, kCGEventMouseMoved, CGPointMake(x, y), kCGMouseButtonLeft /*ignored*/);
    CGEventPost(kCGHIDEventTap, move);
    CFRelease(move);
    CFRelease(source);
}
#else
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <unistd.h>
#define SLEEP(milliseconds) usleep(milliseconds * 1000)
#define MOUSE_MOVE(x, y) moveMouse(x, y)
void moveMouse(int x, int y) {
    Display *display = XOpenDisplay(NULL);
    XWarpPointer(display, None, DefaultRootWindow(display), 0, 0, 0, 0, x, y);
    XFlush(display);
    XCloseDisplay(display);
}
#endif

void simulateMouseMovement(int distance, int duration) {
    int startX, startY;
    int simulationDuration = 3600000; // 1 godzina
    clock_t startTime = clock();

    #ifdef _WIN32
    POINT cursorPos;
    GetCursorPos(&cursorPos);
    startX = cursorPos.x;
    startY = cursorPos.y;
    #elif __APPLE__
    CGEventRef event = CGEventCreate(NULL);
    CGPoint currentPos = CGEventGetLocation(event);
    startX = (int)currentPos.x;
    startY = (int)currentPos.y;
    CFRelease(event);
    #else
    Display *display = XOpenDisplay(NULL);
    Window root = DefaultRootWindow(display);
    XWindowAttributes attrs;
    XGetWindowAttributes(display, root, &attrs);
    startX = attrs.width / 2;
    startY = attrs.height / 2;
    XCloseDisplay(display);
    #endif

    while ((clock() - startTime) * 1000 / CLOCKS_PER_SEC < simulationDuration) {
        for (int i = 0; i < distance; i++) {
            MOUSE_MOVE(startX, startY - i);
            SLEEP(duration / distance);
        }

        for (int i = 0; i < distance; i++) {
            MOUSE_MOVE(startX, startY - distance + i);
            SLEEP(duration / distance);
        }
    }
}

int main() {
    char response;
    int distance = 200;
    int duration = 1000;

    do {
        simulateMouseMovement(distance, duration);

        printf("Symulacja zakończona.\n");
        printf("Czy chcesz uruchomić symulację ponownie? (T/N): ");
        scanf(" %c", &response);
        response = tolower(response);
    } while (response == 't');

    printf("Koniec programu.\n");

    return 0;
}
