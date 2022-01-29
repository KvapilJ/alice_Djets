//
// Macro to subtract B feed-down from inclusive jet pT spectrum before unfolding
//
// Author: B.Trzeciak (barbara.antonina.trzeciak@cern.ch)
//

#include "config.h"

double plotmin, plotmax; //declared later
Int_t maxObsBinN;
TString fefffile;
Bool_t fuseEff;
TH2D* fTrueSpectrumKineNum;
TH2D* fTrueSpectrumKineDen;
TH2D* fMeasSpectrumKineNum;
TH2D* fMeasSpectrumKineDen;
TH2D *resSlices[fzptJetMeasN][fzptJetMeasN];




TH2D* GetInputHist2D(TString fn, TString sname);
TH1* GetInputHist(TString inFile = "JetPtSpectra_effScale.root", TString histName = "hjetpt",TH1 *hh = nullptr);
TH2D* GetInputSimHist(TString inFolder, TString sname);
RooUnfoldResponse* LoadDetectorMatrix(TString MCfile, TString out, Bool_t ispostfix = false, TString postfix = "");


void ScaleHist(TH1 *hh, int full = 0);
void setHistoDetails(TH1 *hh, Color_t color, Style_t Mstyle, double Msize = 1.1, Width_t Lwidth = 2, Style_t Lstyle = 1);
void SaveCanvas(TCanvas *c, TString name = "tmp");

void subtractB_afterFolding(TH2D *hFD_central_binned, TH2D *hFD_central_binned_fold,TH2D *hFD_up,TH2D *hFD_down,TH2D *hData_binned,TH2D *kineeff1,TH2D *kineeff2,TString outPlotDir,TString outSpectraDir, bool isSys);
TH1* GetUpSys(TH1D **hFD, const int nFiles = 11, TH1D *hFD_up = nullptr);
TH1* GetDownSys(TH1D **hFD, const int nFiles = 11, TH1D *hFD_down = nullptr);
TH2* foldB(RooUnfoldResponse *response, TH2D *hFD, TH2D *folded );
void SparseToTree(TString h, Bool_t isPostfix, TString postfix);


