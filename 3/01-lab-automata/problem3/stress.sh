#!/bin/bash

while true
do
    python gen.py > problem3.in
    ./a.out || break
    cat problem3.out
done
