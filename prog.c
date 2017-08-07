#include <stdio.h>
#include<string.h>
#include <ctype.h>
#include <stdlib.h>
#define MAX_NUM_OF_TOKENS 100
#define MAX_LABLE_NAME 100
enum status {FIRST,NUM,ALPHA,ERROR,FIRST_IS_R,REG,INSIDE_MATRIX_FIRST_IS_R} ;
enum TYPE {TYPE_ERROR,TYPE_REG,TYPE_MET};
int main(void)
{    
  char input[1024];
  char *string[MAX_NUM_OF_TOKENS];
  char label_name[MAX_NUM_OF_TOKENS][MAX_LABLE_NAME];
  int  type[MAX_NUM_OF_TOKENS] = {0} , first_arg[MAX_NUM_OF_TOKENS] , secound_arg[MAX_NUM_OF_TOKENS] ; /* 10=int , 1-9 registers , 11 matrix , 12 */            
  char c,delimit[]=" \t\r\n\v\f,"; 
  int i = 0, j ,state;

  if(fgets(input, sizeof input, stdin)) 
                                        
                                        
  {                                     
    string[i]=strtok(input,delimit);    
    while(string[i]!=NULL)                    
    {
      printf("string [%d]=%s",i,string[i]);
      
       for(j=0 , state=0;(c=string[i][j]) != '\0';j++)
       {
	printf("\n %c \n", string[i][j]  );
	switch(state){
	    case FIRST:
		if( isalpha(c) )
		{
		    if( c == 'r' )
		    {
		    state = FIRST_IS_R;
		    break;
		    }
		    state = ALPHA;
		    break;
		}
		else
		if( isdigit(c) )
		}
		    state = NUM;
		    break;
		}
		else 
		{
		    state = ERROR;
	        }

	    case ALPHA:
		if( isalpha(c) )
		{
		    break;
		}
		if( c == '[' )
		{	
		    type[i] = MATRIX;
		    state = FIRST_IS_BR;
		    strncat(label_name[i],string[i][j],j)
		    label_name[i][j+1] = '\0';
		    break;
		}
		break;

	    case FIRST_IS_BR:
		if( c == 'r' )
		{
		    state = INSIDE_MATRIX_FIRST_IS_R;
		    break;
		}
		if( isdigit(c) )
		{
		   first_arg[i] = atoi(c); 
		}

		
       }
	i++;
      string[i]=strtok(NULL,delimit);

    }
/*    for (j=0;j<i;j++)
    printf("%s", string[j]); */
  }

    
  return 0;
}
