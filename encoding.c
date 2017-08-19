

int encoding(operation_list *command , D_row_p DC_table , I_row_p IC_table , sym_row_p sym_head , parser_table_p parser_t_p , int *DC , int *IC)
{

/*    strcpy(label_name[i],parser_t_p->label_name[i]);
    strcpy(temp_string[i],parser_t_p->temp_string[i]);
    strcpy(label_name[i],parser_t_p->label_name[i]);
    type[i] = parser_t_p->type[i]; 
    first_arg[i] = parser_t_p->first_arg[i]; 
    second_arg[i] = parser_t_p->second_arg[i]; */

    int i = 0 ;
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
		bin_to_weird(DC_table[*DC].binary_op,DC_table[*DC].weird_four_op);

		(*DC)++;
		printf("DC value incrised by 1 %d" , *DC);
		}
	    if(parser_t_p->type[1] != 0)
		printf("ERROR , Too many arguments .string command %d ", parser_t_p->type[1]);
	    return 0;

	case DATA:
	    for(i=0;parser_t_p->type[i];i++)
		/* need to add check that all types are numbers */
		{
		dec_to_bin(parser_t_p->first_arg[i],DC_table[*DC].binary_op,WORD_LEN);
		bin_to_weird(DC_table[*DC].binary_op,DC_table[*DC].weird_four_op);
		(*DC)++;
		printf("DC value incrised by 1 %d" , *DC);
		}
	    return 0;

	case MAT:
	    for(i=1;parser_t_p->type[i];i++)
		/* need to add check that all types are numbers */
		{
		dec_to_bin(parser_t_p->first_arg[i],DC_table[*DC].binary_op,WORD_LEN);
		bin_to_weird(DC_table[*DC].binary_op,DC_table[*DC].weird_four_op);
		(*DC)++;
		printf("DC value incrised by 1 %d" , *DC);
		}
	    return 0;
	}


	
/**IC encoding starts*/
	strcpy(IC_table[*IC].binary_op,command->op_code_bin);/*Command op number*/
	IC_table[*IC].binary_op[8] = '0';/*encoding of a command is always absulote 00*/
	IC_table[*IC].binary_op[9] = '0';
