//
//  Execute with:
  //  gSystem->SetIncludePath("-I. -I$ALICE_ROOT/include -I$ALICE_PHYSICS/include -I$ALICE_ROOT/ANALYSIS/macros -I$ROOTSYS/include");
//  gROOT->LoadMacro("AliDJetRawYieldUncertaintyLocal.cxx++")
//  .L ExtractDJetRawYieldUncertainty.C
//  EvaluateBinPerBinUncertainty(...) //to be done for each pT bin in which you have a mass spectrum
//  ExtractDJetRawYieldUncertainty(...) //to build the uncertainty for the various bins of the jet pT spectrum

#include "AliDJetRawYieldUncertaintyLocal.h"

double sigmajet[] = {0,0};

//R=0.2 Auro cut
//jet pT 5-7
/*
const int ptbinsDN = 5;
Int_t nDbins = ptbinsDN;
double ptDbins[ptbinsDN+1] = {2,3,4,5,6,7};
Int_t nJetbins = 5;
Double_t ptJetbins[6] = {0.4,0.6,0.7,0.8,0.9,1.02};
double sigmaD[ptbinsDN] = {0.0102078,0.0106905,0.0113938,0.011917,0.0124129};
TString efffile = "/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_z02F/Default_AnalysisResults_Run2w18b.root/efficiency/DjetEff_prompt_jetpt5.00_7.00.root";
*/
//jet pT 7-10
/*
const int ptbinsDN = 5;
Int_t nDbins = ptbinsDN;
double ptDbins[ptbinsDN+1] = {4,5,6,7,8,10};
Int_t nJetbins = 5;
Double_t ptJetbins[6] = {0.4,0.6,0.7,0.8,0.9,1.02};
double sigmaD[ptbinsDN] = {0.0110206,0.0118391,0.0125744,0.0129398,0.0135101};
TString efffile = "/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_z02F/Default_AnalysisResults_Run2w18b.root/efficiency/DjetEff_prompt_jetpt7.00_10.00.root";
*/
//jet pT 10-15
/*
const int ptbinsDN = 6;
Int_t nDbins = ptbinsDN;
double ptDbins[ptbinsDN+1] = {5,6,7,8,10,12,15};
Int_t nJetbins = 5;
Double_t ptJetbins[6] = {0.4,0.6,0.7,0.8,0.9,1.02};
double sigmaD[ptbinsDN] = {0.0119677,0.0127522,0.0129833,0.0138348,0.0146075,0.015442};
TString efffile = "/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_z02F/Default_AnalysisResults_Run2w18b.root/efficiency/DjetEff_prompt_jetpt10.00_15.00.root";
*/
//jet pT 15-50
/*
const int ptbinsDN = 4;
Int_t nDbins = ptbinsDN;
double ptDbins[ptbinsDN+1] = {10,12,16,24,36};
Int_t nJetbins = 5;
Double_t ptJetbins[6] = {0.4,0.6,0.7,0.8,0.9,1.02};
double sigmaD[ptbinsDN] = {0.0147802,0.0160004,0.0177687,0.0206198};
TString efffile = "/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_z02F/Default_AnalysisResults_Run2w18b.root/efficiency/DjetEff_prompt_jetpt15.00_50.00.root";
*/


