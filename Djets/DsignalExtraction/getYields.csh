#!/bin/bash

# B. Trzeciak (Utrecht University)
#-- script to run D-jets yield extraction

##### data output settings
dataFile=$1
isEff=$2
effFile=$3
isRefl=$4
refFile=$5
ispostfix=$6
postfix=$7
dirOut=$8
isMoreFile=$9
prod=${10}
isprefix=${11}
isReflSys=${12}
ReflSysScale=${13}
zBin=${14}
################################################
############### D-jet signal
################################################

root -l -b -q signalExtraction_SB.C'("'$dataFile'", '$isEff', "'$effFile'",'$isRefl', "'$refFile'", '$ispostfix', "'$postfix'", "'$dirOut'", 1, '$isMoreFile',"'$prod'",'$isprefix','$isReflSys','$ReflSysScale','$zBin')' #> outfile 2>&1
