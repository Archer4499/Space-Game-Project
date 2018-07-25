#pragma once

#include <string>

// Returns the index of the next non-white-space(Except \n) character in str after the index i
int skipWhiteSpace(std::string str, int i) {
    while (str[i] == ' '  || str[i] == '\t' ||
           str[i] == '\v' || str[i] == 'f'  ||
           str[i] == '\r')
        ++i;
    return i;
}
