#pragma once

extern const char *prog_name;
extern int prog_name_trim;

enum log_level
{
	ll_only_error = 0x01,
	ll_only_warn = 0x02,
	ll_only_info = 0x04,
	ll_only_debug = 0x08,
	ll_only_sysfail = 0x10,

	ll_error = ll_only_error,
	ll_warn = ll_error | ll_only_warn | ll_only_sysfail,
	ll_info = ll_warn | ll_only_info,
	ll_debug = ll_info | ll_only_debug
};

#if ! defined LOG_LEVEL_INITIAL
#define LOG_LEVEL_INITIAL ((enum log_level) -1)
#endif

/* Returns previous level */
enum log_level log_level_set(enum log_level mask);
enum log_level log_level_include(enum log_level mask);
enum log_level log_level_exclude(enum log_level mask);
enum log_level log_level_get();

extern void _log_info(const char *file, int line, const char *func, const char *format, ...);
extern void _log_warn(const char *file, int line, const char *func, const char *format, ...);
extern void _log_error(const char *file, int line, const char *func, const char *format, ...);
extern void _log_sysfail(const char *file, int line, const char *func, const char *syscall, const char *format, ...);

#define log_info(format, ...) _log_info(__FILE__, __LINE__, __func__, format, ##__VA_ARGS__)
#define log_warn(format, ...) _log_warn(__FILE__, __LINE__, __func__, format, ##__VA_ARGS__)
#define log_error(format, ...) _log_error(__FILE__, __LINE__, __func__, format, ##__VA_ARGS__)

#define log_sysfail(syscall, format, ...) _log_sysfail(__FILE__, __LINE__, __func__, syscall, format, ##__VA_ARGS__)

#if defined SIMPLE_LOGGING_DEBUG
extern void _log_debug(const char *file, int line, const char *func, const char *format, ...);
#define log_debug(format, ...) _log_debug(__FILE__, __LINE__, __func__, format, ##__VA_ARGS__);
#else
#define log_debug(format, ...) do { } while (0)
#endif
