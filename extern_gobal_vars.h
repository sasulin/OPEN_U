/*MAMAN 14 Final Project*/
/*Shmuel Asulin ,ID:036760676 */
/*Yotam Klein* , ID:066546896 */

/*Assembler*/
/*Const global vars definition */



/*Only const vars are defined here ,Following MAMAN 14 instractions */

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


