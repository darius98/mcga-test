#ifndef KKTEST_RUNNER_OS_EXTENSION_H_
#define KKTEST_RUNNER_OS_EXTENSION_H_

#include <string>

#include <utils/comparable.hpp>


namespace kktest_runner {

class Extension: public Comparable<std::string> {
public:
    Extension(const std::string& _extension);
    Extension(const Extension& other);
    Extension(Extension&& other);

    Extension& operator=(const Extension& other);
    Extension& operator=(Extension&& other);

    std::string toString() const;
private:
    std::string extension;

    std::string key() const override;
};

}

#endif
