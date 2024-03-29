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
#include "style.C"

void setHistoDetails(TH1 *h, Color_t color, Style_t Mstyle, Width_t width);

 double zmin = 0, zmax = 2.;
 //double jetmin = 2, jetmax = 50;
 double plotmin = 2, plotmax = 36;


    const int ptbinsJetN = 11;
    float ptJetbins[ptbinsJetN+1] = {2,3,4,5,6,7,8,10,12,16,24,36};

    int promptColor = kRed+1;
    int nonpromptColor = kBlue+1;


void drawEff( int Rpar = 4 )
{

    style();
    gStyle->SetOptStat(000);

    gStyle->SetLegendFont(42);
    //gStyle->SetLegendTextSize(0.05);


//stringstream sst;
//sst.clear(); sst.str("");


   //TFile *inFilePrompt = new TFile("out_806Preliminary/DjetEff_prompt_jetpt2_50.root","read");
   //TFile *inFileFD = new TFile("out_806Base_FD/DjetEff_nonPrompt_jetpt2_50.root","read");

  // TFile *inFilePrompt = new TFile("/home/jackbauer/Work/alice/analysis/pp5TeV/D0jet/results_cutTight/DzeroR03_def_437_old0/Default/efficiency/DjetEff_prompt_jetpt5.00_50.00.root","read");
  // TFile *inFileFD = new TFile("/home/jackbauer/Work/alice/analysis/pp5TeV/D0jet/results_cutTight/DzeroR03_def_437_old0/Default/efficiency/DjetEff_nonPrompt_jetpt5.00_50.00.root","read");

   TFile *inFilePrompt = new TFile("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_X04/Default_AnalysisResults_Run2w18b.root/efficiency/DjetEff_prompt_jetpt5.00_50.00.root","read");
   TFile *inFileFD = new TFile("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_X04/Default_AnalysisResults_Run2w18b.root/efficiency/DjetEff_nonPrompt_jetpt5.00_50.00.root","read");


    TH1F *hEffPrompt = (TH1F*)inFilePrompt->Get("hEff_reb");
    TH1F *hEffNonPrompt = (TH1F*)inFileFD->Get("hEff_reb");

    hEffPrompt->SetTitle("");
            hEffPrompt->SetMarkerColor(promptColor);
            hEffPrompt->SetLineColor(promptColor);
            hEffPrompt->SetMarkerStyle(20);
            hEffPrompt->SetMarkerSize(1.2);
            hEffPrompt->GetXaxis()->SetTitle("#it{p}_{T,D^{0}} (GeV/#it{c})");
            //hmass[i]->GetYaxis()->SetTitle(Form("Entries/%.1f MeV/#it{c^{2}}",hmass[i]->GetBinWidth(1)*1000));
            //hEffPrompt->GetYaxis()->SetTitle("D^{*+} Acceptance #times Efficiency");
            hEffPrompt->GetYaxis()->SetTitle("Acceptance #times Efficiency");
            hEffPrompt->GetXaxis()->SetLabelSize(0.04);
            hEffPrompt->GetXaxis()->SetTitleSize(0.05);
            hEffPrompt->GetXaxis()->SetTitleOffset(1.);
            hEffPrompt->GetYaxis()->SetTitleOffset(1.1);
            hEffPrompt->GetYaxis()->SetLabelSize(0.045);
            hEffPrompt->GetYaxis()->SetTitleSize(0.05);
            hEffPrompt->GetXaxis()->SetRangeUser(plotmin,plotmax);
            hEffPrompt->SetMaximum(hEffPrompt->GetMaximum()*3.5);


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
            hEffNonPrompt->SetMaximum(hEffNonPrompt->GetMaximum()*2);


    TLegend *leg = new TLegend(0.5,0.25,0.85,0.40);
    leg->SetTextSize(0.045);
    leg->AddEntry(hEffPrompt,"Prompt D^{0}","p");
    leg->AddEntry(hEffNonPrompt,"Feed-down D^{0}","p");

    TPaveText *pvALICE = new TPaveText(0.15,0.85,0.8,0.9,"brNDC");
    pvALICE->SetFillStyle(0);
    pvALICE->SetBorderSize(0);
    pvALICE->SetTextFont(42);
    pvALICE->SetTextSize(0.045);
    pvALICE->SetTextAlign(11);
    pvALICE->AddText("This Thesis");

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
    pvD->AddText("D^{0} #rightarrow K^{-}#pi^{+} and charge conj.");

      TPaveText *pvJet = new TPaveText(0.42,0.6-shift,0.9,0.64-shift,"brNDC");
    pvJet->SetFillStyle(0);
    pvJet->SetBorderSize(0);
    pvJet->SetTextFont(42);
    pvJet->SetTextSize(0.045);
    pvJet->SetTextAlign(11);
    //pvJet->AddText("in Charged Jets, Anti-#it{k}_{T}, #it{R} = 0.4");
    pvJet->AddText("in charged jets, anti-#it{k}_{T}, #it{R} = 0.4");


   // TPaveText *pvEta = new TPaveText(0.42,0.54-shift,0.8,0.59-shift,"brNDC");
    TPaveText *pvEta = new TPaveText(0.425,0.54-shift,0.8,0.59-shift,"brNDC");
    pvEta->SetFillStyle(0);
    pvEta->SetBorderSize(0);
    pvEta->SetTextFont(42);
    pvEta->SetTextSize(0.045);
    pvEta->SetTextAlign(11);
    pvEta->AddText("|#it{#eta}_{lab}^{jet}| < 0.5");
    //pvEta->AddText("|#it{#eta}_{jet}| < 0.6");

    TCanvas *cEff = new TCanvas("cEff","cEff",1000,800);
    //cEff->SetBatch();
    cEff->SetLogy();
    //cMass->Divide(3,1);
    //cMass->cd(1);
    hEffPrompt->Draw();
    hEffNonPrompt->Draw("same");

    pvALICE->Draw("same");
    pvEn->Draw("same");
    pvJet->Draw("same");
    pvD->Draw("same");
    pvEta->Draw("same");
    leg->Draw("same");


  //  cEff->SaveAs("DjetEff_Sim_2.png");
    cEff->SaveAs("DjetEff_thesis_2.pdf");
  //  cEff->SaveAs("DjetEff_Sim_2.eps");
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

  //  c->SaveAs(name+".png");
    c->SaveAs(name+".pdf");

}
