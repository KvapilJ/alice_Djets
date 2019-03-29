#!/bin/bash
rm dataFileList
for eachfile in DATA/*.root
do
  echo $eachfile >> ./dataFileList
done
