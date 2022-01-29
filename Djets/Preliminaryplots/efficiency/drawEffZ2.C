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




void drawEffZ2( int Rpar = 4 , int z = 0)
{

    style();
    gStyle->SetOptStat(000);

    gStyle->SetLegendFont(42);
    //gStyle->SetLegendTextSize(0.05);

    double zmin = 0, zmax = 2.;
    //double jetmin = 2, jetmax = 50;
    double plotmin=0, plotmax=0;
    int ptbinsJetN = 0;
    float ptJetbins[11];
    TFile *inFilePrompt[4];// = new TFile("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts/Default_AnalysisResults_Run2.root/efficiency/DjetEff_prompt_jetpt5.00_50.00.root","read");
    TFile *inFileFD[4];// = new TFile("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts/Default_AnalysisResults_Run2.root/efficiency/DjetEff_nonPrompt_jetpt5.00_50.00.root","read");


   inFilePrompt[0] = new TFile("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_z04F/Default_AnalysisResults_Run2w18b.root/efficiency/DjetEff_prompt_jetpt5.00_7.00.root","read");
   inFileFD[0] = new TFile("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_z04F/Default_AnalysisResults_Run2w18b.root/efficiency/DjetEff_nonPrompt_jetpt5.00_7.00.root","read");


        inFilePrompt[1] = new TFile("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_z04F/Default_AnalysisResults_Run2w18b.root/efficiency/DjetEff_prompt_jetpt7.00_10.00.root","read");
        inFileFD[1] = new TFile("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_z04F/Default_AnalysisResults_Run2w18b.root/efficiency/DjetEff_nonPrompt_jetpt7.00_10.00.root","read");


        inFilePrompt[2] = new TFile("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_z04F/Default_AnalysisResults_Run2w18b.root/efficiency/DjetEff_prompt_jetpt10.00_15.00.root","read");
        inFileFD[2] = new TFile("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_z04F/Default_AnalysisResults_Run2w18b.root/efficiency/DjetEff_nonPrompt_jetpt10.00_15.00.root","read");

        inFilePrompt[3] = new TFile("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_z04F/Default_AnalysisResults_Run2w18b.root/efficiency/DjetEff_prompt_jetpt15.00_50.00.root","read");
        inFileFD[3] = new TFile("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_z04F/Default_AnalysisResults_Run2w18b.root/efficiency/DjetEff_nonPrompt_jetpt15.00_50.00.root","read");


    Int_t jetpt[5] = {5,7,10,15,50};



       int promptColor[4] = {kRed+1,kBlue+1,kGreen+2,kMagenta+2};
       int nonpromptColor = kBlue+1;


//stringstream sst;
//sst.clear(); sst.str("");


   //TFile *inFilePrompt = new TFile("out_806Preliminary/DjetEff_prompt_jetpt2_50.root","read");
   //TFile *inFileFD = new TFile("out_806Base_FD/DjetEff_nonPrompt_jetpt2_50.root","read");

  // TFile *inFilePrompt = new TFile("/home/jackbauer/Work/alice/analysis/pp5TeV/D0jet/results_cutTight/DzeroR03_def_437_old0/Default/efficiency/DjetEff_prompt_jetpt5_50.root","read");
  // TFile *inFileFD = new TFile("/home/jackbauer/Work/alice/analysis/pp5TeV/D0jet/results_cutTight/DzeroR03_def_437_old0/Default/efficiency/DjetEff_nonPrompt_jetpt5_50.root","read");





    TH1F *hEffPrompt[4];//(TH1F*)hEffPrompttmp->Rebin(ptbinsJetN,"hEffPrompt",ptJetbins)
    TH1F *hEffNonPrompt[4];//(TH1F*)inFileFD->Get("hEff_reb");

    for(int i =0;i<4;i++){
        hEffPrompt[i] = (TH1F*)inFilePrompt[i]->Get("hEff_reb");
        hEffNonPrompt[i] = (TH1F*)inFileFD[i]->Get("hEff_reb");
    }

for(int i =0;i<4;i++){
    hEffPrompt[i]->SetTitle("");
            hEffPrompt[i]->SetMarkerColor(promptColor[i]);
            hEffPrompt[i]->SetLineColor(promptColor[i]);
            hEffPrompt[i]->SetMarkerStyle(20+i);
            hEffPrompt[i]->SetMarkerSize(1.2);
            hEffPrompt[i]->GetXaxis()->SetTitle("#it{p}_{T,D^{0}} (GeV/#it{c})");
            //hmass[i]->GetYaxis()->SetTitle(Form("Entries/%.1f MeV/#it{c^{2}}",hmass[i]->GetBinWidth(1)*1000));
            //hEffPrompt->GetYaxis()->SetTitle("D^{*+} Acceptance #times Efficiency");
            hEffPrompt[i]->GetYaxis()->SetTitle("Acceptance #times Efficiency");
            hEffPrompt[i]->GetXaxis()->SetLabelSize(0.04);
            hEffPrompt[i]->GetXaxis()->SetTitleSize(0.05);
            hEffPrompt[i]->GetXaxis()->SetTitleOffset(1.);
            hEffPrompt[i]->GetYaxis()->SetTitleOffset(1.1);
            hEffPrompt[i]->GetYaxis()->SetLabelSize(0.045);
            hEffPrompt[i]->GetYaxis()->SetTitleSize(0.05);
            hEffPrompt[i]->GetXaxis()->SetRangeUser(plotmin,plotmax);
         //   hEffPrompt[i]->SetMaximum(hEffPrompt->GetMaximum()*4.5);


   hEffNonPrompt[i]->SetTitle("");
            hEffNonPrompt[i]->SetMarkerColor(nonpromptColor);
            hEffNonPrompt[i]->SetLineColor(nonpromptColor);
            hEffNonPrompt[i]->SetMarkerStyle(21);
            hEffNonPrompt[i]->SetMarkerSize(1.2);
            hEffNonPrompt[i]->GetXaxis()->SetTitle("#it{p}_{T}^{D} (GeV/#it{c})");
            //hmass[i]->GetYaxis()->SetTitle(Form("Entries/%.1f MeV/#it{c^{2}}",hmass[i]->GetBinWidth(1)*1000));
            hEffNonPrompt[i]->GetYaxis()->SetTitle("Acceptance #times Efficiency");
            hEffNonPrompt[i]->GetXaxis()->SetLabelSize(0.04);
            hEffNonPrompt[i]->GetXaxis()->SetTitleSize(0.05);
            hEffNonPrompt[i]->GetXaxis()->SetTitleOffset(1.);
            hEffNonPrompt[i]->GetYaxis()->SetLabelSize(0.045);
            hEffNonPrompt[i]->GetYaxis()->SetTitleSize(0.05);
            hEffNonPrompt[i]->GetXaxis()->SetRangeUser(plotmin,plotmax);
           // hEffNonPrompt[i]->SetMaximum(hEffNonPrompt->GetMaximum()*2.5);

}
    TLegend *leg = nullptr;
    leg = new TLegend(0.5,0.2,0.85,0.45);
    leg->SetTextSize(0.038);
    leg->AddEntry(hEffPrompt[0],Form("%d < #it{p}_{T,jet} < %d GeV/#it{c}",5,7),"p");
    leg->AddEntry(hEffPrompt[1],Form("%d < #it{p}_{T,jet} < %d GeV/#it{c}",7,10),"p");
    leg->AddEntry(hEffPrompt[2],Form("%d < #it{p}_{T,jet} < %d GeV/#it{c}",10,15),"p");
    leg->AddEntry(hEffPrompt[3],Form("%d < #it{p}_{T,jet} < %d GeV/#it{c}",15,50),"p");

    //leg->AddEntry(hEffNonPrompt[0],"Feed-down D^{0}","p");

    TPaveText *pvALICE = new TPaveText(0.15,0.85,0.8,0.9,"brNDC");
    pvALICE->SetFillStyle(0);
    pvALICE->SetBorderSize(0);
    pvALICE->SetTextFont(42);
    pvALICE->SetTextSize(0.038);
    pvALICE->SetTextAlign(11);
    pvALICE->AddText("This Thesis");

    TPaveText *pvEn= new TPaveText(0.15,0.80,0.8,0.85,"brNDC");
    pvEn->SetFillStyle(0);
    pvEn->SetBorderSize(0);
    pvEn->SetTextFont(42);
    pvEn->SetTextSize(0.038);
    pvEn->SetTextAlign(11);
    //pvEn->AddText("p-Pb, #sqrt{#it{s}_{NN}} = 5.02 TeV");
    pvEn->AddText("PYTHIA 6 + GEANT 3, pp, #sqrt{#it{s}} = 13 TeV");

    double shift = -0.1;
    double shiftx = -0.26;

    TPaveText *pvD = new TPaveText(0.42+shiftx,0.65-shift,0.9+shiftx,0.69-shift,"brNDC");
    pvD->SetFillStyle(0);
    pvD->SetBorderSize(0);
    pvD->SetTextFont(42);
    pvD->SetTextSize(0.038);
    pvD->SetTextAlign(11);
    //pvD->AddText("D^{*+} #rightarrow D^{0}#pi^{+} and charge conj.");
    pvD->AddText("D^{0} #rightarrow K^{-}#pi^{+} and charge conj.");

      TPaveText *pvJet = new TPaveText(0.42+shiftx,0.6-shift,0.9+shiftx,0.64-shift,"brNDC");
    pvJet->SetFillStyle(0);
    pvJet->SetBorderSize(0);
    pvJet->SetTextFont(42);
    pvJet->SetTextSize(0.038);
    pvJet->SetTextAlign(11);
    //pvJet->AddText("in Charged Jets, Anti-#it{k}_{T}, #it{R} = 0.4");
    pvJet->AddText("in charged jets, anti-#it{k}_{T}, #it{R} = 0.4,");


   // TPaveText *pvEta = new TPaveText(0.42,0.54-shift,0.8,0.59-shift,"brNDC");
    TPaveText *pvEta = new TPaveText(0.425+shiftx,0.54-shift,0.8+shiftx,0.59-shift,"brNDC");
    pvEta->SetFillStyle(0);
    pvEta->SetBorderSize(0);
    pvEta->SetTextFont(42);
    pvEta->SetTextSize(0.038);
    pvEta->SetTextAlign(11);
    pvEta->AddText("|#it{#eta}_{lab}^{jet}| < 0.5");
    //pvEta->AddText("|#it{#eta}_{jet}| < 0.6");
  //  hEffPrompt->GetYaxis()->SetRangeUser(6E-2,2.0);
    const int     fptbinsDN = 11;
    double        fptbinsDA[fptbinsDN+1] = {2,3,4,5,6,7,8,10,12,16,24,36};

    TH1D *htmp = new TH1D("htmp","htmp",fptbinsDN,fptbinsDA);


    htmp->SetTitle("");
    htmp->SetMarkerSize(1.2);
    htmp->GetXaxis()->SetTitle("#it{p}_{T,D^{0}} (GeV/#it{c})");
    htmp->GetYaxis()->SetTitle("Acceptance #times Efficiency");
    htmp->GetXaxis()->SetLabelSize(0.04);
    htmp->GetXaxis()->SetTitleSize(0.05);
    htmp->GetXaxis()->SetTitleOffset(1.);
    htmp->GetYaxis()->SetTitleOffset(1.1);
    htmp->GetYaxis()->SetLabelSize(0.045);
    htmp->GetYaxis()->SetTitleSize(0.05);
    htmp->GetYaxis()->SetRangeUser(0,0.4);

    TCanvas *cEff = new TCanvas("cEff","cEff",1000,800);
    //cEff->SetBatch();
    //cEff->SetLogy();
    //cMass->Divide(3,1);
    //cMass->cd(1);
    htmp->Draw();
    hEffPrompt[0]->Draw("same");
    hEffPrompt[1]->Draw("same");
    hEffPrompt[2]->Draw("same");
    hEffPrompt[3]->Draw("same");


  //  hEffNonPrompt->Draw("same");

    pvALICE->Draw("same");
    pvEn->Draw("same");
    pvJet->Draw("same");
    pvD->Draw("same");
    pvEta->Draw("same");
    leg->Draw("same");


 //   cEff->SaveAs(Form("DjetEff_Sim_2_%d.png",z));
    cEff->SaveAs("prompteff.pdf");
  //  cEff->SaveAs(Form("DjetEff_Sim_2_%d.eps",z));
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
