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
#include "TF1.h"
#include "style.C"
#include <TPDF.h>

void setHistoDetails(TH1 *h, Color_t color, Style_t Mstyle, Width_t width);

 double zmin = 0, zmax = 2.;
    double jetmin = 0, jetmax = 50;
    double plotmin = 0, plotmax = 50;

    const int ptbinsDN = 11;
    float ptDbins[ptbinsDN+1] = {2,3,4,5,6,7,8,10,12,16,24,36};

    double efficiency[ptbinsDN];// = { 0.0353325, 0.0678059, 0.109101, 0.168871, 0.243708, 0.307365, 0.324496, 0.361858 };


    int massColor = kBlack;
    int signalColor = kRed+1;
    int SBColor = kGreen+3;

void drawSB2()
{

    style();
    gStyle->SetOptStat(000);
    gStyle->SetLegendFont(42);
    gStyle->SetPadLeftMargin(0.15);
    gStyle->SetPadRightMargin(0.02);
    gStyle->SetPadTopMargin(0.02);


    TFile *inFile = new TFile("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_X04/Default_AnalysisResults_Run2w18b.root/signalExtraction/JetPtSpectra_SB_eff.root","read");


     int bin1 = 0, bin2 = 1, bin3 = 2;


    TH1F *hmean = (TH1F*)inFile->Get("hmean");
    TH1F *hsigma = (TH1F*)inFile->Get("hsigma");
    TH1F *hsign = (TH1F*)inFile->Get("hsign");
    TH1F *hsb = (TH1F*)inFile->Get("hsb");

    TH1F* hmass[ptbinsDN];
    TH1F* hmass_l[ptbinsDN];
    TH1F* hmass_u[ptbinsDN];
    TH1F* hmass_c[ptbinsDN];
    TF1* fullfit[ptbinsDN];
    TF1* bkgfit[ptbinsDN];

    for(int i=0; i<ptbinsDN; i++){
            hmass[i] = (TH1F*)inFile->Get(Form("hmass_%d",i));
            hmass[i]->SetTitle("");
            hmass[i]->SetMarkerColor(massColor);
            hmass[i]->SetLineColor(massColor);
            hmass[i]->SetMarkerStyle(20);
            hmass[i]->SetMarkerSize(1.2);
            hmass[i]->GetXaxis()->SetRangeUser(1.72,2.0);
            hmass[i]->GetXaxis()->SetTitle("#it{M}(K#pi) (GeV/#it{c}^{2})");
            hmass[i]->GetYaxis()->SetTitle(Form("Entries/%.0f MeV/#it{c}^{2}",hmass[i]->GetBinWidth(1)*1000));
            hmass[i]->GetXaxis()->SetLabelSize(0.045);
            hmass[i]->GetXaxis()->SetTitleSize(0.055);
            hmass[i]->GetYaxis()->SetLabelSize(0.045);
            hmass[i]->GetYaxis()->SetTitleSize(0.055);

            hmass[i]->SetMaximum(hmass[i]->GetMaximum()*1.3);
            hmass[i]->SetMinimum(1);

            hmass_l[i] = (TH1F*)inFile->Get(Form("hmass_l_%d",i));
            hmass_l[i]->SetTitle("");
            hmass_l[i]->SetMarkerColor(massColor);
            hmass_l[i]->SetLineColor(SBColor);
            hmass_l[i]->SetFillColor(SBColor);
            hmass_l[i]->SetFillStyle(3004);
            hmass_l[i]->SetLineWidth(1);

            hmass_u[i] = (TH1F*)inFile->Get(Form("hmass_u_%d",i));
            hmass_u[i]->SetTitle("");
            hmass_u[i]->SetMarkerColor(massColor);
            hmass_u[i]->SetLineColor(SBColor);
            hmass_u[i]->SetFillColor(SBColor);
            hmass_u[i]->SetFillStyle(3004);
            hmass_u[i]->SetLineWidth(1);

            hmass_c[i] = (TH1F*)inFile->Get(Form("hmass_c_%d",i));
            hmass_c[i]->SetTitle("");
            hmass_c[i]->SetMarkerColor(massColor);
            hmass_c[i]->SetLineColor(signalColor);
            hmass_c[i]->SetFillColor(signalColor);
            hmass_c[i]->SetFillStyle(3005);
            hmass_c[i]->SetLineWidth(1);

            fullfit[i] = (TF1*)inFile->Get(Form("fullfit_%d",i));
            //fullfit[i]->SetNpx(150);
            fullfit[i]->SetLineWidth(2);
            fullfit[i]->SetLineColor(4);

            bkgfit[i] = (TF1*)inFile->Get(Form("bkgFitWRef_%d",i));
            //bkgfit[i]->SetNpx(150);
            bkgfit[i]->SetLineWidth(2);
            bkgfit[i]->SetLineStyle(1);
            bkgfit[i]->SetLineColor(2);

    }

    hmass[bin1]->SetMaximum(hmass[bin1]->GetMaximum()*1.3);
    hmass[bin2]->SetMaximum(hmass[bin2]->GetMaximum()*1.12);
    hmass[bin3]->SetMaximum(hmass[bin3]->GetMaximum()*1.25);

    for(int i = 0; i<ptbinsDN;i++){
    hmass[i]->GetYaxis()->SetTitleOffset(1.4);
    hmass[i]->GetYaxis()->SetTitleOffset(1.4);
    hmass[i]->GetYaxis()->SetTitleOffset(1.4);

    hmass[i]->GetXaxis()->SetTitleOffset(1.1);
    hmass[i]->GetXaxis()->SetTitleOffset(1.1);
    hmass[i]->GetXaxis()->SetTitleOffset(1.1);

    if(i>2)hmass[i]->SetMaximum(hmass[i]->GetMaximum()*1.15);
    }


    TLegend *legBands = new TLegend(0.181,0.86,0.8,0.95);
    legBands->SetTextSize(0.04);
    legBands->AddEntry(hmass_c[0],"Signal region","f");
    legBands->AddEntry(hmass_l[0],"Side bands","f");

 TLegend *lines = new TLegend(0.65,0.86,0.90,0.95);
    lines->SetTextSize(0.04);
    lines->AddEntry(fullfit[bin3],"Signal + bkg","l");
    lines->AddEntry(bkgfit[bin3],"Background","l");

    TPaveText *pvALICE = new TPaveText(0.175,0.890,0.6,0.93,"brNDC");
    pvALICE->SetFillStyle(0);
    pvALICE->SetBorderSize(0);
    pvALICE->SetTextFont(42);
    pvALICE->SetTextSize(0.048);
    pvALICE->SetTextAlign(11);
    pvALICE->AddText("This Thesis");


    TPaveText *pvEn = new TPaveText(0.168,0.88,0.8,0.92,"brNDC");
    pvEn->SetFillStyle(0);
    pvEn->SetBorderSize(0);
    pvEn->SetTextFont(42);
    pvEn->SetTextSize(0.048);
    pvEn->SetTextAlign(11);
    pvEn->AddText("pp, #sqrt{#it{s}} = 13 TeV");

    TPaveText *pvD = new TPaveText(0.18,0.84,0.55,0.875,"brNDC");
    pvD->SetFillStyle(0);
    pvD->SetBorderSize(0);
    pvD->SetTextFont(42);
    pvD->SetTextSize(0.047);
    pvD->SetTextAlign(11);
    pvD->AddText("D^{0} #rightarrow K^{-}#pi^{+} and charge conj.");

    TPaveText *pvJet = new TPaveText(0.181,0.79,0.55,0.83,"brNDC");
    pvJet->SetFillStyle(0);
    pvJet->SetBorderSize(0);
    pvJet->SetTextFont(42);
    pvJet->SetTextSize(0.046);
    pvJet->SetTextAlign(11);
    pvJet->AddText("in charged jets, anti-#it{k}_{T}, #it{R} = 0.4");

    TPaveText *pvEta = new TPaveText(0.1881,0.725,0.4,0.78,"brNDC");
    pvEta->SetFillStyle(0);
    pvEta->SetBorderSize(0);
    pvEta->SetTextFont(42);
    pvEta->SetTextSize(0.046);
    pvEta->SetTextAlign(11);
    pvEta->AddText("|#it{#eta}_{lab}^{jet}| < 0.5");


    TPaveText *pvpt[ptbinsDN];
    TPaveText *pvsig[ptbinsDN];
    TPaveText *pvsb[ptbinsDN];
    TPaveText *pvmean[ptbinsDN];
    TPaveText *pvsigma[ptbinsDN];
   for(int i = 0; i<ptbinsDN;i++){
       std::cout<<"huhu"<<i<<std::endl;
        if(i==0)pvpt[i]  = new TPaveText(0.62,0.74,0.9,0.775,"brNDC");
        else if(i==1)pvpt[i]  = new TPaveText(0.62,0.86,0.9,0.895,"brNDC");
        else if(i==2)pvpt[i]  = new TPaveText(0.62,0.67,0.9,0.705,"brNDC");
        else pvpt[i]  = new TPaveText(0.62,0.79,0.9,0.825,"brNDC");
        pvpt[i]->SetFillStyle(0);
        pvpt[i]->SetBorderSize(0);
        pvpt[i]->SetTextFont(42);
        pvpt[i]->SetTextSize(0.046);
        pvpt[i]->AddText(Form("%.0f < #it{p}_{T,D^{0}} < %.0f GeV/#it{c}",ptDbins[i],ptDbins[i+1]));
std::cout<<"A"<<std::endl;
        pvsig[i] = new TPaveText(0.57,0.60,0.9,0.65,"brNDC");
        pvsig[i]->SetFillStyle(0);
        pvsig[i]->SetBorderSize(0);
        pvsig[i]->SetTextFont(42);
        pvsig[i]->SetTextSize(0.04);
        pvsig[i]->AddText(Form("S (2#sigma) = %.1f #pm %.1f",hsign->GetBinContent(hsign->FindBin((ptDbins[i]+ptDbins[i+1])/2.)),hsign->GetBinError(hsign->FindBin((ptDbins[i]+ptDbins[i+1])/2.))));
std::cout<<"B"<<std::endl;
        pvsb[i] = new TPaveText(0.17,0.45,0.5,0.49,"brNDC");
        pvsb[i]->SetFillStyle(0);
        pvsb[i]->SetBorderSize(0);
        pvsb[i]->SetTextFont(42);
        pvsb[i]->SetTextSize(0.046);
        pvsb[i]->AddText(Form("S/B (2#sigma) = %.2f",hsb->GetBinContent(hsb->FindBin((ptDbins[i]+ptDbins[i+1])/2. ))));
std::cout<<"C"<<std::endl;
        if(i==0)pvmean[i] = new TPaveText(0.178,0.67,0.62,0.72,"brNDC");
        else if(i==1)pvmean[i] = new TPaveText(0.178,0.77,0.62,0.82,"brNDC");
        else if(i==2)pvmean[i] = new TPaveText(0.178,0.72,0.62,0.77,"brNDC");
        else pvmean[i] = new TPaveText(0.178,0.87,0.62,0.92,"brNDC");
        pvmean[i]->SetFillStyle(0);
        pvmean[i]->SetBorderSize(0);
        pvmean[i]->SetTextFont(42);
        pvmean[i]->SetTextSize(0.046);
        pvmean[i]->SetTextAlign(11);
        pvmean[i]->AddText(Form("#mu = %.2f #pm %.2f MeV/#it{c}^{2}",1000*hmean->GetBinContent(hmean->FindBin((ptDbins[i]+ptDbins[i+1])/2. )),1000*hmean->GetBinError(hmean->FindBin((ptDbins[i]+ptDbins[i+1])/2. ))));
std::cout<<"D"<<std::endl;
        if(i==0)pvsigma[i] = new TPaveText(0.179,0.62,0.62,0.68,"brNDC");
        else if(i==1)pvsigma[i] = new TPaveText(0.179,0.72,0.62,0.78,"brNDC");
        else if(i==2)pvsigma[i] = new TPaveText(0.179,0.77,0.62,0.83,"brNDC");
        else pvsigma[i] = new TPaveText(0.179,0.82,0.62,0.88,"brNDC");
        pvsigma[i]->SetFillStyle(0);
        pvsigma[i]->SetBorderSize(0);
        pvsigma[i]->SetTextFont(42);
        pvsigma[i]->SetTextSize(0.046);
        pvsigma[i]->SetTextAlign(11);
        pvsigma[i]->AddText(Form("#sigma = %.1f #pm %.1f MeV/#it{c}^{2}",hsigma->GetBinContent(hsigma->FindBin((ptDbins[i]+ptDbins[i+1])/2. )),hsigma->GetBinError(hsigma->FindBin((ptDbins[i]+ptDbins[i+1])/2. ))));


   }




    TCanvas *cMass = new TCanvas("cMass","cMass",3000,4000);
    cMass->Divide(3,4);
    for(int i = 0; i<11;i++){
    cMass->cd(i+1);
    hmass[i]->Draw();
    hmass_l[i]->Draw("hsame");
    hmass_u[i]->Draw("hsame");
    hmass_c[i]->Draw("hsame");
    hmass[i]->Draw("same");
    bkgfit[i]->Draw("same");
    fullfit[i]->Draw("same");
    pvpt[i]->Draw("same");
    pvmean[i]->Draw("same");
    pvsigma[i]->Draw("same");
    pvsb[i]->Draw("same");
    }

    cMass->cd(1);
    pvALICE->Draw("same");
    pvD->Draw("same");
    pvJet->Draw("same");
    pvEta->Draw("same");


    cMass->cd(2);
    pvEn->Draw("same");

    cMass->cd(3);

    legBands->Draw("same");
    lines->Draw("same");

    cMass->Print("DjetInMass_DPt_thesis_full.pdf");


}

void setHistoDetails(TH1 *h, Color_t color, Style_t Mstyle, Size_t size = 0.9, Width_t width=2, int scale = 0){

    if(scale)h->Scale(1,"width");
    h->SetMarkerStyle(Mstyle);
    h->SetMarkerColor(color);
    h->SetMarkerSize(size);
    h->SetLineColor(color);
    h->SetLineWidth(width);
    h->SetTitle(0);
    h->GetXaxis()->SetTitle("p_{T}^{D*}(GeV/c)");

    return;

}

void SaveCanvas(TCanvas *c, TString name = "tmp"){

    c->SaveAs(name+".png");
    c->SaveAs(name+".pdf");

}
