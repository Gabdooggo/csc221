#include <iostream>
#include <ApplicationServices/ApplicationServices.h>
#include <CoreFoundation/CoreFoundation.h>
#include <cstdlib>
#include <string>

// 🔧 Change this to whatever you want to open
static const char* kPathToOpen = "/Users/gabrielcallauprellwitz/Desktop/Screenshot 2025-09-25 at 10.08.05 AM.png";

// If you want a side button instead, set to -1 and use btn==N below.
bool kUseRightClick = true;  // true = right click, false = side button
static const int  kSideButtonNum = 3;     // common side buttons: 3 or 4
double Lcounter = -1.0;
const double kWindowSec = 0.30;

static CFMachPortRef g_eventTap = nullptr;

static void openTarget() {
    std::string cmd = std::string("open '") + kPathToOpen + "' &";
    std::system(cmd.c_str());
}

static CGEventRef eventCallback(CGEventTapProxy, CGEventType type, CGEventRef e, void*) {
    if (type == kCGEventTapDisabledByTimeout || type == kCGEventTapDisabledByUserInput) {
        if (g_eventTap) CGEventTapEnable(g_eventTap, true);
        return e;
    }
    if (type != kCGEventLeftMouseDown && type != kCGEventRightMouseDown && type != kCGEventOtherMouseDown) return e;

    // Require Shift to be held so we don't kill normal right-click behavior
    CGEventFlags flags = CGEventGetFlags(e);
    //bool shiftHeld = (flags & kCGEventFlagMaskShift) != 0;
   // if (!shiftHeld) return e;

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
            return e; // consume the click (optional)
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
    return e;
}

int main() {
    std::cout << "Event tap starting...\n";
    CGEventMask mask = 
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
