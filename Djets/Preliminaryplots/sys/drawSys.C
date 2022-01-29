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
    Double_t ptJetbins[ptbinsJetN+1] = {2.,3.,4.,5.,6.,7.,8.,10.,12.,16.,24.,36.};
  //  Double_t xbins[27] = {2.,3.,4.,5.,6.,7.,8.,10.,12.,16.,24.,36.};

    int promptColor = kGreen+1;
    int nonpromptColor = kBlue+1;


void drawSys()
{

    style();
    gStyle->SetOptStat(000);

    gStyle->SetLegendFont(42);

    Int_t plot = 0;


    TFile *inFile = nullptr;
    TFile *inFile2 = nullptr;
    if(plot ==0){
       inFile = new TFile("/mnt/hgfs/vmware/data_141019/MC_trEff/AnalysisResults_TrEff_Run2.root","read");
    }
    else if(plot ==1){
       inFile = new TFile("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_X04/systematics/SBRANGE/SystematicsOut.root","read");
    }
    else if(plot ==2){
       inFile = new TFile("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_X04/systematics/CUTS/SystematicsOut.root","read");
    }
    else if(plot ==3){
       inFile = new TFile("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_X04/systematics/CUTSnoL2/SystematicsOut.root","read");
    }
    else if(plot ==4){
       inFile = new TFile("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_X04/systematics/JES/SystematicsOut.root","read");
    }
    else if(plot ==5){
       inFile = new TFile("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_X04/systematics/BFDu/SystematicsOut.root","read");
       inFile2 = new TFile("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_X04/systematics/BFDd/SystematicsOut.root","read");
    }
    else if(plot ==6){
       inFile = new TFile("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_X04/systematics/REFL/SystematicsOut.root","read");
    }

    TH1D *histo = nullptr;
    THStack *stack = nullptr;
    THStack *stack2 = nullptr;
     TH1D *histo2 = nullptr;
     TH2F *histo2D = nullptr;
    if(plot==0){
        TDirectoryFile *dir = (TDirectoryFile*)inFile->Get("MatchEff_Dzero");
        TList *list = (TList*)dir->Get("systUnc_Dzero");
        histo2D = (TH2F*)list->FindObject("fhSystMatchEffD");
        histo   = (TH1D*)histo2D->ProfileX("_pfx",1,-1,"");
    }
    else if(plot==4){
        stack = (THStack*)inFile->Get("hStackRatio_");
        histo   = (TH1D*)inFile->Get("hVAR");
    }
    else if( plot==5){
        stack = (THStack*)inFile2->Get("hStackRatio_");
        histo   = (TH1D*)inFile2->Get("hMaxVAR");
        stack2 = (THStack*)inFile->Get("hStackRatio_");
        histo2   = (TH1D*)inFile->Get("hMaxVAR");
    }
    else if( plot==6){
        stack = (THStack*)inFile->Get("hStackRatio_");
        histo   = (TH1D*)inFile->Get("hMaxVAR");
    }
    else{
        stack = (THStack*)inFile->Get("hStackRatio_");
        histo   = (TH1D*)inFile->Get("hRMS");
    }

    if(!histo) std::cout<<" ERROR NO HISTOGRAM"<<std::endl;

    if(plot==0){
        histo2D->SetTitle("");
        histo2D->GetYaxis()->SetTitle("Uncertainty (%)");
        histo2D->GetXaxis()->SetTitle("#it{p}_{T,D^{0}} (GeV/#it{c})");
        histo2D->GetXaxis()->SetLabelSize(0.045);
        histo2D->GetXaxis()->SetTitleSize(0.05);
        histo2D->GetXaxis()->SetTitleOffset(1.15);
        histo2D->GetYaxis()->SetTitleOffset(1.15);
        histo2D->GetYaxis()->SetLabelSize(0.045);
        histo2D->GetYaxis()->SetTitleSize(0.05);
         histo2D->GetYaxis()->SetLabelOffset(0.01);
         histo2D->GetXaxis()->SetLabelOffset(0.01);
        histo2D->GetXaxis()->SetRangeUser(2,36);
        histo2D->GetYaxis()->SetRangeUser(3,7);
        histo2D->GetYaxis()->SetDecimals();

        histo2D->GetZaxis()->SetLabelSize(0.045);
        histo2D->GetZaxis()->SetTitleSize(0.05);
        histo2D->GetZaxis()->SetTitleOffset(1.15);
        histo2D->GetZaxis()->SetLabelOffset(0.01);
    }

if(true){
    histo->SetTitle("");
    histo->GetYaxis()->SetTitle("Uncertainty (%)");
    histo->GetXaxis()->SetTitle("#it{p}_{T,D^{0}} (GeV/#it{c})");
    histo->GetXaxis()->SetLabelSize(0.045);
    histo->GetXaxis()->SetTitleSize(0.05);
    histo->GetXaxis()->SetTitleOffset(1.15);
    histo->GetYaxis()->SetTitleOffset(1.15);
    histo->GetYaxis()->SetLabelSize(0.045);
    histo->GetYaxis()->SetTitleSize(0.05);
     histo->GetYaxis()->SetLabelOffset(0.01);
     histo->GetXaxis()->SetLabelOffset(0.01);
    histo->GetXaxis()->SetRangeUser(plotmin,plotmax);
}
if(plot >= 1){
histo->GetYaxis()->SetDecimals();
histo->SetMarkerColor(kRed+1);
histo->SetLineColor(kRed+1);
histo->SetFillStyle(3005);
histo->SetFillColor(kRed+1);
}
if(plot == 5){
histo2->GetYaxis()->SetDecimals();
histo2->SetMarkerColor(kRed+1);
histo2->SetLineColor(kRed+1);
histo2->SetFillStyle(3005);
histo2->SetFillColor(kRed+1);
histo->GetYaxis()->SetDecimals();
histo->SetMarkerColor(kBlue+1);
histo->SetLineColor(kBlue+1);
histo->SetFillStyle(3005);
histo->SetFillColor(kBlue+1);
}

    TLegend *leg = nullptr ;
    if(plot < 2)    leg    = new TLegend(0.6,0.55,0.9,0.75);
    else if(plot == 2 || plot ==3) leg = new TLegend(0.14,0.5,0.5,0.7);
    else if(plot == 4) leg = new TLegend(0.14,0.2,0.5,0.4);
    else if(plot == 5) leg = new TLegend(0.5,0.35,0.7,0.55);
    else leg    = new TLegend(0.6,0.55,0.9,0.75);
    leg->SetTextFont(42);
    leg->SetTextSize(0.04);
   // leg->AddEntry(histo,"B Feed-Down Fraction","p");


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
 if(plot ==0) text = pt->AddText("PYTHIA6+GEANT3");
  if(plot >=1)  text = pt->AddText(Form("charged jets, anti-#it{k}_{T}, #it{R} = 0.%d, |#it{#eta}_{lab}^{jet}| < 0.%d",4,5));
 if(plot >=1) text = pt->AddText(Form ("with D^{0}, %d < #it{p}_{T,D^{0}} < %d GeV/#it{c}",2,36));
//  Form ("D^{0}, %d < #it{p}_{T,D^{0}} < %d GeV/#it{c}",2,3)
   // text = pt->AddText(Form ("%d < #it{p}_{T,ch jet} < %d GeV/#it{c}",2,50));


 if(plot ==0){
    histo2   = (TH1D*)histo->Rebin(ptbinsJetN+1,"hnew",ptJetbins);
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
    histo2->SetLineWidth(2);
    }
   // histo2->SetMarkerSize(0);

 if(plot ==3){
     std::cout<<histo->GetBinCenter(histo->GetNbinsX())<<" "<<histo->GetBinContent(histo->GetNbinsX())<<std::endl;
 }
 if(plot ==2){
     histo->SetBinContent(histo->GetNbinsX(),8.83688);
 }

 if(plot ==5){
     for(int i = 1; i <= histo->GetNbinsX();i++){
         //
     histo->SetBinContent(i,-histo->GetBinContent(i));
 }
 }




    TCanvas *cEff = nullptr;
  //  if(plot ==0) cEff = new TCanvas("cEff","cEff",1000,800);
  //  else {
        cEff = new TCanvas("cEff","cEff",1000*2,800);
        cEff->Divide(2,1);
  //  }

  //  hFD_ratio->GetYaxis()->SetRangeUser(0,0.8);
 //   hFD_ratio->GetXaxis()->SetRangeUser(2,36);
  if(plot ==0){
      cEff->cd(1);
      cEff->cd(1)->SetLogz();
      cEff->cd(1)->SetRightMargin(0.15);
      histo2D->Draw("colz");
              cEff->cd(2);
    histo->Draw();
    histo2->Draw("same");
  }
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
