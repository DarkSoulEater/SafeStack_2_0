#include "stdio.h"
#include "logging.h"

void assert_(const char* assertion, const char* file, int line) noexcept {
  MessageLogger(MsgType::eFatal, "ASSERT: \"%s\" in %s:%d\n", assertion, file, line);
}

void assert_x(const char* where, const char* what, const char* file, int line) noexcept {
  MessageLogger(MsgType::eFatal, "ASSERT failure in %s: \"%s\", file %s, line %d", where, what, file, line);
}
