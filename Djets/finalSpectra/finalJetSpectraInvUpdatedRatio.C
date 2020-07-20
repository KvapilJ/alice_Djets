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
TH1D* GetUpSys(TH1D **hh, Int_t nFiles = 0);
TH1D* GetDownSys(TH1D **hh, Int_t nFiles = 0);
TH1* GetInputHist(TString inFile, TString histName);
void ScaleHist(TH1 *hh, int full);
void drawFinal(TString outPlotDir);
TH1D* GetRatio(TH1D *h1, TH1D *h2);
TH1D* GetRatio(TH1D *h1, TH1D *h2, Double_t shift);

/*
static TString fFiles[9] = {
    "/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_X02/Default_AnalysisResults_Run2w18b.root/unfolding_Bayes_5/finalSpectraNEW/JetPtSpectrum_final.root",
    "/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_X04/Default_AnalysisResults_Run2w18b.root/unfolding_Bayes_5/finalSpectraNEW/JetPtSpectrum_final.root",
    "/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_X06/Default_AnalysisResults_Run2w18b.root/unfolding_Bayes_5/finalSpectraNEW/JetPtSpectrum_final.root",
    };
*/
static TString fFiles[9] = {
    "/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_Z02/Default_AnalysisResults_Run2w18b.root/unfolding_2D_5/finalSpectraNEW25/JetPtSpectrum_final.root",
    "/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_04zF/Default_AnalysisResults_Run2w18b.root/unfolding_2D_5/finalSpectraNEW25/JetPtSpectrum_final.root",
    "/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_Z06/Default_AnalysisResults_Run2w18b.root/unfolding_2D_5/finalSpectraNEW25/JetPtSpectrum_final.root",
    };


std::tuple<TGraphAsymmErrors*, TH1D*, TH1D*, TH1D*> GetSim(TString simname, Int_t type, Int_t nFiles, TString *names, TString simDir, Double_t simScaling, UInt_t nBins, Double_t *xBins);
std::tuple<TGraphAsymmErrors*, TH1D*, TH1D*, TH1D*> GetDataSimRatio(TString simname, TH1D *data_cent, TH1D *sim_cent, TH1D *sim_up, TH1D *sim_down, UInt_t nBins, Double_t *xBins);
std::tuple<TGraphAsymmErrors*, TH1D*,TGraphAsymmErrors*, TH1D*> GetData(TString dataFile, TString histBase, Double_t dataScaling, UInt_t nBins, Double_t *xBins, Double_t *systUncD_down, Double_t *systUncD_up);
std::tuple<TCanvas*, TPad*, TPad*,  TPad*, TH1D*, TH1D*, TH1D*> PrepareCanvas(UInt_t xAxisBins, Double_t *xAxis);
void PlaceOnPadData(TPad* pad,TGraphAsymmErrors* histo1, TH1D* histo2, Size_t markersize);
void PlaceOnPadSim(TPad* pad, TGraphAsymmErrors* histo, Color_t ci, Style_t style, Style_t linestyle);
void PlaceOnPadSim(TPad* pad, TH1D* histo, Color_t ci, Style_t style, Style_t linestyle);
void TerminateCanvas(TPad* pad1,TPad* pad2,TPad* pad3,TH1D* histo1,TH1D* histo2,TH1D* histo3);

std::tuple<TH1D*, TH1D*, TH1D*> GetRatio(TH1D *sim_cent, TH1D *sim_up, TH1D *sim_down,TH1D *sim2_cent, TH1D *sim2_up, TH1D *sim2_down);


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

  //  UInt_t xAxisBins = 8;
//    Double_t *xAxis = new Double_t[xAxisBins+1]{5,6,8,10,12,14,20,30,50};
    UInt_t xAxisBins = 5;
    Double_t *xAxis = new Double_t[xAxisBins+1]{0.4,0.6,0.7,0.8,0.9,1.0};
   // plotRanges[0]=0; plotRanges[1]=2.1; plotRanges[2]=0.1; plotRanges[3]=3.5;
     plotRanges[0]=0.4; plotRanges[1]=1.6; plotRanges[2]=0.0; plotRanges[3]=5;


    // ----------------- Initiate Canvas ---------------------
    TCanvas *canvas;
    TPad *upPad, *dowmPad, *dowmPad2;
    TH1D *placeholder_up, *placeholder_down, *placeholder_down2;
    std::tie(canvas,upPad,dowmPad,dowmPad2,placeholder_up,placeholder_down,placeholder_down2) = PrepareCanvas(xAxisBins,xAxis);

    // ----------------- data ---------------------
  //  TGraphAsymmErrors *hDataSys, *hDataSysRatio;
    TH1D *hData_binned2,*hData_binned4,*hData_binned6, *hData_binned_ratio;
 //   std::tie(hDataSys, hData_binned, hDataSysRatio, hData_binned_ratio) = GetData(dataFile, histBase, dataScaling, xAxisBins, xAxis, systUncD_down, systUncD_up);
   hData_binned2 = dynamic_cast<TH1D*>(GetInputHist(fFiles[0],"hData_binned"));
   hData_binned4 = dynamic_cast<TH1D*>(GetInputHist(fFiles[1],"hData_binned"));
   hData_binned6 = dynamic_cast<TH1D*>(GetInputHist(fFiles[2],"hData_binned"));

   TH1D *hDataR26 = dynamic_cast<TH1D*>(hData_binned2->Clone("hDataR26"));
   hDataR26->Divide(hData_binned6);
   hDataR26->SetLineColor(static_cast<Color_t>(TColor::GetColor("#990000")));
   hDataR26->SetMarkerColor(static_cast<Color_t>(TColor::GetColor("#990000")));
   TH1D *hDataR46 = dynamic_cast<TH1D*>(hData_binned4->Clone("hDataR46"));
   hDataR46->Divide(hData_binned6);
   hDataR46->SetLineColor(static_cast<Color_t>(TColor::GetColor("#999900")));
   hDataR46->SetMarkerColor(static_cast<Color_t>(TColor::GetColor("#999900")));
   hDataR46->SetMarkerStyle(22);

   TH1D *hData_binned2R,*hData_binned4R,*hData_binned6R;
