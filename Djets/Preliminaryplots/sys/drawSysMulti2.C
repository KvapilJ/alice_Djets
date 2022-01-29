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


    const int ptbinsJetN = 8;
    Double_t ptJetbins[ptbinsJetN+1] = {5,6,8,10,12,14,20,30,50};

    //Double_t varbins[40] = {5,6,8,10,12,14,20,30,50};
  //  Double_t xbins[27] = {2.,3.,4.,5.,6.,7.,8.,10.,12.,16.,24.,36.};

    int promptColor = kGreen+1;
    int nonpromptColor = kBlue+1;


void drawSysMulti2()
{

    style();
    gStyle->SetOptStat(000);

    gStyle->SetLegendFont(42);

    Int_t plot = 6;


    TFile *inFile[10];

    inFile[0] = new TFile("/home/kvapil/Desktop/alice_Djets/Djets/systematics/YieldExtractionXsection/YieldDistribution_Dzero_5.0to6.0.root","read");
    inFile[1] = new TFile("/home/kvapil/Desktop/alice_Djets/Djets/systematics/YieldExtractionXsection/YieldDistribution_Dzero_6.0to8.0.root","read");
    inFile[2] = new TFile("/home/kvapil/Desktop/alice_Djets/Djets/systematics/YieldExtractionXsection/YieldDistribution_Dzero_8.0to10.0.root","read");
    inFile[3] = new TFile("/home/kvapil/Desktop/alice_Djets/Djets/systematics/YieldExtractionXsection/YieldDistribution_Dzero_10.0to12.0.root","read");
    inFile[4] = new TFile("/home/kvapil/Desktop/alice_Djets/Djets/systematics/YieldExtractionXsection/YieldDistribution_Dzero_12.0to14.0.root","read");
    inFile[5] = new TFile("/home/kvapil/Desktop/alice_Djets/Djets/systematics/YieldExtractionXsection/YieldDistribution_Dzero_14.0to20.0.root","read");
    inFile[6] = new TFile("/home/kvapil/Desktop/alice_Djets/Djets/systematics/YieldExtractionXsection/YieldDistribution_Dzero_20.0to30.0.root","read");
    inFile[7] = new TFile("/home/kvapil/Desktop/alice_Djets/Djets/systematics/YieldExtractionXsection/YieldDistribution_Dzero_30.0to50.0.root","read");

    inFile[8] = new TFile("/home/kvapil/Desktop/alice_Djets/Djets/systematics/YieldExtractionXsection/04/out/YieldExtraction.root","read");

    inFile[9] = new TFile("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_X04/Default_AnalysisResults_Run2w18b.root/signalExtraction/JetPtSpectra_SB_eff.root","read");



    TH1D *histo[8];
    TH1F *histoL[41];
    TH1F *histoR;
    TH1F *histoD;
    TH2F *histoVar = new TH2F("histoVar","histoVar",ptbinsJetN,ptJetbins,40,0,41);
    histo[0]   = (TH1D*)inFile[0]->Get("fJetPtBinYieldDistribution_Bin3");
    histo[1]   = (TH1D*)inFile[1]->Get("fJetPtBinYieldDistribution_Bin4");
    histo[2]   = (TH1D*)inFile[2]->Get("fJetPtBinYieldDistribution_Bin5");
    histo[3]   = (TH1D*)inFile[3]->Get("fJetPtBinYieldDistribution_Bin6");
    histo[4]   = (TH1D*)inFile[4]->Get("fJetPtBinYieldDistribution_Bin7");
    histo[5]   = (TH1D*)inFile[5]->Get("fJetPtBinYieldDistribution_Bin8");
    histo[6]   = (TH1D*)inFile[6]->Get("fJetPtBinYieldDistribution_Bin9");
    histo[7]   = (TH1D*)inFile[7]->Get("fJetPtBinYieldDistribution_Bin10");

    histoR = (TH1F*)inFile[8]->Get("sysUnc");

    histoD = (TH1F*)inFile[9]->Get("hjetptspectrumReb");
    histoD->SetLineColor(kBlue+1);
    histoD->SetMarkerColor(kBlue+1);


    for(int i=0;i<8;i++){if(!histo[i]) std::cout<<" ERROR NO HISTOGRAM"<<std::endl;}
    if(!histoR) std::cout<<" ERROR NO HISTOGRAM"<<std::endl;
    for(int i=0;i<=40;i++) histoL[i] = (TH1F*)histoR->Clone(Form("histoL%d",i));

    for (Int_t pt = 0; pt<8;pt++){
        for (Int_t var = 0; var<=40;var++){
         //   std::cout<<"pt "<<pt+1<<" trial "<<var<<" content "<<histo[pt]->GetBinContent(var+1)<<std::endl;
            histoL[var]->SetBinContent(pt+4,histo[pt]->GetBinContent(var+1));
            histoL[var]->SetBinError(pt+4,0);
            histoL[var]->SetFillStyle(0);
            histoL[var]->GetYaxis()->SetRangeUser(0.5,1.5);
        }
    }

    for (Int_t var = 0; var<40;var++) histoL[var]->Divide(histoD);

    for (Int_t pt = 0; pt<8;pt++){
        for (Int_t var = 0; var<=40;var++){
            histoVar->SetBinContent(pt+1,var+1,histoL[var]->GetBinContent(pt+4));
           // std::cout<<histoL[var]->GetBinContent(pt+4)<<std::endl;
        }
    }




    histoVar->SetTitle("");
    histoVar->GetYaxis()->SetTitle("Trial");
    histoVar->GetXaxis()->SetTitle("#it{p}_{T,D^{0}} (GeV/#it{c})");
    histoVar->GetZaxis()->SetTitle("Ratio to central");
    histoVar->GetXaxis()->SetLabelSize(0.045);
    histoVar->GetXaxis()->SetTitleSize(0.05);
    histoVar->GetXaxis()->SetTitleOffset(1.15);
    histoVar->GetYaxis()->SetTitleOffset(1.15);
    histoVar->GetYaxis()->SetLabelSize(0.045);
    histoVar->GetYaxis()->SetTitleSize(0.05);
     histoVar->GetYaxis()->SetLabelOffset(0.01);
     histoVar->GetXaxis()->SetLabelOffset(0.01);
    histoVar->GetXaxis()->SetRangeUser(plotmin,plotmax);
    histoVar->GetZaxis()->SetLabelSize(0.045);
    histoVar->GetZaxis()->SetTitleSize(0.05);
    histoVar->GetZaxis()->SetTitleOffset(1.15);
    histoVar->GetZaxis()->SetDecimals();

    histoR->SetTitle("");

    histoR->GetXaxis()->SetTitle("#it{p}_{T,D^{0}} (GeV/#it{c})");
  histoR->GetYaxis()->SetTitle("Uncertainty (%)");
    histoR->GetXaxis()->SetLabelSize(0.045);
    histoR->GetXaxis()->SetTitleSize(0.05);
    histoR->GetXaxis()->SetTitleOffset(1.15);
    histoR->GetYaxis()->SetTitleOffset(1.15);
    histoR->GetYaxis()->SetLabelSize(0.045);
    histoR->GetYaxis()->SetTitleSize(0.05);
     histoR->GetYaxis()->SetLabelOffset(0.01);
     histoR->GetXaxis()->SetLabelOffset(0.01);
    histoR->GetXaxis()->SetRangeUser(plotmin,plotmax);

    Int_t MyPalette[256];
   // Double_t Length[] = {0.75,0.8,0.85,0.9,0.95,1,1,1.05,1.1,1.15,1.2,1.25};
    Double_t Length[] = {0,.1,.2,.3,.4,.5,.5,.6,.7,.8,.9,1.};

  /*  Double_t Red[] = {0.101960784, 0.301960784, 0.529411765, 0.729411765, 0.878431373, 1., 0.992156863, 0.956862745, 0.839215686, 0.698039216, 0.403921569};
    Double_t Green[] = {0.101960784, 0.301960784, 0.529411765, 0.729411765, 0.878431373, 1, 0.858823529, 0.647058824, 0.376470588, 0.094117647, 0};
    Double_t Blue[] = {0, 0.301960784, 0.529411765, 0.729411765, 0.878431373, 1, 0.780392157, 0.509803922, 0.301960784, 0.168627451, 0.121568627};
*/
    /*Double_t Red[] = {0.17647058823529413, 0.3264129181084199, 0.5019607843137256, 0.6941945405613226, 0.8470588235294119, 0.9691657054978854, 0.996078431372549, 0.989926951172626, 0.8784313725490196, 0.6979623221837754, 0.4980392156862745};
    Double_t Green[] = {0.0, 0.14994232987312578, 0.45098039215686286, 0.6662821991541715, 0.8549019607843138, 0.9668589004229142, 0.8784313725490196, 0.7174163783160323, 0.5098039215686274, 0.34286812764321417, 0.23137254901960785};
    Double_t Blue[] = {0.29411764705882354, 0.528642829680892, 0.6745098039215688, 0.8206074586697425, 0.9215686274509803, 0.9636293733179546, 0.7137254901960784, 0.382160707420223, 0.0784313725490196, 0.023683198769703956, 0.03137254901960784};
*/
/*
    Double_t Red[] = {0.601922337562476, 0.6941945405613226, 0.7710880430603615, 0.8470588235294119, 0.9090349865436371, 0.9691657054978854, 0.9820838139177239, 0.996078431372549, 0.994079200307574, 0.989926951172626, 0.9319492502883506};
    Double_t Green[] = {0.5629373317954633, 0.6662821991541715, 0.7609381007304884, 0.8549019607843138, 0.912879661668589, 0.9668589004229142, 0.9244136870434448, 0.8784313725490196, 0.7984621299500192, 0.7174163783160323, 0.6094579008073817};
    Double_t Blue[] = {0.750480584390619, 0.8206074586697425, 0.8715878508266052, 0.9215686274509803, 0.9455594002306805, 0.9636293733179546, 0.8436755094194541, 0.7137254901960784, 0.5477893118031527, 0.382160707420223, 0.22422145328719723};
*/
    Double_t Red[] = {0.8470588235294119, 0.7243367935409459, 0.5788542868127644, 0.4275278738946559, 0.2964244521337947, 0.17647058823529413, 0.4980392156862745, 0.6579777008842752, 0.8023068050749711, 0.9230296039984621, 0.9928489042675894, 0.996078431372549};
    Double_t Green[] = {0.8549019607843138, 0.7031141868512112, 0.5371011149557863, 0.32241445597846985, 0.11995386389850059, 0.0, 0.23137254901960785, 0.32056901191849285, 0.4387543252595155, 0.5928489042675893, 0.7492502883506343, 0.8784313725490196};
    Double_t Blue[] = {0.9215686274509804, 0.8408304498269897, 0.7329488658208382, 0.6136101499423299, 0.48173779315647824, 0.29411764705882354, 0.03137254901960784, 0.025221068819684733, 0.054748173779315644, 0.1999231064975008, 0.4456747404844289, 0.7137254901960784};


    Double_t Red2[12];
    Double_t Green2[12];
    Double_t Blue2[12];

    for(int i=0;i<12;i++){
        if(i<6){
            std::cout<<"i: "<<i<<" inv: "<<5-i<<std::endl;
            Red2[i] = Red[5-i];
            Green2[i] = Green[5-i];
            Blue2[i] = Blue[5-i];
        }
        else{
            std::cout<<"i: "<<i<<" inv: "<<17-i<<std::endl;
            Red2[i] = Red[17-i];
            Green2[i] = Green[17-i];
            Blue2[i] = Blue[17-i];
        }
    }

    Int_t FI = TColor::CreateGradientColorTable(12, Length, Red2, Green2, Blue2, 256);
    for (int i=0;i<256;i++) MyPalette[i] = FI+i;
    gStyle->SetPalette(256, MyPalette);




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
//  Form ("D^{0}, %d < #it{p}_{T,D^{0}} < %d GeV/#it{c}",2,3)
   // text = pt->AddText(Form ("%d < #it{p}_{T,ch jet} < %d GeV/#it{c}",2,50));


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
