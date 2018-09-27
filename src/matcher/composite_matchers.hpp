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
        m1Matches = m1->matches(obj);
        m2Matches = m2->matches(obj);
        return m1Matches && m2Matches;
    }

    void describeExpectation(Description* description) override {
        m1->describeExpectation(description);
        description->append(" and ");
        m2->describeExpectation(description);
    }

    void describeFailure(Description* description) override {
        if (m1Matches) {
            m2->describeFailure(description);
        } else {
            m1->describeFailure(description);
        }
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
        m1Matches = m1->matches(obj);
        m2Matches = m2->matches(obj);
        return m1Matches || m2Matches;
    }

    void describeExpectation(Description* description) override {
        m1->describeExpectation(description);
        description->append(" or ");
        m2->describeExpectation(description);
    }

    void describeFailure(Description* description) override {
        m1->describeFailure(description);
        description->append(" and ");
        m2->describeFailure(description);
    }
private:
    M1* m1;
    M2* m2;

    bool m1Matches = false;
    bool m2Matches = false;
};

template<class M, IS_MATCHER(M)>
class NotMatcher: public BaseMatcher {
public:
    explicit NotMatcher(M* _matcher): matcher(_matcher) {}

    template<class T>
    bool matches(const T& obj) {
        return !matcher->matches(obj);
    }

    void describeExpectation(Description* description) override {
        description->append("not ");
        matcher->describeExpectation(description);
    }

    void describeFailure(Description* description) override {
        matcher->describeExpectation(description);
    }
private:
    M* matcher;
};

}

#endif
