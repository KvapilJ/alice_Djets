//-----------------------------------------------------------------------
//  Author B.Trzeciak
//  Utrecht University
//  barbara.antonina.trzeciak@cern.ch
//-----------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include "TObject.h"
#include "TMath.h"
#include "TFile.h"
#include "TDirectoryFile.h"
#include "TList.h"
#include "TCanvas.h"
#include "TPaveText.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TSystem.h"
#include "TRandom3.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TF1.h"
#include "THnSparse.h"
#include "TDatabasePDG.h"
#include "AliNormalizationCounter.h"
#include "TStyle.h"
#include "AliHFInvMassFitter.h"
#include "TProfile.h"
#include "AliAnalysisTaskDmesonJets.h"
#include "THStack.h"
#include "TGraphAsymmErrors.h"
#include "TLegendEntry.h"
#include "TPad.h"
#include "../Preliminaryplots/FD/style.C"

//Double_t fRpar = 0.4;
//double jetEta = 0.9 - fRpar;
//double dy = 2*jetEta;
//Bool_t pdf = false;

static Double_t dy;
static Bool_t pdf;
static Int_t *jetpTbins;
static Int_t *DpTbins[2];
static Int_t zBin;
static Double_t plotRanges[4]{0,2,10e-9,1};


//Color_t colors[] = {1,8,4,2,6,kOrange-1,kGray+2,kCyan+1,kMagenta+2,kViolet+5,kYellow+2};
//Int_t markers[] = {20,21,22,23,24,25,26,27,28,29,30,32,33,34};
//Style_t linestyle[] = {1,2,3,4,5,6,7,8,9,10,11,12,13};

void ScaleHist(TH1 *hh, int full = 0);
void setHistoDetails(TH1 *hh, Color_t color, Style_t Mstyle, Size_t Msize = 1.1f, Width_t Lwidth = 2, Style_t Lstyle = 1);
void SaveCanvas(TCanvas *c, TString name = "tmp");
void getSystematics(TString inDir, TString outPlotDir);
TH1D* GetUpSys(TH1D *central, TH1D **hh, Int_t nFiles = 0);
TH1D* GetDownSys(TH1D *central, TH1D **hh, Int_t nFiles = 0);
TH1* GetInputHist(TString inFile, TString histName);
TGraphAsymmErrors* GetInputGraph(TString inFile, TString histName);
void ScaleHist(TH1 *hh, int full);
void drawFinal(TString outPlotDir);
TH1D* GetRatio(TH1D *h1, TH1D *h2);
TH1D* GetRatio(TH1D *h1, TH1D *h2, Double_t shift);


static TString fFiles[9] = {
    "/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_X02/Default_AnalysisResults_Run2w18b.root/unfolding_Bayes_5/finalSpectraNEW/JetPtSpectrum_final_fullGlobal.root",
    "/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_X04/Default_AnalysisResults_Run2w18b.root/unfolding_Bayes_5/finalSpectraNEW/JetPtSpectrum_final_fullGlobal.root",
    "/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_X06/Default_AnalysisResults_Run2w18b.root/unfolding_Bayes_5/finalSpectraNEW/JetPtSpectrum_final_fullGlobal.root",
    };

static TString fFiles2[9] = {
    "/home/kvapil/Desktop/auro/Jakub_APW2020_globaljetpt/JetPtSpectrum_final_fullGlobal_R02.root",
    "/home/kvapil/Desktop/auro/Jakub_APW2020_globaljetpt/JetPtSpectrum_final_fullGlobal_R04.root",
    "/home/kvapil/Desktop/auro/Jakub_APW2020_globaljetpt/JetPtSpectrum_final_fullGlobal_R06.root",
    };

/*
static TString fFiles[9] = {
    "/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_Z02/Default_AnalysisResults_Run2w18b.root/unfolding_2D_5/finalSpectraNEW25/JetPtSpectrum_final.root",
    "/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_04zF/Default_AnalysisResults_Run2w18b.root/unfolding_2D_5/finalSpectraNEW25/JetPtSpectrum_final.root",
    "/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_Z06/Default_AnalysisResults_Run2w18b.root/unfolding_2D_5/finalSpectraNEW25/JetPtSpectrum_final.root",
    };
*/

std::tuple<TGraphAsymmErrors*, TGraphAsymmErrors*, TH1D**> GetSim(TString dataFile,TString simname, Int_t nVar);
std::tuple<TGraphAsymmErrors*, TH1D*, TH1D*, TH1D*> GetDataSimRatio(TString simname, TH1D *data_cent, TH1D *sim_cent, TH1D *sim_up, TH1D *sim_down, UInt_t nBins, Double_t *xBins);
std::tuple<TGraphAsymmErrors*, TH1D*,TGraphAsymmErrors*, TH1D*> GetData(TString dataFile, TString histBase, Double_t dataScaling, UInt_t nBins, Double_t *xBins, Double_t *systUncD_down, Double_t *systUncD_up);
std::tuple<TCanvas*, TPad*, TPad*,  TPad*, TH1D*, TH1D*, TH1D*> PrepareCanvas(UInt_t xAxisBins, Double_t *xAxis);
void PlaceOnPadData(TPad* pad,TGraphAsymmErrors* histo1, TH1D* histo2, Size_t markersize,Style_t markerstyle,Double_t scale);
void PlaceOnPad(TPad* pad, TGraphAsymmErrors* histo, Color_t ci, Style_t style, Style_t linestyle, TString opt,Double_t scale);
void PlaceOnPad(TPad* pad, TH1D* histo, Color_t ci, Style_t style, Style_t linestyle, TString opt,Double_t scale);
void TerminateCanvas(TPad* pad1,TPad* pad2,TPad* pad3,TH1D* histo1,TH1D* histo2,TH1D* histo3);
std::tuple<TCanvas*, TPad**, TH1D**> PrepareCanvas8Pad(UInt_t xAxisBins, Double_t *xAxisl, Double_t *xAxisr);
std::tuple<TCanvas*, TPad**, TH1D**> PrepareCanvas6Pad(UInt_t xAxisBins, Double_t *xAxisl, Double_t *xAxisr);
std::tuple<TCanvas*, TPad**, TH1D**> PrepareCanvas4Pad(UInt_t xAxisBins, Double_t *xAxis);
void TerminateCanvas8Pad(TPad **pad, TH1D** histo);

std::tuple<TH1D*, TH1D*, TH1D*> GetRatio(TH1D *sim_cent, TH1D *sim_up, TH1D *sim_down,TH1D *sim2_cent, TH1D *sim2_up, TH1D *sim2_down);
std::tuple<TH1D*, TGraphAsymmErrors*, TH1D*,  TGraphAsymmErrors*> GetData(TString dataFile, TString histBase1, TString histBase2, TString histBase3, TString histBase4);

std::tuple<TH1D*, TGraphAsymmErrors*> CalculateDataRRatio(TH1D *stat1,TGraphAsymmErrors *sys1, TH1D *stat2,TGraphAsymmErrors *sys2,Double_t shift);
TGraphAsymmErrors* CalculateSimRRatio(TGraphAsymmErrors *hist1,TH1D **var1,TGraphAsymmErrors *hist2,TH1D **var2, UInt_t vars);
TGraphAsymmErrors* Divide(TGraphAsymmErrors *hist1,TH1D *hist2);

void finalJetSpectraInvUpdatedRatio(
)

