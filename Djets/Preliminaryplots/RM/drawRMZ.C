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


void drawRMZ()
{

    style();
    gStyle->SetOptStat(000);
    gStyle->SetLegendFont(42);


   TFile *inFileFD1 = new TFile("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_z04F/ResponseFD.root","read");

    TH2D *hFD_ratio = (TH2D*)inFileFD1->Get("RM");



            hFD_ratio->SetTitle("");
            hFD_ratio->GetXaxis()->SetTitle("#it{z}_{||}^{ch,det}");
            hFD_ratio->GetXaxis()->SetLabelSize(0.0);
            hFD_ratio->GetXaxis()->SetTitleSize(0.032);
            hFD_ratio->GetXaxis()->SetTitleOffset(1.1);
            hFD_ratio->GetYaxis()->SetTitle("#it{z}_{||}^{ch,gen}");
            hFD_ratio->GetYaxis()->SetTitleOffset(1.1);
            hFD_ratio->GetYaxis()->SetLabelSize(0.0);
            hFD_ratio->GetYaxis()->SetTitleSize(0.032);
          //  hFD_ratio->GetXaxis()->SetRangeUser(plotmin,plotmax);

            TPaveText *pTitleJ1 = new TPaveText(0.2,0.9,0.8,1,"brNDC");
            pTitleJ1->SetFillStyle(0);
            pTitleJ1->SetBorderSize(0);
            pTitleJ1->SetTextFont(42);
            pTitleJ1->SetTextSize(0.032);
            pTitleJ1->SetTextAlign(21);
            pTitleJ1->AddText("#it{p}_{T,ch jet}^{det} (GeV/#it{c})");

            TString txt = "#it{p}_{T,ch jet}^{gen} (GeV/#it{c})";
            TLatex *pTitleJ2 = new TLatex(27.5,15,txt.Data());
            pTitleJ2->SetTextFont(42);
            pTitleJ2->SetTextSize(0.032);
            pTitleJ2->SetTextAlign(11);
            pTitleJ2->SetTextAngle(-450);



    TPaveText *pt = new TPaveText(0.125,0.7,0.8,0.875,"NB NDC");
    pt->SetBorderSize(0);
    pt->SetFillStyle(0);
    pt->SetTextAlign(13);
    pt->SetTextFont(42);
    pt->SetTextSize(0.02);
    //TText *text = pt->AddText("ALICE Preliminary");
    TText *text = new TText;
    text = pt->AddText("This Thesis"); //uncomment
    text = pt->AddText("pp, #sqrt{#it{s}} = 13 TeV");
    text = pt->AddText("PYTHIA6+GEANT3");
    text = pt->AddText("charged jets with b#rightarrowD^{0}, anti-#it{k}_{T}");
    text = pt->AddText(Form("#it{R} = 0.%d, |#it{#eta}_{lab}^{jet}| < 0.%d",4,5));
    //text = pt->AddText(Form ("%d < #it{p}_{T,ch jet} < %d GeV/#it{c}",2,50));

    TCanvas *cEff = new TCanvas("cEff","cEff",950,800);

    gStyle->SetOptStat(000);
    gStyle->SetLegendFont(42);
    cEff->cd()->SetLeftMargin(0.13);
    cEff->cd()->SetBottomMargin(0.15);
    cEff->cd()->SetTopMargin(.11);
    cEff->cd()->SetRightMargin(.23);
gStyle->SetPalette(57);
    cEff->SetLogz();
    hFD_ratio->Draw("colz");
    TGaxis *axisV1 = new TGaxis(0, 0, 0, 1, 0.4, 0.6, 1, "-L");axisV1->SetLabelFont(43);axisV1->SetLabelSize(17);axisV1->SetLabelOffset(0.032);axisV1->Draw();
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
    pTitleJ2->Draw("same");
    pt->Draw("same");

    TPaletteAxis *palette = (TPaletteAxis *)hFD_ratio->GetListOfFunctions()->FindObject("palette");

    palette->SetX1NDC(0.89);
    palette->SetX2NDC(0.93);
    palette->SetY1NDC(0.15);
    palette->SetY2NDC(0.9);
    gPad->Modified();
    gPad->Update();

    cEff->SaveAs("DjetRM_FD_thesis.pdf");


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
