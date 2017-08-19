#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "aux_func.h"
#include "scan.h"
#include "parser.c"
#include "encoding.c"
#include "encoding_first_scan.c"

bool 		is_comment(char *arr,char *arr_tmp);
bool 		is_empty(char *arr);
bool 		check_op(char *op_string,bool*,bool*,bool*,bool*);
bool 		check_label(char *label,sym_row_p head,bool* , bool);
char 		*tok_label(char * arr,char * arr_tmp,int label_pos,bool*);
char 		*tok_get(char *arr , char *arr_tmp);
void 		add_symbol(sym_row_p head, char *label,int IC,int DC,bool is_ext, bool is_data_op);
int 		return_label_address(char *label,sym_row_p head);


bool first_scan(FILE *fp , sym_row_p sym_head, I_row_p IC_table , D_row_p DC_table )
{
	int IC,DC  /*Counters*/
		,row_num,i,
		op_len;

	bool error,is_label,is_op,is_data_op,is_ext,is_ent;
	parser_table parser_t ;
	char row_buf[MAX_ROW_LEN];	
	char arr_tmp[MAX_ROW_LEN];	
	char label_buf[MAX_LABEL_SIZE*2];
	char *label ,*op_tok, *buf_p;
	

	IC=INITIAL_IC;
	DC=INITIAL_DC;

	sym_head->dec_add=IC;
	
/*Loop on input file*/
	
	error=NO;
	row_num=1;


/***************************************************************/	
/*DATA HANDLING*/
/*	strcpy (DC_table[5].binary_op,"0000000000");
	printf("DATA IN BINARY:%s\n",DC_table[5].binary_op);*/
	
/*	DC+=IC;

	dec_to_bin('a',DC_table[DC].binary_op,WORD_LEN);
	bin_to_weird(DC_table[DC].binary_op,DC_table[DC].weird_four_op);
	printf("WEIRD:%s\n",DC_table[DC].weird_four_op);

	dec_to_bin('b',DC_table[DC+1].binary_op,WORD_LEN);
	bin_to_weird(DC_table[DC+1].binary_op,DC_table[DC+1].weird_four_op);
	printf("WEIRD:%s\n",DC_table[DC+1].weird_four_op);

	dec_to_bin('c',DC_table[DC+2].binary_op,WORD_LEN);
	bin_to_weird(DC_table[DC+2].binary_op,DC_table[DC+2].weird_four_op);
	printf("WEIRD:%s\n",DC_table[DC+2].weird_four_op);

	dec_to_bin('d',DC_table[3].binary_op,WORD_LEN);
	bin_to_weird(DC_table[3].binary_op,DC_table[3].weird_four_op);
	printf("WEIRD:%s\n",DC_table[3].weird_four_op);

	dec_to_bin('e',DC_table[4].binary_op,WORD_LEN);
	bin_to_weird(DC_table[4].binary_op,DC_table[4].weird_four_op);
	printf("WEIRD:%s\n",DC_table[4].weird_four_op);

	dec_to_bin('f',DC_table[5].binary_op,WORD_LEN);
	bin_to_weird(DC_table[5].binary_op,DC_table[5].weird_four_op);
	printf("WEIRD:%s\n",DC_table[5].weird_four_op);

	dec_to_bin('\0',DC_table[6].binary_op,WORD_LEN);
	bin_to_weird(DC_table[6].binary_op,DC_table[6].weird_four_op);
	printf("WEIRD:%s\n",DC_table[6].weird_four_op);

	dec_to_bin(6,DC_table[7].binary_op,WORD_LEN);
	bin_to_weird(DC_table[7].binary_op,DC_table[7].weird_four_op);
	printf("WEIRD:%s\n",DC_table[7].weird_four_op);

	dec_to_bin(-9,DC_table[8].binary_op,WORD_LEN);
	bin_to_weird(DC_table[8].binary_op,DC_table[8].weird_four_op);
	printf("WEIRD:%s\n",DC_table[8].weird_four_op);

	dec_to_bin(15,DC_table[9].binary_op,WORD_LEN);
	bin_to_weird(DC_table[9].binary_op,DC_table[9].weird_four_op);
	printf("WEIRD:%s\n",DC_table[9].weird_four_op);

	dec_to_bin(22,DC_table[10].binary_op,WORD_LEN);
	bin_to_weird(DC_table[10].binary_op,DC_table[10].weird_four_op);
	printf("WEIRD:%s\n",DC_table[10].weird_four_op);

	dec_to_bin(1,DC_table[11].binary_op,WORD_LEN);
	bin_to_weird(DC_table[11].binary_op,DC_table[11].weird_four_op);
	printf("WEIRD:%s\n",DC_table[11].weird_four_op);

	dec_to_bin(2,DC_table[12].binary_op,WORD_LEN);
	bin_to_weird(DC_table[12].binary_op,DC_table[12].weird_four_op);
	printf("WEIRD:%s\n",DC_table[12].weird_four_op);

	dec_to_bin(3,DC_table[13].binary_op,WORD_LEN);
	bin_to_weird(DC_table[13].binary_op,DC_table[13].weird_four_op);
	printf("WEIRD:%s\n",DC_table[13].weird_four_op);

	dec_to_bin(4,DC_table[14].binary_op,WORD_LEN);
	bin_to_weird(DC_table[14].binary_op,DC_table[14].weird_four_op);
	printf("WEIRD:%s\n",DC_table[14].weird_four_op);
*/
/***************************************************************/

	while(fgets(row_buf,MAX_ROW_LEN,fp) !=NULL)
	{
		buf_p=row_buf; /*Global pointer to search the recorded row*/
		op_len=0;

		is_op=NO;	
		is_data_op=NO;	
		is_label=NO;	
		is_ext=NO;	
		is_ent=NO;	


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

		/*	3)CHECK AND SAVE LABELS	(IF APPEAR AT BEGINNING OF LINE)*/

		label=tok_label(row_buf,arr_tmp,START,&error);	
		if(label!=NULL)
		{
			no_space(label);			
			strcpy(label_buf,label);
			printf("in row#%d Found label %s\n",row_num,label);
			buf_p=strchr(row_buf,':')+1;
			is_label=check_label(label_buf,sym_head,&error,NO);
		}
		else is_label=NO;				
	/*		4)IS DATA INSTRUCTION? .data, .string .mat?	
			Or one of the 15 operations	*/																	
		op_tok=tok_get(buf_p,arr_tmp);

		if(check_op(
		op_tok,&is_op,&is_data_op,&is_ext,&is_ent))
		{
			printf("in row#%d THE OPERATION IS: %s\n",row_num,op_tok);
			buf_p=strstr(row_buf,op_tok);
			op_len=strlen(op_tok);
			buf_p+=op_len;
			printf("in row#%d THE ARGUMENT STRING IS: %s\n",row_num,buf_p);
			
						/*SEND TO ARGUMENT PARSING*/

			if (	(!is_ext) && (!is_ent)	)
			{
			    for ( i = 0 ; i < OP_NUM ; i++)
				{
				    
				    if(strcmp(op_list[i].name,op_tok) == 0)
					break;
				  /*  printf("\n %s != %s \n" , op_list[i].name , op_tok); */
				}

			    parser(buf_p , &parser_t);
			    /*printf("\n THE  i%d  OPERATION IS: %d TYPE IS %d  \n",i,op_list[i].EnuM,parser_t.type[0]); */
			    encoding_first_scan( &op_list[i],  DC_table ,  IC_table ,  sym_head ,&parser_t , &DC , &IC);
			}


		}
		else
		{ 
			printf("ERROR ,in row#%d OPERATION %s DOESN'T EXIST\n",row_num,op_tok);				
			error=YES;
			row_num++;
			continue;	
		}
	
	/*MANAGING EXTERNAL LABELS*/
	if(is_ext)
	{
		arr_tmp[0]='\0';
		label = tok_label(buf_p,arr_tmp,MID,&error); /*Call tok_label function to search in the middle of the line*/
		if(label!=NULL)
		{
			no_space(label);			
			strcpy(label_buf,label);
			printf("in row#%d Found label %s\n",row_num,label);
			is_label=check_label(label_buf,sym_head,&error,NO);
		}
		else is_label=NO;			
	}

	if (is_label)		
			add_symbol(sym_head,label_buf,IC,DC,is_ext,is_data_op);	
	
	
		row_num++; /*Line ends*/
	} /*End of while(fgets...*/   


	if (error)				/*End of lines*/
	{
		printf("ERRORS FOUND IN INPUT FILE!!!\n");					
	}
	
	return error;
	/*print_sym_table(sym_head);*/
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

void dec_to_weird (char *quad_num ,int dec_num)
{   /* Convert decimal int to base four string*/
   int i;
   int base;
   base=WEIRD_BASE;
   for(i=0;i<base;i++)
   {
	 quad_num[i] = (dec_num % base) + 'a';
	 dec_num = dec_num / base;
   }

   	quad_num[base]='\0';
	reverse(quad_num);

}


bool is_comment(char *arr , char *arr_tmp)
	{
		strcpy(arr_tmp,arr);
		no_space(arr_tmp);

		if (*arr_tmp==';') return YES;
		else return NO;	
	}

bool is_empty(char *arr)
	{
		int i,empty_flag;
		empty_flag=1;
		for (i=0; (i<MAX_ROW_LEN) && (arr[i]!='\0') ;i++)
		{
			if( isspace(arr[i]) ) continue;
			else empty_flag=0;
		}
		return empty_flag;
	}


char *tok_label(char *arr,char *arr_tmp,int label_pos,bool *error)
{

	/*If row starts with a label*/
	if (label_pos == START)
	{
		strcpy(arr_tmp,arr);
	/*	printf("%s\n",arr_tmp);*/
		strtok(arr_tmp,":");

		if (!(strcmp(arr_tmp,arr)))
			return NULL;	
		else 
			return arr_tmp;	
	}
	
	/*If label appears after an external op*/
	else if (label_pos == MID)
	{
		arr_tmp=tok_get(arr,arr_tmp);
		no_space(arr_tmp);

		if (*arr_tmp == '\0') 
		{	
			printf("ERROR!LABEL after extern is empty!!! expected label name\n");
			*error=YES;
			return NULL;
		}
		return arr_tmp;		
	}
	return NULL;
}


void no_space(char *str)
{
   char* p1 = str;
   char* p2 = str;
   while(*p2 != '\0')
   {
     *p1 = *p2++;
       if(	((*p1)!='\t')  && ((*p1)!=(' '))) 
       	  p1++;
	   
   }
   *p1 = '\0';
}


void add_symbol(sym_row_p head,char *label,int IC,int DC,
				bool is_ext, bool is_data_op)
{
	sym_row_p tmp;
	if (head->next==NULL)
	{
		strcpy(head->label,label);
		head->is_ext=is_ext;
		head->is_data_op=is_data_op;	
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

			if (is_data_op)
				tmp->dec_add=DC;
			else
				tmp->dec_add=IC;

			tmp->is_ext=is_ext;
			tmp->is_data_op=is_data_op;	
			tmp->next=sym_alloc();
        	tmp->next->next=NULL;
	}
	
        
}



char *tok_get(char *arr , char *arr_tmp)
{
	int i,j;
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

bool check_op(  char *op_string,
				bool *is_op, 
				bool *is_data_op,
				bool *is_ext, 
				bool *is_ent)
{
	int i;

	no_space(op_string);
	for (i=0;strcmp(op_words[i],LAST);i++)
	{
		if(!strcmp(op_string,op_words[i]))			
		{
			*is_op=YES;				
			return YES;				
		}
	}
	
	if( !strcmp(op_string,".extern") ) 
	{
		*is_ext=YES;
		return YES;
	}


	if( !strcmp(op_string,".entry") ) 
	{
		*is_ent=YES;	
		return YES;		
	}

	for (i=0;strcmp(data_op_words[i],LAST);i++)
	{
		if(!strcmp(op_string,data_op_words[i]))			
		{
			*is_data_op=YES;		
			return YES;			
		}
	}
	
	return NO;
}

bool check_label(char *label,sym_row_p head,bool *error , bool should_exists)
{
/*This function tests if a label is OK
it tests:
	1) A letter at the first char
	2) Label length
	3) If reserved word
	4) If already exists in symbol_table
*/
	int i;
	sym_row_p tmp; 
	bool exists = NO;
/*1*/
	if (!isalpha(label[0]))
	{ 
		printf
		("LABEL: %s is illegal!\nA LABEL has to start with a letter\n",label);
		*error=YES;
		return NO;
	}

/*2*/
	if (strlen(label)>MAX_LABEL_SIZE) 
	{
		printf
		("LABEL: %s is illegal!\nA LABEL cannot be longer than %d\n",label,MAX_LABEL_SIZE);
		*error=YES;
		return NO;
	}	
/*3*/
	for (i=0;strcmp(reserved_words[i],LAST);i++);
	{
		if (!strcmp(label,reserved_words[i]))
		{
			printf
			("LABEL: %s is illegal!\n%s is a reserved word!\n",label,reserved_words[i]);
			*error=YES;
			return NO;
		}
	}
/*4*/	

    for(tmp=head;tmp->next!=NULL;tmp=tmp->next)
	{	
		if (!strcmp(label,tmp->label))
		{
			exists = YES;
		}
	}
    if (should_exists)
    {
	if(exists)
	return NO;
	printf("LABEL: %s does not exists!!!\n",label);
	*error=YES;
	return YES;
    }
    else 
    {
	if(exists)
	{
	    printf
	    ("LABEL: %s already exists!!!\n",label);
	    *error=YES;
	    return NO;
	}    
    }
    return YES;

}

int return_label_address(char *label,sym_row_p head)
{
    sym_row_p tmp; 

    for(tmp=head;tmp->next!=NULL;tmp=tmp->next)
	{	
		if (!strcmp(label,tmp->label))
		{
		    return tmp->dec_add ; 	
		}
	}
    return 0;

}


void dec_to_bin(int dec_num,char* word, int size)
{
	int i;
	for (i=size ;i>0;i--)
	{
		if( (dec_num&( 1<<(i-1) )) !=0)
		{
			word[size-i]='1';	
		}
		else
		{
			word[size-i]='0';	
		}	
	}			
}

void bin_to_weird(char *bin,char *weird)
{
	int i;
	int dec;
	for (i=0;i<WORD_SIZE;i+=2)
	{
		dec=(bin[i] - '0')*2+(bin[i+1]-'0');
		weird[i/2]=('a'+dec);
	}
}
