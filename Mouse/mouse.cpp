#include <iostream>
#include <ApplicationServices/ApplicationServices.h>
#include <CoreFoundation/CoreFoundation.h>
#include <cstdlib>
#include <string>
#include <cmath>

// 🔧 Change this to whatever you want to open
static const char* kPathToOpen = "/Users/gabrielcallauprellwitz/Desktop/Screenshot 2025-09-25 at 10.08.05 AM.png";

// If you want a side button instead, set to -1 and use btn==N below.
bool kUseRightClick = true;  // true = right click, false = side button
bool eKey = false;
static const int  kSideButtonNum = 3;     // common side buttons: 3 or 4
double Lcounter = -1.0;
const double kWindowSec = 0.30;
CGEventMask mask = 0;

static CFMachPortRef g_eventTap = nullptr;

static void openTarget() {
    std::string cmd = std::string("open '") + kPathToOpen + "' &";
    std::system(cmd.c_str());
}

static void pressKey(CGKeyCode keycode) {
    CGEventRef keyDown = CGEventCreateKeyboardEvent(nullptr, keycode, true);
    CGEventRef keyUp   = CGEventCreateKeyboardEvent(nullptr, keycode, false);
    CGEventPost(kCGHIDEventTap, keyDown);
    CGEventPost(kCGHIDEventTap, keyUp);
    CFRelease(keyDown);
    CFRelease(keyUp);
}

static CGEventRef eventCallback(CGEventTapProxy, CGEventType type, CGEventRef e, void*) {
    CGKeyCode key = (CGKeyCode)CGEventGetIntegerValueField(e, kCGKeyboardEventKeycode);
    if (type == kCGEventTapDisabledByTimeout || type == kCGEventTapDisabledByUserInput) {
        if (g_eventTap) CGEventTapEnable(g_eventTap, true);
        return e;
    }
    // Require Shift to be held so we don't kill normal right-click behavior
    CGEventFlags flags = CGEventGetFlags(e);
    //bool shiftHeld = (flags & kCGEventFlagMaskShift) != 0;
   // if (!shiftHeld) return e;

    if(type == kCGEventKeyDown && key == 29 && !eKey)
    {
        eKey = true;
        std::cout << "The full function is now working.\n";
        return e;
        // click 0 for the full function to work and to disable it.
    }

    if(type == kCGEventKeyDown && key == 29 && eKey)
    {
        eKey = false;
        std::cout << "The full function ceased.\n";
        return e;
    }

    if(eKey)
    {
    if(type == kCGEventLeftMouseDown)
    {
        Lcounter = CFAbsoluteTimeGetCurrent();

    }

    if (type == kCGEventRightMouseDown) {
        std::cout << "RIGHT down\n";
        double now = CFAbsoluteTimeGetCurrent();
        bool combo = (Lcounter > 0.0) && ((now - Lcounter) <= kWindowSec);
        if(combo)
        {
            std::cout << "You did the left-right click combo!\n";
            openTarget();
            Lcounter = -1.0;
            // return nullptr; // uncomment to CONSUME the right-click
            return e; // pass through (show context menu too)
        }
        return e;
    }

    if (type == kCGEventOtherMouseDown) {
        int64_t btn = CGEventGetIntegerValueField(e, kCGMouseEventButtonNumber);
        if (btn == kSideButtonNum) {
            std::system((std::string("open '") + kPathToOpen + "' &").c_str());
            return e; // consume (optional)
        }
    }
}  
    return e;
}

int main() {
    std::cout << "Event tap starting...\n";
   
    mask |= CGEventMaskBit(kCGEventKeyDown) | 
    CGEventMaskBit(kCGEventLeftMouseDown)  |
    CGEventMaskBit(kCGEventRightMouseDown) |
    CGEventMaskBit(kCGEventOtherMouseDown); //This is kind of a library of events where you just put all of your events down in here. And then call it again using mask. 

    g_eventTap = CGEventTapCreate(kCGSessionEventTap,
                                  kCGHeadInsertEventTap,
                                  kCGEventTapOptionDefault,
                                  mask,
                                  eventCallback,
                                  nullptr);
    if (!g_eventTap) return 1;

    CFRunLoopSourceRef src = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, g_eventTap, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), src, kCFRunLoopCommonModes);
    CGEventTapEnable(g_eventTap, true);
    CFRunLoopRun();
    return 0;
}
