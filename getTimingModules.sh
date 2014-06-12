#! /bin/bash

# create output directory

if [ ! -d timing_info ] ; then
    mkdir timing_info
fi

# Compute timing info for all modules in iterativeTracking

for tkReco_module in initialStep lowPtTripletStep pixelPairStep detachedTripletStep mixedTripletStep pixelLessStep tobTecStep ConvStep #remove all csv files
  do
  
  rm timing_info/${tkReco_module}_CPU_sorted.csv
  rm timing_info/${tkReco_module}_REAL_sorted.csv

done

rm timing_info/generalTracks_CPU.csv
rm timing_info/generalTracks_REAL.csv
rm timing_info/conversionStepTracks_CPU.csv
rm timing_info/conversionStepTracks_REAL.csv
rm averages_CPU.csv
rm averages_REAL.csv

#make a CSV for each Iteration, either CPU or REAL

for filenumber in $(seq 1 `find . -name 'CMSSW_*.stdout' | wc -l`) #begin loop over all stdouts
  do

  for module in initialStepSeeds initialStepTrackCandidates initialStepTracks initialStepSelector #loop over initialStep, missing Clusters, could add an if statement later
    do
    
    grep -x "TimeReport   [0-9]\.[0-9][0-9]*   [0-9]\.[0-9]*   [0-9]\.[0-9]*   [0-9]\.[0-9]* $module" /uscms/home/kmcdermo/Concurrency/CMSSW_5_3_13/src/trackingTutorial/ttbar_5_3_13_timing/res/CMSSW_$filenumber.stdout | tee >(printf "%s,%s\n" "$(cut -d " " -f 4)" "$module" >> timing_info/initialStep_CPU.csv) >  >(printf "%s,%s\n" "$(cut -d " " -f 7)" "$module" >> timing_info/initialStep_REAL.csv)  

  done # end loop for initialStep

  for iteration in lowPtTripletStep pixelPairStep pixelLessStep tobTecStep #begin loop over the iterations (exept initialStep, mixedTripletStep, detachedTripletStep  
    do

    for module in ${iteration}Clusters ${iteration}Seeds ${iteration}TrackCandidates ${iteration}Tracks ${iteration}Selector #begin loop over modules in each iteration   
      do    

      grep -x "TimeReport   [0-9]\.[0-9][0-9]*   [0-9]\.[0-9]*   [0-9]\.[0-9]*   [0-9]\.[0-9]* $module" /uscms/home/kmcdermo/Concurrency/CMSSW_5_3_13/src/trackingTutorial/ttbar_5_3_13_timing/res/CMSSW_$filenumber.stdout | tee >(printf "%s,%s\n" "$(cut -d " " -f 4)" "$module" >> timing_info/${iteration}_CPU.csv) >  >(printf "%s,%s\n" "$(cut -d " " -f 7)" "$module" >> timing_info/${iteration}_REAL.csv)  
      
    done #end loop over modules 

  done #end loop over iterations except mixedTripletStep, initialStep, detachedTripletStep
  
  for module in detachedTripletStepClusters detachedTripletStepSeeds detachedTripletStepTrackCandidates detachedTripletStepTracks detachedTripletStepSelector detachedTripletStep #begin loop over modules in mixedTripletStep
    do
    
    grep -x "TimeReport   [0-9]\.[0-9][0-9]*   [0-9]\.[0-9]*   [0-9]\.[0-9]*   [0-9]\.[0-9]* $module" /uscms/home/kmcdermo/Concurrency/CMSSW_5_3_13/src/trackingTutorial/ttbar_5_3_13_timing/res/CMSSW_$filenumber.stdout | tee >(printf "%s,%s\n" "$(cut -d " " -f 4)" "$module" >> timing_info/detachedTripletStep_CPU.csv) >  >(printf "%s,%s\n" "$(cut -d " " -f 7)" "$module" >> timing_info/detachedTripletStep_REAL.csv)  
    
  done #end loop over modules in mixedTripletStep

  for module in mixedTripletStepClusters mixedTripletStepSeedsA mixedTripletStepSeedsB mixedTripletStepSeeds mixedTripletStepTrackCandidates mixedTripletStepTracks mixedTripletStepSelector mixedTripletStep #begin loop over modules in mixedTripletStep
    do
    
    grep -x "TimeReport   [0-9]\.[0-9][0-9]*   [0-9]\.[0-9]*   [0-9]\.[0-9]*   [0-9]\.[0-9]* $module" /uscms/home/kmcdermo/Concurrency/CMSSW_5_3_13/src/trackingTutorial/ttbar_5_3_13_timing/res/CMSSW_$filenumber.stdout | tee >(printf "%s,%s\n" "$(cut -d " " -f 4)" "$module" >> timing_info/mixedTripletStep_CPU.csv) >  >(printf "%s,%s\n" "$(cut -d " " -f 7)" "$module" >> timing_info/mixedTripletStep_REAL.csv)  
    
  done #end loop over modules in mixedTripletStep

  # create CSV for module in tkReco "generalTracks"
  grep -x "TimeReport   [0-9]\.[0-9][0-9]*   [0-9]\.[0-9]*   [0-9]\.[0-9]*   [0-9]\.[0-9]* generalTracks" /uscms/home/kmcdermo/Concurrency/CMSSW_5_3_13/src/trackingTutorial/ttbar_5_3_13_timing/res/CMSSW_$filenumber.stdout | tee >(printf "%s,generalTracks\n" "$(cut -d " " -f 4)" >> timing_info/generalTracks_CPU.csv) >  >(printf "%s,generalTracks\n" "$(cut -d " " -f 7)" >> timing_info/generalTracks_REAL.csv)  


  for module in convClusters photonConvTrajSeedFromSingleLeg convTrackCandidates convStepTracks convStepSelector #ConvStep in iterativeTacking
    do
    
    grep -x "TimeReport   [0-9]\.[0-9][0-9]*   [0-9]\.[0-9]*   [0-9]\.[0-9]*   [0-9]\.[0-9]* $module" /uscms/home/kmcdermo/Concurrency/CMSSW_5_3_13/src/trackingTutorial/ttbar_5_3_13_timing/res/CMSSW_$filenumber.stdout | tee >(printf "%s,%s\n" "$(cut -d " " -f 4)" "$module" >> timing_info/ConvStep_CPU.csv) >  >(printf "%s,%s\n" "$(cut -d " " -f 7)" "$module" >> timing_info/ConvStep_REAL.csv)  

  done

  # create CSV for module in tkReco "conversionStepTracks"
  grep -x "TimeReport   [0-9]\.[0-9][0-9]*   [0-9]\.[0-9]*   [0-9]\.[0-9]*   [0-9]\.[0-9]* conversionStepTracks" /uscms/home/kmcdermo/Concurrency/CMSSW_5_3_13/src/trackingTutorial/ttbar_5_3_13_timing/res/CMSSW_$filenumber.stdout | tee >(printf "%s,conversionStepTracks\n" "$(cut -d " " -f 4)" >> timing_info/conversionStepTracks_CPU.csv) >  >(printf "%s,conversionStepTracks\n" "$(cut -d " " -f 7)" >> timing_info/conversionStepTracks_REAL.csv)  

