#ifndef COMMON_STRUTIL_INCLUDE_STRING_KEY_COMPARABLE_H_
#define COMMON_STRUTIL_INCLUDE_STRING_KEY_COMPARABLE_H_

#include <string>

namespace strutil {

class StringKeyComparable {
public:
    virtual ~StringKeyComparable();

    bool operator<=(const StringKeyComparable& other) const;
    bool operator>=(const StringKeyComparable& other) const;
    bool operator<(const StringKeyComparable& other) const;
    bool operator>(const StringKeyComparable& other) const;
    bool operator==(const StringKeyComparable& other) const;
    bool operator!=(const StringKeyComparable& other) const;

    bool operator<=(const std::string& other) const;
    bool operator>=(const std::string& other) const;
    bool operator<(const std::string& other) const;
    bool operator>(const std::string& other) const;
    bool operator==(const std::string& other) const;
    bool operator!=(const std::string& other) const;

    friend bool operator<=(const std::string& lhs, const StringKeyComparable& other);
    friend bool operator>=(const std::string& lhs, const StringKeyComparable& other);
    friend bool operator<(const std::string& lhs, const StringKeyComparable& other);
    friend bool operator>(const std::string& lhs, const StringKeyComparable& other);
    friend bool operator==(const std::string& lhs, const StringKeyComparable& other);
    friend bool operator!=(const std::string& lhs, const StringKeyComparable& other);

private:
    virtual std::string key() const = 0;
};

}

#endif
