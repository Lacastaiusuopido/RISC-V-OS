#include "ulib.h"

// 执行系统调用
static inline uint64 syscall(uint64 syscall_num, uint64 a0, uint64 a1, uint64 a2, uint64 a3, uint64 a4, uint64 a5) {
    uint64 ret;
    asm volatile (
        "mv a7, %1\n"
        "mv a0, %2\n"
        "mv a1, %3\n"
        "mv a2, %4\n"
        "mv a3, %5\n"
        "mv a4, %6\n"
        "mv a5, %7\n"
        "ecall\n"
        "mv %0, a0"
        : "=r" (ret)
        : "r" (syscall_num), "r" (a0), "r" (a1), "r" (a2), "r" (a3), "r" (a4), "r" (a5)
        : "a0", "a1", "a2", "a3", "a4", "a5", "a7"
    );
    return ret;
}

// 写入系统调用
int write(int fd, const void *buf, size_t count) {
    return syscall(SYS_write, fd, (uint64)buf, count, 0, 0, 0);
}

// 退出系统调用
void exit(int status) {
    syscall(SYS_exit, status, 0, 0, 0, 0, 0);
    while(1); // 防止返回
}

// 获取进程ID系统调用
int getpid(void) {
    return syscall(SYS_getpid, 0, 0, 0, 0, 0, 0);
}

// 睡眠系统调用
void sleep(uint64 milliseconds) {
    syscall(SYS_sleep, milliseconds, 0, 0, 0, 0, 0);
}

// 让出CPU系统调用
void yield(void) {
    syscall(SYS_yield, 0, 0, 0, 0, 0, 0);
}

// 获取系统时间系统调用
uint64 time(void) {
    return syscall(SYS_time, 0, 0, 0, 0, 0, 0);
}

// 执行程序系统调用
int exec(const char *path, char *const argv[]) {
    return syscall(SYS_exec, (uint64)path, (uint64)argv, 0, 0, 0, 0);
}

// 创建子进程系统调用
int fork(void) {
    return syscall(SYS_fork, 0, 0, 0, 0, 0, 0);
}

// 等待子进程系统调用
int wait(int *status) {
    return syscall(SYS_wait, (uint64)status, 0, 0, 0, 0, 0);
}

// 打开文件系统调用
int open(const char *path, int flags) {
    return syscall(SYS_open, (uint64)path, flags, 0, 0, 0, 0);
}

// 关闭文件系统调用
int close(int fd) {
    return syscall(SYS_close, fd, 0, 0, 0, 0, 0);
}

// 读取文件系统调用
int read(int fd, void *buf, size_t count) {
    return syscall(SYS_read, fd, (uint64)buf, count, 0, 0, 0);
}

// 输出字符串
void puts(const char *s) {
    write(1, s, strlen(s));
}

// 计算字符串长度
size_t strlen(const char *s) {
    size_t len = 0;
    while(s[len] != '\0') {
        len++;
    }
    return len;
}

// 将整数转换为字符串
static char* itoa(int value, char* result, int base) {
    // 检查base是否支持
    if (base < 2 || base > 36) {
        *result = '\0';
        return result;
    }

    char* ptr = result, *ptr1 = result;
    char tmp_char;
    int tmp_value;

    // 处理负数
    if (value < 0 && base == 10) {
        *ptr++ = '-';
        value = -value;
    }

    // 处理0
    if (value == 0) {
        *ptr++ = '0';
        *ptr = '\0';
        return result;
    }

    // 转换整数为字符串
    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "0123456789abcdefghijklmnopqrstuvwxyz"[tmp_value - value * base];
    } while (value);

    // 添加字符串结束符
    *ptr-- = '\0';

    // 反转字符串
    if (*result == '-') {
        ptr1++;
    }
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }

    return result;
}