void subtractFD_2D(
TString roounfoldpwd = "",
TString dataFile = "JetPtSpectra_SB_FASTwoSDD_eff_ptD3.root",
TString dataAnalysisFile = "",
TString nonPromptEffDir = "",
TString simDir = "../simulations/POWHEG/out/",
TString MCfile = "",
TString outSpectraDir = "out_FDPythia",
//Int_t zBin=0,
Bool_t isSys = 1,
Bool_t effScale = true,
Bool_t isPostfix = false,
TString postfix = ""
)
{

std::cout<<"A"<<std::endl;
    gSystem->Load(Form("%s",roounfoldpwd.Data()));
    fefffile = nonPromptEffDir;
    fuseEff = effScale;

    gStyle->SetOptStat(0000);

    TString outPlotDir = outSpectraDir;
    outPlotDir+="/plots2D";
    gSystem->Exec(Form("mkdir %s",outSpectraDir.Data()));
    gSystem->Exec(Form("mkdir %s",outPlotDir.Data()));
std::cout<<"A"<<std::endl;
    TFile *File = new TFile(dataAnalysisFile,"read");
    TDirectoryFile* dir = nullptr;
    TList *histList= nullptr;
    double nEv;
    dir = dynamic_cast<TDirectoryFile*>(File->Get("PWG3_D2H_DmesonsForJetCorrelationsMBN0"));
    if(!dir) std::cout<<"no dir"<<std::endl;
    AliNormalizationCounter *c = dynamic_cast<AliNormalizationCounter*>(dir->Get("NormalizationCounter"));
    nEv = c->GetNEventsForNorm();


std::cout<<"A"<<std::endl;
    double dataLum = nEv/(sigma_in*1000) ;//Luminosity in mbar
    double simScaling;
    if(fSystem){
      if(fDmesonSpecie) simScaling = BRDstar*APb*dataLum;
      else simScaling = BRDzero*APb*dataLum;
    }
    else {
      if(fDmesonSpecie) simScaling = BRDstar*dataLum;
      else simScaling = BRDzero*dataLum;
    }
std::cout<<"A"<<std::endl;
    // ----------------- data eff corrected D-jet pT spectrum ---------------------
    TH2D* hData_binned = GetInputHist2D(dataFile, "hjetptspectrumReb");
std::cout<<"A"<<std::endl;
    // ----------------- B->D simulation ---------------------
    int cent = 0;
    bool jet = 1; // this code is for jet spectra subtraction

    int simNr = 0; // 0 - central value
    int nFiles = fBsimN;
    TH1D *hFD[fzptJetMeasN][fBsimN];
    TH1D *hFD_binned[fzptJetMeasN][fBsimN];

    for (Int_t bin = 1; bin <= fzptJetMeasN; bin++ ){
        for (int nr=simNr; nr<fBsimN; nr++){
            TString file = simDir;
            file += Form("%d",bin);
            file += "/JetPt_";
            file += fRunB[nr];
            file += "_Dpt"; file += fzptbinsDA[bin-1][0]; file += "_"; file += fzptbinsDA[bin-1][fzptbinsDN[bin-1]];
            file += "_Jetpt"; file += fzptJetMeasA[bin-1]; file += "_"; file += fzptJetMeasA[bin];
            file += "_effScaled";
            if(fDmesonSpecie) file += "_Dstar";
            else file += "_Dzero";
            file += ".root";
            TH1D *lhtmp = nullptr;

            lhtmp = dynamic_cast<TH1D*>(GetInputHist(file, "hz", lhtmp));
            lhtmp->GetYaxis()->SetTitle("d#sigma/dz (mb)");
            hFD[bin-1][nr] = dynamic_cast<TH1D*>(lhtmp->Clone(Form("hFD_%d%d",nr,bin)));
           // for (Int_t z = 1; z <= 6;z++){
           // std::cout<<"pre 1D nr "<<nr<<" bin "<<bin<<" 1D bin "<<z<< " value "<<hFD[bin-1][0]->GetBinContent(z)<<std::endl;
          //  }
            hFD_binned[bin-1][nr] = dynamic_cast<TH1D*>(lhtmp->Rebin(fzbinsJetMeasN[bin-1],Form("hFD_binned_%d%d",nr,bin),fzbinsJetMeasA[bin-1]));
          //  for (Int_t z = 1; z <= hFD_binned[0][0]->GetNbinsX();z++){
          //  std::cout<<"post 1D nr "<<nr<<" bin "<<bin<<" 1D bin "<<z<< " value "<<hFD_binned[bin-1][0]->GetBinContent(z)<<std::endl;
          //  }
        }
    }
std::cout<<"A"<<std::endl;
    TH2D* htmp =new TH2D("htmp","htmp",fzbinsJetMeasN[fzptJetMeasN-1],fzbinsJetMeasA[fzptJetMeasN-1],fzptJetMeasN,fzptJetMeasA);
    for (Int_t pt = 0; pt < fzptJetMeasN;pt++){
        for (Int_t z = 1; z <= hFD_binned[0][0]->GetNbinsX();z++){
           // std::cout<<"2D bin "<<z<<" pt "<<pt<<" 1D bin "<<z<< " value "<<hFD_binned[pt][0]->GetBinContent(z)<<std::endl;
            htmp->SetBinContent(z,pt+1,hFD_binned[pt][0]->GetBinContent(z));
            htmp->SetBinError(z,pt+1,hFD_binned[pt][0]->GetBinError(z));
        }
    }
std::cout<<"A"<<std::endl;
    TH2D *hFD_central_binned = dynamic_cast<TH2D*>(htmp->Clone("hFD_central_binned"));

    setHistoDetails(hFD_central_binned,4,24);

    hFD_central_binned->Scale(simScaling);



    TH1D *hFD_up_binned[fzptJetMeasN];
    TH1D *hFD_down_binned[fzptJetMeasN];
    TH1D *hFD_up[fzptJetMeasN];
    TH1D *hFD_down[fzptJetMeasN];
    TH2D *hFD_up_binned2D;
    TH2D *hFD_down_binned2D;
    if(isSys){
        for (Int_t pt = 0; pt < fzptJetMeasN;pt++){

            // ----------------- B->D syst. (rebinned)---------------------
            // get up unc

            hFD_up_binned[pt] = dynamic_cast<TH1D*>(hFD_binned[pt][0]->Clone(Form("hFD_up_binned%d",pt)));
            hFD_up_binned[pt] = dynamic_cast<TH1D*>(GetUpSys(hFD_binned[pt],fBsimN,hFD_up_binned[pt]));
            setHistoDetails(hFD_up_binned[pt],4,24,0,2,2);
            hFD_up_binned[pt]->Scale(simScaling);

            // get down unc
            hFD_down_binned[pt] = dynamic_cast<TH1D*>(hFD_binned[pt][0]->Clone(Form("hFD_down_binned%d",pt)));
            hFD_down_binned[pt] = dynamic_cast<TH1D*>(GetDownSys(hFD_binned[pt],fBsimN,hFD_down_binned[pt]));
            setHistoDetails(hFD_down_binned[pt],4,24,0,2,2);
            hFD_down_binned[pt]->Scale(simScaling);

        }

        TH2D* htmp2 =new TH2D("htmp2","htmp2",fzbinsJetMeasN[fzptJetMeasN-1],fzbinsJetMeasA[fzptJetMeasN-1],fzptJetMeasN,fzptJetMeasA);
        for (Int_t pt = 0; pt < fzptJetMeasN;pt++){
            for (Int_t z = 1; z <= hFD_up_binned[0]->GetNbinsX();z++){
                htmp2->SetBinContent(z,pt+1,hFD_up_binned[pt]->GetBinContent(z));
                htmp2->SetBinError(z,pt+1,hFD_up_binned[pt]->GetBinError(z));
            }
        }

        hFD_up_binned2D = dynamic_cast<TH2D*>(htmp2->Clone("hFD_up_binned2D"));

        TH2D* htmp3 =new TH2D("htmp3","htmp3",fzbinsJetMeasN[fzptJetMeasN-1],fzbinsJetMeasA[fzptJetMeasN-1],fzptJetMeasN,fzptJetMeasA);
        for (Int_t pt = 0; pt < fzptJetMeasN;pt++){
            for (Int_t z = 1; z <= hFD_down_binned[0]->GetNbinsX();z++){
                htmp3->SetBinContent(z,pt+1,hFD_down_binned[pt]->GetBinContent(z));
                htmp3->SetBinError(z,pt+1,hFD_down_binned[pt]->GetBinError(z));
            }
        }

        hFD_down_binned2D = dynamic_cast<TH2D*>(htmp3->Clone("hFD_down_binned2D"));
    }


    RooUnfoldResponse *response = LoadDetectorMatrix(MCfile,outPlotDir,isPostfix,postfix);
    //missing MC
    TH2D* hKineRatioMC = dynamic_cast<TH2D*>(fMeasSpectrumKineNum->Clone("hKineRatioMC"));
    hKineRatioMC->Sumw2("B");
    hKineRatioMC->Divide(hKineRatioMC,fMeasSpectrumKineDen,1,1,"B");
    hKineRatioMC->SetTitle("Kinematic efficiency on missing MC");

    //missinf RECO
    TH2D* hKineRatioRec = dynamic_cast<TH2D*>(fTrueSpectrumKineNum->Clone("hKineRatioRec"));
    hKineRatioRec->Sumw2("B");
    hKineRatioRec->Divide(hKineRatioRec,fTrueSpectrumKineDen,1,1,"B");
    hKineRatioRec->SetTitle("Kinematic efficiency on missing reco");


hFD_central_binned->Multiply(hKineRatioRec);
if(isSys)hFD_up_binned2D->Multiply(hKineRatioRec);
if(isSys)hFD_down_binned2D->Multiply(hKineRatioRec);

std::cout<<"B"<<std::endl;
    TH2D *hFD_central_binned_fold = dynamic_cast<TH2D*>(foldB(response, hFD_central_binned,hFD_central_binned_fold));

    // TH2D *hFD_central_binned_fold = hFD_central_binned;
    TH2D *hFD_up_binned_fold = nullptr;
    TH2D *hFD_down_binned_fold = nullptr;
    if(isSys)hFD_up_binned_fold = dynamic_cast<TH2D*>(foldB(response, hFD_up_binned2D,hFD_up_binned_fold));
    if(isSys)hFD_down_binned_fold = dynamic_cast<TH2D*>(foldB(response, hFD_down_binned2D,hFD_down_binned_fold));
/*
    for(Int_t ii = 1; ii <= hFD_central_binned_fold->GetNbinsX();ii++){
        std::cout<<"bef: "<<hFD_central_binned_fold->GetBinContent(ii,3)<<" "<<hKineRatioMC->GetBinContent(ii,3)<<" "<<hFD_central_binned_fold->GetBinContent(ii,3)/hKineRatioMC->GetBinContent(ii,3)<<std::endl;
    }
*/

hFD_central_binned_fold->Divide(hKineRatioMC);
if(isSys)hFD_up_binned_fold->Divide(hKineRatioMC);
if(isSys)hFD_down_binned_fold->Divide(hKineRatioMC);
/*
for(Int_t ii = 1; ii <= hFD_central_binned_fold->GetNbinsX();ii++){
    std::cout<<"pos: "<<hFD_central_binned_fold->GetBinContent(ii,3)<<std::endl;
}*/
std::cout<<"B"<<std::endl;
         /////////////////////// fold B->D spectra ///////////////////////////////////
        subtractB_afterFolding(hFD_central_binned, hFD_central_binned_fold,hFD_up_binned_fold,hFD_down_binned_fold,hData_binned,hKineRatioMC,hKineRatioRec,outPlotDir,outSpectraDir, isSys);
        //else subtractB_afterFolding(bkgRM,detRM,hFD_central,hFD_up,hFD_down,hData,outPlotDir,outSpectraDir,isSys);


    return;
}

