#include <algorithm>

#include <EasyFlags.hpp>

#include <os/folder.hpp>
#include <utils/string.hpp>
#include "explorer.hpp"

using namespace std;

AddArgument(string, argumentRootFolder)
    .Name("root_dir")
    .DefaultValue(".")
    .ImplicitValue(".");

namespace kktest_runner {

class Explorer {
public:
    explicit Explorer(const Folder& _rootFolder): rootFolder(_rootFolder) {}

    vector<File> findTestCases() const {
        vector<File> testCases;
        pair<vector<File>, vector<Folder>> children = rootFolder.children();
        for (const File& file: children.first) {
            if (isTestCase(file)) {
                testCases.push_back(file);
            }
        }
        for (const Folder& folder: children.second) {
            vector<File> subTestCases = Explorer(folder).findTestCases();
            testCases.insert(testCases.end(),
                             subTestCases.begin(), subTestCases.end());
        }
        sort(testCases.begin(), testCases.end());
        return testCases;
    }

private:
    static bool isTestCase(const File& file) {
        // TODO: Optimize this! For now, ignore some
        // common heavy but useless folders
        if (containsSubstring(file.toString(), ".git") ||
                containsSubstring(file.toString(), "CMakeFiles")) {
            return false;
        }

        if (!file.isExecutable()) {
            return false;
        }

        string cmd = "/usr/bin/ldd " + file.toString();

        char buffer[32];
        string result;
        shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
        if (!pipe) {
            throw runtime_error("popen() failed!");
        }
        while (!feof(pipe.get())) {
            if (fgets(buffer, 32, pipe.get()) != nullptr) {
                result += buffer;
            }
        }
        return containsSubstring(result, "kktest");
    }

    Folder rootFolder;
}; 

vector<File> explore() {
    return Explorer(Folder(argumentRootFolder)).findTestCases();
}

}
