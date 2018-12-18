#if !defined (__CINT__) || defined (__CLING__)
#include "AliVEvent.h"
#include "AliAnalysisManager.h"
#include "TROOT.h"
#include <iostream>
#include "AliEmcalJetTask.h"
#include "AliPhysicsSelectionTask.h"
R__ADD_INCLUDE_PATH($ALICE_ROOT)
#include "ANALYSIS/macros/AddTaskPIDResponse.C"
R__ADD_INCLUDE_PATH($ALICE_PHYSICS)
#include "OADB/macros/AddTaskPhysicsSelection.C"
#include "AliAnalysisTaskSEDmesonsFilterCJQA.h"
#include "AddTaskSEDmesonsFilterCJQA.C"
#include "PWGJE/EMCALJetTasks/macros/AddTaskEmcalJet.C"
#include "AddTaskDFilterAndCorrelationsQA.C"
#endif

void AddTasksFlavourJetMCQA(const Int_t iCandType = 1 /*0 = D0, 1=Dstar...*/,
   const TString sCutFile = "cutsHF/D0toKpiCutsppRecVtxNoPileupRejNoEMCAL.root",
   const Double_t dJetPtCut   = 0.,
   const Double_t dJetAreaCut = 0.,
   const char *acctype = "TPC",
   const TString sRunPeriod = "LHC10b",
   const Int_t    uBeamType = 0,
   const UInt_t uTriggerMask = AliVEvent::kAny, /*for jets; the D mesons trigger is defined in the cut object*/
   const Bool_t bIsMC = kFALSE,
   const Bool_t bIsReco = kFALSE,
   const Bool_t bIsMap = kFALSE,
   const Bool_t bRM = kFALSE,
   const Bool_t bRMEff = kFALSE,
   const Bool_t bPythia = kFALSE,
   const Bool_t isPrompt = kTRUE,
   TString sText="",/*completes the name of the candidate task lists*/
   Bool_t doBkg = kFALSE
   )
{

   TString rhoName = "";
   TString rhoNameBkg = "";
   TString rhoNameMC = "";

   const Int_t    nRadius = 1;
   const Double_t aRadius[] = {  0.4  };
   const TString  sRadius[] = { "R04" };
   
    
   //=============================================================================
   
   AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
   
   if (!mgr) {
      ::Error("AddTasksFlavourJetQA.C::AddTasksFlavourJetQA", "No analysis manager to connect to.");
      return;
   }
   
   TString type = mgr->GetInputEventHandler()->GetDataType();
   if (!type.Contains("ESD") && !type.Contains("AOD")) {
      ::Error("AddTasksFlavourJetQA.C::AddTasksFlavourJetQA", "Task manager to have an ESD or AOD input handler.");
      return;
   }
   
   if (!mgr->GetInputEventHandler()) {
      ::Error("AddTasksFlavourJetQA.C::AddTasksFlavourJetQA", "This task requires an input event handler");
      return;
   }
   //=============================================================================
   
   
    //Centrality Selection
   //gROOT->LoadMacro("$ALICE_PHYSICS/OADB/COMMON/MULTIPLICITY/macros/AddTaskMultSelection.C");
   //AliMultSelectionTask *taskMult = AddTaskMultSelection();

   //D mesons -- PID
   gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDResponse.C");
   AliAnalysisTaskSE *taskRespPID = AddTaskPIDResponse(bIsMC,kFALSE,kTRUE,"1");
    
   gROOT->LoadMacro("$ALICE_PHYSICS/OADB/macros/AddTaskPhysicsSelection.C");
   // -- Physics selection task
   if(!bIsMC){
        AliPhysicsSelectionTask *physSelTask = AddTaskPhysicsSelection(kFALSE,kTRUE);
        if (!physSelTask) {
                std::cout << "no physSelTask";
                return; 
        }
    }
    else {
        AliPhysicsSelectionTask *physSelTask = AddTaskPhysicsSelection(kTRUE,kFALSE);
        if (!physSelTask) {
                std::cout << "no physSelTask";
                return; 
        }    
   }
   // --
   
	// Improver task for some MC
   // gROOT->LoadMacro("$ALICE_PHYSICS/PWGHF/vertexingHF/macros/AddTaskImproveITS.C");
   //AliAnalysisTaskSEImproveITS *taskImprover = AddTaskImproveITS(kFALSE,"alien:///alice/cern.ch/user/a/afestant/filesForImprover/pPb2016/ITSgraphs_Current.root","alien:///alice/cern.ch/user/a/afestant/filesForImprover/pPb2016/ITSgraphs_NewAll-X0.3-Res4um.root",0 );
   
  
    //D meson filtering task
    gROOT->LoadMacro("AddTaskSEDmesonsFilterCJQA.C"); // local, modified task
    //gROOT->LoadMacro("$ALICE_PHYSICS/PWGJE/FlavourJetTasks/macros/AddTaskSEDmesonsFilterCJ.C"); // code from AliPhysics
    
    
    //D-jet correlation task
    gROOT->LoadMacro("AddTaskDFilterAndCorrelationsQA.C");  // local, modified task
   //gROOT->LoadMacro("$ALICE_PHYSICS/PWGJE/FlavourJetTasks/macros/AddTaskDFilterAndCorrelations.C"); // code from AliPhysics
    
    
    //Jet task
    gROOT->LoadMacro("$ALICE_PHYSICS/PWGJE/EMCALJetTasks/macros/AddTaskEmcalJet.C");
    
    
    //loop to run over D jets candiates (usually up to 5)
    for(Int_t i=0; i<2  ; i++)
    {
        
        TString TaskText = sText;
        TaskText += Form("N%d",i);
        
        //Filtering task. It will create a set of particles with the D meson instead of the daughters
        AliAnalysisTaskSEDmesonsFilterCJQA::ECandidateType eCandType;
        if(iCandType==0) eCandType = AliAnalysisTaskSEDmesonsFilterCJQA::ECandidateType::kD0toKpi;
        if(iCandType==1) eCandType = AliAnalysisTaskSEDmesonsFilterCJQA::ECandidateType::kDstartoKpipi;
        AliAnalysisTaskSEDmesonsFilterCJQA *filter = AddTaskSEDmesonsFilterCJQA(eCandType,sCutFile,bIsMC,bIsReco,TaskText);
        filter->SetCombineDmesons(kTRUE);
        filter->SetMultipleCandidates(kTRUE); //Analyse one candidate per event
        filter->SetAnalysedCandidate(i); //Number of the candidate that will be analysed (0 = first candidate)

        if(isPrompt){
                filter->SetRejectDfromB(kTRUE);
                filter->SetKeepOnlyDfromB(kFALSE);
        }
        else{
                filter->SetRejectDfromB(kFALSE);
                filter->SetKeepOnlyDfromB(kTRUE);
        }
        
        filter->SetBuildRMEff(bRMEff);
        filter->SetUsePythia(bPythia);
        // set filter bits if needed
        
       //This is the particle container with the tracks of the event
        AliTrackContainer* trackCont1 = filter->AddTrackContainer("tracks");
        trackCont1->SetClassName("AliAODTrack");
        trackCont1->SetTrackFilterType(AliEmcalTrackSelection::kCustomTrackFilter);
        trackCont1->SetAODFilterBits((1<<4)|(1<<9));
        
        AliMCParticleContainer* trackContMC = filter->AddMCParticleContainer("mcparticles");
        trackContMC->SetClassName("AliAODMCParticle");
        
        TString candArrName = "Dcandidates";
        TString sbArrName = "DSBcandidates";
        TString DcandAndTracks = "DcandidatesAndTracks";
        TString DBkgAndTracks = "DSBcandidatesAndTracks";
        TString MCDcandAndTracks = "MCDcandidatesAndTracks";
        TString candName;
        if(iCandType==0) candName = "D0";
        if(iCandType==1) candName = "DStar";
        
        candArrName += candName;
        candArrName += TaskText;
        sbArrName += candName;
        sbArrName += TaskText;
        DcandAndTracks += candName;
        DcandAndTracks += TaskText;
        DBkgAndTracks += candName;
        DBkgAndTracks += TaskText;
        MCDcandAndTracks += candName;
        MCDcandAndTracks += TaskText;
        
        if (bIsMC) {
            candArrName += "MC";
            sbArrName  += "MC";
            DcandAndTracks += "MC";
            DBkgAndTracks += "MC";
            MCDcandAndTracks += "MC";
            if (bIsReco)
            {
                candArrName += "rec";
                sbArrName += "rec";
                DcandAndTracks += "rec";
                DBkgAndTracks += "rec";
                MCDcandAndTracks += "rec";
                
            }
        }
        
        TString AKTJet = "AKTJet";
        AKTJet += TaskText;


        AliEmcalJetTask *taskFJDandTracks = AddTaskEmcalJet(DcandAndTracks,"",AliJetContainer::antikt_algorithm,aRadius[0],AliJetContainer::kFullJet,0.15,0.30,0.005,AliJetContainer::pt_scheme,AKTJet,0.,kFALSE,kFALSE);
        taskFJDandTracks->SelectCollisionCandidates(uTriggerMask);  
        
        //Generated level jets. No pT>0.15 GeV/c cut should be applied here!
        AliEmcalJetTask *taskFJMCDandTracks = AddTaskEmcalJet(MCDcandAndTracks,"",AliJetContainer::antikt_algorithm,aRadius[0],AliJetContainer::kFullJet,0.0,0.0,0.005,AliJetContainer::pt_scheme,AKTJet,0.,kFALSE,kFALSE);
        taskFJMCDandTracks->SelectCollisionCandidates(uTriggerMask);  
        
        
        //For Data. Comment this part if you run on Monte Carlo
    /*    AliAnalysisTaskFlavourJetCorrelationsTest *CorrTask = AddTaskDFilterAndCorrelationsQA(
                                                                                         iCandType,
                                                                                         sCutFile,
                                                                                         bIsMC,
                                                                                         bIsReco,
                                                                                         TaskText,
                                                                                         taskFJDandTracks->GetName(),
                                                                                         DcandAndTracks,
                                                                                         "",
                                                                                         "",
                                                                                         "",
                                                                                         "",
                                                                                         "",
                                                                                         "",
                                                                                         aRadius[0],
                                                                                         dJetPtCut,
                                                                                         acctype,
                                                                                         dJetAreaCut,
                                                                                         AliAnalysisTaskFlavourJetCorrelationsTest::kConstituent);
        //Flag to build the Response Matrix
        CorrTask->SetBuildResponseMatrix(bRM);
        CorrTask->SetBuildResponseMatrixEff(bRMEff);
        //if to use only Pythia tracks for MC
        CorrTask->SetUsePythia(bPythia);
        
        //Container with generated level particles and D meson instead of the daughters
        AliMCParticleContainer *MCpartCont  = CorrTask->AddMCParticleContainer(MCDcandAndTracks);
        AliJetContainer *jetContMC = CorrTask->AddJetContainer(taskFJMCDandTracks->GetName(),acctype,aRadius[0]);
        if(jetContMC) {
            jetContMC->ConnectParticleContainer(MCpartCont);
            jetContMC->SetJetPtCut(dJetPtCut);
            jetContMC->SetPercAreaCut(dJetAreaCut);
        }
        */
      
      
    }
   
   return;
}