done #end loop over all files



########################### Sort and get "averages"




# Sort by modules to make it easier to manipulate the files

for module in initialStepSeeds initialStepTrackCandidates initialStepTracks initialStepSelector #loop over initialStep
  do

#  echo "here $module"
  
  grep -x "[0-9]\.[0-9].*,$module" timing_info/initialStep_CPU.csv >> timing_info/initialStep_CPU_sorted.csv
  grep -x "[0-9]\.[0-9].*,$module" timing_info/initialStep_REAL.csv >> timing_info/initialStep_REAL_sorted.csv
  
  grep -x "[0.9]\.[0.9].*,$module" timing_info/initialStep_CPU_sorted.csv | printf "%s,%s\n" "$(cut -d "," -f 1| awk '{sum+=$1};END{print sum/NR}' )" "$module" >> averages_CPU.csv
  grep -x "[0.9]\.[0.9].*,$module" timing_info/initialStep_REAL_sorted.csv | printf "%s,%s\n" "$(cut -d "," -f 1| awk '{sum+=$1};END{print sum/NR}' )" "$module" >> averages_REAL.csv

done

for iteration in lowPtTripletStep pixelPairStep pixelLessStep tobTecStep  
  do

  for module in ${iteration}Clusters ${iteration}Seeds ${iteration}TrackCandidates ${iteration}Tracks ${iteration}Selector 
    do

 #     echo "here $module"

    grep -x "[0-9]\.[0-9].*,$module" timing_info/${iteration}_CPU.csv >> timing_info/${iteration}_CPU_sorted.csv
    grep -x "[0-9]\.[0-9].*,$module" timing_info/${iteration}_REAL.csv >> timing_info/${iteration}_REAL_sorted.csv

    grep -x "[0.9]\.[0.9].*,$module" timing_info/${iteration}_CPU_sorted.csv | printf "%s,%s\n" "$(cut -d "," -f 1| awk '{sum+=$1};END{print sum/NR}' )" "$module" >> averages_CPU.csv
    grep -x "[0.9]\.[0.9].*,$module" timing_info/${iteration}_REAL_sorted.csv | printf "%s,%s\n" "$(cut -d "," -f 1| awk '{sum+=$1};END{print sum/NR}' )" "$module" >> averages_REAL.csv

  done

