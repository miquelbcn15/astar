# Optimitzacio
OPT = -g -Wall

# Tot:
all : write astar

# Utilitats:
#  - Write the binary file
write : write.c writebin.c auxFun.c readCsv.c writebin.h readCsv.h auxFun.h
	gcc -o write $(OPT) write.c writebin.c readCsv.c auxFun.c -lm
astar: newMain.c astar.c astar.h writebin.c writebin.h auxFun.c auxFun.h
	gcc -o astar $(OPT) newMain.c astar.c writebin.c auxFun.c -lm

# - Read the binary file
# read : testRead.c writebin.o auxFun.o writebin.h auxFun.h
# 	gcc -o read $(OPT) testRead.c writebin.o auxFun.o

# Biblioteca
# auxFun.o: auxFun.c auxFun.h
# 	gcc -c $(OPT) auxFun.c
# writebin.o : writebin.c writebin.h auxFun.c
# 	gcc -c $(OPT) writebin.c auxFun.c
# readCsv.o : readCsv.c readCsv.h auxFun.o
# 	gcc -c $(OPT) readCsv.c auxFun.c

# Neteja
clean : 
	rm -f *.o *.html *.txt *.data
realclean : clean
	rm -f write astar

