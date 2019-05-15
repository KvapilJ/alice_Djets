#!/bin/bash
path=/home/kvapil/work/analysis/pp_run2/D0jet/data_250319/
file=AnalysisResults_MC_LHC18defghiklmnop.root
out=/home/kvapil/work/analysis/pp_run2/D0jet/scriptout
mkdir /home/kvapil/work/analysis/pp_run2/D0jet/scriptout
aliroot -l -b -q DsignalExtraction/signalExtraction_refl.C+'("'$path$file'","'$out'",0,"",0,"")'