{

    // ----------------------------------------------------------------
    // ------------------- ENABLE SIMULATIONS HERE --------------------
    // ----------------------------------------------------------------
    Bool_t ePowhegPythia6 = false;
    Bool_t ePowhegPythia8 = true;
    Bool_t ePythia6 = false;
    Bool_t ePythia8 = false;
    Bool_t ePythia8SoftMode2 = false;
    Bool_t ePowhegPythia6dijet = false;
    Bool_t ePowhegPythia8dijet = false;

    // ----------------------------------------------------------------

    UInt_t xAxisBins = 8;
     Double_t *xAxis = new Double_t[xAxisBins+1]{5,6,8,10,12,14,20,30,50};
    Double_t *xAxisl = new Double_t[xAxisBins+1]{5,6,8,10,12,14,20,30,49.99};
    Double_t *xAxisr = new Double_t[xAxisBins+1]{5,6,8,10,12,14,20,30,50};
  //  UInt_t xAxisBins = 5;
   // Double_t *xAxis = new Double_t[xAxisBins+1]{0.4,0.6,0.7,0.8,0.9,1.0};
   // plotRanges[0]=0; plotRanges[1]=2.1; plotRanges[2]=0.1; plotRanges[3]=3.5;
     plotRanges[0]=0.4; plotRanges[1]=1.6; plotRanges[2]=0.0; plotRanges[3]=5;


    // ----------------- Initiate Canvas ---------------------
   /* TCanvas *canvas;
    TPad *upPad, *dowmPad, *dowmPad2;
    TH1D *placeholder_up, *placeholder_down, *placeholder_down2;
    std::tie(canvas,upPad,dowmPad,dowmPad2,placeholder_up,placeholder_down,placeholder_down2) = PrepareCanvas(xAxisBins,xAxis);*/

    TCanvas *canvas;
    TPad **Pad;
    TH1D **placeholder;
    std::tie(canvas,Pad,placeholder) = PrepareCanvas8Pad(xAxisBins,xAxisl, xAxisr);

    // ----------------- data ---------------------
  //  TGraphAsymmErrors *hDataSys, *hDataSysRatio;
 //   TH1D *hData_binned2,*hData_binned4,*hData_binned6;
 //   std::tie(hDataSys, hData_binned, hDataSysRatio, hData_binned_ratio) = GetData(dataFile, histBase, dataScaling, xAxisBins, xAxis, systUncD_down, systUncD_up);
 /*  hData_binned2 = dynamic_cast<TH1D*>(GetInputHist(fFiles[0],"hData_binned"));
   hData_binned4 = dynamic_cast<TH1D*>(GetInputHist(fFiles[1],"hData_binned"));
   hData_binned6 = dynamic_cast<TH1D*>(GetInputHist(fFiles[2],"hData_binned"));

   TH1D *hDataR26 = dynamic_cast<TH1D*>(hData_binned2->Clone("hDataR26"));
   hDataR26->Divide(hData_binned6);
   hDataR26->SetLineColor(static_cast<Color_t>(TColor::GetColor("#990000")));
   hDataR26->SetMarkerColor(static_cast<Color_t>(TColor::GetColor("#990000")));
   TH1D *hDataR24 = dynamic_cast<TH1D*>(hData_binned2->Clone("hDataR24"));
   hDataR24->Divide(hData_binned4);
   hDataR24->SetLineColor(static_cast<Color_t>(TColor::GetColor("#999900")));
   hDataR24->SetMarkerColor(static_cast<Color_t>(TColor::GetColor("#999900")));
   hDataR24->SetMarkerStyle(22);*/

//   TH1D *hData_binned2R,*hData_binned4R,*hData_binned6R;
    Int_t markers[3]{20,21,47};
    Double_t plotScale[3]{0.05,-1,20};

   TH1D *s13_hData_binned[3],*s13_hData_binned_ratio[3];
   TGraphAsymmErrors *s13_hData_binned_syst[3],*s13_hData_binned_ratio_syst[3];
   for(Int_t iradius = 0; iradius < 3;iradius++){
       std::tie(s13_hData_binned[iradius], s13_hData_binned_syst[iradius], s13_hData_binned_ratio[iradius], s13_hData_binned_ratio_syst[iradius])
               =GetData(fFiles[iradius],"hData_binned","haeData_binned_syst","hData_binned_ratio","haeData_binned_syst_ratio");
       PlaceOnPadData(Pad[0],s13_hData_binned_syst[iradius],s13_hData_binned[iradius],0.9,markers[iradius],plotScale[iradius]);
       PlaceOnPadData(Pad[2*(iradius+1)],s13_hData_binned_ratio_syst[iradius],s13_hData_binned_ratio[iradius],0,markers[iradius],-1);
   }

   TH1D *s5_hData_binned[3],*s5_hData_binned_ratio[3];
   TGraphAsymmErrors *s5_hData_binned_syst[3],*s5_hData_binned_ratio_syst[3];
   for(Int_t iradius = 0; iradius < 3;iradius++){
       std::tie(s5_hData_binned[iradius], s5_hData_binned_syst[iradius], s5_hData_binned_ratio[iradius], s5_hData_binned_ratio_syst[iradius])
               =GetData(fFiles2[iradius],"hData_binned","haeData_binned_syst","hData_binned_ratio","haeData_binned_syst_ratio");
       PlaceOnPadData(Pad[1],s5_hData_binned_syst[iradius],s5_hData_binned[iradius],0.9,markers[iradius],plotScale[iradius]);
       PlaceOnPadData(Pad[2*(iradius+1)+1],s5_hData_binned_ratio_syst[iradius],s5_hData_binned_ratio[iradius],0,markers[iradius],-1);
   }



   TGraphAsymmErrors *s13_PowhegPythia8[3],*s13_PowhegPythia8_ratio[3];
   TH1D **s13_PowhegPythia8_var[3];
   for(Int_t iradius = 0; iradius < 3;iradius++){
       std::tie(s13_PowhegPythia8[iradius], s13_PowhegPythia8_ratio[iradius], s13_PowhegPythia8_var[iradius])
               =GetSim(fFiles[iradius],"PowhegPythia8",8);
       PlaceOnPad(Pad[0],s13_PowhegPythia8[iradius],-1,-1,-1,"same2p",plotScale[iradius]);
       PlaceOnPad(Pad[2*(iradius+1)],s13_PowhegPythia8_ratio[iradius],-1,-1,-1,"same2p",-1);
   }

   TGraphAsymmErrors *s5_PowhegPythia8[3],*s5_PowhegPythia8_ratio[3];
   TH1D **s5_PowhegPythia8_var[3];
   for(Int_t iradius = 0; iradius < 3;iradius++){
       std::tie(s5_PowhegPythia8[iradius], s5_PowhegPythia8_ratio[iradius], s5_PowhegPythia8_var[iradius])
               =GetSim(fFiles2[iradius],"PowhegPythia8",8);
       PlaceOnPad(Pad[1],s5_PowhegPythia8[iradius],-1,-1,-1,"same2p",plotScale[iradius]);
       PlaceOnPad(Pad[2*(iradius+1)+1],s5_PowhegPythia8_ratio[iradius],-1,-1,-1,"same2p",-1);
   }

   TGraphAsymmErrors *s13_Pythia8[3],*s13_Pythia8_ratio[3];
   TH1D **s13_Pythia8_var[3];
   for(Int_t iradius = 0; iradius < 3;iradius++){
       std::tie(s13_Pythia8[iradius], s13_Pythia8_ratio[iradius], s13_Pythia8_var[iradius])
               =GetSim(fFiles[iradius],"Pythia8",1);
       PlaceOnPad(Pad[0],s13_Pythia8[iradius],kViolet+2,-1,-1,"same1",plotScale[iradius]);
       PlaceOnPad(Pad[2*(iradius+1)],s13_Pythia8_ratio[iradius],kViolet+2,-1,-1,"same1",-1);
   }

   TGraphAsymmErrors *s5_Pythia8[3],*s5_Pythia8_ratio[3];
   TH1D **s5_Pythia8_var[3];
   for(Int_t iradius = 0; iradius < 3;iradius++){
       std::tie(s5_Pythia8[iradius], s5_Pythia8_ratio[iradius], s5_Pythia8_var[iradius])
               =GetSim(fFiles2[iradius],"Pythia8",1);
       PlaceOnPad(Pad[1],s5_Pythia8[iradius],kViolet+2,-1,-1,"same1",plotScale[iradius]);
       PlaceOnPad(Pad[2*(iradius+1)+1],s5_Pythia8_ratio[iradius],kViolet+2,-1,-1,"same1",-1);
   }
std::cout<<"b"<<std::endl;
   TGraphAsymmErrors *s13_Pythia6[3],*s13_Pythia6_ratio[3];
   TH1D **s13_Pythia6_var[3];
   for(Int_t iradius = 0; iradius < 3;iradius++){
       std::tie(s13_Pythia6[iradius], s13_Pythia6_ratio[iradius], s13_Pythia6_var[iradius])
               =GetSim(fFiles[iradius],"Pythia8Soft2",1);
       PlaceOnPad(Pad[0],s13_Pythia6[iradius],-1,-1,-1,"same1",plotScale[iradius]);
       PlaceOnPad(Pad[2*(iradius+1)],s13_Pythia6_ratio[iradius],-1,-1,-1,"same1",-1);
   }

   TGraphAsymmErrors *s5_Pythia6[3],*s5_Pythia6_ratio[3];
   TH1D **s5_Pythia6_var[3];
   for(Int_t iradius = 0; iradius < 3;iradius++){
       std::tie(s5_Pythia6[iradius], s5_Pythia6_ratio[iradius], s5_Pythia6_var[iradius])
               =GetSim(fFiles2[iradius],"Pythia8Soft2",1);
       PlaceOnPad(Pad[1],s5_Pythia6[iradius],-1,-1,-1,"same1",plotScale[iradius]);
       PlaceOnPad(Pad[2*(iradius+1)+1],s5_Pythia6_ratio[iradius],-1,-1,-1,"same1",-1);
   }
std::cout<<"b"<<std::endl;
   //*************************************************************************************************
   //******************************************DATA***************************************************
   //*************************************************************************************************

   TCanvas *canvas2;
   TPad **Pad2;
   TH1D **placeholder2;
   std::tie(canvas2,Pad2,placeholder2) = PrepareCanvas6Pad(xAxisBins,xAxisl, xAxisr);

   TH1D *s13_hData_binned26,*s13_hData_binned24,*s13_hData_binned26_ratio,*s13_hData_binned24_ratio;
   TGraphAsymmErrors *s13_hData_binned_syst26,*s13_hData_binned_syst24,*s13_hData_binned_syst26_ratio,*s13_hData_binned_syst24_ratio;
   std::tie(s13_hData_binned26, s13_hData_binned_syst26) = CalculateDataRRatio(s13_hData_binned[0],s13_hData_binned_syst[0],s13_hData_binned[2],s13_hData_binned_syst[2],0.5);
   std::tie(s13_hData_binned24, s13_hData_binned_syst24) = CalculateDataRRatio(s13_hData_binned[0],s13_hData_binned_syst[0],s13_hData_binned[1],s13_hData_binned_syst[1],-999);
   std::tie(s13_hData_binned26_ratio, s13_hData_binned_syst26_ratio) = CalculateDataRRatio(s13_hData_binned_ratio[0],s13_hData_binned_ratio_syst[0],s13_hData_binned_ratio[2],s13_hData_binned_ratio_syst[2],0.5);
   std::tie(s13_hData_binned24_ratio, s13_hData_binned_syst24_ratio) = CalculateDataRRatio(s13_hData_binned_ratio[0],s13_hData_binned_ratio_syst[0],s13_hData_binned_ratio[1],s13_hData_binned_ratio_syst[1],-999);

  /* for(Int_t i = 1 ; i< s13_hData_binned26->GetNbinsX();i++){
       s13_hData_binned26->SetBinContent(i,s13_hData_binned26->GetBinContent(i)+0.5);
   }*/
  /*
   for(Int_t i = 1 ; i< s13_hData_binned_syst26->GetX();i++){
       s13_hData_binned26->SetBinContent(i,s13_hData_binned26->GetBinContent(i)+0.5);
   }*/

   TH1D *s5_hData_binned26,*s5_hData_binned24,*s5_hData_binned26_ratio,*s5_hData_binned24_ratio;
     TGraphAsymmErrors *s5_hData_binned_syst26,*s5_hData_binned_syst24,*s5_hData_binned_syst26_ratio,*s5_hData_binned_syst24_ratio;
     std::tie(s5_hData_binned26, s5_hData_binned_syst26) = CalculateDataRRatio(s5_hData_binned[0],s5_hData_binned_syst[0],s5_hData_binned[2],s5_hData_binned_syst[2],0.5);
     std::tie(s5_hData_binned24, s5_hData_binned_syst24) = CalculateDataRRatio(s5_hData_binned[0],s5_hData_binned_syst[0],s5_hData_binned[1],s5_hData_binned_syst[1],-999);
     std::tie(s5_hData_binned26_ratio, s5_hData_binned_syst26_ratio) = CalculateDataRRatio(s5_hData_binned_ratio[0],s5_hData_binned_ratio_syst[0],s5_hData_binned_ratio[2],s5_hData_binned_ratio_syst[2],0.5);
     std::tie(s5_hData_binned24_ratio, s5_hData_binned_syst24_ratio) = CalculateDataRRatio(s5_hData_binned_ratio[0],s5_hData_binned_ratio_syst[0],s5_hData_binned_ratio[1],s5_hData_binned_ratio_syst[1],-999);

      PlaceOnPadData(Pad2[0],s13_hData_binned_syst26,s13_hData_binned26,0.9,21,-1);
      PlaceOnPadData(Pad2[0],s13_hData_binned_syst24,s13_hData_binned24,0.9,20,0.3);
      PlaceOnPadData(Pad2[1],s5_hData_binned_syst26,s5_hData_binned26,0.9,21,-1);
      PlaceOnPadData(Pad2[1],s5_hData_binned_syst24,s5_hData_binned24,0.9,20,0.3);

      PlaceOnPadData(Pad2[4],s13_hData_binned_syst24_ratio,s13_hData_binned24_ratio,0,20,-1);
      PlaceOnPadData(Pad2[3],s5_hData_binned_syst26_ratio,s5_hData_binned26_ratio,0,21,-1);
      PlaceOnPadData(Pad2[5],s5_hData_binned_syst24_ratio,s5_hData_binned24_ratio,0,20,-1);
      PlaceOnPadData(Pad2[2],s13_hData_binned_syst26_ratio,s13_hData_binned26_ratio,0,21,-1);


      //******************************************POWHEG+PYTHIA8***************************************************

    TGraphAsymmErrors *s13_PowhegPythia824,*s13_PowhegPythia826;
    TGraphAsymmErrors *s13_PowhegPythia824_ratio,*s13_PowhegPythia826_ratio;

    s13_PowhegPythia824 = CalculateSimRRatio(s13_PowhegPythia8[0], s13_PowhegPythia8_var[0],s13_PowhegPythia8[1],s13_PowhegPythia8_var[1], 8);
    s13_PowhegPythia824_ratio = Divide(s13_PowhegPythia824,s13_hData_binned24);
    s13_PowhegPythia826 = CalculateSimRRatio(s13_PowhegPythia8[0], s13_PowhegPythia8_var[0],s13_PowhegPythia8[2],s13_PowhegPythia8_var[2], 8);
    s13_PowhegPythia826_ratio = Divide(s13_PowhegPythia826,s13_hData_binned26);

    PlaceOnPad(Pad2[0],s13_PowhegPythia824,kBlue+2,24,-1,"same2p",0.3);
    PlaceOnPad(Pad2[0],s13_PowhegPythia826,kBlue+2,24,-1,"same2p",-1);
    PlaceOnPad(Pad2[2],s13_PowhegPythia824_ratio,kBlue+2,24,-1,"same2p",-1);
    PlaceOnPad(Pad2[4],s13_PowhegPythia826_ratio,kBlue+2,24,-1,"same2p",-1);

    TGraphAsymmErrors *s5_PowhegPythia824,*s5_PowhegPythia826;
    TGraphAsymmErrors *s5_PowhegPythia824_ratio,*s5_PowhegPythia826_ratio;

    s5_PowhegPythia824 = CalculateSimRRatio(s5_PowhegPythia8[0], s5_PowhegPythia8_var[0],s5_PowhegPythia8[1],s5_PowhegPythia8_var[1], 8);
    s5_PowhegPythia824_ratio = Divide(s5_PowhegPythia824,s5_hData_binned24);
    s5_PowhegPythia826 = CalculateSimRRatio(s5_PowhegPythia8[0], s5_PowhegPythia8_var[0],s5_PowhegPythia8[2],s5_PowhegPythia8_var[2], 8);
    s5_PowhegPythia826_ratio = Divide(s5_PowhegPythia826,s5_hData_binned26);

    PlaceOnPad(Pad2[1],s5_PowhegPythia824,kBlue+2,24,-1,"same2p",0.3);
    PlaceOnPad(Pad2[1],s5_PowhegPythia826,kBlue+2,24,-1,"same2p",-1);
    PlaceOnPad(Pad2[3],s5_PowhegPythia824_ratio,kBlue+2,24,-1,"same2p",-1);
    PlaceOnPad(Pad2[5],s5_PowhegPythia826_ratio,kBlue+2,24,-1,"same2p",-1);

    //******************************************PYTHIA8***************************************************

    TGraphAsymmErrors *s13_Pythia824,*s13_Pythia826;
    TGraphAsymmErrors *s13_Pythia824_ratio,*s13_Pythia826_ratio;

    s13_Pythia824 = CalculateSimRRatio(s13_Pythia8[0], nullptr,s13_Pythia8[1],nullptr, 0);
    s13_Pythia824_ratio = Divide(s13_Pythia824,s13_hData_binned24);
    s13_Pythia826 = CalculateSimRRatio(s13_Pythia8[0], nullptr,s13_Pythia8[2],nullptr, 0);
    s13_Pythia826_ratio = Divide(s13_Pythia826,s13_hData_binned26);

    PlaceOnPad(Pad2[0],s13_Pythia824,kViolet+2,-1,3,"same1",0.3);
    PlaceOnPad(Pad2[0],s13_Pythia826,kViolet+2,-1,3,"same1",-1);
    PlaceOnPad(Pad2[2],s13_Pythia824_ratio,kViolet+2,-1,3,"same1",-1);
    PlaceOnPad(Pad2[4],s13_Pythia826_ratio,kViolet+2,-1,3,"same1",-1);

    TGraphAsymmErrors *s5_Pythia824,*s5_Pythia826;
    TGraphAsymmErrors *s5_Pythia824_ratio,*s5_Pythia826_ratio;

    s5_Pythia824 = CalculateSimRRatio(s5_Pythia8[0], nullptr,s5_Pythia8[1],nullptr, 0);
    s5_Pythia824_ratio = Divide(s5_Pythia824,s5_hData_binned24);
    s5_Pythia826 = CalculateSimRRatio(s5_Pythia8[0], nullptr,s5_Pythia8[2],nullptr, 0);
    s5_Pythia826_ratio = Divide(s5_Pythia826,s5_hData_binned26);

    PlaceOnPad(Pad2[1],s5_Pythia824,kViolet+2,-1,3,"same1",0.3);
    PlaceOnPad(Pad2[1],s5_Pythia826,kViolet+2,-1,3,"same1",-1);
    PlaceOnPad(Pad2[3],s5_Pythia824_ratio,kViolet+2,-1,3,"same1",-1);
    PlaceOnPad(Pad2[5],s5_Pythia826_ratio,kViolet+2,-1,3,"same1",-1);

    //******************************************PYTHIA6***************************************************


    TGraphAsymmErrors *s13_Pythia624,*s13_Pythia626;
    TGraphAsymmErrors *s13_Pythia624_ratio,*s13_Pythia626_ratio;

    s13_Pythia624 = CalculateSimRRatio(s13_Pythia6[0], nullptr,s13_Pythia6[1],nullptr, 0);
    s13_Pythia624_ratio = Divide(s13_Pythia624,s13_hData_binned24);
    s13_Pythia626 = CalculateSimRRatio(s13_Pythia6[0], nullptr,s13_Pythia6[2],nullptr, 0);
    s13_Pythia626_ratio = Divide(s13_Pythia626,s13_hData_binned26);

    PlaceOnPad(Pad2[0],s13_Pythia624,kOrange+2,-1,2,"same1",0.3);
    PlaceOnPad(Pad2[0],s13_Pythia626,kOrange+2,-1,2,"same1",-1);
    PlaceOnPad(Pad2[2],s13_Pythia624_ratio,kOrange+2,-1,2,"same1",-1);
    PlaceOnPad(Pad2[4],s13_Pythia626_ratio,kOrange+2,-1,2,"same1",-1);

    TGraphAsymmErrors *s5_Pythia624,*s5_Pythia626;
    TGraphAsymmErrors *s5_Pythia624_ratio,*s5_Pythia626_ratio;

    s5_Pythia624 = CalculateSimRRatio(s5_Pythia6[0], nullptr,s5_Pythia6[1],nullptr, 0);
    s5_Pythia624_ratio = Divide(s5_Pythia624,s5_hData_binned24);
    s5_Pythia626 = CalculateSimRRatio(s5_Pythia6[0], nullptr,s5_Pythia6[2],nullptr, 0);
    s5_Pythia626_ratio = Divide(s5_Pythia626,s5_hData_binned26);

    PlaceOnPad(Pad2[1],s5_Pythia624,kOrange+2,-1,2,"same1",0.3);
    PlaceOnPad(Pad2[1],s5_Pythia626,kOrange+2,-1,2,"same1",-1);
    PlaceOnPad(Pad2[3],s5_Pythia624_ratio,kOrange+2,-1,2,"same1",-1);
    PlaceOnPad(Pad2[5],s5_Pythia626_ratio,kOrange+2,-1,2,"same1",-1);


    TCanvas *canvas3;
    TPad **Pad3;
    TH1D **placeholder3;
    std::tie(canvas3,Pad3,placeholder3) = PrepareCanvas4Pad(xAxisBins,xAxis);
Double_t scale2[3]{0.5,-1,2};

    TH1D *s13s5_hData_binned[3],*s13s5_hData_binned_ratio[3];
    TGraphAsymmErrors *s13s5_hData_binned_syst[3],*s13s5_hData_binned_syst_ratio[3];
    for(Int_t iEn = 0;iEn<3;iEn++){
        std::tie(s13s5_hData_binned[iEn], s13s5_hData_binned_syst[iEn]) = CalculateDataRRatio(s13_hData_binned[iEn],s13_hData_binned_syst[iEn],s5_hData_binned[iEn],s5_hData_binned_syst[iEn],-999);
        std::tie(s13s5_hData_binned_ratio[iEn], s13s5_hData_binned_syst_ratio[iEn]) = CalculateDataRRatio(s13_hData_binned_ratio[iEn],s13_hData_binned_ratio_syst[iEn],s5_hData_binned_ratio[iEn],s5_hData_binned_ratio_syst[iEn],-999);

        PlaceOnPadData(Pad3[0],s13s5_hData_binned_syst[iEn],s13s5_hData_binned[iEn],0.9,markers[iEn],scale2[iEn]);
        PlaceOnPadData(Pad3[iEn+1],s13s5_hData_binned_syst_ratio[iEn],s13s5_hData_binned_ratio[iEn],0,markers[iEn],-1);
    }

    TGraphAsymmErrors *s13s5_PowhegPythia8[3];
    TGraphAsymmErrors *s13s5_PowhegPythia8_ratio[3];
    for(Int_t iEn = 0;iEn<3;iEn++){
    s13s5_PowhegPythia8[iEn] = CalculateSimRRatio(s13_PowhegPythia8[iEn], s13_PowhegPythia8_var[iEn],s5_PowhegPythia8[iEn],s5_PowhegPythia8_var[iEn], 8);
    s13s5_PowhegPythia8_ratio[iEn] = Divide(s13s5_PowhegPythia8[iEn],s13s5_hData_binned[iEn]);
    PlaceOnPad(Pad3[0],s13s5_PowhegPythia8[iEn],kBlue+2,24,-1,"same2p",scale2[iEn]);
    PlaceOnPad(Pad3[iEn+1],s13s5_PowhegPythia8_ratio[iEn],kBlue+2,24,-1,"same2p",-1);
    }

    TGraphAsymmErrors *s13s5_Pythia8[3];
     TGraphAsymmErrors *s13s5_Pythia8_ratio[3];
     for(Int_t iEn = 0;iEn<3;iEn++){
     s13s5_Pythia8[iEn] = CalculateSimRRatio(s13_Pythia8[iEn], nullptr,s5_Pythia8[iEn],nullptr, 0);
     s13s5_Pythia8_ratio[iEn] = Divide(s13s5_Pythia8[iEn],s13s5_hData_binned[iEn]);
     PlaceOnPad(Pad3[0],s13s5_Pythia8[iEn],kViolet+2,-1,3,"same1",scale2[iEn]);
     PlaceOnPad(Pad3[iEn+1],s13s5_Pythia8_ratio[iEn],kViolet+2,-1,3,"same1",-1);
     }

     TGraphAsymmErrors *s13s5_Pythia6[3];
      TGraphAsymmErrors *s13s5_Pythia6_ratio[3];
      for(Int_t iEn = 0;iEn<3;iEn++){
      s13s5_Pythia6[iEn] = CalculateSimRRatio(s13_Pythia6[iEn],nullptr,s5_Pythia6[iEn],nullptr, 0);
      s13s5_Pythia6_ratio[iEn] = Divide(s13s5_Pythia6[iEn],s13s5_hData_binned[iEn]);
      PlaceOnPad(Pad3[0],s13s5_Pythia6[iEn],kGreen+1,-1,2,"same1",scale2[iEn]);
      PlaceOnPad(Pad3[iEn+1],s13s5_Pythia6_ratio[iEn],kGreen+1,-1,2,"same1",-1);
      }








    // ----------------- Legend and text PAD---------------------
    TLegend *leg =nullptr;
    //Double_t shift = 0.06*(2+ePowhegPythia6+ePowhegPythia8+ePythia6+ePythia8+ePythia8SoftMode2+ePowhegPythia6dijet);
    Double_t shift = 0.06*(2+3);
    leg = new TLegend(0.35,0.85-shift,0.85,0.85,nullptr,"NB NDC");
    leg->SetBorderSize(0);
    leg->SetTextFont(43);
    leg->SetTextSize(18);
    leg->SetLineColor(1);
    leg->SetLineStyle(1);
    leg->SetLineWidth(1);
    leg->SetFillColor(0);
    leg->SetFillStyle(0);
  //      leg->AddEntry(hDataR24,"Data #it{#sigma}(R=0.4)/#it{#sigma}(R=0.6) (+0.5)","fp");
 //   leg->AddEntry(hDataR26,"Data #it{#sigma}(R=0.2)/#it{#sigma}(R=0.6)","fp");
leg->AddEntry(s13_hData_binned_syst[2],"Data R=0.6 (x20)","fp");

    leg->AddEntry(s13_hData_binned_syst[1],"Data R=0.4","fp");
       leg->AddEntry(s13_hData_binned_syst[0],"Data R=0.2 (x0.05)","fp");
    leg->AddEntry(s13_PowhegPythia8[0],"POWHEG hvq + PYTHIA 8","pf");
    leg->AddEntry(s13_Pythia6[0],"PYTHIA 8 Monash 2013 Soft mode 2","l");
    leg->AddEntry(s13_Pythia8[0],"PYTHIA 8 Monash 2013","l");
   // leg->AddEntry(simPythia8Soft26,"PYTHIA 8 Monash 2013 Soft mode 2","pf");
    //leg->AddEntry(simPowhegPythia6dijet,"POWHEG dijet + PYTHIA 8 Salvatore","pf");

    TLegend *leg2=nullptr;
    //Double_t shift = 0.06*(2+ePowhegPythia6+ePowhegPythia8+ePythia6+ePythia8+ePythia8SoftMode2+ePowhegPythia6dijet);
    leg2 = new TLegend(0.35,0.85-shift,0.85,0.85,nullptr,"NB NDC");
    leg2->SetBorderSize(0);
    leg2->SetTextFont(43);
    leg2->SetTextSize(18);
    leg2->SetLineColor(1);
    leg2->SetLineStyle(1);
    leg2->SetLineWidth(1);
    leg2->SetFillColor(0);
    leg2->SetFillStyle(0);
        leg2->AddEntry(s13_hData_binned_syst24,"Data #it{#sigma}(R=0.2)/#it{#sigma}(R=0.4) scale: +0.3","fp");
    leg2->AddEntry(s13_hData_binned_syst26,"Data #it{#sigma}(R=0.2)/#it{#sigma}(R=0.6)","fp");
//leg->AddEntry(s13_hData_binned_syst[2],"Data R=0.6 (x20)","fp");
//    leg->AddEntry(s13_hData_binned_syst[1],"Data R=0.4","fp");
 //      leg->AddEntry(s13_hData_binned_syst[0],"Data R=0.2 (x0.05)","fp");
    leg2->AddEntry(s13_PowhegPythia824,"POWHEG hvq + PYTHIA 8","pf");
    leg2->AddEntry(s13_Pythia624,"PYTHIA 8 Monash 2013 soft mode 2","l");
    leg2->AddEntry(s13_Pythia824,"PYTHIA 8 Monash 2013","l");

    TLegend *leg3 =nullptr;
    //Double_t shift = 0.06*(2+ePowhegPythia6+ePowhegPythia8+ePythia6+ePythia8+ePythia8SoftMode2+ePowhegPythia6dijet);
    Double_t shift3 = 0.06*(2+3);
    leg3 = new TLegend(0.2,0.75-shift3,0.5,0.75,nullptr,"NB NDC");
    leg3->SetBorderSize(0);
    leg3->SetTextFont(43);
    leg3->SetTextSize(18);
    leg3->SetLineColor(1);
    leg3->SetLineStyle(1);
    leg3->SetLineWidth(1);
    leg3->SetFillColor(0);
    leg3->SetFillStyle(0);
leg3->AddEntry(s13s5_hData_binned_syst[2],"Data R=0.6 (x2)","fp");

    leg3->AddEntry(s13s5_hData_binned_syst[1],"Data R=0.4","fp");
       leg3->AddEntry(s13s5_hData_binned_syst[0],"Data R=0.2 (x0.5)","fp");

       TLegend *leg4 =nullptr;
       //Double_t shift = 0.06*(2+ePowhegPythia6+ePowhegPythia8+ePythia6+ePythia8+ePythia8SoftMode2+ePowhegPythia6dijet);
       Double_t shift4 = 0.06*(2+3);
       leg4 = new TLegend(0.5,0.75-shift4,0.85,0.75,nullptr,"NB NDC");
       leg4->SetBorderSize(0);
       leg4->SetTextFont(43);
       leg4->SetTextSize(18);
       leg4->SetLineColor(1);
       leg4->SetLineStyle(1);
       leg4->SetLineWidth(1);
       leg4->SetFillColor(0);
       leg4->SetFillStyle(0);
    leg4->AddEntry(s13_PowhegPythia8[0],"POWHEG hvq + PYTHIA 8","pf");
  //  leg4->AddEntry(s13_Pythia6[0],"PYTHIA 6 Perugia 2011","l");
    leg4->AddEntry(s13_Pythia8[0],"PYTHIA 8 Monash 2013","l");


    TPaveText *pt[4];
    pt[0] = new TPaveText(0.2,0.9,0.85,0.95,"NB NDC");
    pt[2] = new TPaveText(0.05,0.9,0.85,0.95,"NB NDC");
    pt[1] = new TPaveText(0.2,0.75,0.85,0.9,"NB NDC");
    pt[3] = new TPaveText(0.2,0.9,0.85,0.95,"NB NDC");
    for(Int_t s = 0; s<4;s++){
        pt[s]->SetBorderSize(0);
        pt[s]->SetFillStyle(0);
        pt[s]->SetTextAlign(13);
        pt[s]->SetTextFont(43);

    }
    pt[0]->SetTextSize(22);
    pt[1]->SetTextSize(18);
    pt[2]->SetTextSize(22);
    pt[3]->SetTextSize(22);
    //pt[0]->AddText("ALICE Preliminary"); //uncomment
    pt[0]->AddText("pp, #sqrt{#it{s}} = 13 TeV");
    pt[2]->AddText("pp, #sqrt{#it{s}} = 5.02 TeV");
    pt[3]->AddText("pp, #sqrt{#it{s}} = 13/5.02 TeV");
    pt[1]->AddText(Form("charged jets, anti-#it{k}_{T} with D^{0}, %d < #it{p}_{T,D^{0}} < %d GeV/#it{c}",static_cast<int>(2),static_cast<int>(36)));
   // pt[1]->AddText(Form ("with D^{0}, %d < #it{p}_{T,D^{0}} < %d GeV/#it{c}",static_cast<int>(2),static_cast<int>(36)));
   // if(type==1 ||type==2)pt[1]->AddText(Form ("%d < #it{p}_{T,jet} < %d GeV/#it{c}",jetpTbins[zBin-1],jetpTbins[zBin]));
   // if(type==1 ||type==2)pt[1]->AddText(Form ("with D^{0}, %d < #it{p}_{T,D^{0}} < %d GeV/#it{c}",DpTbins[0][zBin-1],DpTbins[1][zBin-1]));
   // if(type==1 ||type==2)pt[1]->AddText(Form ("%d < #it{p}_{T,jet} < %d GeV/#it{c} with D^{0}, %d < #it{p}_{T,D^{0}} < %d GeV/#it{c}",jetpTbins[zBin-1],jetpTbins[zBin],DpTbins[0][zBin-1],DpTbins[1][zBin-1]));
    //if(type==1 ||type==2)pt[1]->AddText(Form ("",DpTbins[0][zBin-1],DpTbins[1][zBin-1]));

    TPaveText *pt2[2];
    pt2[0] = new TPaveText(0.65,0.85,0.95,0.95,"NB NDC");
    pt2[1] = new TPaveText(0.65,0.85,0.95,0.95,"NB NDC");
    for(Int_t s = 0; s<2;s++){
        pt2[s]->SetBorderSize(0);
        pt2[s]->SetFillStyle(0);
        pt2[s]->SetTextAlign(13);
        pt2[s]->SetTextFont(43);
        pt2[s]->SetTextSize(19);
    }

    pt2[0]->AddText("#it{#sigma}(R=0.2)/#it{#sigma}(R=0.4)");
    pt2[1]->AddText("#it{#sigma}(R=0.2)/#it{#sigma}(R=0.6)");

    TPaveText *pt3[3];
    pt3[0] = new TPaveText(0.17,0.88,0.95,0.95,"NB NDC");
    pt3[1] = new TPaveText(0.17,0.91,0.95,0.95,"NB NDC");
    pt3[2] = new TPaveText(0.17,0.92,0.95,0.95,"NB NDC");
    for(Int_t s = 0; s<3;s++){
        pt3[s]->SetBorderSize(0);
        pt3[s]->SetFillStyle(0);
        pt3[s]->SetTextAlign(13);
        pt3[s]->SetTextFont(43);
        pt3[s]->SetTextSize(19);
    }

    pt3[0]->AddText("R=0.2");
    pt3[1]->AddText("R=0.4");
    pt3[2]->AddText("R=0.6");

    Pad3[0]->cd();
    pt[3]->Draw();
    pt[1]->Draw();
    leg3->Draw();
leg4->Draw();

    Pad[0]->cd();
    pt[0]->Draw();
    pt[1]->Draw();
    Pad[1]->cd();
    pt[2]->Draw();
    leg->Draw();
    /*dowmPad->cd();
    pt2[0]->Draw();
    dowmPad2->cd();
    pt2[1]->Draw();*/
    Pad[2]->cd();
    pt3[0]->Draw();
    Pad[4]->cd();
    pt3[1]->Draw();
    Pad[6]->cd();
    pt3[2]->Draw();

    Pad3[1]->cd();
    pt3[0]->Draw();
    Pad3[2]->cd();
    pt3[1]->Draw();
    Pad3[3]->cd();
    pt3[2]->Draw();

    Pad2[0]->cd();
    pt[0]->Draw();
    pt[1]->Draw();
    Pad2[1]->cd();
    pt[2]->Draw();
    leg2->Draw();
    Pad2[2]->cd();
    pt2[0]->Draw();
    Pad2[4]->cd();
    pt2[1]->Draw();

    // ----------------- Terminate Canvas ---------------------
 //   TerminateCanvas(upPad,dowmPad,dowmPad2,placeholder_up,placeholder_down,placeholder_down2);*/
    TerminateCanvas8Pad(Pad,placeholder);

 /*   std::cout<<"ipad0"<<std::endl;
    Pad[0]->cd();
    placeholder[0]->Draw("sameaxis");
    std::cout<<"ipad1"<<std::endl;
    Pad[1]->cd();
    placeholder[1]->Draw("sameaxis");
    for(Int_t ipad = 2; ipad<8;ipad++){
        Pad[ipad]->cd();
        placeholder[ipad]->Draw("sameaxis");
        placeholder[ipad]->Draw("sameaxig");
    }*/
    canvas->SaveAs("/home/kvapil/Desktop/ctest.png");
/*
    TFile *ofile = new TFile(Form("%s/JetPtSpectrum_final.root",outSpectraDir.Data()),"RECREATE");
    hData_binned->Write();
    hDataSys->Write();
    hData_binned_ratio->Write();
    hDataSysRatio->Write();
    if(ePowhegPythia6){
        simPowhegPythia6_cent->Write();
        simPowhegPythia6_up->Write();
        simPowhegPythia6_down->Write();
        simPowhegPythia6->Write();
        simPowhegPythia6_cent_R->Write();
        simPowhegPythia6_up_R->Write();
        simPowhegPythia6_down_R->Write();
        simPowhegPythia6_R->Write();
    }

    if(ePowhegPythia8){
        simPowhegPythia8_cent->Write();
        simPowhegPythia8_up->Write();
        simPowhegPythia8_down->Write();
        simPowhegPythia8->Write();
        simPowhegPythia8_cent_R->Write();
        simPowhegPythia8_up_R->Write();
        simPowhegPythia8_down_R->Write();
        simPowhegPythia8_R->Write();
    }

    if(ePythia6){
        simPythia6_cent->Write();
        simPythia6_up->Write();
        simPythia6_down->Write();
        simPythia6->Write();
        simPythia6_cent_R->Write();
        simPythia6_up_R->Write();
        simPythia6_down_R->Write();
        simPythia6_R->Write();
    }

    if(ePythia8){
        simPythia8_cent->Write();
        simPythia8_up->Write();
        simPythia8_down->Write();
        simPythia8->Write();
        simPythia8_cent_R->Write();
        simPythia8_up_R->Write();
        simPythia8_down_R->Write();
        simPythia8_R->Write();
    }

    if(ePythia8SoftMode2){
        simPythia8Soft2_cent->Write();
        simPythia8Soft2_up->Write();
        simPythia8Soft2_down->Write();
        simPythia8Soft2->Write();
        simPythia8Soft2_cent_R->Write();
        simPythia8Soft2_up_R->Write();
        simPythia8Soft2_down_R->Write();
        simPythia8Soft2_R->Write();
    }

    if(ePowhegPythia6dijet){
        simPowhegPythia6dijet_cent->Write();
        simPowhegPythia6dijet_up->Write();
        simPowhegPythia6dijet_down->Write();
        simPowhegPythia6dijet->Write();
        simPowhegPythia6dijet_cent_R->Write();
        simPowhegPythia6dijet_up_R->Write();
        simPowhegPythia6dijet_down_R->Write();
        simPowhegPythia6dijet_R->Write();
    }

    ofile->Close();*/
    return;
}

