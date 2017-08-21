/*MAMAN 14 Final Project*/
/*Shmuel Asulin ,ID:036760676  */
/*Yotam Klein* , ID:066546896 */

/*Assembler*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aux_func.h"
#include "scan.h"


int encoding_second_scan(operation_list *command , D_row_p DC_table , I_row_p IC_table , sym_row_p sym_head , parser_table_p parser_t_p , int *DC , int *IC ,int row_num)
{

    int k , label_address;
    bool error_flag = 0 ;
/*DC encoding was already done in the first scan*/
    if (command->EnuM == STRING || 
	command->EnuM == DATA  || 
	command->EnuM == MAT)	    
    return 0;
	

/*Error if more than 2 tokens for a command*/
    if(parser_t_p->type[2])
    {
    printf("ERROR! in row#%d: Command %s have too many (tokens > 2 ) \n",row_num,command->name);
    return 1;
    }



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
			    printf("ERROR! in row#%d: Command %s can't get immediate addressing \n",row_num,command->name);
			    return 1 ;
			}
			IC_table[*IC].binary_op[4] = '0';
			IC_table[*IC].binary_op[5] = '0';
			break;
		    
		    case TYPE_MATRIX:
			if(!command->src_matrix)
			{
			    printf("ERROR! in row#%d: Command %s can't get  matrix addressing\n",row_num,command->name);
			    return 1 ;
			}
			IC_table[*IC].binary_op[4] = '1';
			IC_table[*IC].binary_op[5] = '0';
			break;

		    case TYPE_LABEL:
			if(!command->src_direct)
			{
			    printf("ERROR! in row#%d: Command %s can't get direct  addressing\n",row_num,command->name);
			    return 1 ;
			}
			IC_table[*IC].binary_op[4] = '0';
			IC_table[*IC].binary_op[5] = '1';
			break;

		    case TYPE_REG:
			if(!command->src_register)
			{
			    printf("ERROR! in row#%d: Command %s can't get reg  addressing\n",row_num,command->name);
			    return 1 ;
			}
			IC_table[*IC].binary_op[4] = '1';
			IC_table[*IC].binary_op[5] = '1';
			break;

		    case TYPE_ERROR:
			printf("ERROR! in row#%d: missing args to Command %s\n",row_num,command->name);
			return 1 ;
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
			    printf("ERROR! in row#%d: Command %s can't get immediate  addressing\n",row_num,command->name);
			    return 1;
			}
			IC_table[*IC].binary_op[6] = '0';
			IC_table[*IC].binary_op[7] = '0';
			break;

		    case TYPE_MATRIX:
			if(!command->dest_matrix)
			{
			    printf("ERROR! in row#%d: Command %s can't get matrix addressing\n",row_num,command->name);
			    return 1;
			}
			IC_table[*IC].binary_op[6] = '1';
			IC_table[*IC].binary_op[7] = '0';
			break;


		    case TYPE_LABEL:
			if(!command->dest_direct)
			{
			    printf("ERROR! in row#%d: Command %s can't get direct  addressing\n",row_num,command->name);
			    return 1;
			}
			IC_table[*IC].binary_op[6] = '0';
			IC_table[*IC].binary_op[7] = '1';
			break;

		    case TYPE_REG:
			if(!command->dest_register)
			{
			    printf("ERROR! in row#%d: Command %s can't get reg  addressing\n",row_num,command->name);
			    return 1;
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
			printf("ERROR! in row#%d: missing args to Command %s\n",row_num,command->name);
			return 1 ;
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
			if(!check_label( parser_t_p->label_name[0] , sym_head , &error_flag , YES,row_num))
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
			if(check_in_limit(parser_t_p->first_arg[0],row_num)) return 1;
			dec_to_bin((parser_t_p->first_arg[0]),IC_table[*IC].binary_op,REG_LEN);
			if(check_in_limit(parser_t_p->second_arg[0],row_num)) return 1;
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
			if(check_in_limit(parser_t_p->first_arg[0],row_num)) return 1;
			dec_to_bin((parser_t_p->first_arg[0]),IC_table[*IC].binary_op,COMMAND_LEN);
			IC_table[*IC].binary_op[8] = '0';
			IC_table[*IC].binary_op[9] = '0';
			break;
		    
		    case TYPE_LABEL:
			/*check label */
			if(!check_label( parser_t_p->label_name[0] , sym_head ,&error_flag , YES,row_num))
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
			if(!check_label( parser_t_p->label_name[k] , sym_head , &error_flag , YES,row_num))
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
			if(check_in_limit(parser_t_p->first_arg[k],row_num)) return 1;
			dec_to_bin((parser_t_p->first_arg[k]),IC_table[*IC].binary_op,COMMAND_LEN);
			IC_table[*IC].binary_op[8] = '0';
			IC_table[*IC].binary_op[9] = '0';
			break;
		    
		    case TYPE_LABEL:
			/*check label */
			if(!check_label( parser_t_p->label_name[k] , sym_head ,&error_flag , YES,row_num))
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

    	
return 0;
}

