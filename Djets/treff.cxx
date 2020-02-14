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
#include "AliAnalysisTaskTrackingSysPropagation.h"
//#include "/home/kvapil/alice/sw/SOURCES/AliRoot/v5-09-43/v5-09-43/ANALYSIS/macros/train/addAODHandler.C"
#include "AliAnalysisAlien.h"
#include "AliAnalysisManager.h"
#include "AliAODInputHandler.h"
#include "TChain.h"
#include <fstream>

class AliAODInputHandler;

AliAnalysisTaskTrackingSysPropagation *rr(Int_t system=0 /*0=pp,1=PbPb*/,
                                                                     AliAnalysisTaskTrackingSysPropagation::DecChannel ch=AliAnalysisTaskTrackingSysPropagation::kD0toKpi,
                                                                     TString filename = "/home/kvapil/Desktop/cristina/D0toKpiCutsNewCut_D0kint7_finerPtBins.root",
                                                                     TString filenameHistME = "",
                                                                     TString filenameHistTrEff = "",
                                                                     TString postname = "",
                                                                     Double_t maxPt=60.)
{

  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  if (!mgr) {
    ::Error("AddTask", "No analysis manager to connect to.");
  }

  // Check the analysis type using the event handlers connected to the analysis manager.
  //==============================================================================
  if (!mgr->GetInputEventHandler()) {
    ::Error("AddTaskImpParDistrib", "This task requires an input event handler");
    return NULL;
  }

  TFile *cu = new TFile("/home/kvapil/Desktop/cristina/D0toKpiCutsNewCut_D0kint7_finerPtBins.root","read");
  AliRDHFCuts * cuts = dynamic_cast<AliRDHFCuts*>(cu->Get("D0toKpiCuts"));
  Double_t bi[7]={1,2,4,6,8,12,24};

  TH1F *me = new TH1F("me","me",6,bi);
  TH1F *tr = new TH1F("tr","tr",6,bi);
  Double_t trco[6] = {0.5,0.5,0.5,1,1,0.5};
  Double_t meco[6] = {1.8,2.7,2.3,2.4,3,2.9};

  for(Int_t i = 1; i<=6;i++){
      me->SetBinContent(i,meco[i-1]);
      tr->SetBinContent(i,trco[i-1]);
  }


  Bool_t stdcuts=kFALSE;
  TFile* filecuts;
  if( filename.EqualTo("") ) {
    stdcuts=kTRUE;
  } else {
    filecuts=TFile::Open(filename.Data());
    if(!filecuts ||(filecuts&& !filecuts->IsOpen())){
      Printf("FATAL: Cut object not found: analysis will not start!\n"); return NULL;
    }
    else printf("Cut object correctly found\n");
  }

  //Analysis Task

  AliRDHFCuts* analysiscuts;

  if(ch==AliAnalysisTaskTrackingSysPropagation::kDstartoKpipi) analysiscuts = (AliRDHFCuts*)filecuts->Get("DStartoKpipiCuts");
  else if(ch==AliAnalysisTaskTrackingSysPropagation::kD0toKpi) analysiscuts = (AliRDHFCuts*)filecuts->Get("D0toKpiCuts");
  else analysiscuts = (AliRDHFCuts*)filecuts->Get("AnalysisCuts");

  TH1F *histoME = me;
  if(!histoME){ Printf("FATAL: Histo with ME syst. not found: analysis will not start!\n"); return NULL;}

  TH1F *histoTrEff = tr;
  if(!histoTrEff) {Printf("FATAL: Histo TrEff not found: analysis will not start!\n"); return NULL;}


  AliAnalysisTaskTrackingSysPropagation *Task = new AliAnalysisTaskTrackingSysPropagation(ch, analysiscuts, histoME, histoTrEff);

  Task->SetMaximumPt(maxPt);
  Task->SetDebugLevel(1);
  mgr->AddTask(Task);

  // Create containers for input/output
  TString outputFileName = AliAnalysisManager::GetCommonFileName();
  outputFileName += Form(":MatchEff_%s",postname.Data());

  AliAnalysisDataContainer *coutput =0x0;
  coutput = mgr->CreateContainer(Form("systUnc_%s",postname.Data()),
                 TList::Class(),
                 AliAnalysisManager::kOutputContainer,
                 outputFileName );


  mgr->ConnectInput(Task,0,mgr->GetCommonInputContainer());
  mgr->ConnectOutput(Task,1,coutput);

  return Task;
}
void treff(){
    gInterpreter->ProcessLine(".include $ROOTSYS/include");
    gInterpreter->ProcessLine(".include $ALICE_ROOT/include");
    AliAnalysisManager *mgr = new AliAnalysisManager("mytask");
    AliAODInputHandler *aodH = new AliAODInputHandler();
    mgr->SetInputEventHandler(aodH);
    AliAnalysisTaskTrackingSysPropagation *prop = rr();
    TChain* chainAOD = new TChain("aodTree");
    TChain *chainAODfriend = new TChain("aodTree");

    chainAOD->Add("/home/kvapil/Desktop/alice_Djets/AliAOD.root");
    chainAODfriend->Add("/home/kvapil/Desktop/alice_Djets/AliAOD.VertexingHF.root");
    chainAOD->AddFriend(chainAODfriend);
    if(!mgr->InitAnalysis()) return;
       mgr->SetDebugLevel(2);
       mgr->PrintStatus();
       mgr->SetUseProgressBar(1, 25);
       mgr->StartAnalysis("local",chainAOD);
}