std::tuple<TH1D*, TGraphAsymmErrors*> CalculateDataRRatio(TH1D *stat1,TGraphAsymmErrors *sys1, TH1D *stat2,TGraphAsymmErrors *sys2,Double_t shift){
    TString name1 = stat1->GetName();
    TString name2 = stat2->GetName();
    TH1D *tmp = dynamic_cast<TH1D*>(stat1->Clone(name1+"_"+name2+"_ratio"));
    tmp->Divide(stat2);

  /*  if(shift > -900){
        for(Int_t i=1;i<=tmp->GetNbinsX();i++){
            tmp->SetBinContent(i,tmp->GetBinContent(i)+shift);
        }
    }*/

    UInt_t nBins = static_cast<UInt_t>(sys1->GetN());
    Double_t *sysuncAbs_down = new Double_t[nBins];
    Double_t *sysuncAbs_up = new Double_t[nBins];
    Double_t *statunc = new Double_t[nBins];
    Double_t *value = new Double_t[nBins];
    Double_t *xval = new Double_t[nBins];
    Double_t *xvalwidth_down = new Double_t[nBins];
    Double_t *xvalwidth_up = new Double_t[nBins];

   // TGraphAsymmErrors *grsysl = new TGraphAsymmErrors(static_cast<Int_t>(nBins),xval,value,xvalwidth,xvalwidth,sysuncAbs_down,sysuncAbs_up);

    for(Int_t bin = 0; bin < sys1->GetN();bin++){
        Double_t x1 = 0,y1=0;
        Double_t x2 = 0,y2=0;
        sys1->GetPoint(bin,x1,y1);
        sys2->GetPoint(bin,x2,y2);

        std::cout<<"hist1:"<<std::endl;
        std::cout<<x1<<" "<<y1<<std::endl;//<<" "<<hData_binned4->GetBinError(bin)/hData_binned4->GetBinContent(bin)<<" "<<hDataR24->GetBinError(bin)/hDataR24->GetBinContent(bin)<<std::endl;
        std::cout<<sys1->GetErrorXlow(bin)<<" "<<sys1->GetErrorXhigh(bin)<<" "<<sys1->GetErrorYlow(bin)/y1<<" "<<sys1->GetErrorYhigh(bin)/y1<<std::endl;

        std::cout<<"hist2:"<<std::endl;
        std::cout<<sys2->GetErrorXlow(bin)<<" "<<sys2->GetErrorXhigh(bin)<<" "<<sys2->GetErrorYlow(bin)/y2<<" "<<sys2->GetErrorYhigh(bin)/y2<<std::endl;
        std::cout<<"merge:"<<std::endl;
        std::cout<<"lq: "<<sys1->GetErrorYlow(bin)/y1<<" "<<sys2->GetErrorYlow(bin)/y2<<std::endl;
        std::cout<<"lq: "<<sys1->GetErrorYlow(bin)/y1*sys1->GetErrorYlow(bin)/y1<<" "<<sys2->GetErrorYlow(bin)/y2*sys2->GetErrorYlow(bin)/y2<<std::endl;
        std::cout<<"lq: "<<sys1->GetErrorYlow(bin)/y1*sys1->GetErrorYlow(bin)/y1+sys2->GetErrorYlow(bin)/y2*sys2->GetErrorYlow(bin)/y2<<std::endl;
       std::cout<<"lq: "<<TMath::Sqrt(sys1->GetErrorYlow(bin)/y1*sys1->GetErrorYlow(bin)/y1+sys2->GetErrorYlow(bin)/y2*sys2->GetErrorYlow(bin)/y2)<<std::endl;
       std::cout<<"uq: "<<TMath::Sqrt(sys1->GetErrorYhigh(bin)/y1*sys1->GetErrorYhigh(bin)/y1+sys2->GetErrorYhigh(bin)/y2*sys2->GetErrorYhigh(bin)/y2)<<std::endl;

       Double_t sysUP = TMath::Sqrt(sys1->GetErrorYhigh(bin)/y1*sys1->GetErrorYhigh(bin)/y1+sys2->GetErrorYhigh(bin)/y2*sys2->GetErrorYhigh(bin)/y2);
       Double_t sysDown = TMath::Sqrt(sys1->GetErrorYlow(bin)/y1*sys1->GetErrorYlow(bin)/y1+sys2->GetErrorYlow(bin)/y2*sys2->GetErrorYlow(bin)/y2);

       xval[bin] = x1;
       xvalwidth_down[bin] = sys1->GetErrorXlow(bin);
       xvalwidth_up[bin] = sys1->GetErrorXhigh(bin);
       value[bin] = y1/y2;
      // std::cout<<j<<" "<<xval[j]<<" "<<xvalwidth[j]<<" "<<value[j]<<" "<<error<<std::endl;
       sysuncAbs_down[bin] = value[bin] * sysDown;
       sysuncAbs_up[bin] = value[bin] * sysUP;

     /*  if(shift > -900){
          value[bin]+=shift;
          //sysuncAbs_down[bin]+=shift;
          //sysuncAbs_up[bin]+=shift;
       }*/
       //if(value[j]>1E-9)statunc[j] = error/ value[j] *100;
std::cout<<"sum: bin: "<<bin<<" xval: "<<x1<<" xwidth low: "<<xvalwidth_down[bin]<<" xwidth high: "<<xvalwidth_down[bin]<<" value: "<<value[bin]<<" sysdown: "<<sysuncAbs_down[bin]<<" sysup: "<<sysuncAbs_up[bin]<<std::endl;
    }
    TGraphAsymmErrors *grsysl = new TGraphAsymmErrors(static_cast<Int_t>(nBins),xval,value,xvalwidth_down,xvalwidth_up,sysuncAbs_down,sysuncAbs_up);
    grsysl->SetName(name1+"_"+name2+"_ratio_sys");
    return std::make_tuple(tmp,grsysl);
}

