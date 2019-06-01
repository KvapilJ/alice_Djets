#!/bin/bash

for i in 34 35 36 37
do
   sed -i "$i"' s/^##*//' run_analysis.csh
   ./run_analysis.csh 0 5 0 0 0 0 0 0 0 0 0 0 0 0
   sed -i "$i"' s/^/#/' run_analysis.csh
done
