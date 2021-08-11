#!/bin/bash

# compile the ring.c file
gcc ring.c -o ring

# execute ring.c for cnt = 10, 100, 1000, 10000
for cnt in 10 100 1000 10000
do
    ./ring 20 0 $cnt >> timing.out
done