done

#Sort detached
for module in detachedTripletStepClusters detachedTripletStepSeeds detachedTripletStepTrackCandidates detachedTripletStepTracks detachedTripletStepSelector detachedTripletStep
  do
  #echo "here $module"
  grep -x "[0-9]\.[0-9].*,$module" timing_info/detachedTripletStep_CPU.csv >> timing_info/detachedTripletStep_CPU_sorted.csv
  grep -x "[0-9]\.[0-9].*,$module" timing_info/detachedTripletStep_REAL.csv >> timing_info/detachedTripletStep_REAL_sorted.csv

  grep -x "[0.9]\.[0.9].*,$module" timing_info/detachedTripletStep_CPU_sorted.csv | printf "%s,%s\n" "$(cut -d "," -f 1| awk '{sum+=$1};END{print sum/NR}' )" "$module" >> averages_CPU.csv
  grep -x "[0.9]\.[0.9].*,$module" timing_info/detachedTripletStep_REAL_sorted.csv | printf "%s,%s\n" "$(cut -d "," -f 1| awk '{sum+=$1};END{print sum/NR}' )" "$module" >> averages_REAL.csv

done

# Sort mixed
for module in mixedTripletStepClusters mixedTripletStepSeedsA mixedTripletStepSeedsB mixedTripletStepSeeds mixedTripletStepTrackCandidates mixedTripletStepTracks mixedTripletStepSelector mixedTripletStep #begin loop over modules in mixedTripletStep
  do
  #echo "here $module"
  grep -x "[0-9]\.[0-9].*,$module" timing_info/mixedTripletStep_CPU.csv >> timing_info/mixedTripletStep_CPU_sorted.csv
  grep -x "[0-9]\.[0-9].*,$module" timing_info/mixedTripletStep_REAL.csv >> timing_info/mixedTripletStep_REAL_sorted.csv

  grep -x "[0.9]\.[0.9].*,$module" timing_info/mixedTripletStep_CPU_sorted.csv | printf "%s,%s\n" "$(cut -d "," -f 1| awk '{sum+=$1};END{print sum/NR}' )" "$module" >> averages_CPU.csv
  grep -x "[0.9]\.[0.9].*,$module" timing_info/mixedTripletStep_REAL_sorted.csv | printf "%s,%s\n" "$(cut -d "," -f 1| awk '{sum+=$1};END{print sum/NR}' )" "$module" >> averages_REAL.csv

done

#Average Gen. tracks
  #echo "here generalTracks"
