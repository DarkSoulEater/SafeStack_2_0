#include <stdlib.h>

#include "../global/global.h"
#include "../global/logging.h"
#include "../global/win.h"

#include "long_array.h"

struct Page {
  void* data;
};

struct LongArray_ {
#if defined(LONG_ARRAY_CANARY_VALIDATION) && !defined(NO_DEBUG)
  void* canary_0 = (void*)Poison::eCanary;
#endif

  size_t page_size_ = 0;
  size_t page_cnt_  = 0;
  size_t elem_size_ = 0;
  size_t size_      = 0;

  Page* pages_ = nullptr;

#if defined(LONG_ARRAY_CANARY_VALIDATION) && !defined(NO_DEBUG)
  void* canary_1 = (void*)Poison::eCanary;
#endif

#if defined(LONG_ARRAY_HASH_VALIDATION) && !defined(NO_DEBUG)
  size_t hash_ = 0;
#endif
};

// -------------Private------------ //

static void PageReallocate(LongArray_ *array, size_t page_cnt) {
  ASSERT(array);
  ASSERT(page_cnt > 0);

  Page *pages = (Page*) calloc(page_cnt, sizeof (Page));
  if (!pages) {
    MessageLogger(MsgType::eFatal, "error: not enough memory to allocate page array. Requested volume: ", page_cnt * sizeof (Page));
  }

  for (size_t i = 0; i < array->page_cnt_; ++i) {
    pages[i].data = array->pages_[i].data;
  }

  free(array->pages_);
  array->pages_ = pages;
  array->page_cnt_ = page_cnt;
}

static inline LongArray HeaderEncode(LongArray_* array) {
  return (LongArray)(array - 1000);
}

static inline LongArray_* HeaderDecode(LongArray handle) {
  return (LongArray_*)handle + 1000;
}

// -------------Debug-------------- //

#define MSG_VALIDATION_TYPE MsgType::eValidation

#define SHOW_ALL_PAGES

#if (defined(NO_DEBUG) && !defined(FORCE_STRUCT_VALIDATION)) || defined(NOT_FORCE_LONG_ARRAY)
# define LONG_ARRAY_VALID(ptr) ;
#else
# define LONG_ARRAY_VALID(ptr) MessageLogger(MSG_VALIDATION_TYPE, "LongArray \"%s\" from %s(), line %d", #ptr, __FUNCTION__, __LINE__); \
                          LongArray_Valid(ptr, 1);                                                                        \
                          MessageLogger(MSG_VALIDATION_TYPE, "%s:%d", __FILE__, __LINE__);                                 \
                          MessageLogger(MSG_VALIDATION_TYPE, "-----------------------------------------------------------------\n");
#endif

static void LongArray_Valid(LongArray_* array, size_t indent) {
  MessageLogger(MSG_VALIDATION_TYPE, "%tLongArray [%p] {", indent, array);

  if (MemoryProtect(array)) {
    MessageLogger(MSG_VALIDATION_TYPE, "%tInvalid pointer to long array: %p", indent, array);
    goto END_VALIDATION;
  }

#if defined(LONG_ARRAY_CANARY_VALIDATION) && !defined(NO_DEBUG)
  MessageLogger(MSG_VALIDATION_TYPE, "%tcanary_0 = %p (%s)", indent + 1, array->canary_0, (array->canary_0 == (void*)Poison::eCanary ? "ok" : "ERROR"));
#endif

  MessageLogger(MSG_VALIDATION_TYPE, "");
  MessageLogger(MSG_VALIDATION_TYPE, "%t.page_size_ = %d (%s)",  indent + 1, array->page_size_, (array->page_size_  > 0  ? "ok" : "ERROR"));
  MessageLogger(MSG_VALIDATION_TYPE, "%t.page_cnt_  = %d (%s)",  indent + 1, array->page_cnt_ , (array->page_cnt_   >= 0 ? "ok" : "ERROR"));
  MessageLogger(MSG_VALIDATION_TYPE, "%t.elem_size  = %d (%s)",  indent + 1, array->elem_size_, (array->elem_size_  > 0  ? "ok" : "ERROR"));
  MessageLogger(MSG_VALIDATION_TYPE, "%t.size_      = %d (%s)",       indent + 1, array->size_     , (array->size_       >= 0 ? "ok" : "ERROR"));

  MessageLogger(MSG_VALIDATION_TYPE, "\n%t.pages_ [%p] %n",   indent + 1, array->pages_);

  if (!array->pages_) {
    MessageLogger(MSG_VALIDATION_TYPE, "= nullptr");
    goto SKIP_PAGES_VALIDATION;
  }

  if (MemoryProtect(array->pages_)) {
    MessageLogger(MSG_VALIDATION_TYPE, "{\n%tInvalid pointer to pages: %p\n}", indent + 2, array->pages_);
    goto SKIP_PAGES_VALIDATION;
  }

#if defined(SHOW_ALL_PAGES)
  MessageLogger(MSG_VALIDATION_TYPE, "{");

  for (size_t i = 0; i < array->page_cnt_; ++i) {
    MessageLogger(MSG_VALIDATION_TYPE, "%t[%d] %n", indent + 2, i);
    if (array->pages_[i].data) {
      MessageLogger(MSG_VALIDATION_TYPE, "{");

      if (MemoryProtect(array->pages_[i].data)) {
        MessageLogger(MSG_VALIDATION_TYPE, "%tInvalid pointer to page data: %p", indent + 3, array->pages_[i].data);
      } else if (array->elem_size_ != sizeof(int)) {
        MessageLogger(MSG_VALIDATION_TYPE, "%tUnknown type elements of array", indent + 3);
      } else {
        for (int j = 0; j < array->page_size_; ++j) {
          MessageLogger(MSG_VALIDATION_TYPE, "%t[%d] = %d", indent + 3, j, ((int*)(array->pages_[i].data))[j]);
          if (i * array->page_size_ + j + 1 == array->size_) {
            MessageLogger(MSG_VALIDATION_TYPE, "%t-----SIZE-----", indent + 4);
          }
        }
      }

      MessageLogger(MSG_VALIDATION_TYPE, "%t}", indent + 2);
    } else {
      MessageLogger(MSG_VALIDATION_TYPE, "= nullptr", indent);
    }
  }

  MessageLogger(MSG_VALIDATION_TYPE, "%t}", indent + 1);
#else
  MessageLogger(MSG_VALIDATION_TYPE, "");
#endif

  MessageLogger(MSG_VALIDATION_TYPE, "");

  SKIP_PAGES_VALIDATION:{};

#if defined(LONG_ARRAY_CANARY_VALIDATION) && !defined(NO_DEBUG)
  MessageLogger(MSG_VALIDATION_TYPE, "%tcanary_1 = %p (%s)", indent + 1, array->canary_1, (array->canary_1 == (void*)Poison::eCanary ? "ok" : "ERROR"));
#endif

#if defined(LONG_ARRAY_HASH_VALIDATION) && !defined(NO_DEBUG)
  // TODO: Проверка Хеша
#endif

  END_VALIDATION:{};
  MessageLogger(MSG_VALIDATION_TYPE, "%t}", indent);
}

