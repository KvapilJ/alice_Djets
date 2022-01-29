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



    const int ptbinsJetN1 = 11;
    float ptJetbins1[ptbinsJetN1+1] = {2,3,4,5,6,8,10,12,14,20,30,50};
    const int ptbinsJetN2 = 11;
    float ptJetbins2[ptbinsJetN2+1] = {2,3,4,5,6,8,10,12,14,20,30,50};
     double plotmin = 0.2, plotmax = 1;

    int promptColor = kRed+1;
    int nonpromptColor = kBlue+1;


void drawFD()
{

    style();
    gStyle->SetOptStat(000);
    gStyle->SetLegendFont(42);

    Int_t z1 = 2;
    Int_t z2 = 5;


   TFile *inFileFD1 = new TFile("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_X04/Default_AnalysisResults_Run2w18b.root/FDsubtraction/JetPtSpectrum_FDsub.root","read");
  // TFile *inFileFD2 = new TFile("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_X04/Default_AnalysisResults_Run2w18b.root/FDsubtraction/JetPtSpectrum_FDsub.root","read");




    TH1D *hFD_ratio1 = (TH1D*)inFileFD1->Get("hData_binned_sub");
    TH1D *hFD_ratio_up1 = (TH1D*)inFileFD1->Get("hData_binned_sub_up");
    TH1D *hFD_ratio_down1 = (TH1D*)inFileFD1->Get("hData_binned_sub_down");

    TH1D *hFD_ratio2 = (TH1D*)inFileFD1->Get("hFD_central_binned_fold");
    TH1D *hFD_ratio_up2 = (TH1D*)inFileFD1->Get("hFD_up_fold");
    TH1D *hFD_ratio_down2 = (TH1D*)inFileFD1->Get("hFD_down_fold");

    TH1D *hFD_data = (TH1D*)inFileFD1->Get("hData_binned");


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
std::cout<<ptvaltheory1[j]<<" "<<ptvalunctheory1[j]<<" "<<valuetheoryerrup1[j]<<" "<<valuetheory1[j]<<" "<<valuetheoryerrdown1[j]<<std::endl;

    }
    std::cout<<"A"<<std::endl;
    TGraphAsymmErrors *hFD_ratio_err1 = new TGraphAsymmErrors(ptbinsJetN1,ptvaltheory1,valuetheory1,ptvalunctheory1,ptvalunctheory1,valuetheoryerrdown1,valuetheoryerrup1);

std::cout<<"A"<<std::endl;

            hFD_ratio1->SetTitle("");
            hFD_ratio1->SetMarkerColor(kRed+1);
            hFD_ratio1->SetLineColor(kRed+1);
            hFD_ratio1->SetMarkerStyle(20);
            hFD_ratio1->SetMarkerSize(1.2);
            hFD_ratio1->GetXaxis()->SetTitle("#it{p}_{T,ch jet} (GeV/#it{c})");
            hFD_ratio1->GetYaxis()->SetTitle("d#it{N}/d#it{p}_{T,ch jet}");
            hFD_ratio1->GetXaxis()->SetLabelSize(0.04);
            hFD_ratio1->GetXaxis()->SetTitleSize(0.05);
            hFD_ratio1->GetXaxis()->SetTitleOffset(1.);
            hFD_ratio1->GetYaxis()->SetTitleOffset(1.1);
            hFD_ratio1->GetYaxis()->SetLabelSize(0.045);
            hFD_ratio1->GetYaxis()->SetTitleSize(0.05);
            hFD_ratio1->GetXaxis()->SetRangeUser(plotmin,plotmax);
          //  hFD_ratio1->SetMaximum(hFD_ratio1->GetMaximum()*3);

            hFD_ratio_err1->SetTitle("");
            hFD_ratio_err1->SetMarkerColor(kRed+1);
            hFD_ratio_err1->SetLineColor(kRed+1);
            hFD_ratio_err1->SetLineWidth(2.0);
            hFD_ratio_err1->SetMarkerStyle(20);
            hFD_ratio_err1->SetMarkerSize(1.2);
            hFD_ratio_err1->GetXaxis()->SetTitle("#it{p}_{T,ch jet} (GeV/#it{c})");
            hFD_ratio_err1->GetYaxis()->SetTitle("d#it{N}/d#it{p}_{T,ch jet}");
            hFD_ratio_err1->GetXaxis()->SetLabelSize(0.04);
            hFD_ratio_err1->GetXaxis()->SetTitleSize(0.05);
            hFD_ratio_err1->GetXaxis()->SetTitleOffset(1.);
            hFD_ratio_err1->GetYaxis()->SetTitleOffset(1.1);
            hFD_ratio_err1->GetYaxis()->SetLabelSize(0.045);
            hFD_ratio_err1->GetYaxis()->SetTitleSize(0.05);
            hFD_ratio_err1->GetXaxis()->SetRangeUser(plotmin,plotmax);
          //  hFD_ratio_err1->SetMaximum(1);
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
                    hFD_ratio2->SetMarkerStyle(24);
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
              //      hFD_ratio2->SetMaximum(hFD_ratio2->GetMaximum()*3);

                    hFD_ratio_err2->SetTitle("");
                    hFD_ratio_err2->SetMarkerColor(kBlue+1);
                    hFD_ratio_err2->SetLineColor(kBlue+1);
                    hFD_ratio_err2->SetLineWidth(2.0);
                    hFD_ratio_err2->SetMarkerStyle(24);
                    hFD_ratio_err2->SetMarkerSize(1.2);
                    hFD_ratio_err2->GetXaxis()->SetTitle("#it{p}_{T,ch jet} (GeV/#it{c})");
                    hFD_ratio_err2->GetYaxis()->SetTitle("Entries");
                    hFD_ratio_err2->GetXaxis()->SetLabelSize(0.04);
                    hFD_ratio_err2->GetXaxis()->SetTitleSize(0.05);
                    hFD_ratio_err2->GetXaxis()->SetTitleOffset(1.);
                    hFD_ratio_err2->GetYaxis()->SetTitleOffset(1.1);
                    hFD_ratio_err2->GetYaxis()->SetLabelSize(0.045);
                    hFD_ratio_err2->GetYaxis()->SetTitleSize(0.05);
                    hFD_ratio_err2->GetXaxis()->SetRangeUser(plotmin,plotmax);
               //     hFD_ratio_err2->SetMaximum(1);

                    TGraphAsymmErrors *hFD_ratio_err3 = (TGraphAsymmErrors*)hFD_ratio_err2->Clone("tmperror");
                    hFD_ratio_err3->SetMarkerColor(kGray+1);
                    hFD_ratio_err3->SetLineColor(kGray+1);

