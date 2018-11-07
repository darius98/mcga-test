#ifndef KKTEST_RUNNER_UTILS_STRING_H_
#define KKTEST_RUNNER_UTILS_STRING_H_

#include <string>


namespace kktest_runner {

std::string toUpper(const std::string& str);

std::string toLower(const std::string& str);

bool startsWith(const std::string& str, const std::string& prefix);

bool startsWith(const std::string& str, char ch);

bool endsWith(const std::string& str, const std::string& suffix);

bool endsWith(const std::string& str, char ch);

bool containsSubstring(const std::string& str, const std::string& substr);

bool containsCharacter(const std::string& str, char ch);

std::string stripTrailingWhitespace(std::string str);

std::string strip(std::string str);

}


#endif
