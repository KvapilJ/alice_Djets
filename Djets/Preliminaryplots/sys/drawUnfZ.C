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


void drawUnfZ()
{

    style();
    gStyle->SetOptStat(000);

    gStyle->SetLegendFont(42);

    TFile * rfile;
    TH1D* h[3][5];
    TH2D* h2[5];
  //  TH1D* hRMS[1];


    rfile = new TFile("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_z04F/Default_AnalysisResults_Run2w18b.root/unfolding_2D_5/unfoldedSpectrum_unfoldedJetSpectrum.root","READ");


    for(Int_t j = 0; j< 5;j++){
        h[0][j] = dynamic_cast<TH1D*>(rfile->Get(Form("Rawproj%d",j+1)));
        h[1][j] = dynamic_cast<TH1D*>(rfile->Get(Form("unfoldedSpectrumKineEff%d",j+1)));
        h[2][j] = dynamic_cast<TH1D*>(rfile->Get(Form("foldedSpectrum%d",j+1)));
        if(!h[0][j]) std::cout<<"Cannot open histo: "<<"0 "<<j<<std::endl;
        if(!h[1][j]) std::cout<<"Cannot open histo: "<<"1 "<<j<<std::endl;
        if(!h[2][j]) std::cout<<"Cannot open histo: "<<"2 "<<j<<std::endl;
    }

    const int     fzptJetTrueN = 5;
    double        fzptJetTrueA[fzptJetTrueN+1] = {2,5,7,10,15,50};
    const int     fzbinsJetTrueN = 5;
    double        fzbinsJetTrueAPrompt[10] ={0.4,0.6,0.7,0.8,0.9,1.0};


    h2[0]= new TH2D ("raw", "raw",fzbinsJetTrueN,fzbinsJetTrueAPrompt,fzptJetTrueN,fzptJetTrueA);
    h2[1]= new TH2D ("unf", "unf",fzbinsJetTrueN,fzbinsJetTrueAPrompt,fzptJetTrueN,fzptJetTrueA);
    h2[2]= new TH2D ("ref", "ref",fzbinsJetTrueN,fzbinsJetTrueAPrompt,fzptJetTrueN,fzptJetTrueA);



    for(Int_t ih = 0; ih < 3;ih++){
        for(Int_t ix = 1; ix< 6;ix++){
            for(Int_t iy = 1; iy< 6;iy++){
                h2[ih]->SetBinContent(ix,iy,h[ih][iy-1]->GetBinContent(ix));
                h2[ih]->SetBinError(ix,iy,h[ih][iy-1]->GetBinError(ix));
            }
        }
    }

    h2[3] = (TH2D*)h2[1]->Clone("unf/raw");
    h2[3]->Divide(h2[0]);
    h2[4] = (TH2D*)h2[2]->Clone("ref/raw");
    h2[4]->Divide(h2[0]);

    for(Int_t ih = 0; ih < 5;ih++){
        h2[ih]->SetTitle("");
       h2[ih]->GetYaxis()->SetTitle("#it{p}_{T,ch jet} (GeV/#it{c})");
       h2[ih]->GetXaxis()->SetTitle("#it{z}_{||}");
        if(ih==3)h2[ih]->GetZaxis()->SetTitle("Unfolded (reg=5) / Measured");
        if(ih==4)h2[ih]->GetZaxis()->SetTitle("Refolded / Measured");
        h2[ih]->GetXaxis()->SetLabelSize(0.045);
        h2[ih]->GetXaxis()->SetTitleSize(0.05);
        h2[ih]->GetXaxis()->SetTitleOffset(1.15);
        h2[ih]->GetYaxis()->SetTitleOffset(1.15);
        h2[ih]->GetYaxis()->SetLabelSize(0.045);
        h2[ih]->GetYaxis()->SetTitleSize(0.05);
        h2[ih]->GetYaxis()->SetLabelOffset(0.01);
        h2[ih]->GetXaxis()->SetLabelOffset(0.01);
       // h2[ih]->GetXaxis()->SetRangeUser(plotmin,plotmax);
        h2[ih]->GetZaxis()->SetLabelSize(0.045);
        h2[ih]->GetZaxis()->SetTitleSize(0.05);
        h2[ih]->GetZaxis()->SetTitleOffset(1.15);
        if(ih==4)h2[ih]->GetZaxis()->SetTitleOffset(1.4);
        h2[ih]->GetZaxis()->SetDecimals();
    }

    h2[3]->GetZaxis()->SetRangeUser(0.6,1.4);
    h2[4]->GetZaxis()->SetRangeUser(0.99,1.01);




    Int_t MyPalette[256];
   // Double_t Length[] = {0.75,0.8,0.85,0.9,0.95,1,1,1.05,1.1,1.15,1.2,1.25};
    Double_t Length[] = {0,.1,.2,.3,.4,.5,.5,.6,.7,.8,.9,1.};

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
    pt = new TPaveText(0.15,0.75,0.8,0.95,"NB NDC");
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
 //   h2[3]->GetZaxis()->SetRangeUser(0.85,1.15);
  //  hRMS[0]->GetYaxis()->SetRangeUser(0,4);
    h2[3]->Draw("colz");
    cEff->cd(2);
    cEff->cd(2)->SetRightMargin(0.185);
    h2[4]->Draw("colz");
 //   hRMS[0]->Draw();
    gPad->Update();
 //   pt->Draw("same");
    cEff->SaveAs("UnfZ.pdf");
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
