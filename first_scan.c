#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "aux_func.h"

#define INITIAL_IC 100
#define INITIAL_DC 0
#define MAX_OP_LEN 8
#define OP_NUM 16
#define DATA_OP_NUM 5

const char *reserved_words[]={"mov","cmp","add",
					  "sub","not","clr",
	 				  "lea","inc","dec",
      				  "jmp","bne","red",
      				  "prn","jsr","rts","stop",
					  "r0","r1","r2","r3","r4","r5","r6","r7",
					  "data","string","mat",".entry",".extern","999"};

const char *op_words[]={"mov","cmp","add",
					  "sub","not","clr",
	 				  "lea","inc","dec",
      				  "jmp","bne","red",
      				  "prn","jsr","rts","stop"};

const char *data_op_words[]={".data",".string",".mat",".entry",".extern"};


const short int sym_size = sizeof(symbol_row);

bool is_comment(char *arr , char *arr_tmp);
bool is_empty(char *arr);
bool check_op (char *op_string, bool * , bool *);
bool check_label(char *label);
char *tok_label(char * arr,char * arr_tmp);
char *tok_op(char *arr , char *arr_tmp);
void no_space(char *str);
sym_row_p sym_alloc(void);
void add_symbol(sym_row_p head, char *label);
void print_sym_table(sym_row_p head);



void first_scan(FILE *fp)
{
	int IC,DC,row_num,row_len,label_len ,op_len; /*Counters*/
	bool error,is_label,is_op,is_data_op;
	char row_buf[MAX_ROW_LEN];	
	char arr_tmp[MAX_ROW_LEN];	
	char *label ,*op_tok, *buf_p;
	sym_row_p sym_head;

	sym_head=sym_alloc();
	sym_head->next=NULL;

	IC=INITIAL_IC;
	DC=INITIAL_DC;

/*Loop on input file*/
	
	error=NO;
	row_num=1;

	while(fgets(row_buf,MAX_ROW_LEN,fp) !=NULL)
	{
		buf_p=row_buf; /*Global pointer to search the recorded row*/
		row_len=strlen(row_buf);
		label_len=0;
		op_len=0;
		/*	1) CHECK AND IGNORE COMMENT LINES*/	
		if (is_comment(row_buf,arr_tmp))
		{
/*			printf("in row#%d Found comment\n",row_num);*/
			row_num++;
			continue;
		}
		/*	2) CHECK AND IGNORE EMPTY LINES	*/
		
		if (is_empty(row_buf))
		{
/*			printf("in row#%d Found empty line\n",row_num);*/
			row_num++;
			continue;
		}

		/*	3)CHECK AND SAVE LABELS	*/
		is_label=NO;	
		label=tok_label(row_buf,arr_tmp);
		
		if(label!=NULL)
		{
			is_label=YES;	
			no_space(label);			
			printf("in row#%d Found label %s\n",row_num,label);
			buf_p=strchr(row_buf,':')+1;
			/*check_label(label);*/
			add_symbol(sym_head,label);			
		}
		else
		{ 
			is_label=NO;	
		}
		


	/*		4)IS DATA INSTRUCTION? .data, .string .mat?	
			Or ne of the 15 operations	*/
																			
		is_op=NO;	
		is_data_op=NO;	
		op_tok=tok_op(buf_p,arr_tmp);

		if(check_op(op_tok,&is_op,&is_data_op))
		{
			printf("in row#%d THE OPERATION IS: %s\n",row_num,op_tok);
			buf_p=strstr(row_buf,op_tok);
			op_len=strlen(op_tok);
			buf_p+=op_len;
			printf("in row#%d THE ARGUMENT STRING IS: %s\n",row_num,buf_p);
						/*SEND TO ARGUMENT PARSING*/	
		}
		else
		{ 
			is_op=NO;
			is_data_op=NO;
			printf("in row#%d OPERATION %s DOESN'T EXIST\n",row_num,op_tok);				
			error=YES;
			row_num++;
			continue;	
		}
		
		row_num++;
	}   


	if (error)				/*End of lines*/
	{
		printf("ERRORS FOUND IN INPUT FILE!!!\n");					
		exit(1);
	}

	print_sym_table(sym_head);
} /*End of First Scan*/



void reverse (char *string) 
{
   int length, c;
   char *begin, *end, temp;
 
   length = strlen(string);
   begin  = string;
   end    = string;
 
   for (c = 0; c < length - 1; c++)
      end++;
 
   for (c = 0; c < length/2; c++)
   {        
      temp   = *end;
      *end   = *begin;
      *begin = temp;
 
      begin++;
      end--;
   }
}