TGraphAsymmErrors* CalculateSimRRatio(TGraphAsymmErrors *hist1,TH1D **var1,TGraphAsymmErrors *hist2,TH1D **var2, UInt_t vars){
    std::cout<<"GETTING SIMS"<<std::endl;
    TString name1 = hist1->GetName();
    TString name2 = hist2->GetName();
    TH1D **var1var2 = new TH1D*[static_cast<ULong_t>(vars)];
    TH1D * hPrompt_up = nullptr;
    TH1D * hPrompt_down = nullptr;
    TH1D * central1 = nullptr;
    TH1D * central2 = nullptr;

    if(var1!=nullptr){
        for(UInt_t ivar = 0; ivar < vars;ivar++){
            var1var2[ivar] = dynamic_cast<TH1D*>(var1[ivar]->Clone(Form(name1+"_"+name2+"_%d",ivar)));
            var1var2[ivar]->Divide(var2[ivar]);
            std::cout<<"VAR: " <<ivar<<" v1: "<<var1[ivar]->GetBinContent(1)<<" v2: "<<var2[ivar]->GetBinContent(1)<<" v1v2: "<<var1var2[ivar]->GetBinContent(1)<<std::endl;
        }
        central1 = dynamic_cast<TH1D*>(var1[0]->Clone(name1+"_"+name2+"_central1"));
        central2 = dynamic_cast<TH1D*>(var1[0]->Clone(name1+"_"+name2+"_central2"));

        for(UInt_t bin=0; bin<hist1->GetN(); bin++){
            Double_t x1 = 0,y1=0;
            Double_t x2 = 0,y2=0;
            hist1->GetPoint(bin,x1,y1);
            hist2->GetPoint(bin,x2,y2);
            central1->SetBinContent(bin+1,y1);
            central2->SetBinContent(bin+1,y2);
            central1->SetBinError(bin+1,0);
            central2->SetBinError(bin+1,0);
            std::cout<<"central1: "<<y1<<" central2: "<<y2<<" div "<<y1/y2<<std::endl;
        }
        central1->Divide(central2);
        for(UInt_t bin=1; bin<=central1->GetNbinsX(); bin++)std::cout<<"central val: "<<central1->GetBinContent(bin)<<std::endl;

        hPrompt_up = GetUpSys(central1,var1var2,vars);
        hPrompt_up->SetName(name1+"_"+name2+"_up");
        // get down unc
        hPrompt_down = GetDownSys(central1,var1var2,vars);
        hPrompt_down->SetName(name1+"_"+name2+"_down");

        std::cout<<" UP: "<<hPrompt_up->GetBinContent(1)<<" DOWN: "<<hPrompt_down->GetBinContent(1)<<std::endl;
    }

    UInt_t nBin = hist1->GetN();
    Double_t *xval = new Double_t[nBin];
    Double_t *xvalwidth_down = new Double_t[nBin];
    Double_t *xvalwidth_up = new Double_t[nBin];
    Double_t *value = new Double_t[nBin];
    Double_t *valuetheoryerrup = new Double_t[nBin];
    Double_t *valuetheoryerrdown = new Double_t[nBin];
    for(UInt_t bin=0; bin<nBin; bin++){
        Double_t x1 = 0,y1=0;
        Double_t x2 = 0,y2=0;
        hist1->GetPoint(bin,x1,y1);
        hist2->GetPoint(bin,x2,y2);
        std::cout<<"SIM: "<<y1<<" "<<y2<< " "<<y1/y2<<std::endl;
        xval[bin] = x1;
        xvalwidth_down[bin] = hist1->GetErrorXlow(bin);
        xvalwidth_up[bin] = hist1->GetErrorXhigh(bin);
        value[bin] = y1/y2;
        if(var1!=nullptr){
            valuetheoryerrup[bin] = hPrompt_up->GetBinContent(hPrompt_up->GetXaxis()->FindBin(xval[bin])) - value[bin];
            valuetheoryerrdown[bin] = value[bin] - hPrompt_down->GetBinContent(hPrompt_up->GetXaxis()->FindBin(xval[bin]));
            std::cout<<"UP: "<<valuetheoryerrup[bin]<<" DOWN: "<<valuetheoryerrdown[bin]<<std::endl;
        }
        else{
            valuetheoryerrup[bin] = 0;
            valuetheoryerrdown[bin] = 0;
        }

    }
    TGraphAsymmErrors *grsystheory = new TGraphAsymmErrors(static_cast<Int_t>(nBin),xval,value,xvalwidth_down,xvalwidth_up,valuetheoryerrdown,valuetheoryerrup);
    grsystheory->SetName(name1+"_"+name2+"_ratio_sys");
    return grsystheory;

}

void TerminateCanvas(TPad* pad1, TPad* pad2, TPad *pad3, TH1D* histo1, TH1D* histo2, TH1D *histo3){
    pad1->cd();
    histo1->Draw("sameaxis");
    pad2->cd();
    histo2->Draw("sameaxis");
    histo2->Draw("sameaxig");
    pad3->cd();
    histo3->Draw("sameaxis");
    histo3->Draw("sameaxig");
}

void TerminateCanvas8Pad(TPad **pad, TH1D** histo){
    std::cout<<"ipad0"<<std::endl;
    pad[0]->cd();
    histo[0]->Draw("sameaxis");
    std::cout<<"ipad1"<<std::endl;
    pad[1]->cd();
    histo[1]->Draw("sameaxis");
    for(Int_t ipad = 2; ipad<8;ipad++){
        pad[ipad]->cd();
        histo[ipad]->Draw("sameaxis");
        histo[ipad]->Draw("sameaxig");
    }
}

