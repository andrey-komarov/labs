#!/bin/bash
i=1
while true
do
    python gen.py > fastminimization.in
    ./fast 
    cat fastminimization.out > isomorphism.in
    ./a.out
    cat fastminimization.out >> isomorphism.in
    ./isomorphism
    f=`cat isomorphism.out`
    if [[ $f != "YES" ]]
    then
        echo ":("
        exit 1
    fi
    let i=i+1
    echo $i
done
