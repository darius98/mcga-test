#ifndef KKTEST_EXTENSIONS_CORE_MATCHERS_KKTEST_EXT_CORE_MATCHERS_IMPL_COMPARISON_MATCHERS_STRING_SPEC_HPP_
#define KKTEST_EXTENSIONS_CORE_MATCHERS_KKTEST_EXT_CORE_MATCHERS_IMPL_COMPARISON_MATCHERS_STRING_SPEC_HPP_

namespace kktest {
namespace core_matchers {

// Equality
template<>
class EqualityMatcher<std::string>: public Matcher {
 public:
    explicit EqualityMatcher(std::string _target);

    bool matches(const std::string& obj);

    bool matches(const char* obj);

    void describe(Description* description);

    void describeMismatch(Description* description, const std::string& obj);

    template<int n>
    operator EqualityMatcher<char[n]>() const;

 protected:
    std::string target;
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


// Non-equality
template<>
class NonEqualityMatcher<std::string>: public Matcher {
 public:
    explicit NonEqualityMatcher(std::string _target);

    bool matches(const std::string& obj);

    bool matches(const char* obj);

    void describe(Description* description);

    void describeMismatch(Description* description, const std::string& obj);

    template<int n>
    operator NonEqualityMatcher<char[n]>() const;

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

// Less than
template<>
class IsLessThanMatcher<std::string>: public Matcher {
 public:
    explicit IsLessThanMatcher(std::string _target);

    bool matches(const std::string& obj);

    bool matches(const char* obj);

    void describe(Description* description);

    void describeMismatch(Description* description, const std::string& obj);

    template<int n>
    operator IsLessThanMatcher<char[n]>() const;

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

// Less than equal
template<>
class IsLessThanEqualMatcher<std::string>: public Matcher {
 public:
    explicit IsLessThanEqualMatcher(std::string _target);

    bool matches(const std::string& obj);

    bool matches(const char* obj);

    void describe(Description* description);

    void describeMismatch(Description* description, const std::string& obj);

    template<int n>
    operator IsLessThanEqualMatcher<char[n]>() const;

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

// Greater than
template<>
class IsGreaterThanMatcher<std::string>: public Matcher {
 public:
    explicit IsGreaterThanMatcher(std::string _target);

    bool matches(const std::string& obj);

    bool matches(const char* obj);

    void describe(Description* description);

    void describeMismatch(Description* description, const std::string& obj);

    template<int n>
    operator IsGreaterThanMatcher<char[n]>() const;

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

// Greater than equal
template<>
class IsGreaterThanEqualMatcher<std::string>: public Matcher {
 public:
    explicit IsGreaterThanEqualMatcher(std::string _target);

    bool matches(const std::string& obj);

    bool matches(const char* obj);

    void describe(Description* description);

    void describeMismatch(Description* description, const std::string& obj);

    template<int n>
    operator IsGreaterThanEqualMatcher<char[n]>() const;

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
}

#endif
