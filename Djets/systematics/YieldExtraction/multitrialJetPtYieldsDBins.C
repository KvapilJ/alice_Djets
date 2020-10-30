#include "TStyle.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TPaveText.h"
#include "TH1D.h"
#include "TROOT.h"
#include <iostream>
#include "style.C"

double scaleFactor = 1;

 int ilog=0;

void multitrialJetPtYieldsDBins(char *outDir = "JetPtComparison" )
{

    style();


    const int nTrials = 48;
    const int sigmameanvar = 6;
    const int bkgvar = 8;

    double plotmin = 0.4;
    if (!ilog) plotmin = 0.4;

//R=0.2
//jet 5-7
/*
int Rpar = 2;
    const int ptbinsJetN = 5;
    int ptbinsmin[] = {2,3,4,5,6};
    int ptbinsmax[] = {3,4,5,6,7};
*/
//jet 7-10
/*
    int Rpar = 2;
    const int ptbinsJetN = 5;
    int ptbinsmin[] = {4,5,6,7,8};
    int ptbinsmax[] = {5,6,7,8,10};
*/
//jet 10-15
/*
    int Rpar = 2;
    const int ptbinsJetN = 6;
    int ptbinsmin[] = {5,6,7,8,10,12};
    int ptbinsmax[] = {6,7,8,10,12,15};
*/
//jet 15-50
/*
    int Rpar = 2;
    const int ptbinsJetN = 4;
    int ptbinsmin[] = {10,12,16,24};
    int ptbinsmax[] = {12,16,24,36};
*/
//R = 0.4 and 0.6
//int Rpar = 4;
//int Rpar = 6;


    const int ptbinsJetN = 5;
    int ptbinsmin[] = {2,3,4,5,6};
   int ptbinsmax[] = {3,4,5,6,7};

//jet 7-10
/*
    const int ptbinsJetN = 6;
    int ptbinsmin[] = {3,4,5,6,7,8};
    int ptbinsmax[] = {4,5,6,7,8,10};
*/
//jet 10-15
/*
    const int ptbinsJetN = 6;
    int ptbinsmin[] = {5,6,7,8,10,12};
    int ptbinsmax[] = {6,7,8,10,12,15};
*/
//jet 15-50
/*
    const int ptbinsJetN = 8;
    int ptbinsmin[] = {5,6,7,8,10,12,16,24};
    int ptbinsmax[] = {6,7,8,10,12,16,24,36};
*/



    TFile *file = TFile::Open("DistributionOfFinalYields_SBApproach_Dzero_AfterDbinSum.root","read");
    TCanvas *cAll = (TCanvas*)file->Get("cDistr");
    cAll->Draw();
    cAll->SaveAs("out/JetPtTrialsDBinsAll.pdf");
    cAll->SaveAs("out/JetPtTrialsDBinsAll.png");

    cAll->SetLogy();
    cAll->SaveAs("out/JetPtTrialsDBinsAll_log.pdf");
    cAll->SaveAs("out/JetPtTrialsDBinsAll_log.png");

    TFile *fileAv = TFile::Open("AverageOfFinalYields_SBApproach_Dzero_AllDBins.root","read");
    TCanvas *cAv = (TCanvas*)fileAv->Get("cDistrAllAvgs");

   /*
    TH1F *hDist[8];
    for(int i=0; i<8; i++){
      hDist[i] = (TH1F*)cAv->FindObject(Form("JetRawYieldAvgDistr_%d",i));
      hDist[i]->SetMinimum(10);

    }
    hDist[i]->Draw();

   TH1F *hDist = (TH1F*)cAv->FindObject("JetRawYieldAvgDistr_0");
  hDist->SetMinimum(10);
   hDist->Draw();


   TH1F *hCent = (TH1F*)cAv->FindObject("JetRawYieldCentral");
*/


    cAv->Draw();

/*
    cAv->SaveAs("plots/JetPtTrialsAVDBins.pdf");
    cAv->SaveAs("plots/JetPtTrialsAVDBins.png");

    cAv->SetLogy();
    cAv->SaveAs("plots/JetPtTrialsAvDBins_log.pdf");
    cAv->SaveAs("plots/JetPtTrialsAvDBins_log.png");*/



    for(int i=0; i<ptbinsJetN; i++){

       TPaveText *pv1 = new TPaveText(0.75,0.8,0.9,0.9,"brNDC");
      pv1->SetFillStyle(0);
      pv1->SetBorderSize(0);
      pv1->AddText(Form("%d < p_{T}^{D0} < %d",ptbinsmin[i],ptbinsmax[i]));

      TFile *fileBin = TFile::Open(Form("DistributionOfFinalYields_SBApproach_Dzero_Bin%d.root",i),"read");
       TCanvas *cc = (TCanvas*)fileBin->Get(Form("cDistr%d",i));
       cc->Draw();
       //TH1D *h = (TH1D*)cc->DrawFrame(0.,0.,1.,1.);
       //h->SetXTitle("x");
       //cc->Modified();

       pv1->Draw("same");
       cc->SaveAs(Form("plots/JetPtTrialsDPt%d.pdf",i));
       cc->SaveAs(Form("plots/JetPtTrialsDPt%d.png",i));

       cc->SetLogy();
       cc->SaveAs(Form("plots/JetPtTrialsDPt%d_log.pdf",i));
       cc->SaveAs(Form("plots/JetPtTrialsDPt%d_log.png",i));
    }


   return;
/*
    for (int ibin=0; ibin<ptbinsJetN; ibin++){
        TFile *file = TFile::Open(Form("effScalemethod/Yield_Dzero_%1.1fto%1.1f.root",ptbinsJet[ibin],ptbinsJet[ibin+1]),"read");
        std::cout << "________________ Reading pT bin: " <<  ptbinsJet[ibin] << "\t" << ptbinsJet[ibin+1] << std::endl;
        TH1D *hDist = (TH1D*)file->Get(Form("fJetPtBinYield_Bin%d",ibin));

        for (int j=0; j< nTrials; j++){
           hYields[j]->SetBinContent(ibin+1,hDist->GetBinContent(j));
            //cout << "bin content: " << hDist->GetBinContent(j) << '\t';
        }
    }


    TCanvas *cc = new TCanvas("cc","cc",800,600);
    cc->SetLogy();
    hYields[0]->Draw("e");

    for(int i=1; i<48; i++){
        hYields[i]->Draw("esame");
   }


   if(ilog){
        cc->SaveAs("plots/JetPtTrialAll_log.pdf");
        cc->SaveAs("plots/JetPtTrialAll_log.png");
    }
    else{
        cc->SaveAs("plots/JetPtTrialAll.pdf");
        cc->SaveAs("plots/JetPtTrialAll.png");
    }
*/


}


 Double_t ScaleX(Double_t x)
{
  Double_t v;
  //v = 10 * x + 100; // "linear scaling" function example
  v = (x + scaleFactor*0.15)-1; // "linear scaling" function example
  return v;
}

