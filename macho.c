#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<strings.h>
#include "structs.h"

void *read_file(FILE *fp)
{
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	rewind(fp);
	void *buffer =  malloc(size);
	fread(buffer, 1, size, fp);
	return buffer;
}

//void get_header(void* buf, void* header)
void get_header(void *buf)
{
	struct mach_header_64 header;
	void* header_ptr = &header;
	bcopy(buf, header, sizeof(struct mach_header_64));	
	//printf("First Byte: %x\n", ((unsigned char*)(buf)));
	print_mach_header_64((struct mach_header_64*)header_ptr);
}

void get_load_segments(void *buff)
{
	struct load_command 
}

void print_mach_header_64(struct mach_header_64* header)
{
	printf("----[ HEADER ]----\n");
	printf("magic:0x%x, cpu_type_t:%d, cpu_subtype_t:%d, filetype:%u, ncmds:%u, sizeofcmds:%u, flags:0x%x, reserved:%d\n", header->magic, header->cputype, header->cpusubtype, header->filetype, header->ncmds, header->sizeofcmds, header->flags, header->reserved);
	
}


void clear_buffer(void* buffer)
{
	free(buffer);
}



int main(int argc, char** argv)
{
	if(argc != 2)return -1;
	FILE* fp;
	if((fp = fopen(argv[1], "r")) != NULL)
	{
		void* buffer = read_file(fp); /*void* buffer contains the entire file*/
		fclose(fp);
		#if 0
		struct mach_header_64 header;
		void* header_ptr = &header;
		get_header(buffer, header_ptr);
		print_mach_header_64((struct mach_header_64*)header_ptr);
		#endif
		get_header(buffer);
		get_load_segments(buffer);
		clear_buffer(buffer);
	}
	return 0;
}
