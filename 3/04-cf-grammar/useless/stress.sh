#!/bin/bash

while true
do
    rm useless.out{,~}
    python gen.py > useless.in
    time ./useless || break
    cp useless.out{,~}
    time java Task || break
    diff -wb useless.out{,~} || break
    cat useless.out
done
