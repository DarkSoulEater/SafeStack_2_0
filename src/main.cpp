#include "tools/stack.h"
#include "tools/long_array.h"

#define INT *(int*)

int main(int arc, const char **arv) {
  OpenMessageFiles();

  Stack* stack = StackAllocate();
  StackPush(stack, 1);
  StackPush(stack, 2);
  StackPush(stack, 3);
  StackPush(stack, 3);
  StackPush(stack, 3);
  StackPush(stack, 3);
  StackPush(stack, 3);
  StackPush(stack, 3);
  StackPush(stack, 3);
  StackPush(stack, 3);
  StackPush(stack, 3);
  StackPush(stack, 3);
  StackPush(stack, 3);
  StackPush(stack, 3);
  StackPush(stack, 3);
  return 0;

  LongArray array = LongArray_Allocate(sizeof (int), 8);
  LongArray_Resize(array, 30);

  INT LongArray_At(array, 15) = 10;
  INT LongArray_At(array, 16) = 3;
  INT LongArray_At(array, 1) = 2;

  LongArray_Resize(array, 15);
  INT LongArray_At(array, 2) = -10;

  Clear(array);
  LongArray_Deallocate(array);

  return 0;
}
