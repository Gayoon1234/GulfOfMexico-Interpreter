#include "Transformations.h"

std::string applyTransformations(const std::string& input) {
    // As new transformations are added, call them here in sequence.
    return 
        removeBrackets(
        swapSemiColonsAndExclamationMarks(
            input
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

