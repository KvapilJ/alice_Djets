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

#include "AliAnalysisTaskSEDmesonsFilterCJ.h"
#include "AliAnalysisTaskDJetCorrelationsQA.h"
//#include "AddTasksFlavourJetMCQA.C"
#endif

//SETTINGS
//************************************

Bool_t runLocal=kTRUE;                                  // flag to run locally on AliAOD.root + AliAOD.VertexingHF.root
TString pathToLocalAODfiles="../..";               // path to find AOD files when running locally
Bool_t runGridTest=kTRUE;                               // flag to run a grid test: kTRUE (+runLocal=kFALSE). To run job on GRID: runGridTest=kFALSE, runLocal=kFALSE
TString runMode="full";                                  // sets the run grid mode: "full", "terminate"

TString aliPhysVersion="vAN-20181202_ROOT6-1";

Bool_t isRunOnMC= kFALSE;                //MC 1 D 0                 // set to kTRUE to run on Mone Carlo and uncomment/comment accordingly the following lines about paths on Alien
Bool_t isReco   = kTRUE;                //mc 1 dD1
//paths on Alien
// Monte Carlo
//TString gridDataDir="/alice/sim/2018/LHC18a4a2_fast/";
//TString gridDataPattern="AOD";
// Data
TString gridDataDir="/alice/sim/2018/LHC18l4a/";
TString gridDataPattern="AOD";


// Alien output director
TString gridWorkingDir="outputMC_LHC18l4a_10%";
TString gridOutputDir="output";

//run numbers
const Int_t nruns = 131;
Int_t runlist[nruns] = {274690, 274708, 274736, 274801, 274802, 274803, 274806, 274807, 274811, 274815, 274817, 274821, 274822, 274877, 274878, 274882, 274883, 274884, 274886, 274889, 274978, 274979, 275067, 275068, 275073,
                        275075, 275076, 275149, 275150, 275151, 275173, 275174, 275177, 275180, 275184, 275188, 275239, 275245, 275246, 275247, 275283, 275314, 275322, 275324, 275326, 275328, 275332, 275333, 275360, 275361,
                        275369, 275372, 275394, 275395, 275401, 275404, 275406, 275443, 275448, 275452, 275453, 275456, 275457, 275459, 275467, 275471, 275472, 275515, 275558, 275559, 275612, 275617, 275621, 275622, 275623,
                        275624, 275647, 275648, 275650, 275657, 275661, 275664, 275847, 275924, 275925, 276012, 276013, 276017, 276019, 276020, 276040, 276041, 276045, 276097, 276098, 276099, 276102, 276104, 276105, 276108,
                        276135, 276140, 276141, 276145, 276166, 276169, 276170, 276177, 276178, 276205, 276230, 276257, 276259, 276290, 276291, 276292, 276294, 276297, 276302, 276307, 276312, 276348, 276351, 276435, 276437,
                        276438, 276439, 276462, 276506, 276507, 276508};// , 282304, 282303, 282302, 282247, 282230, 282229, 282227, 282224, 282206, 282189, 282147, 282146, 282127, 282126, 282125, 282123, 282122, 282120, 282119, 282118, 282099, 282098, 282078, 282051, 282050, 282031, 282025, 282021, 282016, 282008, 282367, 282366, 282365, 282343};

//Task configuration
TString cutFile="../cuts_pp/Dzero/D0toKpiCuts_pp13TeV_pPbBase_newNorm.root";          // file containing the cuts for the different mesons
  														 // to generate the cut file: 1) move to cutfile directory
  														 //                           2) .L makeCutsTreeCreator.C
  														 //                           3) makeCutsTreeCreator();
  														 // to run with ROOT5/6 generate the cut file using AliPhysics built on ROOT5/6

