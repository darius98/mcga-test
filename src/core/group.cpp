#include "group.hpp"
#include "memory.hpp"

#include <new>

namespace mcga::test {

Group::Ptr::Ptr(Group* group): raw(group) {
    raw->refCount++;
}

void Group::Ptr::del() {
    if (raw != nullptr) {
        if (--raw->refCount == 0) {
            raw->~Group();
            deallocate_group(raw);
        }
    }
}

Group::Ptr::Ptr() = default;

Group::Ptr::Ptr(const Ptr& other) noexcept: raw(other.raw) {
    if (raw != nullptr) {
        raw->refCount++;
    }
}

Group::Ptr::Ptr(Ptr&& other) noexcept: raw(other.raw) {
    other.raw = nullptr;
}

Group::Ptr& Group::Ptr::operator=(const Ptr& other) noexcept {
    if (this != &other) {
        del();
        raw = other.raw;
        if (raw != nullptr) {
            raw->refCount++;
        }
    }
    return *this;
}

Group::Ptr& Group::Ptr::operator=(Ptr&& other) noexcept {
    if (this != &other) {
        del();
        raw = other.raw;
        other.raw = nullptr;
    }
    return *this;
}

Group::Ptr::~Ptr() {
    del();
}

Group* Group::Ptr::operator->() noexcept {
    return raw;
}

const Group* Group::Ptr::operator->() const noexcept {
    return raw;
}

Group& Group::Ptr::operator*() noexcept {
    return *raw;
}

const Group& Group::Ptr::operator*() const noexcept {
    return *raw;
}

bool Group::Ptr::operator==(std::nullptr_t) const noexcept {
    return raw == nullptr;
}

bool Group::Ptr::operator!=(std::nullptr_t) const noexcept {
    return raw != nullptr;
}

Group::Ptr
  Group::make(GroupConfig config, Context context, Ptr parentGroup, int id) {
    const auto storage = allocate_group();
    const auto group = new (storage)
      Group(std::move(config), context, std::move(parentGroup), id);
    return Ptr(group);
}

Group::Group(GroupConfig config, Context context, Ptr parentGroup, int id)
        : GroupConfig(std::move(config)), context(context),
          parentGroup(std::move(parentGroup)), id(id) {
}

const String& Group::getDescription() const {
    return description;
}

const Context& Group::getContext() const {
    return context;
}

bool Group::isOptional() const {
    if (optional) {
        return true;
    }
    auto* parent = parentGroup.raw;
    while (parent != nullptr) {
        if (parent->optional) {
            return true;
        }
        parent = parent->parentGroup.raw;
    }
    return false;
}

int Group::getId() const {
    return id;
}

GroupPtr Group::getParentGroup() const {
    return parentGroup;
}

bool Group::hasParentGroup() const {
    return parentGroup != nullptr;
}

void Group::addSetUp(Executable func) {
    setUpFuncs.push_back(std::move(func));
}

void Group::addTearDown(Executable func) {
    tearDownsFuncs.push_front(std::move(func));
}

}  // namespace mcga::test
