/*MAMAN 14 Final Project*/
/*Shmuel Asulin ,ID:          */
/*Yotam Klein* , ID:066546896 */

/*Assembler*/

/*
  Parser function 
  Parse one line each time:

  1.Divide line to tokens.
  2.Parse each token and divide to arguments.
  3.Return each argument and argument type in parser_table struct.
  
  *If error function return 1 ,print error details and line number.
  else return 0.
*/

int parser(char *input ,parser_table_p parser_t_p ,int row_num)
{ 
    char *token[MAX_ROW_LEN]= {0} ;
    char case_string_cat[MAX_ROW_LEN] = {'\0'} ;
    char label_name[MAX_ROW_LEN][MAX_LABEL_SIZE] = {{0}} ;
    char temp_string[MAX_ROW_LEN][MAX_LABEL_SIZE] = {{0}} ;
    int  type[MAX_ROW_LEN] = {0} , 
         first_arg[MAX_ROW_LEN] = {0} , 
         second_arg[MAX_ROW_LEN]  = {0};
 
    char c = '\0',delimit[]=" \n\t\r\v\f,",bad_token[] =",," ; 
    int k ,i = 0, j ,state=FIRST ,  found_digit_flag, found_br_flag ,sign_flag;

    
    strcpy(parser_t_p->label_name[0],label_name[0]);
    strcpy(parser_t_p->temp_string[0],temp_string[0]);
    strcpy(parser_t_p->label_name[0],label_name[0]);
    parser_t_p->type[0] = type[0]; 
    parser_t_p->type[1] = type[1]; 
    parser_t_p->first_arg[0] = first_arg[0]; 
    parser_t_p->second_arg[0] = second_arg[0]; 

    /*looks for ,, in the input string */
    if(strstr( bad_token , input ))
    {
    	printf("ERROR ,in row#%d recieve bad_token %s \n ",row_num, bad_token);
    	return 1;
    }
    /*extract first token*/
      token[i]=strtok(input,delimit);
    
      while(token[i]!=NULL)                    
      {
	 /*string could have been splited to 2(or more) tokens*/
	 if( state == IN_STRING)
	 {
	    strcat(case_string_cat,token[i-1]);
	    strcat(case_string_cat,token[i]);
	    token[--i] = case_string_cat;
	 }
	 else
	 {
	 state=FIRST;
	 j = 0;
	 }
         /*printf("\n token [%d]=%s",i,token[i]);*/

         /*parse token i starts*/
         for( found_digit_flag = 0 , sign_flag = 0 , found_br_flag = 0; 
         ( c=token[i][j] ) != '\0' && c != ' ' ;j++)
    
         {
         /* printf("\n%d %c  %d\n", j , c , state); */
          switch(state){
              case FIRST:
	      /*first char of token*/
          	if(isalpha(c))
          	{
		    /*One alpha char will always be label*/
		    if( token[i][1] == '\0' )
		    {
          	    state = ALPHA;
          	    j--;
          	    break;
		    }
		    /*If first char is r check if reg or label*/
          	    if( c == 'r' )
          	    {
          	    state = FIRST_CHAR_IN_TOKEN_IS_R;
          	    break;
          	    }
		    /*In all other cases have to be label*/
          	    state = ALPHA;
          	    j--;
          	    break;
          	}
          	else
		/*If first is digit it have to be number*/
          	if( isdigit(c) )
          	{
          	    type[i] = TYPE_NUM;
          	    first_arg[i] = atoi(token[i]);
          	    state = NUM;
          	    break;
          	}
          	else
		/*If first is sign it have to be number*/
          	if( c == '-' || c == '+' )
          	{
          	    state = NUM;
          	    break;
          	}
          	else
		/*If first is pound it have to be immediate*/
          	if( c == '#' )
          	{
          	    type[i] = TYPE_DIRECT;
          	    state = POUND;
		    k = 0;
          	    break;
          	}
          	else
		/*If first is " it have to be string*/
          	if( c == '"' )
          	{
          	    type[i] = TYPE_STRING;
          	    state = IN_STRING;
          	    break;
          	}
          	else
		/*If first is [ it have to be mat definition */
          	if( c == '[' )
          	{
          	    type[i] = TYPE_DEF_METRIX;
          	    k = 0;
          	    state = DEF_METRIX_FIRST_ARG;
          	    break;
          	}
		/*All other cases are not legal*/
          	else	
          	{
          	    state = ERROR;
		    printf("ERROR ,in row#%d recieved bad char %c in the begining of token %s \n ",row_num,c,token[i] );
		    return 1;
                }
    
    
	    /*Second stage of sorting*/


              case DEF_METRIX_FIRST_ARG:
              {
          	if( c == ']' )
          	{
          	    if(!found_digit_flag)
          	    {
			printf("ERROR ,in row#%d missing args in matrix token %s \n ",row_num,token[i] );
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
          	printf("ERROR ,in row# %d , token %s  ,while defined metrix  expected digit but get %c \n",row_num,token[i] , c);
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
          	    printf("ERROR ,in row# %d . while defined metrix unexpected [  \n",row_num);
		    return 1;
          	}
    
          	if( c == ']' )
          	{
          	    if(!(found_digit_flag && found_br_flag))
          	    {
			printf("ERROR ,in row#%d missing args in matrix token %s \n ",row_num,token[i] );
          		return 1;
          	    }
          	    second_arg[i] = atoi(temp_string[i]); 
          	    state = NO_MORE_ARGS;
          	    break;
    
          	}
          	if(isdigit(c))
          	{
          	    if(!(found_br_flag))
          	    {
          		printf("ERROR ,in row# %d . while defined metrix , missing [ \n",row_num);
          		return 1;
          	    }
    
          	   temp_string[i][k++] = c;
          	   temp_string[i][k] = '\0';
          	   found_digit_flag = 1;
          	   break;
          	}
		/*All other cases are not legal*/
          	printf("ERROR ,in row# %d , token %s  ,while defined metrix  expected digit but get %c \n",row_num,token[i] , c);
          	return 1;
              }
    
 

              case IN_STRING:
          	if( c == '"' )
          	{
          	    type[i] = TYPE_STRING;
          	    state = NO_MORE_ARGS;
          	    strcpy(temp_string[i],token[i]);
          	    break;
          	}
          	break;
    

    
              case POUND:
          	if( c == '-' || c == '+' )
          	{
          	    if(sign_flag)
          	    {
          		printf("ERROR ,in row%d , After POUND unexpected %c in the middle of argument \n",row_num , c );
          		return 1;
          	    }
          	    if(isdigit(token[i][j+1]))
          	    {
          	    	sign_flag = 1;
          		temp_string[i][k] = c;
          		temp_string[i][++k] = '\0';
          		break;
          	    }
          	printf("ERROR ,in row# %d , token %s  , expected digit but get %c \n",row_num,token[i] , temp_string[i][j+1] );
          	    return 1;
          	}
          	if( isdigit(c) )
          	{
          	    sign_flag = 1;
          	    temp_string[i][k] = c;
          	    temp_string[i][++k] = '\0';
          	    first_arg[i] = atoi(temp_string[i]);
          	    break;
          	}
          	else
          	{
		/*All other cases are not legal*/
          	printf("ERROR ,in row# %d , token %s  expected digit but get %c \n",row_num,token[i] , c);
          	return 1;
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
		/*All other cases are not legal*/
          	printf("ERROR ,in row# %d , token %s  expected digit but get %c \n",row_num,token[i] , c);
          	return 1;
          	}
    


              case ALPHA:
          	if( isalpha(c) || isdigit(c))
          	{
          	    type[i] = TYPE_LABEL;
          	    label_name[i][j] = c;
          	    label_name[i][j+1] = '\0';
          	    break;
          	}
		/*Check if token is a matrix label*/
          	if( c == '[' )
          	{	
          	    type[i] = TYPE_MATRIX;
          	    state = READING_MATRIX_FIRST_ARG;
          	    break;
          	}
          	else
          	{		
		/*All other cases are not legal*/
          	printf("ERROR ,in row# %d , token %s  unexpected value %c \n",row_num,token[i] , c);
          	return 1;
          	}
          	break;
    

              case READING_MATRIX_FIRST_ARG:
          	if( c == 'r' )
          	{
          	    state = INSIDE_MATRIX_FIRST_R;
          	    break;
          	}
          	printf("ERROR , in row# %d ,  token %s  READING MATRIX  ARGUMENT expected r but get %c \n",row_num,token[i] , c );
          	return 1;
    
    
      
              case INSIDE_MATRIX_FIRST_R:
          	if( ((c>='0') && (c<='7')) )
          	{
          	    first_arg[i] = c - '0' ;
          	    if(!(token[i][j + 1] == ']'))
          	    {
          		printf("ERROR in row# %d , token %s , expected ] but get %c \n" , row_num ,token[i] ,  token[i][j + 1] );
          		return 1;
          	    }
          	    if(!(token[i][j + 2] == '['))
          	    {
          		printf("ERROR in row# %d , token %s , expected [ but get %c \n" , row_num ,token[i] ,  token[i][j + 2] );
          		return 1;
          	    }			
          	    j = j + 2 ;
          	    state = READING_MATRIX_SECOUND_ARG ;
          	    break;
          	}
          	printf("ERROR in row# %d , token %s , expected 0 - 7 but get %c \n" , row_num ,token[i] ,  c );
          	return 1;
    
    
              case READING_MATRIX_SECOUND_ARG:
          	if( c == 'r' )
          	{
          	    state = INSIDE_MATRIX_SECOUND_R;
          	    break;
          	}
          	printf("ERROR , in row# %d ,  token %s  READING MATRIX  ARGUMENT expected r but get %c \n",row_num,token[i] , c );
          	return 1;
    
    
              case INSIDE_MATRIX_SECOUND_R:
          	if( ((c>='0') && (c<='7')) )
          	{
          	    second_arg[i] = c - '0';
          	    if(!(token[i][j + 1] == ']'))
          	    {
          		printf("ERROR in row# %d , token %s , expected ] but get %c \n" , row_num ,token[i] ,  token[i][j + 1] );
          		return 1;
          	    }
          	    j++;
          	    state = NO_MORE_ARGS ;
          	    break;
          	}
          	printf("ERROR in row# %d , token %s , expected 0 - 7 but get %c \n" , row_num ,token[i] ,  c );
          	return 1;
    
              case FIRST_CHAR_IN_TOKEN_IS_R:
          	if( ((c>='0') && (c<='7')) )
          	{
          	    type[i] = TYPE_REG;
          	    first_arg[i]  = c - '0';
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
          	printf("ERROR in row# %d , No more chars was expected , but get %c ", row_num, c );
          	return 1;
    
          }
          	
         }



	    /*copy values to stract */
	    strcpy(parser_t_p->label_name[i],label_name[i]);
	    strcpy(parser_t_p->temp_string[i],temp_string[i]);
	    strcpy(parser_t_p->label_name[i],label_name[i]);
	    parser_t_p->type[i] = type[i]; 
	    parser_t_p->type[i + 1] = type[i + 1]; 
	    parser_t_p->first_arg[i] = first_arg[i]; 
	    parser_t_p->second_arg[i] = second_arg[i]; 

    
          i++;
	  /*extracting second token*/
        token[i]=strtok(NULL,delimit);
    
      }
    
    return 0;
}
