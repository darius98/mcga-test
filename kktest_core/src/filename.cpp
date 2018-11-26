#include <fsystem>
#include <strutil>

#include <kktest_impl/filename.hpp>

using namespace fsystem;
using namespace std;
using namespace strutil;

namespace kktest {

string trimFileName(const string& fileName) {
    static string cwd(Folder::currentWorkingFolder().toString());
    if (startsWith(fileName, cwd)) {
        return fileName.substr(cwd.size());
    }
    return fileName;
}

}
