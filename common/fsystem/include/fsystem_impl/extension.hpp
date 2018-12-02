#ifndef COMMON_FSYSTEM_INCLUDE_FSYSTEM_IMPL_EXTENSION_HPP_
#define COMMON_FSYSTEM_INCLUDE_FSYSTEM_IMPL_EXTENSION_HPP_

#include <string>

#include <strutil.hpp>

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

}  // namespace fsystem

#endif  // COMMON_FSYSTEM_INCLUDE_FSYSTEM_IMPL_EXTENSION_HPP_
