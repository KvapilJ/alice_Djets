//this is the macro that submit the job

#include <ctime>
#include <iostream>
#if !defined (__CINT__) || defined (__CLING__)
#include "TGrid.h"
#include "AliAnalysisGrid.h"
#include "AliVEvent.h"
#include "TSystem.h"
#include "TStopwatch.h"
#include "AliAnalysisManager.h"
#include "AliAODInputHandler.h"
#include "AliESDInputHandler.h"
#include "TROOT.h"
#include "AliAnalysisAlien.h"
#include "TMacro.h"

#include "AddTasksFlavourJetMCQA.C"


//#include "PWG/EMCAL/macros/CreateAODChain.C"
//#include "PWG/EMCAL/macros/CreateESDChain.C"

#endif

void LoadLibs();

AliAnalysisGrid* CreateAlienHandler(const char* uniqueName, const char* gridDir, const char* gridMode, const char* runNumbers, const Int_t nrunspermaster,
                                    const char* pattern, TString additionalCode, TString additionalHeaders, Int_t maxFilesPerWorker,
                                    Int_t workerTTL, Bool_t isMC);

//______________________________________________________________________________
void AnalysisTrainCorrJetsLocalQA (
                                 const char*    dataType            = "AOD",                       // set the analysis type, AOD, ESD or sESD
                                 Bool_t         useGrid             = kFALSE,                      // local or grid
                                 TString        localfilename       = "files_aod.txt",
                                 const char*    gridMode            = "test",                      // set the grid run mode (can be "full", "test", "offline", "submit" or "terminate")
                                 const char*    pattern             = "/AOD/*/AliAOD.root",   		// file pattern (here one can specify subdirs like passX etc.) (used on grid)
                                 const char*    gridDir             = "/alice/sim/2017/LHC17d2a_fast_new/",
                                 const char*    runNumbers          = "265525 265334",
                                 const Int_t    nrunspermaster      = 50,
                                 UInt_t         numLocalFiles       = 2,                          // number of files analyzed locally
                                 const char*    runPeriod           = "lhc16q",                    // set the run period (used on grid)
                                 const char*    uniqueName          = "QATest",           			// sets base string for the name of the task on the grid
                                 UInt_t         pSel                = AliVEvent::kINT7,             // used event selection for every task except for the analysis tasks
                                 Bool_t         useTender           = kFALSE,                      // trigger, if tender task should be used
                                 Bool_t         isMC                = kTRUE,                      // trigger, if MC handler should be used
                                 Bool_t         isReco              = kTRUE,
                                 Bool_t         isMap               = kFALSE,
                                 Bool_t         bRM                 = kFALSE,
                                 Bool_t         bRMEff              = kTRUE,
                                 Bool_t         bPythia             = kFALSE,
                                 Bool_t         isPrompt            = kTRUE,
                                 // Here you have to specify additional code files you want to use but that are not in aliroot
                                 const char*    addCXXs             = "AliAnalysisTaskFlavourJetCorrelationsQA.cxx AliAnalysisTaskSEDmesonsFilterCJQA.cxx", // to add local tasks
                                 const char*    addHs               = "AliAnalysisTaskFlavourJetCorrelationsQA.h AliAnalysisTaskSEDmesonsFilterCJQA.h",
                                 // These two settings depend on the dataset and your quotas on the AliEN services
                                 Int_t          maxFilesPerWorker   = 5,
                                 Int_t          workerTTL           = 14400, // 3600, // 14400,
                                 Int_t          nfiletestmode       = 1
                                 )
{
    
    // Some pre-settings and constants
    TStopwatch watch;
    watch.Start();
    
    enum AlgoType {kKT, kANTIKT};
    enum JetType  {kFULLJETS, kCHARGEDJETS, kNEUTRALJETS};
    gSystem->SetFPEMask();
    gSystem->Setenv("ETRAIN_ROOT", ".");
    gSystem->Setenv("ETRAIN_PERIOD", runPeriod);
    // change this objects to strings
    TString usedData(dataType);
    TString additionalCXXs(addCXXs);
    TString additionalHs(addHs);
    std::cout << dataType << " analysis chosen" << std::endl;
    if (useGrid)
    {
        std::cout << "-- using AliEn grid.\n";
        if (usedData == "sESD")
        {
            std::cout << "Skimmed ESD analysis not available on the grid!" << std::endl;
            return;
        }
    }
    else
        std::cout << "-- using local analysis.\n";
    
    
    // Load necessary libraries
    LoadLibs();
    
    // Create analysis manager
    AliAnalysisManager* mgr = new AliAnalysisManager(uniqueName);
    
    // Check type of input and create handler for it
    TString localFiles("-1");
    if(usedData == "AOD")
    {
        localFiles = localfilename;
       // gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/train/AddAODHandler.C");
      //  AliAODInputHandler* aodH = AddAODHandler();
        TMacro aodinadd(gSystem->ExpandPathName("$ALICE_ROOT/ANALYSIS/macros/train/AddAODHandler.C"));
        AliAODInputHandler *aodH = reinterpret_cast<AliAODInputHandler *>(aodinadd.Exec());
    }
    else if((usedData == "ESD") || (usedData == "sESD"))
    {
        if (usedData == "ESD")
            localFiles = "files_esd.txt";
        else
            localFiles = "files_sesd.txt";
        
     //   gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/train/AddESDHandler.C");
     //   AliESDInputHandler* esdH = AddESDHandler();
        TMacro esdinadd(gSystem->ExpandPathName("$ALICE_ROOT/ANALYSIS/macros/train/AddESDHandler.C"));
        AliESDInputHandler *esdH = reinterpret_cast<AliESDInputHandler *>(esdinadd.Exec());
    }
    else
    {
        std::cout << "Data type not recognized! You have to specify ESD, AOD, or sESD!\n";
    }
    
    if(!useGrid)
        std::cout << "Using " << localFiles.Data() << " as input file list.\n";
    
    TGrid::Connect("alien://"); //For Alien connection
    gROOT->LoadMacro("AliAnalysisTaskSEDmesonsFilterCJQA.cxx++g");
    gROOT->LoadMacro("AliAnalysisTaskFlavourJetCorrelationsQA.cxx++g");
    gROOT->LoadMacro("AddTasksFlavourJetMCQA.C");
   
     AddTasksFlavourJetMCQA(0,"alien:///alice/cern.ch/user/b/btrzecia/pp5TeV/CutsDzero/D0toKpiCuts_pp5TeV_pPbBase.root",0.,0.,"TPCFID",runPeriod,0,pSel,isMC,isReco,isMap,bRM,bRMEff,bPythia,isPrompt,"MB"); // Jet(1,..) for D*
    
    
    
    // Set the physics selection for all given tasks
  /*  TObjArray *toptasks = mgr->GetTasks();
    for (Int_t i=0; i<toptasks->GetEntries(); ++i)
    {
        AliAnalysisTaskSE *task = dynamic_cast<AliAnalysisTaskSE*>(toptasks->At(i));
        if (!task)
            continue;
        if (task->InheritsFrom("AliPhysicsSelectionTask"))
            continue;
        ::Info("setPSel", "Set physics selection for %s (%s)", task->GetName(), task->ClassName());
        task->SelectCollisionCandidates(pSel);
    }
    */
    
    if(gridMode=="full") mgr->SetUseProgressBar(1, 25);
    

    if (!mgr->InitAnalysis())
        return;
    mgr->PrintStatus();
    
    if (useGrid)
    {  // GRID CALCULATION
        
        AliAnalysisGrid *plugin = CreateAlienHandler(uniqueName, gridDir, gridMode, runNumbers, nrunspermaster, pattern, additionalCXXs, additionalHs, maxFilesPerWorker, workerTTL, isMC);
        plugin->SetNtestFiles(nfiletestmode);
        
        mgr->SetGridHandler(plugin);
        
        // start analysis
        std::cout << "Starting GRID Analysis...";
        if(gridMode=="test") mgr->SetDebugLevel(10);
        else mgr->SetDebugLevel(0);
        mgr->StartAnalysis("grid");
    }
    else
    {  // LOCAL CALCULATION
        
        TChain* chain = 0;
        if (usedData == "AOD")
        {
            Printf("Run Create AOD Chain");

                TMacro aodadd(gSystem->ExpandPathName("$ALICE_PHYSICS/PWG/EMCAL/macros/CreateAODChain.C"));
                chain = reinterpret_cast<TChain *>(aodadd.Exec("localFiles.Data(),numLocalFiles,0,kTRUE,kTRUE"));

        //        gROOT->LoadMacro("$ALICE_PHYSICS/PWG/EMCAL/macros/CreateAODChain.C");
        //        chain = CreateAODChain(localFiles.Data(), numLocalFiles,0,kTRUE,kTRUE);

            //Printf("Chain Friend has %d entries", ((TTree*)chain->GetFriend())->GetEntriesFast());
        }
        else
        {  // ESD or skimmed ESD
            TMacro esdadd(gSystem->ExpandPathName("$$ALICE_PHYSICS/PWG/EMCAL/macros/CreateESDChain.C"));
            chain = reinterpret_cast<TChain *>(esdadd.Exec("localFiles.Data(),numLocalFiles"));

       //     gROOT->LoadMacro("$ALICE_PHYSICS/PWG/EMCAL/macros/CreateESDChain.C");
       //     chain = CreateESDChain(localFiles.Data(), numLocalFiles);

        }
        
        // start analysis
        std::cout << "Starting LOCAL Analysis...";
        mgr->SetDebugLevel(10);
        mgr->StartAnalysis("local", chain);
    }
    watch.Stop();
    watch.Print();
}

