#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aux_func.h"

#define MEMORY_SIZE 256



I_table_row main_table[MEMORY_SIZE];

int main(int argc , char *argv[])
{
	int i;
	char buf[MAX_ROW_LEN];	

/*Input file*/
	FILE *fp;

	if (argc==1) /*No arguments*/
	{
		fprintf(stderr,"No input files!!!\n");
		exit(1);
	}	

	fp=fopen(argv[1],"r");
	if (!fp)
	{
		fprintf(stderr,
		"Cannot open %s	No such input file!!!\n", argv[1]);
		exit(1);
	}	


/*Loop on input file*/
	while(fgets(buf,MAX_ROW_LEN,fp) !=NULL)
	{
		printf("%s\n",buf);
	}   

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
