#!/bin/bash

# change the working directory to the path of the script
cd "${0%/*}"

time for i in {1..100}
do
   ./task4
   echo ""
done
