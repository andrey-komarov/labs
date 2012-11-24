#!/bin/bash

while true
do
    python gen.py > cf.in
    time ./cf 2>/dev/null || break
    cp cf.out{,~}
    time java e
    diff cf.out{,~} || break
    cat cf.out
done
