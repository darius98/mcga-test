#ifndef COMMON_FSYSTEM_INCLUDE_EXTENSION_H_
#define COMMON_FSYSTEM_INCLUDE_EXTENSION_H_

#include <string>

#include <strutil>


namespace fsystem {

class Extension: public strutil::StringKeyComparable {
public:
    explicit Extension(const std::string& _extension);
    Extension(const Extension& other);
    Extension(Extension&& other) noexcept;

    Extension& operator=(const Extension& other);
    Extension& operator=(Extension&& other) noexcept;

    std::string toString() const;

private:
    std::string extension;

    std::string key() const override;
};

}

#endif
