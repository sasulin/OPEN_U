#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "aux_func.h"

#define INITIAL_IC 100
#define INITIAL_DC 0

const short int sym_size = sizeof(symbol_row);

int is_comment(char *arr , char *arr_tmp);
int is_empty(char *arr);
char *tok_label(char * arr,char * arr_tmp);
void no_space(char *str);

void first_scan(FILE *fp)
{

	int IC,DC,row; /*Counters*/
	bool error,is_label;
	char row_buf[MAX_ROW_LEN];	
	char arr_tmp[MAX_ROW_LEN];	
	char *label;
	symbol_p sym_head;
	
	sym_head=(symbol_p)malloc(sym_size);
	sym_head->next=NULL;

	IC=INITIAL_IC;
	DC=INITIAL_DC;

/*Loop on input file*/
	
	error=NO;
	row=1;

	while(fgets(row_buf,MAX_ROW_LEN,fp) !=NULL)
	{
		/*	1) CHECK AND IGNORE COMMENT LINES*/	
		if (is_comment(row_buf,arr_tmp))
			continue;

		/*	2) CHECK AND IGNORE EMPTY LINES	*/
/*		if (is_empty(row_buf))
			continue;
*/	
		/*	3)CHECK AND SAVE LABELS	*/
		is_label=NO;	
		label=tok_label(row_buf,arr_tmp);
		if(label!=NULL)
		{
			is_label=YES;	
			printf("row #%d, %s\n",row,label);
		}
		else
		{ 
			is_label=NO;	
		/*	printf("NO LABEL FOUND!\n");*/
		}
	/*		4)IS DATA INSTRUCTION? .data, .string .mat?	*/

		row++;
	}   

	if (error)				/*End of lines*/
		exit(1);

}


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

int is_comment(char *arr , char *arr_tmp)
	{
		strcpy(arr_tmp,arr);
		no_space(arr_tmp);

		if (*arr_tmp==';')
		{
			printf("COMMENT LINE\n");
			return 1;
		}
		else return 0;	
	}

int is_empty(char *arr)
	{
		int i;
		for (i=0; (i<MAX_ROW_LEN) && (arr[i]!='\0') ;i++)
		{
			if( isspace(arr[i]) )
				continue;
			else 
				return 0;
		}
			printf("EMPTY LINE\n"); /*NEVER REACHES HERE*/
			return 1;
	}


char *tok_label(char *arr , char *arr_tmp)
{
	strcpy(arr_tmp,arr);
/*	printf("%s\n",arr_tmp);*/
	strtok(arr_tmp,":");
/*	printf("%s\n",arr_tmp);*/

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
