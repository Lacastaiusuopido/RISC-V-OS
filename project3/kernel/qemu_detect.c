#include "types.h"
#include "console.h"

// 检测QEMU环境和支持的特权模式
void detect_qemu_environment() {
    console_printf_QEMU("检测QEMU环境...\n");
    
    // 读取misa寄存器
    uint64 misa = 0;
    asm volatile("csrr %0, misa" : "=r" (misa));
    console_printf_QEMU("MISA寄存器: 0x%lx\n", misa);
    
    // 检查支持的扩展
    console_printf_QEMU("支持的扩展: ");
    char extensions[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 0; i < 26; i++) {
        if (misa & (1ULL << i)) {
            console_printf_QEMU("%c ", extensions[i]);
        }
    }
    console_printf_QEMU("\n");
    
    // 特别关注S扩展（Supervisor模式）和U扩展（User模式）
    if (misa & (1ULL << 18)) {  // S扩展
        console_printf_QEMU("支持S模式 (Supervisor Mode)\n");
    } else {
        console_printf_QEMU("不支持S模式\n");
    }
    
    if (misa & (1ULL << 20)) {  // U扩展
        console_printf_QEMU("支持U模式 (User Mode)\n");
    } else {
        console_printf_QEMU("不支持U模式\n");
    }
    
    // 检查委托寄存器
    uint64 medeleg = 0;
    asm volatile("csrr %0, medeleg" : "=r" (medeleg));
    console_printf_QEMU("MEDELEG寄存器: 0x%lx\n", medeleg);
    
    uint64 mideleg = 0;
    asm volatile("csrr %0, mideleg" : "=r" (mideleg));
    console_printf_QEMU("MIDELEG寄存器: 0x%lx\n", mideleg);
    
    // 检查当前特权模式
    // RISC-V没有直接提供读取当前模式的CSR，但我们可以尝试读取不同特权级的寄存器
    
    // 尝试读取M模式特有的寄存器
    uint64 mvendorid = 0;
    asm volatile("csrr %0, mvendorid" : "=r" (mvendorid));
    console_printf_QEMU("MVENDORID: 0x%lx\n", mvendorid);
    
    // 尝试读取S模式特有的寄存器
    uint64 sstatus = 0;
    asm volatile("csrr %0, sstatus" : "=r" (sstatus));
    console_printf_QEMU("SSTATUS: 0x%lx\n", sstatus);
    
    // 检查QEMU特定的设备
    volatile uint32 *uart = (volatile uint32 *)0x10000000;  // QEMU UART地址
    console_printf_QEMU("UART地址: 0x%lx\n", (uint64)uart);
    
    // 检查QEMU的HTIF接口（如果存在）
    volatile uint64 *htif_console = (volatile uint64 *)0x40008000;
    console_printf_QEMU("HTIF控制台地址: 0x%lx\n", (uint64)htif_console);
    
    console_printf_QEMU("QEMU环境检测完成\n");
}