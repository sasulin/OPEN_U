#define LABEL_SIZE 31 /*Defined in instructions*/
#define INST_SIZE 10 
#define WORD_SIZE 10 
#define ADD_SIZE 5
#define MAX_ROW_LEN 100

void reverse (char *string); 
void dec_to_quad  (char *quad_num ,int dec_num);
void quad_weird (char *quad_num);
void parsing(char *line);

enum INSTRUCTION
{
	MOV,CMP,ADD,
	SUB,NOT,CLR,
	LEA,INC,DEC,
	JMP,BNE,RED,
	PRN,JSR,RTS,STOP   
};

typedef unsigned short int word[WORD_SIZE];

typedef struct I_table_row * row_p;

typedef struct I_table_row
{
	char label[LABEL_SIZE];
	unsigned int dec_add;
	char weird_four_add[ADD_SIZE];
	char inst[INST_SIZE];
	word row_word;
}I_table_row;

typedef struct symbol_table_row
{
	char symbol[LABEL_SIZE];
	unsigned int dec_add;
	char weird_four_add[ADD_SIZE];	
}symbol_table_row;