void PlaceOnPad(TPad* pad,TGraphAsymmErrors* histo, Color_t ci, Style_t style, Style_t linestyle, TString opt,Double_t scale){
    pad->cd();
    histo->SetFillColor(1);
    histo->SetFillStyle(0);
    if(ci!=-1)histo->SetLineColor(ci);
    if(ci!=-1)histo->SetMarkerColor(ci);
   if(style!=-1) histo->SetMarkerStyle(style);
    if(linestyle!=-1)histo->SetLineStyle(linestyle);
   // histo->SetFillStyle(3005);
   // histo->SetMarkerSize(markersize); //add up
    histo->SetLineWidth(2);
    if(scale>-1){
        TString tmp = "";
        TGraphAsymmErrors* histo_tmp = dynamic_cast<TGraphAsymmErrors*>(histo->Clone(tmp+histo->GetName()+"_plotCp"));
        for (int i=0;i<histo_tmp->GetN();i++) histo_tmp->GetY()[i] += scale;
        //for (int i=0;i<histo_tmp->GetN();i++) histo_tmp->GetEYhigh()[i] *= scale;
        //for (int i=0;i<histo_tmp->GetN();i++) histo_tmp->GetEYlow()[i] *= scale;
        if(opt !="") histo_tmp->Draw(opt);
        else histo_tmp->Draw("psame");
    }
    else if(opt !="") histo->Draw(opt);
    else histo->Draw("psame");
}

void PlaceOnPad(TPad* pad,TH1D* histo, Color_t ci, Style_t style, Style_t linestyle, TString opt,Double_t scale){
    pad->cd();
    histo->SetFillColor(1);
    histo->SetFillStyle(0);
    histo->SetLineColor(ci);
    histo->SetMarkerColor(ci);
    histo->SetMarkerStyle(style);
    histo->SetLineStyle(linestyle);
   // histo->SetFillStyle(3005);
   // histo->SetMarkerSize(markersize); //add up
    histo->SetLineWidth(2);
    if(scale>-1){
        TString tmp = "";
        TH1D* histo_tmp = dynamic_cast<TH1D*>(histo->Clone(tmp+histo->GetName()+"_plotCp"));
        histo_tmp->Scale(scale);
        if(opt !="") histo_tmp->Draw(opt);
        else histo_tmp->Draw("psame");
    }
    else if(opt !="") histo->Draw(opt);
    else histo->Draw("psame");
}

void PlaceOnPadData(TPad* pad,TGraphAsymmErrors* histo1, TH1D* histo2, Size_t markersize, Style_t markerstyle,Double_t scale){
    pad->cd();
    Color_t ci;// = static_cast<Color_t>(TColor::GetColor("#990000"));
    //histo1->SetLineColor(ci);
    //histo1->SetMarkerColor(ci);
    histo1->SetMarkerStyle(markerstyle);
    histo1->SetMarkerSize(markersize);
    ci = static_cast<Color_t>(TColor::GetColor("#cccccc"));
    histo1->SetFillColor(ci);
    histo1->SetLineColor(ci);
    histo1->SetLineWidth(0);
    if(scale>-1){
        TString tmp = "";
        TGraphAsymmErrors* histo_tmp = dynamic_cast<TGraphAsymmErrors*>(histo1->Clone(tmp+histo1->GetName()+"_plotCp"));
        for (int i=0;i<histo_tmp->GetN();i++) histo_tmp->GetY()[i] += scale;
  //      for (int i=0;i<histo_tmp->GetN();i++) histo_tmp->GetEYhigh()[i] += scale;
  //      for (int i=0;i<histo_tmp->GetN();i++) histo_tmp->GetEYlow()[i] += scale;
        histo_tmp->Draw("2p same");
    }    else histo1->Draw("2p same");
    //data central w stat. unc.
    //ci = static_cast<Color_t>(TColor::GetColor("#990000"));
   // histo2->SetLineColor(ci);
   // histo2->SetMarkerColor(ci);
    histo2->SetMarkerStyle(markerstyle);
    histo2->SetMarkerSize(markersize);
    if(scale>-1){
        TString tmp = "";
        TH1D* histo_tmp = dynamic_cast<TH1D*>(histo2->Clone(tmp+histo2->GetName()+"_plotCp"));
        //histo_tmp->Scale(scale);
        for(Int_t i = 1 ; i<= histo_tmp->GetNbinsX();i++){
            histo_tmp->SetBinContent(i,histo_tmp->GetBinContent(i)+scale);
        }
        histo_tmp->Draw("same p  e0 x0");
    }    else histo2->Draw("same p  e0 x0");
};

std::tuple<TCanvas*, TPad**, TH1D**> PrepareCanvas8Pad(UInt_t xAxisBins, Double_t *xAxisl, Double_t *xAxisr){
    style();
    //prepare main canvas
    TCanvas *FinalSpectrum = new TCanvas("FinalSpectrum", "FinalSpectrum",0,45,1000,700);
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    FinalSpectrum->SetHighLightColor(2);
    FinalSpectrum->Range(0,0,1,1);
    FinalSpectrum->SetFillColor(0);
    FinalSpectrum->SetBorderMode(0);
    FinalSpectrum->SetBorderSize(2);
    FinalSpectrum->SetFrameBorderMode(0);
    TPad **pad = new TPad*[static_cast<ULong_t>(10)];
    //0  1
    //2  3
    //4  5
    //6  7
/*
    for(Int_t ipad = 0;ipad <8;ipad++){
        if(ipad ==0) pad[ipad] = new TPad("pad0", "pad0",0,     0.5  ,0.545 ,1);
        if(ipad ==1) pad[ipad] = new TPad("pad1", "pad1",0.545 ,0.5  ,1     ,1);
        if(ipad ==2) pad[ipad] = new TPad("pad2", "pad2",0,     0.35 ,0.545 ,0.5);
        if(ipad ==3) pad[ipad] = new TPad("pad3", "pad3",0.545 ,0.35 ,1     ,0.5);
        if(ipad ==4) pad[ipad] = new TPad("pad4", "pad4",0,     0.20  ,0.545 ,0.35);
        if(ipad ==5) pad[ipad] = new TPad("pad5", "pad5",0.545 ,0.20  ,1     ,0.35);
        if(ipad ==6) pad[ipad] = new TPad("pad6", "pad6",0,     0    ,0.545 ,0.2001);
        if(ipad ==7) pad[ipad] = new TPad("pad7", "pad7",0.545 ,0    ,1     ,0.2001);
        pad[ipad]->Draw();
        pad[ipad]->cd();
      //  if(ipad <2)pad[ipad]->Range(-1.986821e-07,-4.69897,33.33333,0.3499945);
     //   else pad[ipad]->Range(-1.986821e-07,-0.9209589,33.33333,2.49);
        pad[ipad]->SetFillColor(0);
        pad[ipad]->SetBorderMode(0);
        pad[ipad]->SetBorderSize(2);
        if(ipad>1)pad[ipad]->SetGridy();
        pad[ipad]->SetTickx(1);
        pad[ipad]->SetTicky(1);
        if(ipad%2==0)pad[ipad]->SetLeftMargin(0.18f);
        else pad[ipad]->SetLeftMargin(0);
        if(ipad < 6)pad[ipad]->SetBottomMargin(0);
        else pad[ipad]->SetBottomMargin(0.27f);
        if(ipad > 1)pad[ipad]->SetTopMargin(0);
        if(ipad%2==0)pad[ipad]->SetRightMargin(0);
        pad[ipad]->SetFrameBorderMode(0);
        pad[ipad]->SetFrameBorderMode(0);
        FinalSpectrum->cd();
    }
    */
    for(Int_t ipad = 0;ipad <8;ipad++){
        Double_t shift = 0.002;
        std::cout<<"padID: "<<ipad<<std::endl;
        if(ipad ==0) pad[ipad] = new TPad("pad0", "pad0",0,     0.5  ,0.545-shift ,1);
        if(ipad ==1) pad[ipad] = new TPad("pad1", "pad1",0.545+shift ,0.5  ,1     ,1);
        if(ipad ==2) pad[ipad] = new TPad("pad2", "pad2",0,     0.36 ,0.545-shift ,0.5);
        if(ipad ==3) pad[ipad] = new TPad("pad3", "pad3",0.545+shift ,0.36 ,1     ,0.5);
        if(ipad ==4) pad[ipad] = new TPad("pad4", "pad4",0,     0.22  ,0.545-shift ,0.36);
        if(ipad ==5) pad[ipad] = new TPad("pad5", "pad5",0.545+shift ,0.22  ,1     ,0.36);
        if(ipad ==6) pad[ipad] = new TPad("pad6", "pad6",0,     0.0    ,0.545-shift ,0.2201);
        if(ipad ==7) pad[ipad] = new TPad("pad7", "pad7",0.545+shift ,0.0    ,1     ,0.2201);
       // if(ipad ==8) pad[ipad] = new TPad("pad8", "pad8",0,     0.05    ,0.545 ,0.2001);
       // if(ipad ==9) pad[ipad] = new TPad("pad9", "pad9",0.545 ,0.05    ,1     ,0.2001);
        pad[ipad]->Draw();
        pad[ipad]->cd();
        if(ipad <2)pad[ipad]->Range(-1.986821e-07,-4.69897,33.33333,0.3499945);
        else pad[ipad]->Range(-1.986821e-07,-0.9209589,33.33333,2.49);
        pad[ipad]->SetFillColor(0);
        pad[ipad]->SetBorderMode(0);
        pad[ipad]->SetBorderSize(2);
        if(ipad>1)pad[ipad]->SetGridy();
        if(ipad<2)pad[ipad]->SetLogy();
        pad[ipad]->SetTickx(1);
        pad[ipad]->SetTicky(1);
        if(ipad%2==0)pad[ipad]->SetLeftMargin(0.2f);
        else pad[ipad]->SetLeftMargin(0+shift+shift+shift+shift);
        if(ipad < 6)pad[ipad]->SetBottomMargin(0);
        else pad[ipad]->SetBottomMargin(0.35f);
        if(ipad > 1)pad[ipad]->SetTopMargin(0);
        if(ipad%2==0)pad[ipad]->SetRightMargin(0+shift);
        pad[ipad]->SetFrameBorderMode(0);
        //pad[ipad]->SetFrameBorderSize(2);
        //pad[ipad]->SetFrameLineWidth(2);
        FinalSpectrum->cd();
    }
/*
    for(Int_t ipad = 0;ipad <10;ipad++){
            if(ipad ==8) pad[ipad] = new TPad("pad6", "pad6",0,     0.    ,0.545 ,0.05);
            if(ipad ==9) pad[ipad] = new TPad("pad7", "pad7",0.545 ,0.    ,1     ,0.05);
            pad[ipad]->Draw();
            pad[ipad]->cd();
          //  if(ipad <2)pad[ipad]->Range(-1.986821e-07,-4.69897,33.33333,0.3499945);
         //   else pad[ipad]->Range(-1.986821e-07,-0.9209589,33.33333,2.49);
            pad[ipad]->SetFillColor(0);
            pad[ipad]->SetBorderMode(0);
            pad[ipad]->SetBorderSize(2);
            pad[ipad]->SetGridy();
            pad[ipad]->SetTickx(1);
            pad[ipad]->SetTicky(1);
            if(ipad%2==0)pad[ipad]->SetLeftMargin(0.18f);
            else pad[ipad]->SetLeftMargin(0);
            pad[ipad]->SetBottomMargin(0.27f);
            pad[ipad]->SetTopMargin(0);
            if(ipad%2==0)pad[ipad]->SetRightMargin(0);
            pad[ipad]->SetFrameBorderMode(0);
            pad[ipad]->SetFrameBorderMode(0);
            FinalSpectrum->cd();
        }
*/


/*
    //Set primitives in bottom pad
    TPad *pad_bottom2 = new TPad("pad_bottom2", "pad_bottom2",0,0,1,0.275);
    pad_bottom2->Draw();
    pad_bottom2->cd();
    pad_bottom2->Range(-1.986821e-07,-0.9209589,33.33333,2.49);
    pad_bottom2->SetFillColor(0);
    pad_bottom2->SetBorderMode(0);
    pad_bottom2->SetBorderSize(2);
    pad_bottom2->SetGridy();
    pad_bottom2->SetTickx(1);
    pad_bottom2->SetTicky(1);
    pad_bottom2->SetLeftMargin(0.18f);
    pad_bottom2->SetTopMargin(0);
    pad_bottom2->SetBottomMargin(0.27f);
    pad_bottom2->SetFrameBorderMode(0);
    pad_bottom2->SetFrameBorderMode(0);

*/
    std::cout<<"a"<<std::endl;
    //TH1D *placeholder[8];
    TH1D **placeholder = new TH1D*[static_cast<ULong_t>(10)];
   /* for(Int_t ipad = 8;ipad <10;ipad++){
        pad[ipad]->cd();
        placeholder[ipad] = new TH1D(Form("placeholder%d",ipad),"Central Values",static_cast<Int_t>(xAxisBins), xAxis);
        if(ipad<2)placeholder[ipad]->SetMinimum(0.1);
        else placeholder[ipad]->SetMinimum(0.7);
        if(ipad<2)placeholder[ipad]->SetMaximum(10);
        else placeholder[ipad]->SetMaximum(1.3);
        placeholder[ipad]->GetXaxis()->SetTitle("#it{p}_{T,ch jet} (GeV/#it{c})");
        placeholder[ipad]->GetXaxis()->SetLabelFont(43);
        if(ipad<6)placeholder[ipad]->GetXaxis()->SetLabelSize(0.035f);
        else placeholder[ipad]->GetXaxis()->SetLabelSize(19);
        if(ipad<6)placeholder[ipad]->GetXaxis()->SetTitleSize(0.035f);
        else placeholder[ipad]->GetXaxis()->SetTitleSize(26);
        placeholder[ipad]->GetXaxis()->SetTitleFont(42);
        //placeholder[ipad]->GetYaxis()->SetRangeUser(plotRanges[2],plotRanges[3]);
        if(ipad<2)placeholder[ipad]->GetYaxis()->SetTitle("Ratio of probability densities");
        else placeholder[ipad]->GetYaxis()->SetTitle("MC/Data");
        placeholder[ipad]->GetYaxis()->SetLabelFont(43);
        placeholder[ipad]->GetYaxis()->SetLabelSize(19);
        placeholder[ipad]->GetYaxis()->SetTitleSize(26);
        placeholder[ipad]->GetYaxis()->SetLabelOffset(0.015f);
        placeholder[ipad]->GetYaxis()->SetTitleOffset(2.f);
        placeholder[ipad]->GetYaxis()->SetTitleFont(43);
        placeholder[ipad]->GetYaxis()->SetDecimals();
        placeholder[ipad]->Draw("axis");
        //placeholder[ipad]->Draw("sameaxig");
    }*/

    for(Int_t ipad = 0;ipad <8;ipad++){
       // if(ipad==6 || ipad==7) continue;
        pad[ipad]->cd();
        if(ipad%2==0)placeholder[ipad] = new TH1D(Form("placeholder%d",ipad),"Central Values",static_cast<Int_t>(xAxisBins), xAxisl);
        else placeholder[ipad] = new TH1D(Form("placeholder%d",ipad),"Central Values",static_cast<Int_t>(xAxisBins), xAxisr);
        if(ipad<2)placeholder[ipad]->SetMinimum(30E-9);
        else placeholder[ipad]->SetMinimum(0.1);
        if(ipad<2)placeholder[ipad]->SetMaximum(100);
        else placeholder[ipad]->SetMaximum(3.3);
        placeholder[ipad]->GetXaxis()->SetTitle("#it{p}_{T,ch jet} (GeV/#it{c})");
        placeholder[ipad]->GetXaxis()->SetLabelFont(43);
        if(ipad<6)placeholder[ipad]->GetXaxis()->SetLabelSize(0.035f);
        else placeholder[ipad]->GetXaxis()->SetLabelSize(19);
       // if(ipad<6)placeholder[ipad]->GetXaxis()->SetTitleSize(0.035f);
        //else
        placeholder[ipad]->GetXaxis()->SetTitleOffset(1.11f);
            placeholder[ipad]->GetXaxis()->SetTitleSize(0.15f);
        placeholder[ipad]->GetXaxis()->SetTitleFont(42);
        //placeholder[ipad]->GetYaxis()->SetRangeUser(plotRanges[2],plotRanges[3]);
        if(ipad<2)placeholder[ipad]->GetYaxis()->SetTitle("#frac{d^{2}#it{#sigma}}{d#it{p}_{T}d#it{#eta}} [mb (GeV/#it{c})^{#minus1}]");
        //if(ipad<2)placeholder[ipad]->GetYaxis()->SetTitle("Ratio of probability densities");
        else if(ipad==4)placeholder[ipad]->GetYaxis()->SetTitle("MC/Data");
        placeholder[ipad]->GetYaxis()->SetLabelFont(43);
        placeholder[ipad]->GetYaxis()->SetLabelSize(19);
        placeholder[ipad]->GetYaxis()->SetTitleSize(23);
        placeholder[ipad]->GetYaxis()->SetLabelOffset(0.015f);
        placeholder[ipad]->GetYaxis()->SetTitleOffset(2.5f);
        placeholder[ipad]->GetYaxis()->SetTitleFont(43);
        placeholder[ipad]->GetYaxis()->SetDecimals();
        if(ipad>1)placeholder[ipad]->GetYaxis()->SetNdivisions(5, 5, 0, kTRUE);
        //std::cout<<placeholder[ipad]->GetYaxis()->GetTickLength()<<std::endl;
        if(ipad==6 || ipad==7) placeholder[ipad]->GetYaxis()->SetTickLength(0.04);
        placeholder[ipad]->Draw("axis");
        //placeholder[ipad]->Draw("sameaxig");
    }
std::cout<<"a"<<std::endl;
 /*   pad[2]->cd();
    placeholder[2] = new TH1D("placeholder2","Central Values",static_cast<Int_t>(xAxisBins), xAxis);
    placeholder[2]->SetMinimum(0.4);
    placeholder[2]->SetMaximum(plotRanges[1]);
   // if(zBin ==0)hEmpty_bottom->GetXaxis()->SetTitle("#it{p}_{T,ch jet} (GeV/#it{c})");
   // else hEmpty_bottom->GetXaxis()->SetTitle("z_{#parallel}");
    placeholder[2]->GetXaxis()->SetLabelFont(43);
    placeholder[2]->GetXaxis()->SetLabelSize(19);
    placeholder[2]->GetXaxis()->SetTitleSize(26);
    placeholder[2]->GetXaxis()->SetTitleOffset(3.f);
    placeholder[2]->GetXaxis()->SetTitleFont(43);
    placeholder[2]->GetYaxis()->SetTitle("MC/Data");
    placeholder[2]->GetYaxis()->SetNdivisions(509);
    placeholder[2]->GetYaxis()->CenterTitle();
    placeholder[2]->GetYaxis()->SetDecimals();
    placeholder[2]->GetYaxis()->SetLabelOffset(0.015f);
    placeholder[2]->GetXaxis()->SetLabelOffset(0.02f);
    placeholder[2]->GetYaxis()->SetLabelFont(43);
    placeholder[2]->GetYaxis()->SetLabelSize(19);
    placeholder[2]->GetYaxis()->SetTitleSize(26);
    placeholder[2]->GetYaxis()->SetTitleOffset(2.f);
    placeholder[2]->GetYaxis()->SetTitleFont(43);
    placeholder[2]->Draw("axis");*/
/*
    pad_bottom2->cd();
    TH1D *hEmpty_bottom2 = new TH1D("hEmpty_bottom2","Central Values",static_cast<Int_t>(xAxisBins), xAxis);
    hEmpty_bottom2->SetMinimum(plotRanges[0]);
    hEmpty_bottom2->SetMaximum(1.6);
    hEmpty_bottom2->GetXaxis()->SetTitle("#it{p}_{T,ch jet} (GeV/#it{c})");
    //else hEmpty_bottom->GetXaxis()->SetTitle("z_{#parallel}");
    hEmpty_bottom2->GetXaxis()->SetLabelFont(43);
    hEmpty_bottom2->GetXaxis()->SetLabelSize(19);
    hEmpty_bottom2->GetXaxis()->SetTitleSize(26);
    hEmpty_bottom2->GetXaxis()->SetTitleOffset(3.f);
    hEmpty_bottom2->GetXaxis()->SetTitleFont(43);
    hEmpty_bottom2->GetYaxis()->SetTitle("MC/Data");
    hEmpty_bottom2->GetYaxis()->SetNdivisions(509);
    hEmpty_bottom2->GetYaxis()->CenterTitle();
    hEmpty_bottom2->GetYaxis()->SetDecimals();
    hEmpty_bottom2->GetYaxis()->SetLabelOffset(0.015f);
    hEmpty_bottom2->GetXaxis()->SetLabelOffset(0.02f);
    hEmpty_bottom2->GetYaxis()->SetLabelFont(43);
    hEmpty_bottom2->GetYaxis()->SetLabelSize(19);
    hEmpty_bottom2->GetYaxis()->SetTitleSize(26);
    hEmpty_bottom2->GetYaxis()->SetTitleOffset(2.f);
    hEmpty_bottom2->GetYaxis()->SetTitleFont(43);
    hEmpty_bottom2->Draw("axis");
*/
    return std::make_tuple(FinalSpectrum,pad, placeholder);

}

