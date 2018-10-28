#ifndef KKTEST_MATCHER_COMPOSITE_MATCHERS_H_
#define KKTEST_MATCHER_COMPOSITE_MATCHERS_H_

#include <core/matcher.hpp>


namespace kktest {

template<class M1, class M2, IS_MATCHER(M1), IS_MATCHER(M2)>
class AndMatcher: public Matcher {
public:
    AndMatcher(M1* _m1, M2* _m2): m1(_m1), m2(_m2) {}

    template<class T>
    bool matches(const T& obj) {
        m1Matches = m1->matches(obj);
        m2Matches = m2->matches(obj);
        return m1Matches && m2Matches;
    }

    void describe(Description& description) override {
        m1->describe(description);
        description << " and ";
        m2->describe(description);
    }

    void describeMismatch(Description& description) override {
        if (m1Matches) {
            m2->describeMismatch(description);
        } else {
            m1->describeMismatch(description);
        }
    }
private:
    M1* m1;
    M2* m2;

    bool m1Matches = false;
    bool m2Matches = false;
};

template<class M1, class M2, IS_MATCHER(M1), IS_MATCHER(M2)>
class OrMatcher: public Matcher {
public:
    OrMatcher(M1* _m1, M2* _m2): m1(_m1), m2(_m2) {}

    template<class T>
    bool matches(const T& obj) {
        m1Matches = m1->matches(obj);
        m2Matches = m2->matches(obj);
        return m1Matches || m2Matches;
    }

    void describe(Description& description) override {
        m1->describe(description);
        description << " or ";
        m2->describe(description);
    }

    void describeMismatch(Description& description) override {
        m1->describeMismatch(description);
        description << " and ";
        m2->describeMismatch(description);
    }
private:
    M1* m1;
    M2* m2;

    bool m1Matches = false;
    bool m2Matches = false;
};

template<class M, IS_MATCHER(M)>
class NotMatcher: public Matcher {
public:
    explicit NotMatcher(M* _matcher): matcher(_matcher) {}

    template<class T>
    bool matches(const T& obj) {
        return !matcher->matches(obj);
    }

    void describe(Description& description) override {
        description << "not ";
        matcher->describe(description);
    }

    void describeMismatch(Description& description) override {
        matcher->describe(description);
    }
private:
    M* matcher;
};

template<class M1, class M2, IS_MATCHER(M1), IS_MATCHER(M2)>
AndMatcher<M1, M2>* both(M1* m1, M2* m2) {
    return new AndMatcher<M1, M2>(m1, m2);
}

template<class M1, class M2, IS_MATCHER(M1), IS_MATCHER(M2)>
OrMatcher<M1, M2>* either(M1* m1, M2* m2) {
    return new OrMatcher<M1, M2>(m1, m2);
}

template<class M, IS_MATCHER(M)>
NotMatcher<M>* isNot(M* matcher) {
    return new NotMatcher<M>(matcher);
}

}

#endif
