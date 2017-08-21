/*MAMAN 14 Final Project*/
/*Shmuel Asulin ,ID:036760676  */
/*Yotam Klein* , ID:066546896 */

/*Assembler*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aux_func.h"
#include "scan.h"


int encoding_first_scan(operation_list *command , D_row_p DC_table , I_row_p IC_table , sym_row_p sym_head , parser_table_p parser_t_p , int *DC , int *IC ,int row_num)

{

    int i = 0 ,k , error_flag = 0;
    switch(command->EnuM)
    {
/*case need DC encoding*/

	case STRING:
	    if(parser_t_p->type[0] != TYPE_STRING )
	    {
		
		printf("ERROR in row#%d:, Wrong argument to .string command\n" ,row_num);
		return 1;
	    } 
	    for(i=1;parser_t_p->temp_string[0][i] != '"';i++)
		{
		dec_to_bin(parser_t_p->temp_string[0][i],DC_table[*DC].binary_op,WORD_SIZE);
		(*DC)++;
		/*printf("DC value incrised by 1 %d" , *DC);		*/
		}

	    /*adding zero to the string end*/
	    dec_to_bin(0,DC_table[*DC].binary_op,WORD_SIZE);
	    (*DC)++;
	    /*Checking that there is no more ags after string*/
	    if(parser_t_p->type[1])
	    {
		printf("ERROR in row#%d: Too many arguments .string command \n",row_num);
		return 1;
	    }
	    return 0;

	case DATA:
	    for(i=0;parser_t_p->type[i];i++)
	    {
		/* check that all types are numbers */
		if(parser_t_p->type[i] != TYPE_NUM )
		{
		printf("ERROR in row#%d: Wrong argument to .data \n" , row_num);
		return 1;
		}
		/* check that all |numbers| < MAX_NUM_10*/
		if(!((parser_t_p->first_arg[i] < MAX_NUM_10)  &&  (parser_t_p->first_arg[i] > (MIN_NUM_10)))  )
		{
		printf("ERROR in row#%d:,\"%d\" is Out Of Range\n",row_num , parser_t_p->first_arg[i] );
		return 1;
		}
		
		/* add to table*/
		dec_to_bin(parser_t_p->first_arg[i],DC_table[*DC].binary_op,WORD_SIZE);
		(*DC)++;
	    }
	    return 0;
	    

	case MAT:
	    for(i=1;parser_t_p->type[i];i++)
	    {
		/* need to add check that all types are numbers */
		if(parser_t_p->type[i] != TYPE_NUM )
		{
		printf("ERROR in row#%d: Wrong argument to .mat command  \n",row_num );
		return 1; 
		}
		/* check that all |numbers| < MAX_NUM_10*/
		if(!((parser_t_p->first_arg[i] < MAX_NUM_10)  &&  (parser_t_p->first_arg[i] > (MIN_NUM_10)))  )
		{
		printf("ERROR in row#%d:\"%d\" is Out Of Range\n",row_num , parser_t_p->first_arg[i] );
		return 1; 
		}
		/* add to table*/
		dec_to_bin(parser_t_p->first_arg[i],DC_table[*DC].binary_op,WORD_SIZE);
		(*DC)++;
	    }

	    if((parser_t_p->first_arg[0])*(parser_t_p->second_arg[0]) < i-1)
	    {
		printf("ERROR in row#%d: Too many arguments .mat command \n",row_num);
		return 1;
	    }

	    /* padding data tabel with zeros */

	    for(--i;(parser_t_p->first_arg[0])*(parser_t_p->second_arg[0]) > i;i++)
	    {
		dec_to_bin(0,DC_table[*DC].binary_op,WORD_SIZE);
		(*DC)++;
	    }
	    return 0;

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
			if(parser_t_p->type[1] == TYPE_REG) 
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