std::tuple<TCanvas*, TPad**, TH1D**> PrepareCanvas4Pad(UInt_t xAxisBins, Double_t *xAxis){
    style();
    //prepare main canvas
    TCanvas *FinalSpectrum = new TCanvas("FinalSpectrum2", "FinalSpectrum2",0,45,700,700);
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    FinalSpectrum->SetHighLightColor(2);
    FinalSpectrum->Range(0,0,1,1);
    FinalSpectrum->SetFillColor(0);
    FinalSpectrum->SetBorderMode(0);
    FinalSpectrum->SetBorderSize(2);
    FinalSpectrum->SetFrameBorderMode(0);
    TPad **pad = new TPad*[static_cast<ULong_t>(10)];
    //0  1
    //2  3
    //4  5

    for(Int_t ipad = 0;ipad <4;ipad++){
        Double_t shift = 0.002;
        std::cout<<"padID: "<<ipad<<std::endl;
        if(ipad ==0) pad[ipad] = new TPad("pad0", "pad0",0,     0.5  ,1 ,1);
        if(ipad ==1) pad[ipad] = new TPad("pad1", "pad1",0,     0.36 ,1 ,0.5);
        if(ipad ==2) pad[ipad] = new TPad("pad2", "pad2",0,     0.22  ,1 ,0.36);
        if(ipad ==3) pad[ipad] = new TPad("pad3", "pad3",0,     0.0    ,1 ,0.2201);
        pad[ipad]->Draw();
        pad[ipad]->cd();
        if(ipad <1)pad[ipad]->Range(-1.986821e-07,-4.69897,33.33333,0.3499945);
        else pad[ipad]->Range(-1.986821e-07,-0.9209589,33.33333,2.49);
        pad[ipad]->SetFillColor(0);
        pad[ipad]->SetBorderMode(0);
        pad[ipad]->SetBorderSize(2);
        if(ipad>0)pad[ipad]->SetGridy();
        if(ipad<1)pad[ipad]->SetLogy();
        pad[ipad]->SetTickx(1);
        pad[ipad]->SetTicky(1);
        pad[ipad]->SetLeftMargin(0.15f);
        if(ipad < 3)pad[ipad]->SetBottomMargin(0);
        else pad[ipad]->SetBottomMargin(0.35f);
        if(ipad > 1)pad[ipad]->SetTopMargin(0);
        pad[ipad]->SetFrameBorderMode(0);
        //pad[ipad]->SetFrameBorderSize(2);
        //pad[ipad]->SetFrameLineWidth(2);
        FinalSpectrum->cd();
    }

    //TH1D *placeholder[8];
    TH1D **placeholder = new TH1D*[static_cast<ULong_t>(10)];

    for(Int_t ipad = 0;ipad <4;ipad++){
       // if(ipad==6 || ipad==7) continue;
        pad[ipad]->cd();
        placeholder[ipad] = new TH1D(Form("placeholder2%d",ipad),"Central Values",static_cast<Int_t>(xAxisBins), xAxis);
        if(ipad<1)placeholder[ipad]->SetMinimum(0.8);
        else placeholder[ipad]->SetMinimum(0.1);
        if(ipad<1)placeholder[ipad]->SetMaximum(1000);
        else placeholder[ipad]->SetMaximum(1.99);
        placeholder[ipad]->GetXaxis()->SetTitle("#it{p}_{T,ch jet} (GeV/#it{c})");
        placeholder[ipad]->GetXaxis()->SetLabelFont(43);
        if(ipad<3)placeholder[ipad]->GetXaxis()->SetLabelSize(0.035f);
        else placeholder[ipad]->GetXaxis()->SetLabelSize(19);
       // if(ipad<6)placeholder[ipad]->GetXaxis()->SetTitleSize(0.035f);
        //else
        placeholder[ipad]->GetXaxis()->SetTitleOffset(1.11f);
            placeholder[ipad]->GetXaxis()->SetTitleSize(0.15f);
        placeholder[ipad]->GetXaxis()->SetTitleFont(42);
        //placeholder[ipad]->GetYaxis()->SetRangeUser(plotRanges[2],plotRanges[3]);
        if(ipad<1)placeholder[ipad]->GetYaxis()->SetTitle("Ratio of cross section");
        //if(ipad<2)placeholder[ipad]->GetYaxis()->SetTitle("Ratio of probability densities");
        else if(ipad==2)placeholder[ipad]->GetYaxis()->SetTitle("MC/Data");
        placeholder[ipad]->GetYaxis()->SetLabelFont(43);
        placeholder[ipad]->GetYaxis()->SetLabelSize(19);
        placeholder[ipad]->GetYaxis()->SetTitleSize(23);
        placeholder[ipad]->GetYaxis()->SetLabelOffset(0.015f);
        placeholder[ipad]->GetYaxis()->SetTitleOffset(1.8f);
        placeholder[ipad]->GetYaxis()->SetTitleFont(43);
        placeholder[ipad]->GetYaxis()->SetDecimals();
        if(ipad>0)placeholder[ipad]->GetYaxis()->SetNdivisions(5, 5, 0, kTRUE);
        //std::cout<<placeholder[ipad]->GetYaxis()->GetTickLength()<<std::endl;
        if(ipad==3) placeholder[ipad]->GetYaxis()->SetTickLength(0.04);
        placeholder[ipad]->Draw("axis");
        //placeholder[ipad]->Draw("sameaxig");
    }

    return std::make_tuple(FinalSpectrum,pad, placeholder);

}

