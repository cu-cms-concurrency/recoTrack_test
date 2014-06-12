#! /bin/bash

############################################################
# Compute timing info for all modules in iterativeTracking
############################################################

##################
in_dir=$1
out_dir=$2
##################

# create output directory

if [ -d $out_dir ] ; then # if it exists, delete all files and start over

    rm -r $out_dir
    mkdir $out_dir

elif [ ! -d $out_dir ] ; then # create otherwise

    mkdir $out_dir

fi

##########################################

# make a CSV for each iterative step, storing timing info of each module for both CPU and WALL CLOCK (REAL)

for filenumber in $(seq 1 `find . -name 'CMSSW_*.stdout' | wc -l`) #begin loop over all stdouts
  do

  for iteration in initialStep lowPtTripletStep pixelPairStep detachedTripletStep mixedTripletStep pixelLessStep tobTecStep generalTracks ConvStep conversionStepTracks #begin loop over the iterations 
    do

    if [ $iteration = "initialStep" ] ; then #initialStep has no ${iteration}Clusters
	for module in ${iteration}Seeds ${iteration}TrackCandidates ${iteration}Tracks ${iteration}Selector 
	  do
	  
	  grep -x "TimeReport   [0-9]\.[0-9][0-9]*   [0-9]\.[0-9]*   [0-9]\.[0-9]*   [0-9]\.[0-9]* $module" ${in_dir}/CMSSW_${filenumber}.stdout | tee >(printf "%s,%s\n" "$(cut -d " " -f 4)" "$module" >> ${out_dir}/${iteration}_CPU.csv) >  >(printf "%s,%s\n" "$(cut -d " " -f 7)" "$module" >> ${out_dir}/${iteration}_REAL.csv)  
	  
	done #end loop over modules for initialStep

    elif [ $iteration = "lowPtTripletStep" ] || [ $iteration = "pixelPairStep" ] || [ $iteration = "pixelLessStep" ] || [ $iteration = "tobTecStep" ] ; then            
	
 	for module in ${iteration}Clusters ${iteration}Seeds ${iteration}TrackCandidates ${iteration}Tracks ${iteration}Selector #begin loop over modules in each iteration (except initial, detached, mixed)                               
	  do
	  
	  grep -x "TimeReport   [0-9]\.[0-9][0-9]*   [0-9]\.[0-9]*   [0-9]\.[0-9]*   [0-9]\.[0-9]* $module" ${in_dir}/CMSSW_${filenumber}.stdout | tee >(printf "%s,%s\n" "$(cut -d " " -f 4)" "$module" >> ${out_dir}/${iteration}_CPU.csv) >  >(printf "%s,%s\n" "$(cut -d " " -f 7)" "$module" >> ${out_dir}/${iteration}_REAL.csv)  
	done #end loop over modules for elif                                                                                                                                                                                    
    
    elif [ $iteration = "detachedTripletStep" ] ; then

	for module in ${iteration}Clusters ${iteration}Seeds ${iteration}TrackCandidates ${iteration}Tracks ${iteration}Selector ${iteration} #begin loop over modules in detachedTripletStep
	  do

	  grep -x "TimeReport   [0-9]\.[0-9][0-9]*   [0-9]\.[0-9]*   [0-9]\.[0-9]*   [0-9]\.[0-9]* $module" ${in_dir}/CMSSW_${filenumber}.stdout | tee >(printf "%s,%s\n" "$(cut -d " " -f 4)" "$module" >> ${out_dir}/${iteration}_CPU.csv) >  >(printf "%s,%s\n" "$(cut -d " " -f 7)" "$module" >> ${out_dir}/${iteration}_REAL.csv)  
	done #end loop over modules for elif  - detachedTripletStep  
 
    elif [ $iteration = "mixedTripletStep" ] ; then

	for module in ${iteration}Clusters ${iteration}SeedsA ${iteration}SeedsB ${iteration}Seeds ${iteration}TrackCandidates ${iteration}Tracks ${iteration}Selector ${iteration} #begin loop over modules in mixeedTripletStep
	  do

	  grep -x "TimeReport   [0-9]\.[0-9][0-9]*   [0-9]\.[0-9]*   [0-9]\.[0-9]*   [0-9]\.[0-9]* $module" ${in_dir}/CMSSW_${filenumber}.stdout | tee >(printf "%s,%s\n" "$(cut -d " " -f 4)" "$module" >> ${out_dir}/${iteration}_CPU.csv) >  >(printf "%s,%s\n" "$(cut -d " " -f 7)" "$module" >> ${out_dir}/${iteration}_REAL.csv)  
	done #end loop over modules for elif  - mixedTripletStep  

    elif [ $iteration = "generalTracks" ] || [ $iteration = "conversionStepTracks" ] ; then
	
	grep -x "TimeReport   [0-9]\.[0-9][0-9]*   [0-9]\.[0-9]*   [0-9]\.[0-9]*   [0-9]\.[0-9]* $iteration" ${in_dir}/CMSSW_${filenumber}.stdout | tee >(printf "%s,%s\n" "$(cut -d " " -f 4)" "$iteration" >> ${out_dir}/${iteration}_CPU.csv) >  >(printf "%s,%s\n" "$(cut -d " " -f 7)" "$iteration" >> ${out_dir}/${iteration}_REAL.csv)  
	
    elif [ $iteration = "ConvStep" ] ; then
	
	for module in convClusters photonConvTrajSeedFromSingleLeg convTrackCandidates convStepTracks convStepSelector #ConvStep in iterativeTacking
	  do

	  grep -x "TimeReport   [0-9]\.[0-9][0-9]*   [0-9]\.[0-9]*   [0-9]\.[0-9]*   [0-9]\.[0-9]* $module" ${in_dir}/CMSSW_${filenumber}.stdout | tee >(printf "%s,%s\n" "$(cut -d " " -f 4)" "$module" >> ${out_dir}/${iteration}_CPU.csv) >  >(printf "%s,%s\n" "$(cut -d " " -f 7)" "$module" >> ${out_dir}/${iteration}_REAL.csv)  

	done #end loop over modules for elif  - ConvStep
	
    fi # end if conditional to match iterativeTk sequence to right modules
    
  done # end loop over iterations  

