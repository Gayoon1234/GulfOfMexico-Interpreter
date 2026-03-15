#include "Transformations.h"

std::string applyTransformations(const std::string& input) {
    // As new transformations are added, call them here in sequence.
    return 
        removeBrackets(
        swapSemiColonsAndExclamationMarks(
        functionKeyword(
                input
            )
        )
    );
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
            // Then transform ';' -> '!'.
            result.push_back('!');
            continue;
        }

        if (ch == '\n' || ch == '\r') {
            // End of line: collapse any trailing '!' on this line into a single ';'.
            if (pendingExcls > 0) {
                result.push_back(';');
                pendingExcls = 0;
            }
            result.push_back(ch);
            continue;
        }

        // Regular character: any pending '!' are not trailing, so emit them as '!'.
        while (pendingExcls > 0) {
            result.push_back('!');
            --pendingExcls;
        }
        result.push_back(ch);
    }

    // End of input: any remaining '!' are trailing in the final line.
    if (pendingExcls > 0) {
        result.push_back(';');
    }

    return result;
}



bool isFunctionLike(const std::string& input) {
    std::string target = "function";
    size_t j = 0;
    for (char c : input) {
        while (j < target.size() && target[j] != c) j++;
        if (j == target.size()) return false;
        j++;
    }
    return true;
}


std::string functionKeyword(const std::string& input) {
    // Find first word: letters only
    size_t start = 0;
    while (start < input.size() && std::isspace(input[start])) start++;

    size_t end = start;
    while (end < input.size() && std::isalpha(input[end])) end++;

    std::string firstWord = input.substr(start, end - start);

    // Transform if function-like
    if (!firstWord.empty() && isFunctionLike(firstWord)) {
        firstWord = "function";
    }

    // Rebuild the line: keep everything after the letters as-is
    return input.substr(0, start) + firstWord + input.substr(end);
}