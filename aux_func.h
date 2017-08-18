/*MAMAN 14 Final Project*/
/*Shmuel Asulin*/
/*Yotam Klein*/

/*Assembler*/
/*#include <stdio.h>
#include <stdlib.h>*/

#define MEMORY_SIZE 256
#define MAX_LABEL_SIZE 31 /*Defined in instructions*/
#define WORD_SIZE 10 
#define WEIRD_BASE 4
#define MAX_ROW_LEN 81
#define FILE_NAME_LEN 65000
#define LAST "999"


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
	char label[MAX_LABEL_SIZE];
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
	bool is_ext;
	bool is_data_op;
    sym_row_p next;	

}symbol_row;

/*Functions declarations*/
bool first_scan(FILE *fp, sym_row_p head,I_row_p,D_row_p);
void reverse (char *string); 
void dec_to_weird  (char *quad_num ,int dec_num);
void quad_weird (char *quad_num);
sym_row_p 	sym_alloc(void);
void 		print_sym_table(sym_row_p head);
void initialize_sym_table(sym_row_p head);
void 		no_space(char *str);
void dec_to_bin(int n,char *word);
void bin_to_weird(char *bin,char *weird);