void dec_to_quad (char *quad_num ,int dec_num)
{   /* convert decimal to base four */
   int i;
   for(i=0;i<4;i++)
   {
	 quad_num[i] = (dec_num % 4) + '0';
	 quad_weird(&quad_num[i]);
	 dec_num = dec_num / 4;
   }
   	quad_num[4]='\0';
	reverse(quad_num);
}

void quad_weird (char *quad_num)
{
		switch(*quad_num)
		{
		case('0'):
			*quad_num='a';
			break;
		case('1'):
			*quad_num='b';
			break;
		case('2'):
			*quad_num='c';
			break;
		case('3'):
			*quad_num='d';
			break;
	}
}

bool is_comment(char *arr , char *arr_tmp)
	{
		strcpy(arr_tmp,arr);
		no_space(arr_tmp);

		if (*arr_tmp==';')
		{
			/*printf("COMMENT LINE\n");*/
			return YES;
		}
		else return NO;	
	}

bool is_empty(char *arr)
	{
		int i,empty_flag;
		empty_flag=1;
		for (i=0; (i<MAX_ROW_LEN) && (arr[i]!='\0') ;i++)
		{
			if( isspace(arr[i]) )
				continue;
			else
				empty_flag=0;
		}
		return empty_flag;
	}


char *tok_label(char *arr , char *arr_tmp)
{
	strcpy(arr_tmp,arr);
/*	printf("%s\n",arr_tmp);*/
	strtok(arr_tmp,":");

	if (!(strcmp(arr_tmp,arr)))
		return NULL;	
	else 
		return arr_tmp;	
}

void no_space(char *str)
{
   char* p1 = str;
   char* p2 = str;
   while(*p2 != '\0')
   {
     *p1 = *p2++;
     /*  if(!isspace(*p1)) */
       if(	((*p1)!='\t')  && ((*p1)!=(' '))) 
       	  p1++;
	   
   }
   *p1 = '\0';
}


void add_symbol(sym_row_p head, char *label)
{
	sym_row_p tmp;
	if (head->next==NULL)
	{
		strcpy(head->label,label);		
		head->next=sym_alloc();
		head->next->next=NULL;
		return;
	}	
	else
	{
		tmp=head;
		while(tmp->next!=NULL)
			tmp=tmp->next;	
			strcpy(tmp->label,label);      
			tmp->next=sym_alloc();
        	tmp->next->next=NULL;
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

void print_sym_table(sym_row_p head)
{
	sym_row_p tmp;
	printf("\n\nCONTENT OF SYMBOL TABLE:\n");
    for(tmp=head;tmp->next!=NULL;tmp=tmp->next)
	{
		printf("Label is: %s\n",tmp->label);	
	}

}

char *tok_op(char *arr , char *arr_tmp)
{
	int i,j;
/*	printf("%s\n",arr);
*/
	for(i=0;(i<MAX_ROW_LEN) && (arr[i]!='\0');i++)
	{	
		if(isspace(arr[i]))
			continue;
		else
		{
			for(j=0;(j<MAX_OP_LEN);i++,j++)	
			{
				arr_tmp[j]=arr[i];		
				if(isspace(arr[i]))
				{
					arr_tmp[j]='\0'; 
					break;
				}
			}	
		}		
	break;			
	}
		return arr_tmp;
}

bool check_op(char *op_string,bool *is_op, bool *is_data_op)
{
	int i;
	for (i=0;i<OP_NUM;i++)
	{
		if(!strcmp(op_string,op_words[i]))			
		{
/*			printf("%s\n",op_string);*/
			*is_op=YES;				
			return YES;				
		}
	}
	
	for (i=0;i<DATA_OP_NUM;i++)
	{
		if(!strcmp(op_string,data_op_words[i]))			
		{
			/*printf("%s\n",op_string);*/
			*is_data_op=YES;		
			return YES;				
		}
	}
	/*printf("OPERATION %s DOESN'T EXIST\n",op_string);*/
	return NO;
}

bool check_label(char *label)
{
	int i;
    printf("Testing Label\n");


	if (!isalnum(label[0]))
	{ 
		printf("LABEL: %s is illigal! \
				 A LABEL has to start with a letter",label);
		return NO;
	}
	if (strlen(label)>30) 
	{
		printf("LABEL: %s is illigal! \
				 A LABEL cannot be longer than %d ",label,LABEL_SIZE);
		return NO;
	}
	
	for (i=0;strcmp(reserved_words[i],"999");i++);
	{
		if (!strcmp(label,reserved_words[i]))
		{
		printf("LABEL: %s is illigal! \
				 %s is a reserved word! ",label,reserved_words[i]);
		return NO;
		}
	}

	return YES;
}
