#pragma once
#include <fcntl.h>
#include <ncurses.h>
#include <netdb.h>
#include <poll.h>
#include <signal.h>
#include <syscall.h>
#include <termcap.h>
#include <termios.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <wait.h>

#include <sys/epoll.h>
#include <sys/inotify.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/signalfd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/timerfd.h>
#include <sys/times.h>
#include <sys/ttychars.h>
#include <sys/ttydefaults.h>
#include <sys/types.h>

#include <arpa/inet.h>

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
