#include <cstdio>

#include "pbge/core/Log.h"

void LogTerminal::write(std::string message){
    puts(message.c_str());
}

void LogTerminal::writeError(std::string message) {
    fprintf(stderr, "%s\n", message.c_str());
}