void subtractB_afterFolding(TH2D *hFD_central_binned,TH2D *hFD_central_binned_fold,TH2D *hFD_up_fold,TH2D *hFD_down_fold,TH2D *hData_binned,TH2D *kineeff1,TH2D *kineeff2,TString outPlotDir,TString outSpectraDir, bool isSys){
    gStyle->SetOptStat(0000);
std::cout<<"C"<<std::endl;
    hData_binned->SetMinimum(1);
    hData_binned->SetMaximum(hData_binned->GetMaximum()*2);



    TH2D *hData_binned_sub = dynamic_cast<TH2D*>(hData_binned->Clone("hData_binned_sub"));
    hData_binned_sub->Add(hFD_central_binned_fold ,-1);
    //hData_binned_sub->Add(hFD_central_binned ,-1);hh
    hData_binned_sub->SetMarkerStyle(20);

    TH2D *hData_binned_sub_up = nullptr;
    TH2D *hData_binned_sub_down = nullptr;
    TH2D *hFD_ratio_up = nullptr;
    TH2D *hFD_ratio_down = nullptr;
if(isSys){
    hData_binned_sub_up = dynamic_cast<TH2D*>(hData_binned->Clone("hData_binned_sub_up"));
    hData_binned_sub_up->Add(hFD_down_fold ,-1);
    setHistoDetails(hFD_down_fold,4,20,0,2,2);
    setHistoDetails(hData_binned_sub_up,2,20,0,2,2);

    hData_binned_sub_down = dynamic_cast<TH2D*>(hData_binned->Clone("hData_binned_sub_down"));
    hFD_up_fold->SetName("hFD_up_fold");
    hData_binned_sub_down->Add(hFD_up_fold ,-1);
    setHistoDetails(hFD_up_fold,4,20,0,2,2);
    setHistoDetails(hData_binned_sub_down,2,20,0,2,2);
}
std::cout<<"C"<<std::endl;
    //-------------- data to sim ratio - B feed-down fraction
    TH2D *hFD_ratio = dynamic_cast<TH2D*>(hFD_central_binned_fold->Clone("hFD_ratio"));
    hFD_ratio->Divide(hData_binned);
    hFD_ratio->GetYaxis()->SetTitle("FD raw sim/data");
    hFD_ratio->SetMinimum(0);
    setHistoDetails(hFD_ratio,8,20);


    TH2D *hFDUnc  = nullptr;
if(isSys){
    hFD_ratio_up = dynamic_cast<TH2D*>(hFD_up_fold->Clone("hFD_ratio_up"));

    hFD_ratio_up->Divide(hData_binned);
    setHistoDetails(hFD_ratio_up,8,24,0,2,2);
    for(int i=1; i<=hFD_ratio_up->GetNbinsX(); i++ ){
        for(int j=1; j<=hFD_ratio_up->GetNbinsY(); j++ ){
            hFD_ratio_up->SetBinError(i,j,0);
        }
    }

    hFD_ratio_down = dynamic_cast<TH2D*>(hFD_down_fold->Clone("hFD_ratio_down"));
    hFD_ratio_down->Divide(hData_binned);
    setHistoDetails(hFD_ratio_down,8,24,0,2,2);
    for(int i=1; i<=hFD_ratio_up->GetNbinsX(); i++ ){
        for(int j=1; j<=hFD_ratio_up->GetNbinsX(); j++ ){
            hFD_ratio_down->SetBinError(i,j,0);
        }
    }


    //-------------- FD uncertanties
    hFDUnc = dynamic_cast<TH2D*>(hData_binned_sub->Clone("hDFUnc"));
    setHistoDetails(hFDUnc,kMagenta+2,20);
    hFDUnc->GetYaxis()->SetTitle("FD sys. unc");

    for(int i=1; i<=hData_binned_sub_up->GetNbinsX(); i++ ){
        for(int j=1; j<=hData_binned_sub_up->GetNbinsY(); j++ ){
            double unc1 = hData_binned_sub_up->GetBinContent(i,j) - hData_binned_sub->GetBinContent(i,j);
            double unc2 = hData_binned_sub->GetBinContent(i,j) - hData_binned_sub_down->GetBinContent(i,j);
            double unc = 0;
            if(unc1>unc2) unc = unc1;
            else unc = unc2;
            if(hData_binned_sub->GetBinContent(i,j)>1E-20) unc /= hData_binned_sub->GetBinContent(i,j);
            hFDUnc->SetBinContent(i,j,unc);
            hFDUnc->SetBinError(i,j,0);
        }

            //cout << "unc: " << unc << endl;
    }
}
std::cout<<"C"<<std::endl;

    //___________________________________drawing and saving 2D
    TFile *ofile = new TFile(Form("%s/JetPtSpectrum_FDsub2D.root",outSpectraDir.Data()),"RECREATE");
    hData_binned->Write();
    hFD_central_binned->Write();
    hFD_central_binned_fold->Write();
    hFD_ratio->Write();

    hData_binned_sub->Write();
    if(isSys){
    hData_binned_sub_up->Write();
    hData_binned_sub_down->Write();
    hFD_up_fold->Write();
    hFD_down_fold->Write();
    hFD_ratio_up->Write();
    hFD_ratio_down->Write();
}
    ofile->Close();
std::cout<<"C"<<std::endl;
    for (Int_t bin = 1; bin <= fzptJetMeasN; bin++){
        TFile *ofile = new TFile(Form("%s/JetPtSpectrum_FDsub%d.root",outSpectraDir.Data(),bin),"RECREATE");
        hData_binned->ProjectionX("hData_binned",bin,bin)->Write();
        hFD_central_binned->ProjectionX("hFD_central_binned",bin,bin)->Write();
        hFD_central_binned_fold->ProjectionX("hFD_central_binned_fold",bin,bin)->Write();
        hFD_ratio->ProjectionX("hFD_ratio",bin,bin)->Write();

        hData_binned_sub->ProjectionX("hData_binned_sub",bin,bin)->Write();
        if(isSys){
        hData_binned_sub_up->ProjectionX("hData_binned_sub_up",bin,bin)->Write();
        hData_binned_sub_down->ProjectionX("hData_binned_sub_down",bin,bin)->Write();
        hFD_up_fold->ProjectionX("hFD_up_fold",bin,bin)->Write();
        hFD_down_fold->ProjectionX("hFD_down_fold",bin,bin)->Write();
        hFD_ratio_up->ProjectionX("hFD_ratio_up",bin,bin)->Write();
        hFD_ratio_down->ProjectionX("hFD_ratio_down",bin,bin)->Write();
        }
        kineeff1->ProjectionX("kineeff1",bin,bin)->Write();
        kineeff2->ProjectionX("kineeff2",bin,bin)->Write();
        ofile->Close();

    }

std::cout<<"C"<<std::endl;


     // ------------------ draw sim and data output ---------------

     TPaveText *pvEn= new TPaveText(0.2,0.80,0.8,0.85,"brNDC");
     pvEn->SetFillStyle(0);
     pvEn->SetBorderSize(0);
     pvEn->SetTextFont(42);
     pvEn->SetTextSize(0.045f);
     pvEn->SetTextAlign(11);
     pvEn->AddText(Form("%s",fSystemS.Data()));

     double shift = 0.09;
     double dshift = 0.06;
     Double_t zshiftx = 0;
     Double_t zshifty = 0;
     Double_t letsConfuseEveryoneWithTheseShifts = 0;
     if(fObservable==Observable::kFragmentation){ zshiftx=-0.37;
         zshifty=0.2;
         letsConfuseEveryoneWithTheseShifts=-0.1;
     }
     TPaveText *pvJet = new TPaveText(0.5+zshiftx,0.65-shift+zshifty,0.9+zshiftx,0.7-shift+zshifty,"brNDC");
     pvJet->SetFillStyle(0);
     pvJet->SetBorderSize(0);
     pvJet->SetTextFont(42);
     pvJet->SetTextSize(0.04f);
     pvJet->SetTextAlign(11);
     pvJet->AddText(Form("Charged Jets, Anti-#it{k}_{T}, #it{R} = 0.%d",Rpar));

     shift+=dshift;
     TPaveText *pvD = new TPaveText(0.5+zshiftx,0.65-shift+zshifty,0.9+zshiftx,0.7-shift+zshifty,"brNDC");
     pvD->SetFillStyle(0);
     pvD->SetBorderSize(0);
     pvD->SetTextFont(42);
     pvD->SetTextSize(0.04f);
     pvD->SetTextAlign(11);
     if(fDmesonSpecie) pvD->AddText("with D^{*+} #rightarrow D^{0}#pi^{+} and charge conj.");
     else pvD->AddText("with D^{0} #rightarrow K^{-}#pi^{+} and charge conj.");
std::cout<<"C"<<std::endl;
     shift+=dshift;
     TPaveText *pvEta = new TPaveText(0.5+zshiftx,0.65-shift+zshifty,0.9+zshiftx,0.7-shift+zshifty,"brNDC");
     pvEta->SetFillStyle(0);
     pvEta->SetBorderSize(0);
     pvEta->SetTextFont(42);
     pvEta->SetTextSize(0.04f);
     pvEta->SetTextAlign(11);
     pvEta->AddText(Form("|#it{#eta}_{jet}| < 0.%d",9-Rpar));

     shift+=dshift;
     TPaveText *pv3 = nullptr;

             std::cout<<"C"<<std::endl;

TH1D *hData_binned1D[fzptJetMeasN];
TH1D *hData_binned_sub1D[fzptJetMeasN];
TH1D *hFD_central_binned_fold1D[fzptJetMeasN];
TH1D *hFD_up_fold1D[fzptJetMeasN];
TH1D *hFD_down_fold1D[fzptJetMeasN];
TH1D *hData_binned_sub_up1D[fzptJetMeasN];
TH1D *hData_binned_sub_down1D[fzptJetMeasN];
TCanvas *cSpectra[fzptJetMeasN];
TLegend *leg1[fzptJetMeasN];



for (Int_t bin = 1; bin <= fzptJetMeasN; bin++){
    hData_binned1D[bin-1] = hData_binned->ProjectionX("hData_binned",bin,bin);
    hData_binned_sub1D[bin-1]= hData_binned_sub->ProjectionX("hData_binned_sub",bin,bin);
    hFD_central_binned_fold1D[bin-1]= hFD_central_binned_fold->ProjectionX("hFD_central_binned_fold",bin,bin);
    if(isSys){
        hFD_up_fold1D[bin-1]= hFD_up_fold->ProjectionX("hFD_up_fold",bin,bin);
        hFD_down_fold1D[bin-1]= hFD_down_fold->ProjectionX("hFD_down_fold",bin,bin);
        hData_binned_sub_up1D[bin-1]= hData_binned_sub_up->ProjectionX("hData_binned_sub_up",bin,bin);;
        hData_binned_sub_down1D[bin-1]= hData_binned_sub_down->ProjectionX("hData_binned_sub_down",bin,bin);
    }

     setHistoDetails(hData_binned1D[bin-1],kGreen+2,21,1.2,2,1);
     setHistoDetails(hFD_central_binned_fold1D[bin-1],4,24);
     setHistoDetails(hData_binned_sub1D[bin-1],2,20);
     hData_binned1D[bin-1]->Scale(1,"width");
     hData_binned_sub1D[bin-1]->Scale(1,"width");
     hFD_central_binned_fold1D[bin-1]->Scale(1,"width");
     if(isSys){
       hFD_up_fold1D[bin-1]->Scale(1,"width");
       hFD_down_fold1D[bin-1]->Scale(1,"width");
       //hFD_up_fold1D[bin-1]->Scale(1,"width");
       //hFD_down_fold1D[bin-1]->Scale(1,"width");
       hData_binned_sub_up1D[bin-1]->Scale(1,"width");
       hData_binned_sub_down1D[bin-1]->Scale(1,"width");
    }
     // compare data and central sim with unc
     cSpectra[bin-1] = new TCanvas(Form("cSpectra%d",bin),Form("cSpectra%d",bin),1000,800);
     cSpectra[bin-1]->SetLogy();
     cSpectra[bin-1]->cd();
     hData_binned1D[bin-1]->GetXaxis()->SetRangeUser(plotmin,plotmax);
     hData_binned1D[bin-1]->Draw();
     hFD_central_binned_fold1D[bin-1]->Draw("same hist");
     hData_binned_sub1D[bin-1]->Draw("same");
     hData_binned1D[bin-1]->GetYaxis()->SetRangeUser(1E2,1E9);

    if(isSys){
         hFD_up_fold1D[bin-1]->Draw("same");
         hFD_down_fold1D[bin-1]->Draw("same");
         hData_binned_sub_up1D[bin-1]->Draw("same");
         hData_binned_sub_down1D[bin-1]->Draw("same");
    }

    pv3 = new TPaveText(0.5+zshiftx,0.65-shift+zshifty+letsConfuseEveryoneWithTheseShifts,0.9+zshiftx,0.7-shift+zshifty,"brNDC");
     pv3->SetFillStyle(0);
     pv3->SetBorderSize(0);
     pv3->SetTextFont(42);
     pv3->SetTextSize(0.04f);
     pv3->SetTextAlign(11);
     pv3->AddText(Form("%d < p_{T,%s} < %d GeV/#it{c}",static_cast<Int_t>(fzptbinsDA[bin-1][0]),fDmesonS.Data(),static_cast<Int_t>(fzptbinsDA[bin-1][fzptbinsDN[bin-1]])));
     pv3->AddText(Form("%d < p_{T,ch. jet} < %d GeV/#it{c}",static_cast<Int_t>(fzptJetMeasA[bin-1]),static_cast<Int_t>(fzptJetMeasA[bin])));


    pv3->Draw("same");
    pvEn->Draw("same");
    pvD->Draw("same");
    pvJet->Draw("same");
    pvEta->Draw("same");

    leg1[bin-1] = new TLegend(0.55,0.65,0.95,0.89);
    leg1[bin-1]->SetBorderSize(0);
    leg1[bin-1]->AddEntry(hData_binned1D[bin-1],"Uncorrected D-Jet yield","p");
    leg1[bin-1]->AddEntry(hFD_central_binned_fold1D[bin-1],"B Feed-Down (POWHEG+PYTHIA)","p");
    leg1[bin-1]->AddEntry(hData_binned_sub1D[bin-1],"FD corrected yield","p");
    leg1[bin-1]->Draw("same");

    cSpectra[bin-1]->SaveAs(Form("%s/JetPtSpectra_FDsub%d.png",outPlotDir.Data(),bin));
    cSpectra[bin-1]->SaveAs(Form("%s/JetPtSpectra_FDsub%d.pdf",outPlotDir.Data(),bin));
}


    TH2D*JetPtSpectra_FDsubUn = dynamic_cast<TH2D*>(hData_binned_sub->Clone("JetPtSpectra_FDsubUn"));
    JetPtSpectra_FDsubUn->GetZaxis()->SetTitle("Rel. unc.");
    JetPtSpectra_FDsubUn->SetLineColor(kGreen+1);
    JetPtSpectra_FDsubUn->SetMarkerColor(kGreen+1);
std::cout<<"C"<<std::endl;
    for(int i=1; i<=hData_binned_sub->GetNbinsX();i++){
         for(int j=1; j<=hData_binned_sub->GetNbinsY();j++){
            double err;
            if(hData_binned_sub->GetBinContent(i,j) > 10E-20) err = hData_binned_sub->GetBinError(i,j)/hData_binned_sub->GetBinContent(i,j);
            else err = 0;
            JetPtSpectra_FDsubUn->SetBinContent(i,j,err);
            JetPtSpectra_FDsubUn->SetBinError(i,j,0);
        }
    }

    JetPtSpectra_FDsubUn->SetMaximum(JetPtSpectra_FDsubUn->GetMaximum()*1.2);
    JetPtSpectra_FDsubUn->SetMinimum(0);

    shift+=0.07;

    TH1D *JetPtSpectra_FDsubUn1D[fzptJetMeasN];
    TCanvas *cSpectrumRebinUnc1D[fzptJetMeasN];
    for (Int_t bin = 1; bin <= fzptJetMeasN; bin++){
        JetPtSpectra_FDsubUn1D[bin-1] = JetPtSpectra_FDsubUn->ProjectionX("JetPtSpectra_FDsubUn1D",bin,bin);
        pv3 = new TPaveText(0.7,0.65-shift,0.9,0.7-shift,"brNDC");
        pv3->SetFillStyle(0);
        pv3->SetBorderSize(0);
        pv3->SetTextFont(42);
        pv3->SetTextSize(0.04f);
        pv3->SetTextAlign(11);
        pv3->AddText(Form("%d < p_{T,%s} < %d GeV/#it{c}",static_cast<Int_t>(fzptbinsDA[bin-1][0]),fDmesonS.Data(),static_cast<Int_t>(fzptbinsDA[bin-1][fzptbinsDN[bin-1]])));
        pv3->AddText(Form("%d < p_{T,ch. jet} < %d GeV/#it{c}",static_cast<Int_t>(fzptJetMeasA[0]),static_cast<Int_t>(fzptJetMeasA[0])));

        cSpectrumRebinUnc1D[bin-1] = new TCanvas(Form("cSpectrumRebinUnc1D%d",bin),Form("cSpectrumRebinUnc1D%d",bin),800,500);
        JetPtSpectra_FDsubUn1D[bin-1]->Draw();
        pv3->Draw("same");

        cSpectrumRebinUnc1D[bin-1]->SaveAs(Form("%s/JetPtSpectra_FDsubUnc%d.png",outPlotDir.Data(),bin));
        cSpectrumRebinUnc1D[bin-1]->SaveAs(Form("%s/JetPtSpectra_FDsubUnc%d.pdf",outPlotDir.Data(),bin));


    }

    TH1D *hFD_ratio1D[fzptJetMeasN];
    TH1D *hFD_ratio_up1D[fzptJetMeasN];
    TH1D *hFD_ratio_down1D[fzptJetMeasN];
    TH1D *hFDUnc1D[fzptJetMeasN];
    TCanvas *cRatio1D[fzptJetMeasN];
    TCanvas *cFDUnc1D[fzptJetMeasN];
    for (Int_t bin = 1; bin <= fzptJetMeasN; bin++){
        hFD_ratio1D[bin-1] = hFD_ratio->ProjectionX("hFD_ratio1D",bin,bin);
        if(isSys)hFD_ratio_up1D[bin-1] = hFD_ratio_up->ProjectionX("hFD_ratio_up1D",bin,bin);
        if(isSys)hFD_ratio_down1D[bin-1] = hFD_ratio_down->ProjectionX("hFD_ratio_down1D",bin,bin);
        cRatio1D[bin-1] = new TCanvas(Form("cRatio1D%d",bin),Form("cRatio1D%d",bin),1000,500);
        hFD_ratio1D[bin-1]->GetYaxis()->SetRangeUser(0,hFD_ratio->GetMaximum()*2);
        hFD_ratio1D[bin-1]->GetXaxis()->SetRangeUser(plotmin,plotmax);
        hFD_ratio1D[bin-1]->Draw();
        if(isSys) hFD_ratio_up1D[bin-1]->Draw("same");
        if(isSys) hFD_ratio_down1D[bin-1]->Draw("same");
        pv3 = new TPaveText(0.7,0.65-shift,0.9,0.7-shift,"brNDC");
        pv3->SetFillStyle(0);
        pv3->SetBorderSize(0);
        pv3->SetTextFont(42);
        pv3->SetTextSize(0.04f);
        pv3->SetTextAlign(11);
        pv3->AddText(Form("%d < p_{T,%s} < %d GeV/#it{c}",static_cast<Int_t>(fzptbinsDA[bin-1][0]),fDmesonS.Data(),static_cast<Int_t>(fzptbinsDA[bin-1][fzptbinsDN[bin-1]])));
        pv3->AddText(Form("%d < p_{T,ch. jet} < %d GeV/#it{c}",static_cast<Int_t>(fzptJetMeasA[0]),static_cast<Int_t>(fzptJetMeasA[0])));
        pv3->Draw("same");

        cRatio1D[bin-1]->SaveAs(Form("%s/FDratio%d.png",outPlotDir.Data(),bin));
        cRatio1D[bin-1]->SaveAs(Form("%s/FDratio%d.pdf",outPlotDir.Data(),bin));


        if(isSys){
            hFDUnc1D[bin-1] = hFDUnc->ProjectionX("hFDUnc1D",bin,bin);
            cFDUnc1D[bin-1] = new TCanvas(Form("cFDUnc1D%d",bin),Form("cFDUnc1D%d",bin),1000,500);
            hFDUnc1D[bin-1]->GetYaxis()->SetRangeUser(0,hFDUnc1D[bin-1]->GetMaximum()*1.5);
            hFDUnc1D[bin-1]->GetXaxis()->SetRangeUser(plotmin,plotmax);
            hFDUnc1D[bin-1]->Draw();
            cFDUnc1D[bin-1]->SaveAs(Form("%s/FDUnc_beforeUnf%d.png",outPlotDir.Data(),bin));
            cFDUnc1D[bin-1]->SaveAs(Form("%s/FDUnc_beforeUnf%d.pdf",outPlotDir.Data(),bin));
        }

    }








return;
}