grep -x "[0.9]\.[0.9].*,generalTracks" timing_info/generalTracks_CPU.csv | printf "%s,generalTracks\n" "$(cut -d "," -f 1| awk '{sum+=$1};END{print sum/NR}' )" >> averages_CPU.csv
grep -x "[0.9]\.[0.9].*,generalTracks" timing_info/generalTracks_REAL.csv | printf "%s,generalTracks\n" "$(cut -d "," -f 1| awk '{sum+=$1};END{print sum/NR}' )" >> averages_REAL.csv

for module in convClusters photonConvTrajSeedFromSingleLeg convTrackCandidates convStepTracks convStepSelector #ConvStep in iterativeTacking
  do
  #echo "here $module"
  grep -x "[0-9]\.[0-9].*,$module" timing_info/ConvStep_CPU.csv >> timing_info/ConvStep_CPU_sorted.csv
  grep -x "[0-9]\.[0-9].*,$module" timing_info/ConvStep_REAL.csv >> timing_info/ConvStep_REAL_sorted.csv

  grep -x "[0.9]\.[0.9].*,$module" timing_info/ConvStep_CPU_sorted.csv | printf "%s,%s\n" "$(cut -d "," -f 1| awk '{sum+=$1};END{print sum/NR}' )" "$module" >> averages_CPU.csv
  grep -x "[0.9]\.[0.9].*,$module" timing_info/ConvStep_REAL_sorted.csv | printf "%s,%s\n" "$(cut -d "," -f 1| awk '{sum+=$1};END{print sum/NR}' )" "$module" >> averages_REAL.csv

done

#Average conv tracks

#  echo "here conversionTracks"
grep -x "[0.9]\.[0.9].*,conversionStepTracks" timing_info/conversionStepTracks_CPU.csv | printf "%s,conversionStepTracks\n" "$(cut -d "," -f 1| awk '{sum+=$1};END{print sum/NR}' )" >> averages_CPU.csv
grep -x "[0.9]\.[0.9].*,conversionStepTracks" timing_info/conversionStepTracks_REAL.csv | printf "%s,conversionStepTracks\n" "$(cut -d "," -f 1| awk '{sum+=$1};END{print sum/NR}' )" >> averages_REAL.csv

for tkReco_module in initialStep lowPtTripletStep pixelPairStep detachedTripletStep mixedTripletStep pixelLessStep tobTecStep ConvStep #remove all temp csv files (except general/conversionStepTracks)
  do
  
  rm timing_info/${tkReco_module}_CPU.csv
  rm timing_info/${tkReco_module}_REAL.csv

done






#
#  for module in pixelPairStepClusters pixelPairStepSeeds pixelPairStepTrackCandidates pixelPairStepTracks pixelPairStepSelector
#    do
#
#    grep "TimeReport   [0-9]\.[0-9][0-9]*   [0-9]\.[0-9]*   [0-9]\.[0-9]*   [0-9]\.[0-9]* $module" /uscms/home/kmcdermo/Concurrency/CMSSW_5_3_13/src/trackingTutorial/ttbar_5_3_13_timing/res/CMSSW_$filenumber.stdout | tee >(printf "%s,%s\n" "$(cut -d " " -f 4)" "$module" >> timing_info/pixelPairStep_CPU.csv) >  >(printf "%s,%s\n" "$(cut -d " " -f 7)" "$module" >> timing_info/pixelPairStep_REAL.csv) 
#
#    done
#


#Compute the totals for each iteration and save to the end of the csv

#
#for module in initialStepSeeds initialStepTrackCandidates initialStepTracks initialStepSelector                                                                                                                                           
#  do    
#
#  total_files_CPU=$(grep "$module" initialStep_CPU.csv | wc -l)
#  
#  
#  for i in $total_files_CPU #i is the iterator for essentially the amount of files that returned an output from the first nested loop
#    do
#
#    
#
#    done
#
#  total_files_REAL=$(grep "$module" initialStep_REAL.csv | wc -l)
#
#  for i in $total_files_REAL #i is the iterator for essentially the amount of files that returned an output from the first nested loop
#    do
#
#    
#
#    done
#
#  
#  
#
#done
#
