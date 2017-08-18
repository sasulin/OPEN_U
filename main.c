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

	int i;
	char prefix[FILE_NAME_LEN];
	char input_file[FILE_NAME_LEN];
	char output_file[FILE_NAME_LEN];
	char ext_file[FILE_NAME_LEN]; 
	char ent_file[FILE_NAME_LEN];	
	bool error;
	sym_row_p sym_head;

	FILE *fp_in;
	FILE *fp_out;
	FILE *fp_ext;
	FILE *fp_ent;

	error=NO;;
	IC_p = &main_table[0];
	DC_p = &data_table[0];

	if (argc==1) /*No arguments*/
	{
		fprintf(stderr,"No input files!!!\n");
		exit(1);
	}	

	while( --argc > 0 )
	{
		strcpy(prefix,argv[argc]);

	    FILE_STRING(input_file,in_post)
	    FILE_STRING(output_file,out_post)
	    FILE_STRING(ext_file,ext_post)
	    FILE_STRING(ent_file,ent_post)

/*		strcpy(input_file,prefix);
		strcat(input_file,in_post);
		strcpy(output_file,prefix);
		strcat(output_file,out_post);
		strcpy(ext_file,prefix);
		strcat(ext_file,ext_post);
		strcpy(ent_file,prefix);
		strcat(ent_file,ent_post);*/
	
		fp_in=fopen(input_file,"r");
		CHECK_OPEN(fp_in,argv[argc])
	
/*		if (!fp_in)
		{
			fprintf(stderr,
			"Cannot open %s	No such input file!!!\n", argv[argc]);
			exit(1);
		}*/
		printf("input file:%s\n****************\n",input_file);


/*Initializing Instruction table (IC)*/	
	putchar('\n');
	for (i=100;i<MEMORY_SIZE;i++)
	{
		main_table[i].dec_add=i;
		dec_to_weird(main_table[i].weird_four_add,
				  	 main_table[i].dec_add);
	}


/*Initializing Data table (DC)*/	
	putchar('\n');
	for (i=0;i<MEMORY_SIZE/2;i++)
	{
		data_table[i].dec_add=i;
		dec_to_weird(data_table[i].weird_four_add,
				  	 data_table[i].dec_add);
	}


/*Initializing Symbols table*/	
		sym_head=sym_alloc();
		sym_head->next=NULL;
	

		error = first_scan(fp_in,sym_head,IC_p,DC_p);
		print_sym_table(sym_head);
		initialize_sym_table(sym_head);
		/*	second_scan(fp_in);*/

		if (error) continue;
			
		fp_out=fopen(output_file,"w+");
		CHECK_OPEN(fp_out,output_file)
	
/*		if (!fp_out)
		{
			fprintf(stderr,
			"Cannot open %s\n",output_file);
			exit(1);
		}*/

		fprintf(fp_out,"input file:%s\n****************\n",input_file);	

		for (i=100;i<MEMORY_SIZE;i++)
		{
			fprintf(fp_out,"%d\t%s\n",main_table[i].dec_add,
			main_table[i].weird_four_add);
		}


		for (i=0;i<MEMORY_SIZE/2;i++)
		{
			fprintf(fp_out,"%d\t%s\t%s\n",data_table[i].dec_add,
			data_table[i].weird_four_add,data_table[i].weird_four_op);
		}


		fp_ext=fopen(ext_file,"w+");
		CHECK_OPEN(fp_ext,ext_file)
	
/*		if (!fp_ext)
		{
			fprintf(stderr,
			"Cannot open %s\n", ext_file);
			exit(1);
		}	*/

		fprintf(fp_ext,"input file:%s\n****************\n",input_file);

		fp_ent=fopen(ent_file,"w+");
		CHECK_OPEN(fp_ent,ent_file)

/*		if (!fp_ent)
		{
			fprintf(stderr,
			"Cannot open %s\n", ent_file);
			exit(1);
		}*/	

		fprintf(fp_ent,"input file:%s\n****************\n",input_file);
	}/*end of while(--argc...)*/
	
return 0;
}

void print_sym_table(sym_row_p head)
{
	sym_row_p tmp;
	printf("\n****CONTENT OF SYMBOL TABLE****\n");

		printf("%6s\t%6s\t%6s\t%6s\n",
		"Label","add","ext","data");

    for(tmp=head;tmp->next!=NULL;tmp=tmp->next)
	{	
		printf("%6s\t%6d\t%6d\t%6d\n"
		,tmp->label,tmp->dec_add,
		tmp->is_ext,tmp->is_data_op);	
	}

}

sym_row_p sym_alloc(void)
{
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
	sym_row_p tmp;
	printf("\n****INITIALIZING SYMBOL TABLE****\n");
	while(head != NULL )
	{
		tmp=head;
		head=head->next;
		free(tmp);
	}

}
