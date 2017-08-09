#include <stdio.h>
#include<string.h>
#include <ctype.h>
#include <stdlib.h>
#define MAX_NUM_OF_TOKENS 100
#define MAX_LABLE_NAME 100
enum status {FIRST,NUM,ALPHA,ERROR,FIRST_IS_R,REG,DIGIT_AFTER_ALPHA,INSIDE_MATRIX_FIRST_IS_R,pound,FIRST_IS_BR,INSIDE_MATRIX_FIRST_IS_NUM} ;
enum TYPE {TYPE_ERROR,TYPE_REG,TYPE_NUM,TYPE_LABEL,TYPE_DIRECT,TYPE_MATRIX};
enum encoding {A,R,E};
enum INSTRUCTION
{
	MOV,CMP,ADD,IS_DATA,
	SUB,NOT,CLR,IS_STRING,
	LEA,INC,DEC,
	JMP,BNE,RED,
	PRN,JSR,RTS,STOP   
};

int main(void)
{    
  char DC_ARRAY[256][11] = {{0}};
  char AC_ARRAY[256][11] = {{0}};
  char input[1024];
  char *token[MAX_NUM_OF_TOKENS];
  char label_name[MAX_NUM_OF_TOKENS][MAX_LABLE_NAME];
  char temp_string[MAX_NUM_OF_TOKENS][MAX_LABLE_NAME];
  int  type[MAX_NUM_OF_TOKENS] = {0} , first_arg[MAX_NUM_OF_TOKENS] , secound_arg[MAX_NUM_OF_TOKENS] ; /* 10=int , 1-9 registers , 11 matrix , 12 */            
  char c = '\0',delimit[]=" \t\r\v\f,"; 
  int i = 0, j ,state , instract = MOV , DC = 0 , AC = 0 ;

  if(fgets(input, sizeof input, stdin)) 
                                        
                                        
  {                                     
    token[i]=strtok(input,delimit);    
    while(token[i]!=NULL)                    
    {
      printf("token [%d]=%s",i,token[i]);
       for(j=0 , state=FIRST; ( c=token[i][j] ) != '\0';j++)
       {
	printf("\n %c \n", c );
	switch(state){
	    case FIRST:
		if( isalpha(c) )
		{
		    type[i] = TYPE_LABEL ;
		    if( c == 'r' );
		    {
		    state = FIRST_IS_R;
		    break;
		    }
		    state = ALPHA;
		    break;
		}
		else
		if( isdigit(c) )
		{
		    type[i] = TYPE_NUM;
		    state = NUM;
		    break;
		}
		if( c == '-' || c == '+' )
		{
		    state = NUM;
		    break;
		}
		else 
		if( c == '#' )
		{
		    type[i] = TYPE_DIRECT;
		    state = pound;
		    break;
		}
		else
		{
		    state = ERROR;
		    printf("ERROR in case FIRST");
		    break;
	        }
	    case pound:
		if( isdigit(c) )
		{
		    first_arg[i] = atoi(token[i+1]);
		    break;
		}

		else
		{
		    state = ERROR;
		    printf("ERROR in case pound");
		    break;
		}

	    case NUM:
		if( isdigit(c) )
		{
		    type[i] = TYPE_NUM;
		    break;
		}
		else
		{
		    state = ERROR;
		    printf("ERROR in case NUM");
		    break;
		}

	    case ALPHA:
		if( isalpha(c) )
		{
		    break;
		}
		if( c == '[' )
		{	
		    type[i] = TYPE_MATRIX;
		    state = FIRST_IS_BR;
		    strncat(label_name[i],token[i],j);
		    label_name[i][j+1] = '\0';
		    break;
		}
		if( isdigit(c) )
		{
		    state = DIGIT_AFTER_ALPHA;
		    break;
		}
		else
		{
		    state = ERROR;
		    printf("ERROR in case ALPHA");
		    break;
		}
		break;


	    case DIGIT_AFTER_ALPHA:
		if( isdigit(c) )
		{
		    break;
		}
		else
		{
		    state = ERROR;
		    printf("ERROR in case DIGIT_AFTER_ALPHA");
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
		   temp_string[i][0] = c;
		   temp_string[i][1] = '\0';
		   first_arg[i] = atoi(temp_string[i]); 
		   state = INSIDE_MATRIX_FIRST_IS_NUM;
		   break;
		}
		break;

	    case INSIDE_MATRIX_FIRST_IS_NUM:
		if( c == 'r' )
		    printf("okok1");
		break;
    
	    case INSIDE_MATRIX_FIRST_IS_R:
		if( c == 'r' )
		    printf("okok2");
		break;


	}
		
       }
	printf("TYPE is %d "  , type[i] );
	i++;
      token[i]=strtok(NULL,delimit);

    }
/*    for (j=0;j<i;j++)
    printf("%s", token[j]); */
  }
	switch(instract){
	    case MOV:
		DC_ARRAY[DC][0] = '0';  	
		DC_ARRAY[DC][1] = '0';  	
		DC_ARRAY[DC][2] = '0';  	
		DC_ARRAY[DC][3] = '0';
		DC_ARRAY[DC][8] = '0';/*encoding of a command is always absulote 00*/
		DC_ARRAY[DC][9] = '0';
		DC_ARRAY[DC][10] = '\0';
		switch(type[0]){
		    case TYPE_MATRIX:
		    {
		    DC_ARRAY[DC][4] = '1';
		    DC_ARRAY[DC][5] = '0';

		    break;
		    }
		    case TYPE_REG:
		    {
		    DC_ARRAY[DC][4] = '1';
		    DC_ARRAY[DC][5] = '1';
		    break;
		    }
		    case TYPE_DIRECT:
		    {
		    DC_ARRAY[DC][4] = '0';
		    DC_ARRAY[DC][5] = '0';
		    break;
		    }
		    case TYPE_LABEL:
		    {
		    DC_ARRAY[DC][4] = '1';
		    DC_ARRAY[DC][5] = '0';
		    break;
		    }
		    case TYPE_ERROR:
		    {
		    printf("\n missing args MOV cmd ");
		    break;
		    }
		}
		switch(type[1]){
		    case TYPE_MATRIX:
		    {
		    DC_ARRAY[DC][6] = '1';
		    DC_ARRAY[DC][7] = '0';
		    break;
		    }
		    case TYPE_REG:
		    {
		    DC_ARRAY[DC][6] = '1';
		    DC_ARRAY[DC][7] = '1';
		    break;
		    }
		    case TYPE_DIRECT:
		    {
		    printf("\n ERROR secound operand can't be direct in MOV cmd ");
		    break;
		    }
		    case TYPE_LABEL:
		    {
		    DC_ARRAY[DC][6] = '1';
		    DC_ARRAY[DC][7] = '0';
		    break;
		    }
		    case TYPE_ERROR:
		    {
		    printf("\n ERROR missing args MOV cmd ");
		    break;
		    }
		}
	    case CMP:
	    case ADD:
	    case SUB:
	    case NOT:
	    case CLR:
	    case LEA:
	    case INC:
	    case DEC:
	    case JMP:
	    case BNE:
	    case RED:
	    case PRN:
	    case JSR:
	    case RTS:
	    case STOP:
	    case IS_DATA:
	    case IS_STRING:
		printf("bla");
	}

  return 0;
}
