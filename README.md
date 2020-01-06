# A Star Algorithm
A Star Algorithm from the Optimization course 19-20

## How to run the code
It is required to set the csv file from the map into the src folder

```bash
cd src/
make all
./write spain.csv
echo 'id_source id_goal' | ./main spain.bin 0.5=weight 0=epsilon
