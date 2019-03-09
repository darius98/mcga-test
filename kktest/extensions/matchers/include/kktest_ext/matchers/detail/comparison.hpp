#pragma once

#include <functional>
#include <string>

#include <kktest_ext/matchers/matcher.hpp>

namespace kktest::matchers::detail {

template<class T>
class EqualityMatcher: public StatelessMatcher {
 public:
    explicit constexpr EqualityMatcher(const T& target): target(target) {}

    template<class O>
    bool matches(const O& obj) const {
        return obj == target;
    }

    void describe(Description* description) const {
        (*description) << "'" << target << "'";
    }

    void describeFailure(Description* description) const {
        (*description) << "not '" << target << "'";
    }

 private:
    const T& target;
};

template<class T>
class NonEqualityMatcher: public StatelessMatcher {
 public:
    explicit constexpr NonEqualityMatcher(const T& target): target(target) {}

    template<class O>
    bool matches(const O& obj) const {
        return obj != target;
    }

    void describe(Description* description) const {
        (*description) << "not '" << target << "'";
    }

    void describeFailure(Description* description) const {
        (*description) << "'" << target << "'";
    }

 private:
    const T& target;
};

template<class T>
class IsLessThanMatcher: public StatelessMatcher {
 public:
    explicit constexpr IsLessThanMatcher(const T& target): target(target) {}

    template<class O>
    bool matches(const O& object) const {
        return object < target;
    }

    void describe(Description* description) const {
        (*description) << "< '" << target << "'";
    }

    void describeFailure(Description* description) const {
        (*description) << ">= '" << target << "'";
    }

 private:
    const T& target;
};

template<class T>
class IsLessThanEqualMatcher: public StatelessMatcher {
 public:
    explicit constexpr IsLessThanEqualMatcher(const T& target):
            target(target) {}

    template<class O>
    bool matches(const O& object) const {
        return object <= target;
    }

    void describe(Description* description) const {
        (*description) << "<= '" << target << "'";
    }

    void describeFailure(Description* description) const {
        (*description) << "> '" << target << "'";
    }

 private:
    const T& target;
};

template<class T>
class IsGreaterThanMatcher: public StatelessMatcher {
 public:
    explicit constexpr IsGreaterThanMatcher(const T& target): target(target) {}

    template<class O>
    bool matches(const O& object) const {
        return object > target;
    }

    void describe(Description* description) const {
        (*description) << "> '" << target << "'";
    }

    void describeFailure(Description* description) const {
        (*description) << "<= '" << target << "'";
    }

 private:
    const T& target;
};

template<class T>
class IsGreaterThanEqualMatcher: public StatelessMatcher {
 public:
    explicit constexpr IsGreaterThanEqualMatcher(const T& target):
            target(target) {}

    template<class O>
    bool matches(const O& object) const {
        return object >= target;
    }

    void describe(Description* description) const {
        (*description) << ">= '" << target << "'";
    }

    void describeFailure(Description* description) const {
        (*description) << "< '" << target << "'";
    }

 private:
    const T& target;
};

template<class T>
class IdentityMatcher: public StatefulMatcher<const void*> {
 public:
    explicit constexpr IdentityMatcher(const T& target):
            address(static_cast<const void*>(&target)) {}
    IdentityMatcher(const IdentityMatcher& other) = default;

    template<class S>
    bool matches(const S& object, const void** state) const {
        *state = &object;
        return &object == address;
    }

    void describe(Description* description) const {
        (*description) << "variable at address '" << address << "'";
    }

    void describeFailure(Description* description,
                         const void** state) const {
        (*description) << "variable at address '" << *state << "'";
    }

 private:
    const void* address;
};

constexpr const std::size_t relevantRange = 20;

template<>
class EqualityMatcher<std::string>: public StatefulMatcher<std::string> {
 public:
    explicit EqualityMatcher(std::string target) : target(std::move(target)) {}

    bool matches(const std::string& obj, std::string* state) const {
        *state = obj;
        return obj == target;
    }

