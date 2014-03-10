#!/bin/bash

#批量生成数据

executable_program=./pushRelabel
files=`find ./newGen_data_20130707 -type f | sort -n`
# i=0

for inputFile in $files
do
    # echo "$i"
    # echo "$inputFile"
    # i=$(($i+1))
    resultFile=${inputFile/newGen_data_20130707/newGen_data_result}
    # echo "$tmp"
    # resultFile=new_data_result/g15_21-94_result
    # echo "$resultFile"
    # dir=${resultFile%/*}
    # echo "$dir"
    # if [ ! -d "$dir" ]
    # then
    #     mkdir -p "$dir"
    # fi
    
    # if [ ! -a "$resultFile" ]
    # then
    #     touch "$resultFile"
    # fi
    
    echo "$inputFile"
    # echo "$resultFile"
    "$executable_program" "$inputFile"  >> "$resultFile"
    echo m
done
