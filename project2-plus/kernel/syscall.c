#include "../include/syscall.h"
#include "../include/console.h"
#include "../include/types.h"
#include "../include/timer.h"

// 前向声明
uint64 sys_yield();

// 系统调用：写入
uint64 sys_write(int fd, const char *buf, uint64 count) {
    console_printf_SYSCALL("sys_write: fd=%d, buf=0x%lx, count=%lu\n", fd, (uint64)buf, count);
    
    // 目前只支持标准输出（fd=1）
    if (fd != 1) return -1;
    
    // 安全检查：确保buf指向有效内存
    // 在实际系统中，需要检查buf是否在用户空间范围内
    
    // 输出字符
    for (uint64 i = 0; i < count; i++) {
        console_putc(buf[i]);
    }
    
    return count;
}

// 系统调用：退出
uint64 sys_exit(int code) {
    console_printf_SYSCALL("sys_exit: code=%d\n", code);
    
    // 在实际系统中，这里应该终止当前进程
    console_printf_SYSCALL("进程退出，退出码: %d\n", code);
    
    // 目前我们没有进程管理，所以只是返回
    return 0;
}

// 系统调用：获取进程ID
uint64 sys_getpid() {
    console_printf_SYSCALL("sys_getpid\n");
    
    // 在实际系统中，这里应该返回当前进程ID
    // 目前我们没有进程管理，所以返回一个固定值
    return 1;
}

// 系统调用：睡眠
uint64 sys_sleep(uint64 milliseconds) {
    console_printf_SYSCALL("sys_sleep: milliseconds=%lu\n", milliseconds);
    
    uint64 start_time = get_time_ms();
    uint64 end_time = start_time + milliseconds;
    
    while (get_time_ms() < end_time) {
        // 在实际系统中，这里应该让当前进程进入睡眠状态
        // 目前我们没有进程管理，所以只是忙等待
        sys_yield(); // 让出CPU
    }
    
    return 0;
}

// 系统调用：让出CPU
uint64 sys_yield() {
    console_printf_SYSCALL("sys_yield\n");
    
    // 在实际系统中，这里应该触发调度器选择下一个进程运行
    // 目前我们没有进程管理，所以只是返回
    return 0;
}

// 系统调用：获取系统时间
uint64 sys_time() {
    console_printf_SYSCALL("sys_time\n");
    
    return get_time_ms();
}

// 系统调用：执行程序
uint64 sys_exec(const char *path, char *const argv[]) {
    console_printf_SYSCALL("sys_exec: path=%s\n", path);
    
    // 在实际系统中，这里应该加载并执行指定的程序
    console_printf_SYSCALL("执行程序: %s\n", path);
    
    // 目前我们没有文件系统和进程管理，所以只是返回错误
    return -1;
}

// 系统调用：创建子进程
uint64 sys_fork() {
    console_printf_SYSCALL("sys_fork\n");
    
    // 在实际系统中，这里应该创建当前进程的副本
    console_printf_SYSCALL("创建子进程\n");
    
    // 目前我们没有进程管理，所以只是返回错误
    return -1;
}

// 系统调用：等待子进程
uint64 sys_wait(int *status) {
    console_printf_SYSCALL("sys_wait: status=0x%lx\n", (uint64)status);
    
    // 在实际系统中，这里应该等待任意子进程终止
    console_printf_SYSCALL("等待子进程\n");
    
    // 目前我们没有进程管理，所以只是返回错误
    return -1;
}

// 系统调用：打开文件
uint64 sys_open(const char *path, int flags) {
    console_printf_SYSCALL("sys_open: path=%s, flags=%d\n", path, flags);
    
    // 在实际系统中，这里应该打开指定的文件
    console_printf_SYSCALL("打开文件: %s\n", path);
    
    // 目前我们没有文件系统，所以只是返回错误
    return -1;
}

// 系统调用：关闭文件
uint64 sys_close(int fd) {
    console_printf_SYSCALL("sys_close: fd=%d\n", fd);
    
    // 在实际系统中，这里应该关闭指定的文件描述符
    console_printf_SYSCALL("关闭文件描述符: %d\n", fd);
    
    // 目前我们没有文件系统，所以只是返回错误
    return -1;
}

// 系统调用：读取文件
uint64 sys_read(int fd, void *buf, uint64 count) {
    console_printf_SYSCALL("sys_read: fd=%d, buf=0x%lx, count=%lu\n", fd, (uint64)buf, count);
    
    // 在实际系统中，这里应该从指定的文件描述符读取数据
    // 目前只支持标准输入（fd=0）
    if (fd != 0) return -1;
    
    // 安全检查：确保buf指向有效内存
    // 在实际系统中，需要检查buf是否在用户空间范围内
    
    // 目前我们没有实现控制台输入，所以只是返回0（表示EOF）
    return 0;
}

// 系统调用处理函数
uint64 syscall(uint64 syscall_num, uint64 a0, uint64 a1, uint64 a2, uint64 a3, uint64 a4, uint64 a5) {
    console_printf_SYSCALL("处理系统调用，系统调用号: %ld (0x%lx)\n", syscall_num, syscall_num);
    
    uint64 ret = -1;
    
    switch (syscall_num) {
        case SYS_write:
            console_printf_SYSCALL("执行write系统调用\n");
            ret = sys_write((int)a0, (const char*)a1, a2);
            break;
        case SYS_exit:
            console_printf_SYSCALL("执行exit系统调用\n");
            ret = sys_exit((int)a0);
            break;
        case SYS_getpid:
            console_printf_SYSCALL("执行getpid系统调用\n");
            ret = sys_getpid();
            break;
        case SYS_sleep:
            console_printf_SYSCALL("执行sleep系统调用\n");
            ret = sys_sleep(a0);
            break;
        case SYS_yield:
            console_printf_SYSCALL("执行yield系统调用\n");
            ret = sys_yield();
            break;
        case SYS_time:
            console_printf_SYSCALL("执行time系统调用\n");
            ret = sys_time();
            break;
        case SYS_exec:
            console_printf_SYSCALL("执行exec系统调用\n");
            ret = sys_exec((const char*)a0, (char *const*)a1);
            break;
        case SYS_fork:
            console_printf_SYSCALL("执行fork系统调用\n");
            ret = sys_fork();
            break;
        case SYS_wait:
            console_printf_SYSCALL("执行wait系统调用\n");
            ret = sys_wait((int*)a0);
            break;
        case SYS_open:
            console_printf_SYSCALL("执行open系统调用\n");
            ret = sys_open((const char*)a0, (int)a1);
            break;
        case SYS_close:
            console_printf_SYSCALL("执行close系统调用\n");
            ret = sys_close((int)a0);
            break;
        case SYS_read:
            console_printf_SYSCALL("执行read系统调用\n");
            ret = sys_read((int)a0, (void*)a1, a2);
            break;
        default:
            console_printf_SYSCALL("未知系统调用: %ld\n", syscall_num);
            break;
    }
    
    console_printf_SYSCALL("系统调用返回值: 0x%lx\n", ret);
    return ret;
}