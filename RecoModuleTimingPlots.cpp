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

void producePlots(std::string inDir, std::string outDir);
void averageCPUPlots(std::string inDir, std::string outDir);
void averageREALPlots(std::string inDir, std::string outDir);
void compareCPUtoREAL(std::string inDir, std::string outDir);

void producePlots(std::string inDir, std::string outDir){

  averageCPUPlots(inDir,outDir);
  averageREALPlots(inDir,outDir);
  compareCPUtoREAL(inDir, outDir);

}

void averageCPUPlots(std::string inDir, std::string outDir){

  // Get average csvs
  
  std::string avCPU_input = inDir;
  avCPU_input.append("/averages_CPU.csv");

  std::ifstream averages_CPU;
  averages_CPU.open(avCPU_input.c_str(),ios::in);

  std::string line;
  std::vector<string> averagesCPU;

  while(std::getline(averages_CPU,line)){
    std::stringstream lineStream(line);
    std::string cell;

    while(std::getline(lineStream,cell,',' )){
      averagesCPU.push_back(cell);
    }
  }

  // Push names of each module into a vector; will be the same for both CPU and CPU -- just use CPU

  std::vector<string>::iterator string_iter;
  std::vector<string> moduleName;

  for(string_iter = averagesCPU.begin() +1; string_iter != averagesCPU.end() +1; string_iter+=2){
    moduleName.push_back(*string_iter);
  }

  // Grab time of each module as a string, then push time of each module into a vector of doubles
  // Index will match the naming vector by construction
  
  std::vector<string> moduleTimeCPU_s;

  for(string_iter = averagesCPU.begin(); string_iter != averagesCPU.end(); string_iter+=2){
    moduleTimeCPU_s.push_back(*string_iter);
  }
  
  std::vector<Double_t> moduleTimeCPU;

  for(string_iter = moduleTimeCPU_s.begin(); string_iter != moduleTimeCPU_s.end(); string_iter++){
    moduleTimeCPU.push_back( atof((*string_iter).c_str()) );
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
  
  // Initialize Histos and Canvases

  TCanvas * canvasCPU[nIterations];
  for(UInt_t i = 0; i < nIterations; i++){
    canvasCPU[i] = new TCanvas();
    canvasCPU[i]->SetBottomMargin(0.17);
  }

  TCanvas * canvasCPU_add[nIterations];
  for(UInt_t i = 0; i < nIterations; i++){
    canvasCPU_add[i] = new TCanvas();
    canvasCPU_add[i]->SetBottomMargin(0.17);
  }

  TCanvas * canvasCPU_total = new TCanvas();
  canvasCPU_total->SetBottomMargin(0.17);

  TCanvas * canvasCPU_total_add = new TCanvas();
  canvasCPU_total_add->SetBottomMargin(0.17);

  TH1D * iterationTimeCPU_h[nIterations];
  for(UInt_t i = 0; i < nIterations; i++){
    iterationTimeCPU_h[i] = new TH1D(Form("%s_h_CPU_Time",iterationName[i].c_str()),Form("%s CPU Time",iterationName[i].c_str()),nModulesInIter[i],0,nModulesInIter[i]);
    iterationTimeCPU_h[i]->SetStats(kFALSE);
    iterationTimeCPU_h[i]->GetYaxis()->SetTitle("Average Time/Event (s)");
    iterationTimeCPU_h[i]->GetYaxis()->SetTitleOffset(1.3);
    iterationTimeCPU_h[i]->SetMinimum(0.0);
  }

  TH1D * iterationTimeCPU_h_add[nIterations];
  for(UInt_t i = 0; i < nIterations; i++){
    iterationTimeCPU_h_add[i] = new TH1D(Form("%s_h_CPU_Time_Add",iterationName[i].c_str()),Form("%s CPU Time Add",iterationName[i].c_str()),nModulesInIter[i],0,nModulesInIter[i]);
    iterationTimeCPU_h_add[i]->SetStats(kFALSE);
    iterationTimeCPU_h_add[i]->GetYaxis()->SetTitle("Average Time:Added/Event (s)");
    iterationTimeCPU_h_add[i]->GetYaxis()->SetTitleOffset(1.3);
    iterationTimeCPU_h_add[i]->SetMinimum(0.0);
  }

  TH1D * iterationTimeCPU_total_h = new TH1D("CPU_total","CPU Total Time for Each Iteration",nIterations+2,0,nIterations+2); // +2 to include gentracks and conversionStepTracks ... hardcode 
  iterationTimeCPU_total_h->SetStats(kFALSE);
  iterationTimeCPU_total_h->GetYaxis()->SetTitle("Average Time/Event (s)");
  iterationTimeCPU_total_h->GetYaxis()->SetTitleOffset(1.3);
  iterationTimeCPU_total_h->SetMinimum(0.0);

  TH1D * iterationTimeCPU_total_h_add = new TH1D("CPU_total_Add","CPU Total Time for Each Iteration Add",nIterations+2,0,nIterations+2); // +2 to include gentracks and conversionStepTracks ... hardcode 
  iterationTimeCPU_total_h_add->SetStats(kFALSE);
  iterationTimeCPU_total_h_add->GetYaxis()->SetTitle("Average Time:Added/Event (s)");
  iterationTimeCPU_total_h_add->GetYaxis()->SetTitleOffset(1.3);
  iterationTimeCPU_total_h_add->SetMinimum(0.0);

  // MakeGraphs that are filled with the steps and their names

  Double_t iterationTimeCPU[nIterations];  // keep track of total time in each iteration
  for(UInt_t i = 0; i < nIterations; i++){iterationTimeCPU[i] = 0;}

  UInt_t bin_number = 0;

  for(UInt_t i = 0; i < nIterations; i++){
    for(UInt_t j = 0; j < moduleName.size(); j++){
      if(j == 0){bin_number = 0;} // reset bin counter
      if( ((moduleName[j].compare(0,iterNameLength[i],iterationName[i].c_str()) == 0)) && (i != 7) ){ // hardcode skip convStep
	bin_number++;

	iterationTimeCPU[i] += moduleTimeCPU[j];
	
	iterationTimeCPU_h[i]->SetBinContent(bin_number,moduleTimeCPU[j]);
	iterationTimeCPU_h[i]->GetXaxis()->SetBinLabel(bin_number,moduleName[j].c_str());

	iterationTimeCPU_h_add[i]->SetBinContent(bin_number,iterationTimeCPU[i]);
	iterationTimeCPU_h_add[i]->GetXaxis()->SetBinLabel(bin_number,moduleName[j].c_str());
      }
      else if( (i == 7) && (((moduleName[j].find("conv") == 0) || (moduleName[j].find("photonConv") == 0)) && (moduleName[j].find("convserionTracks") != 0)) ){ //hard code convStep
	bin_number++;

	iterationTimeCPU[i] += moduleTimeCPU[j];
	
	iterationTimeCPU_h[i]->SetBinContent(bin_number,moduleTimeCPU[j]);
	iterationTimeCPU_h[i]->GetXaxis()->SetBinLabel(bin_number,moduleName[j].c_str());

	iterationTimeCPU_h_add[i]->SetBinContent(bin_number,iterationTimeCPU[i]);
	iterationTimeCPU_h_add[i]->GetXaxis()->SetBinLabel(bin_number,moduleName[j].c_str());
      }
    }
  }

  // Make Total Time Histo
  
  Double_t totalIterTimeCPU[nIterations+2];
  for(UInt_t i = 0; i < nIterations+2; i++){totalIterTimeCPU[i] = 0;}

  for(UInt_t i = 0; i < nIterations+2; i++){
    for(UInt_t ii = 0; ii <= i; ii++){
      totalIterTimeCPU[i] += iterationTimeCPU[ii];
    }
    
    if(i<nIterations){
      iterationTimeCPU_total_h->SetBinContent(i+1,iterationTimeCPU[i]);
      iterationTimeCPU_total_h->GetXaxis()->SetBinLabel(i+1,iterationName[i].c_str());
      
      iterationTimeCPU_total_h_add->SetBinContent(i+1,totalIterTimeCPU[i]);
      iterationTimeCPU_total_h_add->GetXaxis()->SetBinLabel(i+1,iterationName[i].c_str());
    }

    else if(i == nIterations){ // include general tracks
      for(UInt_t j = 0; j < moduleName.size(); j++){
	if(moduleName[j].find("generalTracks") == 0){
	  totalIterTimeCPU[i] += moduleTimeCPU[j];
	  
	  iterationTimeCPU_total_h->SetBinContent(i+1,moduleTimeCPU[j]);
	  iterationTimeCPU_total_h->GetXaxis()->SetBinLabel(i+1,moduleName[j].c_str());
	  
	  iterationTimeCPU_total_h_add->SetBinContent(i+1,totalIterTimeCPU[i]);
	  iterationTimeCPU_total_h_add->GetXaxis()->SetBinLabel(i+1,moduleName[j].c_str());
	}
      }
    }
    
    else if(i == nIterations+1){ // include conversionStepTracks
      for(UInt_t j = 0; j < moduleName.size(); j++){
	if(moduleName[j].find("generalTracks") == 0){
	  totalIterTimeCPU[i] += moduleTimeCPU[j];
	} // hack way of adding previous time...
	if(moduleName[j].find("conversionStepTracks") == 0){
	  totalIterTimeCPU[i] += moduleTimeCPU[j];

	  iterationTimeCPU_total_h->SetBinContent(i+1,moduleTimeCPU[j]);
	  iterationTimeCPU_total_h->GetXaxis()->SetBinLabel(i+1,moduleName[j].c_str());
	  
	  iterationTimeCPU_total_h_add->SetBinContent(i+1,totalIterTimeCPU[i]);
	  iterationTimeCPU_total_h_add->GetXaxis()->SetBinLabel(i+1,moduleName[j].c_str());
	}
      }
    }
  } // End loop for creating total plots

  // Draw graphs and save them
  // Write histos to root file
  
  TFile * timing_plots_CPU = new TFile(Form("%s/timing_plots_CPU.root",outDir.c_str()),"RECREATE");
  timing_plots_CPU->cd();

  for(UInt_t i = 0; i < nIterations; i++){
    canvasCPU[i]->cd();
    iterationTimeCPU_h[i]->Draw();
    iterationTimeCPU_h[i]->Write();
    canvasCPU[i]->Print(Form("%s/%sCPU.png",outDir.c_str(),iterationName[i].c_str()),"png");
    
    canvasCPU_add[i]->cd();
    iterationTimeCPU_h_add[i]->Draw();
    iterationTimeCPU_h_add[i]->Write();
    canvasCPU_add[i]->Print(Form("%s/%sCPU_add.png",outDir.c_str(),iterationName[i].c_str()),"png");
  }

  canvasCPU_total->cd();
  iterationTimeCPU_total_h->Draw();
  iterationTimeCPU_total_h->Write();
  canvasCPU_total->Print(Form("%s/iterationTotalCPU.png",outDir.c_str()),"png");

  canvasCPU_total_add->cd();
  iterationTimeCPU_total_h_add->Draw();
  iterationTimeCPU_total_h_add->Write();
  canvasCPU_total_add->Print(Form("%s/iterationTotalCPU_add.png",outDir.c_str()),"png");

  // Close the ROOT file
  timing_plots_CPU->Close();
}

void averageREALPlots(string inDir, string outDir){

  // Get average csvs

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

  // Push names of each module into a vector; will be the same for both REAL and REAL -- just use REAL

  std::vector<string>::iterator string_iter;
  std::vector<string> moduleName;

  for(string_iter = averagesREAL.begin() +1; string_iter != averagesREAL.end() +1; string_iter+=2){
    moduleName.push_back(*string_iter);
  }

  // Grab time of each module as a string, then push time of each module into a vector of Double_ts
  // Index will match the naming vector by construction
  
  std::vector<string> moduleTimeREAL_s;

  for(string_iter = averagesREAL.begin(); string_iter != averagesREAL.end(); string_iter+=2){
    moduleTimeREAL_s.push_back(*string_iter);
  }
  
  std::vector<Double_t> moduleTimeREAL;

  for(string_iter = moduleTimeREAL_s.begin(); string_iter != moduleTimeREAL_s.end(); string_iter++){
    moduleTimeREAL.push_back( atof((*string_iter).c_str()) );
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
  
  // Initialize Histos and Canvases

  TCanvas * canvasREAL[nIterations];
  for(UInt_t i = 0; i < nIterations; i++){
    canvasREAL[i] = new TCanvas();
    canvasREAL[i]->SetBottomMargin(0.17);
  }

  TCanvas * canvasREAL_add[nIterations];
  for(UInt_t i = 0; i < nIterations; i++){
    canvasREAL_add[i] = new TCanvas();
    canvasREAL_add[i]->SetBottomMargin(0.17);
  }

  TCanvas * canvasREAL_total = new TCanvas();
  canvasREAL_total->SetBottomMargin(0.17);

  TCanvas * canvasREAL_total_add = new TCanvas();
  canvasREAL_total_add->SetBottomMargin(0.17);

  TH1D * iterationTimeREAL_h[nIterations];
  for(UInt_t i = 0; i < nIterations; i++){
    iterationTimeREAL_h[i] = new TH1D(Form("%s_h_REAL_Time",iterationName[i].c_str()),Form("%s REAL Time",iterationName[i].c_str()),nModulesInIter[i],0,nModulesInIter[i]);
    iterationTimeREAL_h[i]->SetStats(kFALSE);
    iterationTimeREAL_h[i]->GetYaxis()->SetTitle("Average Time/Event (s)");
    iterationTimeREAL_h[i]->GetYaxis()->SetTitleOffset(1.3);
    iterationTimeREAL_h[i]->SetMinimum(0.0);
  }

  TH1D * iterationTimeREAL_h_add[nIterations];
  for(UInt_t i = 0; i < nIterations; i++){
    iterationTimeREAL_h_add[i] = new TH1D(Form("%s_h_REAL_Time_Add",iterationName[i].c_str()),Form("%s REAL Time Add",iterationName[i].c_str()),nModulesInIter[i],0,nModulesInIter[i]);
    iterationTimeREAL_h_add[i]->SetStats(kFALSE);
    iterationTimeREAL_h_add[i]->GetYaxis()->SetTitle("Average Time:Added/Event (s)");
    iterationTimeREAL_h_add[i]->GetYaxis()->SetTitleOffset(1.3);
    iterationTimeREAL_h_add[i]->SetMinimum(0.0);
  }

  TH1D * iterationTimeREAL_total_h = new TH1D("REAL_total","REAL Total Time for Each Iteration",nIterations+2,0,nIterations+2); // +2 to include gentracks and conversionStepTracks ... hardcode 
  iterationTimeREAL_total_h->SetStats(kFALSE);
  iterationTimeREAL_total_h->GetYaxis()->SetTitle("Average Time/Event (s)");
  iterationTimeREAL_total_h->GetYaxis()->SetTitleOffset(1.3);
  iterationTimeREAL_total_h->SetMinimum(0.0);

  TH1D * iterationTimeREAL_total_h_add = new TH1D("REAL_total_Add","REAL Total Time for Each Iteration Add",nIterations+2,0,nIterations+2); // +2 to include gentracks and conversionStepTracks ... hardcode 
  iterationTimeREAL_total_h_add->SetStats(kFALSE);
  iterationTimeREAL_total_h_add->GetYaxis()->SetTitle("Average Time:Added/Event (s)");
  iterationTimeREAL_total_h_add->GetYaxis()->SetTitleOffset(1.3);
  iterationTimeREAL_total_h_add->SetMinimum(0.0);

  // MakeGraphs that are filled with the steps and their names

  Double_t iterationTimeREAL[nIterations];  // keep track of total time in each iteration
  for(UInt_t i = 0; i < nIterations; i++){iterationTimeREAL[i] = 0;}

  UInt_t bin_number = 0;

  for(UInt_t i = 0; i < nIterations; i++){
    for(UInt_t j = 0; j < moduleName.size(); j++){
      if(j == 0){bin_number = 0;} // reset bin counter
      if( ((moduleName[j].compare(0,iterNameLength[i],iterationName[i].c_str()) == 0)) && (i != 7) ){ // hardcode skip convStep
	bin_number++;

	iterationTimeREAL[i] += moduleTimeREAL[j];
	
	iterationTimeREAL_h[i]->SetBinContent(bin_number,moduleTimeREAL[j]);
	iterationTimeREAL_h[i]->GetXaxis()->SetBinLabel(bin_number,moduleName[j].c_str());

	iterationTimeREAL_h_add[i]->SetBinContent(bin_number,iterationTimeREAL[i]);
	iterationTimeREAL_h_add[i]->GetXaxis()->SetBinLabel(bin_number,moduleName[j].c_str());
      }
      else if( (i == 7) && (((moduleName[j].find("conv") == 0) || (moduleName[j].find("photonConv") == 0)) && (moduleName[j].find("convserionTracks") != 0)) ){ //hard code convStep
	bin_number++;

	iterationTimeREAL[i] += moduleTimeREAL[j];
	
	iterationTimeREAL_h[i]->SetBinContent(bin_number,moduleTimeREAL[j]);
	iterationTimeREAL_h[i]->GetXaxis()->SetBinLabel(bin_number,moduleName[j].c_str());

	iterationTimeREAL_h_add[i]->SetBinContent(bin_number,iterationTimeREAL[i]);
	iterationTimeREAL_h_add[i]->GetXaxis()->SetBinLabel(bin_number,moduleName[j].c_str());
      }
    }
  }

  // Make Total Time Histo
  
  Double_t totalIterTimeREAL[nIterations+2];
  for(UInt_t i = 0; i < nIterations+2; i++){totalIterTimeREAL[i] = 0;}

  for(UInt_t i = 0; i < nIterations+2; i++){
    for(UInt_t ii = 0; ii <= i; ii++){
      totalIterTimeREAL[i] += iterationTimeREAL[ii];
    }
    
    if(i<nIterations){
      iterationTimeREAL_total_h->SetBinContent(i+1,iterationTimeREAL[i]);
      iterationTimeREAL_total_h->GetXaxis()->SetBinLabel(i+1,iterationName[i].c_str());
      
      iterationTimeREAL_total_h_add->SetBinContent(i+1,totalIterTimeREAL[i]);
      iterationTimeREAL_total_h_add->GetXaxis()->SetBinLabel(i+1,iterationName[i].c_str());
    }

    else if(i == nIterations){ // include general tracks
      for(UInt_t j = 0; j < moduleName.size(); j++){
	if(moduleName[j].find("generalTracks") == 0){
	  totalIterTimeREAL[i] += moduleTimeREAL[j];
	  
	  iterationTimeREAL_total_h->SetBinContent(i+1,moduleTimeREAL[j]);
	  iterationTimeREAL_total_h->GetXaxis()->SetBinLabel(i+1,moduleName[j].c_str());
	  
	  iterationTimeREAL_total_h_add->SetBinContent(i+1,totalIterTimeREAL[i]);
	  iterationTimeREAL_total_h_add->GetXaxis()->SetBinLabel(i+1,moduleName[j].c_str());
	}
      }
    }
    
    else if(i == nIterations+1){ // include conversionStepTracks
      for(UInt_t j = 0; j < moduleName.size(); j++){
	if(moduleName[j].find("generalTracks") == 0){
	  totalIterTimeREAL[i] += moduleTimeREAL[j];
	} // hack way of adding previous time...
	if(moduleName[j].find("conversionStepTracks") == 0){
	  totalIterTimeREAL[i] += moduleTimeREAL[j];

	  iterationTimeREAL_total_h->SetBinContent(i+1,moduleTimeREAL[j]);
	  iterationTimeREAL_total_h->GetXaxis()->SetBinLabel(i+1,moduleName[j].c_str());
	  
	  iterationTimeREAL_total_h_add->SetBinContent(i+1,totalIterTimeREAL[i]);
	  iterationTimeREAL_total_h_add->GetXaxis()->SetBinLabel(i+1,moduleName[j].c_str());
	}
      }
    }
  } // End loop for creating total plots

  // Draw graphs and save them
  // Write histos to root file

  TFile * timing_plots_REAL = new TFile(Form("%s/timing_plots_REAL.root",outDir.c_str()),"RECREATE");

  timing_plots_REAL->cd();

  for(UInt_t i = 0; i < nIterations; i++){
    canvasREAL[i]->cd();
    iterationTimeREAL_h[i]->Draw();
    iterationTimeREAL_h[i]->Write();
    canvasREAL[i]->Print(Form("%s/%sREAL.png",outDir.c_str(),iterationName[i].c_str()),"png");
    
    canvasREAL_add[i]->cd();
    iterationTimeREAL_h_add[i]->Draw();
    iterationTimeREAL_h_add[i]->Write();
    canvasREAL_add[i]->Print(Form("%s/%sREAL_add.png",outDir.c_str(),iterationName[i].c_str()),"png");
  }

  canvasREAL_total->cd();
  iterationTimeREAL_total_h->Draw();
  iterationTimeREAL_total_h->Write();
  canvasREAL_total->Print(Form("%s/iterationTotalREAL.png",outDir.c_str()),"png");

  canvasREAL_total_add->cd();
  iterationTimeREAL_total_h_add->Draw();
  iterationTimeREAL_total_h_add->Write();
  canvasREAL_total_add->Print(Form("%s/iterationTotalREAL_add.png",outDir.c_str()),"png");

  // Close the ROOT file
  timing_plots_REAL->Close();
}

void compareCPUtoREAL(std::string inDir, std::string outDir){

  // Initialize Variables for grabbing old histos, creating new histos

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

  // Push names of each module into a vector; will be the same for both REAL and REAL -- just use REAL

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

  // Get the Histos
  
  TFile * timing_plots_REAL = new TFile(Form("%s/timing_plots_REAL.root",outDir.c_str()));
  TFile * timing_plots_CPU = new TFile(Form("%s/timing_plots_CPU.root",outDir.c_str()));

  TH1D * iterationTimeCPU_h[nIterations];
  TH1D * iterationTimeCPU_h_add[nIterations];
  TH1D * iterationTimeREAL_h[nIterations];
  TH1D * iterationTimeREAL_h_add[nIterations];

  for(UInt_t i = 0; i < nIterations; i++){
    iterationTimeCPU_h[i] = (TH1D*)timing_plots_CPU->Get(Form("%s_h_CPU_Time",iterationName[i].c_str()));
    iterationTimeCPU_h_add[i] = (TH1D*)timing_plots_CPU->Get(Form("%s_h_CPU_Time_Add",iterationName[i].c_str()));
    iterationTimeREAL_h[i] = (TH1D*)timing_plots_REAL->Get(Form("%s_h_REAL_Time",iterationName[i].c_str()));
    iterationTimeREAL_h_add[i] = (TH1D*)timing_plots_REAL->Get(Form("%s_h_REAL_Time_Add",iterationName[i].c_str()));
  }

  TH1D * iterationTimeCPU_total_h = (TH1D*)timing_plots_CPU->Get("CPU_total");
  TH1D * iterationTimeCPU_total_h_add = (TH1D*)timing_plots_CPU->Get("CPU_total_Add");
  TH1D * iterationTimeREAL_total_h = (TH1D*)timing_plots_REAL->Get("REAL_total");
  TH1D * iterationTimeREAL_total_h_add = (TH1D*)timing_plots_REAL->Get("REAL_total_Add");
    
  // Initialize Comparision Plots

  TCanvas * canvasCompare[nIterations];
  for(UInt_t i = 0; i < nIterations; i++){
    canvasCompare[i] = new TCanvas();
    canvasCompare[i]->SetBottomMargin(0.17);
  }

  TCanvas * canvasCompare_add[nIterations];
  for(UInt_t i = 0; i < nIterations; i++){
    canvasCompare_add[i] = new TCanvas();
    canvasCompare_add[i]->SetBottomMargin(0.17);
  }

  TCanvas * canvasCompare_total = new TCanvas();
  canvasCompare_total->SetBottomMargin(0.17);

  TCanvas * canvasCompare_total_add = new TCanvas();
  canvasCompare_total_add->SetBottomMargin(0.17);

  TH1D * iterationTimeCompare_h[nIterations];
  for(UInt_t i = 0; i < nIterations; i++){
    iterationTimeCompare_h[i] = new TH1D(Form("%s_h_Compare_Time",iterationName[i].c_str()),Form("%s Compare CPU/REAL Time",iterationName[i].c_str()),nModulesInIter[i],0,nModulesInIter[i]);
    iterationTimeCompare_h[i]->SetStats(kFALSE);
    iterationTimeCompare_h[i]->GetYaxis()->SetTitle("Ratio of Average Time/Event (s)");
    iterationTimeCompare_h[i]->GetYaxis()->SetTitleOffset(1.3);
    iterationTimeCompare_h[i]->SetMinimum(0.0);
  }

  TH1D * iterationTimeCompare_h_add[nIterations];
  for(UInt_t i = 0; i < nIterations; i++){
    iterationTimeCompare_h_add[i] = new TH1D(Form("%s_h_Compare_Time_Add",iterationName[i].c_str()),Form("%s Compare CPU/REAL Time Add",iterationName[i].c_str()),nModulesInIter[i],0,nModulesInIter[i]);
    iterationTimeCompare_h_add[i]->SetStats(kFALSE);
    iterationTimeCompare_h_add[i]->GetYaxis()->SetTitle("Ratio of Average Time:Added/Event (s)");
    iterationTimeCompare_h_add[i]->GetYaxis()->SetTitleOffset(1.3);
    iterationTimeCompare_h_add[i]->SetMinimum(0.0);
  }

  TH1D * iterationTimeCompare_total_h = new TH1D("Compare_total","Compare CPU/REAL Total Time for Each Iteration",nIterations+2,0,nIterations+2); // +2 to include gentracks and conversionStepTracks ... hardcode 
  iterationTimeCompare_total_h->SetStats(kFALSE);
  iterationTimeCompare_total_h->GetYaxis()->SetTitle("Ratio of Average Time/Event (s)");
  iterationTimeCompare_total_h->GetYaxis()->SetTitleOffset(1.3);
  iterationTimeCompare_total_h->SetMinimum(0.0);

  TH1D * iterationTimeCompare_total_h_add = new TH1D("Compare_total_add","Compare CPU/REAL Total Time for Each Iteration Add",nIterations+2,0,nIterations+2); // +2 to include gentracks and conversionStepTracks ... hardcode 
  iterationTimeCompare_total_h_add->SetStats(kFALSE);
  iterationTimeCompare_total_h_add->GetYaxis()->SetTitle("Ratio of Average Time:Added/Event (s)");
  iterationTimeCompare_total_h_add->GetYaxis()->SetTitleOffset(1.3);
  iterationTimeCompare_total_h_add->SetMinimum(0.0);

  // Fill the Histos

  UInt_t bin_number = 0;

  for(UInt_t i = 0; i < nIterations; i++){
    iterationTimeCompare_h[i]->Divide(iterationTimeCPU_h[i],iterationTimeREAL_h[i]);
    iterationTimeCompare_h_add[i]->Divide(iterationTimeCPU_h_add[i],iterationTimeREAL_h_add[i]);
    
    for(UInt_t j = 0; j < moduleName.size(); j++){
      if(j == 0){bin_number = 0;} // reset bin counter
      if( ((moduleName[j].compare(0,iterNameLength[i],iterationName[i].c_str()) == 0)) && (i != 7) ){ // hardcode skip convStep
	bin_number++;

	iterationTimeCompare_h[i]->GetXaxis()->SetBinLabel(bin_number,moduleName[j].c_str());
	iterationTimeCompare_h_add[i]->GetXaxis()->SetBinLabel(bin_number,moduleName[j].c_str());
      }
      else if( (i == 7) && (((moduleName[j].find("conv") == 0) || (moduleName[j].find("photonConv") == 0)) && (moduleName[j].find("convserionTracks") != 0)) ){ //hard code convStep
	bin_number++;

	iterationTimeCompare_h[i]->GetXaxis()->SetBinLabel(bin_number,moduleName[j].c_str());
	iterationTimeCompare_h_add[i]->GetXaxis()->SetBinLabel(bin_number,moduleName[j].c_str());
      }
    }
  }

  // Make Total Time Histo

  iterationTimeCompare_total_h->Divide(iterationTimeCPU_total_h,iterationTimeREAL_total_h);
  iterationTimeCompare_total_h_add->Divide(iterationTimeCPU_total_h_add,iterationTimeREAL_total_h_add);
  
  for(UInt_t i = 0; i < nIterations+2; i++){
    if(i < nIterations){      
      iterationTimeCompare_total_h->GetXaxis()->SetBinLabel(i+1,iterationName[i].c_str());
      iterationTimeCompare_total_h_add->GetXaxis()->SetBinLabel(i+1,iterationName[i].c_str());
    }
    else if(i == nIterations){ // include general tracks    
      iterationTimeCompare_total_h->GetXaxis()->SetBinLabel(i+1,"generalTracks");
      iterationTimeCompare_total_h_add->GetXaxis()->SetBinLabel(i+1,"generalTracks");
    }
    else if(i == nIterations+1){ // include conversionStepTracks
      iterationTimeCompare_total_h->GetXaxis()->SetBinLabel(i+1,"conversionStepTracks");
      iterationTimeCompare_total_h_add->GetXaxis()->SetBinLabel(i+1,"conversionStepTracks");
    }
  }

  // Draw graphs and save them
  // Write histos to root file

  TFile * compare_CPU_to_REAL = new TFile(Form("%s/compareCPUtoREAL.root",outDir.c_str()),"RECREATE");

  for(UInt_t i = 0; i < nIterations; i++){
    canvasCompare[i]->cd();
    iterationTimeCompare_h[i]->Draw();
    iterationTimeCompare_h[i]->Write();

    compare_CPU_to_REAL->cd();
    canvasCompare[i]->Print(Form("%s/%sCompare.png",outDir.c_str(),iterationName[i].c_str()),"png");
    
    canvasCompare_add[i]->cd();
    iterationTimeCompare_h_add[i]->Draw();
    iterationTimeCompare_h_add[i]->Write();

    compare_CPU_to_REAL->cd();
    canvasCompare_add[i]->Print(Form("%s/%sCompare_add.png",outDir.c_str(),iterationName[i].c_str()),"png");
  }
  
  canvasCompare_total->cd();
  iterationTimeCompare_total_h->Draw();
  iterationTimeCompare_total_h->Write();
  
  compare_CPU_to_REAL->cd();
  canvasCompare_total->Print(Form("%s/iterationTotalCompare.png",outDir.c_str()),"png");
  
  canvasCompare_total_add->cd();
  iterationTimeCompare_total_h_add->Draw();
  iterationTimeCompare_total_h_add->Write();
  
  compare_CPU_to_REAL->cd();
  canvasCompare_total_add->Print(Form("%s/iterationTotalCompare_add.png",outDir.c_str()),"png");
    
}
