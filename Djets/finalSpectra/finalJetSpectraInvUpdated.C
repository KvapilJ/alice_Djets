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
#include "TSVDUnfold.h"
#include "TGraphAsymmErrors.h"
#include "TLegendEntry.h"
#include "TPad.h"
#include "../Preliminaryplots/FD/style.C"

Double_t fRpar = 0.4;
double jetEta = 0.9 - fRpar;
double dy = 2*jetEta;
Bool_t pdf = false;


//Color_t colors[] = {1,8,4,2,6,kOrange-1,kGray+2,kCyan+1,kMagenta+2,kViolet+5,kYellow+2};
//Int_t markers[] = {20,21,22,23,24,25,26,27,28,29,30,32,33,34};
//Style_t linestyle[] = {1,2,3,4,5,6,7,8,9,10,11,12,13};

void ScaleHist(TH1 *hh, int full = 0);
void setHistoDetails(TH1 *hh, Color_t color, Style_t Mstyle, Double_t Msize = 1.1, Width_t Lwidth = 2, Style_t Lstyle = 1);
void SaveCanvas(TCanvas *c, TString name = "tmp");
void getSystematics(TString inDir, TString outPlotDir);
TH1D* GetUpSys(TH1D **hh, Int_t nFiles = 0);
TH1D* GetDownSys(TH1D **hh, Int_t nFiles = 0);
TH1* GetInputHist(TString inFile, TString histName);
void ScaleHist(TH1 *hh, int full);
void drawFinal(TString outPlotDir);

TString fPowhegPythia6[9] = {
    "AnalysisResults_FastSim_powheg+pythia6_charm_central",
    "AnalysisResults_FastSim_powheg+pythia6_charm_mc13",
    "AnalysisResults_FastSim_powheg+pythia6_charm_mc17",
    "AnalysisResults_FastSim_powheg+pythia6_charm_uF1uR05",
    "AnalysisResults_FastSim_powheg+pythia6_charm_uF05uR1",
    "AnalysisResults_FastSim_powheg+pythia6_charm_uF2uR1",
    "AnalysisResults_FastSim_powheg+pythia6_charm_uF1uR2",
    "AnalysisResults_FastSim_powheg+pythia6_charm_uF05uR05",
    "AnalysisResults_FastSim_powheg+pythia6_charm_uF2uR2"
    };
TString fPowhegPythia8[9] = {
    "AnalysisResults_FastSim_powheg+pythia8_charm_central",
    "AnalysisResults_FastSim_powheg+pythia8_charm_mc13",
    "AnalysisResults_FastSim_powheg+pythia8_charm_mc17",
    "AnalysisResults_FastSim_powheg+pythia8_charm_uF1uR05",
    "AnalysisResults_FastSim_powheg+pythia8_charm_uF05uR1",
    "AnalysisResults_FastSim_powheg+pythia8_charm_uF2uR1",
    "AnalysisResults_FastSim_powheg+pythia8_charm_uF1uR2",
    "AnalysisResults_FastSim_powheg+pythia8_charm_uF05uR05",
    "AnalysisResults_FastSim_powheg+pythia8_charm_uF2uR2"
    };


TString fPythia6[9] = {
   "AnalysisResults_FastSim_pythia6_charm"
   // "AnalysisResults_FastSim_pythia6_charmTREESCALING"
};

TString fPythia8[9] = {
   "AnalysisResults_FastSim_pythia8_charm"
};

TString fPythia8SoftMode2[9] = {
   "AnalysisResults_FastSim_pythia8_charm_Color2Soft50"
};
TString fPowhegPythia6dijet[9] = {
   "AnalysisResults_FastSim_powheg+pythia6_dijet46"
};

