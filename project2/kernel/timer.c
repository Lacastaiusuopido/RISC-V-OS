// timer.c - 时钟相关实现

#include "../include/types.h"
#include "../include/riscv.h"
#include "../include/timer.h"
#include "../include/console.h"

// QEMU RISC-V的CLINT (Core Local Interruptor)地址
#define CLINT_BASE      0x2000000
#define CLINT_MTIME     (CLINT_BASE + 0xBFF8)  // 计时器寄存器
#define CLINT_MTIMECMP  (CLINT_BASE + 0x4000)  // 计时器比较寄存器

// 时钟频率 (QEMU默认为10MHz)
#define CLOCK_FREQ      10000000

// 每次时钟中断的时间间隔（纳秒）
#define TIMER_INTERVAL_NS 10000000  // 10ms

// 系统启动以来的毫秒计数
static uint64 ticks = 0;

// 初始化时钟
void timer_init() {
    // 设置第一次时钟中断
    timer_set_next();
    console_printf("时钟初始化完成\n");
}

// 设置下一次时钟中断
void timer_set_next() {
    // 读取当前时间
    uint64 *mtime_ptr = (uint64*)CLINT_MTIME;
    uint64 mtime = *mtime_ptr;
    
    // 设置下一次中断时间
    uint64 *mtimecmp_ptr = (uint64*)CLINT_MTIMECMP;
    *mtimecmp_ptr = mtime + TIMER_INTERVAL_NS;
}

// 获取当前时间（毫秒）
uint64 get_time_ms() {
    uint64 *mtime_ptr = (uint64*)CLINT_MTIME;
    uint64 mtime = *mtime_ptr;
    
    // 转换为毫秒
    return (mtime * 1000) / CLOCK_FREQ;
}

// 处理时钟中断
void timer_handler() {
    ticks++;
    
    // 设置下一次时钟中断
    timer_set_next();
    
    // 每秒打印一次（每100个tick）
    if (ticks % 100 == 0) {
        console_printf("时钟中断: %d 秒\n", ticks / 100);
    }
    
    // 在实际系统中，这里应该触发调度器进行进程切换
}