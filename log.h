#pragma once
#include "std.h"

extern const char *prog_name;
extern int prog_name_trim;

extern void _log_info(const char *file, int line, const char *func, const char *format, ...);
extern void _log_warn(const char *file, int line, const char *func, const char *format, ...);
extern void _log_error(const char *file, int line, const char *func, const char *format, ...);

#define log_info(format, ...) _log_info(__FILE__, __LINE__, __func__, format, ##__VA_ARGS__);
#define log_warn(format, ...) _log_warn(__FILE__, __LINE__, __func__, format, ##__VA_ARGS__);
#define log_error(format, ...) _log_error(__FILE__, __LINE__, __func__, format, ##__VA_ARGS__);

#define log_sysfail(syscall, format, ...) _log_error(__FILE__, __LINE__, __func__, syscall "(" format ") failed (%d)", ##__VA_ARGS__, errno);

#if defined SIMPLE_LOGGING_DEBUG
extern void _log_debug(const char *file, int line, const char *func, const char *format, ...);
#define log_debug(format, ...) _log_debug(__FILE__, __LINE__, __func__, format, ##__VA_ARGS__);
#else
#define log_debug(format, ...) do { } while (0)
#endif
