obj-m += vmm.o
vmm-objs := cpu.o kernelmod.o
#memory_asm-objs := module.o main.o

all: cpu.o
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) V=1 modules

cpu.o: cpu.asm
	nasm -f elf64 -o $@ $^ && echo "" > .cpu.o.cmd

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
