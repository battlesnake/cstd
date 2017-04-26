#if 0
(
set -euo pipefail
declare -r out="$(mktemp)"
trap "rm -f '$out'" EXIT ERR
gcc -Wall -Wextra -Werror -g -DTEST_log -D_GNU_SOURCE -DSIMPLE_LOGGING_THREADSAFE -DSIMPLE_LOGGING_DEBUG -std=gnu11 -o "$out" "$0"
valgrind --leak-check=full --track-origins=yes --quiet "$out"
gcc -Wall -Wextra -Werror -g -DTEST_log -DSIMPLE_LOGGING_DEBUG -std=gnu11 -o "$out" "$0"
valgrind --leak-check=full --track-origins=yes --quiet "$out"
)
exit 0
#endif
#include "std.h"
#include "log.h"

#define LOCATION "\x1b[0m[\x1b[94m%s\x1b[0m@\x1b[36m%s\x1b[0m:\x1b[35m%d\x1b[0m]  "

const char *prog_name = NULL;
int prog_name_trim = -5;

static enum log_level log_level = LOG_LEVEL_INITIAL;

#if defined SIMPLE_LOGGING_THREADSAFE
#include <pthread.h>

static pthread_mutex_t mx;

static void lock()
{
	static bool init = false;
	if (!init) {
		init = true;
		pthread_mutex_init(&mx, NULL);
	}
	pthread_mutex_lock(&mx);
}

static void unlock()
{
	pthread_mutex_unlock(&mx);
}
#else
#define lock()
#define unlock()
#endif

enum log_level log_level_set(enum log_level mask)
{
	lock();
	enum log_level prev = log_level;
	log_level = mask;
	unlock();
	return prev;
}

enum log_level log_level_include(enum log_level mask)
{
	lock();
	enum log_level prev = log_level;
	log_level |= mask;
	unlock();
	return prev;
}

enum log_level log_level_exclude(enum log_level mask)
{
	lock();
	enum log_level prev = log_level;
	log_level &= ~mask;
	unlock();
	return prev;
}

enum log_level log_level_get()
{
	lock();
	enum log_level result = log_level;
	unlock();
	return result;
}

static void _log(enum log_level level, const char *template, const char *file, int line, const char *func, const char *format, va_list args)
{
	va_list args2;
	va_copy(args2, args);
	int msglen = vsnprintf(NULL, 0, format, args);
	char msgbuf[msglen + 1];
	vsprintf(msgbuf, format, args2);
	va_end(args2);

	const char *progfmt;
	int proglen;
	if (prog_name) {
		if (prog_name_trim < 0) {
			progfmt = "[%.*s]  ";
			proglen = snprintf(NULL, 0, progfmt, -prog_name_trim, prog_name);
		} else if (prog_name_trim >= 0) {
			progfmt = "[%*s]  ";
			proglen = snprintf(NULL, 0, progfmt, prog_name_trim, prog_name);
		}
	} else {
		proglen = 0;
	}

	const char *locfmt = LOCATION;
	const int loclen = snprintf(NULL, 0, locfmt, func, file, line);

	const char *tmpfmt = template;
	const int tmplen = snprintf(NULL, 0, tmpfmt, msgbuf);

	char buf[2 + proglen + loclen + tmplen + 1 + 1];
	char *p = buf;

	if (prog_name) {
		if (prog_name_trim < 0) {
			p += sprintf(p, progfmt, -prog_name_trim, prog_name);
		} else if (prog_name_trim >= 0) {
			p += sprintf(p, progfmt, prog_name_trim, prog_name);
		}
	} else {
		p += sprintf(p, "  ");
	}

	p += sprintf(p, locfmt, func, file, line);

	p += sprintf(p, tmpfmt, msgbuf);

	p += sprintf(p, "\n");

	lock();
	if (log_level & level) {
		fprintf(stderr, "%s", buf);
	}
	unlock();
}

void _log_info(const char *file, int line, const char *func, const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	_log(ll_only_info, "%s", file, line, func, format, ap);
	va_end(ap);
}

void _log_warn(const char *file, int line, const char *func, const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	_log(ll_only_warn, "\x1b[1;33m%s\x1b[0m", file, line, func, format, ap);
	va_end(ap);
}

void _log_error(const char *file, int line, const char *func, const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	_log(ll_only_error, "\x1b[1;31m%s\x1b[0m", file, line, func, format, ap);
	va_end(ap);
}

#if defined SIMPLE_LOGGING_DEBUG
void _log_debug(const char *file, int line, const char *func, const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	_log(ll_only_debug, "\x1b[2;4m%s\x1b[0m", file, line, func, format, ap);
	va_end(ap);
}
#endif

void _log_sysfail(const char *file, int line, const char *func, const char *syscall, const char *format, ...)
{
	char fmtbuf[100];
#if (_POSIX_C_SOURCE >= 200112L) && ! defined _GNU_SOURCE
	const char *errstr = strerror(errno);
#else
	char errbuf[100];
	const char *errstr = strerror_r(errno, errbuf, sizeof(errbuf));
#endif
	/* BUG: syscall & strerror(errno) may not contain % char - not enforced */
	snprintf(fmtbuf, sizeof(fmtbuf), "%s(%s) failed with errno=%d (%s)", syscall, format, errno, errstr);
	va_list ap;
	va_start(ap, format);
	_log(ll_only_sysfail, "\x1b[1;2;31m%s\x1b[0m", file, line, func, fmtbuf, ap);
	va_end(ap);
}

#if defined TEST_log
int main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;
	log_info("This is some %s.", "information");
	log_warn("This is a %s.", "warning");
	log_error("This is an %s.", "error");
	log_debug("This is a %s.", "debug message");
	log_sysfail("exit", "%d", 0);
}
#endif