//______________________________________________________________________________
void LoadLibs()
{
    // Load common libraries (better too many than too few)
 
    //gSystem->Load("libPythia6");
    //gSystem->Load("libEGPythia6");
    //gSystem->Load("liblhapdf");
    //gSystem->Load("libAliPythia6");
    
    gSystem->Load("libTree");
    gSystem->Load("libGeom");
    gSystem->Load("libVMC");
    gSystem->Load("libPhysics");
    // Add aditional libraries
    gSystem->Load("libTree");
    gSystem->Load("libMinuit");
    gSystem->Load("libProof");
    gSystem->Load("libSTEERBase");
    gSystem->Load("libESD");
    gSystem->Load("libAOD");
    // Load analysis framework libraries
    gSystem->Load("libSTEERBase");
    gSystem->Load("libESD");
    gSystem->Load("libAOD");
    gSystem->Load("libANALYSIS");
    gSystem->Load("libANALYSISalice");
    gSystem->Load("libOADB");
    gSystem->Load("libCORRFW");
    gSystem->Load("libCGAL");
    gSystem->Load("libfastjet");
    gSystem->Load("libsiscone");
    gSystem->Load("libsiscone_spherical");
    gSystem->Load("libfastjetplugins");
    gSystem->Load("libfastjettools");
    gSystem->Load("libfastjetcontribfragile");
    // Add aditional AliRoot libraries
    gSystem->Load("libCORRFW");
    gSystem->Load("libPWGflowBase");
    gSystem->Load("libPWGflowTasks");
    gSystem->Load("libPWGHFbase");
    gSystem->Load("libPWGHFvertexingHF");
    gSystem->Load("libCORRFW");
    gSystem->Load("libPWGflowBase");
    gSystem->Load("libPWGflowTasks");
    gSystem->Load("libPWGHFbase");
    gSystem->Load("libPWGHFvertexingHF");
    
       // include paths
    gSystem->AddIncludePath("-Wno-deprecated");
    gSystem->AddIncludePath("-I$ALICE_ROOT -I$ALICE_PHYSICS -I$ALICE_PHYSICS/include -I$ALICE_ROOT/EMCAL -I$ALICE_PHYSICS/PWG/EMCAL -I$ALICE_PHYSICS/PWGJE -I$ALICE_PHYSICS/PWGJE/EMCALJetTasks -I$ALICE_PHYSICS/PWGJE/EMCALJetTasks/UserTasks -I$ALICE_ROOT/ANALYSIS/Tender -I$ALICE_ROOT/ANALYSIS/TenderSupplies");
    gSystem->AddIncludePath(" -I$ROOTSYS/include -I$ALICE_PHYSICS/PWGHF/ -I$ALICE_PHYSICS/PWGHF/base -I$ALICE_PHYSICS/PWGHF/vertexingHF -I$ALICE_PHYSICS/PWGJE/FlavourJetTasks");
    gSystem->AddIncludePath("-I$ALICE_ROOT -I$ALICE_ROOT/include -I$ALICE_ROOT/EMCAL -I$ALICE_PHYSICS/EMCAL -I$ALICE_PHYSICS");
    gSystem->AddIncludePath("-I$ALICE_PHYSICS/PWGDQ/dielectron -I$ALICE_PHYSICS/PWGHF/hfe");
    gSystem->AddIncludePath("-I$ALICE_PHYSICS/JETAN -I$ALICE_PHYSICS/JETAN/fastjet");}
