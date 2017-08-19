int encoding_first_scan(operation_list *command , D_row_p DC_table , I_row_p IC_table , sym_row_p sym_head , parser_table_p parser_t_p , int *DC , int *IC )

{

    int i = 0 ,k , error_flag = 0;
    switch(command->EnuM)
    {
/*case need DC encoding*/

	case STRING:
	    if(parser_t_p->type[0] != TYPE_STRING )
	    {
		
		printf("\nERROR , Wrong argument to .string command %d != %d \n" , parser_t_p->type[0] ,TYPE_STRING);
		break;
	    } 
	    for(i=1;parser_t_p->temp_string[0][i] != '"';i++)
		{
		dec_to_bin(parser_t_p->temp_string[0][i],DC_table[*DC].binary_op,WORD_LEN);
		(*DC)++;
		printf("DC value incrised by 1 %d" , *DC);		
		}

	    /*adding zero to the string end*/
	    dec_to_bin(0,DC_table[*DC].binary_op,WORD_LEN);
	    (*DC)++;
	    /*Checking that there is no more ags after string*/
	    if(parser_t_p->type[1] != 0)
	    {
		printf("ERROR , Too many arguments .string command %d ", parser_t_p->type[1]);
		error_flag = 1 ;
	    }
	    return error_flag;

	case DATA:
	    for(i=0;parser_t_p->type[i];i++)
	    {
		/* check that all types are numbers */
		if(parser_t_p->type[i] != TYPE_NUM )
		{
		printf("\nERROR , Wrong argument to .data command %d != %d \n" , parser_t_p->type[i] ,TYPE_NUM);
		error_flag = 1 ;
		break;
		}
		/* check that all |numbers| < MAX_NUM*/
		if(!((parser_t_p->first_arg[i] < MAX_NUM)  &&  (parser_t_p->first_arg[i] > (-MAX_NUM)))  )
		{
		printf("\nERROR , Number is Out Of Range  |%d| > MAX_NUM \n" , parser_t_p->first_arg[i] );
		error_flag = 1 ;
		break;
		}

		
		dec_to_bin(parser_t_p->first_arg[i],DC_table[*DC].binary_op,WORD_LEN);
		(*DC)++;
		printf("DC value incrised by 1 %d" , *DC);
	    }
	    return error_flag;

	case MAT:
	    for(i=1;parser_t_p->type[i];i++)
	    {
		/* need to add check that all types are numbers */
		if(parser_t_p->type[i] != TYPE_NUM )
		{
		printf("\nERROR , Wrong argument to .mat command %d != %d \n" , parser_t_p->type[i] ,TYPE_NUM);
		error_flag = 1 ;
		break;
		}
		/* check that all |numbers| < MAX_NUM*/
		if(!((parser_t_p->first_arg[i] < MAX_NUM)  &&  (parser_t_p->first_arg[i] > (-MAX_NUM)))  )
		{
		printf("\nERROR , Number is Out Of Range  |%d| > MAX_NUM \n" , parser_t_p->first_arg[i] );
		error_flag = 1 ;
		break;
		}
		
		dec_to_bin(parser_t_p->first_arg[i],DC_table[*DC].binary_op,WORD_LEN);
		(*DC)++;
	    }
	    /* padding data tabel with zeros */

	    for(--i;(parser_t_p->first_arg[0])*(parser_t_p->second_arg[0]) > i;i++)
	    {
		dec_to_bin(0,DC_table[*DC].binary_op,WORD_LEN);
		(*DC)++;
		printf("DC value incrised by 1 %d" , *DC);

	    }
	    return error_flag;
	}


	
/**IC count starts*/

/* save word for operation */
(*IC)++; 
/* if source operand not legal skip */
    if(	command->src_direct || 
	command->src_immediate || 
	command->src_register || 
	command->src_matrix
    )
    {
	k = 1;    


	    switch(parser_t_p->type[0])
		{		    
		    case TYPE_MATRIX:
			/* save word for mat label */
			(*IC)++;
			/* encode matrix */
			break;

		    
		    case TYPE_REG:
			if(parser_t_p->type[1] == TYPE_REG) /* need to check that is working */
			{
			    (*IC)++;
			    return 0;
			}  
			break;


		    case TYPE_DIRECT:
			break;
		    
		    case TYPE_LABEL:
			break;

		    
		}

	    (*IC)++;


    }

    else
    {
	k = 0;
    }




	    switch(parser_t_p->type[k])
		{		    
		    case TYPE_MATRIX:
		    	/* save word for mat label */
			(*IC)++;
			/* encode matrix */
			break;

		    
		    case TYPE_REG:
			break;


		    case TYPE_DIRECT:
			break;
		    
		    case TYPE_LABEL:
			break;
		}
	    (*IC)++;

    	
return error_flag;
}

