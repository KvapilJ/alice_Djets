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



    const int ptbinsJetN1 = 7;
    float ptJetbins1[ptbinsJetN1+1] = {0.2,0.4,0.5,0.6,0.7,0.8,0.9,1.02};
    const int ptbinsJetN2 = 6;
    float ptJetbins2[ptbinsJetN2+1] = {0.4,0.5,0.6,0.7,0.8,0.9,1.02};
     double plotmin = 0.2, plotmax = 1;

    int promptColor = kRed+1;
    int nonpromptColor = kBlue+1;


void drawFDZ2()
{

    style();
    gStyle->SetOptStat(000);

    gStyle->SetLegendFont(42);
    //gStyle->SetLegendTextSize(0.05);


//stringstream sst;
//sst.clear(); sst.str("");
    Int_t z1 = 2;
    Int_t z2 = 5;


   //TFile *inFilePrompt = new TFile("out_806Preliminary/DjetEff_prompt_jetpt2_50.root","read");
   //TFile *inFileFD = new TFile("out_806Base_FD/DjetEff_nonPrompt_jetpt2_50.root","read");

  // TFile *inFilePrompt = new TFile("/home/jackbauer/Work/alice/analysis/pp5TeV/D0jet/results_cutTight/DzeroR03_def_437_old0/Default/efficiency/DjetEff_prompt_jetpt5_50.root","read");
  // TFile *inFileFD = new TFile("/home/jackbauer/Work/alice/analysis/pp5TeV/D0jet/results_cutTight/DzeroR03_def_437_old0/Default/efficiency/DjetEff_nonPrompt_jetpt5_50.root","read");


   TFile *inFileFD1 = new TFile(Form("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts/Default_AnalysisResults_Run2.root/FDsubtraction/JetPtSpectrum_FDsub%d.root",z1),"read");
   TFile *inFileFD2 = new TFile(Form("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts/Default_AnalysisResults_Run2.root/FDsubtraction/JetPtSpectrum_FDsub%d.root",z2),"read");


    TH1D *hFD_ratio1 = (TH1D*)inFileFD1->Get("hFD_ratio");
    TH1D *hFD_ratio_up1 = (TH1D*)inFileFD1->Get("hFD_ratio_up");
    TH1D *hFD_ratio_down1 = (TH1D*)inFileFD1->Get("hFD_ratio_down");

    TH1D *hFD_ratio2 = (TH1D*)inFileFD2->Get("hFD_ratio");
    TH1D *hFD_ratio_up2 = (TH1D*)inFileFD2->Get("hFD_ratio_up");
    TH1D *hFD_ratio_down2 = (TH1D*)inFileFD2->Get("hFD_ratio_down");

    //TGraphAsymmErrors *hFD_ratio_err = new TGraphAsymmErrors(hFD_ratio);
   /* for(Int_t i = 1; i <= ptbinsJetN;i++){
        hFD_ratio_err->SetPointEYhigh(i,hFD_ratio_up->GetBinContent(i)-hFD_ratio->GetBinContent(i));
        std::cout<<hFD_ratio_up->GetBinCenter(i)<<" "<<hFD_ratio->GetBinCenter(i)<<" "<<hFD_ratio_up->GetBinContent(i)<<" "<<hFD_ratio->GetBinContent(i)<<" "<<hFD_ratio_up->GetBinContent(i)-hFD_ratio->GetBinContent(i)<<std::endl;
        hFD_ratio_err->SetPointEYlow(i,hFD_ratio->GetBinContent(i)-hFD_ratio_down->GetBinContent(i));
    }*/
std::cout<<"A"<<std::endl;
    Double_t *xAxis1 = new double[ptbinsJetN1+1];
    for(int k=0; k<ptbinsJetN1+1; k++) xAxis1[k] = ptJetbins1[k];

    Double_t ptvaltheory1[ptbinsJetN1+1];
    Double_t ptvalunctheory1[ptbinsJetN1+1];
    Double_t valuetheory1[ptbinsJetN1+1];
    Double_t valuetheoryerrup1[ptbinsJetN1+1];
    Double_t valuetheoryerrdown1[ptbinsJetN1+1];
    for(int j=0; j<ptbinsJetN1; j++){
            ptvaltheory1[j] = (xAxis1[j]+xAxis1[j+1]) / 2.;
            ptvalunctheory1[j] = (xAxis1[j+1]-xAxis1[j]) / 2.;
            valuetheory1[j] = hFD_ratio1->GetBinContent(hFD_ratio1->GetXaxis()->FindBin(ptvaltheory1[j]));
            valuetheoryerrup1[j] = hFD_ratio_up1->GetBinContent(hFD_ratio_up1->GetXaxis()->FindBin(ptvaltheory1[j])) - valuetheory1[j];
            valuetheoryerrdown1[j] = valuetheory1[j] - hFD_ratio_down1->GetBinContent(hFD_ratio_down1->GetXaxis()->FindBin(ptvaltheory1[j]));
//std::cout<<ptvaltheory[j]<<" "<<ptvalunctheory[j]<<" "<<valuetheoryerrup[j]<<" "<<valuetheory[j]<<" "<<valuetheoryerrdown[j]<<std::endl;

    }
    std::cout<<"A"<<std::endl;
    TGraphAsymmErrors *hFD_ratio_err1 = new TGraphAsymmErrors(ptbinsJetN1,ptvaltheory1,valuetheory1,ptvalunctheory1,ptvalunctheory1,valuetheoryerrdown1,valuetheoryerrup1);

std::cout<<"A"<<std::endl;

            hFD_ratio1->SetTitle("");
            hFD_ratio1->SetMarkerColor(kRed+1);
            hFD_ratio1->SetLineColor(kRed+1);
            hFD_ratio1->SetMarkerStyle(20);
            hFD_ratio1->SetMarkerSize(1.2);
            hFD_ratio1->GetXaxis()->SetTitle("#it{z}^{ch. jet}_{#parallel}");
            hFD_ratio1->GetYaxis()->SetTitle("B Feed-Down Fraction");
            hFD_ratio1->GetXaxis()->SetLabelSize(0.04);
            hFD_ratio1->GetXaxis()->SetTitleSize(0.05);
            hFD_ratio1->GetXaxis()->SetTitleOffset(1.);
            hFD_ratio1->GetYaxis()->SetTitleOffset(1.1);
            hFD_ratio1->GetYaxis()->SetLabelSize(0.045);
            hFD_ratio1->GetYaxis()->SetTitleSize(0.05);
            hFD_ratio1->GetXaxis()->SetRangeUser(plotmin,plotmax);
            hFD_ratio1->SetMaximum(hFD_ratio1->GetMaximum()*3);

            hFD_ratio_err1->SetTitle("");
            hFD_ratio_err1->SetMarkerColor(kRed+1);
            hFD_ratio_err1->SetLineColor(kRed+1);
            hFD_ratio_err1->SetLineWidth(2.0);
            //hFD_ratio_err->SetFillColor(promptColor);
            //hFD_ratio_err->SetFillColorAlpha(kGreen,0.9);
            hFD_ratio_err1->SetMarkerStyle(20);
            hFD_ratio_err1->SetMarkerSize(1.2);
            hFD_ratio_err1->GetXaxis()->SetTitle("#it{p}_{T,ch jet} (GeV/#it{c})");
            hFD_ratio_err1->GetYaxis()->SetTitle("B Feed-Down Fraction");
            hFD_ratio_err1->GetXaxis()->SetLabelSize(0.04);
            hFD_ratio_err1->GetXaxis()->SetTitleSize(0.05);
            hFD_ratio_err1->GetXaxis()->SetTitleOffset(1.);
            hFD_ratio_err1->GetYaxis()->SetTitleOffset(1.1);
            hFD_ratio_err1->GetYaxis()->SetLabelSize(0.045);
            hFD_ratio_err1->GetYaxis()->SetTitleSize(0.05);
            hFD_ratio_err1->GetXaxis()->SetRangeUser(plotmin,plotmax);
            hFD_ratio_err1->SetMaximum(1);
std::cout<<"A"<<std::endl;

            Double_t *xAxis2 = new double[ptbinsJetN2+1];
            for(int k=0; k<ptbinsJetN2+1; k++) xAxis2[k] = ptJetbins2[k];
std::cout<<"A"<<std::endl;
            Double_t ptvaltheory2[ptbinsJetN2+1];
            Double_t ptvalunctheory2[ptbinsJetN2+1];
            Double_t valuetheory2[ptbinsJetN2+1];
            Double_t valuetheoryerrup2[ptbinsJetN2+1];
            Double_t valuetheoryerrdown2[ptbinsJetN2+1];
            for(int j=0; j<ptbinsJetN2; j++){
                    ptvaltheory2[j] = (xAxis2[j]+xAxis2[j+1]) / 2.;
                    ptvalunctheory2[j] = (xAxis2[j+1]-xAxis2[j]) / 2.;
                    valuetheory2[j] = hFD_ratio2->GetBinContent(hFD_ratio2->GetXaxis()->FindBin(ptvaltheory2[j]));
                    valuetheoryerrup2[j] = hFD_ratio_up2->GetBinContent(hFD_ratio_up2->GetXaxis()->FindBin(ptvaltheory2[j])) - valuetheory2[j];
                    valuetheoryerrdown2[j] = valuetheory2[j] - hFD_ratio_down2->GetBinContent(hFD_ratio_down2->GetXaxis()->FindBin(ptvaltheory2[j]));
        //std::cout<<ptvaltheory[j]<<" "<<ptvalunctheory[j]<<" "<<valuetheoryerrup[j]<<" "<<valuetheory[j]<<" "<<valuetheoryerrdown2[j]<<std::endl;

            }
            std::cout<<"A"<<std::endl;
            TGraphAsymmErrors *hFD_ratio_err2 = new TGraphAsymmErrors(ptbinsJetN2,ptvaltheory2,valuetheory2,ptvalunctheory2,ptvalunctheory2,valuetheoryerrdown2,valuetheoryerrup2);

std::cout<<"A"<<std::endl;

                    hFD_ratio2->SetTitle("");
                    hFD_ratio2->SetMarkerColor(kBlue+1);
                    hFD_ratio2->SetLineColor(kBlue+1);
                    hFD_ratio2->SetMarkerStyle(22);
                    hFD_ratio2->SetMarkerSize(1.2);
                    hFD_ratio2->GetXaxis()->SetTitle("#it{z}^{ch. jet}_{#parallel}");
                    hFD_ratio2->GetYaxis()->SetTitle("B Feed-Down Fraction");
                    hFD_ratio2->GetXaxis()->SetLabelSize(0.04);
                    hFD_ratio2->GetXaxis()->SetTitleSize(0.05);
                    hFD_ratio2->GetXaxis()->SetTitleOffset(1.);
                    hFD_ratio2->GetYaxis()->SetTitleOffset(1.1);
                    hFD_ratio2->GetYaxis()->SetLabelSize(0.045);
                    hFD_ratio2->GetYaxis()->SetTitleSize(0.05);
                    hFD_ratio2->GetXaxis()->SetRangeUser(plotmin,plotmax);
                    hFD_ratio2->SetMaximum(hFD_ratio2->GetMaximum()*3);

                    hFD_ratio_err2->SetTitle("");
                    hFD_ratio_err2->SetMarkerColor(kBlue+1);
                    hFD_ratio_err2->SetLineColor(kBlue+1);
                    hFD_ratio_err2->SetLineWidth(2.0);
                    //hFD_ratio_err->SetFillColor(promptColor);
                    //hFD_ratio_err->SetFillColorAlpha(kGreen,0.9);
                    hFD_ratio_err2->SetMarkerStyle(22);
                    hFD_ratio_err2->SetMarkerSize(1.2);
                    hFD_ratio_err2->GetXaxis()->SetTitle("#it{p}_{T,ch jet} (GeV/#it{c})");
                    hFD_ratio_err2->GetYaxis()->SetTitle("B Feed-Down Fraction");
                    hFD_ratio_err2->GetXaxis()->SetLabelSize(0.04);
                    hFD_ratio_err2->GetXaxis()->SetTitleSize(0.05);
                    hFD_ratio_err2->GetXaxis()->SetTitleOffset(1.);
                    hFD_ratio_err2->GetYaxis()->SetTitleOffset(1.1);
                    hFD_ratio_err2->GetYaxis()->SetLabelSize(0.045);
                    hFD_ratio_err2->GetYaxis()->SetTitleSize(0.05);
                    hFD_ratio_err2->GetXaxis()->SetRangeUser(plotmin,plotmax);
                    hFD_ratio_err2->SetMaximum(1);

                    TGraphAsymmErrors *hFD_ratio_err3 = (TGraphAsymmErrors*)hFD_ratio_err2->Clone("tmperror");
                    hFD_ratio_err3->SetMarkerColor(kGray+1);
                    hFD_ratio_err3->SetLineColor(kGray+1);

std::cout<<"A"<<std::endl;


/*

    TLegend *leg = new TLegend(0.45,0.65,0.9,0.91);
    leg->SetHeader("POWHEG D^{0}:","L");
    leg->SetTextFont(42);
    leg->SetTextSize(0.027);
    leg->AddEntry(hFD_ratio1,Form("%d < #it{p}_{T,D^{0}} < %d GeV/#it{c}, %d < #it{p}_{T,jet} < %d GeV/#it{c}",2,7,5,7),"p");
    leg->AddEntry(hFD_ratio2,Form("%d < #it{p}_{T,D^{0}} < %d GeV/#it{c}, %d < #it{p}_{T,jet} < %d GeV/#it{c}",3,36,15,50),"p");

    leg->AddEntry(hFD_ratio_err3,"Systematic Uncertainties","F");*/
TLegend *leg = new TLegend(0.45,0.8,0.9,0.95);
leg->SetHeader("POWHEG:","L");
leg->SetTextFont(42);
leg->SetTextSize(0.04);
TLegend *leg2 = new TLegend(0.45,0.63,0.9,0.78);
leg2->SetTextFont(42);
leg2->SetTextSize(0.04);
TLegend *leg3 = new TLegend(0.45,0.55,0.9,0.63);
leg3->SetTextFont(42);
leg3->SetTextSize(0.04);

leg->AddEntry(hFD_ratio1,Form("#splitline{%d < #it{p}_{T,D^{0}} < %d GeV/#it{c}}{%d < #it{p}_{T,jet} < %d GeV/#it{c}}",2,7,5,7),"p");
leg2->AddEntry(hFD_ratio2,Form("#splitline{%d < #it{p}_{T,D^{0}} < %d GeV/#it{c}}{%d < #it{p}_{T,jet} < %d GeV/#it{c}}",3,36,15,50),"p");

leg3->AddEntry(hFD_ratio_err3,"Systematic Uncertainties","F");
hFD_ratio1->SetMarkerSize(1.5);
hFD_ratio2->SetMarkerSize(1.5);
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
std::cout<<"A"<<std::endl;
    TPaveText *pt = new TPaveText(0.15,0.7,0.8,0.95,"NB NDC");
    pt->SetBorderSize(0);
    pt->SetFillStyle(0);
    pt->SetTextAlign(13);
    pt->SetTextFont(42);
    pt->SetTextSize(0.04);
    //TText *text = pt->AddText("ALICE Preliminary");
    TText *text = new TText;
    text = pt->AddText("ALICE Preliminary"); //uncomment
    text = pt->AddText("pp, #sqrt{#it{s}} = 13 TeV");
    //text = pt->AddText(Form("charged jets, anti-#it{k}_{T}, #it{R} = 0.%d, |#it{#eta}_{lab}^{jet}| < 0.%d",4,5));
    text = pt->AddText("charged jets, anti-#it{k}_{T}");
    text = pt->AddText(Form("#it{R} = 0.%d, |#it{#eta}_{lab}^{jet}| < 0.%d",4,5));
    text = pt->AddText("with D^{0}");
    //if(z==2)text = pt->AddText(Form ("with D^{0}, %d < #it{p}_{T,D^{0}} < %d GeV/#it{c}, %d < #it{p}_{T,jet} < %d GeV/#it{c}",2,7,5,7));
    //if(z==3)text = pt->AddText(Form ("with D^{0}, %d < #it{p}_{T,D^{0}} < %d GeV/#it{c}, %d < #it{p}_{T,jet} < %d GeV/#it{c}",2,10,7,10));
    //if(z==4)text = pt->AddText(Form ("with D^{0}, %d < #it{p}_{T,D^{0}} < %d GeV/#it{c}, %d < #it{p}_{T,jet} < %d GeV/#it{c}",2,15,10,15));
    //if(z==5)text = pt->AddText(Form ("with D^{0}, %d < #it{p}_{T,D^{0}} < %d GeV/#it{c}, %d < #it{p}_{T,jet} < %d GeV/#it{c}",3,36,15,50));

    TCanvas *cEff = new TCanvas("cEff","cEff",1000,800);
    //cEff->SetBatch();
    //cEff->SetLogy();
    //cMass->Divide(3,1);
    //cMass->cd(1);
  //  hFD_ratio->Draw();
  //  hFD_ratio_up->Draw("same");
  //  hFD_ratio_down->Draw("same");
    //hFD_ratio->Draw();
    //hFD_ratio_err->GetXaxis()->SetRangeUser(0,50);
    hFD_ratio2->SetBinContent(1,100);
    hFD_ratio2->GetYaxis()->SetRangeUser(0,1);
    hFD_ratio2->Draw();
    hFD_ratio_err1->Draw("E5");
    hFD_ratio_err2->Draw("E5");
    hFD_ratio1->Draw("same");
    hFD_ratio2->Draw("same");
    pt->Draw("same");

std::cout<<"A"<<std::endl;
 //   pvALICE->Draw("same");
 //   pvEn->Draw("same");
  //  pvJet->Draw("same");
  //  pvD->Draw("same");
  //  pvEta->Draw("same");
    leg3->Draw("same");
    leg2->Draw("same");
    leg->Draw("same");


    cEff->SaveAs(Form("DjetFD_sum.png"));
    cEff->SaveAs(Form("DjetFD_sum.pdf"));
    cEff->SaveAs(Form("DjetFD_sum.eps"));
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