void LongArray_Valid_(LongArray _handle, size_t indent) {
  LongArray_ *array = HeaderDecode(_handle);
  LongArray_Valid(array, indent);
}

// -------------Public------------- //

#define GET_ARRAY_PTR(handle) LongArray_ *array = HeaderDecode(handle);

void* LongArray_At(LongArray _handle, size_t i) {
  GET_ARRAY_PTR(_handle);
  LONG_ARRAY_VALID(array);

  if (i < 0) {
    MessageLogger(MsgType::eFatal, "Index less than 0. Array size: %d. i: %d", array->size_, i); // TODO: Можно сделать вариантивность, а не просто критическую ошибку
  }

  if (i >= array->size_) {
    MessageLogger(MsgType::eFatal, "Going out of bounds long array. Array size: %d. i: %d", array->size_, i); // TODO: Можно сделать вариантивность, а не просто критическую ошибку
  }

  size_t page_ind = i / array->page_size_;
  if (array->pages_[page_ind].data == nullptr) {
    array->pages_[page_ind].data = calloc(array->page_size_, array->elem_size_);

    if (array->pages_[page_ind].data == nullptr) {
      MessageLogger(MsgType::eFatal, "Failed to allocate memory for long array page");
    }
  }

  return (char *)array->pages_[page_ind].data + (i - page_ind * array->page_size_) * array->elem_size_;
}

size_t LongArray_Size(LongArray _handle) {
  GET_ARRAY_PTR(_handle);
  LONG_ARRAY_VALID(array);
  return array->size_;
}

void LongArray_Resize(LongArray _handle, size_t size) {
  GET_ARRAY_PTR(_handle);
  LONG_ARRAY_VALID(array);

  if (size <= array->size_) {
    array->size_ = size;
    return;
  }

  if (array->page_cnt_ * array->page_size_ < size) {
    PageReallocate(array, size / array->page_size_ + 1);
  }

  array->size_ = size;
  LONG_ARRAY_VALID(array);
}

bool LongArray_Empty(LongArray _handle) {
  GET_ARRAY_PTR(_handle);
  LONG_ARRAY_VALID(array);
  return !array->size_;
}

void LongArray_Clear(LongArray _handle) {
  LongArray_Resize(_handle, 0);
}

LongArray LongArray_Allocate (size_t elem_size, size_t page_size) {
  LongArray_* array = (LongArray_*) calloc(1, sizeof(LongArray_));
  if (!array) {
    MessageLogger(MsgType::eFatal, "Failed to allocate memory for long array");
  }

  array->page_size_ = page_size;
  array->elem_size_ = elem_size;

#if defined(LONG_ARRAY_CANARY_VALIDATION) && !defined(NO_DEBUG)
  array->canary_0 = (void*)Poison::eCanary;
  array->canary_1 = (void*)Poison::eCanary;
#endif

#if defined(LONG_ARRAY_HASH_VALIDATION) && !defined(NO_DEBUG)
  array->hash_ = 0; // TODO: реалиовать хеш (проинцилизировать хеш)
#endif

  return HeaderEncode(array);
}

void LongArray_Deallocate(LongArray _handle) {
  GET_ARRAY_PTR(_handle);
  LONG_ARRAY_VALID(array);
  // TODO: Написать очистку массива
}

