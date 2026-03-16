#include "Transformations.h"

#include <sstream>
#include <functional>
#include <cctype>

std::string applyTransformations(const std::string& input) {
    // As new transformations are added, call them here in sequence.
    return
        addDebug(
            removeBrackets(
                swapSemiColonsAndExclamationMarks(
                    functionKeyword(
                        input
                    )
                )
            )
        );
}


/*
 * Generic helper:
 * Reads the input line-by-line and applies a transformation to each line.
 */
std::string transformLines(
    const std::string& input,
    const std::function<std::string(const std::string&)>& lineTransform
) {
    std::stringstream ss(input);
    std::string line;
    std::string result;

    bool first = true;

    while (std::getline(ss, line)) {
        if (!first) {
            result.push_back('\n');
        }
        first = false;

        result += lineTransform(line);
    }

    return result;
}


std::string removeBrackets(const std::string& input) {
    std::string result = input;
    for (char& ch : result) {
        if (ch == '(' || ch == ')') {
            ch = ' ';
        }
    }
    return result;
}


std::string swapSemiColonsAndExclamationMarks(const std::string& input) {
    std::string result;
    result.reserve(input.size());

    std::size_t pendingExcls = 0;

    for (char ch : input) {
        if (ch == '!') {
            // Might be part of a trailing run; delay emitting until we know.
            ++pendingExcls;
            continue;
        }

        if (ch == ';') {
            // Any pending '!' before a ';' are not trailing, so emit them as '!'.
            while (pendingExcls > 0) {
                result.push_back('!');
                --pendingExcls;
            }

            // Transform ';' -> '!'
            result.push_back('!');
            continue;
        }

        if (ch == '\n' || ch == '\r') {
            // End of line: collapse trailing '!' into a single ';'
            if (pendingExcls > 0) {
                result.push_back(';');
                pendingExcls = 0;
            }

            result.push_back(ch);
            continue;
        }

        // Regular character
        while (pendingExcls > 0) {
            result.push_back('!');
            --pendingExcls;
        }

        result.push_back(ch);
    }

    // End of input: trailing '!' become ';'
    if (pendingExcls > 0) {
        result.push_back(';');
    }

    return result;
}


bool isFunctionLike(const std::string& input) {
    std::string target = "function";
    size_t j = 0;

    for (char c : input) {
        while (j < target.size() && target[j] != c) {
            j++;
        }

        if (j == target.size()) {
            return false;
        }

        j++;
    }

    return true;
}


/*
 * Line-level transformation for the function keyword rule.
 */
std::string functionKeywordLine(const std::string& input) {
    size_t start = 0;
    while (start < input.size() && std::isspace(static_cast<unsigned char>(input[start]))) {
        start++;
    }

    size_t end = start;
    while (end < input.size() && std::isalpha(static_cast<unsigned char>(input[end]))) {
        end++;
    }

    std::string firstWord = input.substr(start, end - start);

    if (!firstWord.empty() && isFunctionLike(firstWord)) {
        firstWord = "function";
    }

    return input.substr(0, start) + firstWord + input.substr(end);
}


std::string functionKeyword(const std::string& input) {
    return transformLines(input, functionKeywordLine);
}

std::string addDebugLine(const std::string& input) {
    if (input.empty()) {
        return input;
    }

    std::string result = input;

    // Find last non-newline character
    size_t pos = result.find_last_not_of("\r\n");
    if (pos == std::string::npos) {
        return result;
    }

    if (result[pos] == '?') {
        result[pos] = '!';             // replace ? with !
        result.insert(0, "debug ");    // add debug prefix
    }

    return result;
}

std::string addDebug(const std::string& input) {
    return transformLines(input, addDebugLine);
}