//R=0.4 auro
//jet pT 5-7
/*
const int ptbinsDN = 5;
Int_t nDbins = ptbinsDN;
double ptDbins[ptbinsDN+1] = {2,3,4,5,6,7};
Int_t nJetbins = 5;
Double_t ptJetbins[6] = {0.4,0.6,0.7,0.8,0.9,1.02};
double sigmaD[ptbinsDN] = {0.00981067,0.0107636,0.0114343,0.0120319,0.0125213};
TString efffile = "/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_z04F/Default_AnalysisResults_Run2w18b.root/efficiency/DjetEff_prompt_jetpt5.00_7.00.root";
*/
//jet pT 7-10
/*
const int ptbinsDN = 6;
Int_t nDbins = ptbinsDN;
double ptDbins[ptbinsDN+1] = {3,4,5,6,7,8,10};
Int_t nJetbins = 5;
Double_t ptJetbins[6] = {0.4,0.6,0.7,0.8,0.9,1.02};
double sigmaD[ptbinsDN] = {0.0107507,0.0112994,0.0119961,0.0126252,0.0130566,0.0136451};
TString efffile = "/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_z04F/Default_AnalysisResults_Run2w18b.root/efficiency/DjetEff_prompt_jetpt7.00_10.00.root";
*/
//jet pT 10-15
/*
const int ptbinsDN = 6;
Int_t nDbins = ptbinsDN;
double ptDbins[ptbinsDN+1] = {5,6,7,8,10,12,15};
Int_t nJetbins = 5;
Double_t ptJetbins[6] = {0.4,0.6,0.7,0.8,0.9,1.02};
double sigmaD[ptbinsDN] = {0.0120098,0.0127895,0.0131533,0.0138485,0.0148092,0.0154531};
TString efffile = "/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_z04F/Default_AnalysisResults_Run2w18b.root/efficiency/DjetEff_prompt_jetpt10.00_15.00.root";
*/
//jet pT 15-50
/*
const int ptbinsDN = 8;
Int_t nDbins = ptbinsDN;
double ptDbins[ptbinsDN+1] = {5,6,7,8,10,12,16,24,36};
Int_t nJetbins = 5;
Double_t ptJetbins[6] = {0.4,0.6,0.7,0.8,0.9,1.02};
double sigmaD[ptbinsDN] = {0.0119613,0.0124824,0.0122888,0.0138271,0.0145933,0.0159077,0.0178557,0.0208333};
TString efffile = "/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_z04F/Default_AnalysisResults_Run2w18b.root/efficiency/DjetEff_prompt_jetpt15.00_50.00.root";
*/


//R=0.6 Auro cut
//jet pT 5-7
/*
const int ptbinsDN = 5;
Int_t nDbins = ptbinsDN;
double ptDbins[ptbinsDN+1] = {2,3,4,5,6,7};
Int_t nJetbins = 5;
Double_t ptJetbins[6] = {0.4,0.6,0.7,0.8,0.9,1.02};
double sigmaD[ptbinsDN] = {0.0101014,0.0109773,0.0116498,0.0123234,0.0126684};
TString efffile = "/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_z06F/Default_AnalysisResults_Run2w18b.root/efficiency/DjetEff_prompt_jetpt5.00_7.00.root";
*/
//jet pT 7-10
/*
const int ptbinsDN = 6;
Int_t nDbins = ptbinsDN;
double ptDbins[ptbinsDN+1] = {3,4,5,6,7,8,10};
Int_t nJetbins = 5;
Double_t ptJetbins[6] = {0.4,0.6,0.7,0.8,0.9,1.02};
double sigmaD[ptbinsDN] = {0.0108824,0.0116365,0.0123407,0.0129615,0.0133065,0.0135919};
TString efffile = "/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_z06F/Default_AnalysisResults_Run2w18b.root/efficiency/DjetEff_prompt_jetpt7.00_10.00.root";
*/
//jet pT 10-15
/*
const int ptbinsDN = 6;
Int_t nDbins = ptbinsDN;
double ptDbins[ptbinsDN+1] = {5,6,7,8,10,12,15};
Int_t nJetbins = 5;
Double_t ptJetbins[6] = {0.4,0.6,0.7,0.8,0.9,1.02};
double sigmaD[ptbinsDN] = {0.0123871,0.0129366,0.0132054,0.0140493,0.0150104,0.0157983};
TString efffile = "/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_z06F/Default_AnalysisResults_Run2w18b.root/efficiency/DjetEff_prompt_jetpt10.00_15.00.root";
*/
//jet pT 15-50
/*
const int ptbinsDN = 8;
Int_t nDbins = ptbinsDN;
double ptDbins[ptbinsDN+1] = {5,6,7,8,10,12,16,24,36};
Int_t nJetbins = 5;
Double_t ptJetbins[6] = {0.4,0.6,0.7,0.8,0.9,1.02};
double sigmaD[ptbinsDN] = {0.0116297,0.0126963,0.0132221,0.0143956,0.0147155,0.0160374,0.0178958,0.0211222};
TString efffile = "/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_z06F/Default_AnalysisResults_Run2w18b.root/efficiency/DjetEff_prompt_jetpt15.00_50.00.root";
*/

//R=0.4 Lc
//jet pT 7-15