    bool matches(const char* obj, std::string* state) const {
        *state = obj;
        return obj == target;
    }

    void describe(Description* description) const {
        (*description) << "'" << target << "'";
    }

    void describeFailure(Description* description, std::string* state) const {
        const std::string& obj = *state;
        std::size_t mismatchIndex = 0;
        while (mismatchIndex < std::min(target.size(), obj.size())
               && target[mismatchIndex] == obj[mismatchIndex]) {
            mismatchIndex += 1;
        }

        // number of spaces we must place the mismatch caret to the right
        // the initial '11' represents the length of
        // "Expected: '" / "     Got: '"
        std::size_t numSpaces = 11u + std::min(mismatchIndex, relevantRange);
        if (mismatchIndex > relevantRange) {
            numSpaces += 3;
        }
        // TODO(darius98): There is a problem with the design here: we need to
        //             count how many characters will be changed by the Streamer
        //             implementation to be represented here by two characters
        //             (so we know where to position the mismatch caret).
        //             This class should not be inter-dependent with the
        //             implementation of Streamer.
        for (std::size_t index = mismatchIndex >= relevantRange
                                 ? mismatchIndex - relevantRange
                                 : 0;
             index < mismatchIndex;
             ++ index) {
            if (target[index] == '\n'
                || target[index] == '\r'
                || target[index] == '\t') {
                numSpaces += 1;
            }
        }

        (*description) << "different at index " << mismatchIndex << ":";
        description->appendRawString("\n\tExpected: '");
        (*description) << extractRelevantSection(target, mismatchIndex);
        description->appendRawString("'\n\t     Got: '");
        (*description) << extractRelevantSection(obj, mismatchIndex);
        description->appendRawString(
                "'\n\t" + std::string(numSpaces, ' ') + "^");
    }

    template<int n>
    operator EqualityMatcher<char[n]>() const; // NOLINT

 protected:
    std::string target;

 private:
    static std::string extractRelevantSection(const std::string& s,
                                              const std::size_t index) {
        auto firstIndex = index >= relevantRange ? index - relevantRange : 0;
        auto lastIndex = std::min(index + relevantRange, s.size() - 1);
        auto relevantSection = s.substr(firstIndex, lastIndex - firstIndex + 1);

        if (index + relevantRange + 1 < s.size()) {
            relevantSection += "...";
        }
        if (index > relevantRange) {
            relevantSection = "..." + relevantSection;
        }
        return relevantSection;
    }
};

template<int n>
class EqualityMatcher<char[n]>: public EqualityMatcher<std::string> {
 public:
    using EqualityMatcher<std::string>::EqualityMatcher;
};

template<int n>
EqualityMatcher<std::string>::operator EqualityMatcher<char[n]>() const {
    return EqualityMatcher<char[n]>(target);
}

template<>
class NonEqualityMatcher<std::string>: public StatelessMatcher {
 public:
    explicit NonEqualityMatcher(std::string target):
            target(std::move(target)) {}

    bool matches(const std::string& obj) const {
        return obj != target;
    }

    bool matches(const char* obj) const {
        return obj != target;
    }

    void describe(Description* description) const {
        (*description) << "not '" << target << "'";
    }

    void describeFailure(Description* description) const {
        (*description) << "'" << target << "'";
    }

    template<int n>
    operator NonEqualityMatcher<char[n]>() const; // NOLINT

 protected:
    std::string target;
};

template<int n>
class NonEqualityMatcher<char[n]>: public NonEqualityMatcher<std::string> {
 public:
    using NonEqualityMatcher<std::string>::NonEqualityMatcher;
};

template<int n>
NonEqualityMatcher<std::string>::operator NonEqualityMatcher<char[n]>() const {
    return EqualityMatcher<char[n]>(target);
}

template<>
class IsLessThanMatcher<std::string>: public StatelessMatcher {
 public:
    explicit IsLessThanMatcher(std::string target):
            target(std::move(target)) {}

    bool matches(const std::string& obj) const {
        return obj < target;
    }

