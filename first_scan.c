#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aux_func.h"

	int IC,DC; /*Counters*/

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

void parsing(char *line)
{
	printf("%s\n",line);
}