//   std::tie(hDataSys, hData_binned, hDataSysRatio, hData_binned_ratio) = GetData(dataFile, histBase, dataScaling, xAxisBins, xAxis, systUncD_down, systUncD_up);
  hData_binned2R = dynamic_cast<TH1D*>(GetInputHist(fFiles[0],"hData_binned_ratio"));
  hData_binned4R = dynamic_cast<TH1D*>(GetInputHist(fFiles[1],"hData_binned_ratio"));
  hData_binned6R = dynamic_cast<TH1D*>(GetInputHist(fFiles[2],"hData_binned_ratio"));

  TH1D *hDataR26R = dynamic_cast<TH1D*>(hData_binned2R->Clone("hDataR26R"));
  hDataR26R->Divide(hData_binned6R);
  hDataR26R->SetLineColor(static_cast<Color_t>(TColor::GetColor("#990000")));
  hDataR26R->SetMarkerColor(static_cast<Color_t>(TColor::GetColor("#990000")));
  TH1D *hDataR46R = dynamic_cast<TH1D*>(hData_binned4R->Clone("hDataR46R"));
  hDataR46R->Divide(hData_binned6R);
  hDataR46R->SetLineColor(static_cast<Color_t>(TColor::GetColor("#999900")));
  hDataR46R->SetMarkerColor(static_cast<Color_t>(TColor::GetColor("#999900")));
  hDataR46R->SetMarkerStyle(22);

   for(Int_t bin = 1; bin <= hDataR46->GetNbinsX();bin++){
       hDataR46->SetBinContent(bin,hDataR46->GetBinContent(bin)+0.5);
   }


   hDataR26->SetMinimum(plotRanges[2]);
   hDataR26->SetMaximum(plotRanges[3]);
   hDataR26->GetYaxis()->SetRangeUser(plotRanges[2],plotRanges[3]);
   hDataR46->SetMinimum(plotRanges[2]);
   hDataR46->SetMaximum(plotRanges[3]);
   hDataR46->GetYaxis()->SetRangeUser(plotRanges[2],plotRanges[3]);
   hDataR26->SetMarkerStyle(20);
   upPad->cd();
    hDataR26->Draw("same p");
    hDataR46->Draw("same p");

