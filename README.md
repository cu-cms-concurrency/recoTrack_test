recoTrack_test
==============

Hello all.  I have included a small python configuration file to run the tracking reconstruction.  Technically, it is rerunning the reconstruction, so I still am looking into how to run it from the beginning.  It works for CMSSW_7_1_0_pre8, using a small RelVal ttbar sample that is currently available.  The goal will obviously be to move to something more stable. 

So after setting up the release area, all you have to do is : cmsRun recoTrack_cfg.py
The instructions (and subsequent modifications) came from the following question on CMS HyperNews:
https://hypernews.cern.ch/HyperNews/CMS/get/recoTracking/1438.html
