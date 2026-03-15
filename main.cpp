#include <iostream>
#include <string>
#include <stdexcept>

#include "util/files/FileUtils.h"
#include "util/transformations/Transformations.h"

// Usage: GulfOfMexicoInterpreter <input-file> <output-file>
int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input-file> <output-file>\n";
        return 1;
    }

    const std::string inputPath = argv[1];
    const std::string outputPath = argv[2];

    try {
        const std::string input = readFileToString(inputPath);
        const std::string output = applyTransformations(input);
        writeStringToFile(outputPath, output);
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << '\n';
        return 1;
    }

    return 0;
}
