/* 
 * kernel.ld - 内核的链接脚本
 * 定义了内核的内存布局和加载地址
 */

OUTPUT_ARCH(riscv)     /* 指定目标架构为RISC-V */
ENTRY(kernel_main)     /* 程序入口点为kernel_main函数 */

SECTIONS {
    /* 
     * 内核加载到物理地址0x80200000
     * 这是在bootloader之后的地址
     */
    . = 0x80200000;    /* 设置当前地址 */

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
     * 内核结束地址，可用于内存分配器
     */
    kernel_end = .;
}