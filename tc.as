;file tc.as

LENGTH:	.data 6,-9,15




.extern A
.extern B
.extern r
.extern D
.extern E 
.extern F
.entry LENGTH
.extern L3
.extern W

  		;
		;

	MAIN:	mov	M1[r2][r7],LENGTH
		add	r2,STR
LOOP:	jmp	END

		prn	#-5
		sub	r1,r4
		inc	K
		mov M1[r3][r3],r3
		bne LOOP
END:	stop
STR			:	.string "abcdef"
K:		.data	22
M1:		.mat [2][2] 1,2,3,4
.extern ASUL

			;End of file
