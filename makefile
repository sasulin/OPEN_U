CC = gcc
CFLAGS = -ansi -pedantic -Wall -g

# -g flag is for debugging information

main: main.o first_scan.o 
#A command that generates the target main:
	$(CC) $(CFLAGS) -o main main.o first_scan.o -lm

main.o : main.c aux_func.h	
	$(CC) $(CFLAGS) -c main.c 

first_scan.o : first_scan.c aux_func.h scan.h parser.c
	$(CC) $(CFLAGS) -c first_scan.c 

clean:  
	rm main.o
	rm first_scan.o	
	