TH1* GetUpSys(TH1D **hFD, const int nFiles, TH1D *hFD_up){


        double bin = 0, binerr = 0;
        double max = 0, maxerr = 0;


        for(int j=1; j<=hFD[0]->GetNbinsX(); j++ ){
            max = hFD[0]->GetBinContent(j);
            for(int i=1;i<nFiles;i++){
                if(hFD[i]->GetBinContent(j) > max){
                        max = hFD[i]->GetBinContent(j);
                        maxerr = hFD[i]->GetBinError(j);
                }

            }
            hFD_up->SetBinContent(j,max);
            hFD_up->SetBinError(j,0);

        }


    return hFD_up;
}

TH1* GetDownSys(TH1D **hFD, const int nFiles, TH1D *hFD_down){


        double bin = 0, binerr = 0;
        double max = 0, maxerr = 0;


        for(int j=1; j<=hFD[0]->GetNbinsX(); j++ ){
            max = hFD[0]->GetBinContent(j);
            for(int i=1;i<nFiles;i++){
                if(hFD[i]->GetBinContent(j) < max){
                        max = hFD[i]->GetBinContent(j);
                        maxerr = hFD[i]->GetBinError(j);
                }

            }
            hFD_down->SetBinContent(j,max);
            hFD_down->SetBinError(j,0);

        }


    return hFD_down;
}

