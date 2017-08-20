CC = gcc
CFLAGS = -ansi -pedantic -Wall -g

# -g flag is for debugging information

assembler: assembler.o scan.o
#A command that generates the target assembler:
	$(CC) $(CFLAGS) -o assembler assembler.o scan.o -lm

assembler.o : assembler.c aux_func.h	
	$(CC) $(CFLAGS) -c assembler.c 

scan.o : scan.c aux_func.h scan.h parser.c encoding_first_scan.c encoding_second_scan.c
	$(CC) $(CFLAGS) -c scan.c

#parser.o : parser.c aux_func.h scan.h
#	$(CC) $(CFLAGS) -c parser.c 

#encoding.o : encoding.c aux_func.h scan.h
#	$(CC) $(CFLAGS) -c encoding.c 


clean:  
	rm *.o
	
