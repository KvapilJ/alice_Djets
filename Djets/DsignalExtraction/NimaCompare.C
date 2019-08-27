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


#include <iostream>
#include <fstream>
#include "TObject.h"
#include "TMath.h"
#include "TFile.h"
#include "TDirectoryFile.h"
#include "TList.h"
#include "TCanvas.h"
#include "TPaveText.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TSystem.h"
#include "TRandom3.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TF1.h"
#include "THnSparse.h"
#include "TDatabasePDG.h"
#include "AliNormalizationCounter.h"
#include "TStyle.h"
#include "AliHFInvMassFitter.h"
#include "TProfile.h"
#include "AliAnalysisTaskDmesonJets.h"
#include "THStack.h"
#include "TSVDUnfold.h"
#include "TGraphAsymmErrors.h"
#include "TLegendEntry.h"
#include "TPad.h"



void NimaCompare(
  TString data = "/mnt/hgfs/vmware/data_250319/data/AnalysisResults_Run2.root"
)
{
    TString histName = "histosD0MBN";
    // get analysis output file
    TString datafile;
    TFile *File;
    TDirectoryFile* dir;
    TDirectoryFile *dir2;
    TList *histList;
    THnSparseF *sparse;

      datafile = data;
      File = new TFile(datafile,"read");
      if(!File) { std::cout << "==== WRONG FILE WITH DATA =====\n\n"; return ;}
      dir=dynamic_cast<TDirectoryFile*>(File->Get("DmesonsForJetCorrelations"));

      TH1D *hJetPt = nullptr;// = new TH1D("hJetPt","Jet pT",45,5,50);
      TH1D *hDPt = nullptr;// = new TH1D("hDPt","D pT",34,2,36);
      TH1D *hDmass = nullptr;// = new TH1D("hDmass","D mass",40,1.7,2.1);
      TH1D *hJetEta = nullptr;// = new TH1D("hJetEta","Jet eta",10,-0.5,0.5);
      TH1D *hDy = nullptr;// = new TH1D("hDy","D y",12,-0.6,0.6);

      Double_t jet[45+1] = {5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50};
      Double_t D[34+1] = {2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36};
      Double_t mass[40+1] = {1.7,1.71,1.72,1.73,1.74,1.75,1.76,1.77,1.78,1.79,
                           1.8,1.81,1.82,1.83,1.84,1.85,1.86,1.87,1.88,1.89,
                           1.9,1.91,1.92,1.93,1.94,1.95,1.96,1.97,1.98,1.99,
                           2.0,2.01,2.02,2.03,2.04,2.05,2.06,2.07,2.08,2.09,
                           2.1};
      Double_t eta[10+1] = {-0.5,-0.4,-0.3,-0.2,-0.1,0,0.1,0.2,0.3,0.4,0.5};
      Double_t y[12+1] = {-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,0,0.1,0.2,0.3,0.4,0.5,0.6};


      for(int i=0;i<3; i++){
          histList =  dynamic_cast<TList*>(dir->Get(Form("%s%d",histName.Data(),i)));

          sparse = dynamic_cast<THnSparseF*>(histList->FindObject("hsDphiz"));
          sparse->GetAxis(1)->SetRangeUser(5,50);  //jet pt
          sparse->GetAxis(2)->SetRangeUser(2,36); //D pt    3- mass 4 - y D, 5 - eta jet
          sparse->GetAxis(5)->SetRangeUser(-0.5,0.5); //Nima
          if(i==0){
              hJetPt=dynamic_cast<TH1D*>(sparse->Projection(1));
              hDPt=dynamic_cast<TH1D*>(sparse->Projection(2));
              hDmass=dynamic_cast<TH1D*>(sparse->Projection(3));
              hJetEta=dynamic_cast<TH1D*>(sparse->Projection(5));
              hDy=dynamic_cast<TH1D*>(sparse->Projection(4));
          }
          else{
              hJetPt->Add(dynamic_cast<TH1D*>(sparse->Projection(1)));
              hDPt->Add(dynamic_cast<TH1D*>(sparse->Projection(2)));
              hDmass->Add(dynamic_cast<TH1D*>(sparse->Projection(3)));
              hJetEta->Add(dynamic_cast<TH1D*>(sparse->Projection(5)));
              hDy->Add(dynamic_cast<TH1D*>(sparse->Projection(4)));
          }

      }


      TH1D *hJetPt2;// = new TH1D("hJetPt","Jet pT",45,5,50);
      TH1D *hDPt2;// = new TH1D("hDPt","D pT",34,2,36);
      TH1D *hDmass2;// = new TH1D("hDmass","D mass",40,1.7,2.1);
      TH1D *hJetEta2;// = new TH1D("hJetEta","Jet eta",10,-0.5,0.5);
      TH1D *hDy2;// = new TH1D("hDy","D y",12,-0.6,0.6);

     // hJetPt2 = static_cast<TH1D*>(hJetPt->Rebin(45,"hJetPt2",jet));
      hDPt2 = static_cast<TH1D*>(hDPt->Rebin(34,"hDPt2",D));
      hDmass2 = static_cast<TH1D*>(hDmass->Rebin(40,"hDmass2",mass));
      hJetEta2 = static_cast<TH1D*>(hJetEta->Rebin(10,"hJetEta2",eta));
      hDy2 = static_cast<TH1D*>(hDy->Rebin(12,"hDy2",y));

      hJetPt->SetName("hJetPt");
      hJetPt->SetTitle("Jet pT");
      hDPt2->SetTitle("D pT");
      hDmass2->SetTitle("D mass");
      hJetEta2->SetTitle("Jet eta");
      hDy2->SetTitle("D y");



    TFile *ofile = new TFile("dataPlots.root","RECREATE");
    hJetPt->Write();
    hDPt2->Write();
    hDmass2->Write();
    hJetEta2->Write();
    hDy2->Write();

    ofile->Close();
    // --------------------------------------------------------

}
