#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aux_func.h"


char *in_post = ".as";
char *out_post = ".ob";
char *ext_post = ".ext";
char *ent_post = ".ent";

I_table_row main_table[MEMORY_SIZE];

int main(int argc , char *argv[])
{
	int i;
	char *input_file;	

/*Input file*/
	FILE *fp;

	if (argc==1) /*No arguments*/
	{
		fprintf(stderr,"No input files!!!\n");
		exit(1);
	}	

	while( --argc > 0 )
	{
	input_file = strcat(argv[argc],in_post);
	fp=fopen(input_file,"r");
	
	if (!fp)
	{
		fprintf(stderr,
		"Cannot open %s	No such input file!!!\n", argv[argc]);
		exit(1);
	}	
	printf("input file:%s\n****************\n",input_file);
	first_scan(fp);

	}
/*	second_scan(fp);*/
	

/*Initializing & Printing Instruction table*/	
	putchar('\n');
	for (i=100;i<MEMORY_SIZE;i++)
	{
		main_table[i].dec_add=i;
		dec_to_quad(main_table[i].weird_four_add,
				  	 main_table[i].dec_add);
	}

	for (i=100;i<MEMORY_SIZE;i++)
	{
		printf("%d\t%s\n",main_table[i].dec_add,
						main_table[i].weird_four_add);
	}

return 0;
}
