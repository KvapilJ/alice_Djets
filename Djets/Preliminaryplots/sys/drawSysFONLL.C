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
#include "THnSparse.h"
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


void drawSysFONLL()
{

    style();
    gStyle->SetOptStat(000);

    gStyle->SetLegendFont(42);

    TString pathin ="/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_ZQMpreliminary/FONLL/y05/";

    TString period[32] = {"AnalysisResults_FONLL_LHC17e_hadronPID",
                        "AnalysisResults_FONLL_LHC17f_hadronPID",
                        "AnalysisResults_FONLL_LHC17h_hadronPID",
                        "AnalysisResults_FONLL_LHC17i_hadronPID",
                        "AnalysisResults_FONLL_LHC17j_hadronPID",
                        "AnalysisResults_FONLL_LHC17k_hadronPID",
                        "AnalysisResults_FONLL_LHC17l_hadronPID",
                        "AnalysisResults_FONLL_LHC17m_hadronPID",
                        "AnalysisResults_FONLL_LHC17o_hadronPID",
                        "AnalysisResults_FONLL_LHC17r_hadronPID",
                        "AnalysisResults_FONLL_LHC16k_hadronPID",
                        "AnalysisResults_FONLL_LHC16l_hadronPID",
                        "AnalysisResults_FONLL_LHC16d_pass1",
                        "AnalysisResults_FONLL_LHC16e_pass1",
                        "AnalysisResults_FONLL_LHC16g_pass1",
                        "AnalysisResults_FONLL_LHC16h_pass1_positiveMagField",
                        "AnalysisResults_FONLL_LHC16h_pass1_negativeMagField",
                        "AnalysisResults_FONLL_LHC16j_pass1",
                        "AnalysisResults_FONLL_LHC16o_pass1_hadronPID",
                        "AnalysisResults_FONLL_LHC16p_pass1",
                        "AnalysisResults_FONLL_LHC18d_hadronPID",
                        "AnalysisResults_FONLL_LHC18e_hadronPID",
                        "AnalysisResults_FONLL_LHC18f_hadronPID",
                        "AnalysisResults_FONLL_LHC18g_hadronPID",
                        "AnalysisResults_FONLL_LHC18h_hadronPID",
                        "AnalysisResults_FONLL_LHC18i_hadronPID",
                        "AnalysisResults_FONLL_LHC18k_hadronPID",
                        "AnalysisResults_FONLL_LHC18l_hadronPID",
                        "AnalysisResults_FONLL_LHC18m_hadronPID",
                        "AnalysisResults_FONLL_LHC18n_hadronPID",
                        "AnalysisResults_FONLL_LHC18o_hadronPID",
                        "AnalysisResults_FONLL_LHC18p_hadronPID" };
    TString mode[2] ={"central","FONLL"};

    TFile *file[32][2];
    TH1D *hist[32][2];
    TH1D *hratio[32][2];

    THnSparseF *s[32][2];
    TH1D *hSpect[32][2];
    TH1D *hCent;
    TH1D *hFONLL;


    for (Int_t m = 0; m<2;m++){
        for (Int_t f = 0; f<32;f++){
            file[f][m] = new TFile(pathin+period[f] +"_"+ mode[m]+".root");
            hist[f][m] = dynamic_cast<TH1D*>(file[f][m]->Get("h_reb"));
            if(!hist[f][m])std::cout<<"hist not found"<<std::endl;
            //THnSparseF* sMC = dynamic_cast<THnSparseF*>(file[f][0]->Get("CFHFccontainer0_central_SelStep8_proj_0_1"));
            if(m==0)s[f][0] = (THnSparseF*)file[f][0]->Get("CFHFccontainer0_central_SelStep8_proj_0_1");
            if(m==1)s[f][1] = (THnSparseT<TArrayF>*)file[f][1]->Get("CFHFccontainer0_FONLL_SelStep8_proj_0_1");
            hSpect[f][m] = dynamic_cast<TH1D*>(s[f][m]->Projection(0));
        }
    }
    TH1D *histo = dynamic_cast<TH1D*>(hist[0][0]->Clone("unc05"));;

    for (Int_t f = 0; f<32;f++){
        hratio[f][0]= dynamic_cast<TH1D*>(hist[f][0]->Clone(Form("h_ratio05_%d",f)));
        hratio[f][0]->Divide(hist[f][0],hist[f][1],1,1,"B");

        if(f==0){
            hCent= dynamic_cast<TH1D*>(hSpect[0][0]->Clone("hCent"));
            hFONLL= dynamic_cast<TH1D*>(hSpect[0][1]->Clone("hFONLL"));
        }
        else{
            hCent->Add(hSpect[f][0]);
            hFONLL->Add(hSpect[f][1]);
        }
    }

    for(Int_t bin = 1; bin <=12;bin++){
        Double_t u05 = 0;
        for (Int_t f = 0; f<32;f++){
            u05 += TMath::Abs(1-hratio[f][0]->GetBinContent(bin));
        }
        histo->SetBinContent(bin,u05/32*100);
        histo->SetBinError(bin,0);
    }


    histo->SetTitle("");
    histo->GetXaxis()->SetTitle("#it{p}_{T}^{D} (GeV/#it{c})");
    histo->GetYaxis()->SetTitle("FONLL / PYTHIA (%)");
    histo->GetXaxis()->SetLabelSize(0.04);
    histo->GetXaxis()->SetTitleSize(0.05);
    histo->GetXaxis()->SetTitleOffset(1.);
    histo->GetYaxis()->SetTitleOffset(1.1);
    histo->GetYaxis()->SetLabelSize(0.045);
    histo->GetYaxis()->SetTitleSize(0.05);
    histo->GetXaxis()->SetRangeUser(plotmin,plotmax);


    TLegend *leg = new TLegend(0.15,0.6,0.6,0.7);
    leg->SetTextFont(42);
    leg->SetTextSize(0.045);
    leg->AddEntry(histo,"B Feed-Down Fraction","p");


   // TPaveText *pt = new TPaveText(0.15,0.67,0.8,0.95,"NB NDC");
    TPaveText *pt = new TPaveText(0.15,0.77,0.8,0.95,"NB NDC");
    pt->SetBorderSize(0);
    pt->SetFillStyle(0);
    pt->SetTextAlign(13);
    pt->SetTextFont(42);
    pt->SetTextSize(0.045);
    //TText *text = pt->AddText("ALICE Preliminary");
    TText *text = new TText;
    text = pt->AddText("This Thesis"); //uncomment
    text = pt->AddText("pp, #sqrt{#it{s}} = 13 TeV");
    text = pt->AddText("PYTHIA6+GEANT3 / FONLL");
   // text = pt->AddText(Form("charged jets with D^{0}, anti-#it{k}_{T}, #it{R} = 0.%d, |#it{#eta}_{lab}^{jet}| < 0.%d",4,5));
  //  text = pt->AddText(Form ("%d < #it{p}_{T,ch jet} < %d GeV/#it{c}",2,50));


    histo->GetXaxis()->SetRangeUser(2.,36.);
    histo->GetYaxis()->SetRangeUser(0.,0.35);
    histo->GetYaxis()->SetDecimals();
    histo->SetMarkerColor(kRed+1);
    histo->SetLineColor(kRed+1);
    histo->SetFillStyle(3005);
    histo->SetFillColor(kRed+1);

hCent->Scale(1./hCent->Integral());
hFONLL->Scale(1./hFONLL->Integral());


    TCanvas *cEff = new TCanvas("cEff","cEff",1000,800);

  //  hFD_ratio->GetYaxis()->SetRangeUser(0,0.8);
 //   hFD_ratio->GetXaxis()->SetRangeUser(2,36);
    histo->Draw("h");
    pt->Draw("same");

    cEff->SaveAs("MCshape.pdf");

    TCanvas *cEff2 = new TCanvas("cEff2","cEff2",1000,800);
    hCent->Draw();
    hFONLL->Draw("same");
    pt->Draw("same");

    cEff2->SaveAs("MCshape2.pdf");




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
