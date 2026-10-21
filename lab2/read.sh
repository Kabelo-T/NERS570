#!/bin/bash

files=( "$(find ./ -name "nuclear_secrets[0-9].txt" -d 3)" )
echo "${files[0]}"
#while read line; do 
#    grep -v -e $line "${files[1]}"
#    echo $line
#done < "${files[0]}"
