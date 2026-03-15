#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <cstdlib>

#include "../util/files/FileUtils.h"

namespace fs = std::filesystem;

struct TestResult {
    std::string name;
    bool passed;
    std::string message;
};

std::string detectInterpreterExecutable(const fs::path& testsDir) {
#ifdef _WIN32
    const std::string exeName = "GulfOfMexicoInterpreter.exe";
#else
    const std::string exeName = "GulfOfMexicoInterpreter";
#endif

    fs::path candidate = testsDir / ".." / exeName;
    if (fs::exists(candidate)) {
        return candidate.string();
    }

    return exeName;
}

int runInterpreter(const std::string& interpreterExe,
                   const fs::path& inputPath,
                   const fs::path& outputPath) {
    std::string command;

#ifdef _WIN32
    // Paths in this project don't contain spaces, so we can avoid extra
    // quoting that sometimes confuses cmd.exe when invoked via system().
    command = interpreterExe + " " + inputPath.string() + " " + outputPath.string();
#else
    command = "'" + interpreterExe + "' '" + inputPath.string() + "' '" + outputPath.string() + "'";
#endif

    std::cout << "Running: " << command << '\n';
    return std::system(command.c_str());
}

TestResult runSingleTest(const fs::path& testDir, const std::string& interpreterExe) {
    const fs::path inputPath = testDir / "input.gom";
    const fs::path expectedPath = testDir / "expected.gom";
    const fs::path actualPath = testDir / "actual.gom";

    TestResult result;
    result.name = testDir.filename().string();
    result.passed = false;

    if (!fs::exists(inputPath) || !fs::exists(expectedPath)) {
        result.message = "Missing input.gom or expected.gom";
        return result;
    }

    const int exitCode = runInterpreter(interpreterExe, inputPath, actualPath);
    if (exitCode != 0) {
        result.message = "Interpreter exited with code " + std::to_string(exitCode);
        return result;
    }

    try {
        const std::string expected = readFileToString(expectedPath.string());
        const std::string actual = readFileToString(actualPath.string());

        if (actual == expected) {
            result.passed = true;
            result.message = "OK";
        } else {
            result.message = "Output differed from expected";
        }
    } catch (const std::exception& ex) {
        result.message = ex.what();
    }

    return result;
}

int main() {
    const fs::path testsDir = fs::canonical(fs::path(__FILE__)).parent_path();
    const std::string interpreterExe = detectInterpreterExecutable(testsDir);

    if (!fs::exists(testsDir) || !fs::is_directory(testsDir)) {
        std::cerr << "Could not find tests directory at: " << testsDir << '\n';
        return 1;
    }

    std::vector<TestResult> results;

    for (const auto& entry : fs::directory_iterator(testsDir)) {
        if (!entry.is_directory()) {
            continue;
        }

        const fs::path testDir = entry.path();
        const fs::path inputPath = testDir / "input.gom";
        const fs::path expectedPath = testDir / "expected.gom";

        if (!fs::exists(inputPath) || !fs::exists(expectedPath)) {
            continue;
        }

        results.push_back(runSingleTest(testDir, interpreterExe));
    }

    if (results.empty()) {
        std::cout << "No tests found.\n";
        return 0;
    }

    int passed = 0;
    int failed = 0;

    for (const auto& r : results) {
        if (r.passed) {
            std::cout << "[PASS] " << r.name << " - " << r.message << '\n';
            ++passed;
        } else {
            std::cout << "[FAIL] " << r.name << " - " << r.message << '\n';
            ++failed;
        }
    }

    std::cout << "\nSummary: " << passed << " passed, " << failed << " failed.\n";

    return failed == 0 ? 0 : 1;
}

