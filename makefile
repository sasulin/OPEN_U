CC = gcc
CFLAGS = -ansi -pedantic -Wall -g

# -g flag is for debugging information

main: main.o first_scan.o
#A command that generates the target main:
	$(CC) $(CFLAGS) -o main main.o first_scan.o -lm

main.o : main.c aux_func.h	
	$(CC) $(CFLAGS) -c main.c 

first_scan.o : first_scan.c aux_func.h scan.h parser.c encoding.c encoding_first_scan.c
	$(CC) $(CFLAGS) -c first_scan.c

#parser.o : parser.c aux_func.h scan.h
#	$(CC) $(CFLAGS) -c parser.c 

#encoding.o : encoding.c aux_func.h scan.h
#	$(CC) $(CFLAGS) -c encoding.c 


clean:  
	rm *.o
	
