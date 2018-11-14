#include "string_key_comparable.hpp"

using namespace std;

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
