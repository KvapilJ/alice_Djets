#include <string>
#include <sstream>
#include <iostream>
#include "TH1.h"
#include "TStyle.h"
#include "TFile.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "TCanvas.h"
#include "TString.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TGraphAsymmErrors.h"
#include "style.C"

void setHistoDetails(TH1 *h, Color_t color, Style_t Mstyle, Width_t width);

 //double zmin = 0, zmax = 2.;
 //double jetmin = 2, jetmax = 50;
 double plotmin = 5, plotmax = 50;


  //  int ptbinsJetN;
   // float ptJetbins[12];

    int promptColor = kGreen+1;
    int nonpromptColor = kBlue+1;


void drawRSZ()
{

    style();
    gStyle->SetOptStat(000);

    gStyle->SetLegendFont(42);
    //gStyle->SetLegendTextSize(0.05);


//stringstream sst;
//sst.clear(); sst.str("");


   //TFile *inFilePrompt = new TFile("out_806Preliminary/DjetEff_prompt_jetpt2_50.root","read");
   //TFile *inFileFD = new TFile("out_806Base_FD/DjetEff_nonPrompt_jetpt2_50.root","read");

  // TFile *inFilePrompt = new TFile("/home/jackbauer/Work/alice/analysis/pp5TeV/D0jet/results_cutTight/DzeroR03_def_437_old0/Default/efficiency/DjetEff_prompt_jetpt5_50.root","read");
  // TFile *inFileFD = new TFile("/home/jackbauer/Work/alice/analysis/pp5TeV/D0jet/results_cutTight/DzeroR03_def_437_old0/Default/efficiency/DjetEff_nonPrompt_jetpt5_50.root","read");
    Int_t z1 = 2;
    const int ptbinsJetN = 5;
    float ptJetbins[ptbinsJetN+1] = {2,3,4,5,6,7};

  /*  Int_t z1 = 5;
 const int ptbinsJetN = 8;
 float ptJetbins[ptbinsJetN+1] = {5,6,7,8,10,12,16,24,36};
*/
   TFile *inFileFD1 = new TFile(Form("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_z04F/Default_AnalysisResults_Run2w18b.root/signalExtraction/JetPtSpectra_SB_eff%d.root",z1),"read");
   TFile *inFileFD2 = new TFile(Form("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_z04F/ReflSys15_AnalysisResults_Run2w18b.root/signalExtraction/JetPtSpectra_SB_eff%d.root",z1),"read");
   TFile *inFileFD3 = new TFile(Form("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_z04F/ReflSys05_AnalysisResults_Run2w18b.root/signalExtraction/JetPtSpectra_SB_eff%d.root",z1),"read");



    TH1D *hFD_ratio = (TH1D*)inFileFD1->Get("hReflRS");
    TH1D *hFD_ratio_up = (TH1D*)inFileFD2->Get("hReflRS");
    TH1D *hFD_ratio_down = (TH1D*)inFileFD3->Get("hReflRS");

    //TGraphAsymmErrors *hFD_ratio_err = new TGraphAsymmErrors(hFD_ratio);
   /* for(Int_t i = 1; i <= ptbinsJetN;i++){
        hFD_ratio_err->SetPointEYhigh(i,hFD_ratio_up->GetBinContent(i)-hFD_ratio->GetBinContent(i));
        std::cout<<hFD_ratio_up->GetBinCenter(i)<<" "<<hFD_ratio->GetBinCenter(i)<<" "<<hFD_ratio_up->GetBinContent(i)<<" "<<hFD_ratio->GetBinContent(i)<<" "<<hFD_ratio_up->GetBinContent(i)-hFD_ratio->GetBinContent(i)<<std::endl;
        hFD_ratio_err->SetPointEYlow(i,hFD_ratio->GetBinContent(i)-hFD_ratio_down->GetBinContent(i));
    }*/

    Double_t *xAxis = new double[ptbinsJetN+1];
    for(int k=0; k<ptbinsJetN+1; k++) xAxis[k] = ptJetbins[k];

    Double_t ptvaltheory[ptbinsJetN+1];
    Double_t ptvalunctheory[ptbinsJetN+1];
    Double_t valuetheory[ptbinsJetN+1];
    Double_t valuetheoryerrup[ptbinsJetN+1];
    Double_t valuetheoryerrdown[ptbinsJetN+1];
    for(int j=0; j<ptbinsJetN; j++){
            ptvaltheory[j] = (xAxis[j]+xAxis[j+1]) / 2.;
            ptvalunctheory[j] = (xAxis[j+1]-xAxis[j]) / 2.;
            valuetheory[j] = hFD_ratio->GetBinContent(hFD_ratio->GetXaxis()->FindBin(ptvaltheory[j]));
            valuetheoryerrup[j] = hFD_ratio_up->GetBinContent(hFD_ratio_up->GetXaxis()->FindBin(ptvaltheory[j])) - valuetheory[j];
            valuetheoryerrdown[j] = valuetheory[j] - hFD_ratio_down->GetBinContent(hFD_ratio_down->GetXaxis()->FindBin(ptvaltheory[j]));
std::cout<<ptvaltheory[j]<<" "<<ptvalunctheory[j]<<" "<<valuetheoryerrup[j]<<" "<<valuetheory[j]<<" "<<valuetheoryerrdown[j]<<std::endl;

    }
    TGraphAsymmErrors *hFD_ratio_err = new TGraphAsymmErrors(ptbinsJetN,ptvaltheory,valuetheory,ptvalunctheory,ptvalunctheory,valuetheoryerrdown,valuetheoryerrup);



            hFD_ratio->SetTitle("");
            hFD_ratio->SetMarkerColor(promptColor);
            hFD_ratio->SetLineColor(promptColor);
            hFD_ratio->SetMarkerStyle(20);
            hFD_ratio->SetMarkerSize(1.2);
            hFD_ratio->GetXaxis()->SetTitle("#it{p}_{T}^{D} (GeV/#it{c})");
            hFD_ratio->GetYaxis()->SetTitle("Reflection/Signal Fraction");
            hFD_ratio->GetXaxis()->SetLabelSize(0.04);
            hFD_ratio->GetXaxis()->SetTitleSize(0.05);
            hFD_ratio->GetXaxis()->SetTitleOffset(1.);
            hFD_ratio->GetYaxis()->SetTitleOffset(1.1);
            hFD_ratio->GetYaxis()->SetLabelSize(0.045);
            hFD_ratio->GetYaxis()->SetTitleSize(0.05);
            hFD_ratio->GetXaxis()->SetRangeUser(plotmin,plotmax);
            hFD_ratio->SetMaximum(hFD_ratio->GetMaximum()*3);

            hFD_ratio_err->SetTitle("");
            hFD_ratio_err->SetMarkerColor(promptColor);
            hFD_ratio_err->SetLineColor(promptColor);
            hFD_ratio_err->SetLineWidth(2.0);
            //hFD_ratio_err->SetFillColor(promptColor);
            //hFD_ratio_err->SetFillColorAlpha(kGreen,0.9);
            hFD_ratio_err->SetMarkerStyle(20);
            hFD_ratio_err->SetMarkerSize(1.2);
            hFD_ratio_err->GetXaxis()->SetTitle("#it{p}_{T}^{D} (GeV/#it{c})");
            hFD_ratio_err->GetYaxis()->SetTitle("Reflection/Signal Fraction");
            hFD_ratio_err->GetXaxis()->SetLabelSize(0.04);
            hFD_ratio_err->GetXaxis()->SetTitleSize(0.05);
            hFD_ratio_err->GetXaxis()->SetTitleOffset(1.);
            hFD_ratio_err->GetYaxis()->SetTitleOffset(1.1);
            hFD_ratio_err->GetYaxis()->SetLabelSize(0.045);
            hFD_ratio_err->GetYaxis()->SetTitleSize(0.05);
            hFD_ratio_err->GetXaxis()->SetRangeUser(plotmin,plotmax);
            hFD_ratio_err->SetMaximum(1);
/*
            hFD_ratio_down->SetTitle("");
            hFD_ratio_down->SetMarkerColor(promptColor);
            hFD_ratio_down->SetLineColor(promptColor);
            hFD_ratio_down->SetMarkerStyle(20);
            hFD_ratio_down->SetMarkerSize(1.2);
            hFD_ratio_down->GetXaxis()->SetTitle("#it{p}_{T,ch jet} (GeV/#it{c})");
            hFD_ratio_down->GetYaxis()->SetTitle("B Feed-Down Fraction");
            hFD_ratio_down->GetXaxis()->SetLabelSize(0.04);
            hFD_ratio_down->GetXaxis()->SetTitleSize(0.05);
            hFD_ratio_down->GetXaxis()->SetTitleOffset(1.);
            hFD_ratio_down->GetYaxis()->SetTitleOffset(1.1);
            hFD_ratio_down->GetYaxis()->SetLabelSize(0.045);
            hFD_ratio_down->GetYaxis()->SetTitleSize(0.05);
            hFD_ratio_down->GetXaxis()->SetRangeUser(plotmin,plotmax);
            hFD_ratio_down->SetMaximum(hFD_ratio_down->GetMaximum()*3);*/

/*
   hEffNonPrompt->SetTitle("");
            hEffNonPrompt->SetMarkerColor(nonpromptColor);
            hEffNonPrompt->SetLineColor(nonpromptColor);
            hEffNonPrompt->SetMarkerStyle(21);
            hEffNonPrompt->SetMarkerSize(1.2);
            hEffNonPrompt->GetXaxis()->SetTitle("#it{p}_{T}^{D} (GeV/#it{c})");
            //hmass[i]->GetYaxis()->SetTitle(Form("Entries/%.1f MeV/#it{c^{2}}",hmass[i]->GetBinWidth(1)*1000));
            hEffNonPrompt->GetYaxis()->SetTitle("Acceptance #times Efficiency");
            hEffNonPrompt->GetXaxis()->SetLabelSize(0.04);
            hEffNonPrompt->GetXaxis()->SetTitleSize(0.05);
            hEffNonPrompt->GetXaxis()->SetTitleOffset(1.);
            hEffNonPrompt->GetYaxis()->SetLabelSize(0.045);
            hEffNonPrompt->GetYaxis()->SetTitleSize(0.05);
            hEffNonPrompt->GetXaxis()->SetRangeUser(plotmin,plotmax);
            hEffNonPrompt->SetMaximum(hEffNonPrompt->GetMaximum()*2);*/


    TLegend *leg = new TLegend(0.15,0.6,0.6,0.7);
    //leg->SetTextSize(0.045);
    leg->SetTextFont(42);
    leg->SetTextSize(0.045);
    leg->AddEntry(hFD_ratio,"B Feed-Down Fraction","p");
    leg->AddEntry(hFD_ratio_err,"POWHEG Systematic Uncertainty","F");
/*
    TPaveText *pvALICE = new TPaveText(0.15,0.85,0.8,0.9,"brNDC");
    pvALICE->SetFillStyle(0);
    pvALICE->SetBorderSize(0);
    pvALICE->SetTextFont(42);
    pvALICE->SetTextSize(0.045);
    pvALICE->SetTextAlign(11);
    pvALICE->AddText("ALICE Preliminary");

    TPaveText *pvEn= new TPaveText(0.15,0.80,0.8,0.85,"brNDC");
    pvEn->SetFillStyle(0);
    pvEn->SetBorderSize(0);
    pvEn->SetTextFont(42);
    pvEn->SetTextSize(0.045);
    pvEn->SetTextAlign(11);
    //pvEn->AddText("p-Pb, #sqrt{#it{s}_{NN}} = 5.02 TeV");
    pvEn->AddText("pp, #sqrt{#it{s}} = 13 TeV");

    double shift = 0.1;
    TPaveText *pvD = new TPaveText(0.42,0.65-shift,0.9,0.69-shift,"brNDC");
    pvD->SetFillStyle(0);
    pvD->SetBorderSize(0);
    pvD->SetTextFont(42);
    pvD->SetTextSize(0.045);
    pvD->SetTextAlign(11);
    //pvD->AddText("D^{*+} #rightarrow D^{0}#pi^{+} and charge conj.");
    pvD->AddText("D^{0} #rightarrow K^{-}#pi^{+} and charge conj., #it{p}_{T,D}>2 GeV/#it{c}");

      TPaveText *pvJet = new TPaveText(0.42,0.6-shift,0.9,0.64-shift,"brNDC");
    pvJet->SetFillStyle(0);
    pvJet->SetBorderSize(0);
    pvJet->SetTextFont(42);
    pvJet->SetTextSize(0.045);
    pvJet->SetTextAlign(11);
    //pvJet->AddText("in Charged Jets, Anti-#it{k}_{T}, #it{R} = 0.4");
    pvJet->AddText("in Charged Jets, Anti-#it{k}_{T}, #it{R} = 0.4");


   // TPaveText *pvEta = new TPaveText(0.42,0.54-shift,0.8,0.59-shift,"brNDC");
    TPaveText *pvEta = new TPaveText(0.425,0.54-shift,0.8,0.59-shift,"brNDC");
    pvEta->SetFillStyle(0);
    pvEta->SetBorderSize(0);
    pvEta->SetTextFont(42);
    pvEta->SetTextSize(0.045);
    pvEta->SetTextAlign(11);
    pvEta->AddText("|#it{#eta}_{lab}^{jet}| < 0.5");
    //pvEta->AddText("|#it{#eta}_{jet}| < 0.6");*/

    TPaveText *pt = new TPaveText(0.15,0.67,0.8,0.95,"NB NDC");
    pt->SetBorderSize(0);
    pt->SetFillStyle(0);
    pt->SetTextAlign(13);
    pt->SetTextFont(42);
    pt->SetTextSize(0.045);
    //TText *text = pt->AddText("ALICE Preliminary");
    TText *text = new TText;
    text = pt->AddText("This Thesis"); //uncomment
    text = pt->AddText("pp, #sqrt{#it{s}} = 13 TeV");
    text = pt->AddText("PYTHIA6+GEANT3");
    text = pt->AddText(Form("charged jets with D^{0}, anti-#it{k}_{T}, #it{R} = 0.%d, |#it{#eta}_{lab}^{jet}| < 0.%d",4,5));
    if(z1==2) text = pt->AddText(Form ("%d < #it{p}_{T,ch jet} < %d GeV/#it{c}",2,7));
    if(z1==5) text = pt->AddText(Form ("%d < #it{p}_{T,ch jet} < %d GeV/#it{c}",5,36));
  //  if(z1==2)text = pt->AddText(Form("%d < #it{p}_{T,D^{0}} < %d GeV/#it{c}, %d < #it{p}_{T,jet} < %d GeV/#it{c}",2,7,5,7));
  //  if(z1==5)text = pt->AddText(Form("%d < #it{p}_{T,D^{0}} < %d GeV/#it{c}, %d < #it{p}_{T,jet} < %d GeV/#it{c}",5,36,15,50));

    TCanvas *cEff = new TCanvas("cEff","cEff",1000,800);
    //cEff->SetBatch();
    //cEff->SetLogy();
    //cMass->Divide(3,1);
    //cMass->cd(1);
  //  hFD_ratio->Draw();
  //  hFD_ratio_up->Draw("same");
  //  hFD_ratio_down->Draw("same");
    //hFD_ratio->Draw();
    hFD_ratio->GetYaxis()->SetRangeUser(0,0.8);
   if(z1==2) hFD_ratio->GetXaxis()->SetRangeUser(2,7);
    if(z1==5)hFD_ratio->GetXaxis()->SetRangeUser(5,36);
    hFD_ratio->Draw("axis");
    hFD_ratio_err->Draw("5e");
    hFD_ratio->Draw("same p  e0 x0");
    hFD_ratio->Draw("sameaxis");
    pt->Draw("same");
    hFD_ratio->Draw("sameaxig");


 //   pvALICE->Draw("same");
 //   pvEn->Draw("same");
  //  pvJet->Draw("same");
  //  pvD->Draw("same");
  //  pvEta->Draw("same");
  //  leg->Draw("same");


  //  cEff->SaveAs("DjetFD_2.png");
    //cEff->SaveAs("DjetRS_thesis.pdf");
    cEff->SaveAs(Form("DjetRS_z%d_thesis.pdf",z1));
   // cEff->SaveAs("DjetFD_2.eps");
   // cEff->Print("DjetEff_Sim_log.pdf");
   // cEff->Print("DjetEff_Sim_log.eps");
   // cEff->Print("DjetEff_Sim_log.png");



}

void setHistoDetails(TH1 *h, Color_t color, Style_t Mstyle, Size_t size = 0.9, Width_t width=2, int scale = 0){

    if(scale)h->Scale(1,"width");
    h->SetMarkerStyle(Mstyle);
    h->SetMarkerColor(color);
    h->SetMarkerSize(size);
    h->SetLineColor(color);
    h->SetLineWidth(width);
    h->SetTitle("");
    h->GetXaxis()->SetTitle("p_{T}^{D^{*+}}(GeV/c)");

    return;

}

void SaveCanvas(TCanvas *c, TString name = "tmp"){

    c->SaveAs(name+".png");
    c->SaveAs(name+".pdf");

}
