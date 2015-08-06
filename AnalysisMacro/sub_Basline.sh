#!/bin/bash

echo $DATE
echo $PWD

chmod 744 Baseline.cc

cd /afs/cern.ch/work/k/kakwok/public/FastTimeTestBeamAnalysis/AnalysisMacro
root -l -b Baseline.cc+

echo `Job done!` 
