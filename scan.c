/*MAMAN 14 Final Project*/
/*Shmuel Asulin ,ID:036760676  */
/*Yotam Klein* , ID:066546896 */

/*Assembler*/

/*This file contains the algorithm for the two scans, diveded into two
main functions: first_scan & second scan, each dependent on several more large
source file: 
			parser.c : parses the command arguments
			encoding_first_scan.c : Operates the IC & DC counters, and encodes the data table.
			encoding_first_scan.c   Encodes the main(Instructions)
and two header files: aux.h & scan.h*/

/*The file also contains several small aux functions*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "aux_func.h"
#include "scan.h"
#include "parser.c"
#include "encoding_first_scan.c"
#include "encoding_second_scan.c"

/***************************************************************/	
bool first_scan(FILE *fp,sym_row_p sym_head,I_row_p IC_table,D_row_p DC_table,int *IC,int *DC)
{
		int IC_NOW,DC_NOW,  /*Counters*/
		row_num,i,
		op_len;

	bool error,is_label,is_op,is_data_op,is_ext,is_ent; /*Boolean flags*/
	parser_table parser_t ;
	sym_row_p tmp;
	char row_buf[MAX_ROW_LEN];	
	char arr_tmp[MAX_ROW_LEN];	
	char label_buf[MAX_LABEL_SIZE*2];
	char *label ,*op_tok, *buf_p;
	
	sym_head->dec_add=*IC;
	
/*Loop on input file*/
	
	error=NO;
	row_num=1;

/***************************************************************/	
	while(fgets(row_buf,MAX_ROW_LEN,fp) !=NULL) /*Loops through all input lines*/
	{

		buf_p=row_buf; /*Global pointer to search current line*/
		op_len=0;

		is_op=NO;	
		is_data_op=NO;	
		is_label=NO;	
		is_ext=NO;	
		is_ent=NO;	

		IC_NOW=*IC;
		DC_NOW=*DC;

		/*	1) CHECK AND IGNORE COMMENT LINES*/	
		if (is_comment(row_buf,arr_tmp))
		{
			row_num++;
			continue;
		}
		/*	2) CHECK AND IGNORE EMPTY LINES	*/	
		if (is_empty(row_buf))
		{
			row_num++;
			continue;
		/*	3)CHECK AND SAVE LABELS	(IF APPEAR AT BEGINNING OF LINE)*/
		label=tok_label(row_buf,arr_tmp,START,&error);	
		if(label!=NULL)
		{
			no_space(label);			
			strcpy(label_buf,label);
			buf_p=strchr(row_buf,':')+1;
			is_label=check_label(label_buf,sym_head,&error,NO,row_num);
		}
		else is_label=NO;				
	/*		4)IS DATA INSTRUCTION? .data, .string .mat?	
			Or one of the other operations(INSTRUCTION)	*/																	
		op_tok=tok_get(buf_p,arr_tmp);

		if(check_op(
		op_tok,&is_op,&is_data_op,&is_ext,&is_ent))
		{
			buf_p=strstr(row_buf,op_tok);
			op_len=strlen(op_tok);
			buf_p+=op_len;
			
			if (  (!is_ext) && (!is_ent)	)
			{
			    for ( i = 0 ; i < OP_NUM ; i++)
				{    
				    if(strcmp(op_list[i].name,op_tok) == 0)
					break;
				}
				/*SEND TO ARGUMENT PARSING*/
			    error = (error || parser(buf_p ,&parser_t , row_num));
				/*SEND TO ENCODING*/
			    error=  (error || encoding_first_scan(&op_list[i],DC_table,IC_table,sym_head,
									&parser_t,DC,IC,row_num));
			}


		}
		else
		{ 
			printf("ERROR! in row#%d: OPERATION %s DOESN'T EXIST\n",row_num,op_tok);				
			error=YES;
			row_num++;
			continue;	
		}

	if(is_label && is_ext)
		printf("WARNING!in row#%d: Ignoring Labels before \".extern\" command\n",row_num);

	
	/*5) MANAGING EXTERNAL LABELS*/
	if(is_ext)
	{
		arr_tmp[0]='\0';
		label = tok_label(buf_p,arr_tmp,MID,&error); 
		if(label!=NULL)
		{
			no_space(label);			
			strcpy(label_buf,label);
			is_label=check_label(label_buf,sym_head,&error,NO,row_num);
		}
		else is_label=NO;			
	}

	if (is_label) /*If label is OK then add it to the symbol table*/
	{	
			if(!is_ent) /*Entry labels are enterned in the second scan*/
				add_symbol(sym_head,label_buf,IC_NOW,DC_NOW,is_ent,is_ext,is_data_op);	
			else 
				printf("WARNING!in row#%d: Ignoring Labels before \".entry\" commands\n",row_num);
	}	
		row_num++; /*Line ends*/
	} /*End of while(fgets...*/   


	--(*IC); /*IC is always one step ahead*/
/*Adding IC final value to DC value*/
    for(tmp=sym_head;tmp->next!=NULL;tmp=tmp->next)
	{	
		if (tmp->is_data_op==YES)
				tmp->dec_add+=(*IC);
	}

	if (error)	/*End of input lines*/
		printf("ERRORS FOUND IN INPUT FILE!!!\n");					

	}/*End of while*/
	return error;
}/*End of First Scan*/


