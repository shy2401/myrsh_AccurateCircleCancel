#!/bin/bash

#批量生成数据

executable_program=./pushRelabel
files=`find ./exp_data -type f | sort -n`
# i=0

for inputFile in $files
do
    # echo "$i"
    # echo "$inputFile"
    # i=$(($i+1))
    tmp=${inputFile/exp_data/result}
    # echo "$tmp"
    resultFile=${tmp%/*}
    # echo "$resultFile"
    dir=${resultFile%/*}
    # echo "$dir"
    if [ ! -d "$dir" ]
    then
        mkdir -p "$dir"
    fi
    
    if [ ! -a "$resultFile" ]
    then
        touch "$resultFile"
    fi
    
    echo "$inputFile"
    "$executable_program" "$inputFile"  >> "$resultFile"
    echo m
done