COMP := -Wall -c
BUILD := -Wall -o retarded-brainfuck

retarded-brainfuck: compiler.o loader instructions
	gcc $(BUILD) obj/compiler.o

compiler.o:
	gcc $(COMP) source/compiler.c -o obj/compiler.o

loader:
	gcc -Wall source/loader.c -o loader

instructions:
	nasm -f elf64 source/instructions.asm -o obj/instructions.o
