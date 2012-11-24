#!/bin/bash

while true
do
    python gen.py > problem5.in
    ./a.out || break
    cat problem5.out
done
