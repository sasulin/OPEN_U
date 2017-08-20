/*MAMAN 14 Final Project*/
/*Shmuel Asulin ,ID:036760676  */
/*Yotam Klein* , ID:066546896 */

/*Assembler*/

/*This program emulates an assembler from 
an imaginary programming language to an imaginary machine code*/

/*The algorithm is based on two scans(passes) on an input file*/

/*The program can recieve several input files at once, but deals with each one
indivudually and one at a time*/

/*The main function depends on another source file and a header file:
	scan.c
	aux_func.h
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aux_func.h"

#define CHECK_OPEN(FP,F_NAME)\
{\
		if(!FP)\
		{\
			fprintf(stderr,"Cannot open \"%s\"\nNo such input file!!!\n",F_NAME);\
			exit(1);\
		}\
}\

#define FILE_STRING(FILE_NAME,TEXT)\
{\
    strcpy(FILE_NAME,prefix);\
    strcat(FILE_NAME,TEXT);\
}\

const short int sym_size = sizeof(symbol_row);

/*Filename extensions*/
char *in_post = ".as";
char *out_post = ".ob";
char *ext_post = ".ext";
char *ent_post = ".ent";

int main(int argc , char *argv[])
{
	I_table_row main_table[MEMORY_SIZE];
	D_table_row data_table[MEMORY_SIZE];
	I_row_p IC_p;
	D_row_p DC_p;

	int i,j,IC,DC;
	char prefix[FILE_NAME_LEN];
	char input_file[FILE_NAME_LEN];
	char output_file[FILE_NAME_LEN];
	char ext_file[FILE_NAME_LEN]; 
	char ent_file[FILE_NAME_LEN];	
	char str_print[WEIRD_BASE+1];
	char str_tmp[MAX_LABEL_SIZE];
	bool error;
	sym_row_p sym_head;
	sym_row_p tmp;

	FILE *fp_in;
	FILE *fp_out;
	FILE *fp_ext;
	FILE *fp_ent;

	error=NO;
	IC_p = &main_table[0];
	DC_p = &data_table[0];
	
	if (argc==1) /*No command line arguments*/
	{
		fprintf(stderr,"No input files!!!\n");
		exit(1);
	}	

	while( --argc > 0 )  /*Loop on all the input files*/
	{
		IC=INITIAL_IC;
		DC=INITIAL_DC;
		initialize_sym_table(sym_head);
		strcpy(prefix,argv[argc]);

	    FILE_STRING(input_file,in_post)
	    FILE_STRING(output_file,out_post)
	    FILE_STRING(ext_file,ext_post)
	    FILE_STRING(ent_file,ent_post)

		fp_in=fopen(input_file,"r");
		CHECK_OPEN(fp_in,argv[argc])


/*Initializing data structures before scanning the file*/

/*Initializing Instruction table(IC)*/	
	putchar('\n');
	for (i=INITIAL_IC;i<MEMORY_SIZE;i++)
	{
		main_table[i].dec_add=i;
		dec_to_weird(main_table[i].weird_four_add,
				  	 		main_table[i].dec_add);

	 for(j=0;j<WORD_SIZE;j++)	
	 	main_table[i].binary_op[j]='0';
	}

/*Initializing Data table (DC)*/	
	putchar('\n');
	for (i=0;i<MEMORY_SIZE/2;i++)
	{
		data_table[i].dec_add=i;
		dec_to_weird(data_table[i].weird_four_add,
				  	 data_table[i].dec_add);

	 for(j=0;j<WORD_SIZE;j++)	
	 	data_table[i].binary_op[j]='0';

	}
/*Initializing Symbols table*/	
		sym_head=sym_alloc();
		sym_head->next=NULL;
	
/****************************************FIRST SCAN****************************************/
		printf("********** Input file: %s **********\n\n",input_file);
		error = first_scan(fp_in,sym_head,IC_p,DC_p,&IC,&DC);

		if(!error)
		{
			fp_out=fopen(output_file,"w+");
			CHECK_OPEN(fp_out,output_file)
		}
		else continue;
/****************************************SECOND SCAN****************************************/
		IC=INITIAL_IC;
		rewind(fp_in);
		error = second_scan(fp_in,sym_head,IC_p,DC_p,&IC,&DC);
		if(error) continue;
/*		print_sym_table(sym_head);*/

/*Printing IC Table*/
 		dec_to_weird(str_print,(IC-INITIAL_IC-1));
		fprintf(fp_out," %s ",str_print);
 		dec_to_weird(str_print,DC);
		fprintf(fp_out," %s\n",str_print);

		for (i=INITIAL_IC;i<(IC-1);i++)
		{
			bin_to_weird(main_table[i].binary_op,main_table[i].weird_four_op);
			/*fprintf(fp_out,"%d\t%s\t%s\t%s\n",*/
			fprintf(fp_out,"%s\t%s\n",
											/*	main_table[i].dec_add,*/
												main_table[i].weird_four_add,
											/*	main_table[i].binary_op,*/
												main_table[i].weird_four_op);
								
		}

/*Re-Initializing Data table (DC)*/	
	putchar('\n');
	for (i=0;i<DC;i++)
	{
		data_table[i].dec_add=i+IC-1;
		dec_to_weird(data_table[i].weird_four_add,
				  	 data_table[i].dec_add);
	}

/****************************************END SCANS****************************************/
		if (error) continue;
/****************************************PRINTING OUTPUT****************************************/
/*Printing DC Table*/
	for (i=0;i<DC;i++)
	{
		bin_to_weird(data_table[i].binary_op,data_table[i].weird_four_op);
/*		fprintf(fp_out,"%d\t%s\t%s\t%s\n",	*/
		fprintf(fp_out,"%s\t%s\n",	
											/*data_table[i].dec_add,*/
											data_table[i].weird_four_add,
										/*	data_table[i].binary_op,*/
											data_table[i].weird_four_op);
	}
	
/*Extern log: writhing file.ext */	
		fp_ext=fopen(ext_file,"w+");
		CHECK_OPEN(fp_ext,ext_file)

	    for(tmp=sym_head;tmp->next!=NULL;tmp=tmp->next)
			if (tmp->is_ext)
			{		
				strcpy(str_tmp,tmp->label);			
				for(i=INITIAL_IC;i<(IC-1);i++)
				{	
					if (!strcmp(main_table[i].label,tmp->label))
					{
						dec_to_weird(str_print,main_table[i].dec_add);
						fprintf(fp_ext,"%s\t%6s\n",str_tmp,str_print);
					}
				}

			}

/*Entry log: writhing file.ent*/	
		fp_ent=fopen(ent_file,"w+");
		CHECK_OPEN(fp_ent,ent_file)

	    for(tmp=sym_head;tmp->next!=NULL;tmp=tmp->next)
			if (tmp->is_ent)
			{							
				dec_to_weird(str_print,tmp->dec_add);
				fprintf(fp_ent,"%s\t%6s\n",tmp->label,str_print);
			}

	}/*end of while(--argc...)*/
	
return 0;
}/*End of main*/




/*This is an optional function for debugging and clarity of the work flow*/
/*It prints the symbol table*/
void print_sym_table(sym_row_p head)
{
	sym_row_p tmp;
	printf("\n****CONTENT OF SYMBOL TABLE****\n");

		printf("%6s\t%6s\t%6s\t%6s\t%6s\n",
		"Label","add","ent","ext","data");

    for(tmp=head;tmp->next!=NULL;tmp=tmp->next)
	{	
		printf("%6s\t%6d\t%6d\t%6d\t%6d\n"
		,tmp->label,tmp->dec_add,tmp->is_ent,
		tmp->is_ext,tmp->is_data_op);	
	}

}

sym_row_p sym_alloc(void)
{
/*This function allocates dynamic memory for a row in the symbol table*/
	sym_row_p node;
	node=(sym_row_p)malloc(sym_size);
	if (!node)
	{
		printf("Failed to allocate memory!!!\n");
		exit(1);
	}

	else return node;
}

void initialize_sym_table(sym_row_p head)
{
/*This functioni frees dynamic memory previously allocated for a the symbol table*/
	sym_row_p tmp;
	while(head != NULL )
	{
		tmp=head;
		head=head->next;
		free(tmp);
	}
}