std::tuple<TCanvas*, TPad**, TH1D**> PrepareCanvas6Pad(UInt_t xAxisBins, Double_t *xAxisl, Double_t *xAxisr){
    style();
    //prepare main canvas
    TCanvas *FinalSpectrum = new TCanvas("FinalSpectrum3", "FinalSpectrum3",0,45,1000,700);
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    FinalSpectrum->SetHighLightColor(2);
    FinalSpectrum->Range(0,0,1,1);
    FinalSpectrum->SetFillColor(0);
    FinalSpectrum->SetBorderMode(0);
    FinalSpectrum->SetBorderSize(2);
    FinalSpectrum->SetFrameBorderMode(0);
    TPad **pad = new TPad*[static_cast<ULong_t>(6)];
    //0  1
    //2  3
    //4  5


    for(Int_t ipad = 0;ipad <6;ipad++){
        Double_t shift = 0.002;
        std::cout<<"padID: "<<ipad<<std::endl;
        if(ipad ==0) pad[ipad] = new TPad("pad0", "pad0",0,     0.5  ,0.545-shift ,1);
        if(ipad ==1) pad[ipad] = new TPad("pad1", "pad1",0.545+shift ,0.5  ,1     ,1);
        if(ipad ==2) pad[ipad] = new TPad("pad2", "pad2",0,     0.275 ,0.545-shift ,0.5);
        if(ipad ==3) pad[ipad] = new TPad("pad3", "pad3",0.545+shift ,0.275 ,1     ,0.5);
        if(ipad ==4) pad[ipad] = new TPad("pad4", "pad4",0,     0  ,0.545-shift ,0.275);
        if(ipad ==5) pad[ipad] = new TPad("pad5", "pad5",0.545+shift ,0  ,1     ,0.275);

       // if(ipad ==8) pad[ipad] = new TPad("pad8", "pad8",0,     0.05    ,0.545 ,0.2001);
       // if(ipad ==9) pad[ipad] = new TPad("pad9", "pad9",0.545 ,0.05    ,1     ,0.2001);
        pad[ipad]->Draw();
        pad[ipad]->cd();
        if(ipad <2)pad[ipad]->Range(-1.986821e-07,-4.69897,33.33333,0.3499945);
        else pad[ipad]->Range(-1.986821e-07,-0.9209589,33.33333,2.49);
        pad[ipad]->SetFillColor(0);
        pad[ipad]->SetBorderMode(0);
        pad[ipad]->SetBorderSize(2);
        if(ipad>1)pad[ipad]->SetGridy();
        pad[ipad]->SetTickx(1);
        pad[ipad]->SetTicky(1);
        if(ipad%2==0)pad[ipad]->SetLeftMargin(0.2f);
        else pad[ipad]->SetLeftMargin(0+shift+shift+shift+shift);
        if(ipad < 4)pad[ipad]->SetBottomMargin(0);
        else pad[ipad]->SetBottomMargin(0.35f);
        if(ipad > 1)pad[ipad]->SetTopMargin(0);
        if(ipad%2==0)pad[ipad]->SetRightMargin(0+shift);
        pad[ipad]->SetFrameBorderMode(0);
        //pad[ipad]->SetFrameBorderSize(2);
        //pad[ipad]->SetFrameLineWidth(2);
        FinalSpectrum->cd();
    }

    TH1D **placeholder = new TH1D*[static_cast<ULong_t>(6)];


    for(Int_t ipad = 0;ipad <6;ipad++){
       // if(ipad==6 || ipad==7) continue;
        pad[ipad]->cd();
        if(ipad%2==0)placeholder[ipad] = new TH1D(Form("placeholder%d",ipad),"Central Values",static_cast<Int_t>(xAxisBins), xAxisl);
        else placeholder[ipad] = new TH1D(Form("placeholder%d",ipad),"Central Values",static_cast<Int_t>(xAxisBins), xAxisr);
        if(ipad<2)placeholder[ipad]->SetMinimum(0.1);
        else placeholder[ipad]->SetMinimum(0.4);
        if(ipad<2)placeholder[ipad]->SetMaximum(2.0);
        else placeholder[ipad]->SetMaximum(2.3);
        placeholder[ipad]->GetXaxis()->SetTitle("#it{p}_{T,ch jet} (GeV/#it{c})");
        placeholder[ipad]->GetXaxis()->SetLabelFont(43);
        if(ipad<4)placeholder[ipad]->GetXaxis()->SetLabelSize(0.035f);
        else placeholder[ipad]->GetXaxis()->SetLabelSize(19);
       // if(ipad<6)placeholder[ipad]->GetXaxis()->SetTitleSize(0.035f);
        //else
        placeholder[ipad]->GetXaxis()->SetTitleOffset(1.11f);
            placeholder[ipad]->GetXaxis()->SetTitleSize(0.15f);
        placeholder[ipad]->GetXaxis()->SetTitleFont(42);
        //placeholder[ipad]->GetYaxis()->SetRangeUser(plotRanges[2],plotRanges[3]);
        if(ipad<2)placeholder[ipad]->GetYaxis()->SetTitle("Ratio of cross section");
        //if(ipad<2)placeholder[ipad]->GetYaxis()->SetTitle("Ratio of probability densities");
        else if(ipad ==2 || ipad==4){
            placeholder[ipad]->GetYaxis()->SetTitle("MC/Data");
            placeholder[ipad]->GetYaxis()->CenterTitle();
        }

        placeholder[ipad]->GetYaxis()->SetLabelFont(43);
        placeholder[ipad]->GetYaxis()->SetLabelSize(19);
        placeholder[ipad]->GetYaxis()->SetTitleSize(23);
        placeholder[ipad]->GetYaxis()->SetLabelOffset(0.015f);
        placeholder[ipad]->GetYaxis()->SetTitleOffset(2.5f);
        placeholder[ipad]->GetYaxis()->SetTitleFont(43);
        placeholder[ipad]->GetYaxis()->SetDecimals();
        if(ipad>1)placeholder[ipad]->GetYaxis()->SetNdivisions(5, 5, 0, kTRUE);
        //std::cout<<placeholder[ipad]->GetYaxis()->GetTickLength()<<std::endl;
        //if(ipad==6 || ipad==7) placeholder[ipad]->GetYaxis()->SetTickLength(0.04);
        placeholder[ipad]->Draw("axis");
        //placeholder[ipad]->Draw("sameaxig");
    }

    return std::make_tuple(FinalSpectrum,pad, placeholder);

}

std::tuple<TCanvas*, TPad*, TPad*,  TPad*, TH1D*, TH1D*, TH1D*> PrepareCanvas(UInt_t xAxisBins, Double_t *xAxis){
    style();
    //prepare main canvas
    TCanvas *FinalSpectrum = new TCanvas("FinalSpectrum2", "FinalSpectrum2",0,45,700,700);
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    FinalSpectrum->SetHighLightColor(2);
    FinalSpectrum->Range(0,0,1,1);
    FinalSpectrum->SetFillColor(0);
    FinalSpectrum->SetBorderMode(0);
    FinalSpectrum->SetBorderSize(2);
    FinalSpectrum->SetFrameBorderMode(0);

    //Set primitives in upper pad
    TPad *pad_top = new TPad("pad_top", "pad_top",0,0.5,1,1);
    pad_top->Draw();
    pad_top->cd();
    pad_top->Range(-1.986821e-07,-4.69897,33.33333,0.3499945);
    pad_top->SetFillColor(0);
    pad_top->SetBorderMode(0);
    pad_top->SetBorderSize(2);
//    if(zBin==0)pad_top->SetLogy();
    pad_top->SetTickx(1);
    pad_top->SetTicky(1);
    pad_top->SetLeftMargin(0.18f);
    pad_top->SetBottomMargin(0);
    pad_top->SetFrameBorderMode(0);
    pad_top->SetFrameBorderMode(0);

    FinalSpectrum->cd();



    //Set primitives in bottom pad
    TPad *pad_bottom = new TPad("pad_bottom", "pad_bottom",0,0.275,1,0.5);
    pad_bottom->Draw();
    pad_bottom->cd();
    pad_bottom->Range(-1.986821e-07,-0.9209589,33.33333,2.49);
    pad_bottom->SetFillColor(0);
    pad_bottom->SetBorderMode(0);
    pad_bottom->SetBorderSize(2);
    pad_bottom->SetGridy();
    pad_bottom->SetTickx(1);
    pad_bottom->SetTicky(1);
    pad_bottom->SetLeftMargin(0.18f);
    pad_bottom->SetTopMargin(0);
    pad_bottom->SetBottomMargin(0);
    pad_bottom->SetFrameBorderMode(0);
    pad_bottom->SetFrameBorderMode(0);

    FinalSpectrum->cd();

    //Set primitives in bottom pad
    TPad *pad_bottom2 = new TPad("pad_bottom2", "pad_bottom2",0,0,1,0.275);
    pad_bottom2->Draw();
    pad_bottom2->cd();
    pad_bottom2->Range(-1.986821e-07,-0.9209589,33.33333,2.49);
    pad_bottom2->SetFillColor(0);
    pad_bottom2->SetBorderMode(0);
    pad_bottom2->SetBorderSize(2);
    pad_bottom2->SetGridy();
    pad_bottom2->SetTickx(1);
    pad_bottom2->SetTicky(1);
    pad_bottom2->SetLeftMargin(0.18f);
    pad_bottom2->SetTopMargin(0);
    pad_bottom2->SetBottomMargin(0.27f);
    pad_bottom2->SetFrameBorderMode(0);
    pad_bottom2->SetFrameBorderMode(0);

    pad_top->cd();
    TH1D *hEmpty_up = new TH1D("hEmpty_up","Central Values",static_cast<Int_t>(xAxisBins), xAxis);
    hEmpty_up->SetMinimum(plotRanges[2]);
    hEmpty_up->SetMaximum(plotRanges[3]);
    hEmpty_up->GetXaxis()->SetTitle("#it{p}_{T,ch jet} (GeV/#it{c})");
    hEmpty_up->GetXaxis()->SetLabelFont(43);
    hEmpty_up->GetXaxis()->SetLabelSize(0.035f);
    hEmpty_up->GetXaxis()->SetTitleSize(0.035f);
    hEmpty_up->GetXaxis()->SetTitleFont(42);
    hEmpty_up->GetYaxis()->SetRangeUser(plotRanges[2],plotRanges[3]);
  //  hEmpty_up->GetYaxis()->SetTitle("Ratio of cross-sections");
    hEmpty_up->GetYaxis()->SetTitle("Ratio of probability densities");
 //   if(zBin ==2 && !pdf)hEmpty_up->GetYaxis()->SetTitle("#frac{d^{2}#it{#sigma}}{d#it{p}_{T}d#it{#eta}} (mb#upointGeV^{#minus1}#upoint#it{c})");
 //   else if(!pdf)hEmpty_up->GetYaxis()->SetTitle("#frac{d^{2}#it{#sigma}}{d#it{z}_{#parallel}d#it{#eta}} (mb)");
 //   else hEmpty_up->GetYaxis()->SetTitle("Probability density");
    hEmpty_up->GetYaxis()->SetLabelFont(43);
    hEmpty_up->GetYaxis()->SetLabelSize(19);
    hEmpty_up->GetYaxis()->SetTitleSize(26);
    hEmpty_up->GetYaxis()->SetLabelOffset(0.015f);
    hEmpty_up->GetYaxis()->SetTitleOffset(2.f);
    hEmpty_up->GetYaxis()->SetTitleFont(43);
    hEmpty_up->GetYaxis()->SetDecimals();
    hEmpty_up->Draw("axis");
    pad_bottom->cd();
    TH1D *hEmpty_bottom = new TH1D("hEmpty_bottom","Central Values",static_cast<Int_t>(xAxisBins), xAxis);
    hEmpty_bottom->SetMinimum(0.4);
    hEmpty_bottom->SetMaximum(plotRanges[1]);
   // if(zBin ==0)hEmpty_bottom->GetXaxis()->SetTitle("#it{p}_{T,ch jet} (GeV/#it{c})");
   // else hEmpty_bottom->GetXaxis()->SetTitle("z_{#parallel}");
    hEmpty_bottom->GetXaxis()->SetLabelFont(43);
    hEmpty_bottom->GetXaxis()->SetLabelSize(19);
    hEmpty_bottom->GetXaxis()->SetTitleSize(26);
    hEmpty_bottom->GetXaxis()->SetTitleOffset(3.f);
    hEmpty_bottom->GetXaxis()->SetTitleFont(43);
    hEmpty_bottom->GetYaxis()->SetTitle("MC/Data");
    hEmpty_bottom->GetYaxis()->SetNdivisions(509);
    hEmpty_bottom->GetYaxis()->CenterTitle();
    hEmpty_bottom->GetYaxis()->SetDecimals();
    hEmpty_bottom->GetYaxis()->SetLabelOffset(0.015f);
    hEmpty_bottom->GetXaxis()->SetLabelOffset(0.02f);
    hEmpty_bottom->GetYaxis()->SetLabelFont(43);
    hEmpty_bottom->GetYaxis()->SetLabelSize(19);
    hEmpty_bottom->GetYaxis()->SetTitleSize(26);
    hEmpty_bottom->GetYaxis()->SetTitleOffset(2.f);
    hEmpty_bottom->GetYaxis()->SetTitleFont(43);
    hEmpty_bottom->Draw("axis");

    pad_bottom2->cd();
    TH1D *hEmpty_bottom2 = new TH1D("hEmpty_bottom2","Central Values",static_cast<Int_t>(xAxisBins), xAxis);
    hEmpty_bottom2->SetMinimum(plotRanges[0]);
    hEmpty_bottom2->SetMaximum(1.6);
    hEmpty_bottom2->GetXaxis()->SetTitle("#it{p}_{T,ch jet} (GeV/#it{c})");
    //else hEmpty_bottom->GetXaxis()->SetTitle("z_{#parallel}");
    hEmpty_bottom2->GetXaxis()->SetLabelFont(43);
    hEmpty_bottom2->GetXaxis()->SetLabelSize(19);
    hEmpty_bottom2->GetXaxis()->SetTitleSize(26);
    hEmpty_bottom2->GetXaxis()->SetTitleOffset(3.f);
    hEmpty_bottom2->GetXaxis()->SetTitleFont(43);
    hEmpty_bottom2->GetYaxis()->SetTitle("MC/Data");
    hEmpty_bottom2->GetYaxis()->SetNdivisions(509);
    hEmpty_bottom2->GetYaxis()->CenterTitle();
    hEmpty_bottom2->GetYaxis()->SetDecimals();
    hEmpty_bottom2->GetYaxis()->SetLabelOffset(0.015f);
    hEmpty_bottom2->GetXaxis()->SetLabelOffset(0.02f);
    hEmpty_bottom2->GetYaxis()->SetLabelFont(43);
    hEmpty_bottom2->GetYaxis()->SetLabelSize(19);
    hEmpty_bottom2->GetYaxis()->SetTitleSize(26);
    hEmpty_bottom2->GetYaxis()->SetTitleOffset(2.f);
    hEmpty_bottom2->GetYaxis()->SetTitleFont(43);
    hEmpty_bottom2->Draw("axis");

    return std::make_tuple(FinalSpectrum,pad_top, pad_bottom, pad_bottom2, hEmpty_up,hEmpty_bottom,hEmpty_bottom2);

}