/***************************************************************/	
bool second_scan(FILE *fp,sym_row_p sym_head,I_row_p IC_table,D_row_p DC_table,int *IC,int *DC)
{

	int row_num,i,op_len;

	bool error,is_label,is_op,is_data_op,is_ext,is_ent;
	parser_table parser_t ;
	char row_buf[MAX_ROW_LEN];	
	char arr_tmp[MAX_ROW_LEN];	
	char label_buf[MAX_LABEL_SIZE*2];
	char *label;
	char *op_tok, *buf_p;
    sym_row_p tmp; 
	
/*Loop on input file*/
	
	error=NO;
	row_num=1;

/***************************************************************/	
	while(fgets(row_buf,MAX_ROW_LEN,fp) !=NULL)
	{

		buf_p=row_buf; 
		op_len=0;

		is_op=NO;	
		is_data_op=NO;	
		is_label=NO;	
		is_ext=NO;	
		is_ent=NO;	

		/*	1) CHECK AND IGNORE COMMENT LINES*/	
		if (is_comment(row_buf,arr_tmp))
		{
			row_num++;
			continue;
		}
		/*	2) CHECK AND IGNORE EMPTY LINES	*/	
		if (is_empty(row_buf))
		{
			row_num++;
			continue;
		}

/*In the second scan we ignore the labels that were already added in the first scan*/
	strcpy(arr_tmp,row_buf);
	strtok(arr_tmp,":");	
		if (strcmp(arr_tmp,row_buf))
			buf_p=strchr(row_buf,':')+1;

		/*	3) PARSING INSTRUCTIONS BEFORE FINAL ENCODING	*/	
		/*	IS DATA INSTRUCTION? .data, .string .mat?	
			Or one of the other operations(INSTRUCTION)	*/					
												
		op_tok=tok_get(buf_p,arr_tmp);
		if(check_op(op_tok,&is_op,&is_data_op,&is_ext,&is_ent))
		{
			buf_p=strstr(row_buf,op_tok);
			op_len=strlen(op_tok);
			buf_p+=op_len;
			

			if (	(!is_ext) && (!is_ent)	)
			{
			    for ( i = 0 ; i < OP_NUM ; i++)
				{ 
				    if(strcmp(op_list[i].name,op_tok) == 0)
					break;
				}

				/*SEND TO ARGUMENT PARSING*/
			    error = (error || parser(buf_p , &parser_t , row_num));
				/*SEND TO ENCODING*/
			    error = (error || encoding_second_scan(&op_list[i],DC_table,IC_table,sym_head,
									&parser_t,DC,IC,row_num));
			}


		}
		else
		{ 
			printf("ERROR! in row#%d OPERATION %s DOESN'T EXIST\n",row_num,op_tok);				
			error=YES;
			row_num++;
			continue;	
		}
	
/*Managing labels after ".entry"*/
	if(is_ent)
	{
		arr_tmp[0]='\0';
		label = tok_label(buf_p,arr_tmp,MID,&error); 
		if(label!=NULL)
		{
			no_space(label);			
			strcpy(label_buf,label);
			is_label=check_label(label_buf,sym_head,&error,YES,row_num);
		}
		else is_label=NO;			
	}
	
	if (is_label)	
	{
    	for(tmp=sym_head;tmp->next!=NULL;tmp=tmp->next)
		{
			if (!strcmp(label,tmp->label))
			{
		    	tmp->is_ent=YES; 	
				break;
			}
		}
	}
	
		row_num++; /*Line ends*/
	} /*End of while(fgets...*/   

	return error;
}/*End of Second Scan*/

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
			printf("ERROR! LABEL after extern is empty!!! expected label name\n");
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


void add_symbol(sym_row_p head,char *label,int IC,int DC,bool is_ent,
				bool is_ext, bool is_data_op)
{
	sym_row_p tmp;
	if (head->next==NULL)
	{

		if (is_data_op)
			head->dec_add=DC;
		else
			head->dec_add=IC;
		if(is_ext)
		{
			head->is_data_op=NO;
			head->dec_add=0;			
		}	

		strcpy(head->label,label);
		head->is_ent=is_ent;
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

/*Updating counters*/
			if (is_data_op)
				tmp->dec_add=DC;
			else
				tmp->dec_add=IC;
			if(is_ext)
				tmp->dec_add=0;			

			tmp->is_ent=is_ent;
			tmp->is_ext=is_ext;
			tmp->is_data_op=is_data_op;	
			tmp->next=sym_alloc();
        	tmp->next->next=NULL;
	}
	
        
}


char *tok_get(char *arr,char *arr_tmp)
{
	int i,j;
	for(i=0;(i<MAX_ROW_LEN) && (arr[i]!='\0');i++)
	{	
		if(isspace(arr[i]))
			continue;
		else
		{
			for(j=0;(j<MAX_LABEL_SIZE);i++,j++)	
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

bool check_label(char *label,sym_row_p head,bool *error , bool should_exists,int row_num)
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
		("ERROR! in row#%d:LABEL %s is illegal!\nA LABEL has to start with a letter\n",row_num,label);
		*error=YES;
		return NO;
	}

/*2*/
	if (strlen(label)>MAX_LABEL_SIZE) 
	{
		printf
		("ERROR! in row#%d:LABEL %s is illegal!\nA LABEL cannot be longer than %d\n",row_num,label,MAX_LABEL_SIZE);
		*error=YES;
		return NO;
	}	
/*3*/
	for (i=0;strcmp(reserved_words[i],LAST);i++);
	{
		if (!strcmp(label,reserved_words[i]))
		{
			printf
			("ERROR! in row#%d:LABEL %s is illegal!\n%s is a reserved word!\n",row_num,label,reserved_words[i]);
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
	if(exists) return YES;
	printf("ERROR! in row#%d:LABEL %s does not exists!!!\n",row_num,label);
	*error=YES;
	return NO;
    }
    else 
    {
	if(exists)
	{
	    printf("ERROR! in row#%d:LABEL %s already exists!!!\n",row_num,label);
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


void dec_to_weird (char *quad_num ,int dec_num)
{   /* Convert decimal int to weird base string*/
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
