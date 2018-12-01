#include <EasyFlags.hpp>

#include <explorer/explorer.hpp>

using fsystem::File;
using fsystem::Folder;
using std::pair;
using std::function;
using std::string;
using std::vector;

namespace {

constexpr const size_t kkTestSigSize = 32;
constexpr const size_t kkTestSigHalfSize = kkTestSigSize >> 1;
const unsigned char kkTestSignatureFirstHalf[] =
        "\x43\x00\xaa\x4f\x56\x6e\x0c\x64\xeb\xa1\xf5\x1d\x7c\xaa\xbc\xe8";
const unsigned char kkTestSignatureSecondHalf[] =
        "\xbf\x03\x2d\x86\x40\x69\x98\x65\xa3\x79\x51\xb4\x8a\x33\xce\x97";

AddArgument(string, argumentRootFolder)
    .Name("root_dir")
    .DefaultValue(".")
    .ImplicitValue(".");

bool containsKKTestSignature(unsigned char* buffer, size_t bufferSize) {
    for (size_t i = 0; i + kkTestSigSize < bufferSize; ++ i) {
        if (memcmp(buffer + i, kkTestSignatureFirstHalf, kkTestSigHalfSize) == 0 &&
                memcmp(buffer + i + kkTestSigHalfSize,
                       kkTestSignatureSecondHalf,
                       kkTestSigHalfSize) == 0) {
            return true;
        }
    }
    return false;
}

bool isTestCase(const File& file) {
    if (!file.isExecutable() || !file.isReadable()) {
        return false;
    }
    string filePath = file.toString();
    const char* filePathCStr = filePath.c_str();
    // read the executable file, looking for the kktest signature.
    // TODO: Move this somewhere else and make it more generic.
    constexpr const size_t capacity = 1024;
    unsigned char buffer[capacity];
    FILE* filePtr = fopen(filePathCStr, "rb");
    size_t readLen = fread(buffer, 1, capacity, filePtr);
    if (ferror(filePtr) != 0) {
        // TODO: Handle.
    }
    if (readLen < kkTestSigSize) {
        return false;
    }
    if (containsKKTestSignature(buffer, readLen)) {
        return true;
    }
    while (!feof(filePtr)) {
        memcpy(buffer, buffer + readLen - kkTestSigSize, kkTestSigSize);
        readLen = fread(buffer + kkTestSigSize, 1, capacity - kkTestSigSize, filePtr);
        readLen += 32;
        if (ferror(filePtr)) {
            // TODO: Handle.
        }
        if (containsKKTestSignature(buffer, readLen)) {
            return true;
        }
    }
    return false;
}

void findTestCases(const Folder& folder, const function<void(File)>& onTestFound) {
    pair<vector<File>, vector<Folder>> children = folder.children();
    for (const File& file: children.first) {
        if (isTestCase(file)) {
            onTestFound(file);
        }
    }
    for (const Folder& childFolder: children.second) {
        findTestCases(childFolder, onTestFound);
    }
}

}

namespace runner {

void explore(const function<void(File)>& onTestFound) {
    findTestCases(Folder(argumentRootFolder), onTestFound);
}

}
