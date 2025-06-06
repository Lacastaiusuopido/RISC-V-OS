/* 
 * boot.ld - 引导加载器的链接脚本
 * 定义了bootloader的内存布局和加载地址
 */

OUTPUT_ARCH(riscv)     /* 指定目标架构为RISC-V */
ENTRY(_start)          /* 程序入口点为_start符号 */

SECTIONS {
    /* 
     * QEMU virt 机器的 -kernel 选项默认加载到 0x80000000
     * 我们将引导加载器放在这个地址
     */
    . = 0x80000000;    /* 设置当前地址为QEMU默认加载地址 */

    .text : {
        *(.text .text.*)  /* 所有.text段（代码） */
    }

    .rodata : {
        *(.rodata .rodata.*)  /* 只读数据，如常量字符串等 */
    }

    .data : {
        *(.data .data.*)  /* 已初始化的可读写数据 */
    }

    .bss : {
        *(.bss .bss.*)    /* 未初始化数据，会被清零 */
    }

    /* 
     * 特殊符号定义，供 bootmain.c 使用 
     * _end 表示 Bootloader 的结束地址，可用于确定内核加载位置
     */
    _end = .;
}