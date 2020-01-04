# Optimitzacio
OPT = -g -Wall

# Tot:
all : write astar

# Utilitats:
#  - Write the binary file
write : write.c writebin.c auxFun.c readCsv.c writebin.h readCsv.h auxFun.h
	gcc -o write $(OPT) write.c writebin.c readCsv.c auxFun.c -lm
astar: main.c astar.c astar.h writebin.c writebin.h auxFun.c auxFun.h
	gcc -o astar $(OPT) main.c astar.c writebin.c auxFun.c -lm

# Neteja
clean : 
	rm -f *.o *.html *.data *.sol *.txt
realclean : clean
	rm -f write astar

