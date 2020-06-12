TARGET=vmm

SRCS = \
	main.c	\
	vmx.c		\
	log.c		\

%.o : %.asm
	nasm -f elf64 $<

CFLAGS= -I../kernelmodule

all:
	nasm -f elf64 cpu.asm
	gcc ${SRCS} cpu.o -o ${TARGET} ${CFLAGS}

clean:
	rm -f *.o ${TARGET}