std::tuple<TGraphAsymmErrors*, TH1D*, TH1D*, TH1D*> GetSim(Int_t nFiles, TString *names, TString simDir, Double_t simScaling, UInt_t nBins, Double_t *xBins);
std::tuple<TGraphAsymmErrors*, TH1D*, TH1D*, TH1D*> GetDataSimRatio(TH1D *data_cent, TH1D *sim_cent, TH1D *sim_up, TH1D *sim_down, UInt_t nBins, Double_t *xBins);
std::tuple<TGraphAsymmErrors*, TH1D*,TGraphAsymmErrors*, TH1D*> GetData(TString dataFile, TString histBase, Double_t dataScaling, UInt_t nBins, Double_t *xBins, Double_t *systUncD_down, Double_t *systUncD_up);
std::tuple<TCanvas*, TPad*, TPad*, TH1D*, TH1D*> PrepareCanvas(Int_t xAxisBins, Double_t *xAxis);
void PlaceOnPadData(TPad* pad,TGraphAsymmErrors* histo1, TH1D* histo2);
void PlaceOnPadSim(TPad* pad,TGraphAsymmErrors* histo, Int_t ci, Int_t style, Int_t linestyle);
void TerminateCanvas(TPad* pad1,TPad* pad2,TH1D* histo1,TH1D* histo2);