Double_t ScaleY(Double_t y)
{
  Double_t v;
  v = 20 * y + 200; // "linear scaling" function example
  return v;
}

Double_t ScaleZ(Double_t z)
{
  Double_t v;
  v = 30 * z + 300; // "linear scaling" function example
  return v;
}

void ScaleAxis(TAxis *a, Double_t (*Scale)(Double_t))
{
  if (!a) return; // just a precaution
  if (a->GetXbins()->GetSize())
    {
      // an axis with variable bins
      // note: bins must remain in increasing order, hence the "Scale"
      // function must be strictly (monotonically) increasing
      TArrayD X(*(a->GetXbins()));
      for(Int_t i = 0; i < X.GetSize(); i++) X[i] = Scale(X[i]);
      a->Set((X.GetSize() - 1), X.GetArray()); // new Xbins
    }
  else
    {
      // an axis with fix bins
      // note: we modify Xmin and Xmax only, hence the "Scale" function
      // must be linear (and Xmax must remain greater than Xmin)
      a->Set( a->GetNbins(),
              Scale(a->GetXmin()), // new Xmin
              Scale(a->GetXmax()) ); // new Xmax
    }
  return;
}

void ScaleXaxis(TH1 *h, Double_t (*Scale)(Double_t))
{
  if (!h) return; // just a precaution
  ScaleAxis(h->GetXaxis(), Scale);
  return;
}

void ScaleYaxis(TH1 *h, Double_t (*Scale)(Double_t))
{
  if (!h) return; // just a precaution
  ScaleAxis(h->GetYaxis(), Scale);
  return;
}

void ScaleZaxis(TH1 *h, Double_t (*Scale)(Double_t))
{
  if (!h) return; // just a precaution
  ScaleAxis(h->GetZaxis(), Scale);
  return;
}
