
#ifndef SAFESTACK_2_0_SRC_TOOLS_STACK_H_
#define SAFESTACK_2_0_SRC_TOOLS_STACK_H_

#include <stdlib.h>

#include "../global/win.h"
#include "../global/logging.h"
#include "../global/global.h"

#include "long_array.h"

#if !defined(T)
# if defined(DISALLOW_UNDECLARED_TYPE)
#  define T  static_assert(0, "Data type of structure is not declared");
# else
#  define T int
# endif
#endif

struct Stack {
#if defined(STACK_CANARY_VALIDATION) && !defined(NO_DEBUG)
  void* canary_0 = (void*)Poison::eCanary;
#endif

  LongArray data_ = {};

#if defined(STACK_CANARY_VALIDATION) && !defined(NO_DEBUG)
  void* canary_1 = (void*)Poison::eCanary;
#endif

#if defined(STACK_HASH_VALIDATION) && !defined(NO_DEBUG)
  size_t hash_ = 0;
#endif
};

// -------------Private------------ //



// -------------Debug-------------- //

#define MSG_VALIDATION_TYPE MsgType::eValidation

#if defined(NDEBUD) && !defined(FORCE_STRUCT_VALIDATION)
# define STACK_VALID(struct) ;
#else
# define STACK_VALID(ptr) MessageLogger(MSG_VALIDATION_TYPE, "Stack \"%s\" from %s(), line %d", #ptr, __FUNCTION__, __LINE__); \
                          Stack_Validation(ptr, 1);                                                                        \
                          MessageLogger(MSG_VALIDATION_TYPE, "%s:%d", __FILE__, __LINE__);                                 \
                          MessageLogger(MSG_VALIDATION_TYPE, "-----------------------------------------------------------------\n");
#endif

#if defined(STACK_FORCE_DATA)
  void LongArray_Valid_(LongArray _handle, size_t indent);
#endif

static void Stack_Validation(Stack* stack, size_t indent) {
  MessageLogger(MSG_VALIDATION_TYPE, "%tStack [%p] {", indent, stack);

  if (MemoryProtect(stack)) {
    MessageLogger(MSG_VALIDATION_TYPE, "%tInvalid pointer to stack: %p", indent, stack);
    goto END_VALIDATION;
  }

#if defined(STACK_CANARY_VALIDATION) && !defined(NO_DEBUG)
  MessageLogger(MSG_VALIDATION_TYPE, "%tcanary_0 = %p (%s)", indent + 1, stack->canary_0, (stack->canary_0 == (void*)Poison::eCanary ? "ok" : "ERROR"));
#endif

  MessageLogger(MSG_VALIDATION_TYPE, "");
  MessageLogger(MSG_VALIDATION_TYPE, "%t.data [%p] %n", indent + 1, stack->data_);
  if (stack->data_ == nullptr) {
    MessageLogger(MSG_VALIDATION_TYPE, "= nullptr");
  } else {
    MessageLogger(MSG_VALIDATION_TYPE, "{");
    LongArray_Valid_(stack->data_, indent + 2);

    MessageLogger(MSG_VALIDATION_TYPE, "%t}", indent + 1);
  }

  MessageLogger(MSG_VALIDATION_TYPE, "");
#if defined(STACK_CANARY_VALIDATION) && !defined(NO_DEBUG)
  MessageLogger(MSG_VALIDATION_TYPE, "%tcanary_1 = %p (%s)", indent + 1, stack->canary_1, (stack->canary_1 == (void*)Poison::eCanary ? "ok" : "ERROR"));
#endif

#if defined(STACK_HASH_VALIDATION) && !defined(NO_DEBUG)
  // TODO: сделать хеширование (проверка хеша)
#endif

  END_VALIDATION:{};
  MessageLogger(MSG_VALIDATION_TYPE, "%t}", indent);
}

// -------------Public------------- //

static Stack* StackAllocate() {
  Stack* stack = (Stack*) calloc(1, sizeof (Stack));
  if (!stack) {
    MessageLogger(MsgType::eFatal, "Failed to allocate memory for stack");
  }

#if defined(STACK_CANARY_VALIDATION) && !defined(NO_DEBUG)
  stack->canary_0 = (void*)Poison::eCanary;
  stack->canary_1 = (void*)Poison::eCanary;
#endif

#if defined(STACK_HASH_VALIDATION) && !defined(NO_DEBUG)
  stack->hash_ = 0; // TODO: сделать хеширование (инициализация хеша)
#endif

  stack->data_ = LongArray_Allocate(sizeof(T), 8);
  STACK_VALID(stack);
  return stack;
}

static inline size_t StackSize(Stack* stack) {
  STACK_VALID(stack);
  ASSERT(stack);

  return Size(stack->data_);
}

static inline void StackPush(Stack* stack, T value) {
  STACK_VALID(stack);
  ASSERT(stack);

  Resize( stack->data_ ,StackSize(stack) + 1);
  *(T*)At(stack->data_,   StackSize(stack) - 1) = value;

  STACK_VALID(stack);
}

static inline T StackPop(Stack *stack) {
  STACK_VALID(stack);
  ASSERT(Empty(stack->data_));

  T t = *(T*)At(stack->data_, StackSize(stack) - 1);
  Resize(stack->data_, StackSize(stack) - 1);

  STACK_VALID(stack);

  return t;
}

static inline T StackTop(Stack *stack) {
  STACK_VALID(stack);
  ASSERT(Empty(stack->data_));

  return *(T*)At(stack->data_, StackSize(stack) - 1);
}

#undef T

#endif //SAFESTACK_2_0_SRC_TOOLS_STACK_H_
