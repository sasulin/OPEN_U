/*MAMAN 14 Final Project*/
/*Shmuel Asulin*/
/*Yotam Klein*/

/*Assembler*/

/*#include <stdio.h>
#include <stdlib.h>*/

#define OP_CODE_BIN_SIZE 5
#define MAX_OP_SIZE 5
#define OP_NUM 16


const char *reserved_words[]={"mov","cmp","add",
					  "sub","not","clr",
	 				  "lea","inc","dec",
      				  "jmp","bne","red",
      				  "prn","jsr","rts","stop",
					  "r0","r1","r2","r3","r4","r5","r6","r7",
					  "data","string","mat",".entry",".extern","999"};

const char *op_words[]={"mov","cmp","add",
					  "sub","not","clr",
	 				  "lea","inc","dec",
      				  "jmp","bne","red",
      				  "prn","jsr","rts","stop"};

const char *data_op_words[]={".data",".string",".mat",".entry",".extern"};

enum INSTRUCTION
{
	MOV,CMP,ADD,
	SUB,NOT,CLR,
	LEA,INC,DEC,
	JMP,BNE,RED,
	PRN,JSR,RTS,STOP   
};

typedef struct operation_list
{
	char name[MAX_OP_SIZE];
	int EnuM;
	char op_code_bin[OP_CODE_BIN_SIZE];

	/*Source location methods*/
	bool src_immediate;
	bool src_direct;
	bool src_matrix;
	bool src_register;

	/*Destination location methods*/
	bool dest_immediate;
	bool dest_direct;
	bool dest_matrix;
	bool dest_register;
}operation_list;

operation_list op_list[OP_NUM]={ {"mov",MOV,"0000"	,YES,YES,YES,YES		,NO,YES,YES,YES},
								 {"cmp",CMP,"0001"	,YES,YES,YES,YES		,YES,YES,YES,YES},
								 {"add",ADD,"0010"	,YES,YES,YES,YES		,NO,YES,YES,YES},
								 {"sub",SUB,"0011"	,YES,YES,YES,YES		,NO,YES,YES,YES},
								 {"not",NOT,"0100"	,NO,NO,NO,NO			,NO,YES,YES,YES},
								 {"clr",CLR,"0101"	,NO,NO,NO,NO			,NO,YES,YES,YES},
								 {"lea",LEA,"0110"	,YES,YES,NO,NO			,NO,YES,YES,YES},
								 {"inc",INC,"0111"	,NO,NO,NO,NO			,NO,YES,YES,YES},
								 {"dec",DEC,"1000"	,NO,NO,NO,NO			,NO,YES,YES,YES},
								 {"jmp",JMP,"1001"	,NO,NO,NO,NO			,NO,YES,YES,YES},
								 {"bne",BNE,"1010"	,NO,NO,NO,NO			,NO,YES,YES,YES},
								 {"red",RED,"1011"	,NO,NO,NO,NO			,NO,YES,YES,YES},
								 {"prn",PRN,"1100"	,NO,NO,NO,NO			,YES,YES,YES,YES},
								 {"jsr",JSR,"1101"	,NO,NO,NO,NO			,NO,YES,YES,YES},
								 {"rts",RTS,"1110"	,NO,NO,NO,NO			,NO,NO,NO,NO},
								 {"stop",STOP,"1111",NO,NO,NO,NO			,NO,NO,NO,NO},
																					};

