ELF := build/crystal.elf
IMAGE := build/crystal.hdd

CC = clang++
ASM = nasm
CFLAGS = -Wall -Wextra -O2 -pipe -g3 -std=c++2a

LDINTERNALFLAGS :=			\
	-Tconfig/linker.ld 		\
	-static					\
	-nostdlib				\
	-no-pie

INTERNALCFLAGS  :=			\
	-Isrc/					\
	-ffreestanding			\
	-Wno-unused-parameter	\
	-fno-stack-protector	\
	-fno-pic				\
	-mno-80387				\
	-mno-mmx				\
	-mno-3dnow				\
	-mno-sse2				\
	-mno-sse2				\
	-mcmodel=kernel			\
	-Werror					\
	-mno-red-zone

CPPFILES	:= $(shell find ./src -type f -name '*.cpp')
ASMFILES	:= $(shell find ./src -type f -name '*.asm')
OBJ			:= $(CPPFILES:.cpp=.o) $(ASMFILES:.asm=.o)
QEMUFLAGS	= -hda $(IMAGE) -debugcon file:/dev/stdout -monitor stdio -d int -no-reboot -no-shutdown

.PHONY: all clean

all: $(ELF)

$(ELF): $(OBJ)
	$(CC) $(LDINTERNALFLAGS) $(OBJ) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $(INTERNALCFLAGS) -c $< -o $@

%.o: %.asm
	$(ASM) $< -f elf64 -o $@

limine/limine-install:
	git clone https://github.com/limine-bootloader/limine.git --depth=1 --branch=v2.0-branch-binary
	make -C limine

$(IMAGE): $(ELF) limine/limine-install
	dd if=/dev/zero bs=1M count=0 seek=64 of=$(IMAGE)
	parted -s $(IMAGE) mklabel msdos
	parted -s $(IMAGE) mkpart primary 1 100%
	echfs-utils -m -p0 $(IMAGE) quick-format 32768
	echfs-utils -m -p0 $(IMAGE) import $(ELF) /crystal.elf
	echfs-utils -m -p0 $(IMAGE) import config/limine.cfg /limine.cfg
	echfs-utils -m -p0 $(IMAGE) import limine/limine.sys /limine.sys
	limine/limine-install $(IMAGE)

run: $(IMAGE)
	qemu-system-x86_64 $(QEMUFLAGS)

clean:
	rm -rf $(ELF) $(OBJ) $(IMAGE)
