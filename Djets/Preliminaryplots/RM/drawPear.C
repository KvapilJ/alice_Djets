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
#include "TROOT.h"
#include "TGraphAsymmErrors.h"
#include "style.C"
#include "TLatex.h"
#include "TGaxis.h"
#include "TPaletteAxis.h"

void setHistoDetails(TH1 *h, Color_t color, Style_t Mstyle, Width_t width);

 //double zmin = 0, zmax = 2.;
 //double jetmin = 2, jetmax = 50;
 double plotmin = 5, plotmax = 50;


    const int ptbinsJetN = 11;
    float ptJetbins[ptbinsJetN+1] = {2,3,4,5,6,7,8,10,12,16,24,36};

    int promptColor = kGreen+1;
    int nonpromptColor = kBlue+1;


void drawPear()
{

    style();
    gStyle->SetOptStat(000);
    gStyle->SetLegendFont(42);


   TFile *inFileFD1 = new TFile("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_X04/outUnfTest.root","read");

    TH2D *hFD_ratio[6];
    TH1D *hFD_ratio2[6];
    for (Int_t i=0;i<6;i++){
        hFD_ratio[i] = (TH2D*)inFileFD1->Get(Form("Pearson,k=%d",i+1));
        hFD_ratio[i]->SetTitle(Form("k=%d",i+1));
        hFD_ratio[i]->GetXaxis()->SetTitle("#it{p}_{T,ch jet}^{gen} (GeV/#it{c})");
        hFD_ratio[i]->GetXaxis()->SetLabelSize(0.04);
        hFD_ratio[i]->GetXaxis()->SetTitleSize(0.04);
        hFD_ratio[i]->GetXaxis()->SetTitleOffset(1.2);
        hFD_ratio[i]->GetYaxis()->SetTitle("#it{p}_{T,ch jet}^{gen} (GeV/#it{c})     ");
        hFD_ratio[i]->GetYaxis()->SetTitleOffset(1.2);
        hFD_ratio[i]->GetYaxis()->SetLabelSize(0.04);
        hFD_ratio[i]->GetYaxis()->SetTitleSize(0.04);
        hFD_ratio[i]->SetMaximum(1);
        hFD_ratio[i]->SetMinimum(-1);
        //hFD_ratio[i]->GetZaxis()->SetNdivisions(5);

        std::cout<<"loading "<<Form("foldmeas%d",i+1)<<std::endl;
        hFD_ratio2[i] = (TH1D*)inFileFD1->Get(Form("foldmeas%d",i+1));
        if(hFD_ratio2[i])std::cout<<"loaded "<<i<<std::endl;
        else std::cout<<"not loaded "<<i<<std::endl;
        hFD_ratio2[i]->SetTitle("");
        hFD_ratio2[i]->GetXaxis()->SetTitle("#it{p}_{T,ch jet} (GeV/#it{c})");
        hFD_ratio2[i]->GetXaxis()->SetLabelSize(0.04);
        hFD_ratio2[i]->GetXaxis()->SetTitleSize(0.04);
        hFD_ratio2[i]->GetXaxis()->SetTitleOffset(1.2);
        hFD_ratio2[i]->GetYaxis()->SetTitle("Refolded k=X / Measured");
        hFD_ratio2[i]->GetYaxis()->SetTitleOffset(1.2);
        hFD_ratio2[i]->GetYaxis()->SetLabelSize(0.04);
        hFD_ratio2[i]->GetYaxis()->SetTitleSize(0.04);
    }



std::cout<<"NOT"<<std::endl;

    TPaveText *pt = new TPaveText(0.15,0.75,0.5,0.95,"NB NDC");
    pt->SetBorderSize(0);
    pt->SetFillStyle(0);
    pt->SetTextAlign(13);
    pt->SetTextFont(42);
    pt->SetTextSize(0.026);
    //TText *text = pt->AddText("ALICE Preliminary");
    TText *text = new TText;
    text = pt->AddText("This Thesis"); //uncomment
    text = pt->AddText("pp, #sqrt{#it{s}} = 13 TeV");
    text = pt->AddText("charged jets, anti-#it{k}_{T}");
    text = pt->AddText(Form("#it{R} = 0.%d, |#it{#eta}_{lab}^{jet}| < 0.%d",4,5));
    text = pt->AddText(Form ("with D^{0}, %d < #it{p}_{T,D^{0}} < %d GeV/#it{c}",2,36));
    //text = pt->AddText(Form ("%d < #it{p}_{T,ch jet} < %d GeV/#it{c}",2,50));
std::cout<<"NOT"<<std::endl;
    TCanvas *cEff = new TCanvas("cEff","cEff",3000,1500);
    cEff->Divide(3,2);

    gStyle->SetOptStat(000);
    gStyle->SetLegendFont(42);

    TLegend *leg = new TLegend(0.7,0.45,0.9,0.65);
    //leg->SetTextSize(0.045);
    leg->SetTextFont(42);
    leg->SetTextSize(0.03);
    leg->AddEntry(hFD_ratio2[0],"k = 1","l");
    leg->AddEntry(hFD_ratio2[1],"k = 2","l");
    leg->AddEntry(hFD_ratio2[2],"k = 3","l");
    leg->AddEntry(hFD_ratio2[3],"k = 4","l");
    leg->AddEntry(hFD_ratio2[4],"k = 5","l");
    leg->AddEntry(hFD_ratio2[5],"k = 6","l");


    TCanvas *cEff2 = new TCanvas("cEff2","cEff2",1000,800);
    cEff2->cd();
    for (Int_t i=0;i<6;i++){
        if(i==0)hFD_ratio2[i]->Draw("HIST");
        else hFD_ratio2[i]->Draw("HIST same");
    }
    leg->Draw("same");
    pt->Draw("same");
    cEff2->SaveAs("iteration.pdf");

std::cout<<"NOT"<<std::endl;


//Double_t Red[] = {0.101960784, 0.301960784, 0.529411765, 0.729411765, 0.878431373, 1., 0.992156863, 0.956862745, 0.839215686, 0.698039216, 0.403921569};
//Double_t Green[] = {0.101960784, 0.301960784, 0.529411765, 0.729411765, 0.878431373, 1, 0.858823529, 0.647058824, 0.376470588, 0.094117647, 0};
//Double_t Blue[] = {0, 0.301960784, 0.529411765, 0.729411765, 0.878431373, 1, 0.780392157, 0.509803922, 0.301960784, 0.168627451, 0.121568627};

/*Double_t Red[] = {0.17647058823529413, 0.3264129181084199, 0.5019607843137256, 0.6941945405613226, 0.8470588235294119, 0.9691657054978854, 0.996078431372549, 0.989926951172626, 0.8784313725490196, 0.6979623221837754, 0.4980392156862745};
Double_t Green[] = {0.0, 0.14994232987312578, 0.45098039215686286, 0.6662821991541715, 0.8549019607843138, 0.9668589004229142, 0.8784313725490196, 0.7174163783160323, 0.5098039215686274, 0.34286812764321417, 0.23137254901960785};
Double_t Blue[] = {0.29411764705882354, 0.528642829680892, 0.6745098039215688, 0.8206074586697425, 0.9215686274509803, 0.9636293733179546, 0.7137254901960784, 0.382160707420223, 0.0784313725490196, 0.023683198769703956, 0.03137254901960784};
*/
/*
Double_t Red[] = {0.601922337562476, 0.6941945405613226, 0.7710880430603615, 0.8470588235294119, 0.9090349865436371, 0.9691657054978854, 0.9820838139177239, 0.996078431372549, 0.994079200307574, 0.989926951172626, 0.9319492502883506};
Double_t Green[] = {0.5629373317954633, 0.6662821991541715, 0.7609381007304884, 0.8549019607843138, 0.912879661668589, 0.9668589004229142, 0.9244136870434448, 0.8784313725490196, 0.7984621299500192, 0.7174163783160323, 0.6094579008073817};
Double_t Blue[] = {0.750480584390619, 0.8206074586697425, 0.8715878508266052, 0.9215686274509803, 0.9455594002306805, 0.9636293733179546, 0.8436755094194541, 0.7137254901960784, 0.5477893118031527, 0.382160707420223, 0.22422145328719723};
*/

Int_t MyPalette[256];
Double_t Length[] = {0,.1,.2,.3,.4,.5,.5,.6,.7,.8,.9,1.};
Double_t Red[] = {0.8470588235294119, 0.7243367935409459, 0.5788542868127644, 0.4275278738946559, 0.2964244521337947, 0.17647058823529413, 0.4980392156862745, 0.6579777008842752, 0.8023068050749711, 0.9230296039984621, 0.9928489042675894, 0.996078431372549};

Double_t Green[] = {0.8549019607843138, 0.7031141868512112, 0.5371011149557863, 0.32241445597846985, 0.11995386389850059, 0.0, 0.23137254901960785, 0.32056901191849285, 0.4387543252595155, 0.5928489042675893, 0.7492502883506343, 0.8784313725490196};

Double_t Blue[] = {0.9215686274509804, 0.8408304498269897, 0.7329488658208382, 0.6136101499423299, 0.48173779315647824, 0.29411764705882354, 0.03137254901960784, 0.025221068819684733, 0.054748173779315644, 0.1999231064975008, 0.4456747404844289, 0.7137254901960784};



Int_t FI = TColor::CreateGradientColorTable(12, Length, Red, Green, Blue, 256);
for (int i=0;i<256;i++) MyPalette[i] = FI+i;
gStyle->SetPalette(256, MyPalette);

   /* Int_t palette[10];
    palette[0] = kBlue+4;
    palette[1] = kBlue+3;
    palette[2] = kBlue+2;
    palette[3] = kBlue+1;
    palette[4] = kBlue;
    palette[5] = kRed;
    palette[6] = kRed+1;
    palette[7] = kRed+2;
    palette[8] = kRed+3;
    palette[9] = kRed+4;*/
    //gStyle->SetPalette(10,palette);

    TPaletteAxis *palette[6];


std::cout<<"NOT"<<std::endl;
//gStyle->SetPalette(55);
    //cEff->SetLogz();
    for (Int_t i=0;i<6;i++){
        cEff->cd(i+1);

        cEff->cd(i+1)->SetLeftMargin(0.13);
        cEff->cd(i+1)->SetBottomMargin(0.15);
        cEff->cd(i+1)->SetTopMargin(.02);
        cEff->cd(i+1)->SetRightMargin(.12);

    hFD_ratio[i]->Draw("colz");
    gPad->Update();
    std::cout<<"get palet "<<i<<std::endl;
     palette[i]= (TPaletteAxis *)hFD_ratio[i]->GetListOfFunctions()->FindObject("palette");

    palette[i]->SetX1NDC(0.89);
    palette[i]->SetX2NDC(0.93);
    palette[i]->SetY1NDC(0.15);
    palette[i]->SetY2NDC(0.97);
    gPad->Modified();
    gPad->Update();
    }
    std::cout<<"NOT"<<std::endl; /*   TGaxis *axisV1 = new TGaxis(0, 0, 0, 1, 0.4, 0.6, 1, "-L");axisV1->SetLabelFont(43);axisV1->SetLabelSize(17);axisV1->SetLabelOffset(0.032);axisV1->Draw();
    TGaxis *axisV2 = new TGaxis(0, 2, 0, 5, 0.7, 1.0, 3, "-L");axisV2->SetLabelFont(43);axisV2->SetLabelSize(17);axisV2->SetLabelOffset(0.032);axisV2->Draw();
    TGaxis *axisV3 = new TGaxis(0, 6, 0,10, 0.6, 1.0, 4, "-L");axisV3->SetLabelFont(43);axisV3->SetLabelSize(17);axisV3->SetLabelOffset(0.032);axisV3->Draw();
    TGaxis *axisV4 = new TGaxis(0,11, 0,15, 0.6, 1.0, 4, "-L");axisV4->SetLabelFont(43);axisV4->SetLabelSize(17);axisV4->SetLabelOffset(0.032);axisV4->Draw();
    TGaxis *axisV5 = new TGaxis(0,16, 0,20, 0.6, 1.0, 4, "-L");axisV5->SetLabelFont(43);axisV5->SetLabelSize(17);axisV5->SetLabelOffset(0.032);axisV5->Draw();
    TGaxis *axisV6 = new TGaxis(0,21, 0,25, 0.6, 1.0, 4, "-L");axisV6->SetLabelFont(43);axisV6->SetLabelSize(17);axisV6->SetLabelOffset(0.032);axisV6->Draw();
    TGaxis *axisV7 = new TGaxis(25, 0, 25, 5,   2,   5, 1, "+");axisV7->SetLabelFont(43);axisV7->SetLabelSize(26);axisV7->SetLabelOffset(0.04);axisV7->Draw();
    TGaxis *axisV8 = new TGaxis(25,10,25,15,   7,  10, 1, "+");axisV8->SetLabelFont(43);axisV8->SetLabelSize(26);axisV8->SetLabelOffset(0.04);axisV8->Draw();
    TGaxis *axisV9 = new TGaxis(25,20, 25,25,  15,  50, 1, "+");axisV9->SetLabelFont(43);axisV9->SetLabelSize(26);axisV9->SetLabelOffset(0.04);axisV9->Draw();

    TGaxis *axisH1 = new TGaxis( 0, 0, 1, 0, .4,.6, 1, "-+L");axisH1->SetLabelFont(43);axisH1->SetLabelSize(17);axisH1->SetLabelOffset(0.01);axisH1->Draw();
    TGaxis *axisH2 = new TGaxis( 2, 0, 5, 0, 0.7,1.0, 3, "-+L");axisH2->SetLabelFont(43);axisH2->SetLabelSize(17);axisH2->SetLabelOffset(0.01);axisH2->Draw();
    TGaxis *axisH3 = new TGaxis( 6, 0,10, 0, 0.6,1.0, 4, "-+L");axisH3->SetLabelFont(43);axisH3->SetLabelSize(17);axisH3->SetLabelOffset(0.01);axisH3->Draw();
    TGaxis *axisH4 = new TGaxis(11, 0,15, 0, 0.6,1.0, 4, "-+L");axisH4->SetLabelFont(43);axisH4->SetLabelSize(17);axisH4->SetLabelOffset(0.01);axisH4->Draw();
    TGaxis *axisH5 = new TGaxis(16, 0,20, 0, 0.6,1.0, 4, "-+L");axisH5->SetLabelFont(43);axisH5->SetLabelSize(17);axisH5->SetLabelOffset(0.01);axisH5->Draw();
    TGaxis *axisH6 = new TGaxis(21, 0,25, 0, 0.6,1.0, 4, "-+L");axisH6->SetLabelFont(43);axisH6->SetLabelSize(17);axisH6->SetLabelOffset(0.01);axisH6->Draw();
    TGaxis *axisH7 = new TGaxis( 0, 25, 5, 25,   2,  5, 1, "-");axisH7->SetLabelFont(43);axisH7->SetLabelSize(26);axisH7->SetLabelOffset(0.02); axisH7->Draw();
    TGaxis *axisH8 = new TGaxis(10, 25,15, 25,  7, 10, 1, "-");axisH8->SetLabelFont(43);axisH8->SetLabelSize(26);axisH8->SetLabelOffset(0.02); axisH8->Draw();
    TGaxis *axisH9 = new TGaxis(20, 25,25, 25,  15, 50, 1, "-");axisH9->SetLabelFont(43);axisH9->SetLabelSize(26);axisH9->SetLabelOffset(0.02); axisH9->Draw();
    pTitleJ1->Draw("same");
    pTitleJ2->Draw("same");*/
   // pt->Draw("same");
/*
    TPaletteAxis *palette = (TPaletteAxis *)hFD_ratio->GetListOfFunctions()->FindObject("palette");

    palette->SetX1NDC(0.89);
    palette->SetX2NDC(0.93);
    palette->SetY1NDC(0.15);
    palette->SetY2NDC(0.9);
    gPad->Modified();
    gPad->Update();*/

    cEff->SaveAs("Pearson.pdf");


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
