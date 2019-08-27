 //-----------------------------------------------------------------------
 //  Author B.Trzeciak
 //  Utrecht University
 //  barbara.antonina.trzeciak@cern.ch
 //-----------------------------------------------------------------------
#include "config.h"



void signalExtraction_refl(
  TString data = "$HOME/Work/alice/analysis/pp5TeV/D0jet/outMC/AnalysisResults_fast_R03_D0MC_def.root",
  TString out = "$HOME/Work/alice/analysis/pp5TeV/D0jet/outMC/reflections",
  Bool_t postfix = 1, TString listName = "cut2",
  Bool_t isMoreFiles = 0,
  TString prod = "kl",   // for more than 1 file, for one file leave it empty)
  Int_t zBin = 0
)
{
  const Int_t     fptbinsDN = 12;
  Double_t        fptbinsDA[fptbinsDN+1] = { 1,2,3,4,5,6,7,8,10,12,16,24,36};
  Int_t fRebinMass = 2;
  Double_t jetmin = -10, jetmax = 50;
  Double_t zmin = -2, zmax = 2;
  Double_t minf = 1.65, maxf = 2.1;
  if(fObservable == kFragmentation){
      jetmin = fzptJetMeasA[zBin-1];
      jetmax = fzptJetMeasA[zBin];
      std::cout<<"JET RANGES: "<<jetmin<<" "<<jetmax<<std::endl;
  }

    TString plotsDir = "/plots";
    TString outdir = out;
    gSystem->Exec(Form("mkdir %s",outdir.Data()));
    gSystem->Exec(Form("mkdir %s%s",outdir.Data(),plotsDir.Data()));

    if(!isMoreFiles) prod="";
    int nFiles = static_cast<int>(prod.Length());

    TString histName = "histosD0MBN";
    // get analysis output file
    TString datafile;
    TFile *File;
    TDirectoryFile* dir;
    TList *histList;
    THnSparseF *sparse;

    TH2D *hInvMassptDSig = nullptr, *hInvMassptDRefl = nullptr;
    if(!isMoreFiles) {
      datafile = data;
      File = new TFile(datafile,"read");
      if(!File) { std::cout << "==== WRONG FILE WITH DATA =====\n\n"; return ;}
      dir=dynamic_cast<TDirectoryFile*>(File->Get("DmesonsForJetCorrelations"));

      for(int i=0;i<NDMC; i++){
          if(postfix) histList =  dynamic_cast<TList*>(dir->Get(Form("%s%d%sMCrec",histName.Data(),i,listName.Data())));
          else histList =  dynamic_cast<TList*>(dir->Get(Form("%s%dMCrec",histName.Data(),i)));
          sparse = dynamic_cast<THnSparseF*>(histList->FindObject("hsDphiz"));
          sparse->GetAxis(0)->SetRangeUser(zmin,zmax);
          sparse->GetAxis(1)->SetRangeUser(jetmin,jetmax);
          if(i==0) {
            hInvMassptDSig = dynamic_cast<TH2D*>(sparse->Projection(2,6));
            hInvMassptDRefl = dynamic_cast<TH2D*>(sparse->Projection(2,7));
          }
          else {
            hInvMassptDSig->Add(dynamic_cast<TH2D*>(sparse->Projection(2,6)));
            hInvMassptDRefl->Add(dynamic_cast<TH2D*>(sparse->Projection(2,7)));
          }
      }
    }
    else {
      for (int j=0;j<nFiles;j++){
          datafile = data;
          datafile += prod.Data()[j];
          datafile += ".root";
          File = new TFile(datafile,"read");
          if(!File) { std::cout << "==== WRONG FILE WITH DATA =====\n\n"; return ;}
          dir=dynamic_cast<TDirectoryFile*>(File->Get("DmesonsForJetCorrelations"));

          for(int i=0;i<2; i++){
              if(postfix) histList =  dynamic_cast<TList*>(dir->Get(Form("%s%d%sMCrec",histName.Data(),i,listName.Data())));
              else histList =  dynamic_cast<TList*>(dir->Get(Form("%s%dMCrec",histName.Data(),i)));
              sparse = dynamic_cast<THnSparseF*>(histList->FindObject("hsDphiz"));
              sparse->GetAxis(0)->SetRangeUser(zmin,zmax);
              sparse->GetAxis(1)->SetRangeUser(jetmin,jetmax);
              if(i==0 && j==0) {
                hInvMassptDSig = dynamic_cast<TH2D*>(sparse->Projection(2,6));
                hInvMassptDRefl = dynamic_cast<TH2D*>(sparse->Projection(2,7));
              }
              else {
                hInvMassptDSig->Add(dynamic_cast<TH2D*>(sparse->Projection(2,6)));
                hInvMassptDRefl->Add(dynamic_cast<TH2D*>(sparse->Projection(2,7)));
              }
          }
      }
    }

    TH1D *hsig[fptbinsDN], *hrefl[fptbinsDN];
    TH1D *hFitReflNewTemp[fptbinsDN], *ratio[fptbinsDN];
    TString formulaSig = "[0]/([2]*TMath::Sqrt(2*TMath::Pi()))*exp(-(x-[1])*(x-[1])/(2*[2]*[2]))";
    TString formulaRef = "[0]/(TMath::Sqrt(2.*TMath::Pi())*[2])*TMath::Exp(-(x-[1])*(x-[1])/(2.*[2]*[2]))+[3]/( TMath::Sqrt(2.*TMath::Pi())*[5])*TMath::Exp(-(x-[4])*(x-[4])/(2.*[5]*[5]))";

    TCanvas *cSig = new TCanvas("cSig","cSig",1600,1800);
    cSig->Divide(4,3);
    TCanvas *cRefl = new TCanvas("cRefl","cRefl",1600,1800);
    cRefl->Divide(4,3);
    TCanvas *cRefl2 = new TCanvas("cRefl2","cRefl2",1600,1800);
    cRefl2->Divide(4,3);
    TCanvas *cRatio = new TCanvas("cRatio","cRatio",1600,1800);
    cRatio->Divide(4,3);

    std::ofstream myfile (Form("%s/Sigmas%s%s.txt",outdir.Data(), postfix ? listName.Data() : "pp" ,(zBin!=0)?Form("%d",zBin):""));

    for(int i=0; i<fptbinsDN; i++){
      hsig[i] = dynamic_cast<TH1D*>(hInvMassptDSig->ProjectionX(Form("histSgn_%d_%d",static_cast<int>(fptbinsDA[i]),static_cast<int>(fptbinsDA[i+1])),hInvMassptDSig->GetYaxis()->FindBin(fptbinsDA[i]), hInvMassptDSig->GetYaxis()->FindBin(fptbinsDA[i+1])-1));
      hrefl[i] = dynamic_cast<TH1D*>(hInvMassptDRefl->ProjectionX(Form("hrefl_%d_%d",static_cast<int>(fptbinsDA[i]),static_cast<int>(fptbinsDA[i+1])),hInvMassptDRefl->GetYaxis()->FindBin(fptbinsDA[i]), hInvMassptDRefl->GetYaxis()->FindBin(fptbinsDA[i+1])-1));
      hsig[i]->Rebin(fRebinMass);
      hrefl[i]->Rebin(fRebinMass);

      hsig[i]->GetXaxis()->SetRangeUser(minf,maxf);
      hrefl[i]->GetXaxis()->SetRangeUser(minf,maxf);
      hsig[i]->SetTitle(Form("%.1lf < pt^{D} < %.1lf",fptbinsDA[i],fptbinsDA[i+1]));
      hrefl[i]->SetTitle(Form("%.1lf < pt^{D} < %.1lf",fptbinsDA[i],fptbinsDA[i+1]));

      TF1 *gaussMCSignal=new TF1("gaussMCSig",formulaSig.Data(),minf,maxf);
      gaussMCSignal->SetParName(0,"IntegralSgn");
      gaussMCSignal->SetParName(1,"Mean");
      gaussMCSignal->SetParName(2,"Sigma");
      gaussMCSignal->SetParameter(0,1);
      gaussMCSignal->SetParameter(1,1.864);
      gaussMCSignal->SetParameter(2,0.010);
      gaussMCSignal->SetLineColor(kOrange+2);
      cSig->cd(i+1);
      gStyle->SetOptFit(11111);
      hsig[i]->Fit("gaussMCSig","RI","",1.65,2.15);
      hsig[i]->Draw();


      if(hsig[i]->GetEntries() > 1){
        myfile <<"pT:"<<fptbinsDA[i]<<"-"<<fptbinsDA[i+1]<<": "<<gaussMCSignal->GetParameter(2)<<" "<<gaussMCSignal->GetParError(2)<<"\n";
      }
      else
        myfile <<"pT:"<<fptbinsDA[i]<<"-"<<fptbinsDA[i+1]<<": "<<"no entries"<<"\n";

      cRefl->cd(i+1);
      TF1 *doublegaussMCRefl=new TF1("doublegaussMCRefl",formulaRef.Data(),minf,maxf);
      doublegaussMCRefl->SetParName(0,"IntegralRefl");
    //  doublegaussMCRefl->SetParName(1,"Mean");
    //  doublegaussMCRefl->SetParName(2,"Sigma");
    //  doublegaussMCRefl->SetParameter(0,1);
    //  doublegaussMCRefl->SetParameter(1,1.864);
    //  doublegaussMCRefl->SetParameter(2,0.010);
      doublegaussMCRefl->SetLineColor(kRed+2);
      doublegaussMCRefl->SetParameter(0, 1);
      doublegaussMCRefl->SetParameter(1, 1);
      doublegaussMCRefl->SetParameter(2, 1);
      doublegaussMCRefl->SetParameter(3, 1);
      doublegaussMCRefl->SetParameter(4, 1);
      doublegaussMCRefl->SetParameter(5, 1);
      hrefl[i]->Fit("doublegaussMCRefl", "MLFR");
      hrefl[i]->Draw();

      cRefl2->cd(i+1);
      TF1 *fFitRefl = hrefl[i]->GetFunction("doublegaussMCRefl");
      //fFitReflection->cd();
      hFitReflNewTemp[i] = dynamic_cast<TH1D*>(hrefl[i]->Clone(Form("histRflFittedDoubleGaus_pt%d_%d",static_cast<int>(fptbinsDA[i]),static_cast<int>(fptbinsDA[i+1]))));
      ratio[i] = dynamic_cast<TH1D*>(hrefl[i]->Clone(Form("ratioRelDistr_pt%d_%d", static_cast<int>(fptbinsDA[i]),static_cast<int>(fptbinsDA[i+1]))));

      for(Int_t iBin2=1; iBin2<=hrefl[i]->GetNbinsX(); iBin2++){
        hFitReflNewTemp[i]->SetBinContent(iBin2, 0.);
        ratio[i]->SetBinContent(iBin2, 0.);

        hFitReflNewTemp[i]->SetBinContent(iBin2, fFitRefl->Eval(hrefl[i]->GetBinCenter(iBin2)));
        ratio[i]->SetBinContent(iBin2, (hrefl[i]->GetBinContent(iBin2) / fFitRefl->Eval(hrefl[i]->GetBinCenter(iBin2))));

      }
      cRefl2->cd(i+1);
      hFitReflNewTemp[i]->Draw();

      cRatio->cd(i+1);
      ratio[i]->GetYaxis()->SetRangeUser(-1.5, 3.);
      ratio[i]->Draw();
      ratio[i]->Fit("pol0", "FM");

    }
    myfile.close();


    // --------------------------------------------------------
    // ----------- write to output file
    TFile *ofile = new TFile(Form("%s/reflectionTemplates_%s%s.root",outdir.Data(), postfix ? listName.Data() : "pp" ,(zBin!=0)?Form("%d",zBin):""),"RECREATE");
    for(int i=0; i<fptbinsDN; i++){
      hsig[i]->Write();
      hrefl[i]->Write();
      hFitReflNewTemp[i]->Write();
      ratio[i]->Write();
    }
    ofile->Close();
    // --------------------------------------------------------

}
