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


void drawSysME()
{

    style();
    gStyle->SetOptStat(000);

    gStyle->SetLegendFont(42);

    Int_t plot = 6;


    TFile *inFile[4];

    inFile[0] = new TFile("/home/kvapil/Desktop/alice_Djets/Djets/D0pp13TeVTrkEff.root","read");
    inFile[1] = new TFile("/home/kvapil/Desktop/alice_Djets/Djets/D0pp13TeVME.root","read");
    //inFile[2] = new TFile("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_X04/Simulations/BFeedDown/AnalysisResults_Run2w18b.root0/JetPt_AnalysisResults_FastSim_powheg+pythia6+evtgen_beauty_uF05uR1_Dpt2_36_effScaled_Dzero.root","read");
   // inFile[3] = new TFile("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_X04/Simulations/BFeedDown/AnalysisResults_Run2w18b.root0/JetPt_AnalysisResults_FastSim_powheg+pythia6+evtgen_beauty_uF1uR05_Dpt2_36_effScaled_Dzero.root","read");



    TH1F *histo[2];
  /*  TH1D *histo2[10];
    TH1F *histoL[41];
    TH1F *histoR;
    TH1F *histoD;
    TH2F *histoVar = new TH2F("histoVar","histoVar",ptbinsJetN,ptJetbins,40,0,41);*/


    //histo[0]   = (TH1F*)inFile[0]->Get("hTrEff");
    Double_t bins[4] = {2,6,12,36};
    histo[0]   = new TH1F("htrk","htrk",3,bins);
    histo[0]->SetBinContent(1,1);
    histo[0]->SetBinContent(2,2);
    histo[0]->SetBinContent(3,1);


    histo[1]   = (TH1F*)inFile[1]->Get("h");

    for(int i=0;i<2;i++){if(!histo[i]) std::cout<<" ERROR NO HISTOGRAM"<<std::endl;}

 for (Int_t pt = 0; pt<2;pt++){
    histo[pt]->SetTitle("");
    if(pt==1)histo[pt]->GetXaxis()->SetTitle("#it{p}_{T,track} (GeV/#it{c})");
    if(pt==0)histo[pt]->GetXaxis()->SetTitle("#it{p}_{T,D^{0}} (GeV/#it{c})");
    if(pt==1)histo[pt]->GetYaxis()->SetTitle("TPC-ITS Matching Efficiency Uncertainty (%)");
    if(pt==0)histo[pt]->GetYaxis()->SetTitle("TPC Tracking Efficiency Uncertainty (%)");
    histo[pt]->GetXaxis()->SetLabelSize(0.045);
    histo[pt]->GetXaxis()->SetTitleSize(0.05);
    histo[pt]->GetXaxis()->SetTitleOffset(1.15);
    histo[pt]->GetYaxis()->SetTitleOffset(1.15);
    histo[pt]->GetYaxis()->SetLabelSize(0.045);
    histo[pt]->GetYaxis()->SetTitleSize(0.045);
     histo[pt]->GetYaxis()->SetLabelOffset(0.01);
     histo[pt]->GetXaxis()->SetLabelOffset(0.01);
     histo[pt]->GetYaxis()->SetDecimals();
     histo[pt]->SetMarkerColor(kRed+1);
     histo[pt]->SetLineColor(kRed+1);
     histo[pt]->SetFillStyle(3005);
     histo[pt]->SetFillColor(kRed+1);
     histo[pt]->GetYaxis()->SetRangeUser(0,3.5);
   }



   // TPaveText *pt = new TPaveText(0.15,0.67,0.8,0.95,"NB NDC");
    TPaveText *pt = nullptr;
    if (plot==0) pt = new TPaveText(0.15,0.77,0.8,0.95,"NB NDC");
    if (plot==1) pt = new TPaveText(0.15,0.75,0.8,0.95,"NB NDC");
    else pt = new TPaveText(0.15,0.8,0.8,0.95,"NB NDC");
    pt->SetBorderSize(0);
    pt->SetFillStyle(0);
    pt->SetTextAlign(13);
    pt->SetTextFont(42);
    pt->SetTextSize(0.045);
    //TText *text = pt->AddText("ALICE Preliminary");
    TText *text = new TText;
    text = pt->AddText("This Thesis"); //uncomment
     text = pt->AddText("pp, #sqrt{#it{s}} = 13 TeV");
 //   text = pt->AddText("POWHEG + PYTHIA 6");
 //   text = pt->AddText(Form("charged jets, anti-#it{k}_{T}, #it{R} = 0.%d, |#it{#eta}_{lab}^{jet}| < 0.%d",4,5));
 //   text = pt->AddText(Form ("with D^{0}, %d < #it{p}_{T,D^{0}} < %d GeV/#it{c}",2,36));
//  Form ("D^{0}, %d < #it{p}_{T,D^{0}} < %d GeV/#it{c}",2,3)
   // text = pt->AddText(Form ("%d < #it{p}_{T,ch jet} < %d GeV/#it{c}",2,50));





  TCanvas *cEff = new TCanvas("cEff","cEff",2000,800);
  cEff->Divide(2,1);

    cEff->cd(1);
    histo[0]->Draw("h");
    pt->Draw("same");
    cEff->cd(2);
    histo[1]->Draw("h");
    cEff->SaveAs("METRK.pdf");






  /*
    TCanvas *cEff = nullptr;
        cEff = new TCanvas("cEff","cEff",1000*2,800);
        cEff->Divide(2,1);

    cEff->cd(1);
    cEff->cd(1)->SetRightMargin(0.175);


    //histoL[0]->Draw();
   // for(int i=1;i<40;i++)histoL[i]->Draw("same");
    //histoD->Draw("same");
    histoVar->GetZaxis()->SetRangeUser(0.75,1.25);
    histoVar->Draw("colz");
    cEff->cd(2);
    histoR->Draw();
    gPad->Update();
    pt->Draw("same");
    cEff->SaveAs("Multi2.pdf");

    */
 //   histo2->Draw("same");

/*}
  if(plot >=1){
      cEff->cd(1);
    stack->Draw("nostack");
    if(plot==5) stack2->Draw("nostack same");
    cEff->cd(2);
    histo->Draw();
    if(plot==5){
        histo2->Draw("same");

  }
  }

if(plot ==0){
    cEff->SaveAs("TrackEff.pdf");
}

if(plot >=1){
    stack->SetTitle("");
    stack->GetXaxis()->SetTitle("#it{p}_{T,D^{0}} (GeV/#it{c})");
    stack->GetXaxis()->SetLabelSize(0.045);
    stack->GetXaxis()->SetTitleSize(0.05);
    stack->GetXaxis()->SetTitleOffset(1.15);
    stack->GetYaxis()->SetTitleOffset(1.15);
    stack->GetYaxis()->SetLabelSize(0.045);
    stack->GetYaxis()->SetTitleSize(0.05);
     stack->GetYaxis()->SetLabelOffset(0.01);
     stack->GetXaxis()->SetLabelOffset(0.01);
    stack->GetXaxis()->SetRangeUser(plotmin,plotmax);
    //if(plot ==1)stack->GetYaxis()->SetLimits(0.85,1.2);


    if(plot ==1){
    stack->GetYaxis()->SetTitle("Ratio to central (S:2#sigma, B:4-9#sigma)");
    leg->AddEntry((TH1D*)stack->GetHists()->At(0),"S:2#sigma, B:3-8#sigma","p");
    leg->AddEntry((TH1D*)stack->GetHists()->At(1),"S:2#sigma, B:4-10#sigma","p");
    leg->AddEntry((TH1D*)stack->GetHists()->At(2),"S:2#sigma, B:4-12#sigma","p");
    leg->AddEntry((TH1D*)stack->GetHists()->At(3),"S:2#sigma, B:4-15#sigma","p");
    leg->AddEntry((TH1D*)stack->GetHists()->At(4),"S:3#sigma, B:4-9#sigma","p");
    leg->AddEntry((TH1D*)stack->GetHists()->At(5),"S:3#sigma, B:4-12#sigma","p");
    stack->SetMinimum(0.85);
    stack->SetMaximum(1.2);
    }
    else if (plot ==2){
        stack->GetYaxis()->SetTitle("Ratio to central cuts");
        leg->AddEntry((TH1D*)stack->GetHists()->At(0),"Looser variation 1","p");
        leg->AddEntry((TH1D*)stack->GetHists()->At(1),"Looser variation 2 (excl. last bin)","p");
        leg->AddEntry((TH1D*)stack->GetHists()->At(2),"Tighter variation 1","p");
        leg->AddEntry((TH1D*)stack->GetHists()->At(3),"Tighter variation 2","p");
        ((TH1D*)stack->GetHists()->At(0))->SetLineStyle(2);
        ((TH1D*)stack->GetHists()->At(1))->SetLineStyle(4);
        ((TH1D*)stack->GetHists()->At(2))->SetLineStyle(5);
        ((TH1D*)stack->GetHists()->At(3))->SetLineStyle(7);
        ((TH1D*)stack->GetHists()->At(0))->SetLineColor(kRed+1);
        ((TH1D*)stack->GetHists()->At(1))->SetLineColor(kGreen+1);
        ((TH1D*)stack->GetHists()->At(2))->SetLineColor(kBlue+1);
        ((TH1D*)stack->GetHists()->At(3))->SetLineColor(kMagenta+1);
        ((TH1D*)stack->GetHists()->At(0))->SetMarkerColor(kRed+1);
        ((TH1D*)stack->GetHists()->At(1))->SetMarkerColor(kGreen+1);
        ((TH1D*)stack->GetHists()->At(2))->SetMarkerColor(kBlue+1);
        ((TH1D*)stack->GetHists()->At(3))->SetMarkerColor(kMagenta+1);
        ((TH1D*)stack->GetHists()->At(1))->SetBinContent(((TH1D*)stack->GetHists()->At(1))->GetNbinsX(),1000);
        stack->SetMinimum(0.5);
        stack->SetMaximum(2.3);
        histo->GetYaxis()->SetRangeUser(0,10);
   }
    else if (plot==3){
        stack->GetYaxis()->SetTitle("Ratio to central cuts");
        leg->AddEntry((TH1D*)stack->GetHists()->At(0),"Looser variation 1","p");
        leg->AddEntry((TH1D*)stack->GetHists()->At(1),"Tighter variation 1","p");
        leg->AddEntry((TH1D*)stack->GetHists()->At(2),"Tighter variation 2","p");
        ((TH1D*)stack->GetHists()->At(0))->SetLineStyle(2);
        ((TH1D*)stack->GetHists()->At(1))->SetLineStyle(5);
        ((TH1D*)stack->GetHists()->At(2))->SetLineStyle(7);
        ((TH1D*)stack->GetHists()->At(0))->SetLineColor(kRed+1);
        ((TH1D*)stack->GetHists()->At(1))->SetLineColor(kBlue+1);
        ((TH1D*)stack->GetHists()->At(2))->SetLineColor(kMagenta+1);;
        ((TH1D*)stack->GetHists()->At(0))->SetMarkerColor(kRed+1);
        ((TH1D*)stack->GetHists()->At(1))->SetMarkerColor(kBlue+1);
        ((TH1D*)stack->GetHists()->At(2))->SetMarkerColor(kMagenta+1);
        stack->SetMinimum(0.5);
        stack->SetMaximum(2.3);
        histo->GetYaxis()->SetRangeUser(0,10);
   }
    else if (plot==4){
        stack->GetYaxis()->SetTitle("Ratio to default");
        leg->AddEntry((TH1D*)stack->GetHists()->At(0),"4% track rejection","p");
        //stack->SetMinimum(0.5);
        //stack->SetMaximum(2.3);
        //histo->GetYaxis()->SetRangeUser(0,10);
   }
    else if (plot==5){
        histo->GetYaxis()->SetRangeUser(-25,25);
        stack->GetYaxis()->SetTitle("Ratio to default");
        leg->AddEntry((TH1D*)stack2->GetHists()->At(0),"Feed-down upper variation","l");
        leg->AddEntry((TH1D*)stack->GetHists()->At(0),"Feed-down lower variation","l");
        ((TH1D*)stack2->GetHists()->At(0))->SetLineColor(kRed+1);
        ((TH1D*)stack->GetHists()->At(0))->SetLineColor(kBlue+1);
        stack->SetMinimum(0.7);
        stack->SetMaximum(1.4);
        //histo->SetMinimum(-25);
        //histo->SetMaximum(20);
   }
    else if (plot==6){
        stack->GetYaxis()->SetTitle("Ratio to default");
        leg->AddEntry((TH1D*)stack->GetHists()->At(0),"reflection -50%","l");
        leg->AddEntry((TH1D*)stack->GetHists()->At(1),"reflection +50%","l");
        stack->SetMinimum(0.95);
        stack->SetMaximum(1.15);
        histo->GetYaxis()->SetRangeUser(0,5);
   }
}
if(plot ==0){
pt->Draw("same");
leg->Draw();
} else{
    cEff->cd(1);
    pt->Draw("same");
    leg->Draw();
}

if(plot ==1)cEff->SaveAs("SBRange.pdf");
if(plot ==2)cEff->SaveAs("CUTS.pdf");
if(plot ==3)cEff->SaveAs("CUTSnoL2.pdf");
if(plot ==4)cEff->SaveAs("JES.pdf");
if(plot ==5)cEff->SaveAs("BFD.pdf");
if(plot ==6)cEff->SaveAs("REFL.pdf");
*/

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
