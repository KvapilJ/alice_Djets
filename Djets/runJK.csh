#!/bin/bash

for i in 29 30 31 32 33
do
   sed -i "$i"' s/^##*//' run_analysis.csh
   ./run_analysis.csh 0 5 0 0 0 0
   sed -i "$i"' s/^/#/' run_analysis.csh
done
