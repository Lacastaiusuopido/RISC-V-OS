#ifndef _SYSCALL_H_
#define _SYSCALL_H_

#include "types.h"

// 系统调用号
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

// 处理系统调用
uint64 syscall(uint64 syscall_num, uint64 a0, uint64 a1, uint64 a2, uint64 a3, uint64 a4, uint64 a5);

#endif // _SYSCALL_H_