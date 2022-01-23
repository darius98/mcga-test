#pragma once

namespace mcga::test {

void* allocate_group();
void deallocate_group(void*);

void* allocate_callback();
void deallocate_callback(void*);

}  // namespace mcga::test