/*addressing modes*/
	switch(parser_t_p->type[0])
		{		    
/*Encrypting encoding number*/
		    case TYPE_DIRECT:
			if(!command->src_immediate)
			    printf("ERROR can't get type immediate");
				IC_table[*IC].binary_op[4] = '0';
				IC_table[*IC].binary_op[5] = '0';
			break;
		    
		    case TYPE_LABEL:
			if(!command->src_direct)
			    printf("ERROR can't get type direct");
			IC_table[*IC].binary_op[4] = '1';
			IC_table[*IC].binary_op[5] = '0';
			break;

		    case TYPE_MATRIX:
			if(!command->src_matrix)
				printf("ERROR can't get type matrix");
				IC_table[*IC].binary_op[4] = '0';
				IC_table[*IC].binary_op[5] = '1';
			break;

		    case TYPE_REG:
			if(!command->src_register)
			    printf("ERROR can't get type reg");
				IC_table[*IC].binary_op[4] = '1';
				IC_table[*IC].binary_op[5] = '1';
			break;

		    case TYPE_ERROR:
			if(	command->dest_direct || 
				command->dest_immediate || 
				command->dest_register || 
				command->dest_matrix
											)

			printf("\n missing args to cmd ");
			break;
		}

		
	switch(parser_t_p->type[1])
		{		    
		    case TYPE_MATRIX:
			if(!command->dest_matrix)
			    printf("ERROR can't get type matrix");
			IC_table[*IC].binary_op[6] = '1';
			IC_table[*IC].binary_op[7] = '0';
			break;

		    
		    case TYPE_REG:
			if(!command->dest_register)
			    printf("ERROR can't get type reg");
			IC_table[*IC].binary_op[6] = '1';
			IC_table[*IC].binary_op[7] = '1';
			break;


		    case TYPE_DIRECT:
			if(!command->dest_immediate)
			    printf("ERROR can't get type immediate");
			IC_table[*IC].binary_op[6] = '0';
			IC_table[*IC].binary_op[7] = '0';
			break;
		    
		    case TYPE_LABEL:
			if(!command->dest_direct)
			    printf("ERROR can't get type direct");
			IC_table[*IC].binary_op[6] = '1';
			IC_table[*IC].binary_op[7] = '0';
			break;

		    case TYPE_ERROR:
			if(command->dest_direct || command->dest_immediate || command->dest_register || command->dest_matrix)
			printf("\n missing args to cmd ");
			break;
		}

	    (*IC)++;

	    
	    switch(parser_t_p->type[0])
		{		    
		    case TYPE_MATRIX:
			dec_to_bin((parser_t_p->first_arg[0]),IC_table[*IC].binary_op,REG_LEN);
			bin_to_weird(IC_table[*IC].binary_op,IC_table[*IC].weird_four_op);
			dec_to_bin((parser_t_p->second_arg[0]),IC_table[*IC].binary_op + 4,REG_LEN);
			bin_to_weird(IC_table[*IC].binary_op,IC_table[*IC].weird_four_op);
			IC_table[*IC].binary_op[8] = '0';
			IC_table[*IC].binary_op[9] = '0';
			break;

		    
		    case TYPE_REG:
		    dec_to_bin((parser_t_p->first_arg[0]),IC_table[*IC].binary_op,REG_LEN);
			bin_to_weird(IC_table[*IC].binary_op,IC_table[*IC].weird_four_op);
			IC_table[*IC].binary_op[8] = '0';
			IC_table[*IC].binary_op[9] = '0';
			if(parser_t_p->type[1] == TYPE_REG) /* need to check that is working */
			{
			    dec_to_bin((parser_t_p->first_arg[1]),IC_table[*IC].binary_op + 4,REG_LEN);
				bin_to_weird(IC_table[*IC].binary_op,IC_table[*IC].weird_four_op);
			    (*IC)++;
			    return 0;
			}  
			break;


		    case TYPE_DIRECT:
			dec_to_bin((parser_t_p->first_arg[0]),IC_table[*IC].binary_op,COMMAND_LEN);
			bin_to_weird(IC_table[*IC].binary_op,IC_table[*IC].weird_four_op);
			IC_table[*IC].binary_op[8] = '0';
			IC_table[*IC].binary_op[9] = '0';
			break;
		    
		    case TYPE_LABEL:
			/*need to find label */	
			IC_table[*IC].binary_op[8] = '0';
			IC_table[*IC].binary_op[9] = '1';
			break;

		    case TYPE_ERROR:
			if(command->dest_direct || command->dest_immediate || command->dest_register || command->dest_matrix)
			printf("\n missing args to cmd ");
			break;
		}
	    (*IC)++;


	    switch(parser_t_p->type[1])
		{		    
		    case TYPE_MATRIX:
			dec_to_bin((parser_t_p->first_arg[1]),IC_table[*IC].binary_op,REG_LEN);
			bin_to_weird(IC_table[*IC].binary_op,IC_table[*IC].weird_four_op);
			dec_to_bin((parser_t_p->second_arg[1]),IC_table[*IC].binary_op + 4,REG_LEN);
			bin_to_weird(IC_table[*IC].binary_op,IC_table[*IC].weird_four_op);
			IC_table[*IC].binary_op[8] = '0';
			IC_table[*IC].binary_op[9] = '0';
			break;

		    
		    case TYPE_REG:
		    dec_to_bin((parser_t_p->first_arg[1]),IC_table[*IC].binary_op + 4,REG_LEN);
			bin_to_weird(IC_table[*IC].binary_op,IC_table[*IC].weird_four_op);
			IC_table[*IC].binary_op[8] = '0';
			IC_table[*IC].binary_op[9] = '0';
			break;


		    case TYPE_DIRECT:
			dec_to_bin((parser_t_p->first_arg[0]),IC_table[*IC].binary_op,COMMAND_LEN);
			bin_to_weird(IC_table[*IC].binary_op,IC_table[*IC].weird_four_op);
			IC_table[*IC].binary_op[8] = '0';
			IC_table[*IC].binary_op[9] = '0';
			break;
		    
		    case TYPE_LABEL:
			/*need to find label */	
			IC_table[*IC].binary_op[8] = '0';
			IC_table[*IC].binary_op[9] = '1';
			break;

		    case TYPE_ERROR:
			if(command->dest_direct || command->dest_immediate || command->dest_register || command->dest_matrix)
			printf("\n missing args to cmd ");
			break;
		}
	    (*IC)++;

    	
return 0;
}

