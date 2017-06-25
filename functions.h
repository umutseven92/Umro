//
// Created by Umut Seven on 25.06.2017.
//

#ifndef UMRO_FUNCTİONS_H
#define UMRO_FUNCTİONS_H

#include <windows.h>

void HideWindow();

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

void DebugPrint(char *message);

#endif //UMRO_FUNCTİONS_H