done # end loop over stdout files

########################### 

#Sort and get average time --> may move to a ROOT macro for better access to statistics

for iteration in initialStep lowPtTripletStep pixelPairStep detachedTripletStep mixedTripletStep pixelLessStep tobTecStep generalTracks ConvStep conversionStepTracks #begin loop over the iterations 
  do
  
  if [ $iteration = "initialStep" ] ; then #initialStep has no ${iteration}Clusters
      for module in ${iteration}Seeds ${iteration}TrackCandidates ${iteration}Tracks ${iteration}Selector 
	do

	grep -x "[0-9]\.[0-9].*,$module" ${out_dir}/${iteration}_CPU.csv >> ${out_dir}/${iteration}_CPU_sorted.csv
	grep -x "[0-9]\.[0-9].*,$module" ${out_dir}/${iteration}_REAL.csv >> ${out_dir}/${iteration}_REAL_sorted.csv
	
	grep -x "[0.9]\.[0.9].*,$module" ${out_dir}/${iteration}_CPU_sorted.csv | printf "%s,%s\n" "$(cut -d "," -f 1 | awk '{sum+=$1};END{print sum/NR}' )" "$module" >> ${out_dir}/averages_CPU.csv
	grep -x "[0.9]\.[0.9].*,$module" ${out_dir}/${iteration}_REAL_sorted.csv | printf "%s,%s\n" "$(cut -d "," -f 1 | awk '{sum+=$1};END{print sum/NR}' )" "$module" >> ${out_dir}/averages_REAL.csv
	
      done #end loop over modules for initialStep
      
  elif [ $iteration = "lowPtTripletStep" ] || [ $iteration = "pixelPairStep" ] ; then #keep average sorted by actual run order of modules as defined in recoTrack_MC_***_cfg.py
      
      for module in ${iteration}Clusters ${iteration}Seeds ${iteration}TrackCandidates ${iteration}Tracks ${iteration}Selector #begin loop over modules                               
	do

	grep -x "[0-9]\.[0-9].*,$module" ${out_dir}/${iteration}_CPU.csv >> ${out_dir}/${iteration}_CPU_sorted.csv
	grep -x "[0-9]\.[0-9].*,$module" ${out_dir}/${iteration}_REAL.csv >> ${out_dir}/${iteration}_REAL_sorted.csv
	
	grep -x "[0.9]\.[0.9].*,$module" ${out_dir}/${iteration}_CPU_sorted.csv | printf "%s,%s\n" "$(cut -d "," -f 1 | awk '{sum+=$1};END{print sum/NR}' )" "$module" >> ${out_dir}/averages_CPU.csv
	grep -x "[0.9]\.[0.9].*,$module" ${out_dir}/${iteration}_REAL_sorted.csv | printf "%s,%s\n" "$(cut -d "," -f 1 | awk '{sum+=$1};END{print sum/NR}' )" "$module" >> ${out_dir}/averages_REAL.csv
	
      done #end loop over modules for elif                                                                                                                                                                                    
      
  elif [ $iteration = "detachedTripletStep" ] ; then
      
      for module in ${iteration}Clusters ${iteration}Seeds ${iteration}TrackCandidates ${iteration}Tracks ${iteration}Selector ${iteration} #begin loop over modules in detachedTripletStep
	do
	
	grep -x "[0-9]\.[0-9].*,$module" ${out_dir}/${iteration}_CPU.csv >> ${out_dir}/${iteration}_CPU_sorted.csv
	grep -x "[0-9]\.[0-9].*,$module" ${out_dir}/${iteration}_REAL.csv >> ${out_dir}/${iteration}_REAL_sorted.csv
	
	grep -x "[0.9]\.[0.9].*,$module" ${out_dir}/${iteration}_CPU_sorted.csv | printf "%s,%s\n" "$(cut -d "," -f 1 | awk '{sum+=$1};END{print sum/NR}' )" "$module" >> ${out_dir}/averages_CPU.csv
	grep -x "[0.9]\.[0.9].*,$module" ${out_dir}/${iteration}_REAL_sorted.csv | printf "%s,%s\n" "$(cut -d "," -f 1 | awk '{sum+=$1};END{print sum/NR}' )" "$module" >> ${out_dir}/averages_REAL.csv
	
      done #end loop over modules for elif  - detachedTripletStep  
      
  elif [ $iteration = "mixedTripletStep" ] ; then
      
      for module in ${iteration}Clusters ${iteration}SeedsA ${iteration}SeedsB ${iteration}Seeds ${iteration}TrackCandidates ${iteration}Tracks ${iteration}Selector ${iteration} #begin loop over modules in mixeedTripletStep
	do

	grep -x "[0-9]\.[0-9].*,$module" ${out_dir}/${iteration}_CPU.csv >> ${out_dir}/${iteration}_CPU_sorted.csv
	grep -x "[0-9]\.[0-9].*,$module" ${out_dir}/${iteration}_REAL.csv >> ${out_dir}/${iteration}_REAL_sorted.csv
	
	grep -x "[0.9]\.[0.9].*,$module" ${out_dir}/${iteration}_CPU_sorted.csv | printf "%s,%s\n" "$(cut -d "," -f 1 | awk '{sum+=$1};END{print sum/NR}' )" "$module" >> ${out_dir}/averages_CPU.csv
	grep -x "[0.9]\.[0.9].*,$module" ${out_dir}/${iteration}_REAL_sorted.csv | printf "%s,%s\n" "$(cut -d "," -f 1 | awk '{sum+=$1};END{print sum/NR}' )" "$module" >> ${out_dir}/averages_REAL.csv	

      done #end loop over modules for elif  - mixedTripletStep  
      
  elif [ $iteration = "pixelLessStep" ] || [ $iteration = "tobTecStep" ] ; then            

      for module in ${iteration}Clusters ${iteration}Seeds ${iteration}TrackCandidates ${iteration}Tracks ${iteration}Selector #begin loop over modules                               
	do

      	grep -x "[0-9]\.[0-9].*,$module" ${out_dir}/${iteration}_CPU.csv >> ${out_dir}/${iteration}_CPU_sorted.csv
	grep -x "[0-9]\.[0-9].*,$module" ${out_dir}/${iteration}_REAL.csv >> ${out_dir}/${iteration}_REAL_sorted.csv
	
	grep -x "[0.9]\.[0.9].*,$module" ${out_dir}/${iteration}_CPU_sorted.csv | printf "%s,%s\n" "$(cut -d "," -f 1 | awk '{sum+=$1};END{print sum/NR}' )" "$module" >> ${out_dir}/averages_CPU.csv
	grep -x "[0.9]\.[0.9].*,$module" ${out_dir}/${iteration}_REAL_sorted.csv | printf "%s,%s\n" "$(cut -d "," -f 1 | awk '{sum+=$1};END{print sum/NR}' )" "$module" >> ${out_dir}/averages_REAL.csv

      done #end loop over next two iterations after mixed
      
  elif [ $iteration = "generalTracks" ] ; then
      
      grep -x "[0-9]\.[0-9].*,$iteration" ${out_dir}/${iteration}_CPU.csv >> ${out_dir}/${iteration}_CPU_sorted.csv
      grep -x "[0-9]\.[0-9].*,$iteration" ${out_dir}/${iteration}_REAL.csv >> ${out_dir}/${iteration}_REAL_sorted.csv
      
      grep -x "[0.9]\.[0.9].*,$iteration" ${out_dir}/${iteration}_CPU_sorted.csv | printf "%s,%s\n" "$(cut -d "," -f 1 | awk '{sum+=$1};END{print sum/NR}' )" "$iteration" >> ${out_dir}/averages_CPU.csv
      grep -x "[0.9]\.[0.9].*,$iteration" ${out_dir}/${iteration}_REAL_sorted.csv | printf "%s,%s\n" "$(cut -d "," -f 1 | awk '{sum+=$1};END{print sum/NR}' )" "$iteration" >> ${out_dir}/averages_REAL.csv
      
  elif [ $iteration = "ConvStep" ] ; then
      
      for module in convClusters photonConvTrajSeedFromSingleLeg convTrackCandidates convStepTracks convStepSelector #ConvStep in iterativeTacking
	do

      	grep -x "[0-9]\.[0-9].*,$module" ${out_dir}/${iteration}_CPU.csv >> ${out_dir}/${iteration}_CPU_sorted.csv
	grep -x "[0-9]\.[0-9].*,$module" ${out_dir}/${iteration}_REAL.csv >> ${out_dir}/${iteration}_REAL_sorted.csv
	
	grep -x "[0.9]\.[0.9].*,$module" ${out_dir}/${iteration}_CPU_sorted.csv | printf "%s,%s\n" "$(cut -d "," -f 1 | awk '{sum+=$1};END{print sum/NR}' )" "$module" >> ${out_dir}/averages_CPU.csv
	grep -x "[0.9]\.[0.9].*,$module" ${out_dir}/${iteration}_REAL_sorted.csv | printf "%s,%s\n" "$(cut -d "," -f 1 | awk '{sum+=$1};END{print sum/NR}' )" "$module" >> ${out_dir}/averages_REAL.csv
	
      done #end loop over modules for elif  - ConvStep
      
  elif [ $iteration = "conversionStepTracks" ] ; then
      
      grep -x "[0-9]\.[0-9].*,$iteration" ${out_dir}/${iteration}_CPU.csv >> ${out_dir}/${iteration}_CPU_sorted.csv
      grep -x "[0-9]\.[0-9].*,$iteration" ${out_dir}/${iteration}_REAL.csv >> ${out_dir}/${iteration}_REAL_sorted.csv
      
      grep -x "[0.9]\.[0.9].*,$iteration" ${out_dir}/${iteration}_CPU_sorted.csv | printf "%s,%s\n" "$(cut -d "," -f 1 | awk '{sum+=$1};END{print sum/NR}' )" "$iteration" >> ${out_dir}/averages_CPU.csv
      grep -x "[0.9]\.[0.9].*,$iteration" ${out_dir}/${iteration}_REAL_sorted.csv | printf "%s,%s\n" "$(cut -d "," -f 1 | awk '{sum+=$1};END{print sum/NR}' )" "$iteration" >> ${out_dir}/averages_REAL.csv

  fi # end if conditional to match iterativeTk sequence to right modules
  
done # end sort and averaging for each module

##########################################

# Remove unsorted files, only keep sorted

for iteration in initialStep lowPtTripletStep pixelPairStep detachedTripletStep mixedTripletStep pixelLessStep tobTecStep generalTracks ConvStep conversionStepTracks #remove all csv files
  do
  
  rm ${out_dir}/${iteration}_CPU.csv
  rm ${out_dir}/${iteration}_REAL.csv

done

##########################################

# Get the total time per iterative step


for iteration in initialStep lowPtTripletStep pixelPairStep detachedTripletStep mixedTripletStep pixelLessStep tobTecStep generalTracks ConvStep conversionStepTracks #begin loop over the iterations 
  do

  if [ $iteration != "ConvStep" ] ; then

      grep "$iteration" ${out_dir}/averages_CPU.csv | cut -d "," -f 1 | printf "%s,%s\n" "$(awk '{sum+=$1};END{print sum}')" "$iteration" >> ${out_dir}/totals.csv

  else

      grep -v "conversionStepTracks" ${out_dir}/averages_CPU.csv | grep -i "conv" | cut -d "," -f 1 | printf "%s,%s\n" "$(awk '{sum+=$1};END{print sum}')" "$iteration" ${out_dir}/totals.csv

  fi

done
