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

struct load_command
{
	uint32_t cmd;
	uint32_t cmdsize;
};

int is_64(FILE* fp)
{
	uint32_t buffer;
	fread(&buffer, sizeof(uint32_t), 1, fp);
	rewind(fp);
	if(buffer == MH_MAGIC_64 || buffer == MH_CIGAM_64) return 1;
	else return 0;
	
}


struct segment_command_64
{
	uint32_t cmd;
	uint32_t cmdsize;
	char segname[16];
	uint64_t vmaddr;
	uint64_t vmsize;
	uint64_t fileoff;
	uint64_t filesize;
	int maxprot;
	int initprot;
	uint32_t nsects;
	uint32_t flags;
};


void print_mach_header_64(struct mach_header_64* header)
{
	printf("----[ HEADER ]----\n");
	printf("magic:%u, cpu_type_t:%d, cpu_subtype_t:%d, filetype:%u, ncmds:%u, sizeofcmds:%u, flags:%u, reserved:%d\n", header->magic, header->cpu_type_t, header->cpu_subtype_t, header->filetype, header->ncmds, header->sizeofcmds, header->flags, header->reserved);
	
}




void parse(FILE* fp)
{
	/* Get Mach-O Header BEGIN */
	if(is_64(fp))
	{
		struct mach_header_64 header;
		fread(&header, sizeof(struct mach_header_64), 1, fp);
		print_mach_header_64(&header);
		
		/*
		print all load commands
		*/

		uint32_t cur_size = header.sizeofcmds;
		//struct load_command* tmp = (struct load_command*)calloc(1, sizeof(struct load_command));
		struct load_command tmp;
		printf("\n----[ LOAD COMMANDS ]----\n");
		for(int i=0; cur_size > 0; i++)
		{
			fread(&tmp, sizeof(struct load_command), 1, fp);
			printf("%d. cmd:0x%x, cmdsize:%u\n", i, tmp.cmd, tmp.cmdsize);
			struct segment_command_64 seg_cmd;
			fread(&seg_cmd, sizeof(struct segment_command_64), 1, fp);
		
			if(seg_cmd.cmd == 0x19)
			{
				printf("cmd:0x%x, cmdsize:%u, segname:%s, vmaddr:0x%llx, vmsize:0x%llx, fileoff:0x%llu, filesize:0x%llu, maxprot:%d, initprot:%d, nsects:%u, flags:0x%x\n", seg_cmd.cmd, seg_cmd.cmdsize, seg_cmd.segname, seg_cmd.vmaddr, seg_cmd.vmsize, seg_cmd.fileoff, seg_cmd.filesize, seg_cmd.maxprot, seg_cmd.initprot, seg_cmd.nsects, seg_cmd.flags);
			}
			else printf("NOT LC_SEGMENT_64");

			fseek(fp, tmp.cmdsize - sizeof(struct segment_command_64) - sizeof(struct load_command), SEEK_CUR);	
			cur_size -= tmp.cmdsize;
		}
		
		//free(tmp);
		//free(header);
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
