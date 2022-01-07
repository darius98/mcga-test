#include "color.hpp"

#include <cstdio>
#include <unistd.h>

void write_if_colorized(std::ostream& stream, const char* ansi_seq) {
    if (mcga::test::feedback::is_terminal(stream)) {
        stream << ansi_seq;
    }
}

namespace mcga::test::feedback {

bool is_terminal(const std::ostream& stream) {
    if (&stream == &std::cout) {
        return isatty(fileno(stdout));
    }
    if (&stream == &std::cerr) {
        return isatty(fileno(stderr));
    }
    return false;
}

std::ostream& reset(std::ostream& stream) {
    write_if_colorized(stream, "\033[00m");
    return stream;
}

std::ostream& bold(std::ostream& stream) {
    write_if_colorized(stream, "\033[1m");
    return stream;
}

std::ostream& dark(std::ostream& stream) {
    write_if_colorized(stream, "\033[2m");
    return stream;
}

std::ostream& underline(std::ostream& stream) {
    write_if_colorized(stream, "\033[4m");
    return stream;
}

std::ostream& blink(std::ostream& stream) {
    write_if_colorized(stream, "\033[5m");
    return stream;
}

std::ostream& reverse(std::ostream& stream) {
    write_if_colorized(stream, "\033[7m");
    return stream;
}

std::ostream& concealed(std::ostream& stream) {
    write_if_colorized(stream, "\033[8m");
    return stream;
}

std::ostream& grey(std::ostream& stream) {
    write_if_colorized(stream, "\033[30m");
    return stream;
}

std::ostream& red(std::ostream& stream) {
    write_if_colorized(stream, "\033[31m");
    return stream;
}

std::ostream& green(std::ostream& stream) {
    write_if_colorized(stream, "\033[32m");
    return stream;
}

std::ostream& yellow(std::ostream& stream) {
    write_if_colorized(stream, "\033[33m");
    return stream;
}

std::ostream& blue(std::ostream& stream) {
    write_if_colorized(stream, "\033[34m");
    return stream;
}

std::ostream& magenta(std::ostream& stream) {
    write_if_colorized(stream, "\033[35m");
    return stream;
}

std::ostream& cyan(std::ostream& stream) {
    write_if_colorized(stream, "\033[36m");
    return stream;
}

std::ostream& white(std::ostream& stream) {
    write_if_colorized(stream, "\033[37m");
    return stream;
}

std::ostream& on_grey(std::ostream& stream) {
    write_if_colorized(stream, "\033[40m");
    return stream;
}

std::ostream& on_red(std::ostream& stream) {
    write_if_colorized(stream, "\033[41m");
    return stream;
}

std::ostream& on_green(std::ostream& stream) {
    write_if_colorized(stream, "\033[42m");
    return stream;
}

std::ostream& on_yellow(std::ostream& stream) {
    write_if_colorized(stream, "\033[43m");
    return stream;
}

std::ostream& on_blue(std::ostream& stream) {
    write_if_colorized(stream, "\033[44m");
    return stream;
}

std::ostream& on_magenta(std::ostream& stream) {
    write_if_colorized(stream, "\033[45m");
    return stream;
}

std::ostream& on_cyan(std::ostream& stream) {
    write_if_colorized(stream, "\033[46m");
    return stream;
}

std::ostream& on_white(std::ostream& stream) {
    write_if_colorized(stream, "\033[47m");
    return stream;
}

}  // namespace mcga::test::feedback
