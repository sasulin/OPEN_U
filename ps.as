;file ps.as

LABEL:.entry LOOP
.entry LENGTH
TEST: .extern L3
.extern W
MAIN:	mov	M1[r2][r7],W


        add  #5,M1[r2][r7]
;THIS IS AN ERROR:
;        .mat [2][3] 1,2,3,4,5,6,7,8


		add	r2,STR
LOOP:	jmp W
		prn	#-5
		sub	r1,r4
		inc	K

		mov M1[r3][r3],r3
		bne L3
END:	stop
STR:	.string "abcdef"
LENGTH:	.data 511,-512,15
K: 		.data 22
;M1:		.mat [2][2] 1,2,3,4,5,6
M1:		.mat [3][3] 1,2
STRUext:	.string "abcd  ef"
;End of file
