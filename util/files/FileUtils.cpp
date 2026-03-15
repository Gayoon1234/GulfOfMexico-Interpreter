#include "FileUtils.h"

#include <fstream>
#include <stdexcept>

std::string readFileToString(const std::string& path) {
    std::ifstream in(path, std::ios::in | std::ios::binary);
    if (!in) {
        throw std::runtime_error("Error: could not open file '" + path + "' for reading.");
    }

    std::string contents;
    in.seekg(0, std::ios::end);
    contents.resize(static_cast<std::size_t>(in.tellg()));
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], static_cast<std::streamsize>(contents.size()));

    if (!in && !in.eof()) {
        throw std::runtime_error("Error while reading file '" + path + "'.");
    }

    return contents;
}

void writeStringToFile(const std::string& path, const std::string& contents) {
    std::ofstream out(path, std::ios::out | std::ios::binary | std::ios::trunc);
    if (!out) {
        throw std::runtime_error("Error: could not open file '" + path + "' for writing.");
    }

    out.write(contents.data(), static_cast<std::streamsize>(contents.size()));

    if (!out) {
        throw std::runtime_error("Error while writing file '" + path + "'.");
    }
}

