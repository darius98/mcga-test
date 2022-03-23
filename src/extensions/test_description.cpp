#include "test_description.hpp"

#include <numeric>
#include <string>
#include <vector>

namespace mcga::test {

std::string getTestFullDescription(const Test& test) {
    std::vector<std::string> groupDescriptions;
    GroupPtr parent = test.getGroup();
    while (parent != nullptr) {
        std::string groupDescription = parent->getDescription().c_str();
        if (!groupDescription.empty()) {
            groupDescriptions.push_back(groupDescription);
        }
        parent = parent->getParentGroup();
    }
    std::string groupDescription
      = std::accumulate(groupDescriptions.rbegin(),
                        groupDescriptions.rend(),
                        std::string(""),
                        [](const std::string& a, const std::string& b) {
                            return a.empty() ? b : (a + "::" + b);
                        });
    if (!groupDescription.empty()) {
        groupDescription += "::";
    }
    return groupDescription + test.getDescription().c_str();
}

}  // namespace mcga::test
