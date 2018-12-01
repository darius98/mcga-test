#include <strutil_impl/string_key_comparable.hpp>

using std::string;

namespace strutil {

StringKeyComparable::~StringKeyComparable() = default;

bool StringKeyComparable::operator<=(const StringKeyComparable& other) const {
    return key() <= other.key();
}
bool StringKeyComparable::operator>=(const StringKeyComparable& other) const {
    return key() >= other.key();
}
bool StringKeyComparable::operator<(const StringKeyComparable& other) const {
    return key() < other.key();
}
bool StringKeyComparable::operator>(const StringKeyComparable& other) const {
    return key() > other.key();
}
bool StringKeyComparable::operator==(const StringKeyComparable& other) const {
    return key() == other.key();
}
bool StringKeyComparable::operator!=(const StringKeyComparable& other) const {
    return key() != other.key();
}

bool StringKeyComparable::operator<=(const string& other) const {
    return key() <= other;
}
bool StringKeyComparable::operator>=(const string& other) const {
    return key() >= other;
}
bool StringKeyComparable::operator<(const string& other) const {
    return key() < other;
}
bool StringKeyComparable::operator>(const string& other) const {
    return key() > other;
}
bool StringKeyComparable::operator==(const string& other) const {
    return key() == other;
}
bool StringKeyComparable::operator!=(const string& other) const {
    return key() != other;
}

bool operator<=(const string& lhs, const StringKeyComparable& other) {
    return lhs <= other.key();
}
bool operator>=(const string& lhs, const StringKeyComparable& other) {
    return lhs >= other.key();
}
bool operator<(const string& lhs, const StringKeyComparable& other) {
    return lhs < other.key();
}
bool operator>(const string& lhs, const StringKeyComparable& other) {
    return lhs > other.key();
}
bool operator==(const string& lhs, const StringKeyComparable& other) {
    return lhs == other.key();
}
bool operator!=(const string& lhs, const StringKeyComparable& other) {
    return lhs != other.key();
}

}