TH2* foldB(RooUnfoldResponse *response, TH2D *hFD, TH2D *folded ){

    folded = new TH2D("folded","folded",fzbinsJetMeasN[fzptJetMeasN-1],fzbinsJetMeasA[fzptJetMeasN-1],fzptJetMeasN,fzptJetMeasA);


    folded  = dynamic_cast<TH2D*>(response->ApplyToTruth(hFD));

    folded->SetLineColor(2);
    folded->SetMarkerColor(2);

    TCanvas *cf = new TCanvas;
    cf->SetLogy();
    //proj[1]->Draw();
    //proj2[1]->Draw("same")
    folded->Draw("ep");
    hFD->Draw("same");
    //folded->Draw("same");

    TCanvas *cM = new TCanvas;
    cM->SetLogz();
    response->Hmeasured()->Draw("colz");
    //cM->SaveAs("FDCombMatrix.png");

    return folded;

}


TH2D* GetInputHist2D(TString fn, TString sname) {

    TH2D* inputplot =new TH2D("fRawSpectrum","fRawSpectrum",fzbinsJetMeasN[fzptJetMeasN-1],fzbinsJetMeasA[fzptJetMeasN-1],fzptJetMeasN,fzptJetMeasA);
    for(Int_t ipt = 1;ipt<=fzptJetMeasN;ipt++){
        TString dummy(fn);
        dummy+=Form("%d.root",ipt);
        std::cout<<"loading: "<<dummy<<std::endl;
        TFile *f  = TFile::Open(dummy);
        if (!f) { Error("LoadRawSpectrum","Raw spectrum file %s not found.",fn.Data());	return 0; }
        TH1D *spectrum = dynamic_cast<TH1D*>(f->Get(sname));
        if (!spectrum) {
            Error("LoadRawSpectrum","Raw spectrum %s could not be gotten from file.",sname.Data());
            return 0;
        }
        Info("LoadRawSpectrum", "%s loaded.", sname.Data());
   std::cout<<"pt bin "<<ipt<<std::endl;
        for(Int_t iz = 0;iz<fzbinsJetMeasN[ipt-1];iz++){
            Int_t Xbin = spectrum->FindBin((fzbinsJetMeasA[ipt-1][iz]+fzbinsJetMeasA[ipt-1][iz+1])/2.);
            Int_t Xbin2 = inputplot->GetXaxis()->FindBin((fzbinsJetMeasA[ipt-1][iz]+fzbinsJetMeasA[ipt-1][iz+1])/2.);
            std::cout<<"2D bin "<<Xbin2<<" pt "<<ipt<<" 1D bin "<<Xbin<< " value "<<spectrum->GetBinContent(Xbin)<<std::endl;
            inputplot->SetBinContent(Xbin2,ipt,spectrum->GetBinContent(Xbin));
            inputplot->SetBinError(Xbin2,ipt,spectrum->GetBinError(Xbin));
        }
        dummy.ReplaceAll(ipt,ipt+1);
    }
    inputplot->Sumw2();
    return inputplot;
}

