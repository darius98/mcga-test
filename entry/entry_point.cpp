#define VERSION "1.0.0"

#include <kktest_ext/feedback.hpp>
#include <kktest_impl/export.hpp>
#include <kktest_impl/main.hpp>

using namespace kktest;
using namespace cppli;
using namespace std;

KKTEST_API int main(int argc, char** argv) {
    Parser parser("Test binary. Generated by The KKTest Framework.");
    parser.addHelpFlag();

    feedback::FeedbackExtension feedbackExtension;
    feedbackExtension.registerCommandLineArgs(parser);

    InternalArgs internalArgs = registerInternalFlags(parser, VERSION);

    parser.interpret(argc, argv);

    return kktest::main({&feedbackExtension}, internalArgs);
}
