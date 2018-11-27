#ifndef COMMON_FSYSTEM_INCLUDE_FSYSTEM_IMPL_PATH_H_
#define COMMON_FSYSTEM_INCLUDE_FSYSTEM_IMPL_PATH_H_

#include <string>
#include <vector>

#include <strutil>

#include <fsystem_impl/error.hpp>

namespace fsystem {

class Path: public strutil::StringKeyComparable {
public:
    static const char  SEP;
    static const char* SELF_FOLDER;
    static const char* PARENT_FOLDER;

    static Path parent(const Path& path);

    static Path join(const std::string& a, const std::string& b);
    static Path join(const Path& a, const std::string& b);
    static Path join(const std::string& a, const Path& b);
    static Path join(const Path& a, const Path& b);

    template<typename... Args>
    static Path join(const std::string& a,
                     const std::string& b,
                     const Args... args) {
        return join(join(a, b), args...);
    }

    template<typename... Args>
    static Path join(const Path& a, const std::string& b, const Args... args) {
        return join(join(a, b), args...);
    }

    template<typename... Args>
    static Path join(const std::string& a, const Path& b, const Args... args) {
        return join(join(a, b), args...);
    }

    template<typename... Args>
    static Path join(const Path& a, const Path& b, const Args... args) {
        return join(join(a, b), args...);
    }

    explicit Path(const std::string& path);

    Path(const Path& other);

    Path(Path&& other) noexcept;

    template<typename... Args>
    Path(const Path& a, const Path& b, const Args... args):
            Path(join(a, b, args...)) {}

    template<typename... Args>
    Path(const Path& a, const std::string& b, const Args... args):
            Path(join(a, b, args...)) {}

    template<typename... Args>
    Path(const std::string& a, const Path& b, const Args... args):
            Path(join(a, b, args...)) {}

    template<typename... Args>
    Path(const std::string& a, const std::string& b, const Args... args):
            Path(join(a, b, args...)) {}

    Path& operator=(const Path& other);
    Path& operator=(Path&& other) noexcept;

    bool isRelative() const;
    bool isAbsolute() const;

    Path absolute() const;

    std::string last() const;
    std::string toString() const;
    std::vector<std::string> getParts() const;

    Path operator+(const Path& other) const;
    Path operator+(const std::string& other) const;

    Path parent() const;

private:
    bool applySpecialPart(const std::string& part);
    void addPart(const std::string& part);
    void addPart(std::string&& part);

    std::string key() const override;

    std::vector<std::string> parts;
    bool relative;

};

}

#endif
