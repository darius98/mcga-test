#pragma once

#include <iostream>

namespace kktest {

template<class... Args>
void EmitWarning(const Args&... args) {
    ((std::cout << "KKTest Warning: ") << ... << args) << "\n";
}

}
