obj-m += vmm.o
vmm-objs := cpu.o kernelmod.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) V=1 modules

%.o : %.asm
	nasm -f elf64 $<

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
