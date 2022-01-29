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
std::tuple<TCanvas*, TPad*, TPad*, TH1D*, TH1D*> PrepareCanvas(UInt_t xAxisBins, Double_t *xAxis);

 //double zmin = 0, zmax = 2.;
 //double jetmin = 2, jetmax = 50;



    const int ptbinsJetN1 = 11;
    Double_t ptJetbins1[ptbinsJetN1+1] = {2,3,4,5,6,8,10,12,14,20,30,50};
    const int ptbinsJetN2 = 11;
    float ptJetbins2[ptbinsJetN2+1] = {2,3,4,5,6,8,10,12,14,20,30,50};
     double plotmin = 0.2, plotmax = 1;

    int promptColor = kRed+1;
    int nonpromptColor = kBlue+1;


void drawClos()
{

    style();
    gStyle->SetOptStat(000);
    gStyle->SetLegendFont(42);

    Int_t z1 = 2;
    Int_t z2 = 5;


   TFile *inFileFD1 = new TFile("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_X04/Default_AnalysisResults_Run2w18b.root/unfolding_BayesClosure_5/unfoldedSpectrum_unfoldedJetSpectrum.root","read");
   TFile *inFileFD2 = new TFile("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_X04/Default_AnalysisResults_Run2w18b.root/ResponseMatrix/DetMatrixClosureData_prompt.root","read");




    TH1D *hFD_ratio1 = (TH1D*)inFileFD1->Get("unfoldedSpectrum");
    TH1D *hFD_ratio2 = (TH1D*)inFileFD2->Get("unfoldedSpectrum");
    //TH1D *hFD_data = (TH1D*)inFileFD1->Get("foldedSpectrum");



        hFD_ratio1->SetTitle("");
        hFD_ratio1->SetMarkerColor(kRed+1);
        hFD_ratio1->SetLineColor(kRed+1);
        hFD_ratio1->SetMarkerStyle(20);
        hFD_ratio1->SetMarkerSize(1.2);
        hFD_ratio1->GetXaxis()->SetTitle("#it{p}_{T,ch jet} (GeV/#it{c})");
        hFD_ratio1->GetYaxis()->SetTitle("Entries");
        hFD_ratio1->GetXaxis()->SetLabelSize(0.04);
        hFD_ratio1->GetXaxis()->SetTitleSize(0.05);
        hFD_ratio1->GetXaxis()->SetTitleOffset(1.);
        hFD_ratio1->GetYaxis()->SetTitleOffset(1.1);
        hFD_ratio1->GetYaxis()->SetLabelSize(0.045);
        hFD_ratio1->GetYaxis()->SetTitleSize(0.05);
        hFD_ratio1->GetXaxis()->SetRangeUser(plotmin,plotmax);

        hFD_ratio2->SetTitle("");
        hFD_ratio2->SetMarkerColor(kBlue+1);
        hFD_ratio2->SetLineColor(kBlue+1);
        hFD_ratio2->SetMarkerStyle(21);
        hFD_ratio2->SetMarkerSize(1.2);
        hFD_ratio2->GetXaxis()->SetTitle("#it{p}_{T,ch jet} (GeV/#it{c})");
        hFD_ratio2->GetYaxis()->SetTitle("Entries");
        hFD_ratio2->GetXaxis()->SetLabelSize(0.04);
        hFD_ratio2->GetXaxis()->SetTitleSize(0.05);
        hFD_ratio2->GetXaxis()->SetTitleOffset(1.);
        hFD_ratio2->GetYaxis()->SetTitleOffset(1.1);
        hFD_ratio2->GetYaxis()->SetLabelSize(0.045);
        hFD_ratio2->GetYaxis()->SetTitleSize(0.05);
        hFD_ratio2->GetXaxis()->SetRangeUser(plotmin,plotmax);

      /*  hFD_data->SetTitle("");
        hFD_data->SetMarkerColor(kGreen+1);
        hFD_data->SetLineColor(kGreen+1);
        hFD_data->SetMarkerStyle(20);
        hFD_data->SetMarkerSize(1.2);
        hFD_data->GetXaxis()->SetTitle("#it{p}_{T,ch jet} (GeV/#it{c})");
        hFD_data->GetYaxis()->SetTitle("Entries");
        hFD_data->GetXaxis()->SetLabelSize(0.04);
        hFD_data->GetXaxis()->SetTitleSize(0.05);
        hFD_data->GetXaxis()->SetTitleOffset(1.);
        hFD_data->GetYaxis()->SetTitleOffset(1.1);
        hFD_data->GetYaxis()->SetLabelSize(0.045);
        hFD_data->GetYaxis()->SetTitleSize(0.05);
        hFD_data->GetXaxis()->SetRangeUser(plotmin,plotmax);
*/



TLegend *leg = new TLegend(0.5,0.4,0.7,0.65);

leg->SetTextFont(42);
leg->SetTextSize(0.05);
leg->AddEntry(hFD_ratio2,"Generated","p");
leg->AddEntry(hFD_ratio1,"Unfolded with Bayes iter = 5","p");
//leg->AddEntry(hFD_data,"Refolded","l");



hFD_ratio1->SetMarkerSize(1.5);
hFD_ratio2->SetMarkerSize(1.5);

std::cout<<"A"<<std::endl;
    TPaveText *pt = new TPaveText(0.2,0.68,0.8,0.92,"NB NDC");
    pt->SetBorderSize(0);
    pt->SetFillStyle(0);
    pt->SetTextAlign(13);
    pt->SetTextFont(42);
    pt->SetTextSize(0.05);
    //TText *text = pt->AddText("ALICE Preliminary");
    TString tmp = "";
    TText *text = new TText;
    text = pt->AddText("This Thesis"); //uncomment
    text = pt->AddText("pp, #sqrt{#it{s}} = 13 TeV");
    //text = pt->AddText(Form("charged jets, anti-#it{k}_{T}, #it{R} = 0.%d, |#it{#eta}_{lab}^{jet}| < 0.%d",4,5));
    text = pt->AddText(Form("charged jets, anti-#it{k}_{T}, #it{R} = 0.%d, |#it{#eta}_{lab}^{jet}| < 0.%d",4,5));
    text = pt->AddText(Form ("with D^{0}, %d < #it{p}_{T,D^{0}} < %d GeV/#it{c}",2,36));
  //  text = pt->AddText(Form("#it{R} = 0.%d, |#it{#eta}_{lab}^{jet}| < 0.%d",4,5));
  //  text = pt->AddText("with D^{0}");

    TCanvas *canvas;
    TPad *upPad, *dowmPad;
    TH1D *placeholder_up, *placeholder_down;
    std::tie(canvas,upPad,dowmPad,placeholder_up,placeholder_down) = PrepareCanvas(ptbinsJetN1,ptJetbins1);

    TH1D *hData_binned_ratio = dynamic_cast<TH1D*>(hFD_ratio1->Clone("hFD_ratio1_ration"));
    hData_binned_ratio->Divide(hFD_ratio2);
    upPad->cd();
    hFD_ratio1->Draw("same");
    hFD_ratio2->Draw("same");
    pt->Draw("same");
    leg->Draw("same");
    dowmPad->cd();
    hData_binned_ratio->Draw("same");



    //TCanvas *cEff = new TCanvas("cEff","cEff",1000,800);



    //cEff->cd()->SetLogy();
   // hFD_ratio2->GetYaxis()->SetRangeUser(100,6E5);

   // hFD_ratio2->Draw("axis");

   /* hFD_ratio_err2->SetFillColor(1);
    hFD_ratio_err2->SetFillStyle(0);
    hFD_ratio_err2->SetLineColor(kBlue+1);
    hFD_ratio_err2->SetMarkerColor(kBlue+1);
    hFD_ratio_err2->SetMarkerStyle(24);
    hFD_ratio_err2->SetLineStyle(1);
    hFD_ratio_err2->SetLineWidth(2);
    hFD_ratio_err2->Draw("E2");

    hFD_ratio_err1->SetFillColor(1);
    hFD_ratio_err1->SetFillStyle(0);
    hFD_ratio_err1->SetLineColor(kRed+1);
    hFD_ratio_err1->SetMarkerColor(kRed+1);
    hFD_ratio_err1->SetMarkerStyle(24);
    hFD_ratio_err1->SetLineStyle(1);
    hFD_ratio_err1->SetLineWidth(2);
    hFD_ratio_err1->Draw("2E");*/

 /*   hFD_ratio2->Draw("same p  e0 x0");
  hFD_ratio2->Draw("sameaxis");
  hFD_ratio2->Draw("sameaxig");

  hFD_ratio1->Draw("same p e0 x0");
hFD_ratio1->Draw("sameaxis");
hFD_ratio1->Draw("sameaxig");*/

//hFD_data->Draw("same");
//pt->Draw("same");


    /*hFD_ratio2->Draw();
    hFD_ratio_err1->Draw("E5");
    hFD_ratio_err2->Draw("E5");
    hFD_ratio1->Draw("same");
    hFD_ratio2->Draw("same");

    */

std::cout<<"A"<<std::endl;
   // leg->Draw("same");

    canvas->SaveAs(Form("DjetClosure_thesis.pdf"));




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

    //c->SaveAs(name+".png");
    c->SaveAs(name+".pdf");

}

