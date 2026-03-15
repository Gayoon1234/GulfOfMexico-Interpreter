#pragma once

#include <string>

// Reads the entire contents of a file into a string.
// Throws std::runtime_error on failure.
std::string readFileToString(const std::string& path);

// Writes the given string to a file, overwriting any existing contents.
// Throws std::runtime_error on failure.
void writeStringToFile(const std::string& path, const std::string& contents);

