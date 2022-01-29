#!/bin/bash

# B. Trzeciak (Utrecht University)
#-- script to run D-jets analysis

################################################
############### FULL with sys
################################################

unfType=0     # unfolding type, 0: bayes. 1: SVD
foldType=0     # unfolding type, 0: 1D. 1: 2D
regPar=5      # regularization parameter for unfolding
isPrior=0     # if 0 deafult prior (MC true) is used, otherwise a prior defined in the macro
priorType=0   # if isPrior != 0, define which prior function you want to use, you can define new ones in the unfolding macro
bkgRMtype=0   # 0: default, if you use external bkg. fluctuation matrix define which one should be used, based on this name of a root file with a corresponding bkg. fluctuation matrix is set in the run_analysis.csh script (you can change this so it corresponds to your file)
zbin=0

#systematics -- these are additional settings - in order to use them you need to configure your run_analysis_*.csh script
doBkg=0            #not Used
doSys=0            #not Used
doRawCutVarSys=0   #not Used
doCutVar=0
doJESSys=0       # - rename mc files
doFDSys=0
doReflSys=0
doRangeSys=0
doPrior=0
doUnfSys=0         # - ranges
doUnfTypeSys=0     # - iter +-1, SVD +-1 + def with ranges

############### Default
doRawSignal=0
# run your run_analysis_*.csh script
echo $unfType $regPar $isPrior $priorType $bkgRMtype $doRawSignal $doCutVar $doRawCutVarSys $doJESSys $doFDSys $doSys $doReflSys $doRangeSys $doUnfSys $zbin $foldType
./run_analysis.csh $unfType $regPar $isPrior $priorType $bkgRMtype $doRawSignal $doCutVar $doRawCutVarSys $doJESSys $doFDSys $doSys $doReflSys $doRangeSys $doUnfSys $zbin $foldType

################################################
############### Additional (examples from D0-jet p-Pb analysis)
################################################

if [ $doPrior -eq 1 ]; then
  for prior in `seq 0 1 8`; do
        isPrior=1
        priorType=$prior
    ./run_analysis.csh $unfType $regPar $isPrior $priorType $bkgRMtype $doRawSignal 0 $doRawCutVarSys 0 0 $doSys 0 0 0 $zbin $foldType
  done
isPrior=0
priorType=0
fi


if [ $doUnfTypeSys -eq 1 ]; then
  ./run_analysis.csh 0 4 $isPrior $priorType $bkgRMtype $doRawSignal 0 $doRawCutVarSys 0 0 $doSys 0 0 0 $zbin $foldType
  ./run_analysis.csh 0 6 $isPrior $priorType $bkgRMtype $doRawSignal 0 $doRawCutVarSys 0 0 $doSys 0 0 0 $zbin $foldType
  ./run_analysis.csh 1 5 $isPrior $priorType $bkgRMtype $doRawSignal 0 $doRawCutVarSys 0 0 $doSys 0 0 0 $zbin $foldType
  ./run_analysis.csh 1 6 $isPrior $priorType $bkgRMtype $doRawSignal 0 $doRawCutVarSys 0 0 $doSys 0 0 1 $zbin $foldType
  ./run_analysis.csh 1 7 $isPrior $priorType $bkgRMtype $doRawSignal 0 $doRawCutVarSys 0 0 $doSys 0 0 0 $zbin $foldType
fi


#./run_analysis.csh $unfType $regPar 1 0 $bkgRMtype $doRawSignal $doCutVar $doRawCutVarSys $doJESSys $doFDSys $doSys $doReflSys $doRangeSys $doUnfSys $zbin $foldType
#./run_analysis.csh $unfType $regPar 1 1 $bkgRMtype $doRawSignal $doCutVar $doRawCutVarSys $doJESSys $doFDSys $doSys $doReflSys $doRangeSys $doUnfSys $zbin $foldType
#./run_analysis.csh $unfType $regPar 1 2 $bkgRMtype $doRawSignal $doCutVar $doRawCutVarSys $doJESSys $doFDSys $doSys $doReflSys $doRangeSys $doUnfSys $zbin $foldType
#./run_analysis.csh $unfType $regPar 1 3 $bkgRMtype $doRawSignal $doCutVar $doRawCutVarSys $doJESSys $doFDSys $doSys $doReflSys $doRangeSys $doUnfSys $zbin $foldType
#./run_analysis.csh $unfType $regPar 1 4 $bkgRMtype $doRawSignal $doCutVar $doRawCutVarSys $doJESSys $doFDSys $doSys $doReflSys $doRangeSys $doUnfSys $zbin $foldType
#./run_analysis.csh $unfType $regPar 1 5 $bkgRMtype $doRawSignal $doCutVar $doRawCutVarSys $doJESSys $doFDSys $doSys $doReflSys $doRangeSys $doUnfSys $zbin $foldType
#./run_analysis.csh $unfType $regPar 1 6 $bkgRMtype $doRawSignal $doCutVar $doRawCutVarSys $doJESSys $doFDSys $doSys $doReflSys $doRangeSys $doUnfSys $zbin $foldType
#./run_analysis.csh $unfType $regPar 1 7 $bkgRMtype $doRawSignal $doCutVar $doRawCutVarSys $doJESSys $doFDSys $doSys $doReflSys $doRangeSys $doUnfSys $zbin $foldType


################################################
############### Background variations -- here, if you have different bkg. fluctuation matrices, you need to set them up first and then run in the loop over them
################################################

if [ $doBkg -gt 0 ]; then
  for bkg in `seq 1 1 11`; do
                bkgRMtype=$bkg
      ./run_analysis.csh $unfType $regPar $isPrior $priorType $bkgRMtype $doRawSignal $doCutVar $doRawCutVarSys $doJESSys $doFDSys $doSys
  done
bkgRMtype=0
fi


################################################
############### Prior variations -- here, you need to set them up first your unfolding macro and then run in the loop over priors that you have
################################################

#if [ $doPrior -gt 0 ]; then
##  for prior in `seq 0 1 8`; do
#        isPrior=1
#        priorType=$prior
#    ./run_analysis.csh $unfType $regPar $isPrior $priorType $bkgRMtype $doRawSignal $doCutVar $doRawCutVarSys $doJESSys $doFDSys $doSys
#  done
#isPrior=0
#priorType=0
#fi
