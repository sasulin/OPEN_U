#include <stdio.h>
#include <stdlib.h>

#define MEMORY_SIZE 256
#define LABEL_SIZE 31 /*Defined in instructions*/
#define INST_SIZE 10 
#define WORD_SIZE 10 
#define ADD_SIZE 5
#define MAX_ROW_LEN 81

void first_scan(FILE *fp);
void reverse (char *string); 
void dec_to_quad  (char *quad_num ,int dec_num);
void quad_weird (char *quad_num);

enum INSTRUCTION
{
	MOV,CMP,ADD,
	SUB,NOT,CLR,
	LEA,INC,DEC,
	JMP,BNE,RED,
	PRN,JSR,RTS,STOP   
};


typedef enum{NO,YES}bool;

typedef unsigned short int word[WORD_SIZE];

/*Defining a row in the INSTRUCTION table*/
/*typedef struct I_table_row * I_row_p;*/

typedef struct I_table_row
{
	char label[LABEL_SIZE];
	unsigned int dec_add;
	char weird_four_add[ADD_SIZE];
	char inst[INST_SIZE];
	word row_word;
}I_table_row;


/*Defining a row in the DATA table*/
/*typedef struct D_table_row * D_row_p*/;

typedef struct D_table_row
{
	char label[LABEL_SIZE];
	unsigned int dec_add;
	char weird_four_add[ADD_SIZE];
	char inst[INST_SIZE];
	word row_word;
}D_table_row;




/*Defining a row in the symbol table*/
typedef struct symbol_table_row *sym_row_p;
typedef struct symbol_table_row
{
	char label[LABEL_SIZE];
	char weird_four_add[ADD_SIZE];
	bool is_op;
	bool is_ext;
    sym_row_p next;	

}symbol_row;
