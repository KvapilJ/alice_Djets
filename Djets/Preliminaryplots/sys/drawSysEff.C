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
#include "THStack.h"
#include "TROOT.h"
#include "TGraphAsymmErrors.h"
#include "style.C"

void setHistoDetails(TH1 *h, Color_t color, Style_t Mstyle, Width_t width);

 //double zmin = 0, zmax = 2.;
 //double jetmin = 2, jetmax = 50;
 double plotmin = 5, plotmax = 50;


    const int ptbinsJetN = 11;
    Double_t ptJetbins[ptbinsJetN+1] = {2,3,4,5,6,8,10,12,14,20,30,50};

    //Double_t varbins[40] = {5,6,8,10,12,14,20,30,50};
  //  Double_t xbins[27] = {2.,3.,4.,5.,6.,7.,8.,10.,12.,16.,24.,36.};

    int promptColor = kGreen+1;
    int nonpromptColor = kBlue+1;


void drawSysEff()
{

    style();
    gStyle->SetOptStat(000);

    gStyle->SetLegendFont(42);

    Int_t plot = 6;


    TFile *inFile[5][2];

    const char *promp[2] = {"nonPrompt","prompt"};

    for(Int_t prompt = 0; prompt <=1; prompt++){
        TString CUTSEFFdefaulfile = Form("Default_AnalysisResults_Run2w18b.root/efficiency/DjetEff_%s_jetpt5.00_50.00.root",promp[prompt]);

        inFile[0][prompt]= new TFile(Form("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_X04/Cuts_Def/Default_AnalysisResults_Run2w18b.root/efficiency/DjetEff_%s_jetpt5.00_50.00.root",promp[prompt]),"read");
        inFile[1][prompt]= new TFile(Form("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_X04/Cuts_L2/Default/efficiency/DjetEff_%s_jetpt5.00_50.00.root",promp[prompt]),"read");
        inFile[2][prompt]= new TFile(Form("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_X04/Cuts_L3/Default/efficiency/DjetEff_%s_jetpt5.00_50.00.root",promp[prompt]),"read");
        inFile[3][prompt]= new TFile(Form("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_X04/Cuts_T2/Default/efficiency/DjetEff_%s_jetpt5.00_50.00.root",promp[prompt]),"read");
        inFile[4][prompt]= new TFile(Form("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_X04/Cuts_T3/Default/efficiency/DjetEff_%s_jetpt5.00_50.00.root",promp[prompt]),"read");
}
      //  CUTSEFF->SetHistoName("hEff_reb");


    TH1D *histo[5][2];


for(Int_t prompt = 0; prompt <=1; prompt++){
    for(Int_t ihist = 0; ihist <5; ihist++){
    histo[ihist][prompt] = (TH1D*)inFile[ihist][prompt]->Get("hEff_reb");
    }
}

for(Int_t prompt = 0; prompt <=1; prompt++){
    for(Int_t ihist = 1; ihist <5; ihist++){
    histo[ihist][prompt]->Divide(histo[0][prompt]);
    }
}

for(Int_t prompt = 0; prompt <=1; prompt++){
    for(Int_t ihist = 0; ihist <5; ihist++){
    histo[ihist][prompt]->SetTitle("");
    histo[ihist][prompt]->GetXaxis()->SetTitle("#it{p}_{T,D^{0}} (GeV/#it{c})");
    if(prompt==0)histo[ihist][prompt]->GetYaxis()->SetTitle("Acceptance #times Efficiency ratio to central");
    if(prompt==1)histo[ihist][prompt]->GetYaxis()->SetTitle("Acceptance #times Efficiency ratio to central");
    histo[ihist][prompt]->GetXaxis()->SetLabelSize(0.045);
    histo[ihist][prompt]->GetXaxis()->SetTitleSize(0.05);
    histo[ihist][prompt]->GetXaxis()->SetTitleOffset(1.15);
    histo[ihist][prompt]->GetYaxis()->SetTitleOffset(1.15);
    histo[ihist][prompt]->GetYaxis()->SetLabelSize(0.045);
    histo[ihist][prompt]->GetYaxis()->SetTitleSize(0.045);
     histo[ihist][prompt]->GetYaxis()->SetLabelOffset(0.01);
     histo[ihist][prompt]->GetXaxis()->SetLabelOffset(0.01);
     histo[ihist][prompt]->GetYaxis()->SetDecimals();
  /*   histo[pt]->SetMarkerColor(kRed+1);
     histo[pt]->SetLineColor(kRed+1);
     histo[pt]->SetFillStyle(3005);
     histo[pt]->SetFillColor(kRed+1);*/
     histo[ihist][prompt]->GetYaxis()->SetRangeUser(0.5,2.0);
   }
}



   // TPaveText *pt = new TPaveText(0.15,0.67,0.8,0.95,"NB NDC");
    TPaveText *pt = nullptr;
    if (plot==0) pt = new TPaveText(0.15,0.77,0.8,0.95,"NB NDC");
    if (plot==1) pt = new TPaveText(0.15,0.75,0.8,0.95,"NB NDC");
    else pt = new TPaveText(0.15,0.75,0.8,0.95,"NB NDC");
    pt->SetBorderSize(0);
    pt->SetFillStyle(0);
    pt->SetTextAlign(13);
    pt->SetTextFont(42);
    pt->SetTextSize(0.045);
    //TText *text = pt->AddText("ALICE Preliminary");
    TText *text = new TText;
    text = pt->AddText("This Thesis"); //uncomment
    text = pt->AddText("pp, #sqrt{#it{s}} = 13 TeV");
  text = pt->AddText(Form("charged jets, anti-#it{k}_{T}, #it{R} = 0.%d, |#it{#eta}_{lab}^{jet}| < 0.%d",4,5));
 text = pt->AddText(Form ("with D^{0}, %d < #it{p}_{T,D^{0}} < %d GeV/#it{c}",2,36));
 TLegend *leg = new TLegend(0.555,0.55,0.9,0.83);
 leg->SetTextFont(42);
 leg->SetTextSize(0.045);

 leg->AddEntry(histo[1][1],"Looser variation 1","p");
 leg->AddEntry(histo[2][1],"Looser variation 2","p");
 leg->AddEntry(histo[3][1],"Tighter variation 1","p");
 leg->AddEntry(histo[4][1],"Tighter variation 2","p");
 histo[1][1]->SetLineStyle(2);
 histo[2][1]->SetLineStyle(4);
 histo[3][1]->SetLineStyle(5);
 histo[4][1]->SetLineStyle(7);
 histo[1][1]->SetLineColor(kRed+1);
 histo[2][1]->SetLineColor(kGreen+1);
 histo[3][1]->SetLineColor(kBlue+1);
 histo[4][1]->SetLineColor(kMagenta+1);
 histo[1][1]->SetMarkerColor(kRed+1);
 histo[2][1]->SetMarkerColor(kGreen+1);
 histo[3][1]->SetMarkerColor(kBlue+1);
 histo[4][1]->SetMarkerColor(kMagenta+1);
 //20,21,22,23
 histo[1][0]->SetMarkerStyle(20);
 histo[2][0]->SetMarkerStyle(21);
 histo[3][0]->SetMarkerStyle(22);
 histo[4][0]->SetMarkerStyle(23);
 histo[1][1]->SetMarkerStyle(20);
 histo[2][1]->SetMarkerStyle(21);
 histo[3][1]->SetMarkerStyle(22);
 histo[4][1]->SetMarkerStyle(23);

 histo[1][0]->SetLineStyle(2);
 histo[2][0]->SetLineStyle(4);
 histo[3][0]->SetLineStyle(5);
 histo[4][0]->SetLineStyle(7);
 histo[1][0]->SetLineColor(kRed+1);
 histo[2][0]->SetLineColor(kGreen+1);
 histo[3][0]->SetLineColor(kBlue+1);
 histo[4][0]->SetLineColor(kMagenta+1);
 histo[1][0]->SetMarkerColor(kRed+1);
 histo[2][0]->SetMarkerColor(kGreen+1);
 histo[3][0]->SetMarkerColor(kBlue+1);
 histo[4][0]->SetMarkerColor(kMagenta+1);

 TLine *line = new TLine(2,1,36,1);
 line->SetLineStyle(2);
 //line->SetLineWidth(2);

 TPaveText *pt2 = new TPaveText(0.75,0.85,0.95,0.95,"NB NDC");
 pt2->SetBorderSize(0);
 pt2->SetFillStyle(0);
 pt2->SetTextAlign(13);
 pt2->SetTextFont(42);
 pt2->SetTextSize(0.045);
 pt2->AddText("Prompt D^{0}"); //uncomment
 TPaveText *pt3 = new TPaveText(0.69,0.85,0.95,0.95,"NB NDC");
 pt3->SetBorderSize(0);
 pt3->SetFillStyle(0);
 pt3->SetTextAlign(13);
 pt3->SetTextFont(42);
 pt3->SetTextSize(0.045);
 pt3->AddText("Feed-down D^{0}");


  TCanvas *cEff = new TCanvas("cEff","cEff",2000,800);
  cEff->Divide(2,1);

    cEff->cd(1);
    histo[1][1]->Draw("");
    histo[2][1]->Draw("same");
    histo[3][1]->Draw("same");
    histo[4][1]->Draw("same");
    line->Draw();
    pt->Draw("same");
    pt2->Draw("same");
    cEff->cd(2);
    histo[1][0]->Draw("");
    histo[2][0]->Draw("same");
    histo[3][0]->Draw("same");
    histo[4][0]->Draw("same");
    line->Draw();
    pt3->Draw("same");
    leg->Draw();
    cEff->SaveAs("CutsEff.pdf");


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
