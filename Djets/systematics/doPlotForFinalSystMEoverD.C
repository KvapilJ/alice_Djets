#include "config.h"
using namespace std;

void doPlotForFinalSystMEoverD() {
    TString meson[4] = {"Dplus","Ds","Dzero","Dstar_25"};
    TString name[4]={"AnalysisResults_962_fast.root","AnalysisResults_962_fast.root","/mnt/hgfs/vmware/data_141019/MC_trEff/AnalysisResults_TrEff_Run2.root","AnalysisResults_962_fast.root"};
//    TString name[4]={"AnalysisResults_961_cent.root","AnalysisResults_961_cent.root","AnalysisResults_961_cent.root","AnalysisResults_961_cent.root"};
//    TString name[4]={"AnalysisResults.root","AnalysisResults.root","AnalysisResults.root","AnalysisResults.root"}
    TFile *_file0[4];


   for(Int_t i=2;i<3;i++){
    _file0[i] = new TFile(Form("%s",name[i].Data()),"READ");
    }
    TDirectoryFile *dir[4];
    TList *list[4];
    TH2F *hPt[4];
    TH2F *hSyst2D[4];
    TH1F *hSyst[4];
    TH1D *hnew[4];
    TH1F *hprof[4];
    TCanvas *c = new TCanvas("c","",900,1000);
    c->Divide(2,1);
    for(int i = 2; i<3; i++) {
       cout<< " i " << i << endl; 
        dir[i] = (TDirectoryFile*)_file0[i]->Get(Form("MatchEff_%s",meson[i].Data()));
TString name2=Form("MatchEff_%s",meson[i].Data());
cout<< " name2 " << name2.Data() << endl;
        list[i] = (TList*)dir[i]->Get(Form("systUnc_%s",meson[i].Data()));
        hPt[i] = (TH2F*)list[i]->FindObject("fhPtDauVsD");
     // list[i].ls();
        hSyst2D[i] = (TH2F*)list[i]->FindObject("fhSystMatchEffD");
        hSyst[i]   = (TH1F*)hSyst2D[i]->ProfileX("_pfx",1,-1,"");
        hprof[i]   = (TH1F*)hPt[i]->ProfileX("_Ptx",1,-1,"");
        hSyst2D[i]->SetTitleSize(0.06,"t");
        c->cd(1);
        gStyle->SetTitleFontSize(0.09);
        gStyle->SetTitleYSize(0.07);
        hPt[i]->SetTitle(Form("%s",meson[i].Data()));
        hPt[i]->SetStats(0);
        hPt[i]->GetYaxis()->SetTitleSize(0.07);
        hPt[i]->GetYaxis()->SetTitleOffset(0.7);
        hPt[i]->GetXaxis()->SetLabelSize(0.05);
        hPt[i]->GetYaxis()->SetLabelSize(0.05);
        hPt[i]->GetXaxis()->SetRangeUser(0.,36.);
        hPt[i]->GetYaxis()->SetRangeUser(0.,30.);
        hPt[i]->DrawCopy("colz");
        hprof[i]->DrawCopy("same");
        c->cd(2);
        hSyst[i]->SetTitleSize(0.06,"t");
        hSyst[i]->SetTitle(Form("%s",meson[i].Data()));
        hSyst[i]->GetXaxis()->SetTitleSize(0.05);
        hSyst[i]->GetXaxis()->SetLabelSize(0.05);
        hSyst[i]->GetYaxis()->SetLabelSize(0.05);
        hSyst[i]->GetYaxis()->SetTitle("Syst. (%)");
        hSyst[i]->GetYaxis()->SetTitleSize(0.07);
        hSyst[i]->GetYaxis()->SetTitleOffset(0.7);
        hSyst[i]->GetXaxis()->SetLabelSize(0.05);
        hSyst[i]->SetStats(0);
        hSyst[i]->GetXaxis()->SetRangeUser(2.,36.);
        hSyst[i]->GetYaxis()->SetRangeUser(2.,10.);
        hSyst[i]->DrawCopy();

        Double_t xbins[27] = {2.,3.,4.,5.,6.,7.,8.,10.,12.,16.,24.,36.};
        hnew[i] = (TH1D*)hSyst[i]->Rebin(12,"hnew",xbins);
        hnew[i]->SetTitle(Form("%s",meson[i].Data()));
        hnew[i]->GetXaxis()->SetRangeUser(2.,36.);
        hnew[i]->GetYaxis()->SetRangeUser(2.,9.);
        hnew[i]->SetLineColor(kRed);
        hnew[i]->DrawCopy("same");
        printf("%s meson:\n",meson[i].Data());
        for(int ii = 1; ii <= hnew[i]->GetNbinsX()-1; ii++) {
            printf("pt %.1f - %.1f, err %.2f\n",hnew[i]->GetBinLowEdge(ii),hnew[i]->GetBinLowEdge(ii+1),hnew[i]->GetBinContent(ii));
        }
        printf("*********************\n");
    }
}
