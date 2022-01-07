#pragma once

#include <iostream>

namespace mcga::test::feedback {

bool is_terminal(const std::ostream& stream);

std::ostream& reset(std::ostream& stream);

std::ostream& bold(std::ostream& stream);
std::ostream& dark(std::ostream& stream);
std::ostream& underline(std::ostream& stream);
std::ostream& blink(std::ostream& stream);
std::ostream& reverse(std::ostream& stream);
std::ostream& concealed(std::ostream& stream);

std::ostream& grey(std::ostream& stream);
std::ostream& red(std::ostream& stream);
std::ostream& green(std::ostream& stream);
std::ostream& yellow(std::ostream& stream);
std::ostream& blue(std::ostream& stream);
std::ostream& magenta(std::ostream& stream);
std::ostream& cyan(std::ostream& stream);
std::ostream& white(std::ostream& stream);

std::ostream& on_grey(std::ostream& stream);
std::ostream& on_red(std::ostream& stream);
std::ostream& on_green(std::ostream& stream);
std::ostream& on_yellow(std::ostream& stream);
std::ostream& on_blue(std::ostream& stream);
std::ostream& on_magenta(std::ostream& stream);
std::ostream& on_cyan(std::ostream& stream);
std::ostream& on_white(std::ostream& stream);

}  // namespace mcga::test::feedback