std::cout<<"A"<<std::endl;

hFD_data->SetTitle("");
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



TLegend *leg = new TLegend(0.3,0.57,0.7,0.74);
//leg->SetHeader("POWHEG:","L");
leg->SetTextFont(42);
leg->SetTextSize(0.04);
/*TLegend *leg2 = new TLegend(0.45,0.63,0.9,0.78);
leg2->SetTextFont(42);
leg2->SetTextSize(0.04);
TLegend *leg3 = new TLegend(0.45,0.55,0.9,0.63);
leg3->SetTextFont(42);
leg3->SetTextSize(0.04);*/

//leg->AddEntry(hFD_ratio1,Form("#splitline{%d < #it{p}_{T,D^{0}} < %d GeV/#it{c}}{%d < #it{p}_{T,jet} < %d GeV/#it{c}}",2,7,5,7),"p");
//leg2->AddEntry(hFD_ratio2,Form("#splitline{%d < #it{p}_{T,D^{0}} < %d GeV/#it{c}}{%d < #it{p}_{T,jet} < %d GeV/#it{c}}",3,36,15,50),"p");

leg->AddEntry(hFD_data,"Uncorrected yield","pE");
leg->AddEntry(hFD_ratio2,"B Feed-down (POWHEG-hvq+PYTHIA6)","pF");
leg->AddEntry(hFD_ratio1,"Feed-down subtracted yield","pFE");



hFD_ratio1->SetMarkerSize(1.5);
hFD_ratio2->SetMarkerSize(1.5);

std::cout<<"A"<<std::endl;
    TPaveText *pt = new TPaveText(0.15,0.78,0.8,0.95,"NB NDC");
    pt->SetBorderSize(0);
    pt->SetFillStyle(0);
    pt->SetTextAlign(13);
    pt->SetTextFont(42);
    pt->SetTextSize(0.04);
    //TText *text = pt->AddText("ALICE Preliminary");
    TString tmp = "";
    TText *text = new TText;
  //  text = pt->AddText("This Thesis"); //uncomment
  //  text = pt->AddText("pp, #sqrt{#it{s}} = 13 TeV");
    //text = pt->AddText(Form("charged jets, anti-#it{k}_{T}, #it{R} = 0.%d, |#it{#eta}_{lab}^{jet}| < 0.%d",4,5));
//    text = pt->AddText(tmp+"charged jets with D^{0}, anti-#it{k}_{T}"+Form("#it{R} = 0.%d, |#it{#eta}_{lab}^{jet}| < 0.%d",4,5));
  //  text = pt->AddText(Form("#it{R} = 0.%d, |#it{#eta}_{lab}^{jet}| < 0.%d",4,5));
  //  text = pt->AddText("with D^{0}");

    text = pt->AddText("This Thesis"); //uncomment
    text = pt->AddText("pp, #sqrt{#it{s}} = 13 TeV");
    text = pt->AddText(Form("charged jets, anti-#it{k}_{T}, #it{R} = 0.%d, |#it{#eta}_{lab}^{jet}| < 0.%d",4,5));
    text = pt->AddText(Form ("with D^{0}, %d < #it{p}_{T,D^{0}} < %d GeV/#it{c}",2,36));


    TCanvas *cEff = new TCanvas("cEff","cEff",1000,800);



    cEff->cd()->SetLogy();
    hFD_ratio2->GetYaxis()->SetRangeUser(100,8E5);

    hFD_ratio2->Draw("axis");

    hFD_ratio_err2->SetFillColor(1);
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
    hFD_ratio_err1->Draw("2E");

    hFD_ratio2->Draw("same p  e0 x0");
  hFD_ratio2->Draw("sameaxis");
  hFD_ratio2->Draw("sameaxig");

  hFD_ratio1->Draw("same p e0 x0");
hFD_ratio1->Draw("sameaxis");
hFD_ratio1->Draw("sameaxig");

hFD_data->Draw("same");
pt->Draw("same");


    /*hFD_ratio2->Draw();
    hFD_ratio_err1->Draw("E5");
    hFD_ratio_err2->Draw("E5");
    hFD_ratio1->Draw("same");
    hFD_ratio2->Draw("same");

    */

std::cout<<"A"<<std::endl;
 //   pvALICE->Draw("same");
 //   pvEn->Draw("same");
  //  pvJet->Draw("same");
  //  pvD->Draw("same");
  //  pvEta->Draw("same");
 //   leg3->Draw("same");
  //  leg2->Draw("same");
    leg->Draw("same");


 //   cEff->SaveAs(Form("DjetFD_sum.png"));
    cEff->SaveAs(Form("DjetFD_thesis.pdf"));
  //  cEff->SaveAs(Form("DjetFD_sum.eps"));
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

    //c->SaveAs(name+".png");
    c->SaveAs(name+".pdf");

}
