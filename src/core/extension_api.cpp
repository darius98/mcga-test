#include "extension_api.hpp"

namespace mcga::test {

ExtensionApi::ExtensionApi(Extension* extensions, std::size_t numExtensions)
        : extensions(extensions), numExtensions(numExtensions) {
}

void ExtensionApi::init() {
    forEach([](Extension ext) {
        ext.vtable->init(ext.data);
    });
}

void ExtensionApi::onGroupDiscovered(const GroupPtr& group) {
    forEach([&group](Extension ext) {
        ext.vtable->onGroupDiscovered(ext.data, group);
    });
}

void ExtensionApi::onTestDiscovered(const Test& test) {
    forEach([&test](Extension ext) {
        ext.vtable->onTestDiscovered(ext.data, test);
    });
}

void ExtensionApi::beforeTestExecution(
  const Test& test, std::optional<Test::ExecutionInfo>& info) {
    forEach([&test, &info](Extension ext) {
        ext.vtable->beforeTestExecution(ext.data, test, info);
    });
}

void ExtensionApi::afterTestExecution(const Test& test) {
    forEach([&test](Extension ext) {
        ext.vtable->afterTestExecution(ext.data, test);
    });
}

void ExtensionApi::onWarning(const Warning& warning) {
    forEach([&warning](Extension ext) {
        ext.vtable->onWarning(ext.data, warning);
    });
}

void ExtensionApi::destroy() {
    forEach([](Extension ext) {
        ext.vtable->destroy(ext.data);
    });
}

}  // namespace mcga::test
