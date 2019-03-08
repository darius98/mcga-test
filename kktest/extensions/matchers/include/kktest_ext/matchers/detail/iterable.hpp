#pragma once

#include <kktest_ext/matchers/comparison.hpp>

namespace kktest::matchers::detail {

class IsEmptyMatcher: public StatelessMatcher {
 public:
    constexpr IsEmptyMatcher() = default;

    template<class T>
    bool matches(const T& object) const {
        return object.empty();
    }

    void describe(Description* description) const override {
        (*description) << "empty iterable";
    }

    void describeFailure(Description* description) const override {
        (*description) << "a non-empty iterable";
    }
};

class IsNotEmptyMatcher: public StatelessMatcher {
 public:
    constexpr IsNotEmptyMatcher() = default;

    template<class T>
    bool matches(const T& object) const {
        return !object.empty();
    }

    void describe(Description* description) const override {
        (*description) << "non-empty iterable";
    }

    void describeFailure(Description* description) const override {
        (*description) << "empty iterable";
    }
};

template<class M>
class IterableSizeMatcher: public StatefulMatcher<typename M::State> {
    static_assert(std::is_base_of<Matcher, M>::value,
                  "SizeMatcher only supports other matchers as template args.");

 public:
    explicit IterableSizeMatcher(const M& sizeMatcher):
            sizeMatcher(sizeMatcher) {}

    template<class T>
    bool matches(const T& obj, typename M::State* state) const {
        if constexpr (M::HasState) {
            return sizeMatcher.matches(obj.size(), state);
        } else {
            return sizeMatcher.matches(obj.size());
        }
    }

    void describe(Description* description) const override {
        (*description) << "iterable where size is ";
        sizeMatcher.describe(description);
    }

    void describeFailure(Description* description,
                         typename M::State* state) const override {
        (*description) << "iterable where size is ";
        if constexpr (M::HasState) {
            sizeMatcher.describeFailure(description, state);
        } else {
            sizeMatcher.describeFailure(description);
        }
    }

 private:
    M sizeMatcher;
};

template<class M>
struct IterableEachState {
    int index = -1;
    typename M::State elementState;
};

template<class M>
class IterableEachMatcher: public StatefulMatcher<IterableEachState<M>> {
    static_assert(std::is_base_of<Matcher, M>::value,
                  "EachMatcher only supports other matchers as template args.");

 public:
    explicit IterableEachMatcher(M elementMatcher):
            elementMatcher(elementMatcher) {}

    template<class T>
    bool matches(const T& iterable, IterableEachState<M>* state) const {
        state->index = -1;
        for (const auto& obj : iterable) {
            state->index += 1;
            bool match;
            if constexpr (M::HasState) {
                match = elementMatcher.matches(obj, &state->elementState);
            } else {
                match = elementMatcher.matches(obj);
            }
            if (!match) {
                return false;
            }
        }
        return true;
    }

    void describe(Description* description) const override {
        (*description) << "an iterable where each element is ";
        elementMatcher.describe(description);
    }

    void describeFailure(Description* description,
                         IterableEachState<M>* state) const override {
        (*description) << "an iterable where at index "
                       << index
                       << " the element is ";
        if constexpr (M::HasState) {
            elementMatcher.describeFailure(description, &state->elementState);
        } else {
            elementMatcher.describeFailure(description);
        }
    }

 private:
    M elementMatcher;
};

template<class M>
class IterableAnyMatcher: public StatelessMatcher {
 public:
    explicit IterableAnyMatcher(M elementMatcher):
            elementMatcher(elementMatcher) {}
    IterableAnyMatcher(const IterableAnyMatcher& other):
            elementMatcher(other.elementMatcher) {}

    template<class T>
    bool matches(const T& collection) const {
        for (const auto& obj : collection) {
            if constexpr (M::HasState) {
                typename M::State state;
                if (elementMatcher.matches(obj, &state)) {
                    return true;
                }
            } else {
                if (elementMatcher.matches(obj)) {
                    return true;
                }
            }
        }
        return false;
    }

    void describe(Description* description) const override {
        (*description) << "an iterable where at least one element is ";
        elementMatcher.describe(description);
    }

    void describeFailure(Description* description) const override {
        (*description) << "an iterable where no element is ";
        elementMatcher.describe(description);
    }

 private:
    M elementMatcher;
};

}
