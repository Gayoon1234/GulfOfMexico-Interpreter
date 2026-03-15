#pragma once

#include <string>

// Applies a set of text transformations to the input.
// Currently: replaces '(' and ')' with single spaces.
std::string applyTransformations(const std::string& input);

// Removes bracket characters like '(' and ')' from the input, replacing them with spaces.
std::string removeBrackets(const std::string& input);

// ; -> ! and ! -> ;
std::string swapSemiColonsAndExclamationMarks(const std::string& input);

