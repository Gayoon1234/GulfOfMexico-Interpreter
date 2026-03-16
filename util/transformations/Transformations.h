#pragma once

#include <string>
#include <functional>

// Applies all configured transformations in sequence.
std::string applyTransformations(const std::string& input);

// Utility: applies a transformation to each line of the input.
std::string transformLines(
    const std::string& input,
    const std::function<std::string(const std::string&)>& lineTransform
);

// Removes bracket characters like '(' and ')' from the input, replacing them with spaces.
std::string removeBrackets(const std::string& input);

// ; -> ! and trailing ! -> ;
std::string swapSemiColonsAndExclamationMarks(const std::string& input);

// Normalizes the first word of each line to "function" if it resembles "function".
std::string functionKeyword(const std::string& input);

//add debug prefix to lines ending in ?
std::string addDebug(const std::string& input);