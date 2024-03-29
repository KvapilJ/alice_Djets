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
#include "THnSparse.h"
#include "AliNormalizationCounter.h"
#include "TGraphAsymmErrors.h"
#include "style.C"

void setHistoDetails(TH1 *h, Color_t color, Style_t Mstyle, Width_t width);

 //double zmin = 0, zmax = 2.;
 //double jetmin = 2, jetmax = 50;
 double plotmin = -1.0, plotmax =1.0;


    const int ptbinsJetN = 11;
    float ptJetbins[ptbinsJetN+1] = {2,3,4,5,6,8,10,12,14,20,30,50};

    int promptColor = kRed+1;
    int nonpromptColor = kBlue+1;


void drawRes()
{

    style();
    gStyle->SetOptStat(000);

    gStyle->SetLegendFont(42);


   TFile *inFile = new TFile("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts/Default_AnalysisResults_Run2.root/FDsubtraction/JetPtSpectrum_FDsub.root","read");

   TFile *File = new TFile("/mnt/hgfs/vmware/data_R04_050219/MC/AnalysisResults_Run2w18b.root","read");
   if(!File) { std::cout << "==== WRONG FILE WITH DATA =====\n\n"; return ;}
   TDirectoryFile *dir=dynamic_cast<TDirectoryFile*>(File->Get("DmesonsForJetCorrelations"));
   TString histName = "histosD0MBN";
   TList *histList;
   THnSparseF *sparse;

   TH1D* hbin1;
   TH1D* hbin2;
   TH1D* hbin3;


   for(int i=0;i<2; i++){
       histList =  dynamic_cast<TList*>(dir->Get(Form("%s%dMCrec",histName.Data(),i)));
       sparse = dynamic_cast<THnSparseF*>(histList->FindObject("ResponseMatrix"));
       sparse->GetAxis(0)->SetRangeUser(-2,2);
       sparse->GetAxis(6)->SetRangeUser(5,6);
       sparse->GetAxis(2)->SetRangeUser(2,36);
       sparse->GetAxis(7)->SetRangeUser(2,36);
       if(i==0) {
         hbin1 = dynamic_cast<TH1D*>(sparse->Projection(10));
       }
       else {
         hbin1->Add(dynamic_cast<TH1D*>(sparse->Projection(10)));
       }

       sparse->GetAxis(6)->SetRangeUser(10,12);
       if(i==0) {
         hbin2 = dynamic_cast<TH1D*>(sparse->Projection(10));
       }
       else {
         hbin2->Add(dynamic_cast<TH1D*>(sparse->Projection(10)));
       }
       sparse->GetAxis(6)->SetRangeUser(20,30);
       if(i==0) {
         hbin3 = dynamic_cast<TH1D*>(sparse->Projection(10));
       }
       else {
         hbin3->Add(dynamic_cast<TH1D*>(sparse->Projection(10)));
       }
   }

 //   TH1D *hFD_ratio = (TH1D*)inFile->Get("hFD_ratio");





            hbin1->SetTitle("");
            hbin1->SetMarkerColor(kRed+1);
            hbin1->SetLineColor(kRed+1);
            hbin1->SetMarkerStyle(20);
            hbin1->SetMarkerSize(1.2);
            hbin1->GetXaxis()->SetTitle("#Delta#it{p}_{T}");
            hbin1->GetYaxis()->SetTitle("Probability Density");
            hbin1->GetXaxis()->SetLabelSize(0.04);
            hbin1->GetXaxis()->SetTitleSize(0.05);
            hbin1->GetXaxis()->SetTitleOffset(1.);
            hbin1->GetYaxis()->SetTitleOffset(1.1);
            hbin1->GetYaxis()->SetLabelSize(0.045);
            hbin1->GetYaxis()->SetTitleSize(0.05);
            hbin1->GetXaxis()->SetRangeUser(plotmin,plotmax);

            //hbin1->SetMaximum(hbin1->GetMaximum()*3);
            hbin1->SetMaximum(hbin1->GetMaximum()*10);
            hbin1->Scale(1./hbin1->GetEntries());
            hbin1->Scale(1,"width");


            hbin2->SetTitle("");
            hbin2->SetMarkerColor(kBlue+1);
            hbin2->SetLineColor(kBlue+1);
            hbin2->SetMarkerStyle(21);
            hbin2->SetMarkerSize(1.2);
            hbin2->GetXaxis()->SetTitle("#Delta#it{p}_{T}");
            hbin2->GetYaxis()->SetTitle("Probability Density");
            hbin2->GetXaxis()->SetLabelSize(0.04);
            hbin2->GetXaxis()->SetTitleSize(0.05);
            hbin2->GetXaxis()->SetTitleOffset(1.);
            hbin2->GetYaxis()->SetTitleOffset(1.1);
            hbin2->GetYaxis()->SetLabelSize(0.045);
            hbin2->GetYaxis()->SetTitleSize(0.05);
            hbin2->GetXaxis()->SetRangeUser(plotmin,plotmax);
            hbin2->SetMaximum(hbin1->GetMaximum()*10);
            hbin2->Scale(1./hbin2->GetEntries());
            hbin2->Scale(1,"width");


            hbin3->SetTitle("");
            hbin3->SetMarkerColor(kGreen+1);
            hbin3->SetLineColor(kGreen+1);
            hbin3->SetMarkerStyle(22);
            hbin3->SetMarkerSize(1.2);
            hbin3->GetXaxis()->SetTitle("#Delta#it{p}_{T}");
            hbin3->GetYaxis()->SetTitle("Probability Density");
            hbin3->GetXaxis()->SetLabelSize(0.04);
            hbin3->GetXaxis()->SetTitleSize(0.05);
            hbin3->GetXaxis()->SetTitleOffset(1.);
            hbin3->GetYaxis()->SetTitleOffset(1.1);
            hbin3->GetYaxis()->SetLabelSize(0.045);
            hbin3->GetYaxis()->SetTitleSize(0.05);
            hbin3->GetXaxis()->SetRangeUser(plotmin,plotmax);
            hbin3->SetMaximum(hbin1->GetMaximum()*10);
            hbin3->Scale(1./hbin3->GetEntries());
            hbin3->Scale(1,"width");;

        //    hbin1->GetYaxis()->SetRangeUser(1E-3,70);
        //    hbin2->GetYaxis()->SetRangeUser(1E-3,70);
        //    hbin3->GetYaxis()->SetRangeUser(1E-3,70);


   // TLegend *leg = new TLegend(0.15,0.55,0.5,0.70);
    TLegend *leg = new TLegend(0.58,0.55,0.92,0.75);
    //leg->SetTextSize(0.045);
    leg->SetTextFont(42);
    leg->SetTextSize(0.035);
    leg->AddEntry(hbin1,"5 < #it{p}^{gen}_{T,ch jet} < 6 GeV/#it{c}","p");
    leg->AddEntry(hbin2,"10 < #it{p}^{gen}_{T,ch jet} < 12 GeV/#it{c}","p");
    leg->AddEntry(hbin3,"20 < #it{p}^{gen}_{T,ch jet} < 30 GeV/#it{c}","p");



    TPaveText *pt = new TPaveText(0.15,0.75,0.5,0.95,"NB NDC");
    pt->SetBorderSize(0);
    pt->SetFillStyle(0);
    pt->SetTextAlign(13);
    pt->SetTextFont(42);
    pt->SetTextSize(0.035);
    //TText *text = pt->AddText("ALICE Preliminary");
    TText *text = new TText;
    text = pt->AddText("This Thesis"); //uncomment
    text = pt->AddText("PYTHIA6+GEANT3, pp, #sqrt{#it{s}} = 13 TeV");
    text = pt->AddText(Form("charged jets, anti-#it{k}_{T}, #it{R} = 0.%d, |#it{#eta}_{lab}^{jet}| < 0.%d",4,5));
    text = pt->AddText(Form ("with D^{0}, %d < #it{p}_{T,D^{0}} < %d GeV/#it{c}",2,36));

    hbin1->GetYaxis()->SetRangeUser(1E-4,800);

    TCanvas *cEff = new TCanvas("cEff","cEff",1000,800);
    cEff->SetLogy();
    hbin1->Draw();
    hbin2->Draw("same");
    hbin3->Draw("same");
    pt->Draw("same");



    leg->Draw("same");


   // cEff->SaveAs("DjetpTres.png");
    cEff->SaveAs("DjetpTres_thesis.pdf");
   // cEff->SaveAs("DjetpTres.eps");




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

    c->SaveAs(name+"_log"+".png");
    c->SaveAs(name+"_log"+".pdf");

}