std::tuple<TGraphAsymmErrors*, TH1D*,TGraphAsymmErrors*, TH1D*> GetData(TString dataFile, TString histBase, Double_t dataScaling, UInt_t nBins, Double_t *xBins, Double_t *systUncD_down, Double_t *systUncD_up){
    std::cout<<"get DATA"<<std::endl;
    TH1D *htmp = dynamic_cast<TH1D*>(GetInputHist(dataFile, histBase));
    std::cout<<"getting "<<dataFile<<" "<<histBase<<std::endl;
    if(!htmp)std::cout<<"not histo"<<std::endl;
    TH1D *hData_binned = dynamic_cast<TH1D*>(htmp->Rebin(static_cast<Int_t>(nBins),"hData_binned", xBins));
    //Double_t data_int = hData_binned->Integral();

    if(pdf){
      hData_binned->Scale(1./hData_binned->Integral());
      hData_binned->Scale(1,"width");
    }else{
        hData_binned->Scale(1,"width");
        hData_binned->Scale(dataScaling);
        hData_binned->Scale(1./dy);
    }

    hData_binned->SetTitle("");
    hData_binned->SetMaximum(hData_binned->GetMaximum()*2);
    //hData_binned->GetYaxis()->SetTitle("d^{2}#sigma/dp_{T}d#it{#eta} (mb)");
    if(!pdf)hData_binned->GetYaxis()->SetTitle("#frac{d^{2}#sigma}{d#it{p}_{T}d#it{#eta}} mb (GeV/#it{c})^{-1}");
    else hData_binned->GetYaxis()->SetTitle("Probability density");

    Double_t *sysuncAbs_down = new Double_t[nBins];
    Double_t *sysuncAbs_up = new Double_t[nBins];
    Double_t *statunc = new Double_t[nBins];
    Double_t *value = new Double_t[nBins];
    Double_t *xval = new Double_t[nBins];
    Double_t *xvalwidth = new Double_t[nBins];
    for(UInt_t j=0; j<nBins; j++){
          xval[j] = (xBins[j]+xBins[j+1]) / 2.;
          xvalwidth[j] = (xBins[j+1]-xBins[j]) / 2.;
          value[j] = hData_binned->GetBinContent(hData_binned->GetXaxis()->FindBin(xval[j]));
          Double_t error = hData_binned->GetBinError(hData_binned->GetXaxis()->FindBin(xval[j]));
          std::cout<<j<<" "<<xval[j]<<" "<<xvalwidth[j]<<" "<<value[j]<<" "<<error<<std::endl;
          sysuncAbs_down[j] = value[j] * systUncD_down[j];
          sysuncAbs_up[j] = value[j] * systUncD_up[j];
          if(value[j]>1E-9)statunc[j] = error/ value[j] *100;
    }
    TGraphAsymmErrors *grsysl = new TGraphAsymmErrors(static_cast<Int_t>(nBins),xval,value,xvalwidth,xvalwidth,sysuncAbs_down,sysuncAbs_up);
    grsysl->SetName("haeData_binned_syst");

    TH1D *hData_binned_ratio = dynamic_cast<TH1D*>(hData_binned->Clone("hData_binned_ratio"));
      double *sysuncRatio_up = new double[nBins];
      double *sysuncRatio_down = new double[nBins];
      double *valRatio = new double[nBins];
      for(UInt_t j=0; j<nBins; j++){
          xval[j] = (xBins[j]+xBins[j+1]) / 2.;
          xvalwidth[j] = (xBins[j+1]-xBins[j]) / 2.;
              double valPred = hData_binned->GetBinContent(hData_binned->GetXaxis()->FindBin(xval[j]));
              if(valPred<1E-9)valPred=1000;
              if(valPred>1E-9)valRatio[j] = 1.0;
              double err = 0;
              if(valPred>1E-9) err=hData_binned->GetBinError(hData_binned->GetXaxis()->FindBin(xval[j])) / valPred;

              sysuncRatio_down[j] = valRatio[j] * systUncD_down[j];
              sysuncRatio_up[j] = valRatio[j] * systUncD_up[j];

              hData_binned_ratio->SetBinContent(hData_binned_ratio->GetXaxis()->FindBin(xval[j]),valRatio[j]);
              hData_binned_ratio->SetBinError(hData_binned_ratio->GetXaxis()->FindBin(xval[j]),err);
      }
      TGraphAsymmErrors *grsysRatio = new TGraphAsymmErrors(static_cast<Int_t>(nBins),xval,valRatio,xvalwidth,xvalwidth,sysuncRatio_down,sysuncRatio_up);
      grsysRatio->SetName("haeData_binned_syst_ratio");
    return std::make_tuple(grsysl, hData_binned, grsysRatio,hData_binned_ratio);
}

TGraphAsymmErrors* Divide(TGraphAsymmErrors *hist1,TH1D *hist2){
    TString name1 = hist1->GetName();
    TString name2 = hist2->GetName();

    UInt_t nBin = hist1->GetN();
    Double_t *xval = new Double_t[nBin];
    Double_t *xvalwidth_down = new Double_t[nBin];
    Double_t *xvalwidth_up = new Double_t[nBin];
    Double_t *value = new Double_t[nBin];
    Double_t *valuetheoryerrup = new Double_t[nBin];
    Double_t *valuetheoryerrdown = new Double_t[nBin];
    for(UInt_t bin=0; bin<nBin; bin++){
        Double_t x1 = 0,y1=0;
        hist1->GetPoint(bin,x1,y1);
        xval[bin] = x1;
        xvalwidth_down[bin] = hist1->GetErrorXlow(bin);
        xvalwidth_up[bin] = hist1->GetErrorXhigh(bin);
        value[bin] = y1/hist2->GetBinContent(bin+1);
        std::cout<<"DIVIDE: "<<y1<<" "<<hist2->GetBinContent(bin+1)<<" "<<value[bin]<<std::endl;
        valuetheoryerrup[bin] = hist1->GetErrorYhigh(bin)/hist2->GetBinContent(bin+1);
        valuetheoryerrdown[bin] = hist1->GetErrorYlow(bin)/hist2->GetBinContent(bin+1);
    }
    TGraphAsymmErrors *grsystheory = new TGraphAsymmErrors(static_cast<Int_t>(nBin),xval,value,xvalwidth_down,xvalwidth_up,valuetheoryerrdown,valuetheoryerrup);
    grsystheory->SetName(name1+"_"+name2+"_ratio_sys");
    return grsystheory;
}


std::tuple<TGraphAsymmErrors*, TGraphAsymmErrors*, TH1D**> GetSim(TString dataFile,TString simname, Int_t nVar){
    TH1D **hPrompt;
    if(nVar>0)hPrompt= new TH1D*[static_cast<ULong_t>(nVar)];
    TFile *jetPtFile = new TFile(dataFile,"read");
    TString name = "";
     TGraphAsymmErrors *h1 = dynamic_cast<TGraphAsymmErrors*>(jetPtFile->Get(name+"haesim"+simname));
     TGraphAsymmErrors *h2 = dynamic_cast<TGraphAsymmErrors*>(jetPtFile->Get(name+"haesim"+simname+"_ratio"));
     for (int nr=0; nr<nVar; nr++){
         hPrompt[nr]= dynamic_cast<TH1D*>(jetPtFile->Get(name+"hsim"+simname+Form("_var%d",nr+1)));
     }
     return std::make_tuple(h1, h2,hPrompt);
}

std::tuple<TGraphAsymmErrors*, TH1D*, TH1D*, TH1D*> GetDataSimRatio(TString simname, TH1D *data_cent, TH1D *sim_cent, TH1D *sim_up, TH1D *sim_down, UInt_t nBins, Double_t *xBins){
    TString name = sim_cent->GetName();
    TString name_up = sim_up->GetName();
    TString name_down = sim_down->GetName();
    TH1D *hPrompt_central_binned_ratio = dynamic_cast<TH1D*>(sim_cent->Clone(name+"_ratio"));
    hPrompt_central_binned_ratio->Divide(data_cent);
    TH1D *hPrompt_down_ratio = dynamic_cast<TH1D*>(sim_down->Clone(name_down+"_ratio"));
    TH1D *hPrompt_up_ratio = dynamic_cast<TH1D*>(sim_up->Clone(name_up+"_ratio"));
    hPrompt_up_ratio->Divide(data_cent);
    hPrompt_down_ratio->Divide(data_cent);

    Double_t *ptvaltheoryratio = new Double_t[nBins];
    Double_t *ptvalunctheoryratio = new Double_t[nBins];
    Double_t *valuetheoryratio = new Double_t[nBins];
    Double_t *valuetheoryerrupratio = new Double_t[nBins];
    Double_t *valuetheoryerrdownratio = new  Double_t[nBins];
    for(UInt_t j=0; j<nBins; j++){
          ptvaltheoryratio[j] = (xBins[j]+xBins[j+1]) / 2.;
          ptvalunctheoryratio[j] = (xBins[j+1]-xBins[j]) / 2.;
          valuetheoryratio[j] = hPrompt_central_binned_ratio->GetBinContent(hPrompt_central_binned_ratio->GetXaxis()->FindBin(ptvaltheoryratio[j]));
          valuetheoryerrupratio[j] = hPrompt_up_ratio->GetBinContent(hPrompt_up_ratio->GetXaxis()->FindBin(ptvaltheoryratio[j])) - valuetheoryratio[j];
          valuetheoryerrdownratio[j] = valuetheoryratio[j] - hPrompt_down_ratio->GetBinContent(hPrompt_down_ratio->GetXaxis()->FindBin(ptvaltheoryratio[j]));
    }
    TGraphAsymmErrors *grsystheoryratio = new TGraphAsymmErrors(static_cast<Int_t>(nBins),ptvaltheoryratio,valuetheoryratio,ptvalunctheoryratio,ptvalunctheoryratio,valuetheoryerrdownratio,valuetheoryerrupratio);
    TString tmp = "hae";
    tmp+=simname;
    grsystheoryratio->SetName(tmp+"_ratio");
    return std::make_tuple(grsystheoryratio, hPrompt_central_binned_ratio, hPrompt_up_ratio,hPrompt_down_ratio);
}
TH1D* GetRatio(TH1D *h1, TH1D *h2){
    TString name1 = h1->GetName();
    TString name2 = h1->GetName();
    TH1D *hRatio = dynamic_cast<TH1D*>(h1->Clone(name1+"_"+name2+"_ratio"));
    hRatio->Divide(h2);
    return hRatio;
}

TH1D* GetRatio(TH1D *h1, TH1D *h2, Double_t shift){
    TString name1 = h1->GetName();
    TString name2 = h1->GetName();
    TH1D *hRatio = dynamic_cast<TH1D*>(h1->Clone(name1+"_"+name2+"_ratio"));
    hRatio->Divide(h2);
    for(Int_t bin = 1; bin <= hRatio->GetNbinsX();bin++){
        hRatio->SetBinContent(bin,hRatio->GetBinContent(bin)+shift);
    }

    return hRatio;
}

/*
std::tuple<TH1D*, TH1D*, TH1D*> GetRatio(TH1D *sim_cent, TH1D *sim_up, TH1D *sim_down,TH1D *sim2_cent, TH1D *sim2_up, TH1D *sim2_down){
    std::cout<<"A"<<std::endl;
    TString name = sim_cent->GetName();
    TString name_up = sim_up->GetName();
    TString name_down = sim_down->GetName();
    TString name2 = sim2_cent->GetName();
    TString name2_up = sim2_up->GetName();
    TString name2_down = sim2_down->GetName();
    std::cout<<"A"<<std::endl;
    TH1D *hPrompt_central_binned_ratio = dynamic_cast<TH1D*>(sim_cent->Clone(name+name2+"_ratio"));
    TH1D *hPrompt_down_ratio = dynamic_cast<TH1D*>(sim_down->Clone(name_down+name2_down+"_ratio"));
    TH1D *hPrompt_up_ratio = dynamic_cast<TH1D*>(sim_up->Clone(name_up+name2_up+"_ratio"));
    std::cout<<"A"<<std::endl;
    hPrompt_central_binned_ratio->Divide(sim2_cent);
    hPrompt_down_ratio->Divide(sim2_down);
    hPrompt_up_ratio->Divide(sim2_up);
    std::cout<<"A"<<std::endl;
    return std::make_tuple(hPrompt_central_binned_ratio,hPrompt_up_ratio,hPrompt_down_ratio);

}
*/
TH1D* GetUpSys(TH1D *central, TH1D **hh, Int_t nFiles){
    double max = 0, maxerr = 0;
    TString name = central->GetName();
    TH1D *hh_up = dynamic_cast<TH1D*>(central->Clone(name +"_up"));

    for(int iBin=1; iBin<central->GetNbinsX()+1; iBin++ ){
      //  std::cout<<"iBin: "<<iBin<<std::endl;
        max = central->GetBinContent(iBin);
        for(Int_t iFile=1; iFile < nFiles; iFile++){
           // std::cout<<"iFile: "<<iFile<<" nfiles "<<nFiles<<" binc "<<hh[iFile]->GetBinContent(iBin)<<std::endl;
            if(hh[iFile]->GetBinContent(iBin) > max){
                    max = hh[iFile]->GetBinContent(iBin);
                    maxerr = hh[iFile]->GetBinError(iBin);
            }
        }
        hh_up->SetBinContent(iBin,max);
        hh_up->SetBinError(iBin,0);
    }

    return hh_up;
}

TH1D* GetDownSys(TH1D *central, TH1D **hh, Int_t nFiles){
    double max = 0, maxerr = 0;
    TString name = central->GetName();
    TH1D *hh_down = dynamic_cast<TH1D*>(central->Clone(name +"_down"));

    for(int iBin=1; iBin<central->GetNbinsX()+1; iBin++ ){
        max = central->GetBinContent(iBin);
        for(Int_t iFile=1; iFile < nFiles; iFile++){
            if(hh[iFile]->GetBinContent(iBin) < max){
                    max = hh[iFile]->GetBinContent(iBin);
                    maxerr = hh[iFile]->GetBinError(iBin);
            }
        }
        hh_down->SetBinContent(iBin,max);
        hh_down->SetBinError(iBin,0);
    }

    return hh_down;
}

TH1* GetInputHist(TString inFile, TString histName){
    TFile *jetPtFile = new TFile(inFile,"read");
    TH1 *hh = dynamic_cast<TH1*>(jetPtFile->Get(histName.Data()));
    return hh;
}

TGraphAsymmErrors* GetInputGraph(TString inFile, TString histName){
    TFile *jetPtFile = new TFile(inFile,"read");
    TGraphAsymmErrors *hh = dynamic_cast<TGraphAsymmErrors*>(jetPtFile->Get(histName.Data()));
    return hh;
}

std::tuple<TH1D*, TGraphAsymmErrors*, TH1D*,  TGraphAsymmErrors*> GetData(TString dataFile, TString histBase1, TString histBase2, TString histBase3, TString histBase4){
    TFile *jetPtFile = new TFile(dataFile,"read");
    TH1D *h1 = dynamic_cast<TH1D*>(jetPtFile->Get(histBase1.Data()));
     TGraphAsymmErrors *h2 = dynamic_cast<TGraphAsymmErrors*>(jetPtFile->Get(histBase2.Data()));
    TH1D *h3 = dynamic_cast<TH1D*>(jetPtFile->Get(histBase3.Data()));
     TGraphAsymmErrors *h4 = dynamic_cast<TGraphAsymmErrors*>(jetPtFile->Get(histBase4.Data()));
     return std::make_tuple(h1, h2, h3,h4);
}

void setHistoDetails(TH1 *hh, Color_t color, Style_t Mstyle, Size_t Msize, Width_t Lwidth, Style_t Lstyle){
    hh->SetMarkerColor(color);
    hh->SetMarkerStyle(Mstyle);;
    hh->SetLineColor(color);
    hh->SetLineWidth(Lwidth);
    hh->SetMarkerSize(Msize);
    hh->SetLineStyle(Lstyle);
    hh->SetTitle("");
    hh->GetXaxis()->SetTitle("p_{T}^{ch,jet} (GeV/c)");
}

void SaveCanvas(TCanvas *c, TString name){
    c->SaveAs(Form("%s.png",name.Data()));
    c->SaveAs(Form("%s.pdf",name.Data()));
}
