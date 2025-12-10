#pragma once
#include <iostream>
#include <string>

// --- CORRECCIÓN: DETECTAR SISTEMA OPERATIVO ---
#ifdef _WIN32
#include <windows.h> // Solo importamos esto si estamos en Windows
#else
#include <unistd.h> // Librería estándar de Linux (opcional)
#endif

// Función para limpiar consola (Compatible con ambos)
inline void clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    // En Linux/Mac usamos clear o códigos ANSI
    std::cout << "\033[2J\033[1;1H";
#endif
}

// Función para pausar (Compatible con ambos)
inline void pauseConsole() {
#ifdef _WIN32
    system("pause");
#else
    std::cout << "Presione Enter para continuar...";
    std::cin.ignore();
    std::cin.get();
#endif
}

// Función para colores (En Linux es nativo, en Windows requiere setup)
inline void activateVirtualTerminal() {
#ifdef _WIN32
    // Código específico de Windows para habilitar colores ANSI
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;
    dwMode |= 0x0004; // ENABLE_VIRTUAL_TERMINAL_PROCESSING
    SetConsoleMode(hOut, dwMode);
#endif
    // En Linux no hace falta hacer nada, los colores funcionan solos.
}