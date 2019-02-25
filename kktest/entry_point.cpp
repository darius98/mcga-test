#include "kktest/core/include/kktest.hpp"

#include "kktest/core/src/main.hpp"
#include "kktest/extensions/feedback/src/ext.hpp"

using namespace std;

namespace kktest {

int run(int argc, char **argv, vector<TestCase>&& tests) {
    return main(argc, argv, move(tests), {
        new feedback::FeedbackExtension()
    });
}

}
