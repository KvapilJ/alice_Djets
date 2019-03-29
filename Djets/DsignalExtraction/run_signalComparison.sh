#!/bin/bash
dataFolder="$HOME/work/analysis/pp_run2/D0jet/data/dataMerge"
workFolder="$HOME/work/analysis/pp_run2/D0jet"
processFolder="$workFolder/SignalComparisonM"
mkdir $processFolder

#aliroot -b -q -x 'signalExtraction_SB.C+("$HOME/work/analysis/pp_run2/D0jet/data/AnalysisResults_LHC16o_pass1.root",0,"",0,"",0,"","$HOME/work/analysis/pp_run2/D0jet/SignalComparison/signalExtraction_LHC16d_pass1",1,0,"")'
filenames=`ls $dataFolder/*.root`
for eachfile in $filenames
do
  echo "processing file: "$eachfile
  period=${eachfile#"$dataFolder/AnalysisResults_"}
  period=${period%".root"}
  mkdir $processFolder/SignalComparison_$period/
  aliroot -b -q -x "signalExtraction_SB.C+(\"$eachfile\",0,\"\",0,\"\",0,\"\",\"$processFolder/SignalComparison_$period\",1,0,\"\")" > $processFolder/SignalComparison_$period/$period.log 2>&1
done

rm $processFolder/SignalComparsonOKfilelist
for eachfile in $processFolder/*/*.root
do
  echo $eachfile >> $processFolder/SignalComparsonOKfilelist
done


