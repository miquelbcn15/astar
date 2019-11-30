# Optimitzacio
OPT = -g -Wall

# Tot:
all : write read

# Utilitats:
#  - Write the binary file
write : testWrite.c writebin.o auxFun.o readCsv.o writebin.h readCsv.h auxFun.h
	gcc -o write $(OPT) testWrite.c writebin.o readCsv.o auxFun.o
# - Read the binary file
read : testRead.c writebin.o auxFun.o writebin.h auxFun.h
	gcc -o read $(OPT) testRead.c writebin.o auxFun.o

# Biblioteca
auxFun.o: auxFun.c auxFun.h
	gcc -c $(OPT) auxFun.c
writebin.o : writebin.c writebin.h auxFun.c
	gcc -c $(OPT) writebin.c auxFun.c
readCsv.o : readCsv.c readCsv.h auxFun.o
	gcc -c $(OPT) readCsv.c auxFun.c

# Neteja
clean : 
	rm -f *.o
realclean : clean
	rm -f write read