void finalJetSpectraInvUpdated(
TString dataFile = "/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_X04/Default_AnalysisResults_Run2w18b.root/unfolding_Bayes_5/unfoldedSpectrum_unfoldedJetSpectrum.root",
TString dataAnalysisFile = "/mnt/hgfs/vmware/data_R04_050219/data/AnalysisResults_Run2w18b.root",
TString simDir = "/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_X04/Simulations/Prompt/AnalysisResults_Run2w18b.root0",
TString outSpectraDir = "/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_X04/Default_AnalysisResults_Run2w18b.root/unfolding_Bayes_5/finalSpectraNEW",
TString histBase = "unfoldedSpectrum"
)
/*
void finalJetSpectraInvUpdated(
TString dataFile = "/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts/Default_AnalysisResults_Run2w18b.root/unfolding_2D_5/unfoldedSpectrum_unfoldedJetSpectrum.root",
TString dataAnalysisFile = "/mnt/hgfs/vmware/data_R04_050219/data/AnalysisResults_Run2w18b.root",
TString simDir = "/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts/Simulations/Prompt/AnalysisResults_Run2w18b.root5",
TString outSpectraDir = "home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts/Default_AnalysisResults_Run2w18b.root/unfolding_2D_5/finalSpectraNEW",
TString histBase = "unfoldedSpectrumKineEff5"
)*/
{
    TString outPlotDir = outSpectraDir;
    outPlotDir+="/plots";
    gSystem->Exec(Form("mkdir %s",outSpectraDir.Data()));
    gSystem->Exec(Form("mkdir %s",outPlotDir.Data()));

    Double_t sigma_in = 0.0578;
    const Double_t  BRDzero = 0.0389;

   const Int_t xAxisBins = 8;
    Double_t xAxis[xAxisBins+1] = {5,6,8,10,12,14,20,30,50};

   // Double_t systUncD_down[xAxisBins] = {0.1,0.1,0.1,0.2,0.2,0.2,0.3,0.3};
   // Double_t systUncD_up[xAxisBins] = {0.4,0.4,0.4,0.5,0.5,0.7,0.7,0.7};
//
    Double_t systUncD_up[xAxisBins] = {0.09,	0.08,	0.09	,0.10,	0.11	,0.13	,0.15	,0.19};
    Double_t systUncD_down[xAxisBins] = {0.10,	0.09,	0.11	,0.12,	0.14,	0.16,	0.23,	0.24};
/*
      const Int_t xAxisBins = 7;
      Double_t xAxis[xAxisBins+1] = {0.2,0.4,0.5,0.6,0.7,0.8,0.9,1.0};

    Double_t systUncD_up[xAxisBins] = {0.2,0.2,0.14	,0.08,	0.08,	0.09,	0.09};
    Double_t systUncD_down[xAxisBins] = {0.2,0.2,0.15,	0.09	,0.09,	0.10,	0.09};
*/


    //get Data n Events
    std::cout<<"get Data n Events"<<std::endl;
    TFile *File = new TFile(dataAnalysisFile,"read");
    TDirectoryFile* dir = dynamic_cast<TDirectoryFile*>(File->Get("PWG3_D2H_DmesonsForJetCorrelationsMBN0"));
    AliNormalizationCounter *c = dynamic_cast<AliNormalizationCounter*>(dir->Get("NormalizationCounter"));
    double nEv = c->GetNEventsForNorm();
    double dataLum = nEv/(sigma_in*1000) ;//Luminosity in mbar
    double simScaling = 0.5; //pp 0.5
    double dataScaling = 1. /(BRDzero * dataLum)/2.;
    std::cout<<"scalings "<<jetEta<<" "<<dy<<" "<<dataScaling<<" "<<BRDzero<<" "<<dataLum<<" "<<nEv<<" "<<sigma_in<<std::endl;


    // ----------------- Initiate Canvas ---------------------
    TCanvas *canvas;
    TPad *upPad, *dowmPad;
    TH1D *placeholder_up, *placeholder_down;
    std::tie(canvas,upPad,dowmPad,placeholder_up,placeholder_down) = PrepareCanvas(xAxisBins,xAxis);

    // ----------------- data ---------------------
    TGraphAsymmErrors *hDataSys, *hDataSysRatio;
    TH1D *hData_binned, *hData_binned_ratio;
    std::tie(hDataSys, hData_binned, hDataSysRatio, hData_binned_ratio) = GetData(dataFile, histBase, dataScaling, xAxisBins, xAxis, systUncD_down, systUncD_up);

    PlaceOnPadData(upPad,hDataSys,hData_binned);
    PlaceOnPadData(dowmPad,hDataSysRatio,hData_binned_ratio);

    TGraphAsymmErrors *simPowhegPythia6dijet;
    TH1D* simPowhegPythia6dijet_cent, *simPowhegPythia6dijet_down, *simPowhegPythia6dijet_up;
    std::tie(simPowhegPythia6dijet, simPowhegPythia6dijet_cent, simPowhegPythia6dijet_up, simPowhegPythia6dijet_down) = GetSim(1, fPowhegPythia6dijet, simDir, simScaling, xAxisBins, xAxis);
    TGraphAsymmErrors *simPowhegPythia6dijet_R;
    TH1D* simPowhegPythia6dijet_cent_R, *simPowhegPythia6dijet_down_R, *simPowhegPythia6dijet_up_R;
    std::tie(simPowhegPythia6dijet_R, simPowhegPythia6dijet_cent_R, simPowhegPythia6dijet_up_R, simPowhegPythia6dijet_down_R) = GetDataSimRatio(hData_binned,simPowhegPythia6dijet_cent, simPowhegPythia6dijet_up, simPowhegPythia6dijet_down, xAxisBins, xAxis);

    PlaceOnPadSim(upPad,simPowhegPythia6dijet,TColor::GetColor("#000099"),24,1);
    PlaceOnPadSim(dowmPad,simPowhegPythia6dijet_R,TColor::GetColor("#000099"),24,1);

    // ----------------- prompt simulation ---------------------
/*    std::cout<<"get POWHWG+PYTHIA6"<<std::endl;
    TGraphAsymmErrors *simPowhegPythia6;
    TH1D* simPowhegPythia6_cent, *simPowhegPythia6_down, *simPowhegPythia6_up;
    std::tie(simPowhegPythia6, simPowhegPythia6_cent, simPowhegPythia6_up, simPowhegPythia6_down) = GetSim(9, fPowhegPythia6, simDir, simScaling, xAxisBins, xAxis);
    TGraphAsymmErrors *simPowhegPythia6_R;
    TH1D* simPowhegPythia6_cent_R, *simPowhegPythia6_down_R, *simPowhegPythia6_up_R;
    std::tie(simPowhegPythia6_R, simPowhegPythia6_cent_R, simPowhegPythia6_up_R, simPowhegPythia6_down_R) = GetDataSimRatio(hData_binned,simPowhegPythia6_cent, simPowhegPythia6_up, simPowhegPythia6_down, xAxisBins, xAxis);

    PlaceOnPadSim(upPad,simPowhegPythia6,TColor::GetColor("#000099"),24,1);
    PlaceOnPadSim(dowmPad,simPowhegPythia6_R,TColor::GetColor("#000099"),24,1);

    std::cout<<"get POWHWG+PYTHIA8"<<std::endl;
    TGraphAsymmErrors *simPowhegPythia8;
    TH1D* simPowhegPythia8_cent, *simPowhegPythia8_down, *simPowhegPythia8_up;
    std::tie(simPowhegPythia8, simPowhegPythia8_cent, simPowhegPythia8_up, simPowhegPythia8_down) = GetSim(9, fPowhegPythia8, simDir, simScaling, xAxisBins, xAxis);
    TGraphAsymmErrors *simPowhegPythia8_R;
    TH1D* simPowhegPythia8_cent_R, *simPowhegPythia8_down_R, *simPowhegPythia8_up_R;
    std::tie(simPowhegPythia8_R, simPowhegPythia8_cent_R, simPowhegPythia8_up_R, simPowhegPythia8_down_R) = GetDataSimRatio(hData_binned,simPowhegPythia8_cent, simPowhegPythia8_up, simPowhegPythia8_down, xAxisBins, xAxis);

    PlaceOnPadSim(upPad,simPowhegPythia8,TColor::GetColor("#990011"),25,1);
    PlaceOnPadSim(dowmPad,simPowhegPythia8_R,TColor::GetColor("#990011"),25,1);


    std::cout<<"get PYTHIA6"<<std::endl;
    TGraphAsymmErrors *simPythia6;
    TH1D* simPythia6_cent, *simPythia6_down, *simPythia6_up;
    std::tie(simPythia6, simPythia6_cent, simPythia6_up, simPythia6_down) = GetSim(1,fPythia6, simDir, simScaling, xAxisBins, xAxis);
    TGraphAsymmErrors *simPythia6_R;
    TH1D* simPythia6_cent_R, *simPythia6_down_R, *simPythia6_up_R;
    std::tie(simPythia6_R, simPythia6_cent_R, simPythia6_up_R, simPythia6_down_R) = GetDataSimRatio(hData_binned,simPythia6_cent, simPythia6_up, simPythia6_down, xAxisBins, xAxis);

    PlaceOnPadSim(upPad,simPythia6,TColor::GetColor("#009933"),26,2);
    PlaceOnPadSim(dowmPad,simPythia6_R,TColor::GetColor("#009933"),26,2);

    std::cout<<"get PYTHIA8"<<std::endl;
    TGraphAsymmErrors *simPythia8;
    TH1D* simPythia8_cent, *simPythia8_down, *simPythia8_up;
    std::tie(simPythia8, simPythia8_cent, simPythia8_up, simPythia8_down) = GetSim(1,fPythia8, simDir, simScaling, xAxisBins, xAxis);
    TGraphAsymmErrors *simPythia8_R;
    TH1D* simPythia8_cent_R, *simPythia8_down_R, *simPythia8_up_R;
    std::tie(simPythia8_R, simPythia8_cent_R, simPythia8_up_R, simPythia8_down_R) = GetDataSimRatio(hData_binned,simPythia8_cent, simPythia8_up, simPythia8_down, xAxisBins, xAxis);

    PlaceOnPadSim(upPad,simPythia8,kViolet+2,27,3);
    PlaceOnPadSim(dowmPad,simPythia8_R,kViolet+2,27,3);

    std::cout<<"get PYTHIA8 soft mode2"<<std::endl;
    TGraphAsymmErrors *simPythia8Soft2;
    TH1D* simPythia8Soft2_cent, *simPythia8Soft2_down, *simPythia8Soft2_up;
    std::tie(simPythia8Soft2, simPythia8Soft2_cent, simPythia8Soft2_up, simPythia8Soft2_down) = GetSim(1,fPythia8SoftMode2, simDir, simScaling, xAxisBins, xAxis);
    TGraphAsymmErrors *simPythia8Soft2_R;
    TH1D* simPythia8Soft2_cent_R, *simPythia8Soft2_down_R, *simPythia8Soft2_up_R;
    std::tie(simPythia8Soft2_R, simPythia8Soft2_cent_R, simPythia8Soft2_up_R, simPythia8Soft2_down_R) = GetDataSimRatio(hData_binned,simPythia8Soft2_cent, simPythia8Soft2_up, simPythia8Soft2_down, xAxisBins, xAxis);

    PlaceOnPadSim(upPad,simPythia8Soft2,kOrange+2,28,4);
    PlaceOnPadSim(dowmPad,simPythia8Soft2_R,kOrange+2,28,4);
*/

    // ----------------- Legend and text PAD---------------------
    TLegend *leg = leg = new TLegend(0.5,0.4,0.8,0.65,nullptr,"NB NDC");
    leg->SetBorderSize(0);
    leg->SetTextFont(43);
    leg->SetTextSize(23);
    leg->SetLineColor(1);
    leg->SetLineStyle(1);
    leg->SetLineWidth(1);
    leg->SetFillColor(0);
    leg->SetFillStyle(0);
    leg->AddEntry(hDataSys,"Data","fp");
    leg->AddEntry(simPowhegPythia6dijet,"POWHEG dijet + PYTHIA 6","pf");
  /*  leg->AddEntry(simPowhegPythia6,"POWHEG hvq + PYTHIA 6","pf");
    leg->AddEntry(simPowhegPythia8,"POWHEG hvq + PYTHIA 8","pf");
    leg->AddEntry(simPythia6,"PYTHIA 6 Perugia 2011","pf");
    leg->AddEntry(simPythia8,"PYTHIA 8 Monash 2013","pf");
    leg->AddEntry(simPythia8Soft2,"PYTHIA 8 Monash 2013 Soft mode 2","pf");*/
    TPaveText *pt[2];
    pt[0] = new TPaveText(0.2,0.85,0.85,0.95,"NB NDC");
    pt[1] = new TPaveText(0.2,0.7,0.85,0.85,"NB NDC");
    for(Int_t s = 0; s<2;s++){
        pt[s]->SetBorderSize(0);
        pt[s]->SetFillStyle(0);
        pt[s]->SetTextAlign(13);
        pt[s]->SetTextFont(43);
        pt[s]->SetTextSize(22);
    }
    pt[0]->AddText("ALICE Preliminary"); //uncomment
    pt[0]->AddText("pp, #sqrt{#it{s}} = 13 TeV");
    pt[1]->AddText(Form("charged jets, anti-#it{k}_{T}, #it{R} = 0.%d, |#it{#eta}_{lab}^{jet}| < 0.%d",4,9-4));
    pt[1]->AddText(Form ("with D^{0}, %d < #it{p}_{T,D^{0}} < %d GeV/#it{c}",static_cast<int>(2),static_cast<int>(36)));
    //pt[1]->AddText(Form ("%d < #it{p}_{T,jet} < %d GeV/#it{c}",static_cast<int>(15),static_cast<int>(50)));
    //pt[1]->AddText(Form ("with D^{0}, %d < #it{p}_{T,D^{0}} < %d GeV/#it{c}",static_cast<int>(3),static_cast<int>(36)));

    upPad->cd();
    leg->Draw();
    pt[0]->Draw();
    pt[1]->Draw();

    // ----------------- Terminate Canvas ---------------------
    TerminateCanvas(upPad,dowmPad,placeholder_up,placeholder_down);

    TFile *ofile = new TFile(Form("%s/JetPtSpectrum_final.root",outSpectraDir.Data()),"RECREATE");
    hData_binned->Write();
    hDataSys->Write();
    hData_binned_ratio->Write();
    hDataSysRatio->Write();
   /* simPowhegPythia6_cent->Write();
    simPowhegPythia6_up->Write();
    simPowhegPythia6_down->Write();
    simPowhegPythia6->Write();
    simPowhegPythia6_cent_R->Write();
    simPowhegPythia6_up_R->Write();
    simPowhegPythia6_down_R->Write();
    simPowhegPythia6_R->Write();*/

    ofile->Close();
    return;
}