const int ptbinsDN = 5;
Int_t nDbins = ptbinsDN;
double ptDbins[ptbinsDN+1] = {3,4,6,8,12,15};
Int_t nJetbins = 5;
Double_t ptJetbins[6] = {0.4,0.6,0.7,0.8,0.9,1.02};
double sigmaD[ptbinsDN] = {0.0107383,0.0117579,0.0128768,0.014076,0.0154531};
TString efffile = "/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts/Default_AnalysisResults_Run2w18b.root/efficiency/DjetEff_prompt_jetpt7.00_15.00.root";


// forward declaration
void EvaluateBinPerBinUncertainty(
   Int_t bin = 0,
   AliDJetRawYieldUncertaintyLocal::DMesonSpecies specie=AliDJetRawYieldUncertaintyLocal::kD0toKpi,  //D-meson decay channel
   AliDJetRawYieldUncertaintyLocal::YieldMethod method=AliDJetRawYieldUncertaintyLocal::kSideband,  //yield extraction method
   Double_t zmin=-1.,   //lower z edge
   Double_t zmax=2.,   //upper z edge
   Bool_t refl=kFALSE
   );
void ExtractDJetRawYieldUncertaintyFull(
   AliDJetRawYieldUncertaintyLocal::DMesonSpecies specie=AliDJetRawYieldUncertaintyLocal::kD0toKpi,  //D-meson decay channel
   AliDJetRawYieldUncertaintyLocal::YieldMethod  method=AliDJetRawYieldUncertaintyLocal::kSideband,  //yield extraction method
   Int_t nTrials=36,  	     //only for SB method: number of random trials for each pT(D) bin to build pT(jet) spectrum variations
   Bool_t allowRepet=kFALSE  //only for SB method: allow repetitions in the extraction of trials in a give pT(D) bin
   );
void ExtractDJetRawYieldUncertainty_FromSB_CoherentTrialChoice(
  AliDJetRawYieldUncertaintyLocal::DMesonSpecies specie=AliDJetRawYieldUncertaintyLocal::kD0toKpi,  //D-meson decay channel
   Int_t nTrials=10
   );
void SetInputParametersDzero(AliDJetRawYieldUncertaintyLocal *interface);
void SetInputParametersDstar(AliDJetRawYieldUncertaintyLocal *interface);


void ExtractDJetRawYieldUncertainty(){
  gROOT->LoadMacro("AliDJetRawYieldUncertaintyLocal.cxx+g");

  for(int i=0; i<ptbinsDN; i++)
    EvaluateBinPerBinUncertainty(i);

  ExtractDJetRawYieldUncertaintyFull();

return;

}

void EvaluateBinPerBinUncertainty(Int_t bin,AliDJetRawYieldUncertaintyLocal::DMesonSpecies specie,AliDJetRawYieldUncertaintyLocal::YieldMethod method,Double_t zmin,Double_t zmax,Bool_t refl)
{

  double ptmin = ptDbins[bin];
  double ptmax = ptDbins[bin+1];

  AliDJetRawYieldUncertaintyLocal *interface = new AliDJetRawYieldUncertaintyLocal();
  Bool_t flagSpecie = interface->SetDmesonSpecie(specie);
  if(!flagSpecie) return;
  interface->SetYieldMethod(method);
  interface->SetPtBinEdgesForMassPlot(ptmin,ptmax);
  interface->SetZedges(zmin,zmax);
  interface->SetFitReflections(refl);

  /*sigmaD = new double[ptbinsDN];
  TFile *FileSigma = new TFile(sigmaFile.Data(),"read");
	TH1F *hSigma = (TH1F*)FileSigma->Get("hsigma");
	for(int i=0;i<ptbinsDN;i++){
		double pt = (ptDbins[i]+ptDbins[i+1]) / 2.;
		sigmaD[i] = hSigma->GetBinContent(hSigma->GetXaxis()->FindBin(pt));
	}*/


  double sigma = 0;
  if(method == AliDJetRawYieldUncertaintyLocal::kEffScale) sigma = sigmajet[bin];
  else sigma = sigmaD[bin];
  Double_t sigmafixed=sigma;
  interface->SetSigmaToFix(sigmafixed);


  if(specie==0) SetInputParametersDzero(interface);  // check the names and the values in the method!!
  else if(specie==1) SetInputParametersDstar(interface);  // check the names and the values in the method!!
  else  {printf("Error in setting the D-meson specie! Exiting...\n"); return;}

  interface->SetDebugLevel(2); //0 = just do the job; 1 = additional printout; 2 = print individual fits

  Bool_t extract = interface->ExtractInputMassPlot();
  if(!extract) {
    printf("Error in extracting the mass plot! Exiting...\n");
    return;
  }

  Bool_t multitrial = interface->RunMultiTrial();
  if(!multitrial) {
    printf("Error in running the MultiTrial code! Exiting...\n");
    return;
  }

  interface->ClearObjects();

  return;
}