std::tuple<TCanvas*, TPad*, TPad*, TH1D*, TH1D*> PrepareCanvas(UInt_t xAxisBins, Double_t *xAxis){
    style();
    //prepare main canvas
    TCanvas *FinalSpectrum = new TCanvas("FinalSpectrum", "FinalSpectrum",0,45,700,700);

    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    FinalSpectrum->SetHighLightColor(2);
    FinalSpectrum->Range(0,0,1,1);
    FinalSpectrum->SetFillColor(0);
    FinalSpectrum->SetBorderMode(0);
    FinalSpectrum->SetBorderSize(2);
    FinalSpectrum->SetFrameBorderMode(0);

    //Set primitives in upper pad
    TPad *pad_top = new TPad("pad_top", "pad_top",0,0.35,1,1);
    pad_top->Draw();
    pad_top->cd();
    pad_top->Range(-1.986821e-07,-4.69897,33.33333,0.3499945);
    pad_top->SetFillColor(0);
    pad_top->SetBorderMode(0);
    pad_top->SetBorderSize(2);
    pad_top->SetLogy();
    pad_top->SetTickx(1);
    pad_top->SetTicky(1);
    pad_top->SetLeftMargin(0.18f);
    pad_top->SetBottomMargin(0);
    pad_top->SetFrameBorderMode(0);
    pad_top->SetFrameBorderMode(0);

    FinalSpectrum->cd();

    //Set primitives in bottom pad
    TPad *pad_bottom = new TPad("pad_bottom", "pad_bottom",0,0,1,0.35);
    pad_bottom->Draw();
    pad_bottom->cd();
    pad_bottom->Range(-1.986821e-07,-0.9209589,33.33333,2.49);
    pad_bottom->SetFillColor(0);
    pad_bottom->SetBorderMode(0);
    pad_bottom->SetBorderSize(2);
    pad_bottom->SetGridy();
    pad_bottom->SetTickx(1);
    pad_bottom->SetTicky(1);
    pad_bottom->SetLeftMargin(0.18f);
    pad_bottom->SetTopMargin(0);
    pad_bottom->SetBottomMargin(0.27f);
    pad_bottom->SetFrameBorderMode(0);
    pad_bottom->SetFrameBorderMode(0);

    pad_top->cd();
    TH1D *hEmpty_up = new TH1D("hEmpty_up","Central Values",static_cast<Int_t>(xAxisBins), xAxis);
    hEmpty_up->SetMinimum(4000);
    hEmpty_up->SetMaximum(5E6);
    hEmpty_up->GetXaxis()->SetTitle("#it{p}_{T,ch jet} (GeV/#it{c})");
    hEmpty_up->GetXaxis()->SetLabelFont(43);
    hEmpty_up->GetXaxis()->SetLabelSize(0.035f);
    hEmpty_up->GetXaxis()->SetTitleSize(0.035f);
    hEmpty_up->GetXaxis()->SetTitleFont(42);
    hEmpty_up->GetYaxis()->SetTitle("Entries");
    hEmpty_up->GetYaxis()->SetLabelFont(43);
    hEmpty_up->GetYaxis()->SetLabelSize(22);
    hEmpty_up->GetYaxis()->SetTitleSize(26);
    hEmpty_up->GetYaxis()->SetLabelOffset(0.015f);
    hEmpty_up->GetYaxis()->SetTitleOffset(2.f);
    hEmpty_up->GetYaxis()->SetTitleFont(43);
    hEmpty_up->GetYaxis()->SetDecimals();
    hEmpty_up->GetXaxis()->SetRangeUser(5,50);
    hEmpty_up->Draw("axis");
    pad_bottom->cd();
    TH1D *hEmpty_bottom = new TH1D("hEmpty_bottom","Central Values",static_cast<Int_t>(xAxisBins), xAxis);
    hEmpty_bottom->SetMinimum(0.9);
    hEmpty_bottom->SetMaximum(1.09);
    hEmpty_bottom->GetXaxis()->SetTitle("#it{p}_{T,ch jet} (GeV/#it{c})");
    hEmpty_bottom->GetXaxis()->SetLabelFont(43);
    hEmpty_bottom->GetXaxis()->SetLabelSize(22);
    hEmpty_bottom->GetXaxis()->SetTitleSize(26);
    hEmpty_bottom->GetXaxis()->SetTitleOffset(3.f);
    hEmpty_bottom->GetXaxis()->SetTitleFont(43);
    hEmpty_bottom->GetYaxis()->SetTitle("Ratio");
    hEmpty_bottom->GetYaxis()->SetNdivisions(509);
    hEmpty_bottom->GetYaxis()->CenterTitle();
    hEmpty_bottom->GetYaxis()->SetDecimals();
    hEmpty_bottom->GetYaxis()->SetLabelOffset(0.015f);
    hEmpty_bottom->GetXaxis()->SetLabelOffset(0.02f);
    hEmpty_bottom->GetYaxis()->SetLabelFont(43);
    hEmpty_bottom->GetYaxis()->SetLabelSize(22);
    hEmpty_bottom->GetYaxis()->SetTitleSize(26);
    hEmpty_bottom->GetYaxis()->SetTitleOffset(2.f);
    hEmpty_bottom->GetYaxis()->SetTitleFont(43);
    hEmpty_bottom->GetXaxis()->SetRangeUser(5,50);
    hEmpty_bottom->Draw("axis");
    TLine *line = new TLine(5,1,50,1);
    line->SetLineStyle(2);
    line->Draw("same");

    return std::make_tuple(FinalSpectrum,pad_top, pad_bottom, hEmpty_up,hEmpty_bottom);

}