const char*    runPeriod           = "LHC18l4b";                    // set the run period (used on grid)
const char*    uniqueName          = "QATest";           			// sets base string for the name of the task on the grid
UInt_t         pSel                = AliVEvent::kINT7;             // used event selection for every task except for the analysis tasks
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

    gROOT->LoadMacro("AliAnalysisTaskSEDmesonsFilterCJQA.cxx++g");
    gROOT->LoadMacro("AliAnalysisTaskDJetCorrelationsQA.cxx++g");

    //currently failing when i>1 at AliAnalysisTaskSEDmesonsFilterCJQA *filter = reinterpret_cast<AliAnalysisTaskSEDmesonsFilterCJQA*>(gInterpreter->ProcessLine
    //dont even enter AddTask...error somewhere in preprocessor???

    for(Int_t i=0; i<1  ; i++){
        TString TaskText = "MB";
        TaskText += Form("N%d",i);
        //Filtering task. It will create a set of particles with the D meson instead of the daughters

    //                        gInterpreter->ProcessLine(Form(".x %s (%d)",
    //                        gSystem->ExpandPathName("AddTasksFlavourJetMCQA.C"),0);

        AliAnalysisTaskSEDmesonsFilterCJ *filter = reinterpret_cast<AliAnalysisTaskSEDmesonsFilterCJ*>(
                    gInterpreter->ProcessLine(Form(".x %s ((AliAnalysisTaskSEDmesonsFilterCJQA::ECandidateType)%d,\"%s\",%d,%d,\"%s\")",
                    gSystem->ExpandPathName("AddTaskSEDmesonsFilterCJQA.C"),AliAnalysisTaskSEDmesonsFilterCJ::ECandidateType::kD0toKpi,
                    cutFile.Data(),
                    isRunOnMC,
                    isReco,
                    TaskText.Data())));



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

      //  AliMCParticleContainer* trackContMC = filter->AddMCParticleContainer("mcparticles");
      //  trackContMC->SetClassName("AliAODMCParticle");

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
        AliEmcalJetTask *taskFJDandTracks =reinterpret_cast<AliEmcalJetTask*>(gInterpreter->ProcessLine(Form(".x %s (\"%s\",\"%s\",(AliJetContainer::EJetAlgo_t)%d,%f,(AliJetContainer::EJetType_t)%d,%f,%f,%f,(AliJetContainer::ERecoScheme_t)%d,\"%s\",%f,%d,%d)",gSystem->ExpandPathName("$ALICE_PHYSICS/PWGJE/EMCALJetTasks/macros/AddTaskEmcalJet.C"),DcandAndTracks.Data(),"",AliJetContainer::EJetAlgo_t::antikt_algorithm,aRadius[0],AliJetContainer::EJetType_t::kFullJet,0.15,0.30,0.005,AliJetContainer::ERecoScheme_t::pt_scheme,AKTJet.Data(),0.,kFALSE,kFALSE)));
        taskFJDandTracks->SelectCollisionCandidates(pSel);
        taskFJDandTracks->SetNeedEmcalGeom(kFALSE);

        //"DcandidatesAndTracksD0MBN0","",AliJetContainer::EJetAlgo_t::antikt_algorithm,0.4,AliJetContainer::kFullJet,0.15,0.30,0.005,AliJetContainer::ERecoScheme_t::pt_scheme,"AKTJetMBN0",0.,kFALSE,kFALSE

        //Generated level jets. No pT>0.15 GeV/c cut should be applied here!
    //    AliEmcalJetTask *taskFJMCDandTracks = AddTaskEmcalJet(MCDcandAndTracks,"",AliJetContainer::antikt_algorithm,aRadius[0],AliJetContainer::kFullJet,0.0,0.0,0.005,AliJetContainer::pt_scheme,AKTJet,0.,kFALSE,kFALSE);
   //data comment
  /*           AliEmcalJetTask *taskFJMCDandTracks =reinterpret_cast<AliEmcalJetTask*>(gInterpreter->ProcessLine(Form(".x %s (\"%s\",\"%s\",(AliJetContainer::EJetAlgo_t)%d,%f,(AliJetContainer::EJetType_t)%d,%f,%f,%f,(AliJetContainer::ERecoScheme_t)%d,\"%s\",%f,%d,%d)",gSystem->ExpandPathName("$ALICE_PHYSICS/PWGJE/EMCALJetTasks/macros/AddTaskEmcalJet.C"),MCDcandAndTracks.Data(),"",AliJetContainer::EJetAlgo_t::antikt_algorithm,aRadius[0],AliJetContainer::EJetType_t::kFullJet,0.0,0.0,0.005,AliJetContainer::ERecoScheme_t::pt_scheme,AKTJet.Data(),0.,kFALSE,kFALSE)));
        taskFJMCDandTracks->SelectCollisionCandidates(pSel);
        taskFJMCDandTracks->SetNeedEmcalGeom(kFALSE);*/

        AliAnalysisTaskDJetCorrelationsQA *CorrTask = reinterpret_cast<AliAnalysisTaskDJetCorrelationsQA*>(
                    gInterpreter->ProcessLine(Form(".x %s ((AliAnalysisTaskDJetCorrelationsQA::ECandidateType)%d,\"%s\",%d,%d,\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",%f,%f,\"%s\",%f,(AliAnalysisTaskDJetCorrelationsQA::ECorrelationMethod)%d)",gSystem->ExpandPathName("$ALICE_PHYSICS/PWGHF/jetsHF/macros/AddTaskDJetCorrelationsQA.C"),AliAnalysisTaskSEDmesonsFilterCJQA::kD0toKpi,
                    cutFile.Data(),
                    isRunOnMC,
                    isReco,
                    TaskText.Data(),
                    "AKTJetMBN0_AKTFullR040_DcandidatesAndTracksD0MBN0_pT0150_pt_scheme",//taskFJDandTracks->GetName(),
                    DcandAndTracks.Data(),
                    "",
                    "",
                    "",
                    "",
                    "",
                    "",
                    aRadius[0],
                    0.0,
                    "TPCFID",
                    0.0,
                    AliAnalysisTaskDJetCorrelationsQA::kConstituent)));

        //Flag to build the Response Matrix
     //   CorrTask->SetBuildResponseMatrix(bRM);
    //    CorrTask->SetBuildResponseMatrixEff(bRMEff);
        //if to use only Pythia tracks for MC
     //   CorrTask->SetUsePythia(bPythia);

        //Container with generated level particles and D meson instead of the daughters
    /*    AliMCParticleContainer *MCpartCont  = CorrTask->AddMCParticleContainer(MCDcandAndTracks);
        AliJetContainer *jetContMC = CorrTask->AddJetContainer(taskFJMCDandTracks->GetName(),"TPC",aRadius[0]);
        if(jetContMC) {
            jetContMC->ConnectParticleContainer(MCpartCont);
            jetContMC->SetJetPtCut(0.0);
            jetContMC->SetPercAreaCut(0.0);
        }*/

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
    gROOT->LoadMacro("AliAnalysisTaskDJetCorrelationsQAloc.cxx++g");
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
        TChain *chainAODfriend = new TChain("aodTree");

        // add a few files to the chain (change this so that your local files are added)
        chainAOD->Add(Form("%s/AliAOD.root",pathToLocalAODfiles.Data()));
        chainAODfriend->Add(Form("%s/AliAOD.VertexingHF.root",pathToLocalAODfiles.Data()));

        chainAOD->AddFriend(chainAODfriend);


        // start the analysis locally, reading the events from the tchain
        mgr->StartAnalysis("local", chainAOD);


    } else {
        // if we want to run on grid, we create and configure the plugin
        AliAnalysisAlien *alienHandler = new AliAnalysisAlien();

        // also specify the include (header) paths on grid
        alienHandler->AddIncludePath("-I. -I$ROOTSYS/include -I$ALICE_ROOT -I$ALICE_ROOT/include -I$ALICE_PHYSICS/include");

        // make sure your source files get copied to grid
        alienHandler->SetAdditionalLibs("AliAnalysisTaskDJetCorrelationsQA.cxx AliAnalysisTaskDJetCorrelationsQA.h AliAnalysisTaskSEDmesonsFilterCJQA.cxx AliAnalysisTaskSEDmesonsFilterCJQA.h");
        alienHandler->SetAnalysisSource("AliAnalysisTaskDJetCorrelationsQA.cxx AliAnalysisTaskSEDmesonsFilterCJQA.cxx");

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
