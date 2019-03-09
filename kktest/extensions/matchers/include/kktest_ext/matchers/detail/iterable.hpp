#pragma once

#include <kktest_ext/matchers/comparison.hpp>

namespace kktest::matchers::detail {

class IsEmptyMatcher: public StatelessMatcher {
 public:
    template<class T>
    bool matches(const T& object) const {
        return object.empty();
    }

    void describe(Description* description) const {
        (*description) << "empty iterable";
    }

    void describeFailure(Description* description) const {
        (*description) << "a non-empty iterable";
    }
};

class IsNotEmptyMatcher: public StatelessMatcher {
 public:
    template<class T>
    bool matches(const T& object) const {
        return !object.empty();
    }

    void describe(Description* description) const {
        (*description) << "non-empty iterable";
    }

    void describeFailure(Description* description) const {
        (*description) << "empty iterable";
    }
};

template<class M>
class IterableSizeMatcher: public StatefulMatcher<typename M::State> {
 public:
    explicit constexpr IterableSizeMatcher(M sizeMatcher):
            sizeMatcher(std::move(sizeMatcher)) {}

    template<class T>
    bool matches(const T& obj, typename M::State* state) const {
        return __matches(sizeMatcher, state, obj.size());
    }

    void describe(Description* description) const {
        (*description) << "iterable where size is ";
        sizeMatcher.describe(description);
    }

    void describeFailure(Description* description,
                         typename M::State* state) const {
        (*description) << "iterable where size is ";
        __describeFailure(description, sizeMatcher, state);
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
 public:
    explicit constexpr IterableEachMatcher(M elementMatcher):
            elementMatcher(std::move(elementMatcher)) {}

    template<class T>
    bool matches(const T& iterable, IterableEachState<M>* state) const {
        state->index = -1;
        for (const auto& obj : iterable) {
            state->index += 1;
            if (!__matches(elementMatcher, &state->elementState, obj)) {
                return false;
            }
        }
        return true;
    }

    void describe(Description* description) const {
        (*description) << "an iterable where each element is ";
        elementMatcher.describe(description);
    }

    void describeFailure(Description* description,
                         IterableEachState<M>* state) const {
        (*description) << "an iterable where at index "
                       << index
                       << " the element is ";
        __describeFailure(description, elementMatcher, &state->elementState);
    }

 private:
    M elementMatcher;
};

template<class M>
class IterableAnyMatcher: public StatelessMatcher {
 public:
    explicit constexpr IterableAnyMatcher(M elementMatcher):
            elementMatcher(std::move(elementMatcher)) {}

    template<class T>
    bool matches(const T& collection) const {
        typename M::State state;
        for (const auto& obj : collection) {
            if (__matches(elementMatcher, &state, obj)) {
                return true;
            }
        }
        return false;
    }

    void describe(Description* description) const {
        (*description) << "an iterable where at least one element is ";
        elementMatcher.describe(description);
    }

    void describeFailure(Description* description) const {
        (*description) << "an iterable where no element is ";
        elementMatcher.describe(description);
    }

 private:
    M elementMatcher;
};

}
