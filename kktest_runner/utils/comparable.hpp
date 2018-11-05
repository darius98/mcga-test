#ifndef KKTEST_RUNNER_UTILS_COMPARABLE_H_
#define KKTEST_RUNNER_UTILS_COMPARABLE_H_


namespace kktest_runner {

template<class T>
class Comparable {
public:
    virtual ~Comparable() = default;

    bool operator<=(const Comparable& other) const {
        return key() <= other.key();
    }
    bool operator>=(const Comparable& other) const {
        return key() >= other.key();
    }
    bool operator<(const Comparable& other) const {
        return key() < other.key();
    }
    bool operator>(const Comparable& other) const {
        return key() > other.key();
    }
    bool operator==(const Comparable& other) const {
        return key() == other.key();
    }
    bool operator!=(const Comparable& other) const {
        return key() != other.key();
    }

    bool operator<=(const T& other) const {
        return key() <= other;
    }
    bool operator>=(const T& other) const {
        return key() >= other;
    }
    bool operator<(const T& other) const {
        return key() < other;
    }
    bool operator>(const T& other) const {
        return key() > other;
    }
    bool operator==(const T& other) const {
        return key() == other;
    }
    bool operator!=(const T& other) const {
        return key() != other;
    }

    template<class K>
    friend bool operator<=(const K& lhs, const Comparable<K>& other) {
        return lhs <= other.key();
    }
    template<class K>
    friend bool operator>=(const K& lhs, const Comparable<K>& other) {
        return lhs >= other.key();
    }
    template<class K>
    friend bool operator<(const K& lhs, const Comparable<K>& other) {
        return lhs < other.key();
    }
    template<class K>
    friend bool operator>(const K& lhs, const Comparable<K>& other) {
        return lhs > other.key();
    }
    template<class K>
    friend bool operator==(const K& lhs, const Comparable<K>& other) {
        return lhs == other.key();
    }
    template<class K>
    friend bool operator!=(const K& lhs, const Comparable<K>& other) {
        return lhs != other.key();
    }

private:
    virtual T key() const = 0;
};

}

#endif
