#include <string>
#include <sstream>
#include <iostream>
#include "TH1.h"
#include "TH2.h"
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


    const int ptbinsJetN = 11;
    Double_t ptJetbins[ptbinsJetN+1] = {2.,3.,4.,5.,6.,7.,8.,10.,12.,16.,24.,36.};
  //  Double_t xbins[27] = {2.,3.,4.,5.,6.,7.,8.,10.,12.,16.,24.,36.};

    int promptColor = kGreen+1;
    int nonpromptColor = kBlue+1;


void drawSysMulti()
{

    style();
    gStyle->SetOptStat(000);

    gStyle->SetLegendFont(42);


    TFile *inFile = new TFile("multi56.root","read");

    TH1F *histo[5];
    histo[0] = (TH1F*)inFile->Get("hSigmaAll");
    histo[1] = (TH1F*)inFile->Get("hMeanAll");
    histo[2] = (TH1F*)inFile->Get("hChi2All");
    histo[3] = (TH1F*)inFile->Get("hBkgAll");
    histo[4] = (TH1F*)inFile->Get("hRawYieldAll");
    for(int i = 0; i<5;i++){
        if(!histo[i])std::cout<<i<<" not found"<<std::endl;
    }


    for(int i = 0; i<5;i++){
    histo[i]->SetTitle("");
    histo[i]->GetXaxis()->SetTitle("Trial");
    //histo[i]->GetYaxis()->SetTitle("Uncertainty (%)");
    histo[i]->GetXaxis()->SetLabelSize(0.04);
    histo[i]->GetXaxis()->SetTitleSize(0.05);
    histo[i]->GetXaxis()->SetTitleOffset(1.);
    if(i<2)histo[i]->GetYaxis()->SetTitleOffset(1.7);
    if(i==2)histo[i]->GetYaxis()->SetTitleOffset(1.2);
    if(i==3 || i==4)histo[i]->GetYaxis()->SetTitleOffset(1.4);
    histo[i]->GetYaxis()->SetLabelSize(0.045);
    histo[i]->GetYaxis()->SetTitleSize(0.05);
    histo[i]->GetYaxis()->SetDecimals();
    histo[i]->SetMarkerColor(kBlue+1);
    histo[i]->SetLineColor(kBlue+1);
    //histo[i]->GetXaxis()->SetRangeUser(-5,100);
    }
    histo[0]->GetYaxis()->SetRangeUser(0.0076,0.012);
  //  histo[0]->GetYaxis()->SetNdivisions(5);
    histo[1]->GetYaxis()->SetRangeUser(1.864,1.869);
    histo[2]->GetYaxis()->SetRangeUser(0.6,3.0);
    TString tmp = "Reduced ";
    histo[2]->GetYaxis()->SetTitle(tmp+histo[2]->GetYaxis()->GetTitle()+" of the mass fit");
    histo[3]->GetYaxis()->SetRangeUser(1300,2100);
    histo[4]->GetYaxis()->SetRangeUser(3000,4000);


    TLegend *leg = new TLegend(0.15,0.6,0.6,0.7);
    leg->SetTextFont(42);
    leg->SetTextSize(0.045);
    //leg->AddEntry(histo,"B Feed-Down Fraction","p");


   // TPaveText *pt = new TPaveText(0.15,0.67,0.8,0.95,"NB NDC");
    TPaveText *pt = new TPaveText(0.15,0.35,0.8,0.95,"NB NDC");
    pt->SetBorderSize(0);
    pt->SetFillStyle(0);
    pt->SetTextAlign(13);
    pt->SetTextFont(42);
    pt->SetTextSize(0.075);
    //TText *text = pt->AddText("ALICE Preliminary");
    TText *text = new TText;
    text = pt->AddText("This Thesis"); //uncomment
    text = pt->AddText("pp, #sqrt{#it{s}} = 13 TeV");
    //text = pt->AddText("PYTHIA6+GEANT3");
    text = pt->AddText("D^{0} in charged jets, anti-#it{k}_{T}");
    text = pt->AddText(Form("#it{R} = 0.%d, |#it{#eta}_{lab}^{jet}| < 0.%d",4,5));
    text = pt->AddText(Form ("%d < #it{p}_{T,D^{0}} < %d GeV/#it{c}",5,6));

/*    TH1D *histo2 = (TH1D*)histo->Rebin(ptbinsJetN+1,"hnew",ptJetbins);
    histo2->SetLineColor(kRed);
    histo->GetXaxis()->SetRangeUser(2.,36.);
    histo->GetYaxis()->SetRangeUser(3.,7.);
    histo2->GetXaxis()->SetRangeUser(2.,36.);
    histo2->GetYaxis()->SetRangeUser(3.,7.);
    histo2->GetYaxis()->SetDecimals();
    histo->GetYaxis()->SetDecimals();
    histo->SetMarkerColor(kBlue+1);
    histo->SetLineColor(kBlue+1);
    histo2->SetLineColor(kRed);
    histo2->SetMarkerColor(kRed);
    histo2->SetLineWidth(2);*/
   // histo2->SetMarkerSize(0);




    TCanvas *cEff = new TCanvas("cEff","cEff",1000*3,800*2);
    cEff->Divide(3,2);

  //  hFD_ratio->GetYaxis()->SetRangeUser(0,0.8);
 //   hFD_ratio->GetXaxis()->SetRangeUser(2,36);
    for(int i = 0; i<5;i++){
    cEff->cd(i+1);
    cEff->cd(i+1)->SetLeftMargin(0.15);
    histo[i]->Draw();
    }
    cEff->cd(6);
    pt->Draw();

    cEff->SaveAs("Multi56.pdf");




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