/*


THnSparseF* sMC = dynamic_cast<THnSparseF*>(histList[i]->FindObject("ResponseMatrix"));
sparseMC[i] = dynamic_cast<THnSparseF*>(sMC->Clone(Form("sparseMC_%d",i)));
sparseMC[i]->GetAxis(6)->SetRangeUser(jetptmin,jetptmax); // jet pT gen
sparseMC[i]->GetAxis(9)->SetRangeUser(-(0.9-fRpar),0.9-fRpar); // MC jet eta
hMC[i] = dynamic_cast<TH1D*>(sparseMC[i]->Projection(7)); // Dpt gen
hMC[i]->SetName(Form("hMC_%d",i));

THnSparseF* sreco = dynamic_cast<THnSparseF*>(histList[i]->FindObject("ResponseMatrix"));
sparsereco[i] = dynamic_cast<THnSparseF*>(sreco->Clone(Form("sparsereco_%d",i)));
sparsereco[i]->GetAxis(6)->SetRangeUser(jetptmin,jetptmax); // jet pT gen
sparsereco[i]->GetAxis(1)->SetRangeUser(0,100); // jet pT reco
sparsereco[i]->GetAxis(4)->SetRangeUser(-(0.9-fRpar),0.9-fRpar); // reco jet eta
hreco[i] = dynamic_cast<TH1D*>(sparsereco[i]->Projection(7)); // Dpt gen
hreco[i]->SetName(Form("hreco_%d",i));


if (!i){
    hMCpt = dynamic_cast<TH1D*>(hMC[0]->Clone("hMCpt"));
    hMCpt_reco = dynamic_cast<TH1D*>(hreco[0]->Clone("hMCpt_reco"));
}
else {
    hMCpt->Add(hMC[i]);
    hMCpt_reco->Add(hreco[i]);
}
TH1D * hEff = dynamic_cast<TH1D*>(hMCpt_reco->Clone("hEff"));
hEff -> Divide(hMCpt_reco,hMCpt,1,1,"b");


*/
