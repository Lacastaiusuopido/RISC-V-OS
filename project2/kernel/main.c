#include "../include/types.h"
#include "../include/riscv.h"
#include "../include/console.h"
#include "../include/trap.h"
#include "../include/timer.h"
#include "../include/syscall.h"
#include "qemu_detect.c"


// 用户程序地址和栈地址
#define USER_PROGRAM_ADDR 0x80400000ULL
#define USER_STACK_TOP    0x80800000ULL

// 用户程序在磁盘上的位置
#define USER_SECTOR 64
#define SECTOR_SIZE 512

extern void trap_vector();

// 简单的磁盘读取函数 - 直接从内存读取
void disk_read(void *dst, uint32 offset, uint32 count) {
    // 整个磁盘镜像已经被QEMU加载到内存中
    // 引导加载器位于 0x80000000
    
    // 计算源地址：引导加载器起始地址 + 偏移量
    char *src = (char*)0x80000000 + offset;
    char *d = (char*)dst;
    
    // 简单的内存拷贝
    for(uint32 i = 0; i < count; i++) {
        d[i] = src[i];
    }
}


// 切换到用户模式并执行用户程序
void switch_to_user_mode() {
    console_printf("准备切换到用户模式...\n");
    
    // 打印用户程序前16字节，检查是否正确加载
    uint8 *program = (uint8*)USER_PROGRAM_ADDR;
    console_printf("用户程序前16字节: ");
    for(int i = 0; i < 16; i++) {
        console_printf("%02x ", program[i]);
    }
    console_printf("\n");
    
    // 确保中断处理已初始化
    console_printf("当前STVEC: 0x%lx\n", r_stvec());
    
    // 启用中断
    uint64 sie = r_sie();
    sie |= SIE_SEIE | SIE_STIE | SIE_SSIE;  // 启用所有S模式中断
    w_sie(sie);
    console_printf("SIE设置后: 0x%lx\n", r_sie());
    
    // 设置SEPC（异常程序计数器）为用户程序入口点
    uint64 user_entry = (uint64)USER_PROGRAM_ADDR;
    w_sepc(user_entry);
    
    // 设置SSTATUS
    uint64 sstatus = r_sstatus();
    sstatus &= ~SSTATUS_SPP;  // 清除SPP位，表示返回到用户模式
    sstatus |= SSTATUS_SPIE;  // 设置SPIE位，确保返回后中断启用
    sstatus |= SSTATUS_SIE;   // 直接启用中断
    w_sstatus(sstatus);
    
    console_printf("最终检查 - SEPC: 0x%lx, SSTATUS: 0x%lx, SIE: 0x%lx\n", 
                  r_sepc(), r_sstatus(), r_sie());
    console_printf("即将执行sret指令...\n");
    
    // 使用内联汇编执行sret指令，同时设置sp寄存器
    asm volatile(
        "li sp, 0x80800000\n"  // 设置sp为用户栈
        "sret\n"               // 切换到用户模式
    );
    
    // 如果执行到这里，说明从用户模式返回了
    console_printf("错误：从用户模式返回\n");
}

// 加载用户程序
void load_user_program() {
    console_printf("加载用户程序...\n");
    
    // 清空用户程序区域
    char *dst = (char*)USER_PROGRAM_ADDR;
    for(uint32 i = 0; i < 16 * SECTOR_SIZE; i++) {
        dst[i] = 0;
    }
    
    // 从磁盘读取用户程序
    disk_read((void*)USER_PROGRAM_ADDR, USER_SECTOR * SECTOR_SIZE, 16 * SECTOR_SIZE);
    
    // 验证用户程序是否成功加载
    uint8 *program = (uint8*)USER_PROGRAM_ADDR;
    if (program[0] == 0 && program[1] == 0 && program[2] == 0 && program[3] == 0) {
        console_printf("警告：用户程序前4字节为零，可能未正确加载\n");
    }
    
    console_printf("用户程序已加载到地址 0x%lx\n", (unsigned long)USER_PROGRAM_ADDR);
}

// 内核入口函数
void kernel_main() {
    // 初始化控制台
    console_init();
    console_printf("控制台初始化完成\n");
    // 检测QEMU环境
    detect_qemu_environment();

     // 设置异常委托
    uint64 medeleg = 0;
    uint64 mideleg = 0;
    
    // 委托所有异常给S模式
    asm volatile("csrw medeleg, %0" : : "r" (0xFFFFULL));
    // 委托所有中断给S模式
    asm volatile("csrw mideleg, %0" : : "r" (0xFFULL));
    
    // 再次读取确认设置成功
    asm volatile("csrr %0, medeleg" : "=r" (medeleg));
    console_printf("设置后MEDELEG: 0x%lx\n", medeleg);
    
    asm volatile("csrr %0, mideleg" : "=r" (mideleg));
    console_printf("设置后MIDELEG: 0x%lx\n", mideleg);
    
    
    // 测试控制台打印功能
    //console_test();
    
    // 初始化中断处理
    trap_init();
    console_printf("中断处理初始化完成\n");
    
    // 初始化时钟
    timer_init();
    console_printf("时钟初始化完成\n");
    
    // 启用中断
    w_sstatus(r_sstatus() | SSTATUS_SIE);
    console_printf("中断已启用\n");
    
    // 启用时钟中断
    w_sie(r_sie() | SIE_STIE);
    console_printf("时钟中断已启用\n");
    
    console_printf("内核初始化完成，准备加载用户程序...\n");
    
    // 加载用户程序
    load_user_program();
    
    // 切换到用户模式并执行用户程序
    switch_to_user_mode();
    
    // 如果从switch_to_user_mode返回，进入空闲循环
    console_printf("进入空闲循环\n");
    while (1) {
        asm volatile("wfi");
    }
}