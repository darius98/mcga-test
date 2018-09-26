#ifndef RUNTIME_TESTING_MATCHER_COMPOSITE_MATCHERS_H_
#define RUNTIME_TESTING_MATCHER_COMPOSITE_MATCHERS_H_

#include "matcher.hpp"


namespace matcher {

template<class M1, class M2, IS_MATCHER(M1), IS_MATCHER(M2)>
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

    void describeExpectation(Description* description) {
        this->m1->describeExpectation(description);
        description->append(" and ");
        this->m2->describeExpectation(description);
    }

    void describeFailure(Description* description) {
        if (this->m1Matches) {
            this->m2->describeFailure(description);
        } else {
            this->m1->describeFailure(description);
        }
    }

    void describeSuccess(Description* description) {
        this->m1->describeSuccess(description);
        description->append(" and ");
        this->m2->describeSuccess(description);
    }
private:
    M1* m1;
    M2* m2;

    bool m1Matches = false;
    bool m2Matches = false;
};

template<class M1, class M2, IS_MATCHER(M1), IS_MATCHER(M2)>
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

    void describeExpectation(Description* description) {
        this->m1->describeExpectation(description);
        description->append(" or ");
        this->m2->describeExpectation(description);
    }

    void describeFailure(Description* description) {
        this->m1->describeFailure(description);
        description->append(" and ");
        this->m2->describeFailure(description);
    }

    void describeSuccess(Description* description) {
        if (this->m1Matches) {
            this->m1->describeSuccess(description);
        } else {
            this->m2->describeSuccess(description);
        }
    }
private:
    M1* m1;
    M2* m2;

    bool m1Matches = false;
    bool m2Matches = false;
};

template<class Matcher, IS_MATCHER(Matcher)>
class NotMatcher: public BaseMatcher {
public:
    explicit NotMatcher(Matcher* matcher): matcher(matcher) {}

    template<class T>
    bool matches(const T& obj) {
        return !this->matcher->matches(obj);
    }

    void describeExpectation(Description* description) {
        description->append("not ");
        this->matcher->describeExpectation(description);
    }

    void describeFailure(Description* description) {
        this->matcher->describeSuccess(description);
    }

    void describeSuccess(Description* description) {
        this->matcher->describeFailure(description);
    }
private:
    Matcher* matcher;
};

}

#endif
