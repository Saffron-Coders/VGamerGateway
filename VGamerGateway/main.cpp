// VGamer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <Windows.h>
#include <WinUser.h>
#include "NetworkInputManager.h"
#include "EventGenerator.h"
#include "InputDecoder.h"

int main()
{
    printf("Starting VGamer...\n");
    fflush(stdout);

    int ret = 0;
    ret = NetInMgr.start();
    if (ret < 0) {
        fprintf(stderr, "Error(%d): NetworkInputManager failed to start.\n", ret);
        return EXIT_FAILURE;
    }
    printf("[Success]: Network input manager initialized.\n");
    fflush(stdout);
    while (true);

#if 0
    // This structure will be used to create the keyboard
    // input event.
    INPUT ip;

    // Pause for 5 seconds.
    Sleep(5000);

    // Set up a generic keyboard event.
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0; // hardware scan code for key
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;

    // Press the "A" key
    ip.ki.wVk = 0x41; // virtual-key code for the "a" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "A" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));
#endif
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