// 格式化输出
int printf(const char *fmt, ...) {
    char c;
    int count = 0;
    char buf[24];
    va_list args;
    
    va_start(args, fmt);
    
    // 创建一个缓冲区来收集普通字符
    char output_buf[256];
    int buf_index = 0;
    
    while ((c = *fmt++)) {
        if (c != '%') {
            // 将普通字符添加到缓冲区
            output_buf[buf_index++] = c;
            count++;
            
            // 如果缓冲区快满了，先输出
            if (buf_index >= 255) {
                write(1, output_buf, buf_index);
                buf_index = 0;
            }
            continue;
        }
        
        // 遇到格式说明符，先输出之前收集的普通字符
        if (buf_index > 0) {
            write(1, output_buf, buf_index);
            buf_index = 0;
        }
        
        // 处理格式说明符
        c = *fmt++;
        if (!c) break;
        
        // 处理宽度和填充
        int width = 0;
        int zero_padding = 0;
        
        if (c == '0') {
            zero_padding = 1;
            c = *fmt++;
            if (!c) break;
        }
        
        // 解析宽度
        while (c >= '0' && c <= '9') {
            width = width * 10 + (c - '0');
            c = *fmt++;
            if (!c) break;
        }
        
        // 检查长整数标志
        int is_long = 0;
        if (c == 'l') {
            is_long = 1;
            c = *fmt++;
            if (!c) break;
        }
        
        switch (c) {
            case 'c': {
                // 字符
                char ch = (char)va_arg(args, int);
                char tmp[2] = {ch, '\0'};
                write(1, tmp, 1);
                count++;
                break;
            }
            
            case 's': {
                // 字符串
                char* s = va_arg(args, char*);
                if (!s) s = "(null)";
                int len = strlen(s);
                
                // 处理宽度填充
                if (width > len && !zero_padding) {
                    char space = ' ';
                    for (int i = 0; i < width - len; i++) {
                        write(1, &space, 1);
                        count++;
                    }
                }
                
                write(1, s, len);
                count += len;
                break;
            }
            
            case 'd': {
                // 有符号十进制整数
                if (is_long) {
                    long value = va_arg(args, long);
                    if (value < 0) {
                        write(1, "-", 1);
                        count++;
                        value = -value;
                    }
                    // 使用无符号长整型转换
                    char num_buf[24];
                    int i = 0;
                    unsigned long uvalue = (unsigned long)value;
                    do {
                        num_buf[i++] = '0' + (uvalue % 10);
                        uvalue /= 10;
                    } while (uvalue > 0);
                    
                    // 处理零填充
                    if (width > i && zero_padding) {
                        char zero = '0';
                        for (int j = 0; j < width - i; j++) {
                            write(1, &zero, 1);
                            count++;
                        }
                    }
                    
                    // 反向输出数字
                    while (i > 0) {
                        i--;
                        write(1, &num_buf[i], 1);
                        count++;
                    }
                } else {
                    int value = va_arg(args, int);
                    itoa(value, buf, 10);
                    int len = strlen(buf);
                    
                    // 处理零填充
                    if (width > len && zero_padding) {
                        char zero = '0';
                        for (int i = 0; i < width - len; i++) {
                            write(1, &zero, 1);
                            count++;
                        }
                    }
                    
                    write(1, buf, len);
                    count += len;
                }
                break;
            }
            
            case 'x': {
                // 十六进制整数
                if (is_long) {
                    unsigned long value = va_arg(args, unsigned long);
                    char num_buf[24];
                    int i = 0;
                    do {
                        int digit = value % 16;
                        num_buf[i++] = digit < 10 ? '0' + digit : 'a' + digit - 10;
                        value /= 16;
                    } while (value > 0);
                    
                    // 处理零填充
                    if (width > i && zero_padding) {
                        char zero = '0';
                        for (int j = 0; j < width - i; j++) {
                            write(1, &zero, 1);
                            count++;
                        }
                    }
                    
                    // 反向输出数字
                    while (i > 0) {
                        i--;
                        write(1, &num_buf[i], 1);
                        count++;
                    }
                } else {
                    unsigned int value = va_arg(args, unsigned int);
                    itoa(value, buf, 16);
                    int len = strlen(buf);
                    
                    // 处理零填充
                    if (width > len && zero_padding) {
                        char zero = '0';
                        for (int i = 0; i < width - len; i++) {
                            write(1, &zero, 1);
                            count++;
                        }
                    }
                    
                    write(1, buf, len);
                    count += len;
                }
                break;
            }
            
            case 'p': {
                // 指针（十六进制）
                unsigned long value = (unsigned long)va_arg(args, void*);
                write(1, "0x", 2);
                
                char num_buf[24];
                int i = 0;
                do {
                    int digit = value % 16;
                    num_buf[i++] = digit < 10 ? '0' + digit : 'a' + digit - 10;
                    value /= 16;
                } while (value > 0);
                
                // 处理零填充
                if (width > i && zero_padding) {
                    char zero = '0';
                    for (int j = 0; j < width - i; j++) {
                        write(1, &zero, 1);
                        count++;
                    }
                }
                
                // 反向输出数字
                while (i > 0) {
                    i--;
                    write(1, &num_buf[i], 1);
                    count++;
                }
                count += 2; // 为"0x"前缀
                break;
            }
            
            case '%': {
                // 百分号
                write(1, "%", 1);
                count++;
                break;
            }
            
            default: {
                // 未知格式，输出原样
                char tmp[4] = {'%', 0, 0, 0};
                int tmp_len = 1;
                
                if (zero_padding) {
                    tmp[tmp_len++] = '0';
                }
                
                if (width > 0) {
                    char width_buf[8];
                    itoa(width, width_buf, 10);
                    write(1, width_buf, strlen(width_buf));
                    count += strlen(width_buf);
                }
                
                if (is_long) {
                    tmp[tmp_len++] = 'l';
                }
                
                tmp[tmp_len++] = c;
                write(1, tmp, tmp_len);
                count += tmp_len;
                break;
            }
        }
    }
    
    // 输出剩余的普通字符
    if (buf_index > 0) {
        write(1, output_buf, buf_index);
    }
    
    va_end(args);
    return count;
}