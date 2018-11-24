#include <EasyFlags.hpp>

#include <explorer/explorer.hpp>

using namespace fsystem;
using namespace strutil;
using namespace std;

AddArgument(string, argumentRootFolder)
    .Name("root_dir")
    .DefaultValue(".")
    .ImplicitValue(".");

namespace runner {

class Explorer {
public:
    explicit Explorer(Folder _rootFolder): rootFolder(move(_rootFolder)) {}

    void findTestCases(const function<void(File)>& onTestFound) const {
        // TODO: Optimize this! For now, ignore some common heavy but useless folders
        if (containsSubstring(rootFolder.toString(), ".git") ||
                containsSubstring(rootFolder.toString(), "CMakeFiles")) {
            return;
        }
        pair<vector<File>, vector<Folder>> children = rootFolder.children();
        for (const File& file: children.first) {
            if (isTestCase(file)) {
                onTestFound(file);
            }
        }
        for (const Folder& folder: children.second) {
            Explorer(folder).findTestCases(onTestFound);
        }
    }

private:
    static bool isTestCase(const File& file) {
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

void explore(const function<void(File)>& onTestFound) {
    Explorer(Folder(argumentRootFolder)).findTestCases(onTestFound);
}

}
