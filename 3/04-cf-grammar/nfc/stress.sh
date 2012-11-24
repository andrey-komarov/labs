#!/bin/bash

while true
do
    python gen.py > nfc.in
    time ./nfc || break
    cp nfc.out{,~}
    time ./a.out
    diff nfc.out{,~} || break
    cat nfc.out
done
