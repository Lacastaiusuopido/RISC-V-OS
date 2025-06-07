#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include "types.h"

// 基础函数声明
void console_init(void);
void console_putc(char c);
void console_puts(const char *s);
int console_getc(void);
int console_printf(const char *fmt, ...);
int console_vprintf(const char *fmt, va_list args);
void console_test(void);
int console_printf_main(const char *fmt, ...);

/* ========== 日志模块定义 ========== */
#define DECLARE_LOG_MODULE(name) \
    void console_printf_##name(const char *fmt, ...);

DECLARE_LOG_MODULE(MAIN)
DECLARE_LOG_MODULE(TRAP)
DECLARE_LOG_MODULE(SYSCALL)
DECLARE_LOG_MODULE(TIMER)
DECLARE_LOG_MODULE(PAGE)
DECLARE_LOG_MODULE(QEMU)
DECLARE_LOG_MODULE(PANIC)

#endif // _CONSOLE_H_