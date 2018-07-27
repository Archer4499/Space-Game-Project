#include "stringUtils.h"


void eatSpaces(const std::string &str, size_t &i, bool newLines) {
    if (newLines) {
        i = str.find_first_not_of(" \t\v\f\r", i);
    } else {
        i = str.find_first_not_of(" \t\v\n\f\r", i);
    }
    if (i == std::string::npos) {
        i = str.length();
    }
}


std::string stringUntilSpace(const std::string &str, size_t &i, const bool shouldEatSpaces) {
    std::string res;
    size_t start = i, end = 0;

    eatSpaces(str, start);

    if ((end = str.find_first_of(" \t\v\n\f\r", start)) != std::string::npos) {
        res = str.substr(start, end - start);
    } else {
        res = str.substr(start);
    }

    i = start + res.length();
    if (shouldEatSpaces) eatSpaces(str, i);

    return res;
}


bool skipComments(const std::string &str, size_t &i) {
    size_t pos = i;
    eatSpaces(str, pos);
    if (pos < str.length() && str[pos] == '#') {
        while (pos < str.length()) {
            if (str[pos] == '\n') {
                ++pos;
                break;
            }
            ++pos;
        }
        i = pos;
        return true;
    }
    return false;
}
