#pragma once

#include <string>

// Starts at i and sets i to the index of the next non-white-space(Except \n) character in str after the index i
void eatSpaces(const std::string &str, size_t &i, bool newLines = false);

// Returns the substring until the next white-space char, stops before \n chars even with shouldEatSpaces
std::string stringUntilSpace(const std::string &str, size_t &i, const bool shouldEatSpaces = true);

// Starts at i and sets i to the index of the char after the next \n
//   if there is a comment before any other text otherwise doesn't change i
// Returns true if a comment was found
bool skipComments(const std::string &str, size_t &i);