//________________________________________
void ExtractDJetRawYieldUncertaintyFull(
  AliDJetRawYieldUncertaintyLocal::DMesonSpecies specie, AliDJetRawYieldUncertaintyLocal::YieldMethod method,Int_t nTrials,Bool_t allowRepet)
{

  AliDJetRawYieldUncertaintyLocal *interface = new AliDJetRawYieldUncertaintyLocal();
  Bool_t flagSpecie = interface->SetDmesonSpecie(specie);
  if(!flagSpecie) return;
  interface->SetYieldMethod(method);
  interface->SetMaxNTrialsForSidebandMethod(nTrials); //only for SB method: number of random trials for each pT(D) bin to build pT(jet) spectrum variations
  interface->SetAllowRepetitionOfTrialExtraction(allowRepet);

  if(specie==0) SetInputParametersDzero(interface);  // here most of the configuration is dummy (not used in the evaluation), you need just the files and some bin ranges
  else if(specie==1) SetInputParametersDstar(interface);  // here most of the configuration is dummy (not used in the evaluation), you need just the files and some bin ranges
  else {printf("Error in setting the D-meson specie! Exiting...\n"); return;}

  interface->SetDebugLevel(2); //0 = just do the job; 1 = additional printout; 2 = print individual fits

  Bool_t evalunc = interface->EvaluateUncertainty();
  if(!evalunc) {
    printf("Error in evaluating the yield uncertainty! Exiting...\n");
    return;
  }

  interface->ClearObjects();

  return;
}

//________________________________________
void ExtractDJetRawYieldUncertainty_FromSB_CoherentTrialChoice(AliDJetRawYieldUncertaintyLocal::DMesonSpecies specie,Int_t nTrials) //number of variations is fixed (all the variations in the pT(D) bins, which should match among the various pT(D) bins!)
{

  AliDJetRawYieldUncertaintyLocal *interface = new AliDJetRawYieldUncertaintyLocal();
  Bool_t flagSpecie = interface->SetDmesonSpecie(specie);
  if(!flagSpecie) return;
  interface->SetYieldMethod(AliDJetRawYieldUncertaintyLocal::kSideband);
  interface->SetMaxNTrialsForSidebandMethod(nTrials);

  if(specie==0) SetInputParametersDzero(interface);  // here most of the configuration is dummy (not used in the evaluation), you need just the files and some bin ranges
  else if(specie==1) SetInputParametersDstar(interface);  // here most of the configuration is dummy (not used in the evaluation), you need just the files and some bin ranges
  else  {printf("Error in setting the D-meson specie! Exiting...\n"); return;}

  interface->SetDebugLevel(2); //0 = just do the job; 1 = additional printout; 2 = print individual fits

  Bool_t evalunc = interface->EvaluateUncertainty_CoherentTrialChoice();
  if(!evalunc) {
    printf("Error in evaluating the yield uncertainty! Exiting...\n");
    return;
  }

  interface->ClearObjects();

  return;
}

