#ifndef SAFESTACK_2_0_SRC_TOOLS_LONG_ARRAY_H_
#define SAFESTACK_2_0_SRC_TOOLS_LONG_ARRAY_H_

const size_t kDefaultPageSize = 8;

typedef void* LongArray;

void*  LongArray_At     (LongArray _handle, size_t i);
size_t LongArray_Size   (LongArray _handle);
void   LongArray_Resize (LongArray _handle, size_t size);

bool   LongArray_Empty  (LongArray _handle);
void   LongArray_Clear  (LongArray _handle);

LongArray LongArray_Allocate  (size_t elem_size, size_t page_size = kDefaultPageSize);
void      LongArray_Deallocate(LongArray _handle);


#if !defined(DISALLOW_SORT_FUNC_NAME)
  inline void*  At    (LongArray _handle, size_t i)    { return LongArray_At(_handle, i);        }
  inline size_t Size  (LongArray _handle)              { return LongArray_Size(_handle);         }
  inline void   Resize(LongArray _handle, size_t size) { return LongArray_Resize(_handle, size); }
  inline bool   Empty (LongArray _handle)              { return LongArray_Empty(_handle);        }
  inline void   Clear (LongArray _handle)              { return LongArray_Clear(_handle);        }
#endif

#endif //SAFESTACK_2_0_SRC_TOOLS_LONG_ARRAY_H_
