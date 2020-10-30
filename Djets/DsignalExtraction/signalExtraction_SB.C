/**************************************************************************
 * Copyright(c) 1998-2017, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

 //-----------------------------------------------------------------------
 //  Author B.Trzeciak
 //  Utrecht University
 //  barbara.antonina.trzeciak@cern.ch
 //-----------------------------------------------------------------------

 // Extraction of raw (or efficiency-corrected) D-jet pT spectrum
 // inv. mass method: side-band


#include "signalExtraction.h"
Int_t fBin;


void signalExtraction_SB(
  TString data = "$HOME/Work/alice/analysis/out/AnalysisResults.root",
  Bool_t isEff = 0,
  TString efffile = "../efficiency/DjetEff_prompt.root",
  Bool_t isRef = 0,
  TString refFile = "test.root",
  Bool_t postfix = 0,
  TString listName = "Cut",
  TString out = "signalExtraction",
  Bool_t save = 1,
  Bool_t isMoreFiles = 0,
  TString prod = "kl",   // for more than 1 file, for one file leave it empty)
  Bool_t isprefix=0,
  Bool_t isRefSys=0,
  Double_t refScale = 1.5,
  Int_t zBin = 0
)
{
    if (fObservable == Observable::kXsection){
        ptbinsDN_=fptbinsDN;
        ptbinsDA_=fptbinsDA;
        jetmin = fptbinsJetMeasA[0];
        jetmax = fptbinsJetMeasA[fptbinsJetMeasN];
        jetplotmin = fptbinsJetMeasA[0];
        jetplotmax = fptbinsJetMeasA[fptbinsJetMeasN];
    }
    else if (fObservable == Observable::kFragmentation){
        ptbinsDN_=fzptbinsDN[zBin-1];
        ptbinsDA_=fzptbinsDA[zBin-1];
        jetmin = fzptJetMeasA[zBin-1];
        jetmax = fzptJetMeasA[zBin];
        jetplotmin = fzptJetMeasA[zBin-1];
        jetplotmax = fzptJetMeasA[fzptJetMeasN];
        std::cout<<"JET RANGES: "<<jetmin<<" "<<jetmax<<std::endl;
    }
    fBin=zBin;


    //TH1F *hmass[ptbinsDN_];
    hmass = new TH1F*[ptbinsDN_];
    hmass_l = new TH1F*[ptbinsDN_];
    hmass_u = new TH1F*[ptbinsDN_];
    hmass_c = new TH1F*[ptbinsDN_];
    fullfit = new TF1*[ptbinsDN_];
    massfit = new TF1*[ptbinsDN_];
    bkgfit = new TF1*[ptbinsDN_];
    bkgRfit = new TF1*[ptbinsDN_];
    hjetpt = new TH1F*[ptbinsDN_];
    hjetpt_sb = new TH1F*[ptbinsDN_];
    hjetptsub = new TH1F*[ptbinsDN_];
    hjetptcorr = new TH1F*[ptbinsDN_];


    fUseRefl = isRef;
    if(fUseRefl) fReflFilename = refFile;
    if(fUseRefl) fisRefSys = isRefSys;
    if(fUseRefl) frefScale = refScale;

    savePlots = save;
    bEff = isEff;
    if(bEff)plotsDir="/plots";
    else plotsDir = "/plotsNoEff";
    if(zBin!=0)plotsDir+=Form("%d",zBin);
    TString outdir = out;
    gSystem->Exec(Form("mkdir %s",outdir.Data()));
    gSystem->Exec(Form("mkdir %s%s",outdir.Data(),plotsDir.Data()));

    if(!isMoreFiles) prod="";
    Int_t nFiles = static_cast<Int_t>(prod.Length());

    TString histName;
		if(!isprefix){
		    if(fDmesonSpecie) histName = "histosDStarMBN";
		    else histName = "histosD0MBN";
		}
		else{
		    if(fDmesonSpecie) histName = "histosDStarMBN";
		    else histName = "histosD0";
		}
    // get analysis output file
    TString datafile;
    TFile *File;
    TDirectoryFile* dir;
    TDirectoryFile *dir2;
    TList *histList;
    THnSparseF *sparse;

    if(!isMoreFiles) {
      datafile = data;
      File = new TFile(datafile,"read");
      if(!File) { std::cout << "==== WRONG FILE WITH DATA =====\n\n"; return ;}
      dir=dynamic_cast<TDirectoryFile*>(File->Get("DmesonsForJetCorrelations"));
      dir2 = dynamic_cast<TDirectoryFile*>(File->Get("PWG3_D2H_DmesonsForJetCorrelationsMBN0"));
      AliNormalizationCounter *c = (AliNormalizationCounter*)dir2->Get("NormalizationCounter");
      nEvents = c->GetNEventsForNorm();

      for(int i=0;i<ND; i++){

		if(!isprefix){
                  if(postfix) histList =  dynamic_cast<TList*>(dir->Get(Form("%s%d%s",histName.Data(),i,listName.Data())));
                  else histList =  dynamic_cast<TList*>(dir->Get(Form("%s%d",histName.Data(),i)));
		}
        else{    if(postfix){ histList =  dynamic_cast<TList*>(dir->Get(Form("%s%sMBN%d",histName.Data(),listName.Data(),i)));}
                  else {std::cout<<postfix<<"----dude! something's wrong, postfix has to be true if prefix is, check again-----"<<std::endl; return;}
		}
          sparse = dynamic_cast<THnSparseF*>(histList->FindObject("hsDphiz"));
          sparse->GetAxis(0)->SetRangeUser(zmin,zmax);
          sparse->GetAxis(1)->SetRangeUser(jetmin,jetmax);
          if(isEta) sparse->GetAxis(5)->SetRangeUser(-jetEta,jetEta);
          if (fObservable == Observable::kXsection){
            if(i==0) hInvMassptD=dynamic_cast<TH3D*>(sparse->Projection(3,1,2));
            else hInvMassptD->Add(dynamic_cast<TH3D*>(sparse->Projection(3,1,2)));
          }
          else if (fObservable == Observable::kFragmentation){
            if(i==0) hInvMassptD=dynamic_cast<TH3D*>(sparse->Projection(3,0,2));
            else hInvMassptD->Add(dynamic_cast<TH3D*>(sparse->Projection(3,0,2)));
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

          for(int i=0;i<ND; i++){

            if(!isprefix){          if(postfix) histList =  dynamic_cast<TList*>(dir->Get(Form("%s%d%s",histName.Data(),i,listName.Data())));
                      else histList =  dynamic_cast<TList*>(dir->Get(Form("%s%d",histName.Data(),i)));
            }
            else {          if(postfix) histList =  dynamic_cast<TList*>(dir->Get(Form("%s%sMBN%d",histName.Data(),listName.Data(),i)));
                              else {std::cout<<postfix<<"----dude! something's wrong,again! postfix has to be true if prefix is, check again-----"<<std::endl; return;}
            }
              sparse = dynamic_cast<THnSparseF*>(histList->FindObject("hsDphiz"));
              //sparse->GetAxis(0)->SetRangeUser(zmin,zmax);
              sparse->GetAxis(1)->SetRangeUser(jetmin,jetmax);
              if(isEta) sparse->GetAxis(5)->SetRangeUser(-jetEta,jetEta);
              if(j==0 && i==0) hInvMassptD=dynamic_cast<TH3D*>(sparse->Projection(3,1,2));
              else hInvMassptD->Add(dynamic_cast<TH3D*>(sparse->Projection(3,1,2)));
          }
      }
    }

    efficiency = new double[ptbinsDN_];
    if(bEff){
        TFile *FileEff = new TFile(efffile.Data(),"read");
        if(!FileEff)std::cout<<"no file"<<std::endl;
        TH1D *hEff = dynamic_cast<TH1D*>(FileEff->Get("hEff_reb"));//must be double
        if(!hEff)std::cout<<"no eff"<<std::endl;
        for(int i=0;i<ptbinsDN_;i++){
            double pt = (ptbinsDA_[i]+ptbinsDA_[i+1]) / 2.;
            efficiency[i] = hEff->GetBinContent(hEff->GetXaxis()->FindBin(pt));
        }
    }
    else {
        for(int i=0;i<ptbinsDN_;i++){
            efficiency[i] = 1;
        }
    }

    // --------------------------------------------------------
    // fit inv. mass in D pT bins and get raw jet pT spectra in the signal and side-bands regions
     Bool_t okSignalExt = rawJetSpectra(outdir,prod);
     if(!okSignalExt) { std::cout << "!!!!!! Something wrong in the raw signal extraction !!!!" << std::endl; return; }
    // --------------------------------------------------------

    // --------------------------------------------------------
   //------------------- draw output histos -------------------
    if(savePlots){
      saveFitParams(outdir,prod);
      saveSpectraPlots(outdir,prod);
    }

    // --------------------------------------------------------
    // ----------- write to output file
    TFile *ofile = new TFile(Form("%s/JetPtSpectra_SB_%s%s.root",outdir.Data(), bEff ? "eff" : "noEff", (zBin!=0)?Form("%d",zBin):""),"RECREATE");
    hmean->Write();
    hsigma->Write();
    hsign->Write();
    hsb->Write();
    hSignal->Write();
    hrelErr->Write();

    hSignalEffcor->Write();
    hReflRS->Write();

    hjetptspectrum->Write();
    hjetptspectrumReb->Write();
    hjetptspectrumRebScaled->Write();
    hjetptspectrumRebEvntScaled->Write();
    hjetptspectrumRebUnc->Write();

    for(int i=0; i<ptbinsDN_; i++){
        if(hjetpt[i]) hjetpt[i]->Write();
        if(hjetpt_sb[i]) hjetpt_sb[i]->Write();
        if(hjetptsub[i]) hjetptsub[i]->Write();
        if(hmass[i]) hmass[i]->Write();
        if(hmass_l[i]) hmass_l[i]->Write();
        if(hmass_u[i]) hmass_u[i]->Write();
        if(hmass_c[i]) hmass_c[i]->Write();
        if(fullfit[i]) fullfit[i]->Write();
        if(massfit[i]) massfit[i]->Write();
        if(bkgfit[i]) bkgfit[i]->Write();
        if(bkgRfit[i] && fUseRefl && fDmesonSpecie == 0) {   bkgRfit[i]->Write(); }
        if(hjetptcorr[i]) hjetptcorr[i]->Write();
    }

    ofile->Close();
    // --------------------------------------------------------

}

Bool_t rawJetSpectra(TString outdir, TString prod){

    hmean = new TH1F("hmean","hmean",ptbinsDN_,ptbinsDA_);
    hsigma = new TH1F("hsigma","hsigma",ptbinsDN_,ptbinsDA_);
    hrelErr = new TH1F("hrelErr","hrelErr",ptbinsDN_,ptbinsDA_);
    hsign = new TH1F("hsign","hsign",ptbinsDN_,ptbinsDA_);
    hsb = new TH1F("hsb","hsb",ptbinsDN_,ptbinsDA_);
    hSignal = new TH1F("hSignal","hSignal",ptbinsDN_,ptbinsDA_);
    hSignal->Sumw2();
    hReflRS = new TH1F("hReflRS","hReflRS",ptbinsDN_,ptbinsDA_);
    hSignalEffcor = new TH1F("hSignalEffcor","hSignalEffcor",ptbinsDN_,ptbinsDA_);
    hSignalEffcor->Sumw2();

    if(fDmesonSpecie) hInvMassptD->GetXaxis()->SetTitle(Form("m(%s)(GeV/c^{2})","K#pi#pi-K#pi"));
    else hInvMassptD->GetXaxis()->SetTitle(Form("m(%s)(GeV/c^{2})","K#pi"));
    hInvMassptD->GetXaxis()->SetTitleSize(0.06f);
    hInvMassptD->GetXaxis()->SetTitleOffset(0.9f);
    hInvMassptD->GetYaxis()->SetTitle("p_{T}^{jet}");
    hInvMassptD->SetTitle("");

    TPaveText *pvProd = new TPaveText(0.8,0.25,0.98,0.3,"brNDC");
    pvProd->SetFillStyle(0);
    pvProd->SetBorderSize(0);
    pvProd->AddText(Form("%s",prod.Data()));

    TPaveText *pvCuts = new TPaveText(0.8,0.3,0.98,0.35,"brNDC");
    pvCuts->SetFillStyle(0);
    pvCuts->SetBorderSize(0);
    pvCuts->AddText(Form("%s",outdir.Data()));

    TPaveText *pvEn= new TPaveText(0.2,0.80,0.8,0.85,"brNDC");
    pvEn->SetFillStyle(0);
    pvEn->SetBorderSize(0);
    pvEn->SetTextFont(42);
    pvEn->SetTextSize(0.06f);
    pvEn->SetTextAlign(11);
    pvEn->AddText(Form("%s",fSystemS.Data()));

    double shift = 0.;
    TPaveText *pvD = new TPaveText(0.15,0.65-shift,0.9,0.7-shift,"brNDC");
    pvD->SetFillStyle(0);
    pvD->SetBorderSize(0);
    pvD->SetTextFont(42);
    pvD->SetTextSize(0.06f);
    pvD->SetTextAlign(11);
    if(fDmesonSpecie) pvD->AddText("D^{*+} #rightarrow D^{0}#pi^{+} and charge conj.");
    else pvD->AddText("D^{0} #rightarrow K^{-}#pi^{+} and charge conj.");

    TPaveText *pvJet =nullptr;
    if(fObservable==Observable::kXsection) pvJet = new TPaveText(0.15,0.55-shift,0.9,0.6-shift,"brNDC");
    if(fObservable==Observable::kFragmentation) pvJet = new TPaveText(0.15,0.45-shift,0.9,0.6-shift,"brNDC");
    pvJet->SetFillStyle(0);
    pvJet->SetBorderSize(0);
    pvJet->SetTextFont(42);
    pvJet->SetTextSize(0.06f);
    pvJet->AddText(Form("in Charged Jets, Anti-#it{k}_{T}, #it{R} = 0.%d",Rpar));
    if(fObservable==Observable::kFragmentation)pvJet->AddText(Form("%d < p_{T,ch. jet} < %d GeV/c",static_cast<Int_t>(fzptJetMeasA[fBin-1]),static_cast<Int_t>(fzptJetMeasA[fBin])));
    pvJet->SetTextAlign(11);

    TPaveText *pvEta = nullptr;
    if(fObservable==Observable::kXsection) pvEta=new TPaveText(0.15,0.45-shift,0.8,0.5-shift,"brNDC");
    if(fObservable==Observable::kFragmentation) pvEta=new TPaveText(0.15,0.35-shift,0.8,0.4-shift,"brNDC");
    pvEta->SetFillStyle(0);
    pvEta->SetBorderSize(0);
    pvEta->SetTextFont(42);
    pvEta->SetTextSize(0.06f);
    pvEta->SetTextAlign(11);
    pvEta->AddText(Form("|#it{#eta}_{jet}| < 0.%d",9-Rpar));


    int xnx = 3, xny=4;
    if(fObservable==Observable::kXsection){
        if(ptbinsDN_>4 && ptbinsDN_<7) { xnx = 2; xny=3; }
        else if(ptbinsDN_>6 && ptbinsDN_<10) { xnx = 3; xny=3; }
        else if(ptbinsDN_>9 && ptbinsDN_<13) { xnx = 3; xny=4; }
        else { xnx = 4; xny=4; }
    }
    else{
        if(ptbinsDN_<=3) { xnx = 2; xny=2; }
        else if(ptbinsDN_>=4 && ptbinsDN_<=5) { xnx = 2; xny=3; }
        else if(ptbinsDN_>=6 && ptbinsDN_<=8) { xnx = 3; xny=3; }
        else if(ptbinsDN_>=9 && ptbinsDN_<=11) { xnx = 3; xny=4; }
        else { xnx = 4; xny=4; }
    }

    TCanvas *c2 = new TCanvas("c2","c2",1200,1200);
    c2->Divide(xnx,xny);
    TCanvas *c2jet = new TCanvas("c2jet","c2jet",1200,1200);
    c2jet->Divide(xnx,xny);
    TCanvas *c2jetcorr = new TCanvas("c2jetcorr","c2jetcorr",1200,1200);
    c2jetcorr->Divide(xnx,xny);

    int firstPtBin = 0;
    if(ptbinsDA_[0] == 1) firstPtBin = 2;
    if(ptbinsDA_[0] == 2) firstPtBin = 3;
    else if(ptbinsDA_[0] == 3) firstPtBin = 4;
    else if(ptbinsDA_[0] == 4) firstPtBin = 5;
    else if(ptbinsDA_[0] == 5) firstPtBin = 6;
    else if(ptbinsDA_[0] == 6) firstPtBin = 7;
    else if(ptbinsDA_[0] == 7) firstPtBin = 8;
    else if(ptbinsDA_[0] == 8) firstPtBin = 9;
    else if(ptbinsDA_[0] == 10) firstPtBin = 10;
    if(!firstPtBin) { std::cout << "==== Wrong first value of the D pT (should be 2,3 or 4) === \n"; return kFALSE; }
    Float_t RS = 0;

    for(int i=0; i<ptbinsDN_; i++){
        std::cout<<"Processing bin: "<<ptbinsDA_[i]<<" "<<ptbinsDA_[i+1]<<" GeV/c"<<std::endl;
        TH1D *hh= hInvMassptD->ProjectionX(Form("hh_%d",i),hInvMassptD->GetYaxis()->FindBin(jetmin), hInvMassptD->GetYaxis()->FindBin(jetmax)-1,hInvMassptD->GetZaxis()->FindBin(ptbinsDA_[i]), hInvMassptD->GetZaxis()->FindBin(ptbinsDA_[i+1])-1);
        hh->Rebin(fRebinMass);
        hh->GetXaxis()->SetRangeUser(static_cast<Double_t>(minf),static_cast<Double_t>(maxf));
        hh->SetTitle(Form("%.1lf < pt^{%s} < %.1lf",ptbinsDA_[i],fDmesonS.Data(),ptbinsDA_[i+1]));

        TH1F *hmassfit = static_cast<TH1F*>(hh->Clone("hmassfit"));

        if(fDmesonSpecie) hmassfit->SetMaximum(hmassfit->GetMaximum()*1.3);

        Float_t hmin = TMath::Max(minf,static_cast<Float_t>(hmassfit->GetBinLowEdge(2)));
        Float_t hmax = TMath::Min(maxf,static_cast<Float_t>(hmassfit->GetBinLowEdge(hmassfit->GetNbinsX())));
       // AliHFMassFitter* fitterp=new AliHFMassFitter((TH1F*)hmassfit,hmin,hmax,1,fbkgtype,0);

        AliHFInvMassFitter* fitterp = new AliHFInvMassFitter(hmassfit,static_cast<Double_t>(hmin),static_cast<Double_t>(hmax),fbkgtype,0);
        fitterp->SetInitialGaussianMean(static_cast<Double_t>(fDmass));
        fitterp->SetInitialGaussianSigma(static_cast<Double_t>(fDsigma));


        if(fUseRefl && fDmesonSpecie == 0) {
          if(fSystem) SetReflection(fitterp,hmin,hmax,RS,i+firstPtBin); // older way from Fabio's templates for p-Pb
          else SetReflection(fitterp,hmin,hmax,RS,static_cast<Int_t>(ptbinsDA_[i]),static_cast<Int_t>(ptbinsDA_[i+1])); // new for pp (new templates from D-jet code)
        }
        fitterp->SetFixGaussianSigma(reflSig); //added fixing singma on MC

        fitterp->MassFitter(kFALSE);
        //fitterp->PrintFunctions();

        TH1F* h=fitterp->GetHistoClone();
        massfit[i] = fitterp->GetMassFunc();
        massfit[i]->SetRange(static_cast<Double_t>(hmin),static_cast<Double_t>(hmax));
        massfit[i]->SetLineColor(4);
        fullfit[i] = h->GetFunction("funcmass");
        if(fullfit[i]) fullfit[i]->SetName(Form("fullfit_%d",i));
        hmass[i] = static_cast<TH1F*>(h->Clone(Form("hmass_%d",i)));
        hmass[i]->SetName(Form("hmass_%d",i));
        hmass[i]->GetYaxis()->SetTitle("Entries");
        hmass[i]->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
        bkgfit[i] = fitterp->GetBackgroundRecalcFunc();
        bkgfit[i]->SetRange(static_cast<Double_t>(hmin),static_cast<Double_t>(hmax));
        bkgfit[i]->SetLineColor(2);
        bkgfit[i]->SetName(Form("bkgFit_%d",i));

        // bkg+reflection function
        if(fUseRefl && fDmesonSpecie == 0) {
          bkgRfit[i] = fitterp->GetBkgPlusReflFunc();
          bkgRfit[i]->SetName(Form("bkgFitWRef_%d",i));
          bkgRfit[i]->SetRange(static_cast<Double_t>(hmin),static_cast<Double_t>(hmax));
          bkgRfit[i]->SetLineColor(15);
          hReflRS->SetBinContent(i+1,static_cast<Double_t>(RS));
          hReflRS->SetBinError(i+1,0);
        }

        TVirtualPad *pad = dynamic_cast<TVirtualPad*>(c2->GetPad(i+1));
        fitterp->DrawHere(pad,3,0);

        Double_t Dsigma = 0, Dmean = 0, DmeanUnc = 0, DsigmaUnc = 0;

        if(!fullfit[i]) { std::cout << "======= Fit failed for bin: " << i << std::endl; continue; }

        Dsigma = fitterp->GetSigma();
        DsigmaUnc  = fitterp->GetSigmaUncertainty();
        Dmean = fitterp->GetMean();
        DmeanUnc = fitterp->GetMeanUncertainty();

        Double_t signal_c_min = Dmean-static_cast<Double_t>(fsigmaSignal)*Dsigma;
        Double_t signal_c_max = Dmean+static_cast<Double_t>(fsigmaSignal)*Dsigma;
        Double_t signal_l_min = Dmean+static_cast<Double_t>(fsigmaBkg[0])*Dsigma;
        Double_t signal_l_max = Dmean+static_cast<Double_t>(fsigmaBkg[1])*Dsigma;
        Double_t signal_u_min = Dmean+static_cast<Double_t>(fsigmaBkg[2])*Dsigma;
        Double_t signal_u_max = Dmean+static_cast<Double_t>(fsigmaBkg[3])*Dsigma;
        if(signal_l_min<static_cast<Double_t>(hmin)) signal_l_min = static_cast<Double_t>(hmin);
        if(signal_u_max>static_cast<Double_t>(hmax)) signal_u_max = static_cast<Double_t>(hmax);

        Double_t binwidth = hmass[i]->GetXaxis()->GetBinWidth(1)*0.5;
        // signal
        Int_t binmin = hmass[i]->GetXaxis()->FindBin(signal_c_min);
        Int_t binmax = hmass[i]->GetXaxis()->FindBin(signal_c_max);
        Double_t min = hmass[i]->GetXaxis()->GetBinCenter(binmin)-binwidth;
        Double_t max = hmass[i]->GetXaxis()->GetBinCenter(binmax-1)+binwidth;
        // side-bands
        Int_t binmin_sb1 = hmass[i]->GetXaxis()->FindBin(signal_l_min);
        Int_t binmax_sb1 = hmass[i]->GetXaxis()->FindBin(signal_l_max);
        Double_t min_sb1 = hmass[i]->GetXaxis()->GetBinCenter(binmin_sb1)-binwidth;
        Double_t max_sb1 = hmass[i]->GetXaxis()->GetBinCenter(binmax_sb1-1)+binwidth;
        // side-bands
        Int_t binmin_sb2 = hmass[i]->GetXaxis()->FindBin(signal_u_min);
        Int_t binmax_sb2 = hmass[i]->GetXaxis()->FindBin(signal_u_max);
        Double_t min_sb2 = hmass[i]->GetXaxis()->GetBinCenter(binmin_sb2)-binwidth;
        Double_t max_sb2 = hmass[i]->GetXaxis()->GetBinCenter(binmax_sb2-1)+binwidth;


        Double_t s=0, serr=0, srelerr=0, bkg=0, bkgerr=0, bkgref=0, ref=0;
        Double_t sb1=0, sb2=0, ref1=0, ref2=0;
        fitterp->Signal(static_cast<Double_t>(fsigmaSignal), s, serr);
        fitterp->Background(min, max ,bkg, bkgerr);
        //fitterp->Background(fsigmaSignal,b,berr);
        sb1 = bkgfit[i]->Integral(min_sb1,max_sb1)/static_cast<Double_t>(hmass[i]->GetBinWidth(1));
        sb2 = bkgfit[i]->Integral(min_sb2,max_sb2)/static_cast<Double_t>(hmass[i]->GetBinWidth(1));
        if(fUseRefl && fDmesonSpecie == 0) {
          bkgref = bkgRfit[i]->Integral(min,max)/static_cast<Double_t>(hmass[i]->GetBinWidth(1));
          ref = bkgref - bkg;
          ref1 = (bkgRfit[i]->Integral(min_sb1,max_sb1)/static_cast<Double_t>(hmass[i]->GetBinWidth(1))) - sb1;
          ref2 = (bkgRfit[i]->Integral(min_sb2,max_sb2)/static_cast<Double_t>(hmass[i]->GetBinWidth(1))) - sb2;
        }

        std::cout<<"R/B:"<<ref/bkg<<" (R1+R2)/(B1+B2):"<<(ref1+ref2)/(sb1+sb2)<<std::endl;

        Double_t signf=0,signferr=0,sob=0,soberr=0;
        fitterp->Significance(static_cast<Double_t>(fsigmaSignal),signf,signferr);
        if(static_cast<Bool_t>(s)) srelerr = serr/s;
        if(static_cast<Bool_t>(bkg)) sob = s/bkg;
        else sob = s;
        if(static_cast<Bool_t>(bkg) && static_cast<Bool_t>(bkgerr)) soberr = TMath::Sqrt((serr/bkg)*(serr/bkg) + (s/bkg/bkg*bkgerr)*(s/bkg/bkg*bkgerr));
        else soberr = serr;

        TPaveText *pvSig;
        if(fDmesonSpecie) pvSig = new TPaveText(0.55,0.47,0.95,0.75,"brNDC");
        else pvSig = new TPaveText(0.15,0.55,0.47,0.9,"brNDC");
        pvSig->SetFillStyle(0);
        pvSig->SetBorderSize(0);
        Bool_t twodigits=kTRUE;
        if(soberr*100. > 35.) twodigits=kFALSE;
        //if(twodigits) pv->AddText(Form("S/B (3#sigma) = (%.2f #pm %.2f)", sob,soberr));
        //else pv->AddText(Form("S/B (3#sigma) = (%.1f #pm %.1f)", sob,soberr));
        if(twodigits) pvSig->AddText(Form("S (2#sigma) = %.2f #pm %.2f", s,serr));
        else pvSig->AddText(Form("S (2#sigma) = %.1f #pm %.1f", s,serr));
        if(twodigits) pvSig->AddText(Form("B (2#sigma) = %.2f #pm %.2f", bkg,bkgerr));
        else pvSig->AddText(Form("B (2#sigma) = %.1f #pm %.1f", bkg,bkgerr));
        pvSig->AddText(Form("Signif.(2#sigma) = %.1f #pm %.1f", signf,signferr));
        pvSig->AddText(Form("S/B(2#sigma) = %.2f #pm %.2f", sob,soberr));
        if(fUseRefl && fDmesonSpecie == 0) pvSig->AddText(Form("R/S = %.2f", RS));
        pvSig->Draw("same");
        //if(isdetails) pvProd->Draw("same");
        //if(isdetails) pvCuts->Draw("same");

        TPaveText *pv;
        if(fDmesonSpecie) pv = new TPaveText(0.55,0.77,0.95,0.9,"brNDC");
        else pv = new TPaveText(0.57,0.67,0.95,0.9,"brNDC");
        pv->SetFillStyle(0);
        pv->SetBorderSize(0);
        if(fDmesonSpecie) pv->AddText(Form("#mu = (%.2f #pm %.2f) MeV/#it{c}^{2}", Dmean*1000,DmeanUnc*1000));
        else pv->AddText(Form("#mu = (%.2f #pm %.2f) GeV/#it{c}^{2}", Dmean,DmeanUnc));
        pv->AddText(Form("#sigma = (%.2f #pm %.2f) MeV/#it{c}^{2}", Dsigma*1000,DsigmaUnc*1000));
        pv->AddText(Form("MC #sigma = (%.2f #pm %.2f) MeV/#it{c}^{2}", reflSig*1000,reflSigErr*1000));
        pv->Draw("same");

        // ---------------- fitting results
        if(fDmesonSpecie) {
          hmean->SetBinContent(i+1,Dmean*1000);
          hmean->SetBinError(i+1,DmeanUnc*1000);
        }
        else {
          hmean->SetBinContent(i+1,Dmean);
          hmean->SetBinError(i+1,DmeanUnc);
        }
        hsigma->SetBinContent(i+1,Dsigma*1000);
        hsigma->SetBinError(i+1,DsigmaUnc*1000);

        hrelErr->SetBinContent(i+1,srelerr);
        hsign->SetBinContent(i+1,signf);
        hsign->SetBinError(i+1,signferr);
        hsb->SetBinContent(i+1,sob);
        hsb->SetBinError(i+1,soberr);
        hSignal->SetBinContent(i+1,s);
        hSignal->SetBinError(i+1,serr);


    //    hSignalEffcor->SetBinContent(i+1,hSignal->GetBinContent(i+1)/(efficiency[i]*nEvents));
    //    hSignalEffcor->SetBinError(i+1,hSignal->GetBinError(i+1)/(efficiency[i]*nEvents));

        hSignalEffcor->SetBinContent(i+1,hSignal->GetBinContent(i+1)/(efficiency[i]));
        hSignalEffcor->SetBinError(i+1,hSignal->GetBinError(i+1)/(efficiency[i]));



        // ---------------- side-band drawing
       hmass_l[i] = static_cast<TH1F*>(hmass[i]->Clone("hmass_l"));
        hmass_l[i]->GetXaxis()->SetRangeUser(signal_l_min,signal_l_max);
        hmass_l[i]->SetName(Form("hmass_l_%d",i));
        hmass_u[i] = static_cast<TH1F*>(hmass[i]->Clone("hmass_u"));
        hmass_u[i]->GetXaxis()->SetRangeUser(signal_u_min,signal_u_max);
        hmass_u[i]->SetName(Form("hmass_u_%d",i));
        hmass_c[i] = static_cast<TH1F*>(hmass[i]->Clone("hmass_c"));
        hmass_c[i]->GetXaxis()->SetRangeUser(signal_c_min,signal_c_max);
        hmass_c[i]->SetName(Form("hmass_c_%d",i));

        hmass_l[i]->SetFillColor(kBlue+2);
        hmass_u[i]->SetFillColor(kBlue+2);
        hmass_c[i]->SetFillColor(kRed+2);
        hmass_l[i]->SetFillStyle(3004);
        hmass_u[i]->SetFillStyle(3004);
        hmass_c[i]->SetFillStyle(3005);

        hmass_l[i]->Draw("hsame");
        hmass_u[i]->Draw("hsame");
        hmass_c[i]->Draw("hsame");

        //----------------------------------
        //-------- jet pt spectrum - signal
        hjetpt[i]=reinterpret_cast<TH1F*>(hInvMassptD->ProjectionY(Form("hjetpt_%d",i),hInvMassptD->GetXaxis()->FindBin(signal_c_min), hInvMassptD->GetXaxis()->FindBin(signal_c_max)-1,hInvMassptD->GetZaxis()->FindBin(ptbinsDA_[i]), hInvMassptD->GetZaxis()->FindBin(ptbinsDA_[i+1])-1));
        hjetpt[i]->GetXaxis()->SetRangeUser(jetplotmin,jetplotmax);
        hjetpt[i]->SetTitle(Form("%.1lf < pt^{%s} < %.1lf",ptbinsDA_[i],fDmesonS.Data(),ptbinsDA_[i+1]));
        hjetpt[i]->SetMarkerColor(kRed+2);
        hjetpt[i]->SetLineColor(kRed+2);

        //------ jet pt spectrum - side bands
        TH1F* hjetpt_sb1=reinterpret_cast<TH1F*>(hInvMassptD->ProjectionY(Form("hjetpt_sb1%d",i),hInvMassptD->GetXaxis()->FindBin(signal_l_min), hInvMassptD->GetXaxis()->FindBin(signal_l_max)-1,hInvMassptD->GetZaxis()->FindBin(ptbinsDA_[i]), hInvMassptD->GetZaxis()->FindBin(ptbinsDA_[i+1])-1));
        hjetpt_sb1->SetTitle(Form("%.1lf < pt^{%s} < %.1lf",ptbinsDA_[i],fDmesonS.Data(),ptbinsDA_[i+1]));
        hjetpt_sb1->SetMarkerColor(kBlue+2);
        hjetpt_sb1->SetLineColor(kBlue+2);
        TH1F* hjetpt_sb2=reinterpret_cast<TH1F*>(hInvMassptD->ProjectionY(Form("hjetpt_sb2%d",i),hInvMassptD->GetXaxis()->FindBin(signal_u_min), hInvMassptD->GetXaxis()->FindBin(signal_u_max)-1,hInvMassptD->GetZaxis()->FindBin(ptbinsDA_[i]), hInvMassptD->GetZaxis()->FindBin(ptbinsDA_[i+1])-1));
        hjetpt_sb2->SetTitle(Form("%.1lf < pt^{%s} < %.1lf",ptbinsDA_[i],fDmesonS.Data(),ptbinsDA_[i+1]));
        hjetpt_sb2->SetMarkerColor(kBlue+2);
        hjetpt_sb2->SetLineColor(kBlue+2);
        hjetpt_sb[i] = static_cast<TH1F*>(hjetpt_sb1->Clone(Form("hjetpt_sb_%d",i)));
        hjetpt_sb[i]->Add(hjetpt_sb2);

        hjetpt[i]->GetXaxis()->SetTitleOffset(1.);

        if(fObservable == Observable::kXsection){
            hjetpt[i]->GetXaxis()->SetRangeUser(jetplotmin,jetplotmax);
            hjetpt[i]->GetXaxis()->SetTitle("p_{T,ch jet} (GeV/c)");
            hjetpt_sb[i]->GetXaxis()->SetRangeUser(jetplotmin,jetplotmax);
        }
        else if(fObservable == Observable::kFragmentation){
            hjetpt[i]->GetXaxis()->SetRangeUser(fzbinsJetMeasA[fBin-1][0],fzbinsJetMeasA[fBin-1][fzbinsJetMeasN[fBin-1]]);
            hjetpt[i]->GetXaxis()->SetTitle("z_{#parallel}");
            hjetpt_sb[i]->GetXaxis()->SetRangeUser(fzbinsJetMeasA[fBin-1][0],fzbinsJetMeasA[fBin-1][fzbinsJetMeasN[fBin-1]]);
        }

        // scale background from side bands to the background under the peak
        //double scalingB = bkg / hjetpt_sb[i]->Integral();

      //  cout << "====================== scaling: " << sb2 << endl;
        Double_t scalingB = bkg/(sb1+sb2);
        Double_t scalingS = 0.0;

        if(fUseRefl && fDmesonSpecie == 0) {
          scalingS = s / ( s+ref - ( (ref1+ref2)*bkg ) / (sb1+sb2) );
        }

        hjetpt_sb[i]->Scale(scalingB);

        //------- subtract background from signal jet
        hjetptsub[i] = static_cast<TH1F*>(hjetpt[i]->Clone(Form("hjetptsub_%d",i)));
       	hjetptsub[i]->Add(hjetpt_sb[i],-1);
        if(fUseRefl && fDmesonSpecie == 0) {
          hjetptsub[i]->Scale(scalingS);
        }
	//hjetptsub[i]->Add(hjetpt_sb[i],-1);

    if(fsigmaSignal==2) hjetptsub[i]->Scale(1./0.9545);
        hjetptsub[i]->SetMarkerColor(kGreen+3);
        hjetptsub[i]->SetLineColor(kGreen+3);


        c2jet->cd(i+1);
        gPad->SetLogy();
        hjetpt[i]->Draw("ep");
        hjetpt_sb[i]->Draw("epsame");
        hjetptsub[i]->Draw("epsame");
        if(isdetails) pvProd->Draw("same");
        if(isdetails) pvCuts->Draw("same");
        TLegend *l1 =nullptr;
        if(fObservable == Observable::kXsection)l1= new TLegend(0.65,0.65,0.88,0.85);
        if(fObservable == Observable::kFragmentation)l1= new TLegend(0.15,0.65,0.38,0.85);
        l1->AddEntry(hjetpt[i],"signal","l");
        l1->AddEntry(hjetpt_sb[i],"SB","l");
        l1->Draw("same");

        TLegend *l2 = new TLegend(0.6,0.75,0.9,0.9);
        l2->AddEntry(hjetptsub[i],"bkg subtracted","l");
        l1->AddEntry(hjetptsub[i],"sig-SB","l");
        //l2->Draw("same");
        if(!hrawjetptspectrum) hrawjetptspectrum = static_cast<TH1F*>(hjetptsub[i]->Clone("hrawjetptspectrum"));
        else hrawjetptspectrum->Add(hjetptsub[i]);

        //------- correct for D* efficiency
        hjetptcorr[i] = static_cast<TH1F*>(hjetptsub[i]->Clone(Form("hjetptcorr_%d",i)));
        if(bEff) hjetptcorr[i]->Scale(1./efficiency[i]); // D efficiency
        hjetptcorr[i]->SetMarkerColor(kBlue+3);
        hjetptcorr[i]->SetLineColor(kBlue+3);

        c2jetcorr->cd(i+1);
        hjetptcorr[i]->Draw("ep");
        if(isdetails) pvProd->Draw("same");
        if(isdetails) pvCuts->Draw("same");
        TLegend *l3 = new TLegend(0.6,0.7,0.9,0.9);
        l3->AddEntry(hjetptcorr[i],"#splitline{corrected jet p_{T}}{spectrum}","l");
        l3->Draw("same");

        //----- add corrected jet pt spectrum distributions in each D pt bin into a one distribution
        if(!hjetptspectrum) hjetptspectrum = static_cast<TH1F*>(hjetptcorr[i]->Clone("hjetptspectrum"));
        else hjetptspectrum->Add(hjetptcorr[i]);


    }

    c2->cd(ptbinsDN_+1);
    pvEn->Draw();
    pvD->Draw("same");
    pvJet->Draw("same");
    pvEta->Draw("same");
    c2jet->cd(ptbinsDN_+1);
    pvEn->Draw();
    pvD->Draw("same");
    pvJet->Draw("same");
    pvEta->Draw("same");

    if(savePlots) SaveCanvas(c2,outdir+plotsDir+"/invMass"+prod);
    if(savePlots) SaveCanvas(c2jet,outdir+plotsDir+"/jetRawSpectrum"+prod);

    return kTRUE;

}

Bool_t SetReflection(AliHFInvMassFitter* &fitter, Float_t fLeftFitRange, Float_t fRightFitRange, Float_t &RS, Int_t iBin) {

  TFile *fileRefl = TFile::Open(fReflFilename.Data());
  if(!fileRefl){
    std::cout << "File " << fReflFilename << " (reflection templates) cannot be opened! check your file path!"; return kFALSE;
  }

  TString fHistnameRefl = "histRflFittedDoubleGaus_ptBin";
  TString fHistnameSign = "histSgn_";
  TH1F *histRefl = dynamic_cast<TH1F*>(fileRefl->Get(Form("%s%d",fHistnameRefl.Data(),iBin)));
  TH1F *histSign = dynamic_cast<TH1F*>(fileRefl->Get(Form("%s%d",fHistnameSign.Data(),iBin)));
  if(!histRefl || !histSign){
    std::cout << "Error in loading the template/signal histrograms! Exiting..." << std::endl; return kFALSE;
  }

  fitter->SetTemplateReflections(histRefl,"template",static_cast<Double_t>(fLeftFitRange),static_cast<Double_t>(fRightFitRange));
  Double_t RoverS = histRefl->Integral(histRefl->FindBin(static_cast<Double_t>(fLeftFitRange)),histRefl->FindBin(static_cast<Double_t>(fRightFitRange)))/histSign->Integral(histSign->FindBin(static_cast<Double_t>(fLeftFitRange)),histSign->FindBin(static_cast<Double_t>(fRightFitRange)));
  if(fisRefSys) RoverS*=frefScale;
  printf("R/S ratio in fit range for bin %d = %1.3f\n",iBin,RoverS);
  fitter->SetFixReflOverS(RoverS);

  RS = static_cast<Float_t>(RoverS);
  return kTRUE;

}


Bool_t SetReflection(AliHFInvMassFitter* &fitter, Float_t fLeftFitRange, Float_t fRightFitRange, Float_t &RS, Int_t ptmin, Int_t ptmax) {

  TFile *fileRefl = TFile::Open(fReflFilename.Data());
  if(!fileRefl){
    std::cout << "File " << fReflFilename << " (reflection templates) cannot be opened! check your file path!"; return kFALSE;
  }

  TString fHistnameRefl = "histRflFittedDoubleGaus_pt";
  TString fHistnameSign = "histSgn_";
  TH1D *histRefl = dynamic_cast<TH1D*>(fileRefl->Get(Form("%s%d_%d",fHistnameRefl.Data(),ptmin,ptmax)));
  TH1D *histSign = dynamic_cast<TH1D*>(fileRefl->Get(Form("%s%d_%d",fHistnameSign.Data(),ptmin,ptmax)));
  if(!histRefl || !histSign){
    std::cout << "Error in loading the template/signal histrograms! Exiting..." << std::endl; return kFALSE;
  }
//std::cout<<"GETTING PARAM "<<histSign->GetFunction("gaussMCSig")->GetParameter(2)<<" "<<histSign->GetFunction("gaussMCSig")->GetParError(2)<<std::endl;
  reflSig=histSign->GetFunction("gaussMCSig")->GetParameter(2);
  reflSigErr=histSign->GetFunction("gaussMCSig")->GetParError(2);
  fitter->SetTemplateReflections(histRefl,"template",static_cast<Double_t>(fLeftFitRange),static_cast<Double_t>(fRightFitRange));
  Double_t RoverS = histRefl->Integral(histRefl->FindBin(static_cast<Double_t>(fLeftFitRange)),histRefl->FindBin(static_cast<Double_t>(fRightFitRange)))/histSign->Integral(histSign->FindBin(static_cast<Double_t>(fLeftFitRange)),histSign->FindBin(static_cast<Double_t>(fRightFitRange)));
  if(fisRefSys) RoverS*=frefScale;
  //printf("R/S ratio in fit range for bin %d = %1.3f\n",iBin,RoverS);
  fitter->SetFixReflOverS(RoverS);

  RS = static_cast<Float_t>(RoverS);
  return kTRUE;

}

void  saveSpectraPlots(TString outdir,TString prod){

      TPaveText *pvProd = new TPaveText(0.75,0.65,0.9,0.7,"brNDC");
      pvProd->SetFillStyle(0);
      pvProd->SetBorderSize(0);
      pvProd->AddText(Form("%s",prod.Data()));

      TPaveText *pvCuts = new TPaveText(0.75,0.6,0.9,0.65,"brNDC");
      pvCuts->SetFillStyle(0);
      pvCuts->SetBorderSize(0);
      pvCuts->AddText(Form("%s",outdir.Data()));

      TPaveText *pvEn= new TPaveText(0.2,0.80,0.8,0.85,"brNDC");
      pvEn->SetFillStyle(0);
      pvEn->SetBorderSize(0);
      pvEn->SetTextFont(42);
      pvEn->SetTextSize(0.045f);
      pvEn->SetTextAlign(11);
      pvEn->AddText(Form("%s",fSystemS.Data()));

      double shift = -0.05;
      Double_t shift_z = 0;
      if(fObservable == Observable::kFragmentation)shift_z=-0.4;

      TPaveText *pvJet = new TPaveText(0.52+shift_z,0.65-shift,0.9+shift_z,0.7-shift,"brNDC");
      pvJet->SetFillStyle(0);
      pvJet->SetBorderSize(0);
      pvJet->SetTextFont(42);
      pvJet->SetTextSize(0.04f);
      pvJet->SetTextAlign(11);
      pvJet->AddText(Form("Charged Jets, Anti-#it{k}_{T}, #it{R} = 0.%d",Rpar));

      shift+=0.07;
      TPaveText *pvD = new TPaveText(0.52+shift_z,0.65-shift,0.9+shift_z,0.7-shift,"brNDC");
      pvD->SetFillStyle(0);
      pvD->SetBorderSize(0);
      pvD->SetTextFont(42);
      pvD->SetTextSize(0.04f);
      pvD->SetTextAlign(11);
      if(fDmesonSpecie) pvD->AddText("with D^{*+} #rightarrow D^{0}#pi^{+} and charge conj.");
      else pvD->AddText("with D^{0} #rightarrow K^{-}#pi^{+} and charge conj.");

      shift+=0.07;
      TPaveText *pvEta = new TPaveText(0.52+shift_z,0.65-shift,0.9+shift_z,0.7-shift,"brNDC");
      pvEta->SetFillStyle(0);
      pvEta->SetBorderSize(0);
      pvEta->SetTextFont(42);
      pvEta->SetTextSize(0.04f);
      pvEta->SetTextAlign(11);
      pvEta->AddText(Form("|#it{#eta}_{jet}| < 0.%d",9-Rpar));

      shift+=0.07;
      TPaveText *pv3 = nullptr;
      if(fObservable==Observable::kXsection) pv3 = new TPaveText(0.52+shift_z,0.65-shift,0.9+shift_z,0.7-shift,"brNDC");
      if(fObservable==Observable::kFragmentation) pv3 = new TPaveText(0.52+shift_z,0.5-shift,0.9+shift_z,0.7-shift,"brNDC");
      pv3->SetFillStyle(0);
      pv3->SetBorderSize(0);
      pv3->SetTextFont(42);
      pv3->SetTextSize(0.04f);
      pv3->SetTextAlign(11);
      pv3->AddText(Form("%d < p_{T,%s} < %d GeV/#it{c}",static_cast<Int_t>(ptbinsDA_[0]),fDmesonS.Data(),static_cast<Int_t>(ptbinsDA_[ptbinsDN_])));
      if(fObservable==Observable::kFragmentation)pv3->AddText(Form("%d < p_{T,ch. jet} < %d",static_cast<Int_t>(fzptJetMeasA[fBin-1]),static_cast<Int_t>(fzptJetMeasA[fBin])));

       // before eff. correction
      TCanvas *cRawSpectrum = new TCanvas("cRawSpectrum","cRawSpectrum",800,600);
      setHistoDetails(hrawjetptspectrum,0,kBlue+1,20,static_cast<Size_t>(1.2));

      if(fObservable == Observable::kXsection){
        hrawjetptspectrum->GetXaxis()->SetTitle("p_{T}^{ch,jet} (GeV/c)");
        hrawjetptspectrum->GetYaxis()->SetTitle("Raw dN/dp_{T}");
        hrawjetptspectrum->GetXaxis()->SetRangeUser(jetplotmin,jetplotmax);
      }
      else if(fObservable == Observable::kFragmentation){
        hrawjetptspectrum->GetYaxis()->SetTitle("dN/dz_{#parallel}");
        hrawjetptspectrum->GetXaxis()->SetTitle("z_{#parallel}");
        hrawjetptspectrum->GetXaxis()->SetRangeUser(fzbinsJetMeasA[fBin-1][0],fzbinsJetMeasA[fBin-1][fzbinsJetMeasN[fBin-1]]);
      }
      hrawjetptspectrum->Draw();
      if(isdetails) pvProd->Draw("same");
      if(isdetails) pvCuts->Draw("same");
      pvEn->Draw("same");

      if(fObservable == Observable::kXsection){
        hjetptspectrum->GetYaxis()->SetTitle("dN/dp_{T}");
        hjetptspectrum->GetXaxis()->SetTitle("p_{T,ch jet} (GeV/c)");
        hjetptspectrum->GetXaxis()->SetRangeUser(jetplotmin,jetplotmax);
      }
      else if(fObservable == Observable::kFragmentation){
        hjetptspectrum->GetYaxis()->SetTitle("dN/dz_{#parallel}");
        hjetptspectrum->GetXaxis()->SetTitle("z_{#parallel}");
        hjetptspectrum->GetXaxis()->SetRangeUser(fzbinsJetMeasA[fBin-1][0],fzbinsJetMeasA[fBin-1][fzbinsJetMeasN[fBin-1]]);
      }

      hjetptspectrum->SetMinimum(1);

      TCanvas *cSpectrum = new TCanvas("cSpectrum","cSpectrum",800,600);
      if(fObservable == Observable::kXsection) cSpectrum->SetLogy();
      setHistoDetails(hjetptspectrum,0,kRed,20,static_cast<Size_t>(1.2));
      if(fObservable == Observable::kFragmentation)hjetptspectrum->GetXaxis()->SetTitle("z_{#parallel}");


      hjetptspectrum->Draw();
      pv3->Draw("same");
      pvEn->Draw("same");
      pvD->Draw("same");
      pvJet->Draw("same");
      pvEta->Draw("same");
      if(isdetails) pvProd->Draw("same");
      if(isdetails) pvCuts->Draw("same");
      if(savePlots) SaveCanvas(cSpectrum,outdir+plotsDir+"/jetPtSpectrum_SB"+prod);
      //fzbinsJetTrueA[fzbinsJetTrueN+1]

      TH1F *hjetptspectrumReb_tmp = static_cast<TH1F*>(hjetptspectrum->Clone("hjetptspectrumReb_tmp"));
      if(fObservable == Observable::kXsection){
        hjetptspectrumReb = static_cast<TH1F*>(hjetptspectrumReb_tmp->Rebin(fptbinsJetMeasN,"hjetptspectrumReb",fptbinsJetMeasA));
        hjetptspectrumRebScaled = static_cast<TH1F*>(hjetptspectrumReb_tmp->Rebin(fptbinsJetMeasN,"hjetptspectrumRebScaled",fptbinsJetMeasA));
        hjetptspectrumRebEvntScaled = static_cast<TH1F*>(hjetptspectrumReb_tmp->Rebin(fptbinsJetMeasN,"hjetptspectrumRebEvntScaled",fptbinsJetMeasA));
      }
      else if(fObservable == Observable::kFragmentation){
        hjetptspectrumReb = static_cast<TH1F*>(hjetptspectrumReb_tmp->Rebin(fzbinsJetMeasN[fBin-1],"hjetptspectrumReb",fzbinsJetMeasA[fBin-1]));
        hjetptspectrumRebScaled = static_cast<TH1F*>(hjetptspectrumReb_tmp->Rebin(fzbinsJetMeasN[fBin-1],"hjetptspectrumRebScaled",fzbinsJetMeasA[fBin-1]));
        hjetptspectrumRebEvntScaled = static_cast<TH1F*>(hjetptspectrumReb_tmp->Rebin(fzbinsJetMeasN[fBin-1],"hjetptspectrumRebEvntScaled",fzbinsJetMeasA[fBin-1]));
      }
      hjetptspectrumRebEvntScaled->Scale(1./nEvents);
      setHistoDetails(hjetptspectrumReb,0,kBlue,20,static_cast<Size_t>(1.2)); // with bin width scaling
      setHistoDetails(hjetptspectrumRebScaled,1,kBlue,20,static_cast<Size_t>(1.2)); // with bin width scaling
      setHistoDetails(hjetptspectrumRebEvntScaled,1,kBlue,20,static_cast<Size_t>(1.2)); // with bin width scaling
      setHistoDetails(hSignalEffcor,0,kBlue,20,static_cast<Size_t>(1.2));
      if(fObservable == Observable::kXsection){
        hjetptspectrumReb->GetXaxis()->SetTitle("p_{T,ch jet} (GeV/c)");
        hjetptspectrumRebScaled->GetXaxis()->SetTitle("p_{T,ch jet} (GeV/c)");
        hjetptspectrumRebEvntScaled->GetXaxis()->SetTitle("p_{T,ch jet} (GeV/c)");
      }
      else if(fObservable == Observable::kFragmentation){
        hjetptspectrumReb->GetXaxis()->SetTitle("z_{#parallel}");
        hjetptspectrumRebScaled->GetXaxis()->SetTitle("z_{#parallel}");
        hjetptspectrumRebEvntScaled->GetXaxis()->SetTitle("z_{#parallel}");
      }
      TCanvas *cSpectrumRebin = new TCanvas("cSpectrumRebin","cSpectrumRebin",800,600);
      if(fObservable == Observable::kXsection) cSpectrumRebin->SetLogy();
      hjetptspectrumRebScaled->Draw();
      pvEn->Draw("same");
      pvD->Draw("same");
      pvJet->Draw("same");
      pvEta->Draw("same");
      if(isdetails) pvProd->Draw("same");
      if(isdetails) pvCuts->Draw("same");
      pv3->Draw("same");

      SaveCanvas(cSpectrumRebin,outdir+plotsDir+"/jetPtSpectrum_SB_Rebin"+prod);

      hjetptspectrumRebUnc = static_cast<TH1F*>(hjetptspectrumReb->Clone("hjetptspectrumRebUnc"));
      hjetptspectrumRebUnc->GetYaxis()->SetTitle("Rel. unc.");


      for(int j=1; j<=hjetptspectrumReb->GetNbinsX();j++){
                  double err;
                  if(static_cast<Bool_t>(hjetptspectrumReb->GetBinContent(j))) err = hjetptspectrumReb->GetBinError(j)/hjetptspectrumReb->GetBinContent(j);
                  else err = 0;
                  hjetptspectrumRebUnc->SetBinContent(j,err);
                  hjetptspectrumRebUnc->SetBinError(j,0);
      }

      hjetptspectrumRebUnc->SetMinimum(0);
      hjetptspectrumRebUnc->SetMaximum(hjetptspectrumRebUnc->GetMaximum()*1.2);

      shift = 0;
      pvJet = new TPaveText(0.15-shift_z,0.65-shift,0.9,0.7-shift,"brNDC");
      pvJet->SetFillStyle(0);
      pvJet->SetBorderSize(0);
      pvJet->SetTextFont(42);
      pvJet->SetTextSize(0.04f);
      pvJet->SetTextAlign(11);
      if(fDmesonSpecie) pvJet->AddText("D^{*+} #rightarrow D^{0}#pi^{+} and charge conj.");
      else pvJet->AddText(Form("Charged Jets, Anti-#it{k}_{T}, #it{R} = 0.%d",Rpar));

      pvD = new TPaveText(0.15-shift_z,0.58-shift,0.9,0.63-shift,"brNDC");
      pvD->SetFillStyle(0);
      pvD->SetBorderSize(0);
      pvD->SetTextFont(42);
      pvD->SetTextSize(0.04f);
      pvD->SetTextAlign(11);
      if(fDmesonSpecie) pvD->AddText("with D^{*+} #rightarrow D^{0}#pi^{+} and charge conj.");
      else pvD->AddText("with D^{0} #rightarrow K^{-}#pi^{+} and charge conj.");

      pvEta = new TPaveText(0.15-shift_z,0.51-shift,0.8,0.56-shift,"brNDC");
      pvEta->SetFillStyle(0);
      pvEta->SetBorderSize(0);
      pvEta->SetTextFont(42);
      pvEta->SetTextSize(0.04f);
      pvEta->SetTextAlign(11);
      pvEta->AddText(Form("|#it{#eta}_{jet}| < 0.%d",9-Rpar));

      if(fObservable==Observable::kXsection) pv3 = new TPaveText(0.15-shift_z,0.44,0.9,0.49,"brNDC");
      if(fObservable==Observable::kFragmentation) pv3 = new TPaveText(0.15-shift_z,0.29,0.9,0.49,"brNDC");
      pv3->SetFillStyle(0);
      pv3->SetBorderSize(0);
      pv3->SetTextFont(42);
      pv3->SetTextSize(0.04f);
      pv3->SetTextAlign(11);
      pv3->AddText(Form("%d < p_{T,%s} < %d GeV/#it{c}",static_cast<Int_t>(ptbinsDA_[0]),fDmesonS.Data(),static_cast<Int_t>(ptbinsDA_[ptbinsDN_])));
      if(fObservable==Observable::kFragmentation)pv3->AddText(Form("%d < p_{T,ch. jet} < %d",static_cast<Int_t>(fzptJetMeasA[fBin-1]),static_cast<Int_t>(fzptJetMeasA[fBin])));

      TCanvas *cSpectrumRebinUnc = new TCanvas("cSpectrumRebinUnc","cSpectrumRebinUnc",800,500);

      //hjetptspectrumRebUnc->GetXaxis()->SetRangeUser(jetplotmin,jetplotmax);
      hjetptspectrumRebUnc->Draw();
      if(isdetails) pvProd->Draw("same");
      if(isdetails) pvCuts->Draw("same");
      pvEn->Draw("same");
      pv3->Draw("same");
      pvD->Draw("same");
      pvJet->Draw("same");
      pvEta->Draw("same");

      SaveCanvas(cSpectrumRebinUnc,outdir+plotsDir+"/jetPtSpectrumUnc_SB_Rebin"+prod);

}


void  saveFitParams(TString outdir,TString prod){

    TPaveText *pvProd = new TPaveText(0.78,0.75,0.9,0.8,"brNDC");
    pvProd->SetFillStyle(0);
    pvProd->SetBorderSize(0);
    pvProd->AddText(Form("%s",prod.Data()));

    TPaveText *pvCuts = new TPaveText(0.78,0.8,0.9,0.85,"brNDC");
    pvCuts->SetFillStyle(0);
    pvCuts->SetBorderSize(0);
    pvCuts->AddText(Form("%s",outdir.Data()));

    TPaveText *pvEn= new TPaveText(0.25,0.80,0.8,0.85,"brNDC");
    pvEn->SetFillStyle(0);
    pvEn->SetBorderSize(0);
    pvEn->SetTextFont(42);
    pvEn->SetTextSize(0.045f);
    pvEn->SetTextAlign(11);
    pvEn->AddText(Form("%s",fSystemS.Data()));

    setHistoDetails(hmean,0,2,20);
    if(fDmesonSpecie) hmean->GetYaxis()->SetTitle("signal #mu (MeV/c^{2})");
    else hmean->GetYaxis()->SetTitle("signal #mu (GeV/c^{2})");
    setHistoDetails(hsigma,0,4,20);
    hsigma->GetYaxis()->SetTitle("signal #sigma (MeV/c^{2})");
    setHistoDetails(hsign,0,4,20);
    hsign->GetYaxis()->SetTitle("significance");
    setHistoDetails(hsb,0,8,20);
    hsb->GetYaxis()->SetTitle("S/B");
    setHistoDetails(hrelErr,0,6,20);
    hrelErr->GetYaxis()->SetTitle("rel. unc.");

    hmean->GetYaxis()->SetTitleOffset(1.8f);


    if(fDmesonSpecie) {
      hmean->GetYaxis()->SetRangeUser(144.8,146);
      hsigma->GetYaxis()->SetRangeUser(0.35,0.8);
    }
    else {
      if(fObservable == Observable::kXsection){
        hmean->GetYaxis()->SetRangeUser(1.855,1.885);
        hsigma->GetYaxis()->SetRangeUser(9,20);
      }
      if(fObservable == Observable::kFragmentation){
          hmean->GetYaxis()->SetRangeUser(1.845,1.885);
          hsigma->GetYaxis()->SetRangeUser(9,35);
      }
    }

    hSignal->SetName("hSignal");
    setHistoDetails(hSignal,0,2,20);
    hSignal->GetYaxis()->SetTitle("yield");
    hSignal->GetYaxis()->SetTitleOffset(1.6f);
    hrelErr->GetYaxis()->SetTitleOffset(1.6f);


    double mean = 145.421;
    TLine *lm = new TLine(ptbinsDA_[0], mean, ptbinsDA_[ptbinsDN_], mean);
    lm->SetLineStyle(2);

    TCanvas *cMassFit = new TCanvas("cMassFit","cMassFit",1200,600);
    cMassFit->Divide(2,1);
    cMassFit->cd(1);
    gPad->SetLeftMargin(0.15f);
    hmean->Draw();
    pvEn->Draw("same");
    if(isdetails) pvProd->Draw("same");
    if(isdetails) pvCuts->Draw("same");
    lm->Draw("same");
    cMassFit->cd(2);
    hsigma->Draw();
    if(isdetails) pvProd->Draw("same");
    if(isdetails) pvCuts->Draw("same");

     if(savePlots)  SaveCanvas(cMassFit,outdir+plotsDir+"/gaussianParams"+prod);

    TCanvas *cSignal = new TCanvas("cSignal","cSignal",1200,1200);
    cSignal->Divide(2,2);

    cSignal->cd(1);
    hSignal->Draw();
    if(isdetails) pvProd->Draw("same");
    if(isdetails) pvCuts->Draw("same");
    cSignal->cd(2);
    hsign->Draw();
    pvEn->Draw("same");
    if(isdetails) pvProd->Draw("same");
    if(isdetails) pvCuts->Draw("same");
    cSignal->cd(3);
    hrelErr->Draw();
    if(isdetails) pvProd->Draw("same");
    if(isdetails) pvCuts->Draw("same");
    cSignal->cd(4);
    hsb->Draw();
    if(isdetails) pvProd->Draw("same");
    if(isdetails) pvCuts->Draw("same");

    if(savePlots) SaveCanvas(cSignal,outdir+plotsDir+"/signalParams"+prod);

    if(hReflRS && fUseRefl && fDmesonSpecie == 0) {
      setHistoDetails(hReflRS,0,kGreen+2,20);
      hReflRS->GetYaxis()->SetTitle("R/S");
      hReflRS->SetMinimum(hReflRS->GetMinimum()*0.5);
      hReflRS->SetMaximum(hReflRS->GetMaximum()*1.2);
      TCanvas *cRS = new TCanvas("cRS","cRS",800,600);
      cRS->cd();
      hReflRS->Draw();
      if(savePlots) SaveCanvas(cRS,outdir+plotsDir+"/RefOverS"+prod);
    }

}
void setHistoDetails(TH1 *h, int scale, Color_t color, Style_t Mstyle, Size_t size, Width_t width){

    if(scale==1)h->Scale(1,"width");
    else if(scale==2){
        h->Scale(1./h->Integral());
        h->Scale(1,"width");
    }
    h->SetMarkerStyle(Mstyle);
    h->SetMarkerColor(color);
    h->SetMarkerSize(size);
    h->SetLineColor(color);
    h->SetLineWidth(width);
    h->SetTitle("");
    h->GetXaxis()->SetTitle(Form("p_{T,%s}(GeV/c)",fDmesonS.Data()));


    return;
}

void SaveCanvas(TCanvas *c, TString name){

    c->SaveAs(Form("%s_pTD%d.png",name.Data(),static_cast<Int_t>(ptbinsDA_[0])));
    c->SaveAs(Form("%s_pTD%d.pdf",name.Data(),static_cast<Int_t>(ptbinsDA_[0])));
    c->SaveAs(Form("%s_pTD%d.svg",name.Data(),static_cast<Int_t>(ptbinsDA_[0])));
}

void setStyle(){


}
