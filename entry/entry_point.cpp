#include "core/src/main.hpp"

#include "extensions/feedback/src/ext.hpp"

KKTEST_EXPORT int main(int argc, char** argv) {
    return kktest::main(argc, argv, {
        new kktest::feedback::FeedbackExtension()
    });
}
