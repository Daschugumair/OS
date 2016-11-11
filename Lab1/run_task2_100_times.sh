#!/bin/bash

# change the working directory to the path of the script
cd "${0%/*}"

for i in {1..100}
do
   ./task2
   echo ""
done
