#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"

unsigned int compile(FILE* _code, FILE* _result, unsigned int _loop_start); //


int main(int argc, char** argv)
{
	int ret;
	FILE *src, *dst;
	if(argc!=3)
	{
		fprintf(stderr, "%s: [source-file] [target-executable]\n", argv[0]);
		return -1;
	}

	src = fopen(argv[1], "r");
	dst = fopen(argv[2], "wb");

	ret = compile(src, dst, 0);

	fflush(dst);
	return ret;
}

unsigned int compile(FILE* _code, FILE* _result, unsigned int _addr) //loop_start is the starting address of the loop
{
	unsigned int curr_addr=_addr; //
	unsigned int loop_start_addr;
	unsigned int loop_end_addr;
	int c;

	if(_addr == 0) //if addr = 0 then we actually should initialize our stack
	{
		fwrite(opcodes[8]._bytecode, sizeof(char), opcodes[8]._size, _result);
		curr_addr+=opcodes[8]._size;
	}

	while((c=fgetc(_code))!=EOF)
	{
		for(int i=0;i<6;i++) //checking and writing 6 first options, see compiler.h
		{
			if(opcodes[i]._c==c)
			{
				fwrite(opcodes[i]._bytecode, sizeof(char), opcodes[i]._size, _result);
				curr_addr+=opcodes[i]._size;//shifting curr_addr "pointer" to next free addr
			}
			else continue;
		}

		if(c=='[')
		{
			//temporary variables for loop start and end operands
			int je_operand;
			int jmp_operand;
			long tmp_pos; //for FILE*, used later

			//writing opcodes
			fwrite(opcodes[6]._bytecode, sizeof(char), opcodes[6]._size, _result);
			curr_addr+=opcodes[6]._size;

			loop_start_addr=curr_addr; //je is here - 6
			//shifting curr_addr
			curr_addr=compile(_code, _result, loop_start_addr); //compiling what's inside the loop
			loop_end_addr = curr_addr;

			//we're doing it recursively in order to support loops inside loops inside loops ... etc

			je_operand = loop_end_addr - loop_start_addr;
			jmp_operand = loop_start_addr - loop_end_addr - 9; //shifting by -9 to include pre-loop checks

			tmp_pos = ftell(_result);

			fseek(_result, loop_start_addr-4, SEEK_SET); //writing je from loop_start to loop_end
			fwrite(&je_operand, sizeof(int), 1, _result);
			fseek(_result, loop_end_addr-4, SEEK_SET);	//writing jmp from loop_end to loop_start
			fwrite(&jmp_operand, sizeof(int), 1, _result);
			fseek(_result, tmp_pos, SEEK_SET); //goint back to previous cursor position

		}
		else if((c==']')&&(_addr!=0))
		{
			fwrite(opcodes[7]._bytecode, sizeof(char), opcodes[7]._size, _result);
			curr_addr+=opcodes[7]._size;
			return curr_addr;
		}
		else if((c==']')&&(_addr==0))
			exit(-1);

	}

	if(_addr == 0) //final write, almost done
	{
		//return sequence
		fwrite(opcodes[9]._bytecode, sizeof(char), opcodes[9]._size, _result);
	}
	return 0;

}
