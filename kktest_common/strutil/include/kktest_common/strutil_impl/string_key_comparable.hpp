#ifndef KKTEST_COMMON_STRUTIL_INCLUDE_KKTEST_COMMON_STRUTIL_IMPL_STRING_KEY_COMPARABLE_HPP_
#define KKTEST_COMMON_STRUTIL_INCLUDE_KKTEST_COMMON_STRUTIL_IMPL_STRING_KEY_COMPARABLE_HPP_

#include <string>

namespace kktest {
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

}  // namespace strutil
}  // namespace kktest

#endif  // KKTEST_COMMON_STRUTIL_INCLUDE_KKTEST_COMMON_STRUTIL_IMPL_STRING_KEY_COMPARABLE_HPP_
