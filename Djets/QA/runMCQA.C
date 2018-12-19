#if !defined (__CINT__) || defined (__CLING__)
#include "AliAnalysisAlien.h"
#include "AliAnalysisManager.h"
#include "AliAODInputHandler.h"

#include "AliPhysicsSelectionTask.h"
#include "AliMultSelectionTask.h"
#include "AliAnalysisTaskPIDResponse.h"

#include "AliAnalysisTaskSEDs.h"
#include "AliAnalysisTaskSED0Mass.h"
#include "AliEmcalJetTask.h"

#include "TChain.h"

#include "AliAnalysisTaskSEDmesonsFilterCJQA.h"
#include "AliAnalysisTaskFlavourJetCorrelationsQA.h"
//#include "AddTasksFlavourJetMCQA.C"
#endif

//SETTINGS
//************************************

Bool_t runLocal=kTRUE;                                  // flag to run locally on AliAOD.root + AliAOD.VertexingHF.root
TString pathToLocalAODfiles="../..";               // path to find AOD files when running locally
Bool_t runGridTest=kFALSE;                               // flag to run a grid test: kTRUE (+runLocal=kFALSE). To run job on GRID: runGridTest=kFALSE, runLocal=kFALSE
TString runMode="terminate";                                  // sets the run grid mode: "full", "terminate"

TString aliPhysVersion="vAN-20181118-1";

Bool_t isRunOnMC=kTRUE;                                 // set to kTRUE to run on Mone Carlo and uncomment/comment accordingly the following lines about paths on Alien
//paths on Alien
// Monte Carlo
//TString gridDataDir="/alice/sim/2018/LHC18a4a2_fast/";
//TString gridDataPattern="AOD";
// Data
TString gridDataDir="/alice/data/2017/LHC17p/";
TString gridDataPattern="/pass1_CENT_wSDD/AOD";


// Alien output director
TString gridWorkingDir="Bplus_20Nov18_TenRuns";
TString gridOutputDir="output";

//run numbers
const Int_t nruns = 10;
Int_t runlist[nruns] = {282342, 282341, 282340, 282314, 282313, 282312, 282309, 282307, 282306, 282305};// , 282304, 282303, 282302, 282247, 282230, 282229, 282227, 282224, 282206, 282189, 282147, 282146, 282127, 282126, 282125, 282123, 282122, 282120, 282119, 282118, 282099, 282098, 282078, 282051, 282050, 282031, 282025, 282021, 282016, 282008, 282367, 282366, 282365, 282343};

//Task configuration
TString cutFile="./cutfile/D0DsDplusLcBplusCuts_pp.root";          // file containing the cuts for the different mesons
  														 // to generate the cut file: 1) move to cutfile directory
  														 //                           2) .L makeCutsTreeCreator.C
  														 //                           3) makeCutsTreeCreator();
  														 // to run with ROOT5/6 generate the cut file using AliPhysics built on ROOT5/6

const char*    runPeriod           = "lhc16q";                    // set the run period (used on grid)
const char*    uniqueName          = "QATest";           			// sets base string for the name of the task on the grid
UInt_t         pSel                = AliVEvent::kINT7;             // used event selection for every task except for the analysis tasks
Bool_t         isReco              = kTRUE;
Bool_t         isMap               = kFALSE;
Bool_t         bRM                 = kFALSE;
Bool_t         bRMEff              = kTRUE;
Bool_t         bPythia             = kFALSE;
Bool_t         isPrompt            = kTRUE;
const Double_t aRadius[] = {  0.4  };


//************************************