/*
   TH1D *simPowhegPythia6_cent26,*simPowhegPythia6_up26,*simPowhegPythia6_down26;
  // TH1D *simPowhegPythia6_cent46,*simPowhegPythia6_up46,*simPowhegPythia6_down46;
   TH1D *simPowhegPythia6_cent2,*simPowhegPythia6_up2,*simPowhegPythia6_down2;
  // TH1D *simPowhegPythia6_cent4,*simPowhegPythia6_up4,*simPowhegPythia6_down4;
   TH1D *simPowhegPythia6_cent6,*simPowhegPythia6_up6,*simPowhegPythia6_down6;
   simPowhegPythia6_cent2 = dynamic_cast<TH1D*>(GetInputHist(fFiles[0],"hsimPowhegPythia8_central"));
   if(!simPowhegPythia6_cent2) std::cout<<"no "<<std::endl;
   simPowhegPythia6_up2 = dynamic_cast<TH1D*>(GetInputHist(fFiles[0],"hsimPowhegPythia8_up"));
   if(!simPowhegPythia6_up2) std::cout<<"no2 "<<std::endl;
   simPowhegPythia6_down2 = dynamic_cast<TH1D*>(GetInputHist(fFiles[0],"hsimPowhegPythia8_down"));
   if(!simPowhegPythia6_down2) std::cout<<"no3 "<<std::endl;
 //  simPowhegPythia6_cent4 = dynamic_cast<TH1D*>(GetInputHist(fFiles[1],"hsimPowhegPythia8_central"));
 //  simPowhegPythia6_up4 = dynamic_cast<TH1D*>(GetInputHist(fFiles[1],"hsimPowhegPythia8_up"));
 //  simPowhegPythia6_down4 = dynamic_cast<TH1D*>(GetInputHist(fFiles[1],"hsimPowhegPythia8_down"));
   simPowhegPythia6_cent6 = dynamic_cast<TH1D*>(GetInputHist(fFiles[2],"hsimPowhegPythia8_central"));
   simPowhegPythia6_up6 = dynamic_cast<TH1D*>(GetInputHist(fFiles[2],"hsimPowhegPythia8_up"));
  simPowhegPythia6_down6 = dynamic_cast<TH1D*>(GetInputHist(fFiles[2],"hsimPowhegPythia8_down"));
  if(!simPowhegPythia6_cent6) std::cout<<"no "<<std::endl;
  if(!simPowhegPythia6_up6) std::cout<<"no "<<std::endl;
  if(!simPowhegPythia6_down6) std::cout<<"no "<<std::endl;
   std::cout<<"here"<<std::endl;
   std::tie(simPowhegPythia6_cent26, simPowhegPythia6_up26, simPowhegPythia6_down26) = GetRatio(simPowhegPythia6_cent2,simPowhegPythia6_up2,simPowhegPythia6_down2,simPowhegPythia6_cent6,simPowhegPythia6_up6,simPowhegPythia6_down6);
  // std::tie(simPowhegPythia6_cent46, simPowhegPythia6_up46, simPowhegPythia6_down46) = GetRatio(simPowhegPythia6_cent4,simPowhegPythia6_up4,simPowhegPythia6_down4,simPowhegPythia6_cent6,simPowhegPythia6_up6,simPowhegPythia6_down6);
   upPad->cd();
   simPowhegPythia6_up26->SetMarkerStyle(22);
   simPowhegPythia6_up26->SetMarkerSize(2);
   simPowhegPythia6_down26->SetMarkerStyle(23);
    //simPowhegPythia6_cent26->Draw("same p");
    simPowhegPythia6_up26->Draw("same P");
    simPowhegPythia6_down26->Draw("same P");
   // simPowhegPythia6_cent46->Draw("same p");
   // simPowhegPythia6_up46->Draw("same l");
   // simPowhegPythia6_down46->Draw("same l");
    std::cout<<simPowhegPythia6_cent26->GetName()<<std::endl;
    std::cout<<simPowhegPythia6_up26->GetName()<<std::endl;
    std::cout<<simPowhegPythia6_down26->GetName()<<std::endl;
    for(Int_t i = 1; i< simPowhegPythia6_up26->GetNbinsX();i++){
        std::cout<<simPowhegPythia6_cent26->GetBinContent(i)<<" "<<simPowhegPythia6_up26->GetBinContent(i)<<" "<<simPowhegPythia6_down26->GetBinContent(i)<<std::endl;
    }*/

    TH1D *simPowhegPythia2,*simPowhegPythia4,*simPowhegPythia6;
    TH1D *simPowhegPythia26,*simPowhegPythia46;
    simPowhegPythia2 = dynamic_cast<TH1D*>(GetInputHist(fFiles[0],"hsimPowhegPythia8_central"));
    simPowhegPythia4 = dynamic_cast<TH1D*>(GetInputHist(fFiles[1],"hsimPowhegPythia8_central"));
    simPowhegPythia6 = dynamic_cast<TH1D*>(GetInputHist(fFiles[2],"hsimPowhegPythia8_central"));
    simPowhegPythia26 = GetRatio(simPowhegPythia2,simPowhegPythia6);
    simPowhegPythia46 = GetRatio(simPowhegPythia4,simPowhegPythia6,0.5);

    TH1D *simPythia62,*simPythia64,*simPythia66;
    TH1D *simPythia626,*simPythia646;
    simPythia62 = dynamic_cast<TH1D*>(GetInputHist(fFiles[0],"hsimPythia6_central"));
    simPythia64 = dynamic_cast<TH1D*>(GetInputHist(fFiles[1],"hsimPythia6_central"));
    simPythia66 = dynamic_cast<TH1D*>(GetInputHist(fFiles[2],"hsimPythia6_central"));
    simPythia626 = GetRatio(simPythia62,simPythia66);
    simPythia646 = GetRatio(simPythia64,simPythia66,0.5);

    TH1D *simPythia82,*simPythia84,*simPythia86;
    TH1D *simPythia826,*simPythia846;
    simPythia82 = dynamic_cast<TH1D*>(GetInputHist(fFiles[0],"hsimPythia8_central"));
    simPythia84 = dynamic_cast<TH1D*>(GetInputHist(fFiles[1],"hsimPythia8_central"));
    simPythia86 = dynamic_cast<TH1D*>(GetInputHist(fFiles[2],"hsimPythia8_central"));
    simPythia826 = GetRatio(simPythia82,simPythia86);
    simPythia846 = GetRatio(simPythia84,simPythia86,0.5);

    TH1D *simPythia8Soft2,*simPythia8Soft4,*simPythia8Soft6;
    TH1D *simPythia8Soft26,*simPythia8Soft46;
    simPythia8Soft2 = dynamic_cast<TH1D*>(GetInputHist(fFiles[0],"hsimPythia8Soft2_central"));
    simPythia8Soft4 = dynamic_cast<TH1D*>(GetInputHist(fFiles[1],"hsimPythia8Soft2_central"));
    simPythia8Soft6 = dynamic_cast<TH1D*>(GetInputHist(fFiles[2],"hsimPythia8Soft2_central"));
    simPythia8Soft26 = GetRatio(simPythia8Soft2,simPythia8Soft6);
    simPythia8Soft46 = GetRatio(simPythia8Soft4,simPythia8Soft6,0.5);

    PlaceOnPadSim(upPad,simPowhegPythia26,static_cast<Color_t>(TColor::GetColor("#990011")),25,1);
    PlaceOnPadSim(upPad,simPythia626,static_cast<Color_t>(TColor::GetColor("#009933")),26,2);
    PlaceOnPadSim(upPad,simPythia826,kViolet+2,27,3);
    PlaceOnPadSim(upPad,simPythia8Soft26,kOrange+2,28,4);

    PlaceOnPadSim(upPad,simPowhegPythia46,static_cast<Color_t>(TColor::GetColor("#990011")),25,1);
    PlaceOnPadSim(upPad,simPythia646,static_cast<Color_t>(TColor::GetColor("#009933")),26,2);
    PlaceOnPadSim(upPad,simPythia846,kViolet+2,27,3);
    PlaceOnPadSim(upPad,simPythia8Soft46,kOrange+2,28,4);

    TH1D *simPowhegPythia2R,*simPowhegPythia4R,*simPowhegPythia6R;
    TH1D *simPowhegPythia26R,*simPowhegPythia46R;
    simPowhegPythia2R = dynamic_cast<TH1D*>(GetInputHist(fFiles[0],"hsimPowhegPythia8_central_ratio"));
    simPowhegPythia4R = dynamic_cast<TH1D*>(GetInputHist(fFiles[1],"hsimPowhegPythia8_central_ratio"));
    simPowhegPythia6R = dynamic_cast<TH1D*>(GetInputHist(fFiles[2],"hsimPowhegPythia8_central_ratio"));
    simPowhegPythia26R = GetRatio(simPowhegPythia2R,simPowhegPythia6R);
    simPowhegPythia46R = GetRatio(simPowhegPythia4R,simPowhegPythia6R);

    TH1D *simPythia62R,*simPythia64R,*simPythia66R;
    TH1D *simPythia626R,*simPythia646R;
    simPythia62R = dynamic_cast<TH1D*>(GetInputHist(fFiles[0],"hsimPythia6_central_ratio"));
    simPythia64R = dynamic_cast<TH1D*>(GetInputHist(fFiles[1],"hsimPythia6_central_ratio"));
    simPythia66R = dynamic_cast<TH1D*>(GetInputHist(fFiles[2],"hsimPythia6_central_ratio"));
    simPythia626R = GetRatio(simPythia62R,simPythia66R);
    simPythia646R = GetRatio(simPythia64R,simPythia66R);

    TH1D *simPythia82R,*simPythia84R,*simPythia86R;
    TH1D *simPythia826R,*simPythia846R;
    simPythia82R = dynamic_cast<TH1D*>(GetInputHist(fFiles[0],"hsimPythia8_central_ratio"));
    simPythia84R = dynamic_cast<TH1D*>(GetInputHist(fFiles[1],"hsimPythia8_central_ratio"));
    simPythia86R = dynamic_cast<TH1D*>(GetInputHist(fFiles[2],"hsimPythia8_central_ratio"));
    simPythia826R = GetRatio(simPythia82R,simPythia86R);
    simPythia846R = GetRatio(simPythia84R,simPythia86R);

    TH1D *simPythia8Soft2R,*simPythia8Soft4R,*simPythia8Soft6R;
    TH1D *simPythia8Soft26R,*simPythia8Soft46R;
    simPythia8Soft2R = dynamic_cast<TH1D*>(GetInputHist(fFiles[0],"hsimPythia8Soft2_central_ratio"));
    simPythia8Soft4R = dynamic_cast<TH1D*>(GetInputHist(fFiles[1],"hsimPythia8Soft2_central_ratio"));
    simPythia8Soft6R = dynamic_cast<TH1D*>(GetInputHist(fFiles[2],"hsimPythia8Soft2_central_ratio"));
    simPythia8Soft26R = GetRatio(simPythia8Soft2R,simPythia8Soft6R);
    simPythia8Soft46R = GetRatio(simPythia8Soft4R,simPythia8Soft6R);

    PlaceOnPadSim(dowmPad2,simPowhegPythia26R,static_cast<Color_t>(TColor::GetColor("#990011")),25,1);
    PlaceOnPadSim(dowmPad2,simPythia626R,static_cast<Color_t>(TColor::GetColor("#009933")),26,2);
    PlaceOnPadSim(dowmPad2,simPythia826R,kViolet+2,27,3);
    PlaceOnPadSim(dowmPad2,simPythia8Soft26R,kOrange+2,28,4);
    dowmPad2->cd();
    hDataR26R->Draw("same P e0 x0");

    PlaceOnPadSim(dowmPad,simPowhegPythia46R,static_cast<Color_t>(TColor::GetColor("#990011")),25,1);
    PlaceOnPadSim(dowmPad,simPythia646R,static_cast<Color_t>(TColor::GetColor("#009933")),26,2);
    PlaceOnPadSim(dowmPad,simPythia846R,kViolet+2,27,3);
    PlaceOnPadSim(dowmPad,simPythia8Soft46R,kOrange+2,28,4);
    dowmPad->cd();
    hDataR46R->Draw("same P e0 x0");

   // simPowhegPythia26->Draw("same P");
   // simPythia626->Draw("same P");
  //  simPythia826->Draw("same P");
  //  simPythia8Soft26->Draw("same P");

   //PlaceOnPadSim(upPad,hData_binned2,static_cast<Color_t>(TColor::GetColor("#000099")),24,1);

 //   PlaceOnPadData(upPad,hDataSys,hData_binned,0.9);
  //  PlaceOnPadData(dowmPad,hDataSysRatio,hData_binned_ratio,0);

