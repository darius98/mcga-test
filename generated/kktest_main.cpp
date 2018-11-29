// TODO(darius98): This should be auto-generated from a list of extensions.

#include <EasyFlags.hpp>

#include <kktest_impl/main.hpp>

#include <kktest_ext/feedback_impl/ext.hpp>

int main(int argc, char** argv) {

    kktest::feedback::FeedbackExtension feedbackExtension;

    easyflags::ParseEasyFlags(argc, argv);

    return kktest::main({

        &feedbackExtension

    });
}
