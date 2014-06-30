#!/bin/bash

############################################################
# Make the timing plots
############################################################

##################
in_dir=$1
sample1_dir=$2
sample2_dir=$3
out_dir=$4
##################

# create output directory

if [ -d $out_dir ] ; then # if it exists, delete all files and start over

    rm -r $out_dir
    mkdir $out_dir

elif [ ! -d $out_dir ] ; then # create otherwise

    mkdir $out_dir

fi

root -b -q -l "SampleComparePlots.C("\"$in_dir\",\"$sample1_dir\",\"$sample2_dir\",\"$out_dir\"")"