/*
    // ----------------- prompt simulation ---------------------
    TGraphAsymmErrors *simPowhegPythia6 = nullptr;
    TH1D* simPowhegPythia6_cent = nullptr, *simPowhegPythia6_down = nullptr, *simPowhegPythia6_up = nullptr;
    TGraphAsymmErrors *simPowhegPythia6_R = nullptr;
    TH1D* simPowhegPythia6_cent_R = nullptr, *simPowhegPythia6_down_R = nullptr, *simPowhegPythia6_up_R = nullptr;
    if(ePowhegPythia6){
        std::cout<<"get POWHWG+PYTHIA6"<<std::endl;
        std::tie(simPowhegPythia6, simPowhegPythia6_cent, simPowhegPythia6_up, simPowhegPythia6_down) = GetSim("simPowhegPythia6",type, 9, fPowhegPythia6, simDir, simScaling, xAxisBins, xAxis);
        std::tie(simPowhegPythia6_R, simPowhegPythia6_cent_R, simPowhegPythia6_up_R, simPowhegPythia6_down_R) = GetDataSimRatio("simPowhegPythia6",hData_binned,simPowhegPythia6_cent, simPowhegPythia6_up, simPowhegPythia6_down, xAxisBins, xAxis);
        PlaceOnPadSim(upPad,simPowhegPythia6,static_cast<Color_t>(TColor::GetColor("#000099")),24,1);
        PlaceOnPadSim(dowmPad,simPowhegPythia6_R,static_cast<Color_t>(TColor::GetColor("#000099")),24,1);
    }

    TGraphAsymmErrors *simPowhegPythia8 = nullptr;
    TH1D* simPowhegPythia8_cent = nullptr, *simPowhegPythia8_down = nullptr, *simPowhegPythia8_up = nullptr;
    TGraphAsymmErrors *simPowhegPythia8_R = nullptr;
    TH1D* simPowhegPythia8_cent_R = nullptr, *simPowhegPythia8_down_R = nullptr, *simPowhegPythia8_up_R = nullptr;
    if(ePowhegPythia8){
        std::cout<<"get POWHWG+PYTHIA8"<<std::endl;
        std::tie(simPowhegPythia8, simPowhegPythia8_cent, simPowhegPythia8_up, simPowhegPythia8_down) = GetSim("simPowhegPythia8",type, 9, fPowhegPythia8, simDir, simScaling, xAxisBins, xAxis);
        std::tie(simPowhegPythia8_R, simPowhegPythia8_cent_R, simPowhegPythia8_up_R, simPowhegPythia8_down_R) = GetDataSimRatio("simPowhegPythia8",hData_binned,simPowhegPythia8_cent, simPowhegPythia8_up, simPowhegPythia8_down, xAxisBins, xAxis);
        PlaceOnPadSim(upPad,simPowhegPythia8,static_cast<Color_t>(TColor::GetColor("#990011")),25,1);
        PlaceOnPadSim(dowmPad,simPowhegPythia8_R,static_cast<Color_t>(TColor::GetColor("#990011")),25,1);
    }

    TGraphAsymmErrors *simPythia6 = nullptr;
    TH1D* simPythia6_cent = nullptr, *simPythia6_down = nullptr, *simPythia6_up = nullptr;
    TGraphAsymmErrors *simPythia6_R = nullptr;
    TH1D* simPythia6_cent_R = nullptr, *simPythia6_down_R = nullptr, *simPythia6_up_R = nullptr;
    if(ePythia6){
        std::cout<<"get PYTHIA6"<<std::endl;
        std::tie(simPythia6, simPythia6_cent, simPythia6_up, simPythia6_down) = GetSim("simPythia6",type, 1,fPythia6, simDir, simScaling, xAxisBins, xAxis);
        std::tie(simPythia6_R, simPythia6_cent_R, simPythia6_up_R, simPythia6_down_R) = GetDataSimRatio("simPythia6",hData_binned,simPythia6_cent, simPythia6_up, simPythia6_down, xAxisBins, xAxis);
        PlaceOnPadSim(upPad,simPythia6,static_cast<Color_t>(TColor::GetColor("#009933")),26,2);
        PlaceOnPadSim(dowmPad,simPythia6_R,static_cast<Color_t>(TColor::GetColor("#009933")),26,2);
    }

    TGraphAsymmErrors *simPythia8 = nullptr;
    TH1D* simPythia8_cent = nullptr, *simPythia8_down = nullptr, *simPythia8_up = nullptr;
    TGraphAsymmErrors *simPythia8_R = nullptr;
    TH1D* simPythia8_cent_R = nullptr, *simPythia8_down_R = nullptr, *simPythia8_up_R = nullptr;
    if(ePythia8){
        std::cout<<"get PYTHIA8"<<std::endl;
        std::tie(simPythia8, simPythia8_cent, simPythia8_up, simPythia8_down) = GetSim("simPythia8",type, 1,fPythia8, simDir, simScaling, xAxisBins, xAxis);
        std::tie(simPythia8_R, simPythia8_cent_R, simPythia8_up_R, simPythia8_down_R) = GetDataSimRatio("simPythia8",hData_binned,simPythia8_cent, simPythia8_up, simPythia8_down, xAxisBins, xAxis);
        PlaceOnPadSim(upPad,simPythia8,kViolet+2,27,3);
        PlaceOnPadSim(dowmPad,simPythia8_R,kViolet+2,27,3);
    }

    TGraphAsymmErrors *simPythia8Soft2 = nullptr;
    TH1D* simPythia8Soft2_cent = nullptr, *simPythia8Soft2_down = nullptr, *simPythia8Soft2_up = nullptr;
    TGraphAsymmErrors *simPythia8Soft2_R = nullptr;
    TH1D* simPythia8Soft2_cent_R = nullptr, *simPythia8Soft2_down_R = nullptr, *simPythia8Soft2_up_R = nullptr;
    if(ePythia8SoftMode2){
        std::cout<<"get PYTHIA8 soft mode2"<<std::endl;
        std::tie(simPythia8Soft2, simPythia8Soft2_cent, simPythia8Soft2_up, simPythia8Soft2_down) = GetSim("simPythia8Soft2",type, 1,fPythia8SoftMode2, simDir, simScaling, xAxisBins, xAxis);
        std::tie(simPythia8Soft2_R, simPythia8Soft2_cent_R, simPythia8Soft2_up_R, simPythia8Soft2_down_R) = GetDataSimRatio("simPythia8Soft2",hData_binned,simPythia8Soft2_cent, simPythia8Soft2_up, simPythia8Soft2_down, xAxisBins, xAxis);
        PlaceOnPadSim(upPad,simPythia8Soft2,kOrange+2,28,4);
        PlaceOnPadSim(dowmPad,simPythia8Soft2_R,kOrange+2,28,4);
    }

    TGraphAsymmErrors *simPowhegPythia6dijet = nullptr;
    TGraphAsymmErrors *simPowhegPythia6dijet_R = nullptr;
    TH1D* simPowhegPythia6dijet_cent = nullptr, *simPowhegPythia6dijet_down = nullptr, *simPowhegPythia6dijet_up = nullptr;
    TH1D* simPowhegPythia6dijet_cent_R = nullptr, *simPowhegPythia6dijet_down_R = nullptr, *simPowhegPythia6dijet_up_R = nullptr;
    if(ePowhegPythia6dijet){
        std::cout<<"get POWHWG+PYTHIA6 dijet"<<std::endl;
        std::tie(simPowhegPythia6dijet, simPowhegPythia6dijet_cent, simPowhegPythia6dijet_up, simPowhegPythia6dijet_down) = GetSim("simPowhegPythia6dijet",type, 1, fPowhegPythia6dijet, simDir, simScaling, xAxisBins, xAxis);
        std::tie(simPowhegPythia6dijet_R, simPowhegPythia6dijet_cent_R, simPowhegPythia6dijet_up_R, simPowhegPythia6dijet_down_R) = GetDataSimRatio("simPowhegPythia6dijet",hData_binned,simPowhegPythia6dijet_cent, simPowhegPythia6dijet_up, simPowhegPythia6dijet_down, xAxisBins, xAxis);
        PlaceOnPadSim(upPad,simPowhegPythia6dijet,static_cast<Color_t>(TColor::GetColor("#000099")),24,1);
        PlaceOnPadSim(dowmPad,simPowhegPythia6dijet_R,static_cast<Color_t>(TColor::GetColor("#000099")),24,1);
    }

    TGraphAsymmErrors *simPowhegPythia8dijet = nullptr;
    TGraphAsymmErrors *simPowhegPythia8dijet_R = nullptr;
    TH1D* simPowhegPythia8dijet_cent = nullptr, *simPowhegPythia8dijet_down = nullptr, *simPowhegPythia8dijet_up = nullptr;
    TH1D* simPowhegPythia8dijet_cent_R = nullptr, *simPowhegPythia8dijet_down_R = nullptr, *simPowhegPythia8dijet_up_R = nullptr;
    if(ePowhegPythia8dijet){
        std::cout<<"get POWHWG+PYTHIA6 dijet"<<std::endl;
        std::tie(simPowhegPythia8dijet, simPowhegPythia8dijet_cent, simPowhegPythia8dijet_up, simPowhegPythia8dijet_down) = GetSim("simPowhegPythia8dijet",type, 1, fPowhegPythia8dijet, simDir, simScaling, xAxisBins, xAxis);
        std::tie(simPowhegPythia8dijet_R, simPowhegPythia8dijet_cent_R, simPowhegPythia8dijet_up_R, simPowhegPythia8dijet_down_R) = GetDataSimRatio("simPowhegPythia8dijet",hData_binned,simPowhegPythia8dijet_cent, simPowhegPythia8dijet_up, simPowhegPythia8dijet_down, xAxisBins, xAxis);
        PlaceOnPadSim(upPad,simPowhegPythia8dijet,static_cast<Color_t>(TColor::GetColor("#009999")),24,1);
        PlaceOnPadSim(dowmPad,simPowhegPythia8dijet_R,static_cast<Color_t>(TColor::GetColor("#009999")),24,1);
    }
*/
    // ----------------- Legend and text PAD---------------------
    TLegend *leg =nullptr;
    Double_t shift = 0.06*(ePowhegPythia6+ePowhegPythia8+ePythia6+ePythia8+ePythia8SoftMode2+ePowhegPythia6dijet);
    leg = new TLegend(0.225,0.4,0.525,0.7,nullptr,"NB NDC");
    leg->SetBorderSize(0);
    leg->SetTextFont(43);
    leg->SetTextSize(18);
    leg->SetLineColor(1);
    leg->SetLineStyle(1);
    leg->SetLineWidth(1);
    leg->SetFillColor(0);
    leg->SetFillStyle(0);
        leg->AddEntry(hDataR46,"Data #it{#sigma}(R=0.4)/#it{#sigma}(R=0.6) (+0.5)","fp");
    leg->AddEntry(hDataR26,"Data #it{#sigma}(R=0.2)/#it{#sigma}(R=0.6)","fp");


    leg->AddEntry(simPowhegPythia26,"POWHEG hvq + PYTHIA 8","pf");
    leg->AddEntry(simPythia626,"PYTHIA 6 Perugia 2011","pf");
    leg->AddEntry(simPythia826,"PYTHIA 8 Monash 2013","pf");
    leg->AddEntry(simPythia8Soft26,"PYTHIA 8 Monash 2013 Soft mode 2","pf");
    //leg->AddEntry(simPowhegPythia6dijet,"POWHEG dijet + PYTHIA 8 Salvatore","pf");
    TPaveText *pt[2];
    pt[0] = new TPaveText(0.2,0.85,0.85,0.95,"NB NDC");
    pt[1] = new TPaveText(0.2,0.75,0.85,0.85,"NB NDC");
    for(Int_t s = 0; s<2;s++){
        pt[s]->SetBorderSize(0);
        pt[s]->SetFillStyle(0);
        pt[s]->SetTextAlign(13);
        pt[s]->SetTextFont(43);
        pt[s]->SetTextSize(22);
    }
    pt[0]->AddText("ALICE Preliminary"); //uncomment
    pt[0]->AddText("pp, #sqrt{#it{s}} = 13 TeV");
    pt[1]->AddText(Form("charged jets, anti-#it{k}_{T} with D^{0}, %d < #it{p}_{T,D^{0}} < %d GeV/#it{c}",static_cast<int>(2),static_cast<int>(36)));
   // pt[1]->AddText(Form ("with D^{0}, %d < #it{p}_{T,D^{0}} < %d GeV/#it{c}",static_cast<int>(2),static_cast<int>(36)));
   // if(type==1 ||type==2)pt[1]->AddText(Form ("%d < #it{p}_{T,jet} < %d GeV/#it{c}",jetpTbins[zBin-1],jetpTbins[zBin]));
   // if(type==1 ||type==2)pt[1]->AddText(Form ("with D^{0}, %d < #it{p}_{T,D^{0}} < %d GeV/#it{c}",DpTbins[0][zBin-1],DpTbins[1][zBin-1]));
   // if(type==1 ||type==2)pt[1]->AddText(Form ("%d < #it{p}_{T,jet} < %d GeV/#it{c} with D^{0}, %d < #it{p}_{T,D^{0}} < %d GeV/#it{c}",jetpTbins[zBin-1],jetpTbins[zBin],DpTbins[0][zBin-1],DpTbins[1][zBin-1]));
    //if(type==1 ||type==2)pt[1]->AddText(Form ("",DpTbins[0][zBin-1],DpTbins[1][zBin-1]));

    TPaveText *pt2[2];
    pt2[0] = new TPaveText(0.65,0.79,0.95,0.95,"NB NDC");
    pt2[1] = new TPaveText(0.65,0.8,0.95,0.95,"NB NDC");
    for(Int_t s = 0; s<2;s++){
        pt2[s]->SetBorderSize(0);
        pt2[s]->SetFillStyle(0);
        pt2[s]->SetTextAlign(13);
        pt2[s]->SetTextFont(43);
        pt2[s]->SetTextSize(19);
    }

    pt2[0]->AddText("#it{#sigma}(R=0.4)/#it{#sigma}(R=0.6)");
    pt2[1]->AddText("#it{#sigma}(R=0.2)/#it{#sigma}(R=0.6)");


    upPad->cd();
    leg->Draw();
    pt[0]->Draw();
    pt[1]->Draw();
    dowmPad->cd();
    pt2[0]->Draw();
    dowmPad2->cd();
    pt2[1]->Draw();

    // ----------------- Terminate Canvas ---------------------
    TerminateCanvas(upPad,dowmPad,dowmPad2,placeholder_up,placeholder_down,placeholder_down2);
 /*   canvas->SaveAs(outPlotDir+"/finalSpectra.png");

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

void PlaceOnPadSim(TPad* pad,TGraphAsymmErrors* histo, Color_t ci, Style_t style, Style_t linestyle){
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
    TString odraaw = "";
    if(linestyle ==1) odraaw = "same 2p";
    else odraaw = "same 1p";
    histo->Draw(odraaw);
}

void PlaceOnPadSim(TPad* pad,TH1D* histo, Color_t ci, Style_t style, Style_t linestyle){
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
    TString odraaw = "";
    if(linestyle ==1) odraaw = "same 2p";
    else odraaw = "same 1p";
    histo->Draw(odraaw);
}

void PlaceOnPadData(TPad* pad,TGraphAsymmErrors* histo1, TH1D* histo2, Size_t markersize){
    pad->cd();
    Color_t ci = static_cast<Color_t>(TColor::GetColor("#990000"));
    histo1->SetLineColor(ci);
    histo1->SetMarkerColor(ci);
    histo1->SetMarkerStyle(20);
    histo1->SetMarkerSize(markersize);
    ci = static_cast<Color_t>(TColor::GetColor("#cccccc"));
    histo1->SetFillColor(ci);
    histo1->SetLineColor(ci);
    histo1->Draw("2p");
    //data central w stat. unc.
    ci = static_cast<Color_t>(TColor::GetColor("#990000"));
    histo2->SetLineColor(ci);
    histo2->SetMarkerColor(ci);
    histo2->SetMarkerStyle(20);
    histo2->SetMarkerSize(markersize);
    histo2->Draw("same p  e0 x0");
};

std::tuple<TCanvas*, TPad*, TPad*,  TPad*, TH1D*, TH1D*, TH1D*> PrepareCanvas(UInt_t xAxisBins, Double_t *xAxis){
    style();
    //prepare main canvas
    TCanvas *FinalSpectrum = new TCanvas("FinalSpectrum", "FinalSpectrum",0,45,700,700);
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


std::tuple<TGraphAsymmErrors*, TH1D*, TH1D*, TH1D*> GetSim(TString simname, Int_t type, Int_t nFiles, TString *names, TString simDir, Double_t simScaling, UInt_t nBins, Double_t *xBins){
    TH1D **hPrompt = new TH1D*[static_cast<ULong_t>(nFiles)];
    TH1D **hPrompt_binned = new TH1D*[static_cast<ULong_t>(nFiles)];
    //TH1D *hPrompt_binned[static_cast<ULong_t>(nFiles)];
    std::cout<<"type "<<type<<std::endl;

    for (int nr=0; nr<nFiles; nr++){
        TString file = simDir;
        file += "/JetPt_";
        file += names[nr];
        if(type ==0){
            file += "_Dpt"; file += 2; file += "_"; file += 36;
        }
        if(type ==1 || type ==2){
            file += "_Dpt"; file += DpTbins[0][zBin-1]; file += "_"; file += DpTbins[1][zBin-1];
            file += "_Jetpt"; file += jetpTbins[zBin-1]; file += "_"; file += jetpTbins[zBin];
        }
        file += "_Dzero";
        file += ".root";
        TH1D *htmp = nullptr;
        if(type ==0){
            htmp = dynamic_cast<TH1D*>(GetInputHist(file, "hPt"));
            if(!htmp) std::cout<<"no histogram in file: "<<file<<std::endl;
            htmp->GetYaxis()->SetTitle("d#sigma/dp_{T} (mb)");
        }
        if(type ==1 || type ==2){
            htmp = dynamic_cast<TH1D*>(GetInputHist(file, "hz"));
            if(!htmp) std::cout<<"no histogram in file: "<<file<<std::endl;
            htmp->GetYaxis()->SetTitle("d#sigma/dz (mb)");
        }
        hPrompt[nr] = dynamic_cast<TH1D*>(htmp->Clone(Form("hPrompt_%d",nr)));
        hPrompt_binned[nr] = dynamic_cast<TH1D*>(htmp->Rebin(static_cast<Int_t>(nBins),Form("hPrompt_binned_%d",nr),xBins));
        if(pdf){
            hPrompt_binned[nr]->Scale(1./hPrompt_binned[nr]->Integral());
            hPrompt_binned[nr]->Scale(1,"width");
        }
    }
    TString tmp = "h";
    tmp+=simname;

    TH1D *hPrompt_central_binned = dynamic_cast<TH1D*>(hPrompt_binned[0]->Clone(tmp+"_central"));
    setHistoDetails(hPrompt_central_binned,4,24);

    // get up unc
    TH1D * hPrompt_up = GetUpSys(hPrompt_binned,nFiles);
    hPrompt_up->SetName(tmp+"_up");
    setHistoDetails(hPrompt_up,4,24,0,2,2);
    // get down unc
    TH1D *hPrompt_down = GetDownSys(hPrompt_binned,nFiles);
    hPrompt_down->SetName(tmp+"_down");
    setHistoDetails(hPrompt_down,4,24,0,2,2);

    if(!pdf){
        hPrompt_central_binned->Scale(simScaling);
        hPrompt_central_binned->Scale(1,"width");
        hPrompt_central_binned->Scale(1./dy);//2*jetEta;
        hPrompt_up->Scale(simScaling);
        hPrompt_up->Scale(1,"width");
        hPrompt_up->Scale(1./dy);
        hPrompt_down->Scale(simScaling);
        hPrompt_down->Scale(1,"width");
        hPrompt_down->Scale(1./dy);
    }

    Double_t *xval = new Double_t[nBins];
    Double_t *xvalwidth = new Double_t[nBins];
    Double_t *valuetheory = new Double_t[nBins];
    Double_t *valuetheoryerrup = new Double_t[nBins];
    Double_t *valuetheoryerrdown = new Double_t[nBins];
    for(UInt_t j=0; j<nBins; j++){
          xval[j] = (xBins[j]+xBins[j+1]) / 2.;
          xvalwidth[j] = (xBins[j+1]-xBins[j]) / 2.;
          valuetheory[j] = hPrompt_central_binned->GetBinContent(hPrompt_central_binned->GetXaxis()->FindBin(xval[j]));
          valuetheoryerrup[j] = hPrompt_up->GetBinContent(hPrompt_up->GetXaxis()->FindBin(xval[j])) - valuetheory[j];
          valuetheoryerrdown[j] = valuetheory[j] - hPrompt_down->GetBinContent(hPrompt_up->GetXaxis()->FindBin(xval[j]));

    }
    TGraphAsymmErrors *grsystheory = new TGraphAsymmErrors(static_cast<Int_t>(nBins),xval,valuetheory,xvalwidth,xvalwidth,valuetheoryerrdown,valuetheoryerrup);
    tmp = "hae";
    tmp+=simname;
    grsystheory->SetName(tmp);
    return std::make_tuple(grsystheory, hPrompt_central_binned, hPrompt_up,hPrompt_down);
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
TH1D* GetUpSys(TH1D **hh, Int_t nFiles){
    double max = 0, maxerr = 0;
    TString name = hh[0]->GetName();
    TH1D *hh_up = dynamic_cast<TH1D*>(hh[0]->Clone(name +"_up"));

    for(int iBin=1; iBin<hh[0]->GetNbinsX()+1; iBin++ ){
      //  std::cout<<"iBin: "<<iBin<<std::endl;
        max = hh[0]->GetBinContent(iBin);
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

TH1D* GetDownSys(TH1D **hh, Int_t nFiles){
    double max = 0, maxerr = 0;
    TString name = hh[0]->GetName();
    TH1D *hh_down = dynamic_cast<TH1D*>(hh[0]->Clone(name +"_down"));

    for(int iBin=1; iBin<hh[0]->GetNbinsX()+1; iBin++ ){
        max = hh[0]->GetBinContent(iBin);
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