void TerminateCanvas(TPad* pad1,TPad* pad2,TH1D* histo1,TH1D* histo2){
    pad1->cd();
    histo1->Draw("sameaxis");
    pad2->cd();
    histo2->Draw("sameaxis");
    histo2->Draw("sameaxig");
}

void PlaceOnPadSim(TPad* pad,TGraphAsymmErrors* histo, Int_t ci, Int_t style, Int_t linestyle){
    pad->cd();
    histo->SetFillColor(1);
    histo->SetFillStyle(0);
    histo->SetLineColor(ci);
    histo->SetMarkerColor(ci);
    histo->SetMarkerStyle(style);
    histo->SetLineStyle(linestyle);
   // histo->SetFillStyle(3005);
    //histo->SetMarkerSize(1.2f); //add up
    histo->SetLineWidth(2);
    TString odraaw = "";
    if(linestyle ==1) odraaw = "2p";
    else odraaw = "1p";
    histo->Draw(odraaw);
}

void PlaceOnPadData(TPad* pad,TGraphAsymmErrors* histo1, TH1D* histo2){
    pad->cd();
    Int_t ci = TColor::GetColor("#990000");
    histo1->SetLineColor(ci);
    histo1->SetMarkerColor(ci);
    histo1->SetMarkerStyle(20);
    histo1->SetMarkerSize(0.9f);
    ci = TColor::GetColor("#cccccc");
    histo1->SetFillColor(ci);
    histo1->SetLineColor(ci);
    histo1->Draw("2p");
    //data central w stat. unc.
    ci = TColor::GetColor("#990000");
    histo2->SetLineColor(ci);
    histo2->SetMarkerColor(ci);
    histo2->SetMarkerStyle(20);
    histo2->SetMarkerSize(0.9f);
    histo2->Draw("same p  e0 x0");
};

