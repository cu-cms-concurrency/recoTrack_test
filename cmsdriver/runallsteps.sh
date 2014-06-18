#! /bin/sh

for n in $(seq 0 13) ; do
  f=$(printf "steps%02d.log" $n)
  /usr/bin/time cmsRun step3_RAW2DIGI_L1Reco_RECO_EI.py nSteps=$n maxEvents=1000 >& $f
done
