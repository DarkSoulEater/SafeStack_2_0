#include <Windows.h>
#include "win.h"

int MemoryProtect(const void *memory_ptr) {
  MEMORY_BASIC_INFORMATION info = {};

  VirtualQuery(memory_ptr, &info, sizeof(info));
  return info.AllocationProtect != PAGE_READWRITE;
}