void TimingPlotsMacro(char * inDir, char * outDir){
  gROOT->LoadMacro("RecoModuleTimingPlots.cpp++g");

  /*  RecoTimingModulePlots * TimingObject = new RecoTimingModulePlots;
  
  TimingObject->SetInDir(inDir);
  TimingObject->SetOutDir(outDir);

  TimingObject->producePlots();*/

  producePlots(inDir,outDir);
}
