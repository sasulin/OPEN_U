/*MAMAN 14 Final Project*/
/*Shmuel Asulin*/
/*Yotam Klein*/

/*Assembler*/

#ifndef OP_CODE_BIN_SIZE
#define OP_CODE_BIN_SIZE 5
#endif
#ifndef MAX_OP_SIZE
#define MAX_OP_SIZE 10
#endif
#ifndef OP_NUM
#define OP_NUM 19
#endif

#ifndef INITIAL_IC
#define INITIAL_IC 100
#endif
#ifndef INITIAL_DC
#define INITIAL_DC 0
#endif
#ifndef MAX_OP_LEN
#define MAX_OP_LEN 8
#endif

#ifndef REG_LEN
#define REG_LEN 4
#endif

#ifndef WORD_LEN
#define WORD_LEN 10
#endif

#ifndef COMMAND_LEN
#define COMMAND_LEN 8
#endif

const char *reserved_words[]={"mov","cmp","add",
                    "sub","not","clr",
                    "lea","inc","dec",
                    "jmp","bne","red",
                    "prn","jsr","rts","stop",
                    "r0","r1","r2","r3","r4","r5","r6","r7",
                    "data","string","mat",".entry",".extern",LAST};

const char *op_words[]={"mov","cmp","add",
           "sub","not","clr",
           "lea","inc","dec",
           "jmp","bne","red",
           "prn","jsr","rts","stop",LAST};

const char *data_op_words[]={".data",".string",".mat",LAST};

enum INSTRUCTION
{
	MOV,CMP,ADD,
	SUB,NOT,CLR,
	LEA,INC,DEC,
	JMP,BNE,RED,
	PRN,JSR,RTS,STOP,
	STRING,DATA,MAT
};

enum LABEL_POSITION {START,MID};

enum status 
             {
                FIRST,
                NUM,ALPHA,ERROR,
                FIRST_CHAR_IN_TOKEN_IS_R,
                REG,DIGIT_AFTER_ALPHA,POUND,
                IN_STRING,STRING_END,
                INSIDE_MATRIX_FIRST_R,READING_MATRIX_FIRST_ARG,
                INSIDE_MATRIX_SECOUND_R,READING_MATRIX_SECOUND_ARG,
                INSIDE_MATRIX_FIRST_IS_NUM,
                DEF_METRIX_FIRST_ARG,
                DEF_METRIX_SECOUND_ARG,
                NO_MORE_ARGS
             };

enum TYPE  {
               TYPE_ERROR,
               TYPE_REG,
               TYPE_NUM,
               TYPE_LABEL,
               TYPE_DIRECT,
               TYPE_MATRIX,
               TYPE_STRING,
               TYPE_DEF_METRIX
           };

enum encoding {A,R,E};


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

operation_list op_list[OP_NUM]={
	{"mov",MOV,"0000"		,YES,YES,YES,YES	,NO,YES,YES,YES},
	{"cmp",CMP,"0001"		,YES,YES,YES,YES	,YES,YES,YES,YES},
	{"add",ADD,"0010"		,YES,YES,YES,YES	,NO,YES,YES,YES},
	{"sub",SUB,"0011"		,YES,YES,YES,YES	,NO,YES,YES,YES},
	{"not",NOT,"0100"		,NO,NO,NO,NO		,NO,YES,YES,YES},
	{"clr",CLR,"0101"		,NO,NO,NO,NO		,NO,YES,YES,YES},
	{"lea",LEA,"0110"		,YES,YES,NO,NO		,NO,YES,YES,YES},
	{"inc",INC,"0111"		,NO,NO,NO,NO		,NO,YES,YES,YES},
	{"dec",DEC,"1000"		,NO,NO,NO,NO		,NO,YES,YES,YES},
	{"jmp",JMP,"1001"		,NO,NO,NO,NO		,NO,YES,YES,YES},
	{"bne",BNE,"1010"		,NO,NO,NO,NO		,NO,YES,YES,YES},
	{"red",RED,"1011"		,NO,NO,NO,NO		,NO,YES,YES,YES},
	{"prn",PRN,"1100"		,NO,NO,NO,NO		,YES,YES,YES,YES},
	{"jsr",JSR,"1101"		,NO,NO,NO,NO		,NO,YES,YES,YES},
	{"rts",RTS,"1110"		,NO,NO,NO,NO		,NO,NO,NO,NO},
	{"stop",STOP,"1111"		,NO,NO,NO,NO		,NO,NO,NO,NO},
	{".string",STRING,"0"	,NO,NO,NO,NO		,NO,NO,NO,NO},
	{".data",DATA,"0"		,NO,NO,NO,NO		,NO,NO,NO,NO},
	{".mat",MAT,"0"			,NO,NO,NO,NO		,NO,NO,NO,NO},
																	};
