#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "config.h"
#include "logging.h"

static FILE* debug_file      = nullptr;
static FILE* warning_file    = nullptr;
static FILE* info_file       = nullptr;
static FILE* fatal_file      = nullptr;
static FILE* validation_file = nullptr;

void MessageLogger(MsgType msg_type, const char *msg, ...) noexcept {
  FILE *out_file = stderr;
  switch (msg_type) {
    case MsgType::eDebug:      out_file = debug_file;      break;
    case MsgType::eWarning:    out_file = warning_file;    break;
    case MsgType::eInfo:       out_file = info_file;       break;
    case MsgType::eFatal:      out_file = fatal_file;      break;
    case MsgType::eValidation: out_file = validation_file; break;
  }

  va_list factor;
  va_start(factor, msg);
  stderr;
  __acrt_iob_func(2);

  bool line_break = true;

  for (const char *c = msg; *c; ++c) {
    if (*c != '%') {
      putc(*c, out_file);
      continue;
    }

    switch (*++c) {
      case 'd': {
        int d = va_arg(factor, int);
        fprintf(out_file, "%d", d);
        break;
      }
      case 's': {
        const char* str = va_arg(factor, const char*);
        fprintf(out_file, "%s", str);
        break;
      }
      case 'p': {
        const void* ptr = va_arg(factor, const void*);
        fprintf(out_file, "%p", ptr);
        break;
      }
      case 't': {
        int t = va_arg(factor, int);
        for (int i = 0; i < t; ++i) {
          fprintf(out_file, "  ");
        }
        break;
      }
      case 'n': {
        line_break = false;
        break;
      }
      default:
        fputs("\nerror: an incorrect message was sent to MessageLogger\n", out_file);
    }
  }

  if (line_break) fputc('\n', out_file);

  if (msg_type == MsgType::eFatal) {
    exit(1);
  }
}

void OpenMessageFiles() {
#if defined(DEBUG_MESSAGE_PATH)
  fopen_s(&debug_file, DEBUG_MESSAGE_PATH, "w");
#else
  debug_file = stdout;
#endif

#if defined(WARNING_MESSAGE_PATH)
  fopen_s(&warning_file, WARNING_MESSAGE_PATH, "w");
#else
  warning_file = stdout;
#endif

#if defined(INFO_MESSAGE_PATH)
  fopen_s(&info_file, INFO_MESSAGE_PATH, "w");
#else
  debug_file = stdout;
#endif

#if defined(FATAL_MESSAGE_PATH)
  fopen_s(&fatal_file, FATAL_MESSAGE_PATH, "w");
#else
  fatal_file = stderr;
#endif

#if defined(VALIDATION_MESSAGE_PATH)
  fopen_s(&validation_file, VALIDATION_MESSAGE_PATH, "w");
#else
  validation_file = stdout;
#endif
}

void CloseMessageFiles() {
#if defined(DEBUG_MESSAGE_PATH)
  fclose(debug_file);
#endif

#if defined(WARNING_MESSAGE_PATH)
  fclose(warning_file);
#endif

#if defined(INFO_MESSAGE_PATH)
  fclose(info_file);
#endif

#if defined(FATAL_MESSAGE_PATH)
  fclose(fatal_file);
#endif

#if defined(VALIDATION_MESSAGE_PATH)
  fclose(validation_file);
#endif
}