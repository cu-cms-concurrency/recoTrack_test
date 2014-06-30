#!/bin/bash

############################################################
# Make the timing plots
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

root -b -q -l "TimingPlotsMacro.C("\"$in_dir\",\"$out_dir\"")"