AliAnalysisGrid* CreateAlienHandler(const char* uniqueName, const char* gridDir, const char* gridMode, const char* runNumbers, const Int_t nrunspermaster,
                                    const char* pattern, TString additionalCode, TString additionalHeaders, Int_t maxFilesPerWorker,
                                    Int_t workerTTL, Bool_t isMC)
{
    TDatime currentTime;
    TString tmpName(uniqueName);
   
    TString tmpAdditionalLibs("");
    // tmpAdditionalLibs = Form("libTree.so libGeom.so libVMC.so libPhysics.so libMinuit.so libMinuit2.so libProof.so libSTEERBase.so libESD.so libAOD.so libOADB.so libANALYSIS.so libCDB.so libRAWDatabase.so libSTEER.so libEVGEN.so libANALYSISalice.so libCORRFW.so libESDfilter.so libSTAT.so libPWGTools.so libPWGHFbase.so libPWGflowBase.so libPWGflowTasks.so libPWGHFvertexingHF.so libEMCALUtils.so libPHOSUtils.so libPWGCaloTrackCorrBase.so libEMCALraw.so libEMCALbase.so libEMCALrec.so libTRDbase.so libVZERObase.so libVZEROrec.so libPWGEMCAL.so libPWGGAEMCALTasks.so libPWGTools.so libPWGCFCorrelationsBase.so libPWGCFCorrelationsDPhi.so libCGAL.so libfastjet.so libsiscone.so libsiscone_spherical.so libfastjetplugins.so libfastjettools.so libfastjetcontribfragile.so libPWGJE.so libPWGmuon.so libPWGJEEMCALJetTasks.so libPWGJEFlavourJetTasks.so %s %s",additionalCode.Data(),additionalHeaders.Data());
     tmpAdditionalLibs = Form("libTree.so libGeom.so libVMC.so libPhysics.so libMinuit.so libMinuit2.so libProof.so libSTEERBase.so libESD.so libAOD.so libOADB.so libANALYSIS.so libCDB.so libRAWDatabase.so libSTEER.so libEVGEN.so libANALYSISalice.so libCORRFW.so libESDfilter.so libSTAT.so libPWGTools.so libPWGHFbase.so libPWGflowBase.so libPWGflowTasks.so libPWGHFvertexingHF.so libEMCALUtils.so libPHOSUtils.so libPWGCaloTrackCorrBase.so libEMCALraw.so libEMCALbase.so libEMCALrec.so libTRDbase.so libVZERObase.so libVZEROrec.so libPWGGAEMCALTasks.so libPWGTools.so libPWGCFCorrelationsBase.so libPWGCFCorrelationsDPhi.so libCGAL.so libfastjet.so libsiscone.so libsiscone_spherical.so libfastjetplugins.so libfastjettools.so libfastjetcontribfragile.so libPWGJE.so libPWGmuon.so libPWGJEEMCALJetTasks.so libPWGJEFlavourJetTasks.so %s %s",additionalCode.Data(),additionalHeaders.Data());
    
    TString macroName("");
    TString execName("");
    TString jdlName("");
    macroName = Form("%s.C", tmpName.Data());
    execName = Form("%s.sh", tmpName.Data());
    jdlName = Form("%s.jdl", tmpName.Data());
    
    AliAnalysisAlien *plugin = new AliAnalysisAlien();
    plugin->SetOverwriteMode();
    plugin->SetRunMode(gridMode);
    
    // Here you can set the (Ali)ROOT version you want to use
    plugin->SetAPIVersion("V1.1x");
    //plugin->SetROOTVersion("v5-34-30-alice-3");
    //plugin->SetAliROOTVersion("v5-07-01-4");
    plugin->SetAliPhysicsVersion("vAN-20181202_ROOT6-1");
    plugin->SetGridDataDir(gridDir); // e.g. "/alice/sim/LHC10a6"
    plugin->SetDataPattern(pattern); //dir structure in run directory
    //plugin->SetFriendChainName("./AliAOD.VertexingHF.root");
    plugin->SetFriendChainName("AliAOD.VertexingHF.root");
    if (!isMC)
        plugin->SetRunPrefix("000");
    
    plugin->AddRunList(runNumbers);
    plugin->SetNrunsPerMaster(nrunspermaster);
    
    plugin->SetGridWorkingDir(Form("%s",tmpName.Data()));
    plugin->SetGridOutputDir("output");
    
    plugin->SetAnalysisSource(additionalCode.Data());
    plugin->SetAdditionalLibs(tmpAdditionalLibs.Data());
    
    plugin->AddIncludePath("-I. -I$ROOTSYS/include -I$ALICE_ROOT -I$ALICE_ROOT/include -I$ALICE_PHYSICS/include -I$ALICE_PHYSICS/PWG/EMCAL -I$ALICE_ROOT/EMCAL -I$ALICE_ROOT/STEER/STEER -I$ALICE_ROOT/STEER/STEERBase -I$ALICE_ROOT/STEER/ESD -I$ALICE_ROOT/STEER/AOD -I$ALICE_ROOT/ANALYSIS/ -I$ALICE_ROOT/ANALYSIS/Tender -I$ALICE_ROOT/ANALYSIS/TenderSupplies -I$ALICE_ROOT/ANALYSIS/ESDfilter -I$ALICE_PHYSICS/OADB -I$ALICE_PHYSICS/PWGHF -I$ALICE_PHYSICS/PWGHF/base -I$ALICE_PHYSICS/PWGHF/vertexingHF -I$ALICE_PHYSICS/PWG/FLOW/Base -I$ALICE_PHYSICS/PWG/FLOW/Tasks  -I$ALICE_PHYSICS/PWGJE  -I$ALICE_PHYSICS/JETAN -I$ALICE_PHYSICS/PWGJE/EMCALJetTasks -I$ALICE_PHYSICS/PWGJE/FlavourJetTasks -I$ALICE_PHYSICS/PWGJE/EMCALJetTasks/UserTasks -g");
    
    //plugin->AddExternalPackage("boost::v1.59.0-1");
    //plugin->AddExternalPackage("cgal::v4.6.3-3");
    //plugin->AddExternalPackage("fastjet::v3.1.3_1.020-3");
    
    plugin->SetDefaultOutputs(kTRUE);
    // merging via jdl
    plugin->SetMergeViaJDL(kTRUE);
    //plugin->SetMergeViaJDL(kFALSE);
    plugin->SetOneStageMerging(kFALSE);
    plugin->SetMaxMergeStages(2);
    
    //plugin->SetMergeExcludes("");
    plugin->SetAnalysisMacro(macroName.Data());
    plugin->SetSplitMaxInputFileNumber(maxFilesPerWorker);
    plugin->SetExecutable(execName.Data());
    plugin->SetTTL(workerTTL);
    plugin->SetInputFormat("xml-single");
    plugin->SetJDLName(jdlName.Data());
    plugin->SetPrice(1);      
    plugin->SetSplitMode("se");
    
    return plugin;
}