void runAnalysis()
{
    // set if you want to run the analysis locally (kTRUE), or on grid (kFALSE)
    Bool_t local = runLocal;
    // if you run on grid, specify test mode (kTRUE) or full grid model (kFALSE)
    Bool_t gridTest = runGridTest;

    // since we will compile a class, tell root where to look for headers
#if !defined (__CINT__) || defined (__CLING__)
    gInterpreter->ProcessLine(".include $ROOTSYS/include");
    gInterpreter->ProcessLine(".include $ALICE_ROOT/include");
#else
    gROOT->ProcessLine(".include $ROOTSYS/include");
    gROOT->ProcessLine(".include $ALICE_ROOT/include");
#endif

    // create the analysis manager
    AliAnalysisManager *mgr = new AliAnalysisManager("AnalysisTaskExample");
    AliAODInputHandler *aodH = new AliAODInputHandler();
    mgr->SetInputEventHandler(aodH);


    // compile the class and load the add task macro
    // here we have to differentiate between using the just-in-time compiler
    // from root6, or the interpreter of root5
#if !defined (__CINT__) || defined (__CLING__)

    AliPhysicsSelectionTask *physseltask = reinterpret_cast<AliPhysicsSelectionTask *>(gInterpreter->ProcessLine(Form(".x %s (%d)", gSystem->ExpandPathName("$ALICE_PHYSICS/OADB/macros/AddTaskPhysicsSelection.C"),isRunOnMC)));

    AliAnalysisTaskPIDResponse *pidResp = reinterpret_cast<AliAnalysisTaskPIDResponse *>(gInterpreter->ProcessLine(Form(".x %s (%d)", gSystem->ExpandPathName("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDResponse.C"),isRunOnMC)));

//    AliMultSelectionTask *multSel = reinterpret_cast<AliMultSelectionTask *>(gInterpreter->ProcessLine(Form(".x %s", gSystem->ExpandPathName("$ALICE_PHYSICS/OADB/COMMON/MULTIPLICITY/macros/AddTaskMultSelection.C"))));
//    //multSel->SetAlternateOADBforEstimators("LHC15o-DefaultMC-HIJING");

  //  gInterpreter->LoadMacro("AliHFTreeHandler.cxx++g");
  //  gInterpreter->LoadMacro("AliHFTreeHandlerD0toKpi.cxx++g");
  //  gInterpreter->LoadMacro("AliHFTreeHandlerDplustoKpipi.cxx++g");
  //  gInterpreter->LoadMacro("AliHFTreeHandlerDstoKKpi.cxx++g");
  //  gInterpreter->LoadMacro("AliHFTreeHandlerLctopKpi.cxx++g");
  //  gInterpreter->LoadMacro("AliHFTreeHandlerBplustoD0pi.cxx++g");
  //  gInterpreter->LoadMacro("AliAnalysisTaskSEHFTreeCreator_v1.cxx++g");
    gROOT->LoadMacro("AliAnalysisTaskSEDmesonsFilterCJQA.cxx++g");
    gROOT->LoadMacro("AliAnalysisTaskFlavourJetCorrelationsQA.cxx++g");
  //  gROOT->LoadMacro("AddTaskSEDmesonsFilterCJQA.C"); // local, modified task
  //  gROOT->LoadMacro("AddTaskDFilterAndCorrelationsQA.C");  // local, modified task
  //  gROOT->LoadMacro("$ALICE_PHYSICS/PWGJE/EMCALJetTasks/macros/AddTaskEmcalJet.C");

   // AddTasksFlavourJetMCQA(0,"alien:///alice/cern.ch/user/b/btrzecia/pp5TeV/CutsDzero/D0toKpiCuts_pp5TeV_pPbBase.root",0.,0.,"TPCFID",runPeriod,0,pSel,isRunOnMC,isReco,isMap,bRM,bRMEff,bPythia,isPrompt,"MB"); // Jet(1,..) for D*
  //  gInterpreter->ProcessLine(Form(".x %s (%d,\"%s\",%f,%f,\"%s\",\"%s\",%d,%d,%d,%d,%d,%d,%d,%d,%d,\"%s\")",gSystem->ExpandPathName("AddTasksFlavourJetMCQA.C"),0, "alien:///alice/cern.ch/user/b/btrzecia/pp5TeV/CutsDzero/D0toKpiCuts_pp5TeV_pPbBase.root",0.0,0.0,"TPCFID",runPeriod,0,pSel,isRunOnMC,isReco,isMap,bRM,bRMEff,bPythia,isPrompt,"MB"));
 //   AliAnalysisTaskSEHFTreeCreator_v1 *task = reinterpret_cast<AliAnalysisTaskSEHFTreeCreator_v1*>(gInterpreter->ProcessLine(Form(".x %s (%d,%d,\"%s\",\"%s\",%d,%d,%d,%d,%d,%d,%d,%d)",gSystem->ExpandPathName("AddTaskHFTreeCreator_v1.C"),isRunOnMC, 0, "HFTreeCreator", cutFile.Data(),1,isRunOnMC,isRunOnMC,0,0,0,0,1)));
    for(Int_t i=0; i<2  ; i++)
    {

        TString TaskText = "MB";
        TaskText += Form("N%d",i);

        //Filtering task. It will create a set of particles with the D meson instead of the daughters
  //      AliAnalysisTaskSEDmesonsFilterCJQA::ECandidateType eCandType;
//        if(iCandType==0) eCandType = AliAnalysisTaskSEDmesonsFilterCJQA::ECandidateType::kD0toKpi;
   //     if(iCandType==1) eCandType = AliAnalysisTaskSEDmesonsFilterCJQA::ECandidateType::kDstartoKpipi;
       // AliAnalysisTaskSEDmesonsFilterCJQA *filter = AddTaskSEDmesonsFilterCJQA(eCandType,sCutFile,bIsMC,bIsReco,TaskText);

   //     TString param = "";
   //           param+= std::to_string(iCandType)+",";
   //           param+=(TString)sCutFile+",";
   //           param+= bIsMC ? "1," : "0,";
   //           param+= bIsReco ? "1," : "0,";
   //           param+=(TString)TaskText;
           //   std::cout<<"TUUUUUUUUUUUU"<<iCandType<<sCutFile<<bIsMC<<bIsReco<<TaskText<<std::endl;
           //   std::cout<<"TUUUUUUUUUUUU"<<param<<std::endl;
     //   AliAnalysisTaskSEDmesonsFilterCJQA *filter = reinterpret_cast<AliAnalysisTaskSEDmesonsFilterCJQA *>(sednadd.Exec(param));
        AliAnalysisTaskSEDmesonsFilterCJQA *filter = reinterpret_cast<AliAnalysisTaskSEDmesonsFilterCJQA*>(gInterpreter->ProcessLine(Form(".x %s ((AliAnalysisTaskSEDmesonsFilterCJQA::ECandidateType)%d,\"%s\",%d,%d,\"%s\")",gSystem->ExpandPathName("AddTaskSEDmesonsFilterCJQA.C"),AliAnalysisTaskSEDmesonsFilterCJQA::kD0toKpi, "../cuts_pp/Dzero/D0toKpiCuts_pp13TeV_pPbBase_newNorm.root",isRunOnMC,isReco,TaskText.Data())));
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
        candName = "D0";


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

        if (isRunOnMC) {
            candArrName += "MC";
            sbArrName  += "MC";
            DcandAndTracks += "MC";
            DBkgAndTracks += "MC";
            MCDcandAndTracks += "MC";
            if (isReco)
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


        //AliEmcalJetTask *taskFJDandTracks = AddTaskEmcalJet(DcandAndTracks,"",AliJetContainer::antikt_algorithm,aRadius[0],AliJetContainer::kFullJet,0.15,0.30,0.005,AliJetContainer::pt_scheme,AKTJet,0.,kFALSE,kFALSE);
        AliEmcalJetTask *taskFJDandTracks =reinterpret_cast<AliEmcalJetTask*>(gInterpreter->ProcessLine(Form(".x %s (\"%s\",\"%s\",(AliJetContainer::EJetAlgo_t)%d,%f,(AliJetContainer::EJetType_t)%d,%f,%f,%f,(AliJetContainer::ERecoScheme_t)%d,\"%s\",%f,%d,%d)",gSystem->ExpandPathName("$ALICE_PHYSICS/PWGJE/EMCALJetTasks/macros/AddTaskEmcalJet.C"),DcandAndTracks.Data(),"",AliJetContainer::antikt_algorithm,aRadius[0],AliJetContainer::kFullJet,0.15,0.30,0.005,AliJetContainer::pt_scheme,AKTJet.Data(),0.,kFALSE,kFALSE)));
        taskFJDandTracks->SelectCollisionCandidates(pSel);

        //Generated level jets. No pT>0.15 GeV/c cut should be applied here!
    //    AliEmcalJetTask *taskFJMCDandTracks = AddTaskEmcalJet(MCDcandAndTracks,"",AliJetContainer::antikt_algorithm,aRadius[0],AliJetContainer::kFullJet,0.0,0.0,0.005,AliJetContainer::pt_scheme,AKTJet,0.,kFALSE,kFALSE);
        AliEmcalJetTask *taskFJMCDandTracks =reinterpret_cast<AliEmcalJetTask*>(gInterpreter->ProcessLine(Form(".x %s (\"%s\",\"%s\",(AliJetContainer::EJetAlgo_t)%d,%f,(AliJetContainer::EJetType_t)%d,%f,%f,%f,(AliJetContainer::ERecoScheme_t)%d,\"%s\",%f,%d,%d)",gSystem->ExpandPathName("$ALICE_PHYSICS/PWGJE/EMCALJetTasks/macros/AddTaskEmcalJet.C"),MCDcandAndTracks.Data(),"",AliJetContainer::antikt_algorithm,aRadius[0],AliJetContainer::kFullJet,0.0,0.0,0.005,AliJetContainer::pt_scheme,AKTJet.Data(),0.,kFALSE,kFALSE)));
        taskFJMCDandTracks->SelectCollisionCandidates(pSel);



        AliAnalysisTaskFlavourJetCorrelationsQA *CorrTask = reinterpret_cast<AliAnalysisTaskFlavourJetCorrelationsQA*>(gInterpreter->ProcessLine(Form(".x %s ((AliAnalysisTaskFlavourJetCorrelationsQA::ECandidateType)%d,\"%s\",%d,%d,\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",%f,%f,\"%s\",%f,(AliAnalysisTaskFlavourJetCorrelationsQA::ECorrelationMethod)%d)",gSystem->ExpandPathName("AddTaskDFilterAndCorrelationsQA.C"),AliAnalysisTaskSEDmesonsFilterCJQA::kD0toKpi,
                                                                                                                                                      "../cuts_pp/Dzero/D0toKpiCuts_pp13TeV_pPbBase_newNorm.root",
                                                                                                                                                      isRunOnMC,
                                                                                                                                                      isReco,
                                                                                                                                                      TaskText.Data(),
                                                                                                                                                      taskFJDandTracks->GetName(),
                                                                                                                                                      DcandAndTracks.Data(),
                                                                                                                                                      "",
                                                                                                                                                      "",
                                                                                                                                                      "",
                                                                                                                                                      "",
                                                                                                                                                      "",
                                                                                                                                                      "",
                                                                                                                                                      aRadius[0],
                                                                                                                                                      0.0,
                                                                                                                                                      "TPC",
                                                                                                                                                      0.0,
                                                                                                                                                      AliAnalysisTaskFlavourJetCorrelationsQA::kConstituent)));



                //Flag to build the Response Matrix
                CorrTask->SetBuildResponseMatrix(bRM);
                CorrTask->SetBuildResponseMatrixEff(bRMEff);
                //if to use only Pythia tracks for MC
                CorrTask->SetUsePythia(bPythia);

                //Container with generated level particles and D meson instead of the daughters
                AliMCParticleContainer *MCpartCont  = CorrTask->AddMCParticleContainer(MCDcandAndTracks);
                AliJetContainer *jetContMC = CorrTask->AddJetContainer(taskFJMCDandTracks->GetName(),"TPC",aRadius[0]);
                if(jetContMC) {
                    jetContMC->ConnectParticleContainer(MCpartCont);
                    jetContMC->SetJetPtCut(0.0);
                    jetContMC->SetPercAreaCut(0.0);
                }

    }
#else

    gROOT->LoadMacro("$ALICE_PHYSICS/OADB/macros/AddTaskPhysicsSelection.C");
    AliPhysicsSelectionTask *physSelTask= AddTaskPhysicsSelection(isRunOnMC);

    gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDResponse.C");
    AliAnalysisTaskPIDResponse *pidResp = AddTaskPIDResponse(isRunOnMC);

//    gROOT->LoadMacro("$ALICE_PHYSICS/OADB/COMMON/MULTIPLICITY/macros/AddTaskMultSelection.C");
//    AliMultSelectionTask *multSel = AddTaskMultSelection();
    //multSel->SetAlternateOADBforEstimators("LHC15o-DefaultMC-HIJING");

 //   gROOT->LoadMacro("AliHFTreeHandler.cxx++g");
 //   gROOT->LoadMacro("AliHFTreeHandlerD0toKpi.cxx++g");
 //   gROOT->LoadMacro("AliHFTreeHandlerDplustoKpipi.cxx++g");
 //   gROOT->LoadMacro("AliHFTreeHandlerDstoKKpi.cxx++g");
  //  gROOT->LoadMacro("AliHFTreeHandlerLctopKpi.cxx++g");
  //  gROOT->LoadMacro("AliHFTreeHandlerBplustoD0pi.cxx++g");
  //  gROOT->LoadMacro("AliAnalysisTaskSEHFTreeCreator_v1.cxx++g");
  //  gROOT->LoadMacro("AddTaskHFTreeCreator_v1.C");
 //   AliAnalysisTaskSEHFTreeCreator_v1 *task = AddTaskHFTreeCreator_v1(isRunOnMC, 0, "HFTreeCreator", cutFile.Data(),1,isRunOnMC,isRunOnMC,0,0,0,0,1);
    gROOT->LoadMacro("AliAnalysisTaskSEDmesonsFilterCJQA.cxx++g");
    gROOT->LoadMacro("AliAnalysisTaskFlavourJetCorrelationsQA.cxx++g");
    gROOT->LoadMacro("AddTasksFlavourJetMCQA.C");
    AddTasksFlavourJetMCQA(0,"alien:///alice/cern.ch/user/b/btrzecia/pp5TeV/CutsDzero/D0toKpiCuts_pp5TeV_pPbBase.root",0.,0.,"TPCFID",runPeriod,0,pSel,isRunOnMC,isReco,isMap,bRM,bRMEff,bPythia,isPrompt,"MB"); // Jet(1,..) for D*

#endif

    if(!mgr->InitAnalysis()) return;
    mgr->SetDebugLevel(2);
    mgr->PrintStatus();
    mgr->SetUseProgressBar(1, 25);

    if(local) {

        // if you want to run locally, we need to define some input
        TChain* chainAOD = new TChain("aodTree");
//        TChain *chainAODfriend = new TChain("aodTree");

        // add a few files to the chain (change this so that your local files are added)
        chainAOD->Add(Form("%s/AliAOD.root",pathToLocalAODfiles.Data()));
    //    chainAODfriend->Add(Form("%s/AliAOD.VertexingHF.root",pathToLocalAODfiles.Data()));

   //     chainAOD->AddFriend(chainAODfriend);


        // start the analysis locally, reading the events from the tchain
        mgr->StartAnalysis("local", chainAOD);



    } else {

        // if we want to run on grid, we create and configure the plugin
        AliAnalysisAlien *alienHandler = new AliAnalysisAlien();

        // also specify the include (header) paths on grid
        alienHandler->AddIncludePath("-I. -I$ROOTSYS/include -I$ALICE_ROOT -I$ALICE_ROOT/include -I$ALICE_PHYSICS/include");

        // make sure your source files get copied to grid
        //alienHandler->SetAdditionalLibs("AliHFCutOptTreeHandler.cxx AliHFCutOptTreeHandler.h");
 //       alienHandler->SetAdditionalLibs("AliHFTreeHandler.cxx AliHFTreeHandler.h AliHFTreeHandlerD0toKpi.cxx AliHFTreeHandlerD0toKpi.h AliHFTreeHandlerDplustoKpipi.cxx AliHFTreeHandlerDplustoKpipi.h AliHFTreeHandlerDstoKKpi.cxx AliHFTreeHandlerDstoKKpi.h AliHFTreeHandlerBplustoD0pi.cxx AliHFTreeHandlerBplustoD0pi.h AliHFTreeHandlerLctopKpi.cxx AliHFTreeHandlerLctopKpi.h AliAnalysisTaskSEHFTreeCreator_v1.cxx AliAnalysisTaskSEHFTreeCreator_v1.h AliAnalysisTaskSEHFTreeCreator_v1.cxx AliAnalysisTaskSEHFTreeCreator_v1.h");
 //       alienHandler->SetAnalysisSource("AliHFTreeHandler.cxx AliHFTreeHandlerD0toKpi.cxx AliHFTreeHandlerDplustoKpipi.cxx AliHFTreeHandlerDstoKKpi.cxx AliHFTreeHandlerBplustoD0pi.cxx AliHFTreeHandlerLctopKpi.cxx AliAnalysisTaskSEHFTreeCreator_v1.cxx");

        alienHandler->SetAdditionalLibs("AliAnalysisTaskFlavourJetCorrelationsQA.cxx AliAnalysisTaskFlavourJetCorrelationsQA.h AliAnalysisTaskSEDmesonsFilterCJQA.cxx AliAnalysisTaskSEDmesonsFilterCJQA.h");
        alienHandler->SetAnalysisSource("AliAnalysisTaskFlavourJetCorrelationsQA.cxx AliAnalysisTaskSEDmesonsFilterCJQA.cxx");

        // select the aliphysics version. all other packages
        // are LOADED AUTOMATICALLY!
        alienHandler->SetAliPhysicsVersion(aliPhysVersion.Data());

        // set the Alien API version
        alienHandler->SetAPIVersion("V1.1x");

        // select the input data
        alienHandler->SetGridDataDir(gridDataDir.Data());
        alienHandler->SetDataPattern(Form("%s/*AliAOD.root",gridDataPattern.Data()));
        alienHandler->SetFriendChainName("AliAOD.VertexingHF.root");

        // MC has no prefix, data has prefix 000
        if(!isRunOnMC)alienHandler->SetRunPrefix("000");


        for(Int_t k=0; k<nruns; k++){
            alienHandler->AddRunNumber(runlist[k]);
        }
        alienHandler->SetNrunsPerMaster(nruns);

        // number of files per subjob
        alienHandler->SetSplitMaxInputFileNumber(1);
        alienHandler->SetExecutable("myTask.sh");

        // specify how many seconds your job may take
        alienHandler->SetTTL(10000);
        alienHandler->SetJDLName("myTask.jdl");

        alienHandler->SetOutputToRunNo(kTRUE);
        alienHandler->SetKeepLogs(kTRUE);

        // merging: run with kTRUE to merge on grid
        // after re-running the jobs in SetRunMode("terminate")
        // (see below) mode, set SetMergeViaJDL(kFALSE)
        // to collect final results
        alienHandler->SetMaxMergeStages(3); //2, 3
        alienHandler->SetMergeViaJDL(kFALSE);

        // define the output folders
        alienHandler->SetGridWorkingDir(gridWorkingDir.Data());
        alienHandler->SetGridOutputDir(gridOutputDir.Data());

        // connect the alien plugin to the manager
        mgr->SetGridHandler(alienHandler);

        if(gridTest) {

            // speficy on how many files you want to run
            alienHandler->SetNtestFiles(1);
            // and launch the analysis
            alienHandler->SetRunMode("test");
            mgr->StartAnalysis("grid");
        }
        else {
            // else launch the full grid analysis
            alienHandler->SetRunMode(runMode.Data()); //terminate
            mgr->StartAnalysis("grid");
        }
    }
}
