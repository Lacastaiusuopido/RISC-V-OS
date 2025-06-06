#ifndef _ULIB_H_
#define _ULIB_H_

#include "../include/types.h"

// 系统调用号 - 与内核定义匹配
#define SYS_write      1
#define SYS_exit       2
#define SYS_getpid     3
#define SYS_sleep      4
#define SYS_yield      5
#define SYS_time       6
#define SYS_exec       7
#define SYS_fork       8
#define SYS_wait       9
#define SYS_open       10
#define SYS_close      11
#define SYS_read       12

// 系统调用函数声明
int write(int fd, const void *buf, size_t count);
void exit(int status) __attribute__((noreturn));
int getpid(void);
void sleep(uint64 milliseconds);
void yield(void);
uint64 time(void);
int exec(const char *path, char *const argv[]);
int fork(void);
int wait(int *status);
int open(const char *path, int flags);
int close(int fd);
int read(int fd, void *buf, size_t count);

// 库函数声明
void puts(const char *s);
size_t strlen(const char *s);
int printf(const char *fmt, ...);

#endif // _ULIB_H_