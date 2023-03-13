#ifndef FORMATTER_H_
#define FORMATTER_H_

#include <ctype.h>
#include <stdio.h>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

class Formatter {
   public:
    static std::string filterNumber(const std::string &s) {
        std::string number;
        for (const char c : s)
            if ((c >= '0' && c <= '9') || c == '.')
                number += c;
        return number;
    }

    static std::string filterString(const std::string &s) {
        std::string number;
        for (const char c : s)
            if ((c >= '0' && c <= '9') || c == ',')
                number += c;
        return number;
    }

    static std::string filterWordString(const std::string &s) {
        std::string value;

        int counter = 0;
        for (const char c : s) {
            if(c == '"') {
                counter++;
            }

            if(counter >= 3) {
                if ((isAlpha(c) || isDigit(c)) && c != '"')
                    value += c;
            }
        }

        return value;
    }

    static float getFloat(const std::string &n) {
        float x;
        try {
            x = std::stof(n);
            return x;
        } catch (std::exception e) {
            return -999;
        }
    }

    static int getInt(const std::string &n) {
        int x = -1;
        try {
            x = stoi(n);
            return x;
        } catch (std::exception e) {
            return -999;
        }
    }

    static int filterInt(const std::string &str, const std::string &type) {
        std::string::size_type pos = 0;
        std::string::size_type prev = 0;
        while ((pos = str.find('\n', prev)) != std::string::npos) {
            std::string subString = str.substr(prev, pos - prev);
            if (subString.find(type) != std::string::npos) {
                int n = getInt(filterNumber(subString));
                return n;
            }
            prev = pos + 1;
        }
        return -1;
    }

    static std::string filterString(const std::string &str, const std::string &type) {
        std::string::size_type pos = 0;
        std::string::size_type prev = 0;
        while ((pos = str.find('\n', prev)) != std::string::npos) {
            std::string subString = str.substr(prev, pos - prev);
            if (subString.find("stringValue") != std::string::npos) {
                return Formatter::filterString(subString);
            }
            prev = pos + 1;
        }
        return "";
    }

    static std::string filterSingleString(const std::string &str) {
        std::string::size_type pos = 0;
        std::string::size_type prev = 0;
        while ((pos = str.find('\n', prev)) != std::string::npos) {
            std::string subString = str.substr(prev, pos - prev);
            if (subString.find("stringValue") != std::string::npos) {
                return Formatter::filterWordString(subString);
            }
            prev = pos + 1;
        }
        return "";
    }

    static float filterFloat(const std::string &str, const std::string &type) {
        std::string::size_type pos = 0;
        std::string::size_type prev = 0;
        while ((pos = str.find('\n', prev)) != std::string::npos) {
            std::string subString = str.substr(prev, pos - prev);
            if (subString.find(type) != std::string::npos) {
                float n = getFloat(filterNumber(subString));
                return n;
            }
            prev = pos + 1;
        }
        return -1.0f;
    }
};

#endif