void ScaleHist(TH1 *hh, int full){

    if(full){
        hh->Scale(1,"width");
        hh->GetYaxis()->SetTitle("d^{2}#sigma/d#etadp_{T} (mb #it{c}/GeV)");
    }
    else {
        hh->Scale(1,"width");
        hh->GetYaxis()->SetTitle("dN/dp_{T} (#it{c}/GeV)");
    }

}

void setHistoDetails(TH1 *hh, Color_t color, Style_t Mstyle, double Msize, Width_t Lwidth, Style_t Lstyle){

    hh->SetMarkerColor(color);
    hh->SetMarkerStyle(Mstyle);;
    hh->SetLineColor(color);
    hh->SetLineWidth(Lwidth);
    hh->SetMarkerSize(Msize);
    hh->SetLineStyle(Lstyle);
   // hh->SetName(name.c_str());

    hh->SetTitle("");
    if(fObservable == kXsection){
      hh->GetXaxis()->SetTitle("p_{T,ch,jet} (GeV/c)");
    }
    if(fObservable == kFragmentation){
      hh->GetXaxis()->SetTitle("z_{#parallel}");
    }


}

void SaveCanvas(TCanvas *c, TString name){

    c->SaveAs(Form("%s.png",name.Data()));
    c->SaveAs(Form("%s.pdf",name.Data()));
    c->SaveAs(Form("%s.svg",name.Data()));

}

TH1* GetInputHist(TString inFile, TString histName, TH1 *hh){

    TFile *jetPtFile = new TFile(inFile,"read");
  hh = dynamic_cast<TH1*>(jetPtFile->Get(histName));

  return hh;

}


