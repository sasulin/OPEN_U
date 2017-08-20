/*MAMAN 14 Final Project*/
/*Shmuel Asulin ,ID:036760676  */
/*Yotam Klein* , ID:066546896 */

/*Assembler*/

#ifndef MEMORY_SIZE
#define MEMORY_SIZE 256
#endif
#ifndef MAX_LABEL_SIZE
#define MAX_LABEL_SIZE 31 /*Defined in instructions*/
#endif
#ifndef WORD_SIZE
#define WORD_SIZE 10 
#endif
#ifndef WEIRD_BASE
#define WEIRD_BASE 4
#endif
#ifndef MAX_ROW_LEN
#define MAX_ROW_LEN 81
#endif
#ifndef FILE_NAME_LEN
#define FILE_NAME_LEN 65535
#endif
#ifndef LAST
#define LAST "999"
#endif
#ifndef INITIAL_IC
#define INITIAL_IC 100
#endif
#ifndef INITIAL_DC
#define INITIAL_DC 0
#endif

typedef enum{NO,YES}bool;

typedef char word[WORD_SIZE+1];

/*Defining a row in the INSTRUCTION table*/
typedef struct I_table_row *I_row_p;

typedef struct I_table_row
{
	char label[MAX_LABEL_SIZE];
	unsigned int dec_add;
	char weird_four_add[WEIRD_BASE+1];
	word binary_op;
	char weird_four_op[WORD_SIZE/2+1];
}I_table_row;


/*Defining a row in the DATA table*/
typedef struct D_table_row *D_row_p;
typedef struct D_table_row
{
	unsigned int dec_add;
	char weird_four_add[WEIRD_BASE+1];
	word binary_op;
	char weird_four_op[WORD_SIZE/2+1];
}D_table_row;


/*Defining a row in the symbol table*/
typedef struct symbol_table_row *sym_row_p;
typedef struct symbol_table_row
{
	char label[MAX_LABEL_SIZE];
	char weird_four_add[WEIRD_BASE+1];
	int dec_add;
	bool is_ent;
	bool is_ext;
	bool is_data_op;
    sym_row_p next;	

}symbol_row;

/*Structure that holds parsing temp parsing information*/
typedef struct parser_table *parser_table_p;
typedef struct parser_table 
{
    char label_name[MAX_ROW_LEN][MAX_LABEL_SIZE];
    char temp_string[MAX_ROW_LEN][MAX_LABEL_SIZE];
    int  type[MAX_ROW_LEN];
    int  first_arg[MAX_ROW_LEN];
    int  second_arg[MAX_ROW_LEN];

} parser_table;

	/*Functions declarations*/
bool        first_scan(FILE *fp, sym_row_p head,I_row_p,D_row_p,int *IC,int *DC);
bool        second_scan(FILE *fp,sym_row_p sym_head,I_row_p IC_table,D_row_p DC_table,int *IC,int *DC);
/*int parser(char *input ,parser_table_p parser_t_p,int);*/

/*File lines*/
bool        is_comment(char *arr,char *arr_tmp);
bool        is_empty(char *arr);

/*parsing*/
bool        check_op(char *op_string,bool*,bool*,bool*,bool*);
char        *tok_label(char * arr,char * arr_tmp,int label_pos,bool*);
char        *tok_get(char *arr , char *arr_tmp);
int         return_label_address(char *label,sym_row_p head);

/*General aux*/
void        no_space(char *str);
void        reverse(char *string); 

/*Base transformations*/
void        dec_to_weird(char *quad_num ,int dec_num);
void        quad_weird(char *quad_num);
void        dec_to_bin(int n,char *word , int);
void        bin_to_weird(char *bin,char *weird);
int 	    check_in_limit(int NUM , int row_num);

/*Symbo table*/
sym_row_p 	sym_alloc(void);
void 		print_sym_table(sym_row_p head);
void        initialize_sym_table(sym_row_p head);
void        add_symbol(sym_row_p head, char *label,int IC,int DC,bool is_ent, bool is_ext, bool is_data_op);

bool        check_label(char *label,sym_row_p head,bool*,bool,int row_number);
