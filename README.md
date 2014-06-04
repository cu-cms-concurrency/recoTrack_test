recoTrack_test
==============

I have included another configuration file generated from cmsDriver.py that will run the tracking reconstruction given some initial MC sample.  The initial sample can generated in the following way:

```
cmsDriver.py <Sample cfi.py file> -s GEN,SIM,DIGI,L1,DIGI2RAW,HLT --conditions auto:startup --eventcontent FEVTDEBUGHLT  -n <number of events> --no_exec
```

The ```<Sample cfi.py file>``` is the configuration file for a generic MC sample, where a plethora can be found in:  /uscmst1/prod/sw/cms/slc5_amd64_gccABC/cms/cmssw/CMSSW_X_Y_Z/src/Configuration/Generator/python/
or, with github:
https://github.com/kmcdermo/cmssw/tree/CMSSW_X_Y_Z/Configuration/Generator/python

I choose TTbar at 8TeV for starters, as it seems to be the consensus benchmark for any sort of tracking performance analysis, i.e. TTbar_8TeV_cfi.py. 

After running cmsDriver once, it will generate a python configuration file that then can be run with cmsRun to generate the MC.  To run, simply do: ```cmsRun <auto_generated_cfg.py file>```.  If you would rather just jump straight to generating and not create the python config file, drop the "--no_exec" option. 

From here, you will need to go into the ```recoTrack_userMC_cfg.py``` and make some small changes. On line 20, indicate the number of events you wish to run over, -1 being the default for all events.  On line 30 you will need to edit the filename and path to the file to match the output from cmsDriver.py.  The string ```'file:<file.root>'``` indicatest that the file is located locally.  To have python find a file published to DAS, provide the full path on that line and removed the ```'file:'``` string.  You can also change the name/path of the output of the trackin on line 50. 

To run, simply type ```cmsRun recoTrack_userMC_cfg.py```. 






I have included a small python configuration file to rerun the tracking reconstruction.  It works for CMSSW_7_1_0_pre8, using a small RelVal ttbar sample that is currently available.  The goal will obviously be to move to something more stable. 

So after setting up the release area, all you have to do is : cmsRun reRecoTrack_cfg.py


The instructions (and subsequent modifications) came from the following question on CMS HyperNews:
https://hypernews.cern.ch/HyperNews/CMS/get/recoTracking/1438.html