RooUnfoldResponse* LoadDetectorMatrix(TString MCfile, TString out, Bool_t isPostix,TString postfix) {
    // Read the TTree reader init
    //TFile *fTreeSparse = new TFile("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts/Default_AnalysisResults_Run2.root/RM.root","READ");
    TFile *fTreeSparse = new TFile(MCfile,"READ");
    if(!fTreeSparse) std::cout<<"MC FILE not found"<<std::endl;
    Double_t jmatch[13];
    Double_t bincontent;
    TTree *tree_ = dynamic_cast<TTree*>(fTreeSparse->Get("ResponseMatrixSum_tree"));
    if(!tree_){
        TString tmp(MCfile);
        TString treepost = "";
        treepost += "TTree";

        if(isPostix){
            if(postfix == "L3" && (Rpar ==2 || Rpar ==6)){
                treepost+=postfix;
                treepost+="_1";
            }
            else{
                treepost+=postfix;
            }
        }
        treepost += "FD.root";
        tmp.Remove(MCfile.Length()-5,5).Append(treepost);
        std::cout<<"MC file do not contain TTree, trying to open TTree sparse in default path!"<<std::endl;
        std::cout<<"opening: "<<tmp<<std::endl;
        fTreeSparse = new TFile(tmp,"READ");
        if(!(fTreeSparse->IsOpen())){
            std::cout<<"TTree file not found, converting MC THnSparse to TTree now!"<<std::endl;
            SparseToTree(MCfile, isPostix,postfix);
            std::cout<<"reading TTree: "<<tmp<<std::endl;
            fTreeSparse = new TFile(tmp,"READ");
            tree_ = dynamic_cast<TTree*>(fTreeSparse->Get("ResponseMatrixSum_tree"));
        }
        else
            tree_ = dynamic_cast<TTree*>(fTreeSparse->Get("ResponseMatrixSum_tree"));
    }

    tree_->SetBranchAddress("coord",&jmatch);
    tree_->SetBranchAddress("bincontent",&bincontent);

    TFile *Feff[fzptJetMeasN];
    TH1D *eff[fzptJetMeasN];

    for (Int_t ifile = 0; ifile<fzptJetMeasN;ifile++){
        if(fuseEff == true){
            std::cout<<"efffile: "<<fefffile+Form("/DjetEff_nonPrompt_jetpt%.2f_%.2f.root",fzptJetMeasA[ifile],fzptJetMeasA[ifile+1])<<std::endl;
            Feff[ifile] = new TFile (fefffile+Form("/DjetEff_nonPrompt_jetpt%.2f_%.2f.root",fzptJetMeasA[ifile],fzptJetMeasA[ifile+1]));
            eff[ifile] = dynamic_cast<TH1D*>(Feff[ifile]->Get("hEff_reb"));
        }
    }

    //ROOunfold init
    std::cout<<"RooUnfold Init"<<std::endl;
    Int_t fzptJetMeasNl = 3;
    TH2D* hTrainTrue= new TH2D ("traintrue", "Binning Truth",fzbinsJetMeasN[fzptJetMeasNl-1],fzbinsJetMeasA[fzptJetMeasNl-1],fzptJetMeasN,fzptJetMeasA);


    for(Int_t i=0;i<fzbinsJetMeasN[fzptJetMeasN-1];i++){
        std::cout<<"binning   "<<i<<" "<<fzbinsJetMeasA[fzptJetMeasN-1][i]<<std::endl;
    }
    TH2D* hTrain= new TH2D ("train", "Binning Measured",fzbinsJetMeasN[fzptJetMeasNl-1],fzbinsJetMeasA[fzptJetMeasNl-1],fzptJetMeasN,fzptJetMeasA);
    fTrueSpectrumKineNum= new TH2D ("fTrueSpectrumKineNum", "True Spectrum Kine Num",fzbinsJetMeasN[fzptJetMeasNl-1],fzbinsJetMeasA[fzptJetMeasNl-1],fzptJetMeasN,fzptJetMeasA);
    fTrueSpectrumKineDen= new TH2D ("fTrueSpectrumKineDen", "True Spectrum Kine Den",fzbinsJetMeasN[fzptJetMeasNl-1],fzbinsJetMeasA[fzptJetMeasNl-1],fzptJetMeasN,fzptJetMeasA);

    fMeasSpectrumKineNum= new TH2D ("fMeasSpectrumKineNum", "Meas Spectrum Kine Num MC",fzbinsJetMeasN[fzptJetMeasNl-1],fzbinsJetMeasA[fzptJetMeasNl-1],fzptJetMeasN,fzptJetMeasA);
    fMeasSpectrumKineDen= new TH2D ("fMeasSpectrumKineDen", "Meas Spectrum Kine Den MC",fzbinsJetMeasN[fzptJetMeasNl-1],fzbinsJetMeasA[fzptJetMeasNl-1],fzptJetMeasN,fzptJetMeasA);


    std::cout<<"Response Setup"<<std::endl;
    RooUnfoldResponse *response = new RooUnfoldResponse();
    response->Setup(hTrain, hTrainTrue);


    std::cout<<"Looping over TTree to fill response matrix"<<std::endl;
    TRandom3 *random = new TRandom3(0);
    //TF2 *pri = new TF2("pri","1-4*(0.0007*(70-x)+0.05*y)",0,70,0,1);
    //Int_t display = 0;
    for(int i=0; i<tree_->GetEntries(); i++){
      tree_->GetEntry(i);
      Bool_t cutOK = false;
      Bool_t cutKineOK = false;
      Bool_t cutKineMCOK = false;
      if (jmatch[0]>=0 && jmatch[1]>=0 && -(0.9-fRpar) <= jmatch[4] && jmatch[4] <= 0.9-fRpar && -(0.9-fRpar) <= jmatch[9] && jmatch[9] <= 0.9-fRpar) { //found on reco also + eta true and eta reco cut
          //DpT cuts
          for(Int_t z = 0;z < fzptJetMeasN;z++){
              if(fzptJetMeasA[z] <= jmatch[1] && jmatch[1] < fzptJetMeasA[z+1]){ //jet det
                  if(2 <= jmatch[6] && jmatch[6] < 50){ //jet mc
                  if(fzptbinsDA[z][0] <= jmatch[2] && jmatch[2] < fzptbinsDA[z][fzptbinsDN[z]]){ //D det
                    if(fzptbinsDA[z][0] <= jmatch[7] && jmatch[7] < fzptbinsDA[z][fzptbinsDN[z]]){ //D mc
                        if(jmatch[0]>=fzbinsJetMeasA[z][0] && jmatch[5]>=fzbinsJetTrueAPrompt[z][0]){
                             cutOK = true;
                         }
                    }
                     }
                  }
              }
          }
          for(Int_t z = 0;z < fzptJetMeasN;z++){
              if(fzptJetMeasA[z] <= jmatch[6] && jmatch[6] < fzptJetMeasA[z+1]){ //jet mc
                  if(fzptbinsDA[z][0] <= jmatch[2] && jmatch[2] < fzptbinsDA[z][fzptbinsDN[z]]){ //D det
                      if(fzptbinsDA[z][0] <= jmatch[7] && jmatch[7] < fzptbinsDA[z][fzptbinsDN[z]]){ //D mc
                      if(jmatch[5]>=fzbinsJetTrueAPrompt[z][0]){ //z mc
                        cutKineOK = true;
                      }
                      }
                  }
              }
          }
          for(Int_t z = 0;z < fzptJetMeasN;z++){
              if(fzptJetMeasA[z] <= jmatch[1] && jmatch[1] < fzptJetMeasA[z+1]){ //jet det
                  if(fzptbinsDA[z][0] <= jmatch[2] && jmatch[2] < fzptbinsDA[z][fzptbinsDN[z]]){ //D det
                  if(fzptbinsDA[z][0] <= jmatch[7] && jmatch[7] < fzptbinsDA[z][fzptbinsDN[z]]){ //D mc
                      if(jmatch[0]>=fzbinsJetMeasA[z][0]){
                        cutKineMCOK = true;
                      }
                      }
                  }
              }
          }
          if(cutOK == true){
              //fot z_true shift values z=1 into previous bin

              Int_t effBin = -1;
              Double_t peff = 1.0;
              if(fuseEff == true){
                for (Int_t ifile = 0; ifile<fzptJetMeasN;ifile++){
                  if(fzptJetMeasA[ifile] < jmatch[1] && jmatch[1] <= fzptJetMeasA[ifile+1]) effBin = ifile;
                }
                peff = eff[effBin]->GetBinContent(eff[effBin]->FindBin(jmatch[2]));
              }


//std::cout<<"Jet pT: "<<jmatch[1]<<" id: "<<effBin<<" DpT: "<<jmatch[2]<<" eff "<<peff<<std::endl;

              hTrainTrue->Fill (jmatch[5],jmatch[6],bincontent/peff);
              hTrain->Fill (jmatch[0],jmatch[1],bincontent/peff);
              response->Fill (jmatch[0],jmatch[1],jmatch[5],jmatch[6],bincontent/peff);
              cutOK=false;
          }

          if(cutKineOK == true){

              for(Int_t z = 0;z < fzptJetMeasN;z++){
                  if(fzptJetMeasA[z] <= jmatch[6] && jmatch[6] <=fzptJetMeasA[z+1]){ //jet mc
                      if(fzbinsJetMeasA[z][0] <= jmatch[0] && jmatch[0] <= fzbinsJetMeasA[z][fzbinsJetMeasN[z]] && 2 <= jmatch[1] && jmatch[1] < 50){ //z det
                         fTrueSpectrumKineNum->Fill(jmatch[5],jmatch[6]);
                      }
                  }
              }
            //  if(2 <= jmatch[1] && jmatch[1] <= 50){
                  //std::cout<<fzptJetMeasA[0]<<" "<<jmatch[1]<<" "<<fzptJetMeasA[fzptJetMeasN]<<" "<<zShiftTrue<<" "<<jmatch[6]<<std::endl;
                 // fTrueSpectrumKineNum->Fill(zShiftTrue,jmatch[6]);
              //}
              fTrueSpectrumKineDen->Fill(jmatch[5],jmatch[6]);
              cutKineOK=false;
          }
          if(cutKineMCOK == true){
              //for z_true shift values z=1 into previous bin

              if(2 <= jmatch[6] && jmatch[6] < 50 && 0.4 <= jmatch[5]){
                  //std::cout<<fzptJetMeasA[0]<<" "<<jmatch[1]<<" "<<fzptJetMeasA[fzptJetMeasN]<<" "<<zShiftTrue<<" "<<jmatch[6]<<std::endl;
                  fMeasSpectrumKineNum->Fill(jmatch[0],jmatch[1]);
              }
           //   else{
           //       std::cout<<"IAM OUT"<<std::endl;
           //   }
            /*  Double_t zShiftDet = -1;
              if(jmatch[0]>1.0) zShiftDet = jmatch[0] - 0.02;
              else zShiftDet = jmatch[0];
              for(Int_t z = 0;z < fzptJetMeasN;z++){
                  if(fzptJetMeasA[z] <= jmatch[1] && jmatch[1] <= fzptJetMeasA[z+1]){ //jet mc
                      if(fzbinsJetMeasA[z][0] <= zShiftTrue && zShiftTrue <= fzbinsJetMeasA[z][fzbinsJetMeasN[z]]){ //D det
                         fMeasSpectrumKineNum->Fill(zShiftDet,jmatch[1]);
                      }
                  }
              }*/
              fMeasSpectrumKineDen->Fill(jmatch[0],jmatch[1]);
              cutKineMCOK=false;
          }
      }

    }

    TCanvas *c = new TCanvas("c","Folding2D_measured_true",1600,800);
    c->Divide(2,1);
    c->cd(1);
    c->cd(1)->SetLogz();
    response->Hmeasured()->Draw("colz");
    c->cd(2);
    c->cd(2)->SetLogz();
    response->Htruth()->Draw("colz");
    c->SaveAs(out+"/Folding2D_Response_MeasuredTrue.png");
    c->SaveAs(out+"/Folding2D_Response_MeasuredTrue.pdf");

    TPaveText *AnaInfo= new TPaveText(0.2,0.1,0.9,0.9,"brNDC");
    AnaInfo->SetFillStyle(0);
    AnaInfo->SetBorderSize(0);
    AnaInfo->SetTextFont(42);
    AnaInfo->SetTextSize(0.065f);
    AnaInfo->SetTextAlign(11);
    AnaInfo->AddText(Form("%s",fSystemS.Data()));
    AnaInfo->AddText(Form("Charged Jets, Anti-#it{k}_{T}, #it{R} = 0.%d",Rpar));
    AnaInfo->AddText("with D^{0} #rightarrow K^{-}#pi^{+} and charge conj.");
    AnaInfo->AddText(Form("|#it{#eta}_{jet}| < 0.%d",9-Rpar));

    TPaveText *pvTrue[fzptJetTrueN];
    TPaveText *pvReco[fzptJetMeasN];
    for(Int_t i = 0;i < fzptJetTrueN;i++){
        for(Int_t j = 0;j < fzptJetMeasN;j++){
            resSlices[i][j]=new TH2D(Form("ResponseSlice%d_%d",i,j),Form("ResponseSlice%d_%d",i,j),fzbinsJetMeasN[fzptJetMeasNl-1],fzbinsJetMeasA[fzptJetMeasNl-1],fzbinsJetMeasN[fzptJetMeasNl-1],fzbinsJetMeasA[fzptJetMeasNl-1]);
            resSlices[i][j]->GetXaxis()->SetTitle("z^{true}");
            resSlices[i][j]->GetYaxis()->SetTitle("z^{reco}");
        }
    }

    for(Int_t i = 0;i < fzptJetTrueN;i++){
        pvTrue[i] = new TPaveText(0.2,0.4,0.8,0.6,"brNDC");
        pvTrue[i]->SetFillStyle(0);
        pvTrue[i]->SetBorderSize(0);
        pvTrue[i]->SetTextFont(42);
        pvTrue[i]->SetTextSize(0.1f);
        pvTrue[i]->SetTextAlign(11);
        pvTrue[i]->AddText(Form("%d < p_{T,jet}^{true} < %d GeV/#it{c}",static_cast<Int_t>(fzptJetMeasA[i]),static_cast<Int_t>(fzptJetMeasA[i+1])));
    }
    for(Int_t i = 0;i < fzptJetMeasN;i++){
        pvReco[i] = new TPaveText(0.2,0.3,0.8,0.7,"brNDC");
        pvReco[i]->SetFillStyle(0);
        pvReco[i]->SetBorderSize(0);
        pvReco[i]->SetTextFont(42);
        pvReco[i]->SetTextSize(0.1f);
        pvReco[i]->SetTextAlign(11);
        pvReco[i]->AddText(Form("%d < p_{T,jet}^{reco} < %d GeV/#it{c}",static_cast<Int_t>(fzptJetMeasA[i]),static_cast<Int_t>(fzptJetMeasA[i+1])));
        pvReco[i]->AddText(Form("%d < p_{T,%s}^{reco} < %d GeV/#it{c}",static_cast<Int_t>(fzptbinsDA[i][0]),fDmesonS.Data(),static_cast<Int_t>(fzptbinsDA[i][fzptbinsDN[i]])));
        pvReco[i]->AddText(Form("%.1f < z_{#parallel}^{reco} < %.1f",static_cast<Double_t>(fzbinsJetMeasA[i][0]),static_cast<Double_t>(fzbinsJetMeasA[i][fzbinsJetMeasN[i]])));
    }

    TH1 *res = response->Hresponse();
    TCanvas *cc = new TCanvas("cc","cc",1600,1600);
    cc->cd();
    cc->SetLogz();
    res->Draw("colz");
    cc->SaveAs(out+"/Folding2D_ResponseMatrix.png");
    cc->SaveAs(out+"/Folding2D_ResponseMatrix.pdf");

    TFile *filres = new TFile(out+"/Response.root","recreate");
    filres->cd();
    res->Write("RM");
    filres->Close();

    for(Int_t recoID = 0;recoID < res->GetNbinsX();recoID++){
        std::cout<<recoID+1<<" reco under "<<res->GetBinContent(recoID+1,0)<<" over "<<res->GetBinContent(recoID+1,res->GetNbinsY()+1)<<std::endl;
        for(Int_t trueID = 0;trueID < res->GetNbinsY();trueID++){
            //if(recoID==0)std::cout<<trueID<<" true under "<<res->GetBinContent(0,trueID+1)<<" over "<<res->GetBinContent(res->GetNbinsX()+1,trueID+1)<<std::endl;
            Int_t recoPtbin = recoID/fzbinsJetMeasN[fzptJetMeasNl-1];
            Int_t recoZbin = recoID%fzbinsJetMeasN[fzptJetMeasNl-1];
            Int_t truePtbin = trueID/fzbinsJetMeasN[fzptJetMeasNl-1];
            Int_t trueZbin = trueID%fzbinsJetMeasN[fzptJetMeasNl-1];
            resSlices[truePtbin][recoPtbin]->SetBinContent(trueZbin+1,recoZbin+1,res->GetBinContent(recoID+1,trueID+1));
        }
    }

    TCanvas *cResSlice = new TCanvas("cResSlice","cResSlice",800*fzptJetTrueN+800,800*fzptJetMeasN+800);
    cResSlice->Divide(fzptJetMeasN+1,fzptJetMeasN+1);
    for(Int_t i = 0;i < fzptJetMeasN;i++){
        for(Int_t j = 0;j < fzptJetMeasN;j++){
            if(i==0 && j==0){
                cResSlice->cd(1);
                AnaInfo->Draw();
            }
            //std::cout<<i<<" "<<j<<" "<<i+fzptJetTrueN*(j)+1+fzptJetTrueN+j+1+1<<std::endl;
            cResSlice->cd(i+fzptJetMeasN*(j)+1+fzptJetMeasN+j+1+1);
            cResSlice->cd(i+fzptJetMeasN*(j)+1+fzptJetMeasN+j+1+1)->SetLogz();
            resSlices[i][j]->Draw("colz");
            if(i==j){
                cResSlice->cd((j+1)*fzptJetMeasN+i+2);
                pvReco[j]->Draw();
            }
        }
        cResSlice->cd(i+2);
        pvTrue[i]->Draw();
    }
    cResSlice->SaveAs(out+"/Folding2D_Response_Slice.png");
    cResSlice->SaveAs(out+"/Folding2D_Response_Slice.pdf");

    return response;
}


