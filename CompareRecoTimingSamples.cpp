#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "TROOT.h"
#include "TStyle.h"
#include "TFile.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TString.h"

void compareSamples(std::string inDir, std::string sample1Dir, std::string sample2Dir, std::string outDir);

void compareSamples(std::string inDir, std::string sample1Dir, std::string sample2Dir, std::string outDir){

  // Get list of module names ... old way... really should automate this

  std::string avREAL_input = inDir;
  avREAL_input.append("/averages_REAL.csv");

  std::ifstream averages_REAL;
  averages_REAL.open(avREAL_input.c_str(),ios::in);
  
  std::string line;
  std::vector<string> averagesREAL;

  while(std::getline(averages_REAL,line)){
    std::stringstream lineStream(line);
    std::string cell;

    while(std::getline(lineStream,cell,',' )){
      averagesREAL.push_back(cell);
    }
  }

  // Push names of each module into a vector; will be the same for both REAL and CPU for both samples -- just use REAL sample 1

  std::vector<string>::iterator string_iter;
  std::vector<string> moduleName;

  for(string_iter = averagesREAL.begin() +1; string_iter != averagesREAL.end() +1; string_iter+=2){
    moduleName.push_back(*string_iter);
  }

  // Initialize all variables for creating the histos

  std::string iterationName[8] = {"initialStep","lowPtTripletStep","pixelPairStep","detachedTripletStep","mixedTripletStep","pixelLessStep","tobTecStep","convStep"}; // have to hardcode unfortunately
  UInt_t nIterations = 8;
  
  UInt_t iterNameLength[nIterations];
  for(UInt_t i = 0; i < nIterations; i++){iterNameLength[i] = iterationName[i].length();}

  UInt_t nModulesInIter[nIterations];
  for(UInt_t i = 0; i < nIterations; i++){nModulesInIter[i] = 0;}
  
  for(UInt_t i = 0; i < nIterations; i++){
    for(UInt_t j = 0; j < moduleName.size(); j++){
      if((moduleName[j].compare(0,iterNameLength[i],iterationName[i].c_str()) == 0) && (i != 7)){ //hard code skip last step, as conv is screwed up
	nModulesInIter[i]++;
      }
      else if( (i == 7) && ( ((moduleName[j].find("conv") == 0) || (moduleName[j].find("photonConv") == 0)) && (moduleName[j].find("convserionTracks") != 0) ) ){ //hard code convStep
	nModulesInIter[i]++;
      }
    }
  }

  // Get the Histos - sample1
  
  TFile * timing_plots_REAL1 = new TFile(Form("%s/timing_plots_REAL.root",sample1Dir.c_str()));
  TFile * timing_plots_CPU1 = new TFile(Form("%s/timing_plots_CPU.root",sample1Dir.c_str()));

  TH1D * iterationTimeCPU_h1[nIterations];
  TH1D * iterationTimeCPU_h_add1[nIterations];
  TH1D * iterationTimeREAL_h1[nIterations];
  TH1D * iterationTimeREAL_h_add1[nIterations];
  
  for(UInt_t i = 0; i < nIterations; i++){
    iterationTimeCPU_h1[i] = (TH1D*)timing_plots_CPU1->Get(Form("%s_h_CPU_Time",iterationName[i].c_str()));
    iterationTimeCPU_h_add1[i] = (TH1D*)timing_plots_CPU1->Get(Form("%s_h_CPU_Time_Add",iterationName[i].c_str()));
    iterationTimeREAL_h1[i] = (TH1D*)timing_plots_REAL1->Get(Form("%s_h_REAL_Time",iterationName[i].c_str()));
    iterationTimeREAL_h_add1[i] = (TH1D*)timing_plots_REAL1->Get(Form("%s_h_REAL_Time_Add",iterationName[i].c_str()));
  }

  TH1D * iterationTimeCPU_total_h1 = (TH1D*)timing_plots_CPU1->Get("CPU_total");
  TH1D * iterationTimeCPU_total_h_add1 = (TH1D*)timing_plots_CPU1->Get("CPU_total_Add");
  TH1D * iterationTimeREAL_total_h1 = (TH1D*)timing_plots_REAL1->Get("REAL_total");
  TH1D * iterationTimeREAL_total_h_add1 = (TH1D*)timing_plots_REAL1->Get("REAL_total_Add");
  
  // Sample 2 histos

  TFile * timing_plots_REAL2 = new TFile(Form("%s/timing_plots_REAL.root",sample2Dir.c_str()));
  TFile * timing_plots_CPU2 = new TFile(Form("%s/timing_plots_CPU.root",sample2Dir.c_str()));

  TH1D * iterationTimeCPU_h2[nIterations];
  TH1D * iterationTimeCPU_h_add2[nIterations];
  TH1D * iterationTimeREAL_h2[nIterations];
  TH1D * iterationTimeREAL_h_add2[nIterations];
  
  for(UInt_t i = 0; i < nIterations; i++){
    iterationTimeCPU_h2[i] = (TH1D*)timing_plots_CPU2->Get(Form("%s_h_CPU_Time",iterationName[i].c_str()));
    iterationTimeCPU_h_add2[i] = (TH1D*)timing_plots_CPU2->Get(Form("%s_h_CPU_Time_Add",iterationName[i].c_str()));
    iterationTimeREAL_h2[i] = (TH1D*)timing_plots_REAL2->Get(Form("%s_h_REAL_Time",iterationName[i].c_str()));
    iterationTimeREAL_h_add2[i] = (TH1D*)timing_plots_REAL2->Get(Form("%s_h_REAL_Time_Add",iterationName[i].c_str()));
  }

  TH1D * iterationTimeCPU_total_h2 = (TH1D*)timing_plots_CPU2->Get("CPU_total");
  TH1D * iterationTimeCPU_total_h_add2 = (TH1D*)timing_plots_CPU2->Get("CPU_total_Add");
  TH1D * iterationTimeREAL_total_h2 = (TH1D*)timing_plots_REAL2->Get("REAL_total");
  TH1D * iterationTimeREAL_total_h_add2 = (TH1D*)timing_plots_REAL2->Get("REAL_total_Add");
  
  // Compare CPU First

  // Initialize Comparision Plots 

  TCanvas * canvasCompareCPU[nIterations];
  for(UInt_t i = 0; i < nIterations; i++){
    canvasCompareCPU[i] = new TCanvas();
    canvasCompareCPU[i]->SetBottomMargin(0.17);
  }

  TCanvas * canvasCompareCPU_add[nIterations];
  for(UInt_t i = 0; i < nIterations; i++){
    canvasCompareCPU_add[i] = new TCanvas();
    canvasCompareCPU_add[i]->SetBottomMargin(0.17);
  }

  TCanvas * canvasCompareCPU_total = new TCanvas();
  canvasCompareCPU_total->SetBottomMargin(0.17);

  TCanvas * canvasCompareCPU_total_add = new TCanvas();
  canvasCompareCPU_total_add->SetBottomMargin(0.17);

  TH1D * iterationTimeCompareCPU_h[nIterations];
  for(UInt_t i = 0; i < nIterations; i++){
    iterationTimeCompareCPU_h[i] = new TH1D(Form("%s_h_CompareCPU_Time",iterationName[i].c_str()),Form("%s Compare Samples CPU Time",iterationName[i].c_str()),nModulesInIter[i],0,nModulesInIter[i]);
    iterationTimeCompareCPU_h[i]->SetStats(kFALSE);
    iterationTimeCompareCPU_h[i]->GetYaxis()->SetTitle("Ratio of Average Time/Event (s)");
    iterationTimeCompareCPU_h[i]->GetYaxis()->SetTitleOffset(1.3);
    iterationTimeCompareCPU_h[i]->SetMinimum(0.0);
  }

  TH1D * iterationTimeCompareCPU_h_add[nIterations];
  for(UInt_t i = 0; i < nIterations; i++){
    iterationTimeCompareCPU_h_add[i] = new TH1D(Form("%s_h_CompareCPU_Time_Add",iterationName[i].c_str()),Form("%s Compare Samples CPU Time Add",iterationName[i].c_str()),nModulesInIter[i],0,nModulesInIter[i]);
    iterationTimeCompareCPU_h_add[i]->SetStats(kFALSE);
    iterationTimeCompareCPU_h_add[i]->GetYaxis()->SetTitle("Ratio of Average Time:Added/Event (s)");
    iterationTimeCompareCPU_h_add[i]->GetYaxis()->SetTitleOffset(1.3);
    iterationTimeCompareCPU_h_add[i]->SetMinimum(0.0);
  }

  TH1D * iterationTimeCompareCPU_total_h = new TH1D("CompareCPU_total","Compare Samples CPU Total Time for Each Iteration",nIterations+2,0,nIterations+2); // +2 to include gentracks and conversionStepTracks ... hardcode 
  iterationTimeCompareCPU_total_h->SetStats(kFALSE);
  iterationTimeCompareCPU_total_h->GetYaxis()->SetTitle("Ratio of Average Time/Event (s)");
  iterationTimeCompareCPU_total_h->GetYaxis()->SetTitleOffset(1.3);
  iterationTimeCompareCPU_total_h->SetMinimum(0.0);

  TH1D * iterationTimeCompareCPU_total_h_add = new TH1D("CompareCPU_total_add","Compare Samples CPUC Total Time for Each Iteration Add",nIterations+2,0,nIterations+2); // +2 to include gentracks and conversionStepTracks ... hardcode 
  iterationTimeCompareCPU_total_h_add->SetStats(kFALSE);
  iterationTimeCompareCPU_total_h_add->GetYaxis()->SetTitle("Ratio of Average Time:Added/Event (s)");
  iterationTimeCompareCPU_total_h_add->GetYaxis()->SetTitleOffset(1.3);
  iterationTimeCompareCPU_total_h_add->SetMinimum(0.0);

  // Fill the Histos

  UInt_t bin_number = 0;

  for(UInt_t i = 0; i < nIterations; i++){
    iterationTimeCompareCPU_h[i]->Divide(iterationTimeCPU_h1[i],iterationTimeCPU_h2[i]);
    iterationTimeCompareCPU_h_add[i]->Divide(iterationTimeCPU_h_add1[i],iterationTimeCPU_h_add2[i]);
    
    for(UInt_t j = 0; j < moduleName.size(); j++){
      if(j == 0){bin_number = 0;} // reset bin counter
      if( ((moduleName[j].compare(0,iterNameLength[i],iterationName[i].c_str()) == 0)) && (i != 7) ){ // hardcode skip convStep
	bin_number++;

	iterationTimeCompareCPU_h[i]->GetXaxis()->SetBinLabel(bin_number,moduleName[j].c_str());
	iterationTimeCompareCPU_h_add[i]->GetXaxis()->SetBinLabel(bin_number,moduleName[j].c_str());
      }
      else if( (i == 7) && (((moduleName[j].find("conv") == 0) || (moduleName[j].find("photonConv") == 0)) && (moduleName[j].find("convserionTracks") != 0)) ){ //hard code convStep
	bin_number++;

	iterationTimeCompareCPU_h[i]->GetXaxis()->SetBinLabel(bin_number,moduleName[j].c_str());
	iterationTimeCompareCPU_h_add[i]->GetXaxis()->SetBinLabel(bin_number,moduleName[j].c_str());
      }
    }
  }

  // Make Total Time Histo

  iterationTimeCompareCPU_total_h->Divide(iterationTimeCPU_total_h1,iterationTimeCPU_total_h2);
  iterationTimeCompareCPU_total_h_add->Divide(iterationTimeCPU_total_h_add1,iterationTimeCPU_total_h_add2);
  
  for(UInt_t i = 0; i < nIterations+2; i++){
    if(i < nIterations){      
      iterationTimeCompareCPU_total_h->GetXaxis()->SetBinLabel(i+1,iterationName[i].c_str());
      iterationTimeCompareCPU_total_h_add->GetXaxis()->SetBinLabel(i+1,iterationName[i].c_str());
    }
    else if(i == nIterations){ // include general tracks    
      iterationTimeCompareCPU_total_h->GetXaxis()->SetBinLabel(i+1,"generalTracks");
      iterationTimeCompareCPU_total_h_add->GetXaxis()->SetBinLabel(i+1,"generalTracks");
    }
    else if(i == nIterations+1){ // include conversionStepTracks
      iterationTimeCompareCPU_total_h->GetXaxis()->SetBinLabel(i+1,"conversionStepTracks");
      iterationTimeCompareCPU_total_h_add->GetXaxis()->SetBinLabel(i+1,"conversionStepTracks");
    }
  }

  // Draw graphs and save them
  // Write histos to root file

  TFile * compare_samples_CPU = new TFile(Form("%s/compareSamplesCPU.root",outDir.c_str()),"RECREATE");

  for(UInt_t i = 0; i < nIterations; i++){
    canvasCompareCPU[i]->cd();
    iterationTimeCompareCPU_h[i]->Draw();
    iterationTimeCompareCPU_h[i]->Write();

    compare_samples_CPU->cd();
    canvasCompareCPU[i]->Print(Form("%s/%sCompareCPU.png",outDir.c_str(),iterationName[i].c_str()),"png");
    
    canvasCompareCPU_add[i]->cd();
    iterationTimeCompareCPU_h_add[i]->Draw();
    iterationTimeCompareCPU_h_add[i]->Write();

    compare_samples_CPU->cd();
    canvasCompareCPU_add[i]->Print(Form("%s/%sCompareCPU_add.png",outDir.c_str(),iterationName[i].c_str()),"png");
  }
  
  canvasCompareCPU_total->cd();
  iterationTimeCompareCPU_total_h->Draw();
  iterationTimeCompareCPU_total_h->Write();
  
  compare_samples_CPU->cd();
  canvasCompareCPU_total->Print(Form("%s/iterationTotalCompareCPU.png",outDir.c_str()),"png");
  
  canvasCompareCPU_total_add->cd();
  iterationTimeCompareCPU_total_h_add->Draw();
  iterationTimeCompareCPU_total_h_add->Write();
  
  compare_samples_CPU->cd();
  canvasCompareCPU_total_add->Print(Form("%s/iterationTotalCompareCPU_add.png",outDir.c_str()),"png");

  // Compare REAL plots

  // Initialize Comparision Plots 

  TCanvas * canvasCompareREAL[nIterations];
  for(UInt_t i = 0; i < nIterations; i++){
    canvasCompareREAL[i] = new TCanvas();
    canvasCompareREAL[i]->SetBottomMargin(0.17);
  }

  TCanvas * canvasCompareREAL_add[nIterations];
  for(UInt_t i = 0; i < nIterations; i++){
    canvasCompareREAL_add[i] = new TCanvas();
    canvasCompareREAL_add[i]->SetBottomMargin(0.17);
  }

  TCanvas * canvasCompareREAL_total = new TCanvas();
  canvasCompareREAL_total->SetBottomMargin(0.17);

  TCanvas * canvasCompareREAL_total_add = new TCanvas();
  canvasCompareREAL_total_add->SetBottomMargin(0.17);

  TH1D * iterationTimeCompareREAL_h[nIterations];
  for(UInt_t i = 0; i < nIterations; i++){
    iterationTimeCompareREAL_h[i] = new TH1D(Form("%s_h_CompareREAL_Time",iterationName[i].c_str()),Form("%s Compare Samples REAL Time",iterationName[i].c_str()),nModulesInIter[i],0,nModulesInIter[i]);
    iterationTimeCompareREAL_h[i]->SetStats(kFALSE);
    iterationTimeCompareREAL_h[i]->GetYaxis()->SetTitle("Ratio of Average Time/Event (s)");
    iterationTimeCompareREAL_h[i]->GetYaxis()->SetTitleOffset(1.3);
    iterationTimeCompareREAL_h[i]->SetMinimum(0.0);
  }

  TH1D * iterationTimeCompareREAL_h_add[nIterations];
  for(UInt_t i = 0; i < nIterations; i++){
    iterationTimeCompareREAL_h_add[i] = new TH1D(Form("%s_h_CompareREAL_Time_Add",iterationName[i].c_str()),Form("%s Compare Samples REAL Time Add",iterationName[i].c_str()),nModulesInIter[i],0,nModulesInIter[i]);
    iterationTimeCompareREAL_h_add[i]->SetStats(kFALSE);
    iterationTimeCompareREAL_h_add[i]->GetYaxis()->SetTitle("Ratio of Average Time:Added/Event (s)");
    iterationTimeCompareREAL_h_add[i]->GetYaxis()->SetTitleOffset(1.3);
    iterationTimeCompareREAL_h_add[i]->SetMinimum(0.0);
  }

  TH1D * iterationTimeCompareREAL_total_h = new TH1D("CompareREAL_total","Compare Samples REAL Total Time for Each Iteration",nIterations+2,0,nIterations+2); // +2 to include gentracks and conversionStepTracks ... hardcode 
  iterationTimeCompareREAL_total_h->SetStats(kFALSE);
  iterationTimeCompareREAL_total_h->GetYaxis()->SetTitle("Ratio of Average Time/Event (s)");
  iterationTimeCompareREAL_total_h->GetYaxis()->SetTitleOffset(1.3);
  iterationTimeCompareREAL_total_h->SetMinimum(0.0);

  TH1D * iterationTimeCompareREAL_total_h_add = new TH1D("CompareREAL_total_add","Compare Samples REAL Total Time for Each Iteration Add",nIterations+2,0,nIterations+2); // +2 to include gentracks and conversionStepTracks ... hardcode 
  iterationTimeCompareREAL_total_h_add->SetStats(kFALSE);
  iterationTimeCompareREAL_total_h_add->GetYaxis()->SetTitle("Ratio of Average Time:Added/Event (s)");
  iterationTimeCompareREAL_total_h_add->GetYaxis()->SetTitleOffset(1.3);
  iterationTimeCompareREAL_total_h_add->SetMinimum(0.0);

  // Fill the Histos

  bin_number = 0;

  for(UInt_t i = 0; i < nIterations; i++){
    iterationTimeCompareREAL_h[i]->Divide(iterationTimeREAL_h1[i],iterationTimeREAL_h2[i]);
    iterationTimeCompareREAL_h_add[i]->Divide(iterationTimeREAL_h_add1[i],iterationTimeREAL_h_add2[i]);
    
    for(UInt_t j = 0; j < moduleName.size(); j++){
      if(j == 0){bin_number = 0;} // reset bin counter
      if( ((moduleName[j].compare(0,iterNameLength[i],iterationName[i].c_str()) == 0)) && (i != 7) ){ // hardcode skip convStep
	bin_number++;

	iterationTimeCompareREAL_h[i]->GetXaxis()->SetBinLabel(bin_number,moduleName[j].c_str());
	iterationTimeCompareREAL_h_add[i]->GetXaxis()->SetBinLabel(bin_number,moduleName[j].c_str());
      }
      else if( (i == 7) && (((moduleName[j].find("conv") == 0) || (moduleName[j].find("photonConv") == 0)) && (moduleName[j].find("convserionTracks") != 0)) ){ //hard code convStep
	bin_number++;

	iterationTimeCompareREAL_h[i]->GetXaxis()->SetBinLabel(bin_number,moduleName[j].c_str());
	iterationTimeCompareREAL_h_add[i]->GetXaxis()->SetBinLabel(bin_number,moduleName[j].c_str());
      }
    }
  }

  // Make Total Time Histo

  iterationTimeCompareREAL_total_h->Divide(iterationTimeREAL_total_h1,iterationTimeREAL_total_h2);
  iterationTimeCompareREAL_total_h_add->Divide(iterationTimeREAL_total_h_add1,iterationTimeREAL_total_h_add2);
  
  for(UInt_t i = 0; i < nIterations+2; i++){
    if(i < nIterations){      
      iterationTimeCompareREAL_total_h->GetXaxis()->SetBinLabel(i+1,iterationName[i].c_str());
      iterationTimeCompareREAL_total_h_add->GetXaxis()->SetBinLabel(i+1,iterationName[i].c_str());
    }
    else if(i == nIterations){ // include general tracks    
      iterationTimeCompareREAL_total_h->GetXaxis()->SetBinLabel(i+1,"generalTracks");
      iterationTimeCompareREAL_total_h_add->GetXaxis()->SetBinLabel(i+1,"generalTracks");
    }
    else if(i == nIterations+1){ // include conversionStepTracks
      iterationTimeCompareREAL_total_h->GetXaxis()->SetBinLabel(i+1,"conversionStepTracks");
      iterationTimeCompareREAL_total_h_add->GetXaxis()->SetBinLabel(i+1,"conversionStepTracks");
    }
  }

  // Draw graphs and save them
  // Write histos to root file

  TFile * compare_samples_REAL = new TFile(Form("%s/compareSamplesREAL.root",outDir.c_str()),"RECREATE");

  for(UInt_t i = 0; i < nIterations; i++){
    canvasCompareREAL[i]->cd();
    iterationTimeCompareREAL_h[i]->Draw();
    iterationTimeCompareREAL_h[i]->Write();

    compare_samples_REAL->cd();
    canvasCompareREAL[i]->Print(Form("%s/%sCompareREAL.png",outDir.c_str(),iterationName[i].c_str()),"png");
    
    canvasCompareREAL_add[i]->cd();
    iterationTimeCompareREAL_h_add[i]->Draw();
    iterationTimeCompareREAL_h_add[i]->Write();

    compare_samples_REAL->cd();
    canvasCompareREAL_add[i]->Print(Form("%s/%sCompareREAL_add.png",outDir.c_str(),iterationName[i].c_str()),"png");
  }
  
  canvasCompareREAL_total->cd();
  iterationTimeCompareREAL_total_h->Draw();
  iterationTimeCompareREAL_total_h->Write();
  
  compare_samples_REAL->cd();
  canvasCompareREAL_total->Print(Form("%s/iterationTotalCompareREAL.png",outDir.c_str()),"png");
  
  canvasCompareREAL_total_add->cd();
  iterationTimeCompareREAL_total_h_add->Draw();
  iterationTimeCompareREAL_total_h_add->Write();
  
  compare_samples_REAL->cd();
  canvasCompareREAL_total_add->Print(Form("%s/iterationTotalCompareREAL_add.png",outDir.c_str()),"png");

    
}
