
#ifndef SAFESTACK_2_0_SRC_GLOBAL_GLOBAL_H_
#define SAFESTACK_2_0_SRC_GLOBAL_GLOBAL_H_

#include "config.h"

void assert_(const char* assertion, const char* file, int line) noexcept;

#if !defined(ASSERT)
# if defined(NO_DEBUG) && !defined(FORCE_ASSERTS)
#   define ASSERT(cond) ;
# else
#   define ASSERT(cond) ((cond) ? static_cast<void>(0) : assert_(#cond, __FILE__, __LINE__))
# endif
#endif

void assert_x(const char* where, const char* what, const char* file, int line) noexcept;

#if !defined(ASSERT_X)
# if defined(NO_DEBUG) && !defined(FORCE_ASSERTS)
#    define ASSERT_X(cond, where, what) ;
# else
#    define ASSERT_X(cond, where, what) ((cond) ? static_cast<void>(0) : assert_x(where, what, __FILE__, __LINE__))
# endif
#endif

// --------------------------------------------------------- //

enum Poison {
  eNull,
  eCanary
};

#endif //SAFESTACK_2_0_SRC_GLOBAL_GLOBAL_H_