//________________________________________
void SetInputParametersDzero(AliDJetRawYieldUncertaintyLocal *interface){

  //Dstar cfg
  //Int_t nDbins = 5;
  //Double_t ptDbins[12] = {2,3,4,5,6,7};

 // Int_t nJetbins = 6;
 // Double_t ptJetbins[12] = {0.4,0.5,0.6,0.7,0.8,0.9,1.02};
// Pb-Pb binning
//  Int_t nJetbins = 7;
//  Double_t ptJetbins[8] = {3,5,10,15,20,25,35,50};

  Double_t DMesonEff[ptbinsDN];
  TFile *FileEff = new TFile(efffile.Data(),"read");
  if(!FileEff) return;
    TH1D *hEff = dynamic_cast<TH1D*>(FileEff->Get("hEff_reb"));
	for(int i=0;i<ptbinsDN;i++){
		double pt = (ptDbins[i]+ptDbins[i+1]) / 2.;
		DMesonEff[i] = hEff->GetBinContent(hEff->GetXaxis()->FindBin(pt));
	}

  //Double_t sigmafixed=0.014;
  Double_t chi2cut=3;
  Bool_t meansigmaVar[6] = {kTRUE,kTRUE,kTRUE,kTRUE,kTRUE,kTRUE}; //set mean/sigma variations: fixedS, fixedS+15%, fixedS+15%, freeS&M, freeS/fixedM, fixedS&M
//  Bool_t bkgVar[8] = {kTRUE,kFALSE,kTRUE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE}; //set bgk variations: exp, lin, pol2, pol3, pol4, pol5, PowLaw, PowLaw*Exp
  Bool_t bkgVar[8] = {kTRUE,kTRUE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE}; //set bgk variations: exp, lin, pol2, pol3, pol4, pol5, PowLaw, PowLaw*Exp

  Int_t nRebinSteps=1;
  //Int_t rebinStep[2]={1};
  Int_t rebinStep[1]={1};
  Int_t nMinMassSteps=2;
  Double_t minMassStep[2]={1.72,1.74};
  Int_t nMaxMassSteps=2;
  Double_t maxMassStep[2]={2.00,2.03};
  Int_t nStepsBC=2;
  Double_t nSigmasBC[2]={3.5,4.0};
  //WARNING! set nmask value to active mean/sigma*active bkg variations!
  //And adjust consequently the following matrix (put an entry for each variation, with value: 0=don't consider it, 1=consider it in the final syst eval)
  Int_t nmask = 12;
  Bool_t mask[12] =    {1,1,   // fixed sigma (Expo, Lin, Pol2, Pol3, Pol4, Pol5, PowLaw, PowLaw*Exp)
			1,1,   // fixed sigma+15%
			1,1,   // fixed sigma-15%
			1,1,   // free sigma, free mean
			1,1,   // free sigma, fixed mean
            1,1};  // fixed mean, fixed sigma

   /*   Int_t nmask = 18;
      Bool_t mask[18] =    {1,1,1,   // fixed sigma (Expo, Lin, Pol2, Pol3, Pol4, Pol5, PowLaw, PowLaw*Exp)
    			1,1, 1,  // fixed sigma+15%
    			1,1, 1,  // fixed sigma-15%
    			1,1, 1,  // free sigma, free mean
    			1,1, 1,  // free sigma, fixed mean
                1,1, 1};  // fixed mean, fixed sigma*/


  interface->SetInputFilename("/mnt/hgfs/vmware/data_250319/data/AnalysisResults_Run2.root");
  interface->SetInputDirname("DmesonsForJetCorrelations");
  interface->SetInputListname("histosD0MBN");
  interface->SetInputObjectname("hsDphiz");

  ////////////////// TODO - check this option
  //interface->SetMassEdgesAndBinWidthForMassPlot(1.5664,2.1664,0.006);
  interface->SetDmesonPtBins(nDbins,ptDbins);
  interface->SetJetPtBins(nJetbins,ptJetbins);
  interface->SetDmesonEfficiency(DMesonEff);
  //interface->SetSigmaToFixDPtBins(sigmaD);
  //interface->SetSigmaToFixJetPtBins(sigmajet);

  interface->SetRebinSpectrumIfSBApproach(kTRUE); //kTRUE=rebin the jet spectrum with ptJetbins[] vals, otherwise use the binning from THnSparse projection

  interface->SetSigmaForSignalRegion(2.); //only for SB method: sigma range of signal region (usually 3 sigma, also 2 is fine if low S/B)
  //interface->SetSigmaToFix(sigmafixed); // TODO check this
  interface->SetChi2Cut(chi2cut);
  interface->SetMeanSigmaVariations(meansigmaVar);
  interface->SetBkgVariations(bkgVar);
  interface->SetRebinSteps(nRebinSteps,rebinStep);
  interface->SetMinMassSteps(nMinMassSteps,minMassStep);
  interface->SetMaxMassSteps(nMaxMassSteps,maxMassStep);
  interface->SetSigmaBinCounting(nStepsBC,nSigmasBC);
  interface->SetMaskOfVariations(nmask,mask);

  return;
}


