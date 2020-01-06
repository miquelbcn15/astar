#!/bin/bash

# An example session of how to execute the code. 
# Requirement: spain.csv file

make realclean
make all

# Generating binary file
./write spain.csv

# Finding the optimal path
echo '240949599 195977239' | ./astar spain.bin > solution.sol

# Generating the html map
python3 map.py solution.sol
