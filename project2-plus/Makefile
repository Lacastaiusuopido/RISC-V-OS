# 工具链配置
TOOLCHAIN = riscv64-unknown-elf-
CC = $(TOOLCHAIN)gcc
LD = $(TOOLCHAIN)ld
OBJCOPY = $(TOOLCHAIN)objcopy
AR = $(TOOLCHAIN)ar

# 编译选项
CFLAGS = -nostdlib -ffreestanding -fno-builtin -fno-stack-protector \
         -Wall -Wextra -Werror -I./include \
         -march=rv64gc -mabi=lp64d -mno-relax -mcmodel=medany \
         -Wno-main -Wno-unused-parameter -O1

# 链接选项
LDFLAGS = -melf64lriscv -no-pie -nostdlib

# 目标文件
BOOT_OBJS = boot/bootasm.o boot/bootmain.o boot/uart.o boot/disk.o
KERNEL_OBJS = kernel/entry.o kernel/main.o kernel/util.o kernel/console.o kernel/trap.o kernel/uart.o \
              kernel/syscall.o kernel/timer.o
USER_OBJS = user/entry.o user/user_program.o user/ulib.o

# 构建规则
all: os.bin

# 编译.S文件
%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

# 编译.c文件
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Boot部分
boot/boot.bin: $(BOOT_OBJS)
	$(LD) $(LDFLAGS) -T boot/boot.ld -o boot/boot.elf $^
	$(OBJCOPY) -O binary boot/boot.elf $@

# Kernel部分
kernel/kernel.elf: $(KERNEL_OBJS)
	$(LD) $(LDFLAGS) -T kernel/kernel.ld -o $@ $^


# User部分
user/user_program.bin: $(USER_OBJS)
	$(LD) $(LDFLAGS) -T user/user.ld -o user/user_program.elf $^
	$(OBJCOPY) -O binary user/user_program.elf $@

# 最终镜像
os.bin: boot/boot.bin kernel/kernel.elf user/user_program.bin
	dd if=/dev/zero of=$@ bs=1M count=32 status=none
	dd if=boot/boot.bin of=$@ conv=notrunc status=none
	dd if=kernel/kernel.elf of=$@ bs=512 seek=4 conv=notrunc status=none
	dd if=user/user_program.bin of=$@ bs=512 seek=64 conv=notrunc status=none
# 验证内核是否正确放置
	hexdump -C -n 32 -s 2048 $@

# 清理
clean:
	rm -f $(BOOT_OBJS) $(KERNEL_OBJS) $(USER_OBJS)
	rm -f boot/boot.elf boot/boot.bin
	rm -f kernel/kernel.elf kernel/kernel.bin
	rm -f user/user_program.elf user/user_program.bin
	rm -f os.bin

# 运行
run: os.bin
	qemu-system-riscv64 -machine virt -nographic -bios none -kernel os.bin
# 声明伪目标
.PHONY: all clean run debug