std::tuple<TCanvas*, TPad*, TPad*, TH1D*, TH1D*> PrepareCanvas(Int_t xAxisBins, Double_t *xAxis){
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
    TPad *pad_top = new TPad("pad_top", "pad_top",0,0.35,1,1);
    pad_top->Draw();
    pad_top->cd();
    pad_top->Range(-1.986821e-07,-4.69897,33.33333,0.3499945);
    pad_top->SetFillColor(0);
    pad_top->SetBorderMode(0);
    pad_top->SetBorderSize(2);
    if(!pdf)pad_top->SetLogy();
    pad_top->SetTickx(1);
    pad_top->SetTicky(1);
    pad_top->SetLeftMargin(0.18f);
    pad_top->SetBottomMargin(0);
    pad_top->SetFrameBorderMode(0);
    pad_top->SetFrameBorderMode(0);

    FinalSpectrum->cd();

    //Set primitives in bottom pad
    TPad *pad_bottom = new TPad("pad_bottom", "pad_bottom",0,0,1,0.35);
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
    pad_bottom->SetBottomMargin(0.27f);
    pad_bottom->SetFrameBorderMode(0);
    pad_bottom->SetFrameBorderMode(0);

    pad_top->cd();
    TH1D *hEmpty_up = new TH1D("hEmpty_up","Central Values",xAxisBins, xAxis);
    hEmpty_up->SetMinimum(2.e-06);
    hEmpty_up->SetMaximum(30);
    hEmpty_up->GetXaxis()->SetTitle("#it{p}_{T,ch jet} (GeV/#it{c})");
    hEmpty_up->GetXaxis()->SetLabelFont(42);
    hEmpty_up->GetXaxis()->SetLabelSize(0.035f);
    hEmpty_up->GetXaxis()->SetTitleSize(0.035f);
    hEmpty_up->GetXaxis()->SetTitleFont(42);
    if(!pdf)hEmpty_up->GetYaxis()->SetTitle("#frac{d^{2}#sigma}{d#it{p}_{T}d#it{#eta}} mb (GeV/#it{c})^{-1}");
    else hEmpty_up->GetYaxis()->SetTitle("Probability density");
    hEmpty_up->GetYaxis()->SetLabelFont(43);
    hEmpty_up->GetYaxis()->SetLabelSize(22);
    hEmpty_up->GetYaxis()->SetTitleSize(26);
    hEmpty_up->GetYaxis()->SetTitleOffset(2.f);
    hEmpty_up->GetYaxis()->SetTitleFont(43);
    hEmpty_up->Draw("axis");
    pad_bottom->cd();
    TH1D *hEmpty_bottom = new TH1D("hEmpty_bottom","Central Values",xAxisBins, xAxis);
    hEmpty_bottom->SetMinimum(0);
    hEmpty_bottom->SetMaximum(10.3);
    hEmpty_bottom->GetXaxis()->SetTitle("#it{p}_{T,ch jet} (GeV/#it{c})");
    hEmpty_bottom->GetXaxis()->SetLabelFont(43);
    hEmpty_bottom->GetXaxis()->SetLabelSize(22);
    hEmpty_bottom->GetXaxis()->SetTitleSize(26);
    hEmpty_bottom->GetXaxis()->SetTitleOffset(3.f);
    hEmpty_bottom->GetXaxis()->SetTitleFont(43);
    hEmpty_bottom->GetYaxis()->SetTitle("theory / data");
    hEmpty_bottom->GetYaxis()->SetNdivisions(509);
    hEmpty_bottom->GetYaxis()->CenterTitle();
    hEmpty_bottom->GetYaxis()->SetDecimals();
    hEmpty_bottom->GetYaxis()->SetLabelOffset(0.007);
    hEmpty_bottom->GetYaxis()->SetLabelFont(43);
    hEmpty_bottom->GetYaxis()->SetLabelSize(22);
    hEmpty_bottom->GetYaxis()->SetTitleSize(26);
    hEmpty_bottom->GetYaxis()->SetTitleOffset(2.f);
    hEmpty_bottom->GetYaxis()->SetTitleFont(43);
    hEmpty_bottom->Draw("axis");

    return std::make_tuple(FinalSpectrum,pad_top, pad_bottom, hEmpty_up,hEmpty_bottom);

}