    bool matches(const char* obj) const {
        return obj < target;
    }

    void describe(Description* description) const {
        (*description) << "< '" << target << "'";
    }

    void describeFailure(Description* description) const {
        (*description) << ">= '" << target << "'";
    }

    template<int n>
    operator IsLessThanMatcher<char[n]>() const; // NOLINT

 protected:
    std::string target;
};

template<int n>
class IsLessThanMatcher<char[n]>: public IsLessThanMatcher<std::string> {
 public:
    using IsLessThanMatcher<std::string>::IsLessThanMatcher;
};

template<int n>
IsLessThanMatcher<std::string>::operator IsLessThanMatcher<char[n]>() const {
    return IsLessThanMatcher<char[n]>(target);
}

template<>
class IsLessThanEqualMatcher<std::string>: public StatelessMatcher {
 public:
    explicit IsLessThanEqualMatcher(std::string target):
            target(std::move(target)) {}

    bool matches(const std::string& obj) const {
        return obj <= target;
    }

    bool matches(const char* obj) const {
        return obj <= target;
    }

    void describe(Description* description) const {
        (*description) << "<= '" << target << "'";
    }

    void describeFailure(Description* description) const {
        (*description) << "> '" << target << "'";
    }

    template<int n>
    operator IsLessThanEqualMatcher<char[n]>() const; // NOLINT

 protected:
    std::string target;
};

template<int n>
class IsLessThanEqualMatcher<char[n]>:
        public IsLessThanEqualMatcher<std::string> {
 public:
    using IsLessThanEqualMatcher<std::string>::IsLessThanEqualMatcher;
};

template<int n>
IsLessThanEqualMatcher<std::string>::
        operator IsLessThanEqualMatcher<char[n]>() const {
    return IsLessThanEqualMatcher<char[n]>(target);
}

template<>
class IsGreaterThanMatcher<std::string>: public StatelessMatcher {
 public:
    explicit IsGreaterThanMatcher(std::string target):
            target(std::move(target)) {}

    bool matches(const std::string& obj) const {
        return obj > target;
    }

    bool matches(const char* obj) const {
        return std::string(obj) > target;
    }

    void describe(Description* description) const {
        (*description) << "> '" << target << "'";
    }

    void describeFailure(Description* description) const {
        (*description) << "<= '" << target << "'";
    }

    template<int n>
    operator IsGreaterThanMatcher<char[n]>() const; // NOLINT

 protected:
    std::string target;
};

template<int n>
class IsGreaterThanMatcher<char[n]>: public IsGreaterThanMatcher<std::string> {
 public:
    using IsGreaterThanMatcher<std::string>::IsGreaterThanMatcher;
};

template<int n>
IsGreaterThanMatcher<std::string>::
        operator IsGreaterThanMatcher<char[n]>() const {
    return IsGreaterThanMatcher<char[n]>(target);
}

template<>
class IsGreaterThanEqualMatcher<std::string>: public StatelessMatcher {
 public:
    explicit IsGreaterThanEqualMatcher(std::string target):
            target(std::move(target)) {}

    bool matches(const std::string& obj) const {
        return obj >= target;
    }

    bool matches(const char* obj) const {
        return obj >= target;
    }

    void describe(Description* description) const {
        (*description) << ">= '" << target << "'";
    }

    void describeFailure(Description* description) const {
        (*description) << "< '" << target << "'";
    }

    template<int n>
    operator IsGreaterThanEqualMatcher<char[n]>() const; // NOLINT

 protected:
    std::string target;
};

template<int n>
class IsGreaterThanEqualMatcher<char[n]>:
        public IsGreaterThanEqualMatcher<std::string> {
 public:
    using IsGreaterThanEqualMatcher<std::string>::IsGreaterThanEqualMatcher;
};

template<int n>
IsGreaterThanEqualMatcher<std::string>::
        operator IsGreaterThanEqualMatcher<char[n]>() const {
    return IsGreaterThanEqualMatcher<char[n]>(target);
}

}