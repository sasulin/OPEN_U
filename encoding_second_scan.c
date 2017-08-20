/*MAMAN 14 Final Project*/
/*Shmuel Asulin ,ID:          */
/*Yotam Klein* , ID:066546896 */

/*Assembler*/

int encoding_second_scan(operation_list *command , D_row_p DC_table , I_row_p IC_table , sym_row_p sym_head , parser_table_p parser_t_p , int *DC , int *IC  )
{

    int k , label_address;
    bool error_flag = 0 ;
/*case need DC encoding*/
	if (command->EnuM==STRING || 
		command->EnuM ==DATA  || 
		command->EnuM==MAT)	    
	return 0;
	
/**IC encoding starts*/
	strcpy(IC_table[*IC].binary_op,command->op_code_bin);/*Command op number*/
	IC_table[*IC].binary_op[8] = '0';/*encoding of a command is always absulote 00*/
	IC_table[*IC].binary_op[9] = '0';
/*addressing modes*/

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
/*Encrypting encoding number*/
		    case TYPE_DIRECT:
			if(!command->src_immediate)
			{
			    printf("ERROR can't get type immediate");
			    error_flag = 1 ;
			}
			IC_table[*IC].binary_op[4] = '0';
			IC_table[*IC].binary_op[5] = '0';
			break;
		    
		    case TYPE_MATRIX:
			if(!command->src_matrix)
			{
			    printf("ERROR can't get type matrix");
			    error_flag = 1 ;
			}
			IC_table[*IC].binary_op[4] = '1';
			IC_table[*IC].binary_op[5] = '0';
			break;

		    case TYPE_LABEL:
			if(!command->src_direct)
			{
			    printf("ERROR can't get type direct");
			    error_flag = 1 ;
			}
			IC_table[*IC].binary_op[4] = '0';
			IC_table[*IC].binary_op[5] = '1';
			break;

		    case TYPE_REG:
			if(!command->src_register)
			{
			    printf("ERROR can't get type reg");
			    error_flag = 1 ;
			}
			IC_table[*IC].binary_op[4] = '1';
			IC_table[*IC].binary_op[5] = '1';
			break;

		    case TYPE_ERROR:
			printf("\n ERROR missing args to cmd ");
			error_flag = 1 ;
			IC_table[*IC].binary_op[4] = '0';
			IC_table[*IC].binary_op[5] = '0';
			break;
		}
    }

    else
    {
	k = 0;
	IC_table[*IC].binary_op[4] = '0';
	IC_table[*IC].binary_op[5] = '0';
    }
		
	switch(parser_t_p->type[k])
		{

		    case TYPE_DIRECT:
			if(!command->dest_immediate)
			{
			    error_flag = 1 ;
			    printf("ERROR can't get type immediate");
			}
			IC_table[*IC].binary_op[6] = '0';
			IC_table[*IC].binary_op[7] = '0';
			break;

		    case TYPE_MATRIX:
			if(!command->dest_matrix)
			{
			    error_flag = 1 ;
			    printf("ERROR can't get type matrix");
			}
			IC_table[*IC].binary_op[6] = '1';
			IC_table[*IC].binary_op[7] = '0';
			break;


		    case TYPE_LABEL:
			if(!command->dest_direct)
			{
			    error_flag = 1 ;
			    printf("ERROR can't get type direct");
			}
			IC_table[*IC].binary_op[6] = '0';
			IC_table[*IC].binary_op[7] = '1';
			break;

		    case TYPE_REG:
			if(!command->dest_register)
			{
			    error_flag = 1 ;
			    printf("ERROR can't get type reg");
			}
			IC_table[*IC].binary_op[6] = '1';
			IC_table[*IC].binary_op[7] = '1';
			break;

		    case TYPE_ERROR:
			if(command->dest_direct || 
			    command->dest_immediate || 
			    command->dest_register || 
			    command->dest_matrix)
			{    
			    error_flag = 1 ;
			    printf("\n missing args to cmd ");
			}
			IC_table[*IC].binary_op[6] = '0';
			IC_table[*IC].binary_op[7] = '0';

			break;
		}

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
			/*check that label exiset */
			if(!check_label( parser_t_p->label_name[0] , sym_head , &error_flag , YES))
			{
			return 1;
			}
				
			/*encode mat label */
			label_address = return_label_address(parser_t_p->label_name[0] , sym_head);
			if(!label_address)
				strcpy(IC_table[*IC].label,parser_t_p->label_name[0]);

			dec_to_bin(label_address,IC_table[*IC].binary_op,COMMAND_LEN);
			IC_table[*IC].binary_op[8] = '1';
			IC_table[*IC].binary_op[9] = '0';
			/* if extern */	
			if(!label_address)
			{
			IC_table[*IC].binary_op[8] = '0';
			IC_table[*IC].binary_op[9] = '1';
			}
			(*IC)++;
			/* encode matrix */
			dec_to_bin((parser_t_p->first_arg[0]),IC_table[*IC].binary_op,REG_LEN);
			dec_to_bin((parser_t_p->second_arg[0]),IC_table[*IC].binary_op + 4,REG_LEN);
			IC_table[*IC].binary_op[8] = '0';
			IC_table[*IC].binary_op[9] = '0';
			break;
 
		    case TYPE_REG:
		    dec_to_bin((parser_t_p->first_arg[0]),IC_table[*IC].binary_op,REG_LEN);
			IC_table[*IC].binary_op[8] = '0';
			IC_table[*IC].binary_op[9] = '0';
			if(parser_t_p->type[1] == TYPE_REG) /* need to check that is working */
			{
			    dec_to_bin((parser_t_p->first_arg[1]),IC_table[*IC].binary_op + 4,REG_LEN);
			    (*IC)++;
			    return 0;
			}  
			break;


		    case TYPE_DIRECT:
			dec_to_bin((parser_t_p->first_arg[0]),IC_table[*IC].binary_op,COMMAND_LEN);
			IC_table[*IC].binary_op[8] = '0';
			IC_table[*IC].binary_op[9] = '0';
			break;
		    
		    case TYPE_LABEL:
			/*check label */
			if(!check_label( parser_t_p->label_name[0] , sym_head ,&error_flag , YES))
			{
			return 1;
			}
			/* find label */
			label_address = return_label_address(parser_t_p->label_name[0] , sym_head);
			if(!label_address)
				strcpy(IC_table[*IC].label,parser_t_p->label_name[0]);
			dec_to_bin(label_address,IC_table[*IC].binary_op,COMMAND_LEN);
			IC_table[*IC].binary_op[8] = '1';
			IC_table[*IC].binary_op[9] = '0';
			
			/* if  extern */	
			if(!label_address)
			{
			IC_table[*IC].binary_op[8] = '0';
			IC_table[*IC].binary_op[9] = '1';
			}
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
		    	/*check that label exist */
			if(!check_label( parser_t_p->label_name[k] , sym_head , &error_flag , YES))
			{
			return 1;
			}
			/*encode mat label */
			label_address = return_label_address(parser_t_p->label_name[k] , sym_head);
			if(!label_address)
				strcpy(IC_table[*IC].label,parser_t_p->label_name[k]);
			dec_to_bin(label_address,IC_table[*IC].binary_op,COMMAND_LEN);
			IC_table[*IC].binary_op[8] = '1';
			IC_table[*IC].binary_op[9] = '0';
			/* if  extern */	

	

			if(!label_address)
			{
			
			
			IC_table[*IC].binary_op[8] = '0';
			IC_table[*IC].binary_op[9] = '1';
			}
			(*IC)++;
			/* encode matrix */
			dec_to_bin((parser_t_p->first_arg[0]),IC_table[*IC].binary_op,REG_LEN);
			dec_to_bin((parser_t_p->second_arg[0]),IC_table[*IC].binary_op + 4,REG_LEN);
			IC_table[*IC].binary_op[8] = '0';
			IC_table[*IC].binary_op[9] = '0';
			break;

		    
		    case TYPE_REG:
		    dec_to_bin((parser_t_p->first_arg[k]),IC_table[*IC].binary_op + 4,REG_LEN);
			IC_table[*IC].binary_op[8] = '0';
			IC_table[*IC].binary_op[9] = '0';
			break;


		    case TYPE_DIRECT:
			dec_to_bin((parser_t_p->first_arg[k]),IC_table[*IC].binary_op,COMMAND_LEN);
			IC_table[*IC].binary_op[8] = '0';
			IC_table[*IC].binary_op[9] = '0';
			break;
		    
		    case TYPE_LABEL:
			/*check label */
			if(!check_label( parser_t_p->label_name[k] , sym_head ,&error_flag , YES))
			{
			return 1;
			}
		    	/* find label address*/
			label_address = return_label_address(parser_t_p->label_name[k] , sym_head);
			if(!label_address)
				strcpy(IC_table[*IC].label,parser_t_p->label_name[k]);

			dec_to_bin(label_address,IC_table[*IC].binary_op,COMMAND_LEN);
			IC_table[*IC].binary_op[8] = '1';
			IC_table[*IC].binary_op[9] = '0';
			/* if not extern */	
			if(!label_address)
			{
			IC_table[*IC].binary_op[8] = '0';
			IC_table[*IC].binary_op[9] = '1';
			}
			break;

		}
	    (*IC)++;

    	
return error_flag;
}

