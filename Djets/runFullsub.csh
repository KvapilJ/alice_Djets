#!/bin/bash

#for i in 29 30 31 32 33 33 33 34 35 36 37
#do
#   sed -i "$i"' s/^##*//' run_analysis.csh
#   ./run_analysis.csh 0 0 0 0 0 0
#   sed -i "$i"' s/^/#/' run_analysis.csh
#done

path=/home/kvapil/work/analysis/pp_run2/D0jet/data_250319/data/sub

for file in $path/*.root
do
  insert=analysisdatafile="sub"${file#$path}
  sed "39 i $insert" run_analysis.csh
#  ./run_analysis.csh 0 0 0 0 0 0
done
