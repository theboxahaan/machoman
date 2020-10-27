#include<stdio.h>
#include<stdlib.h>

int main(int argc, char** argv)
{
	int x = 0;
	int* ptr = &x;
	FILE *fp = fopen(argv[1], "r");
	fread(ptr, sizeof(int), 1, fp);
	printf("--> %d\n %p\n", *ptr, ptr);
	return 0;
}
