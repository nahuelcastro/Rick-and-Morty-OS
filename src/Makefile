# ** por compatibilidad se omiten tildes **
# ==============================================================================
# TRABASO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
# ==============================================================================

MCOPY=mcopy

KERNEL_SRC=kernel.asm
KERNEL_OBJ=kernel.o
KERNEL_BIN=kernel.bin
PRNG_OBJ=prng.o seed.o

OBJ=$(KERNEL_OBJ) gdt.o idt.o isr.o pic.o mmu.o screen.o tss.o game.o sched.o $(PRNG_OBJ)

# Dependencias, generado por el flag -MMD de gcc, util para recompilar cuando actualizamos headers.
DEP=$(OBJ:%.o=%.d)
DISK_IMG=diskette.img
DISK_IMG_BZ=diskette.img.bz2

CFLAGS=-Wall -Wextra -pedantic -std=c99 -Werror -Og -ggdb \
  -m32 -ffreestanding -fno-pie -fno-zero-initialized-in-bss -fno-stack-protector

TASKSCFLAGS=$(CFLAGS) -fno-toplevel-reorder

TASKIDLE=idle.tsk
TASKRICK=taskRick.tsk
TASKMORTY=taskMorty.tsk

TASKS=tareas.tsk

TASKSBINS=$(TASKIDLE) $(TASKRICK) $(TASKMORTY)

CODESIZE=$$((65536 - 0x1200))
TASKSSIZE=$$((4 * (4096) * 2 + 4096)) # Dos tareas de 4 paginas + 1 pagina idle.
KERNELSIZE=$$(($(CODESIZE)+$(TASKSSIZE)))

TASKSTARTADDR=0x01D00000

CC=gcc
NASM=nasm
NASMFLAGS=-felf32 -g -F DWARF
OBJCOPY=objcopy
NM=nm
LD=ld
LDTASKFLAGS=-s -static -m elf_i386 --oformat binary -b elf32-i386 -e task -Ttext $(TASKSTARTADDR)
LDFLAGS=-static -m elf_i386 -b elf32-i386 -e start -Ttext 0x1200

QUIET = @

.PHONY=clean all image tasks

all:  clean tasks image 

format:
	clang-format -i *.h *.c

%.o: %.c
	$(CC) $(CFLAGS) -MMD -c $< -o $@

%.o: %.asm
	$(NASM) $(NASMFLAGS) $< -MD $(basename $@).d -o $@

seed:
	RNG=$$(head -c 32 /dev/urandom | shasum -a 256 | cut -c1-16);\
	sed -i "s/seed.*/seed=0x$${RNG}ull;/" seed.c;\

-include $(DEP)

kernel.bin: $(OBJ) tasks
	@echo 'Linkeando kernel...'
	$(LD) $(LDFLAGS) -o $@.elf $(OBJ)
	@echo 'Extrayendo tabla de simbolos...'
	$(NM) $@.elf | grep -v " U " | awk '{print "0x" $$1 " " $$3}' > kernel.sym
	@echo 'Generando binario sin informacion elf...'
	$(OBJCOPY) -S -O binary $@.elf $@
	@echo ''
	@echo 'Generando imagen del kernel...'
	mv kernel.bin kernel.bin.tmp
	dd if=/dev/zero of=kernel.bin bs=1 count=$(KERNELSIZE) conv=notrunc status=noxfer > /dev/null
	dd if=kernel.bin.tmp of=kernel.bin bs=1 count=$(CODESIZE) conv=notrunc status=noxfer > /dev/null
	dd if=$(TASKS) of=kernel.bin bs=1 count=$(TASKSSIZE) seek=$(CODESIZE) conv=notrunc status=noxfer > /dev/null
	rm kernel.bin.tmp
	@echo ''

image: kernel.bin $(DISK_IMG)
	@echo 'Copiando el $(KERNEL_BIN) a la imagen de diskette'
	$(MCOPY) -o -i $(DISK_IMG) $(KERNEL_BIN) ::/
	@echo ''

$(DISK_IMG): $(DISK_IMG_BZ)
	bzip2 -dk $(DISK_IMG_BZ)

idle.tsk: idle.asm
	@echo 'Compilando tarea idle...'
	$(NASM) -fbin -o $@ $^
	@echo ''
	@echo 'Generando imagen de la tarea idle...'
	mv idle.tsk idle.tsk.tmp
	dd if=/dev/zero bs=1 count=4K of=$@ status=noxfer > /dev/null 2>&1
	dd if=$@.tmp bs=1 count=4K seek=0 conv=notrunc of=$@ status=noxfer > /dev/null 2>&1
	rm $@.tmp
	@echo ''

task%.tsko: task%.c
	$(CC) $(TASKSCFLAGS) -MMD -c -o $@ $^

%.tsk: %.tsko
	@echo 'Linkeando tarea...'
	$(LD) $(LDTASKFLAGS) -o $@.tmp $^
	@echo ''
	mv $@.tmp $@

tasks: $(TASKSBINS)
	@echo 'Generando imagen de las tareas...'
	$(QUIET) dd if=/dev/zero   bs=1 count=$(TASKSSIZE) of=$(TASKS) status=noxfer > /dev/null 2>&1
	$(QUIET) dd if=$(TASKRICK)   bs=1 count=16K of=$(TASKS) seek=0K   status=noxfer > /dev/null 2>&1
	$(QUIET) dd if=$(TASKMORTY)   bs=1 count=16K of=$(TASKS) seek=16K  status=noxfer > /dev/null 2>&1
	$(QUIET) dd if=$(TASKIDLE) bs=1 count=4K of=$(TASKS) seek=32K  status=noxfer > /dev/null 2>&1
	@echo ''

clean:
	@echo 'Limpiando todo...'
	rm -f *.o
	rm -f *.bin
	rm -f *.tmp
	rm -f *.ctsko
	rm -f kernel.sym
	rm -f *.ctsk
	rm -f *.tsko
	rm -f *.tsk
	rm -f *.d
	rm -f $(DISK_IMG)
	rm -rf *~
	@echo ''