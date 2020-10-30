#ifndef OLOADER_H
#define OLOADER_H
#include <iostream>
#include "TString.h"
#include "TH1.h"
#include "TLegend.h"
#include "TFile.h"
#include "TLine.h"
#include "THStack.h"
#include "TMath.h"
#include "TSystem.h"
#include "TCanvas.h"
#include "TGaxis.h"
#include "TROOT.h"
#include <fstream>

//#define NAME_OF( v ) #v


class SysRatio {

public:
    enum Method{
        kStandart,
        kMax,
        kError
    };
    SysRatio(UInt_t nFiles, TString name);
    ~SysRatio(){}
    void SetDefaultFile(TString filename);
    void SetVarFilelist(TString *filelist);
    void SetVarLegendDesc(TString *legendlist);
    void SetPath(TString path){path_ = path;}
    void SetHistoName(TString hname){histoName_ = hname;}
    void SetDefHistoName(TString hname){histoDefName_ = hname;}
    void SetOutPath(TString filename){outPath_ = filename;}
    void SetOutFolder(TString filename){outFolder_ = filename;}
    void SetTitle(TString filename){name_ = filename;}
    void SetDefaultLegend(TString filename){legendDefDesc_ = filename;}
    void SetMethod(Method meth){method_ = meth;}
    void SetAxisTitle(TString titlex, TString titley){titleX_ = titlex; titleY_ = titley;}
    void SetRatioRangesY(Double_t ratioMin, Double_t ratioMax){ratioMin_ = ratioMin; ratioMax_ = ratioMax; ratioRanges_ = true;}
    void GetRMS();
    void GetMean();
    void Write();
    void Run();
    void Draw();
    void Load();
    void ResetRun();
    void PushMain(TH1D* h,TString leg){h->SetLineColor(colors_[7]);
                                       h->SetMarkerColor(colors_[7]);
                                       h->SetMarkerStyle(markers_[7]);TH1D_.push_back(dynamic_cast<TH1D*>(h));leg_->AddEntry(h,leg);hStackMain_->Add(h,"nostack");}
    void PushRatio(TH1D* h,TString leg){h->SetLineColor(colors_[7]);
                                        h->SetMarkerColor(colors_[7]);
                                        h->SetMarkerStyle(markers_[7]);hRatio_.push_back(dynamic_cast<TH1D*>(h));legRatio_->AddEntry(h,leg);hStackRatio_->Add(h,"nostack");}
    void SetNfiles(UInt_t n){nFiles_=n;}
    void SetStatUp(TH1D *hstat){hstatUp_ = hstat;}
    void SetStatDown(TH1D *hstat){hstatDown_ = hstat;}

private:
    //global
    UInt_t nFiles_ = 0;
    TString path_ = "";
    TString histoName_= "";
    TString histoDefName_ = "";
    TString name_ = "";
    TString outPath_ = "";
    TString outFolder_ = "";
    Color_t colors_[12] = {2,kViolet+5,kRed+1,kGreen+3,kMagenta+2,4,6,kCyan+1,8,kOrange-1,kGray+1,kRed+2};
    Style_t markers_[14] = {20,21,22,23,24,25,26,27,28,29,30,32,33,34};
    Style_t linestyle_[15] = {1,2,3,4,5,6,7,8,9,10,2,3,4,5,15};
    //default plot
    TFile *TFileDefault_{nullptr};
    TH1D *TH1DDefault_{nullptr};
    TString legendDefDesc_= "DEFAULT";
    //variations
    std::vector<TFile*> TFile_;
    std::vector<TH1D*> TH1D_;
    std::vector<TString> legendVarDesc_;
    //ratios to default
    std::vector<TH1D*> hRatio_;
    //
    TH1D *hRMS_{nullptr};
    TH1D *hMean_{nullptr};
    //legends
    TLegend *leg_{nullptr};
    TLegend *legRatio_{nullptr};
    //plotting
    THStack *hStackMain_{nullptr};
    THStack *hStackRatio_{nullptr};
    TLine *line_{nullptr};

    TCanvas *cStackMain_{nullptr};
    TCanvas *cStack_{nullptr};
    TCanvas *cRMS_{nullptr};
    TCanvas *cMean_{nullptr};

    Bool_t taskInhibit{false};
    Method method_{kStandart};
    TH1D *hstatUp_{nullptr};
    TH1D *hstatDown_{nullptr};

    TString titleX_ = "";
    TString titleY_ = "";

    Double_t ratioMax_ = 0;
    Double_t ratioMin_ = 0;

    Bool_t ratioRanges_ = false;









};
#endif