std::tuple<TGraphAsymmErrors*, TH1D*,TGraphAsymmErrors*, TH1D*> GetData(TString dataFile, TString histBase, Double_t dataScaling, UInt_t nBins, Double_t *xBins, Double_t *systUncD_down, Double_t *systUncD_up){
    std::cout<<"get DATA"<<std::endl;
    TH1D *htmp = dynamic_cast<TH1D*>(GetInputHist(dataFile, histBase));
    std::cout<<"getting "<<dataFile<<" "<<histBase<<std::endl;
    if(!htmp)std::cout<<"not histo"<<std::endl;
    TH1D *hData_binned = dynamic_cast<TH1D*>(htmp->Rebin(nBins,"hData_binned", xBins));
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
    for(int j=0; j<nBins; j++){
          xval[j] = (xBins[j]+xBins[j+1]) / 2.;
          xvalwidth[j] = (xBins[j+1]-xBins[j]) / 2.;
          value[j] = hData_binned->GetBinContent(hData_binned->GetXaxis()->FindBin(xval[j]));
          Double_t error = hData_binned->GetBinError(hData_binned->GetXaxis()->FindBin(xval[j]));
          std::cout<<j<<" "<<xval[j]<<" "<<xvalwidth[j]<<" "<<value[j]<<" "<<error<<std::endl;
          sysuncAbs_down[j] = value[j] * systUncD_down[j];
          sysuncAbs_up[j] = value[j] * systUncD_up[j];
          if(value[j]>1E-9)statunc[j] = error/ value[j] *100;
    }
    TGraphAsymmErrors *grsysl = new TGraphAsymmErrors(nBins,xval,value,xvalwidth,xvalwidth,sysuncAbs_down,sysuncAbs_up);
    grsysl->SetName("haeData_binned_syst");

    TH1D *hData_binned_ratio = dynamic_cast<TH1D*>(hData_binned->Clone("hData_binned_ratio"));
      double *sysuncRatio_up = new double[nBins];
      double *sysuncRatio_down = new double[nBins];
      double *valRatio = new double[nBins];
      for(int j=0; j<nBins; j++){
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
      TGraphAsymmErrors *grsysRatio = new TGraphAsymmErrors(nBins,xval,valRatio,xvalwidth,xvalwidth,sysuncRatio_down,sysuncRatio_up);
      grsysRatio->SetName("haeData_binned_syst_ratio");
    return std::make_tuple(grsysl, hData_binned, grsysRatio,hData_binned_ratio);
}


std::tuple<TGraphAsymmErrors*, TH1D*, TH1D*, TH1D*> GetSim(Int_t nFiles, TString *names, TString simDir, Double_t simScaling, UInt_t nBins, Double_t *xBins){
    TH1D *hPrompt[nFiles];
    TH1D *hPrompt_binned[nFiles];

    for (int nr=0; nr<nFiles; nr++){
        TString file = simDir;
        file += "/JetPt_";
        file += names[nr];
        file += "_Dpt"; file += 2; file += "_"; file += 36;
        //file += "_Jetpt"; file += 15; file += "_"; file += 50;
        file += "_Dzero";
        file += ".root";
        TH1D *htmp = nullptr;
        htmp = dynamic_cast<TH1D*>(GetInputHist(file, "hPt"));
        if(!htmp) std::cout<<"no histogram in file: "<<file<<std::endl;
        htmp->GetYaxis()->SetTitle("d#sigma/dp_{T} (mb)");
      //  htmp = dynamic_cast<TH1D*>(GetInputHist(file, "hz"));
       // if(!htmp) std::cout<<"no histogram in file: "<<file<<std::endl;
        htmp->GetYaxis()->SetTitle("d#sigma/dz (mb)");
        hPrompt[nr] = dynamic_cast<TH1D*>(htmp->Clone(Form("hPrompt_%d",nr)));
        hPrompt_binned[nr] = dynamic_cast<TH1D*>(htmp->Rebin(nBins,Form("hPrompt_binned_%d",nr),xBins));
        if(pdf){
            hPrompt_binned[nr]->Scale(1./hPrompt_binned[nr]->Integral());
            hPrompt_binned[nr]->Scale(1,"width");
        }
    }

    TH1D *hPrompt_central_binned = dynamic_cast<TH1D*>(hPrompt_binned[0]->Clone("hPrompt_central_binned"));
    setHistoDetails(hPrompt_central_binned,4,24);

    // get up unc
    TH1D * hPrompt_up = GetUpSys(hPrompt_binned,nFiles);
    setHistoDetails(hPrompt_up,4,24,0,2,2);
    // get down unc
    TH1D *hPrompt_down = GetDownSys(hPrompt_binned,nFiles);
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
    for(int j=0; j<nBins; j++){
          xval[j] = (xBins[j]+xBins[j+1]) / 2.;
          xvalwidth[j] = (xBins[j+1]-xBins[j]) / 2.;
          valuetheory[j] = hPrompt_central_binned->GetBinContent(hPrompt_central_binned->GetXaxis()->FindBin(xval[j]));
          valuetheoryerrup[j] = hPrompt_up->GetBinContent(hPrompt_up->GetXaxis()->FindBin(xval[j])) - valuetheory[j];
          valuetheoryerrdown[j] = valuetheory[j] - hPrompt_down->GetBinContent(hPrompt_up->GetXaxis()->FindBin(xval[j]));

    }
    TGraphAsymmErrors *grsystheory = new TGraphAsymmErrors(nBins,xval,valuetheory,xvalwidth,xvalwidth,valuetheoryerrdown,valuetheoryerrup);
    return std::make_tuple(grsystheory, hPrompt_central_binned, hPrompt_up,hPrompt_down);
}

std::tuple<TGraphAsymmErrors*, TH1D*, TH1D*, TH1D*> GetDataSimRatio(TH1D *data_cent, TH1D *sim_cent, TH1D *sim_up, TH1D *sim_down, UInt_t nBins, Double_t *xBins){
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
    TGraphAsymmErrors *grsystheoryratio = new TGraphAsymmErrors(nBins,ptvaltheoryratio,valuetheoryratio,ptvalunctheoryratio,ptvalunctheoryratio,valuetheoryerrdownratio,valuetheoryerrupratio);
    return std::make_tuple(grsystheoryratio, hPrompt_central_binned_ratio, hPrompt_up_ratio,hPrompt_down_ratio);
}

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

void setHistoDetails(TH1 *hh, Color_t color, Style_t Mstyle, Double_t Msize, Width_t Lwidth, Style_t Lstyle){
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
