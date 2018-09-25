#ifndef RUNTIME_TESTING_MATCHER_COMPOSITE_MATCHERS_H_
#define RUNTIME_TESTING_MATCHER_COMPOSITE_MATCHERS_H_

#include "matcher.hpp"


namespace matcher {

template<class M1, class M2>
class AndMatcher: public BaseMatcher {
public:
    AndMatcher(M1* _m1, M2* _m2): m1(_m1), m2(_m2) {}

    template<class T>
    bool matches(const T& obj) {
        this->m1Matches = this->m1->matches(obj);
        if (!this->m1Matches) {
            return false;
        }
        this->m2Matches = this->m2->matches(obj);
        return this->m2Matches;
    }

    template<class T>
    void describe(const T& obj, Description* description) {
        if (this->m1Matches) {
            this->m2->describe(obj, description);
        } else {
            this->m1->describe(obj, description);
        }
    }
private:
    M1* m1;
    M2* m2;

    bool m1Matches = false;
    bool m2Matches = false;
};

template<class M1, class M2>
class OrMatcher: public BaseMatcher {
public:
    OrMatcher(M1* _m1, M2* _m2): m1(_m1), m2(_m2) {}

    template<class T>
    bool matches(const T& obj) {
        this->m1Matches = this->m1->matches(obj);
        if (this->m1Matches) {
            return true;
        }
        this->m2Matches = this->m2->matches(obj);
        return this->m2Matches;
    }

    template<class T>
    void describe(const T& obj, Description* description) {
        description->append("(");
        this->m2->describe(obj, description);
        description->append(") OR (");
        this->m1->describe(obj, description);
        description->append(")");
    }
private:
    M1* m1;
    M2* m2;

    bool m1Matches = false;
    bool m2Matches = false;
};

template<class Matcher>
class NotMatcher: public BaseMatcher {
public:
    explicit NotMatcher(Matcher* matcher): matcher(matcher) {}

    template<class T>
    bool matches(const T& obj) {
        return !this->matcher->matches(obj);
    }

    template<class T>
    void describe(const T& obj, Description* description) {
        description->append("NOT(");
        this->matcher->describe(obj, description);
        description->append(")");
    }

private:
    Matcher* matcher;
};

}

#endif
