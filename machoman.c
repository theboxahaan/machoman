#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
/*
Doing no error handling of any sort. So run properly (-_-')
Reference-
https://opensource.apple.com/source/xnu/xnu-6153.11.26/EXTERNAL_HEADERS/mach-o/loader.h
*/

#define MH_MAGIC_64 0xfeedfacf /* the 64-bit mach magic number */
#define MH_CIGAM_64 0xcffaedfe /* NXSwapInt(MH_MAGIC_64) */

struct mach_header_64
{
	uint32_t magic;
	int cpu_type_t;
	int cpu_subtype_t;
	uint32_t filetype;
	uint32_t ncmds;
	uint32_t sizeofcmds;
	uint32_t flags;
	uint32_t reserved;

};

int is_64(FILE* fp)
{
	uint32_t buffer;
	fread(&buffer, sizeof(uint32_t), 1, fp);
	rewind(fp);
	if(buffer == MH_MAGIC_64 || buffer == MH_CIGAM_64) return 1;
	else return 0;
	
}

void print_mach_header_64(struct mach_header_64* header)
{
	printf("magic:%u, cpu_type_t:%d, cpu_subtype_t:%d, filetype:%u, ncmds:%u, sizeofcmds:%u, flags:%u, reserved:%d\n", header->magic, header->cpu_type_t, header->cpu_subtype_t, header->filetype, header->ncmds, header->sizeofcmds, header->flags, header->reserved);
	
}

void parse(FILE* fp)
{
	/* Get Mach-O Header BEGIN */
	if(is_64(fp))
	{
		struct mach_header_64* header = (struct mach_header_64* )calloc(1, sizeof(struct mach_header_64));
		fread(header, sizeof(struct mach_header_64), 1, fp);
		print_mach_header_64(header);
		free(header);
	}
	else printf("Not 64 bit\n");
	/* Mach-O Header END */
}

 
int main(int argc, char** argv)
{
	if(argc !=2) return -1;
	/* Get file pointer for argv[1] */
	FILE* fp;
	if((fp = fopen(argv[1], "r")) != NULL )
	{
		parse(fp);
		fclose(fp);	
	}
	else printf("File Not Opened \n");
	
	return 0;
}
