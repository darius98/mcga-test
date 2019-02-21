#include "core/include/kktest.hpp"

#include "core/src/main.hpp"
#include "extensions/feedback/src/ext.hpp"

using namespace std;

namespace kktest {

int run(int argc, char **argv, vector<TestCase>&& tests) {
    return main(argc, argv, move(tests), {
        new feedback::FeedbackExtension()
    });
}

}
