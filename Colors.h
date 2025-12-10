#pragma once
#include <string>

namespace Color {
    // Colores de Texto
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string BLUE = "\033[34m";
    const std::string CYAN = "\033[36m";
    const std::string WHITE = "\033[37m";
	const std::string MAGENTA = "\033[95m";

    // Formato
    const std::string RESET = "\033[0m"; // Resetea todo
    const std::string BOLD = "\033[1m";
    const std::string UNDERLINE = "\033[4m";
}