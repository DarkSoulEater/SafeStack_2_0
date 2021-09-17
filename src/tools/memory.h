#ifndef SAFESTACK_2_0_SRC_TOOLS_MEMORY_H_
#define SAFESTACK_2_0_SRC_TOOLS_MEMORY_H_

#include "../global/global.h"

#if !defined(T)
# if defined(DISALLOW_UNDECLARED_TYPE)
#  define T  static_assert(0,
  "Data type of structure is not declared");
# else
#  define T int
# endif
#endif

const size_t kMemorySize = 100;

struct Memory {
  size_t size_ = 0;
  T data_[kMemorySize];
};

static inline T* At_(Memory* memory, size_t ind) {
  ASSERT(memory);
  ASSERT(ind >= 0 && ind < kMemorySize);
  return &memory->data_[0] + ind;
}

static inline bool IsEmpty_(Memory* memory) {
  ASSERT(memory); return memory->size_;
}

static inline size_t Size_(Memory* memory) {
  ASSERT(memory); return memory->size_;
}

static inline void Resize_(Memory* memory, size_t size) {
  ASSERT(false && "error: memory does not support expansion");
}

#endif //SAFESTACK_2_0_SRC_TOOLS_MEMORY_H_
