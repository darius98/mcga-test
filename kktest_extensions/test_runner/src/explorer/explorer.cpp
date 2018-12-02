#include <cstring>

#include <utility>
#include <vector>

#include <kktest_extension_api.hpp>

#include <explorer/explorer.hpp>

using std::pair;
using std::function;
using std::vector;
using std::filesystem::directory_iterator;
using std::filesystem::is_directory;
using std::filesystem::is_regular_file;
using std::filesystem::path;
using std::filesystem::perms;
using std::filesystem::status;

namespace kktest {
namespace test_runner {

constexpr const size_t kkTestSigSize = 32u;
constexpr const size_t kkTestSigHalfSize = kkTestSigSize >> 1u;
const unsigned char kkTestSignatureFirstHalf[] =
        "\x43\x00\xaa\x4f\x56\x6e\x0c\x64\xeb\xa1\xf5\x1d\x7c\xaa\xbc\xe8";
const unsigned char kkTestSignatureSecondHalf[] =
        "\xbf\x03\x2d\x86\x40\x69\x98\x65\xa3\x79\x51\xb4\x8a\x33\xce\x97";

bool containsKKTestSignature(unsigned char* buffer, size_t bufferSize) {
    for (size_t i = 0; i + kkTestSigSize < bufferSize; ++i) {
        if (memcmp(buffer + i, kkTestSignatureFirstHalf, kkTestSigHalfSize) == 0 &&
                memcmp(buffer + i + kkTestSigHalfSize,
                       kkTestSignatureSecondHalf,
                       kkTestSigHalfSize) == 0) {
            return true;
        }
    }
    return false;
}

bool isTestCase(const path& file) {
    auto fileStatus = status(file);
    auto expectedRXPerms = perms::owner_read | perms::owner_exec;
    auto actualRXPerms = fileStatus.permissions() & expectedRXPerms;
    if (actualRXPerms != expectedRXPerms) {
        return false;
    }
    const char* filePathCStr = file.c_str();
    // read the executable file, looking for the kktest signature.
    // TODO(darius98): Move this somewhere else and make it more generic.
    constexpr const size_t capacity = 1024;
    unsigned char buffer[capacity];
    FILE* filePtr = fopen(filePathCStr, "rb");
    size_t readLen = fread(buffer, 1, capacity, filePtr);
    if (ferror(filePtr) != 0) {
        // TODO(darius98): Handle.
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
            // TODO(darius98): Handle.
        }
        if (containsKKTestSignature(buffer, readLen)) {
            return true;
        }
    }
    return false;
}

void findTestCases(const path& folder, const function<void(path)>& onTestFound) {
    for (const path& child : directory_iterator(folder)) {
        if (is_regular_file(child) && isTestCase(child)) {
            onTestFound(child);
        }
        if (is_directory(child)) {
            findTestCases(child, onTestFound);
        }
    }
}

void explore(const path& rootPath, const function<void(path)>& onTestFound) {
    if (is_regular_file(rootPath)) {
        if (isTestCase(rootPath)) {
            onTestFound(rootPath);
        }
    } else if (is_directory(rootPath)) {
        findTestCases(rootPath, onTestFound);
    }
}

}  // namespace test_runner
}  // namespace kktest
