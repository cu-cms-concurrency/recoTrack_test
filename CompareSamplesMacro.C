void CompareSamplesMacro(char * inDir, char * sample1Dir, char * sample2Dir, char * outDir){
  gROOT->LoadMacro("CompareRecoTimingSamples.cpp++g");

  /*  RecoTimingModulePlots * TimingObject = new RecoTimingModulePlots;
  
  TimingObject->SetInDir(inDir);
  TimingObject->SetOutDir(outDir);

  TimingObject->producePlots();*/

  compareSamples(inDir,sample1Dir,sample2Dir,outDir);
}