void SparseToTree(TString MCfile, Bool_t isPostfix, TString postfix)
{
// Read the THnSparse
    TFile *File = new TFile(MCfile,"read");
    TDirectoryFile* dir=(TDirectoryFile*)File->Get("DmesonsForJetCorrelations");
    TString histName;
    if(fDmesonSpecie) histName = "histosDStarMBN";
                    else histName = "histosD0MBN";
    Int_t NDMC = 0;
    if(isPostfix){
        if(postfix == "L3" && (Rpar ==2 || Rpar ==6)){
            NDMC = 1;
        }
        else{
            NDMC = 2;
        }
    }
    else{
        NDMC = 2;
    }

    TList *histList[NDMC];
    THnSparseF *sparseMC[NDMC];
    THnSparseF *h = nullptr;

    for(int i=0; i<2; i++){
        histList[i] =  (TList*)dir->Get(Form("%s%d%sFDMCrec",histName.Data(),i,isPostfix?postfix.Data():""));
        sparseMC[i] = (THnSparseF*)histList[i]->FindObject("ResponseMatrix");
        std::cout<<sparseMC[i]->GetNbins()<<std::endl;
        if(!i)h = dynamic_cast<THnSparseF*>(sparseMC[0]->Clone("ResponseMatrixSum"));
        else h->Add(sparseMC[i]);
    }
    //copy from here: https://root.cern.ch/root/html/tutorials/tree/drawsparse.C.html
    // Creates a TTree and fills it with the coordinates of all
    // filled bins. The tree will have one branch for each dimension,
    // and one for the bin content.
    TString outName(MCfile);
    outName.Remove(outName.Length()-5,5);
    outName+="TTree";
    if(isPostfix){
        if(postfix == "L3"){
            outName+=postfix;
            outName+="_1";
        }
        else{
            outName+=postfix;
        }
    }
   // if(isPostfix)outName+=postfix;
    outName+="FD.root";
    std::cout<<"Converting THnSparse: "<<MCfile<<std::endl;
    std::cout<<"Into TTree: "<<outName<<std::endl;
   TFile *File2 = new TFile(outName,"RECREATE");
   Int_t dim = h->GetNdimensions();
   TString name(h->GetName()); name += "_tree";
   TString title(h->GetTitle()); title += " tree";

   TTree* tree = new TTree(name, title);
   Double_t* x = new Double_t[dim + 1];
   memset(x, 0, sizeof(Double_t) * (dim + 1));

   TString branchname;
   for (Int_t d = 0; d < dim; ++d) {
      if (branchname.Length())
         branchname += ":";
      TAxis* axis = h->GetAxis(d);
      branchname += axis->GetName();
      branchname += "/D";
   }
   tree->Branch("coord", x, branchname);
   tree->Branch("bincontent", &x[dim], "bincontent/D");

   const int percentPrint = 1;
   int totalWindows = h->GetNbins();
   int percent;
   int step = totalWindows / (100/percentPrint);
   int nextPrint = step;

   Int_t *bins = new Int_t[dim];
   for (Long64_t i = 0; i < h->GetNbins(); ++i) {
      if (i >= nextPrint)
       {
           percent = (100 * i) / totalWindows;
           std::cout << "\r" << std::string(percent/percentPrint , '|') << percent << "%";
           std::cout.flush();
           nextPrint += step;
       }
      x[dim] = h->GetBinContent(i, bins);
      for (Int_t d = 0; d < dim; ++d) {
         x[d] = h->GetAxis(d)->GetBinCenter(bins[d]);
      }

      tree->Fill();

   }
   std::cout<<std::endl<<"tree entries "<< tree->GetEntries()<<std::endl;
   File2->Write(nullptr,TObject::kOverwrite);
   File2->Close();
}
