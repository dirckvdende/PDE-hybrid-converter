
#include "util.h"
#include <string>

std::string stripWhitespace(const std::string &txt) {
    size_t start = 0, end = txt.size();
    while (start < end && txt[start] == ' ')
        start++;
    while (start < end && txt[end - 1] == ' ')
        end--;
    return txt.substr(start, end - start);
}

std::vector<std::string> splitString(const std::string &txt) {
    size_t start = 0, end = txt.find(',');
    std::vector<std::string> out;
    while (start < txt.size()) {
        out.push_back(stripWhitespace(txt.substr(start, end - start)));
        start = end == std::string::npos ? end : end + 1;
        if (start < txt.size())
            end = txt.find(',', start);
    }
    return out;
}