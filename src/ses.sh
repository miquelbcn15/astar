#!/bin/bash

# Generates data for the Barcelona-Sevilla route using different values for
# epsilon and w (weight)
# We use the heursitic function known as harversine

# Building the executable
make realclean
make all

# Setting epsilon to 0 and modifying w: by default epsilon is 0
index=0
for i in $(LANG=en_us seq -f "%f" 0 0.1 1)
do
    cat ../data/barcelonasevilla.txt | ./astar spain.bin ${i} 0 >\
        ../data/weight0${index}.sol
    index=$((index+1))
done

# Setting weight to 1/2 and modifying epsilon: by default epsilon is 0
index=0
for i in $(LANG=en_us seq -f "%f" 0 0.1 1)
do
    cat ../data/barcelonasevilla.txt | ./astar spain.bin .5 ${i} >\
        ../data/epsilon0${index}.sol
    index=$((index+1))
done
