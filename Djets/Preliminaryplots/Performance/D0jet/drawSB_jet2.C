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
void SaveCanvas(TCanvas *c, TString name = "tmp");

 //double zmin = 0, zmax = 2.;
    //double jetmin = 0, jetmax = 50;
    //double plotmin = 0, plotmax = 50;

    const int ptbinsDN = 11;
    float ptDbins[ptbinsDN+1] = {2,3,4,5,6,7,8,10,12,16,24,36};

    const int ptbinsJetN = 8;
    double ptJetbins[ptbinsJetN+1] = { 5,6,8,10,12,14,20,30,50};

    int massColor = kBlack;
    int signalColor = kRed+1;
    //int SBColor = kGreen+3;
    int SBColor = kGreen+3;
    int subColor = kBlue+1;
    double markersize = 2.;
    //double markersize = 1.6;
    //int markerstyle[] = { 20,21,33 };
    int markerstyle[] = { 24,25,27 };

    double ltextsize = 0.043;

void drawSB_jet2( int Rpar = 4 )
{

    style();
    gStyle->SetOptStat(000);


    gStyle->SetLegendFont(42);
    //gStyle->SetLegendTextSize(0.05);
    gStyle->SetPadLeftMargin(0.15);
    gStyle->SetPadRightMargin(0.02);
    gStyle->SetPadTopMargin(0.02);

    TFile *inFile = new TFile("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_X04/Default_AnalysisResults_Run2w18b.root/signalExtraction/JetPtSpectra_SB_eff.root","read");

    int bin1 = 0, bin2 = 4, bin3 = 8;

    int islog = 1;


    TH1D* hjetpt[ptbinsDN];
    TH1D* hjetpt_s[ptbinsDN];
    TH1D* hjetptsub[ptbinsDN];
    TH1F* hmass_c[ptbinsDN];

    TH1F* hjetptcorr[ptbinsDN];


    for(int i=0; i<ptbinsDN; i++){
            TH1D *hjet = (TH1D*)inFile->Get(Form("hjetpt_%d",i));
            hjetpt[i] = (TH1D*)hjet->Rebin(ptbinsJetN,Form("hjetpt_%d",i),ptJetbins);

            hjetpt[i]->SetTitle("");
            hjetpt[i]->SetMarkerColor(signalColor);
            hjetpt[i]->SetLineColor(signalColor);
            hjetpt[i]->SetLineWidth(2);
            hjetpt[i]->SetMarkerStyle(markerstyle[0]);
            hjetpt[i]->SetMarkerSize(markersize);

            hjetpt[i]->GetXaxis()->SetTitle("#it{p}_{T,ch jet} (GeV/#it{c})");
            hjetpt[i]->GetYaxis()->SetTitle("Entries");
            hjetpt[i]->GetXaxis()->SetLabelSize(0.045);
            hjetpt[i]->GetXaxis()->SetTitleSize(0.055);
            hjetpt[i]->GetYaxis()->SetLabelSize(0.045);
            hjetpt[i]->GetYaxis()->SetTitleSize(0.055);

            hjetpt[i]->GetYaxis()->SetTitleOffset(1.4);

            hjetpt[i]->SetMaximum(hjetpt[i]->GetMaximum()*1.55);
            hjetpt[i]->SetMaximum(1600);
            if(islog) hjetpt[i]->SetMaximum(hjetpt[i]->GetMaximum()*10);
            hjetpt[i]->SetMinimum(-50);
            if(islog) hjetpt[i]->SetMinimum(1);

            TH1D *hjets = (TH1D*)inFile->Get(Form("hjetpt_sb_%d",i));
            hjetpt_s[i] = (TH1D*)hjets->Rebin(ptbinsJetN,Form("hjetpt_s_%d",i),ptJetbins);
            hjetpt_s[i]->SetTitle("");
            hjetpt_s[i]->SetMarkerColor(SBColor);
            hjetpt_s[i]->SetLineColor(SBColor);
            hjetpt_s[i]->SetLineWidth(2);
            hjetpt_s[i]->SetMarkerStyle(markerstyle[1]);
            hjetpt_s[i]->SetMarkerSize(markersize);


            TH1D *hjetsub = (TH1D*)inFile->Get(Form("hjetptsub_%d",i));
            hjetptsub[i] = (TH1D*)hjetsub->Rebin(ptbinsJetN,Form("hjetptsub_%d",i),ptJetbins);
            hjetptsub[i]->SetTitle("");
            hjetptsub[i]->SetMarkerColor(subColor);
            hjetptsub[i]->SetLineColor(subColor);
            hjetptsub[i]->SetLineWidth(2);
            hjetptsub[i]->SetMarkerStyle(markerstyle[2]);
            hjetptsub[i]->SetMarkerSize(markersize+0.5);

    }
std::cout<<"A"<<std::endl;

//     if(islog) hjetpt[2]->SetMaximum(hjetpt[bin3]->GetMaximum()*100);

     TH1F *hh = (TH1F*)hjetpt[0]->Clone("hh");
     hh->SetMarkerSize(0);

     Double_t shiftx = 0.05;
     Double_t shifty = 0.075;

    TLegend *legBands1 = new TLegend(0.15+shiftx,0.76+shifty,0.7+shiftx,0.86+shifty);
    legBands1->SetTextSize(ltextsize);
    legBands1->SetFillStyle(0);
    legBands1->SetTextAlign(13);

    legBands1->AddEntry(hjetpt[0],"#splitline{Signal region}{|#it{M}(K#pi)-#it{M}_{D^{0}}|<2#sigma}","p");

    TLegend *legBands2 = new TLegend(0.15+shiftx,0.65+shifty,0.7+shiftx,0.75+shifty);
    legBands2->SetTextSize(ltextsize);
    legBands2->SetFillStyle(0);
    legBands2->SetTextAlign(13);
      legBands2->AddEntry(hjetpt_s[0],"#splitline{Side bands (SB)}{4#sigma<|#it{M}(K#pi)-#it{M}_{D^{0}}|<9#sigma}","p");


    TLegend *legBands23 = new TLegend(0.182+shiftx,0.605+shifty,0.6+shiftx,0.625+shifty);
    legBands23->SetTextSize(ltextsize);
    legBands23->SetFillStyle(0);
    legBands23->SetTextAlign(11);
    legBands23->AddEntry(hh,"normalised to Signal region","p");
std::cout<<"B"<<std::endl;
     TLegend *legBands3 = new TLegend(0.15+shiftx,0.54+shifty,0.7+shiftx,0.59+shifty);
    legBands3->SetTextSize(ltextsize);
    legBands3->SetFillStyle(0);
    legBands3->SetTextAlign(13);
    legBands3->AddEntry(hjetptsub[0],"Signal - SB","p");

    //TPaveText *pvALICE = new TPaveText(0.25,0.88,0.6,0.92,"brNDC");
    //TPaveText *pvALICE = new TPaveText(0.247,0.88,0.6,0.92,"brNDC");
    //TPaveText *pvALICE = new TPaveText(0.187,0.88,0.6,0.92,"brNDC");
    TPaveText *pvALICE = new TPaveText(0.185,0.88,0.6,0.92,"brNDC");
    pvALICE->SetFillStyle(0);
    pvALICE->SetBorderSize(0);
    pvALICE->SetTextFont(42);
    pvALICE->SetTextSize(0.055);
    pvALICE->SetTextAlign(11);
    pvALICE->AddText("This Thesis");

    TPaveText *pvEn = new TPaveText(0.55,0.88,0.8,0.92,"brNDC");
    pvEn->SetFillStyle(0);
    pvEn->SetBorderSize(0);
    pvEn->SetTextFont(42);
    pvEn->SetTextSize(0.053);
    pvEn->SetTextAlign(11);
    //pvEn->AddText("p-Pb, #sqrt{#it{s}_{NN}} = 5.02 TeV, 603M events");
    pvEn->AddText("pp, #sqrt{#it{s}} = 13 TeV");

    //TPaveText *pvD = new TPaveText(0.25,0.82,0.55,0.87,"brNDC");
    //TPaveText *pvD = new TPaveText(0.247,0.82,0.55,0.87,"brNDC");
    TPaveText *pvD = new TPaveText(0.187,0.82,0.55,0.87,"brNDC");
    pvD->SetFillStyle(0);
    pvD->SetBorderSize(0);
    pvD->SetTextFont(42);
    pvD->SetTextSize(0.053);
    pvD->SetTextAlign(11);
    pvD->AddText("D^{0} #rightarrow K^{-}#pi^{+} and charge conj.");

    //TPaveText *pvJet = new TPaveText(0.25,0.77,0.55,0.82,"brNDC");
    TPaveText *pvJet = new TPaveText(0.189,0.77,0.55,0.82,"brNDC");
    pvJet->SetFillStyle(0);
    pvJet->SetBorderSize(0);
    pvJet->SetTextFont(42);
    pvJet->SetTextSize(0.053);
    pvJet->SetTextAlign(11);
    pvJet->AddText("in charged jets, anti-#it{k}_{T}, #it{R} = 0.4");
std::cout<<"C"<<std::endl;
    //TPaveText *pvEta = new TPaveText(0.25,0.72,0.6,0.77,"brNDC");
    //TPaveText *pvEta = new TPaveText(0.247,0.72,0.6,0.77,"brNDC");
    TPaveText *pvEta = new TPaveText(0.187,0.72,0.6,0.77,"brNDC");
    pvEta->SetFillStyle(0);
    pvEta->SetBorderSize(0);
    pvEta->SetTextFont(42);
    pvEta->SetTextSize(0.053);
    pvEta->SetTextAlign(11);
    pvEta->AddText("|#it{#eta}_{lab}^{jet}| < 0.5");
    //pvEta->AddText("|#it{#eta}_{jet}| < 0.6");

    TPaveText *pvpt[ptbinsDN];

      for(int i=0; i<ptbinsDN; i++){
          if(i==0 || i==1)pvpt[i] = new TPaveText(0.58,0.66,0.85,0.71,"brNDC");
          else if(i==2)pvpt[i] = new TPaveText(0.58,0.51,0.85,0.56,"brNDC");
          else pvpt[i] = new TPaveText(0.58,0.76,0.85,0.81,"brNDC");
    pvpt[i]->SetFillStyle(0);
    pvpt[i]->SetBorderSize(0);
    pvpt[i]->SetTextFont(42);
    pvpt[i]->SetTextSize(0.053);
    pvpt[i]->AddText(Form("%.0f < #it{p}_{T,D^{0}} < %.0f GeV/#it{c}",ptDbins[i],ptDbins[i+1]));

      }


std::cout<<"D"<<std::endl;


    TCanvas *cMass = new TCanvas("cMass","cMass",2160,3032);



    cMass->Divide(3,4);
    cMass->cd(1);
    gPad->SetLogy(islog);

   for(int i=0; i<ptbinsDN; i++){
       cMass->cd(i+1);
       gPad->SetLogy(islog);
    hjetpt[i]->Draw();
    hjetpt[i]->Draw("same");
    hjetpt[i]->Draw("same");
    hjetpt_s[i]->Draw("same");
    hjetpt_s[i]->Draw("same");
    hjetpt_s[i]->Draw("same");
    hjetptsub[i]->Draw("same");
    hjetptsub[i]->Draw("same");
    hjetptsub[i]->Draw("same");
    pvpt[i]->Draw("same");
   }


    cMass->cd(1);
    pvALICE->Draw("same");
    pvD->Draw("same");
    pvJet->Draw("same");
    pvEta->Draw("same");

pvEn->Draw("same");

cMass->cd(3);
    legBands1->Draw("same");
    legBands2->Draw("same");
    legBands23->Draw("same");
    legBands3->Draw("same");


std::cout<<"F"<<std::endl;
    if(islog) SaveCanvas(cMass,"RawJetPt_thesis_log_full");
    else SaveCanvas(cMass,"RawJetPt_Perf_2");
std::cout<<"G"<<std::endl;
}

void setHistoDetails(TH1 *h, Color_t color, Style_t Mstyle, Size_t size = 0.9, Width_t width=2, int scale = 0){

    if(scale)h->Scale(1,"width");
    h->SetMarkerStyle(Mstyle);
    h->SetMarkerColor(color);
    h->SetMarkerSize(size);
    h->SetLineColor(color);
    h->SetLineWidth(width);
    h->SetTitle("");
    h->GetXaxis()->SetTitle("p_{T}^{#D^0}(GeV/c)");

    return;

}

void SaveCanvas(TCanvas *c, TString name){
std::cout<<"F1"<<std::endl;
   // c->SaveAs(name+".png");
    std::cout<<"F2"<<std::endl;
    c->SaveAs(name+".pdf");
  //  c->SaveAs(name+".eps");
  //  c->SaveAs(name+".root");
 //   c->SaveAs(name+".C");


}