//________________________________________
void SetInputParametersDstar(AliDJetRawYieldUncertaintyLocal *interface){

  //Dstar cfg
//  Int_t nDbins = 10;
//  Double_t ptDbins[11] = {3,4,5,6,7,8,10,12,16,24,36};

//  Int_t nJetbins = 9;
 // Double_t ptJetbins[9] = {2,4,6,8,10,12,16,24,40};
//  Double_t ptJetbins[10] = {3,4,5,6,8,10,14,20,30,50};

  Double_t DMesonEff[10];
  TFile *FileEff = new TFile("/home/basia/Work/alice/analysis/pPb_run2/Efficiencies/out_806Preliminary/DjetEff_prompt_jetpt2_50.root");
    TH1D *hEff = dynamic_cast<TH1D*>(FileEff->Get("hEff_reb"));
	for(int i=0;i<ptbinsDN;i++){
		double pt = (ptDbins[i]+ptDbins[i+1]) / 2.;
		double eff = hEff->GetBinContent(hEff->GetXaxis()->FindBin(pt));
		DMesonEff[i] = eff;
	}

 //  = {0.0201558, 0.0489161, 0.0820775, 0.137601, 0.173475, 0.236302, 0.272683, 0.306574, 0.361937, 0.44086};

 // Double_t sigmafixed=0.0006;

  Double_t chi2cut=3;
  Bool_t meansigmaVar[6] = {kTRUE,kTRUE,kTRUE,kTRUE,kTRUE,kTRUE}; //set mean/sigma variations: fixedS, fixedS+15%, fixedS+15%, freeS&M, freeS/fixedM, fixedS&M
  Bool_t bkgVar[8] = {kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kTRUE,kTRUE}; //set bgk variations: exp, lin, pol2, pol3, pol4, pol5, PowLaw, PowLaw*Exp

  //Bool_t meansigmaVar[6] = {kFALSE,kFALSE,kFALSE,kTRUE,kFALSE,kFALSE}; //set mean/sigma variations: fixedS, fixedS+15%, fixedS+15%, freeS&M, freeS/fixedM, fixedS&M
  //Bool_t bkgVar[8] = {kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kTRUE,kTRUE}; //set bgk variations: exp, lin, pol2, pol3, pol4, pol5, PowLaw, PowLaw*Exp

  Int_t nRebinSteps=2;
  Int_t rebinStep[2]={1,2};
  //Int_t nRebinSteps=1;
  //Int_t rebinStep[2]={1};
  Int_t nMinMassSteps=2;
  Double_t minMassStep[2]={0.140,0.142};
  Int_t nMaxMassSteps=2;
  Double_t maxMassStep[2]={0.158,0.160};
  Int_t nStepsBC=2;
  Double_t nSigmasBC[2]={3.5,4.0};
  //WARNING! set nmask value to active mean/sigma*active bkg variations!
  //And adjust consequently the following matrix (put an entry for each variation, with value: 0=don't consider it, 1=consider it in the final syst eval)
  Int_t nmask = 12;
  Bool_t mask[12] =    {1,1,   // fixed sigma (Expo, Lin, Pol2, Pol3, Pol4, Pol5, PowLaw, PowLaw*Exp)
			1,1,   // fixed sigma+15%
			1,1,   // fixed sigma-15%
			1,1,   // free sigma, free mean
			1,1,   // free sigma, fixed mean
			1,1};  // fixed mean, fixed sigma



  interface->SetInputFilename("/home/basia/Work/alice/analysis/pPb_run2/outData/cuts806_preliminary/AnalysisResults_FASTwoSDD.root");
  interface->SetInputDirname("DmesonsForJetCorrelations");
  interface->SetInputListname("histosDStarMBN");
  interface->SetInputObjectname("hsDphiz");

  interface->SetDmesonPtBins(nDbins,ptDbins);
  interface->SetJetPtBins(nJetbins,ptJetbins);
  interface->SetDmesonEfficiency(DMesonEff);
  //interface->SetSigmaToFixDPtBins(sigmaD);
  //interface->SetSigmaToFixJetPtBins(sigmajet);

  interface->SetRebinSpectrumIfSBApproach(kTRUE); //kTRUE=rebin the jet spectrum with ptJetbins[] vals, otherwise use the binning from THnSparse projection

  interface->SetSigmaForSignalRegion(3.); //only for SB method: sigma range of signal region (usually 3 sigma, also 2 is fine if low S/B)
  //interface->SetSigmaToFix(sigmafixed);
  interface->SetChi2Cut(chi2cut);
  interface->SetMeanSigmaVariations(meansigmaVar);
  interface->SetBkgVariations(bkgVar);
  interface->SetRebinSteps(nRebinSteps,rebinStep);
  interface->SetMinMassSteps(nMinMassSteps,minMassStep);
  interface->SetMaxMassSteps(nMaxMassSteps,maxMassStep);
  interface->SetSigmaBinCounting(nStepsBC,nSigmasBC);
  interface->SetMaskOfVariations(nmask,mask);

  return;
}
/*
//________________________________________
void SetInputParametersDzero(AliDJetRawYieldUncertaintyLocal *interface){

  //Dzero cfg
  Int_t nDbins = 9;
  Double_t ptDbins[10] = {3,4,5,6,7,8,12,16,24,36};
  Int_t nJetbins = 9;
 // Double_t ptJetbins[9] = {2,4,6,8,10,12,16,24,40};
  Double_t ptJetbins[10] = {3,4,5,6,8,10,14,20,30,50};

  Double_t DMesonEff[9] = { 0.03644752, 0.05664352 ,0.07682878 ,0.08783701, 0.09420746, 0.1047988, 0.1338670, 0.2143196, 0.2574591}; //chopping 0-1, 1-2

  Double_t sigmafixed=0.014;
  Double_t chi2cut=3;
  Bool_t meansigmaVar[6] = {kTRUE,kTRUE,kTRUE,kTRUE,kTRUE,kTRUE}; //set mean/sigma variations: fixedS, fixedS+15%, fixedS+15%, freeS&M, freeS/fixedM, fixedS&M
  Bool_t bkgVar[8] = {kTRUE,kFALSE,kTRUE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE}; //set bgk variations: exp, lin, pol2, pol3, pol4, pol5, PowLaw, PowLaw*Exp
  Int_t nRebinSteps=1;
  Int_t rebinStep[1]={1};
  Int_t nMinMassSteps=2;
  Double_t minMassStep[2]={1.72,1.74};
  Int_t nMaxMassSteps=2;
  Double_t maxMassStep[2]={2.00,2.03};
  Int_t nStepsBC=2;
  Double_t nSigmasBC[2]={3.5,4.0};
  //WARNING! set nmask value to active mean/sigma*active bkg variations!
  //And adjust consequently the following matrix (put an entry for each variation, with value: 0=don't consider it, 1=consider it in the final syst eval)
  Int_t nmask = 12;
  Bool_t mask[12] =    {1,1,   // fixed sigma (Expo, Lin, Pol2, Pol3, Pol4, Pol5, PowLaw, PowLaw*Exp)
			1,1,   // fixed sigma+15%
			1,1,   // fixed sigma-15%
			1,1,   // free sigma, free mean
			1,1,   // free sigma, fixed mean
			1,1};  // fixed mean, fixed sigma


  interface->SetInputFilename("./AnalysisResults_Djets_pp.root");
  interface->SetInputTreename("AliAnalysisTaskDmesonJets_AnyINT_D0");
  interface->SetInputDBranchname("DmesonJet");
  interface->SetInputJetBranchname("Jet_AKTChargedR040_pt_scheme");

  interface->SetMassEdgesAndBinWidthForMassPlot(1.5664,2.1664,0.006);
  interface->SetDmesonPtBins(nDbins,ptDbins);
  interface->SetJetPtBins(nJetbins,ptJetbins);
  interface->SetDmesonEfficiency(DMesonEff);

  interface->SetSigmaForSignalRegion(3.); //only for SB method: sigma range of signal region (usually 3 sigma, also 2 is fine if low S/B)
  interface->SetSigmaToFix(sigmafixed);
  interface->SetChi2Cut(chi2cut);
  interface->SetMeanSigmaVariations(meansigmaVar);
  interface->SetBkgVariations(bkgVar);
  interface->SetRebinSteps(nRebinSteps,rebinStep);
  interface->SetMinMassSteps(nMinMassSteps,minMassStep);
  interface->SetMaxMassSteps(nMaxMassSteps,maxMassStep);
  interface->SetSigmaBinCounting(nStepsBC,nSigmasBC);
  interface->SetMaskOfVariations(nmask,mask);

  return;
}
*/
