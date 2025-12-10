#pragma once
#include <iostream>
#include <limits>   
#include <cstdlib>  
#define NOMINMAX
#include <windows.h>  
#include "Colors.h"

void activateVirtualTerminal() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

void clearConsole() {
    std::system("cls");
}

void pauseConsole() {
    std::cout << "\n\n" << Color::YELLOW
        << "Presione Enter para continuar..." << Color::RESET;
    std::cin.clear(); 
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}