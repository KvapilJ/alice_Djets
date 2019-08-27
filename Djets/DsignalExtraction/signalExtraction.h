#ifndef SIGNALEXTRACTION_H
#define SIGNALEXTRACTION_H

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

//
//-----------------------------------------------------------------------
//  Author B.Trzeciak
//  Utrecht University
//  barbara.antonina.trzeciak@cern.ch
//-----------------------------------------------------------------------


#include "config.h"

    Double_t jetmin, jetmax;
    Double_t jetplotmin, jetplotmax;

    Bool_t isEta = 0;
    Double_t jetEta = 0.4;


    Bool_t savePlots = 1;
    Bool_t bEff = 0;
    Bool_t isptcut = 1;
    Bool_t isdetails = 0;

    Bool_t fisRefSys;
    Double_t frefScale;

    TString fReflFilename;
    TString plotsDir;
    // mass fit params

    //------- efficiency
    Double_t *efficiency = nullptr;

    Double_t nEvents = 0;

    Int_t ptbinsDN_;
    Double_t *ptbinsDA_;

    TH1F** hmass;
    TH1F **hmass_l;
    TH1F **hmass_u;
    TH1F **hmass_c;
    TF1** fullfit;
    TF1** massfit;
    TF1** bkgfit;
    TF1** bkgRfit;

    TH1F** hjetpt;
    TH1F**hjetpt_sb;
    TH1F **hjetptsub;
    TH1F **hjetptcorr;

    TH1F *hjetptspectrum;
    TH1F *hrawjetptspectrum;
    TH1F *hjetptspectrumReb;
    TH1F *hjetptspectrumRebScaled;
    TH1F *hjetptspectrumRebEvntScaled;
    TH1F *hjetptspectrumRebUnc;

    TH1F *hmean;
    TH1F *hsigma;
    TH1F *hrelErr;
    TH1F *hsign;
    TH1F *hsb;
    TH1F *hSignal;
    TH3D* hInvMassptD;
    TH1F* hReflRS;

    TH1F *hSignalEffcor;
    Double_t reflSig, reflSigErr;


    void setHistoDetails(TH1 *h, int scale, Color_t color, Style_t Mstyle, Size_t size = 0.9f, Width_t width=2);
    Bool_t rawJetSpectra(TString outdir, TString prod);
    void  saveSpectraPlots(TString outdir,TString prod);
    void  saveFitParams(TString outdir,TString prod);
    Bool_t SetReflection(AliHFInvMassFitter* &fitter, Float_t fLeftFitRange, Float_t fRightFitRange, Float_t &RS, Int_t iBin);
    Bool_t SetReflection(AliHFInvMassFitter* &fitter, Float_t fLeftFitRange, Float_t fRightFitRange, Float_t &RS, Int_t ptmin, Int_t ptmax);
    void SaveCanvas(TCanvas *c, TString name = "tmp");

#endif
