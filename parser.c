#define MAX_NUM_OF_TOKENS 100
#define MAX_LABLE_NAME 100

int parser(char *input)
{ 

    enum status {FIRST,NUM,ALPHA,ERROR,FIRST_CHAR_IN_TOKEN_IS_R,REG,DIGIT_AFTER_ALPHA,INSIDE_MATRIX_FIRST_R,pound,READING_MATRIX_FIRST_ARG,INSIDE_MATRIX_FIRST_IS_NUM,STRING,STRING_END,DEF_METRIX_FIRST_ARG,DEF_METRIX_SECOUND_ARG,INSIDE_MATRIX_SECOUND_R,READING_MATRIX_SECOUND_ARG,NO_MORE_ARGS} ;
    enum TYPE {TYPE_ERROR,TYPE_REG,TYPE_NUM,TYPE_LABEL,TYPE_DIRECT,TYPE_MATRIX,TYPE_STRING,TYPE_DEF_METRIX};
    enum encoding {A,R,E};

    char *token[MAX_NUM_OF_TOKENS];
    char label_name[MAX_NUM_OF_TOKENS][MAX_LABLE_NAME];
    char temp_string[MAX_NUM_OF_TOKENS][MAX_LABLE_NAME];
    int  type[MAX_NUM_OF_TOKENS] = {0} , first_arg[MAX_NUM_OF_TOKENS] = { 0 } , secound_arg[MAX_NUM_OF_TOKENS]  = { 0 }; 
    char c = '\0',delimit[]=" \n\t\r\v\f,"; 
    int k ,i = 0, j ,state ,  found_digit_flag, found_br_flag ,sign_flag;

                                         
    {                                     
	
      token[i]=strtok(input,delimit);
    
      while(token[i]!=NULL)                    
      {
         printf("\n token [%d]=%s",i,token[i]);
         for(j=0 , state=FIRST , found_digit_flag = 0 , sign_flag = 0 ; 
         ( c=token[i][j] ) != '\0' && c != ' ' ;j++)
    
         {
          printf("\n%d %c  %d\n", j , c , state);
          switch(state){
              case FIRST:
          	if(isalpha(c))
          	{
		    if( token[i][1] == '\0' )
		    {
          	    state = ALPHA;
          	    j--;
		    }
          	    if( c == 'r' )
          	    {
          	    printf("found r %c" , c);
          	    state = FIRST_CHAR_IN_TOKEN_IS_R;
          	    break;
          	    }
          	    state = ALPHA;
          	    j--;
          	    break;
          	}
          	else
          	if( isdigit(c) )
          	{
          	    type[i] = TYPE_NUM;
          	    first_arg[i] = atoi(token[i]);
          	    state = NUM;
          	    break;
          	}
          	else
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
		    k = 0;
          	    break;
          	}
          	else
          	if( c == '"' )
          	{
          	    type[i] = TYPE_STRING;
          	    state = STRING;
          	    break;
          	}
          	else
          	if( c == '[' )
          	{
          	    type[i] = TYPE_DEF_METRIX;
          	    k = 0;
          	    state = DEF_METRIX_FIRST_ARG;
          	    break;
          	}
          	else	
          	{
          	    state = ERROR;
          	    printf("ERROR in case FIRST");
          	    break;
                  }
          	printf("first is aplpha");
    
    
              case DEF_METRIX_FIRST_ARG:
              {
          	if( c == ']' )
          	{
          	    if(!found_digit_flag)
          	    {
          		printf("ERROR in case DEF_METRIX no args in matrix");
          		return 1;
          	    }
          	    first_arg[i] = atoi(temp_string[i]); 
          	    state = DEF_METRIX_SECOUND_ARG;
          	    found_digit_flag = 0;
          	    k = 0;
          	    break;
    
          	}
          	if(isdigit(c))
          	{
    
          	   temp_string[i][k++] = c;
          	   temp_string[i][k] = '\0';
          	   found_digit_flag = 1;
          	   break;
          	}
          	printf(" \n ERROR in case DEF_METRIX first expected digit but get %c \n" , c);
          	return 1;
              }	
    
              case DEF_METRIX_SECOUND_ARG:
              {
          	if( c == '[' )
          	{
          	    if(!found_br_flag)
          	    {
          		found_br_flag = 1;
          		break;
          	    }
          	    printf(" \n ERROR in case DEF_METRIX  unexpected [ ");
          	}
    
          	if( c == ']' )
          	{
          	    if(!(found_digit_flag && found_br_flag))
          	    {
          		printf("ERROR in case DEF_METRIX no args in matrix");
          		return 1;
          	    }
          	    secound_arg[i] = atoi(temp_string[i]); 
          	    state = NO_MORE_ARGS;
          	    break;
    
          	}
          	if(isdigit(c))
          	{
          	    if(!(found_br_flag))
          	    {
          		printf("ERROR in case DEF_METRIX missing [");
          		return 1;
          	    }
    
          	   temp_string[i][k++] = c;
          	   temp_string[i][k] = '\0';
          	   found_digit_flag = 1;
          	   break;
          	}
          	printf(" \n ERROR in case DEF_METRIX expected digit but get %c \n" , c);
          	return 1;
              }
    
    
              case STRING:
          	if( c == '"' )
          	{
          	    type[i] = TYPE_STRING;
          	    state = NO_MORE_ARGS;
          	    strcpy(temp_string[i],token[i]);
          	    break;
          	}
          	break;
    
    
              case pound:
          	if( c == '-' || c == '+' )
          	{
          	    if(sign_flag)
          	    {
          		printf("ERROR in case pound expected number but get %c " , c );
          		return 1;
          	    }
          	    if(isdigit(token[i][j+1]))
          	    {
          	    	sign_flag = 1;
          		temp_string[i][k] = c;
          		temp_string[i][++k] = '\0';
          		break;
          	    }
          	    printf("ERROR in case pound expected number but get %c " , temp_string[i][j+1] );
          	    return 1;
          	}
          	if( isdigit(c) )
          	{
          	    temp_string[i][k] = c;
          	    temp_string[i][++k] = '\0';
		    printf("found direct %s " , temp_string[i] );
          	    first_arg[i] = atoi(temp_string[i]);
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
          	    first_arg[i] = atoi(token[i]);
          	    break;
          	}
          	else
          	{
          	    state = ERROR;
          	    printf("ERROR in case NUM");
          	    break;
          	}
    
              case ALPHA:
          	if( isalpha(c) || isdigit(c))
          	{
          	    printf("\n %c is alpha" , c);
          	    type[i] = TYPE_LABEL;
          	    label_name[i][j] = c;
          	    label_name[i][j+1] = '\0';
          	    break;
          	}
          	if( c == '[' )
          	{	
          	    type[i] = TYPE_MATRIX;
          	    state = READING_MATRIX_FIRST_ARG;
          	    printf("\nfound matrix %s \n j is %d \n" , label_name[i] , j );
          	    break;
          	}
          	else
          	{
          	    state = ERROR;
          	    printf("ERROR in case ALPHA");
          	    break;
          	}
          	break;
    
              case READING_MATRIX_FIRST_ARG:
          	if( c == 'r' )
          	{
          	    state = INSIDE_MATRIX_FIRST_R;
          	    break;
          	}
          	printf("Error in case READING_MATRIX_FIRST_ARG expected r but get %c " , c );
          	return 1;
    
    
      
              case INSIDE_MATRIX_FIRST_R:
          	if( ((c>='0') && (c<='7')) )
          	{
          	    first_arg[i] = c;
          	    printf("\n reg is r%c" , first_arg[i] );
          	    if(!(token[i][j + 1] == ']'))
          	    {
          		printf(" error in case INSIDE_MATRIX_FIRST_R expected ] but get %c ",  token[i][j + 1] );
          		return 1;
          	    }
          	    if(!(token[i][j + 2] == '['))
          	    {
          		printf(" error in case INSIDE_MATRIX_FIRST_R expected [ but get %c ",  token[i][j + 2] );
          		return 1;
          	    }			
          	    j = j + 2 ;
          	    state = READING_MATRIX_SECOUND_ARG ;
          	    break;
          	}
          	printf(" error in case INSIDE_MATRIX_FIRST_R expected 0 - 7 but get %c ", c );
          	return 1;
    
    
              case READING_MATRIX_SECOUND_ARG:
          	if( c == 'r' )
          	{
          	    state = INSIDE_MATRIX_SECOUND_R;
          	    break;
          	}
          	printf(" error in case READING_MATRIX_SECOUND_ARG expected r but get %c ", c );
          	return 1;
    
    
              case INSIDE_MATRIX_SECOUND_R:
          	if( ((c>='0') && (c<='7')) )
          	{
          	    secound_arg[i] = c;
          	    printf("\n reg is r%c" , secound_arg[i] );
          	    if(!(token[i][j + 1] == ']'))
          	    {
          		printf(" error in case INSIDE_MATRIX_SECOUND_R expected ] but get %c ",  token[i][j + 1] );
          		return 1;
          	    }
          	    j++;
          	    state = NO_MORE_ARGS ;
          	    break;
          	}
          	printf(" error in case INSIDE_MATRIX_SECOUND_R expected 0 - 7 but get %c ", c );
          	return 1;
    
              case FIRST_CHAR_IN_TOKEN_IS_R:
          	if( ((c>='0') && (c<='7')) )
          	{
          	    type[i] = TYPE_REG;
          	    first_arg[i] = c;
          	    printf("\n reg is r%c" , first_arg[i] );
          	    label_name[i][0] = 'r';
          	    label_name[i][j] = c;
          	    label_name[i][j+1] = '\0';
          	    state = ALPHA ;
          	    break;
    
          	}
          	state = ALPHA ;
          	j--;
          	break;
    
    
              case NO_MORE_ARGS:
          	printf("Error in case NO_MORE_ARGS expected end of string but get %c ", c );
          	return 1;
    
          }
          	
         }
          printf("\n TYPE is %d "  , type[i] );
          printf("\n first_arg %d " , first_arg[i] );
          printf("\n secound_arg %d " , secound_arg[i] );
          printf("\n temp_string %s " , temp_string[i] );
          printf("\n label_name %s " , label_name[i] );
    
          i++;
        token[i]=strtok(NULL,delimit);
    
      }
        for (j=0;j<i;j++)
      printf("%s", token[j]); 
    }
    
    return 0;
}
