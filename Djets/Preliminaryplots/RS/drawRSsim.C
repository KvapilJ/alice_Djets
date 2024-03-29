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
    int ptDbins[ptbinsDN+1] = {2,3,4,5,6,7,8,10,12,16,24,36};

    double efficiency[ptbinsDN];// = { 0.0353325, 0.0678059, 0.109101, 0.168871, 0.243708, 0.307365, 0.324496, 0.361858 };


    int massColor = kBlack;
    int signalColor = kRed+1;
    int SBColor = kGreen+3;

void drawRSsim()
{

    style();
    gStyle->SetOptStat(000);

    gStyle->SetLegendFont(42);
    //gStyle->SetLegendTextSize(0.05);
    gStyle->SetPadLeftMargin(0.15);
    gStyle->SetPadRightMargin(0.02);
    gStyle->SetPadTopMargin(0.02);


    TFile *inFile = new TFile("/mnt/hgfs/vmware/data_R04_050219/MC/reflections/AnalysisResults_Run2w18b.root/reflectionTemplates_pp.root","read");


     int bin1 = 0, bin2 = 2, bin3 = 5;


  /*  TH1F *hmean = (TH1F*)inFile->Get("hmean");
    TH1F *hsigma = (TH1F*)inFile->Get("hsigma");
    TH1F *hsign = (TH1F*)inFile->Get("hsign");
    TH1F *hsb = (TH1F*)inFile->Get("hsb");*/

    TH1F* hmass[ptbinsDN];
    TH1F* hmass_l[ptbinsDN];
    TH1F* hmass_u[ptbinsDN];
    TH1F* hmass_c[ptbinsDN];
 //   TF1* fullfit[ptbinsDN];
    TF1* bkgfit[ptbinsDN];
  /*  TH1F* hjetpt[ptbinsDN];
    TH1F *hjetpt_s[ptbinsDN];
    TH1F *hjetptsub[ptbinsDN];
    TH1F *hjetptcorr[ptbinsDN];*/



    for(int i=0; i<ptbinsDN; i++){
        std::cout<<"HU "<<i<<std::endl;
            hmass[i] = (TH1F*)inFile->Get(Form("histSgn_%d_%d",ptDbins[i],ptDbins[i+1]));
            hmass[i]->SetTitle("");
            hmass[i]->SetMarkerColor(kWhite);
            hmass[i]->SetLineColor(kWhite);
            hmass[i]->SetLineWidth(0);
            hmass[i]->SetMarkerStyle(1);
            hmass[i]->SetMarkerSize(0);
            hmass[i]->GetXaxis()->SetRangeUser(1.72,2.0);
            //hmass[i]->GetXaxis()->SetTitle("#it{M}(K#pi) (GeV/#it{c^{2}})");
            hmass[i]->GetXaxis()->SetTitle("#it{M}(K#pi) (GeV/#it{c}^{2})");
            hmass[i]->GetYaxis()->SetTitle(Form("Entries/%.0f MeV/#it{c}^{2}",hmass[i]->GetBinWidth(1)*1000));
            hmass[i]->GetXaxis()->SetLabelSize(0.045);
            hmass[i]->GetXaxis()->SetTitleSize(0.055);
            hmass[i]->GetYaxis()->SetLabelSize(0.045);
            hmass[i]->GetYaxis()->SetTitleSize(0.055);

            hmass[i]->GetYaxis()->SetMaxDigits(4);
            if(i>3)hmass[i]->GetYaxis()->SetMaxDigits(3);


            hmass[i]->SetMaximum(hmass[i]->GetMaximum()*1.3);
            hmass[i]->SetMinimum(0);

      //      hmass[i]->Scale(1./16000);

       /*     hmass_l[i] = (TH1F*)inFile->Get(Form("hmass_l_%d",i));
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
            hmass_u[i]->SetLineWidth(1);*/

       /*     hmass_c[i] = (TH1F*)inFile->Get(Form("histSgn_%d_%d",ptDbins[i],ptDbins[i+1]));
            hmass_c[i]->SetTitle("");
            hmass_c[i]->SetMarkerColor(massColor);
            hmass_c[i]->SetLineColor(signalColor);
            hmass_c[i]->SetFillColor(signalColor);
            hmass_c[i]->SetFillStyle(3005);
            hmass_c[i]->SetLineWidth(1);*/
/*
            fullfit[i] = (TF1*)inFile->Get(Form("fullfit_%d",i));
            //fullfit[i]->SetNpx(150);
            fullfit[i]->SetLineWidth(2);
            fullfit[i]->SetLineColor(4);*/

            bkgfit[i] = (TF1*)inFile->Get(Form("histRflFittedDoubleGaus_pt%d_%d",ptDbins[i],ptDbins[i+1]));
            //bkgfit[i]->SetNpx(150);
            bkgfit[i]->SetLineWidth(2);
            bkgfit[i]->SetLineStyle(1);
            bkgfit[i]->SetLineColor(2);

    }

    TF1 * tmpfit = (TF1*)bkgfit[0]->Clone("tmpfit");
    tmpfit->SetLineColor(kOrange+2);
    TF1 * tmpfit2 = (TF1*)bkgfit[0]->Clone("tmpfit2");
    tmpfit2->SetLineColor(kRed+2);


    //hmass[0]->SetMaximum(hmass[0]->GetMaximum()*1.1);
 /*   hmass[bin1]->SetMaximum(hmass[bin1]->GetMaximum()*1.3);
    hmass[bin2]->SetMaximum(hmass[bin2]->GetMaximum()*1.12);
    hmass[bin3]->SetMaximum(hmass[bin3]->GetMaximum()*1.25);

    hmass[bin1]->GetYaxis()->SetTitleOffset(1.4);
    hmass[bin2]->GetYaxis()->SetTitleOffset(1.4);
    hmass[bin3]->GetYaxis()->SetTitleOffset(1.4);

    hmass[bin1]->GetXaxis()->SetTitleOffset(1.1);
    hmass[bin2]->GetXaxis()->SetTitleOffset(1.1);
    hmass[bin3]->GetXaxis()->SetTitleOffset(1.1);*/

   // TLegend *legBands = new TLegend(0.18,0.86,0.8,0.95);
    TLegend *legBands = new TLegend(0.181,0.75,0.55,0.85);
    legBands->SetTextSize(0.04);
    legBands->AddEntry(tmpfit,"Signal mass fit","l");
    legBands->AddEntry(tmpfit2,"Reflected mass fit","l");

 TLegend *lines = new TLegend(0.65,0.86,0.90,0.95);
//        lines->AddEntry(hjetpt[i],"signal","l");
//        lines->AddEntry(hjetpt_sb[i],"SB","l");
//        lines->Draw("same");
    lines->SetTextSize(0.04);
  //  lines->AddEntry(fullfit[bin3],"Signal + bkg","l");
  //  lines->AddEntry(bkgfit[bin3],"Background","l");

    Double_t shift = -0.04;

    //TPaveText *pvALICE = new TPaveText(0.1995,0.90,0.6,0.94,"brNDC");
    //TPaveText *pvALICE = new TPaveText(0.18,0.90,0.6,0.94,"brNDC");
    TPaveText *pvALICE = new TPaveText(0.175,0.890+shift,0.6,0.93+shift,"brNDC");
    pvALICE->SetFillStyle(0);
    pvALICE->SetBorderSize(0);
    pvALICE->SetTextFont(42);
    pvALICE->SetTextSize(0.048);
    pvALICE->SetTextAlign(11);
    pvALICE->AddText("This Thesis");

    //TPaveText *pvEn = new TPaveText(0.2,0.90,0.8,0.94,"brNDC");
    //TPaveText *pvEn = new TPaveText(0.168,0.90,0.8,0.94,"brNDC");
    TPaveText *pvEn = new TPaveText(0.168+shift,0.88,0.8,0.92+shift,"brNDC");
    pvEn->SetFillStyle(0);
    pvEn->SetBorderSize(0);
    pvEn->SetTextFont(42);
    pvEn->SetTextSize(0.048);
    pvEn->SetTextAlign(11);
    //pvEn->AddText("p-Pb, #sqrt{#it{s}_{NN}} = 5.02 TeV, 603M events");
    //pvEn->AddText("p-Pb, #sqrt{#it{s}_{NN}} = 5.02 TeV");
    pvEn->AddText("pp, #sqrt{#it{s}} = 13 TeV");

    //TPaveText *pvD = new TPaveText(0.2,0.85,0.55,0.89,"brNDC");
    //TPaveText *pvD = new TPaveText(0.1795,0.85,0.55,0.89,"brNDC");
    //TPaveText *pvD = new TPaveText(0.18,0.85,0.55,0.89,"brNDC");
    TPaveText *pvD = new TPaveText(0.18,0.84+shift,0.55,0.875+shift,"brNDC");
    pvD->SetFillStyle(0);
    pvD->SetBorderSize(0);
    pvD->SetTextFont(42);
    pvD->SetTextSize(0.047);
    pvD->SetTextAlign(11);
    pvD->AddText("PYTHIA6+GEANT3");

    //TPaveText *pvJet = new TPaveText(0.2035,0.80,0.55,0.84,"brNDC");
    //TPaveText *pvJet = new TPaveText(0.1815,0.80,0.55,0.84,"brNDC");
    TPaveText *pvJet = new TPaveText(0.181,0.79+shift,0.55,0.83+shift,"brNDC");
    pvJet->SetFillStyle(0);
    pvJet->SetBorderSize(0);
    pvJet->SetTextFont(42);
    pvJet->SetTextSize(0.046);
    pvJet->SetTextAlign(11);
    pvJet->AddText("D^{0} #rightarrow K^{-}#pi^{+} and charge conj.");

    //TPaveText *pvEta = new TPaveText(0.2,0.75,0.4,0.79,"brNDC");
    //TPaveText *pvEta = new TPaveText(0.21,0.75,0.4,0.79,"brNDC");
    //TPaveText *pvEta = new TPaveText(0.188,0.75,0.4,0.79,"brNDC");
    //TPaveText *pvEta = new TPaveText(0.1881,0.75,0.4,0.79,"brNDC");
    TPaveText *pvEta = new TPaveText(0.1881,0.725+shift,0.4,0.78+shift,"brNDC");
    pvEta->SetFillStyle(0);
    pvEta->SetBorderSize(0);
    pvEta->SetTextFont(42);
    pvEta->SetTextSize(0.046);
    pvEta->SetTextAlign(11);
    pvEta->AddText("|#it{#eta}_{lab}^{jet}| < 0.5");
    //pvEta->AddText("|#it{#eta}_{jet}| < 0.6");

    //TPaveText *pvpt1 = new TPaveText(0.62,0.74,0.9,0.77,"brNDC");
    TPaveText *pvpt1 = new TPaveText(0.64,0.69,0.92,0.725,"brNDC");
    pvpt1->SetFillStyle(0);
    pvpt1->SetBorderSize(0);
    pvpt1->SetTextFont(42);
    pvpt1->SetTextSize(0.046);
    pvpt1->AddText(Form("%d < #it{p}_{T,D^{0}} < %d GeV/#it{c}",ptDbins[bin1],ptDbins[bin1+1]));

    //TPaveText *pvpt2 = new TPaveText(0.62,0.84,0.85,0.88,"brNDC");
    TPaveText *pvpt2 = new TPaveText(0.62,0.82,0.85,0.81,"brNDC");
    pvpt2->SetFillStyle(0);
    pvpt2->SetBorderSize(0);
    pvpt2->SetTextFont(42);
    pvpt2->SetTextSize(0.046);
    pvpt2->AddText(Form("%d < #it{p}_{T,D^{0}} < %d GeV/#it{c}",ptDbins[bin2],ptDbins[bin2+1]));

    TPaveText *pvpt3 = new TPaveText(0.62,0.82,0.85,0.81,"brNDC");
    pvpt3->SetFillStyle(0);
    pvpt3->SetBorderSize(0);
    pvpt3->SetTextFont(42);
    pvpt3->SetTextSize(0.046);
    pvpt3->AddText(Form("%d < #it{p}_{T,D^{0}} < %.d GeV/#it{c}",ptDbins[bin3],ptDbins[bin3+1]));
/*
    TPaveText *pvsig1 = new TPaveText(0.57,0.60,0.9,0.65,"brNDC");
    pvsig1->SetFillStyle(0);
    pvsig1->SetBorderSize(0);
    pvsig1->SetTextFont(42);
    pvsig1->SetTextSize(0.04);
    //pvsig1->AddText(Form("S (3#sigma) = %.1f #pm %.1f",hsign->GetBinContent(hsign->FindBin((ptDbins[bin1]+ptDbins[bin1+1])/2.)),hsign->GetBinError(hsign->FindBin((ptDbins[bin1]+ptDbins[bin1+1])/2.))));
    pvsig1->AddText(Form("S (2#sigma) = %.1f #pm %.1f",hsign->GetBinContent(hsign->FindBin((ptDbins[bin1]+ptDbins[bin1+1])/2.)),hsign->GetBinError(hsign->FindBin((ptDbins[bin1]+ptDbins[bin1+1])/2.))));

    //TPaveText *pvsb1 = new TPaveText(0.18,0.45,0.5,0.49,"brNDC");
    //TPaveText *pvsb1 = new TPaveText(0.13,0.45,0.5,0.49,"brNDC");
    //TPaveText *pvsb1 = new TPaveText(0.123,0.45,0.5,0.49,"brNDC");
    TPaveText *pvsb1 = new TPaveText(0.17,0.45,0.5,0.49,"brNDC");
    pvsb1->SetFillStyle(0);
    pvsb1->SetBorderSize(0);
    pvsb1->SetTextFont(42);
    //pvsb1->SetTextSize(0.04);
    pvsb1->SetTextSize(0.046);
    //pvsb1->AddText(Form("S/B (3#sigma) = %.2f",hsb->GetBinContent(hsb->FindBin((ptDbins[bin1]+ptDbins[bin1+1])/2. ))));
    pvsb1->AddText(Form("S/B (2#sigma) = %.2f",hsb->GetBinContent(hsb->FindBin((ptDbins[bin1]+ptDbins[bin1+1])/2. ))));

    //TPaveText *pvmean1 = new TPaveText(0.18,0.67,0.62,0.71,"brNDC");
    //TPaveText *pvmean1 = new TPaveText(0.178,0.67,0.62,0.71,"brNDC");
    //TPaveText *pvmean1 = new TPaveText(0.178,0.67,0.62,0.71,"brNDC");
    TPaveText *pvmean1 = new TPaveText(0.178,0.67,0.62,0.72,"brNDC");
    pvmean1->SetFillStyle(0);
    pvmean1->SetBorderSize(0);
    pvmean1->SetTextFont(42);
    pvmean1->SetTextSize(0.046);
    pvmean1->SetTextAlign(11);
    pvmean1->AddText(Form("#mu = %.2f #pm %.2f MeV/#it{c}^{2}",1000*hmean->GetBinContent(hmean->FindBin((ptDbins[bin1]+ptDbins[bin1+1])/2. )),1000*hmean->GetBinError(hmean->FindBin((ptDbins[bin1]+ptDbins[bin1+1])/2. ))));
//std::cout<<hmean->GetBinError(hmean->FindBin((ptDbins[bin1]+ptDbins[bin1+1])/2.))<<std::endl;
//    pvmean1->AddText(Form("#mu = %.3f #pm 0.001 GeV/#it{c^{2}}",hmean->GetBinContent(hmean->FindBin((ptDbins[bin1]+ptDbins[bin1+1])/2. )),hmean->GetBinError(hmean->FindBin((ptDbins[bin1]+ptDbins[bin1+1])/2. ))));

    //TPaveText *pvsigma1 = new TPaveText(0.18,0.63,0.62,0.67,"brNDC");
    //TPaveText *pvsigma1 = new TPaveText(0.179,0.63,0.62,0.67,"brNDC");
    TPaveText *pvsigma1 = new TPaveText(0.179,0.62,0.62,0.68,"brNDC");
    pvsigma1->SetFillStyle(0);
    pvsigma1->SetBorderSize(0);
    pvsigma1->SetTextFont(42);
    pvsigma1->SetTextSize(0.046);
    pvsigma1->SetTextAlign(11);
    pvsigma1->AddText(Form("#sigma = %.1f #pm %.1f MeV/#it{c}^{2}",hsigma->GetBinContent(hsigma->FindBin((ptDbins[bin1]+ptDbins[bin1+1])/2. )),hsigma->GetBinError(hsigma->FindBin((ptDbins[bin1]+ptDbins[bin1+1])/2. ))));

    TPaveText *pvsig2 = new TPaveText(0.57,0.73,0.9,0.78,"brNDC");
    pvsig2->SetFillStyle(0);
    pvsig2->SetBorderSize(0);
    pvsig2->SetTextFont(42);
    pvsig2->SetTextSize(0.04);
    //pvsig2->AddText(Form("S (3#sigma) = %.1f #pm %.1f",hsign->GetBinContent(hsign->FindBin((ptDbins[bin2]+ptDbins[bin2+1])/2.)),hsign->GetBinError(hsign->FindBin((ptDbins[bin2]+ptDbins[bin2+1])/2.))));
    pvsig2->AddText(Form("S (2#sigma) = %.1f #pm %.1f",hsign->GetBinContent(hsign->FindBin((ptDbins[bin2]+ptDbins[bin2+1])/2.)),hsign->GetBinError(hsign->FindBin((ptDbins[bin2]+ptDbins[bin2+1])/2.))));

//    TPaveText *pvsb2 = new TPaveText(0.18,0.45,0.5,0.49,"brNDC");
//    TPaveText *pvsb2 = new TPaveText(0.123,0.45,0.5,0.49,"brNDC");
    TPaveText *pvsb2 = new TPaveText(0.170,0.45,0.5,0.49,"brNDC");
    pvsb2->SetFillStyle(0);
    pvsb2->SetBorderSize(0);
    pvsb2->SetTextFont(42);
    pvsb2->SetTextSize(0.046);
    //pvsb2->AddText(Form("S/B (3#sigma) = %.2f",hsb->GetBinContent(hsb->FindBin((ptDbins[bin2]+ptDbins[bin2+1])/2. ))));
    pvsb2->AddText(Form("S/B (2#sigma) = %.2f",hsb->GetBinContent(hsb->FindBin((ptDbins[bin2]+ptDbins[bin2+1])/2. ))));


    //TPaveText *pvmean2 = new TPaveText(0.18,0.77,0.62,0.81,"brNDC");
    TPaveText *pvmean2 = new TPaveText(0.178,0.785,0.62,0.81,"brNDC");
    pvmean2->SetFillStyle(0);
    pvmean2->SetBorderSize(0);
    pvmean2->SetTextFont(42);
    pvmean2->SetTextSize(0.046);
    pvmean2->SetTextAlign(11);
    //pvmean2->AddText(Form("#mu = %.2f #pm %.2f GeV/#it{c^{2}}",hmean->GetBinContent(hmean->FindBin((ptDbins[bin2]+ptDbins[bin2+1])/2. )),hmean->GetBinError(hmean->FindBin((ptDbins[bin2]+ptDbins[bin2+1])/2. ))));
    //pvmean2->AddText(Form("#mu = %.3f #pm 0.001 GeV/#it{c^{2}}",hmean->GetBinContent(hmean->FindBin((ptDbins[bin2]+ptDbins[bin2+1])/2. ))));
    pvmean2->AddText(Form("#mu = %.2f #pm %.2f MeV/#it{c}^{2}",1000*hmean->GetBinContent(hmean->FindBin((ptDbins[bin2]+ptDbins[bin2+1])/2. )),1000*hmean->GetBinError(hmean->FindBin((ptDbins[bin2]+ptDbins[bin2+1])/2. ))));

    //TPaveText *pvsigma2 = new TPaveText(0.18,0.73,0.62,0.77,"brNDC");
    TPaveText *pvsigma2 = new TPaveText(0.1797,0.735,0.62,0.77,"brNDC");
    pvsigma2->SetFillStyle(0);
    pvsigma2->SetBorderSize(0);
    pvsigma2->SetTextFont(42);
    pvsigma2->SetTextSize(0.046);
    pvsigma2->SetTextAlign(11);
    pvsigma2->AddText(Form("#sigma = %.1f #pm %.1f MeV/#it{c}^{2}",hsigma->GetBinContent(hsigma->FindBin((ptDbins[bin2]+ptDbins[bin2+1])/2. )),hsigma->GetBinError(hsigma->FindBin((ptDbins[bin2]+ptDbins[bin2+1])/2. ))));

    TPaveText *pvsig3 = new TPaveText(0.17,0.82,0.9,0.87,"brNDC");
    pvsig3->SetFillStyle(0);
    pvsig3->SetBorderSize(0);
    pvsig3->SetTextFont(42);
    pvsig3->SetTextSize(0.04);
    //pvsig3->AddText(Form("S (3#sigma) = %.1f #pm %.1f",hsign->GetBinContent(hsign->FindBin((ptDbins[bin3]+ptDbins[bin3+1])/2.)),hsign->GetBinError(hsign->FindBin((ptDbins[bin3]+ptDbins[bin3+1])/2.))));
    pvsig3->AddText(Form("S (2#sigma) = %.1f #pm %.1f",hsign->GetBinContent(hsign->FindBin((ptDbins[bin3]+ptDbins[bin3+1])/2.)),hsign->GetBinError(hsign->FindBin((ptDbins[bin3]+ptDbins[bin3+1])/2.))));

//    TPaveText *pvsb3 = new TPaveText(0.18,0.45,0.5,0.49,"brNDC");
    TPaveText *pvsb3 = new TPaveText(0.170,0.45,0.5,0.49,"brNDC");
    pvsb3->SetFillStyle(0);
    pvsb3->SetBorderSize(0);
    pvsb3->SetTextFont(42);
    pvsb3->SetTextSize(0.046);
    //pvsb3->AddText(Form("S/B (3#sigma) = %.2f",hsb->GetBinContent(hsb->FindBin((ptDbins[bin3]+ptDbins[bin3+1])/2. ))));
    pvsb3->AddText(Form("S/B (2#sigma) = %.2f",hsb->GetBinContent(hsb->FindBin((ptDbins[bin3]+ptDbins[bin3+1])/2. ))));

    //TPaveText *pvmean3 = new TPaveText(0.18,0.72,0.62,0.76,"brNDC");
    TPaveText *pvmean3 = new TPaveText(0.17,0.73,0.62,0.76,"brNDC");
    pvmean3->SetFillStyle(0);
    pvmean3->SetBorderSize(0);
    pvmean3->SetTextFont(42);
    pvmean3->SetTextSize(0.046);
    pvmean3->SetTextAlign(11);
    //pvmean3->AddText(Form("#mu = %.3f #pm %.3f GeV/#it{c^{2}}",hmean->GetBinContent(hmean->FindBin((ptDbins[bin3]+ptDbins[bin3+1])/2. )),hmean->GetBinError(hmean->FindBin((ptDbins[bin3]+ptDbins[bin3+1])/2. ))));
    pvmean3->AddText(Form("#mu = %.2f #pm %.2f MeV/#it{c}^{2}",1000*hmean->GetBinContent(hmean->FindBin((ptDbins[bin3]+ptDbins[bin3+1])/2. )),1000*hmean->GetBinError(hmean->FindBin((ptDbins[bin3]+ptDbins[bin3+1])/2. ))));

    //TPaveText *pvsigma3 = new TPaveText(0.18,0.68,0.62,0.72,"brNDC");
    //TPaveText *pvsigma3 = new TPaveText(0.1797,0.68,0.62,0.72,"brNDC");
    TPaveText *pvsigma3 = new TPaveText(0.17,0.68,0.62,0.72,"brNDC");
    pvsigma3->SetFillStyle(0);
    pvsigma3->SetBorderSize(0);
    pvsigma3->SetTextFont(42);
    pvsigma3->SetTextSize(0.046);
    pvsigma3->SetTextAlign(11);
    pvsigma3->AddText(Form("#sigma = %.1f #pm %.1f MeV/#it{c}^{2}",hsigma->GetBinContent(hsigma->FindBin((ptDbins[bin3]+ptDbins[bin3+1])/2. )),hsigma->GetBinError(hsigma->FindBin((ptDbins[bin3]+ptDbins[bin3+1])/2. ))));
*/
    TCanvas *cMass = new TCanvas("cMass","cMass",3000,900);
    //TCanvas *cMass = new TCanvas("cMass","cMass",3000,1000);
    //TCanvas *cMass = new TCanvas("cMass","cMass",1800,600);
    //TCanvas *cMass = new TCanvas("cMass","cMass");
    //cMass->SetBatch();
    cMass->Divide(3,1);
    cMass->cd(1);
    cMass->cd(1)->SetTopMargin(0.06);
    hmass[bin1]->Draw("axis");
    hmass[bin1]->Draw("same func");
//    hmass_l[bin1]->Draw("hsame");
//    hmass_u[bin1]->Draw("hsame");
   // hmass_c[bin1]->Draw("h");
  //  hmass[bin1]->Draw("same");
    bkgfit[bin1]->Draw("same");
  //  fullfit[bin1]->Draw("same");

    pvpt1->Draw("same");
    pvALICE->Draw("same");
    pvD->Draw("same");
    pvJet->Draw("same");
//    pvmean1->Draw("same");
 //   pvsigma1->Draw("same");
 //   pvsb1->Draw("same");
  //  pvEta->Draw("same");
    //pvsig1->Draw("same");
    //legBands->Draw("same");
    cMass->cd(2)->SetTopMargin(0.06);
    cMass->cd(3)->SetTopMargin(0.06);

    cMass->cd(2);
    hmass[bin2]->Draw("axis");
    hmass[bin2]->Draw("same func");
    bkgfit[bin2]->Draw("same");
     pvpt2->Draw("same");

    cMass->cd(3);
    hmass[bin3]->Draw("axis");
    hmass[bin3]->Draw("same func");
    bkgfit[bin3]->Draw("same");
     pvpt3->Draw("same");
     legBands->Draw("same");

/*
    pvpt2->Draw("same");
    pvEn->Draw("same");
    pvmean2->Draw("same");
    pvsigma2->Draw("same");
    pvsb2->Draw("same");
    //pvsig2->Draw("same");


    cMass->cd(3);
    hmass[bin3]->Draw();
    hmass_l[bin3]->Draw("hsame");
    hmass_u[bin3]->Draw("hsame");
    hmass_c[bin3]->Draw("hsame");
    hmass[bin3]->Draw("same");
    bkgfit[bin3]->Draw("same");
    fullfit[bin3]->Draw("same");

    pvpt3->Draw("same");
    //pvEta->Draw("same");
    pvmean3->Draw("same");
    pvsigma3->Draw("same");
    pvsb3->Draw("same");
    //pvsig3->Draw("same");
    legBands->Draw("same");
    lines->Draw("same");
*/
    //cMass->SaveAs("DjetInMass_DPt_Perf.png");
    //cMass->SaveAs("DjetInMass_DPt_Perf.pdf");
    //cMass->SaveAs("DjetInMass_DPt_Perf.eps");
    //cMass->Print("DjetInMass_DPt_Perf_v2.pdf");
//    cMass->Print("DjetInMass_DPt_Perf_2.eps");
    cMass->Print("DjetRF_DPt_thesis.pdf");
//    cMass->Print("DjetInMass_DPt_Perf_2.png");
//    cMass->Print("DjetInMass_DPt_Perf_2.root");
 //   cMass->Print("DjetInMass_DPt_Perf_2.C");

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
