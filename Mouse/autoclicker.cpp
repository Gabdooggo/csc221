#include <iostream>
#include <ApplicationServices/ApplicationServices.h>
#include <CoreFoundation/CoreFoundation.h>
#include <cstdlib>
#include <string>
#include <cmath>
#include <chrono>
#include <thread>


static bool command = true;
static bool clicker = false;
static int counter = 0;
CGEventMask mask = 0;
static CFMachPortRef g_eventTap = nullptr;
int cperms = 1000; // clicks per milisecond

static void clickMouse(bool right = false) {
    // Get current mouse position
    CGEventRef event = CGEventCreate(nullptr);
    CGPoint pos = CGEventGetLocation(event);
    CFRelease(event);

    CGMouseButton btn = right ? kCGMouseButtonRight : kCGMouseButtonLeft;
    CGEventType down = right ? kCGEventRightMouseDown : kCGEventLeftMouseDown;
    CGEventType up   = right ? kCGEventRightMouseUp   : kCGEventLeftMouseUp;

    CGEventRef clickDown = CGEventCreateMouseEvent(nullptr, down, pos, btn);
    CGEventRef clickUp   = CGEventCreateMouseEvent(nullptr, up, pos, btn);

    CGEventPost(kCGHIDEventTap, clickDown);
    CGEventPost(kCGHIDEventTap, clickUp);

    CFRelease(clickDown);
    CFRelease(clickUp);
}

static void moveMouseTo(double x, double y) {
    CGPoint p = CGPointMake(x, y);                 // screen coords: origin = top-left of main display
    CGEventRef move = CGEventCreateMouseEvent(nullptr, kCGEventMouseMoved, p, kCGMouseButtonLeft);
    CGEventPost(kCGHIDEventTap, move);
    CFRelease(move);
}

static void pressKey(CGKeyCode keycode) {
    CGEventRef keyDown = CGEventCreateKeyboardEvent(nullptr, keycode, true);
    CGEventRef keyUp   = CGEventCreateKeyboardEvent(nullptr, keycode, false);
    CGEventPost(kCGHIDEventTap, keyDown);
    CGEventPost(kCGHIDEventTap, keyUp);
    CFRelease(keyDown);
    CFRelease(keyUp);
}

static CGEventRef eventCallback(CGEventTapProxy, CGEventType type, CGEventRef e, void*) 
{
    CGKeyCode key = (CGKeyCode)CGEventGetIntegerValueField(e, kCGKeyboardEventKeycode);
    if (type == kCGEventTapDisabledByTimeout || type == kCGEventTapDisabledByUserInput) {
        return e;
    }

     if(type == kCGEventKeyDown && key == 25 && !clicker)
    {
        clicker = true;
        std::cout << "Autoclicker is on\n";
        // click 9 for the full function to work and to disable it.
        return e; //passes the event through
    }

    if(type == kCGEventKeyDown && key == 25 && clicker)
    {
        clicker = false;
        counter = 0;
        std::cout << "Autoclicker is off\n";
        return e;
    }

     if(clicker && counter <= 100)
    {
        //clickMouse(true); //right click
        clickMouse(false); //left click
        //pressKey(29); //press 0
        counter += 1;
        command = false;
        //moveMouseTo(1248.52, 194.191); //This will actually move your mouse which is bad
        std::this_thread::sleep_for(std::chrono::milliseconds(cperms)); // update every 100ms
    }

    if(counter >= 100)
    {
        clicker = false;
        counter = 0;
        std::cout << "Auto clicker is off\n";
    }



return e;

}
int main()
{
    std::cout << "press '0' to set the amount of clicks per time (Deafults is 1/s))\n";
    std::cout << "hint: lower the number for faster clicks.\n";
    std::cout << "Press '9' to toggle code on/off.\n";


   mask |= CGEventMaskBit(kCGEventKeyDown) |
    kCGEventMaskForAllEvents;

     g_eventTap = CGEventTapCreate(kCGSessionEventTap,
                                 kCGHeadInsertEventTap,
                                  kCGEventTapOptionDefault,
                                  mask,
                                  eventCallback,
                                  nullptr);
    if (!g_eventTap) return 1;

CFRunLoopSourceRef src = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, g_eventTap, 0);

// Add it to the current run loop
CFRunLoopAddSource(CFRunLoopGetCurrent(), src, kCFRunLoopCommonModes);

// Enable the tap
CGEventTapEnable(g_eventTap, true);

// Start the run loop — keeps your callback alive
CFRunLoopRun();

    return 0;
}
