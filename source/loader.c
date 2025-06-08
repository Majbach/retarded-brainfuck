#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>


int main(int argc, char** argv)
{
	FILE* f;
	void (*program)();
	unsigned char* page;

	if(argc<2)
		return -1;

	else if((f=fopen(argv[1],"rb"))<0)
		return -1;

	else if((program=mmap(NULL, 65535, PROT_READ|PROT_WRITE|PROT_EXEC, MAP_PRIVATE|MAP_ANON, 0, 0))!=NULL)
	{
		page=(unsigned char*)program;
		int i=0;
		int c;

		while((c=fgetc(f))!=EOF)
		{
			page[i]=c;
			i++;
		}
		(*program)();
		return 0;
	}
}
