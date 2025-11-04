#include <ApplicationServices/ApplicationServices.h>
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    while (true) {
        CGEventRef event = CGEventCreate(nullptr);
        CGPoint cursor = CGEventGetLocation(event);
        CFRelease(event);

        std::cout << "Mouse position: X=" << cursor.x << ", Y=" << cursor.y << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // update every 100ms
    }

    return 0;
}
