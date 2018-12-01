// TODO(darius98): This should be auto-generated from a list of extensions.
#include <kktest_impl/main.hpp>

// @<AUTO-GENERATE>[IMPORTS]
#include <kktest_ext/feedback_impl/ext.hpp>

int main(int argc, char** argv) {
    std::vector<kktest::Extension*> extensions;
    kktest::ArgumentsApi* argumentsApi = kktest::ArgumentsApi::create("KKTest test binary.");

// @<AUTO-GENERATE>[EXTENSIONS_DEF]
kktest::feedback::FeedbackExtension feedbackExtension;
feedbackExtension.registerCommandLineArguments(argumentsApi);
extensions.push_back(&feedbackExtension);

    kktest::InternalArgs internalArgs = kktest::registerInternalFlags(argumentsApi);
    argumentsApi->interpret(argc, argv);
    int ret = kktest::main(extensions, internalArgs);
    delete argumentsApi;
    return ret;
}
