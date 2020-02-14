import qbs

Project {
    minimumQbsVersion: "1.7.1"

    CppApplication {
        property string rootPath: "~/alice/sw/ubuntu1804_x86-64/ROOT/latest"
        property string AlirootPath: "~/alice/sw/ubuntu1804_x86-64/AliRoot/latest"
        property string AliPhysics: "~/alice/sw/ubuntu1804_x86-64/AliPhysics/latest"
        property string MAliEMCAL: "~/alice/sw/ubuntu1804_x86-64/AliPhysics/latest/PWG/EMCAL/macros"
        property string RooUnfPath: "~/alice/sw/ubuntu1804_x86-64/RooUnfold/V02-00-01-alice4-1"
        cpp.includePaths: [RooUnfPath + "/include", rootPath + "/include",AlirootPath + "/include",AliPhysics+"/include",MAliEMCAL+"/",AlirootPath,AliPhysics]
        cpp.libraryPaths: [RooUnfPath + "/lib", rootPath + "/lib",AlirootPath + "/lib",AliPhysics+"/lib"]
        cpp.staticLibraries: ["Gui","Core","Cling","RIO","Net","Hist","Graf","Graf3d","Gpad","Tree","Rint","Postscript","Rint","Matrix","Physics","MathCore","Thread","pthread","m","dl","tbb","Imt","ANALYSISaliceBase","ANALYSIS","STEERBase","XMLParser","EG","Geom","Minuit","VMC","ANALYSISalice","AOD","ESD","TreePlayer","XMLIO","STAT","MultiProc","ANALYSIS"]


        consoleApplication: true
        property string pDcuts: "Djets/cuts_pp/Dzero/"
        property string pDeff: "Djets/efficiency/"
        property string pDsignal: "Djets/DsignalExtraction/"
        property string pDQA: "Djets/QA/"
        property string pDResponse: "Djets/ResponseMatrix/"
        property string pPWGHF: "~/alice/AliPhysics/PWGHF/"
        property string pDsim: "Djets/POWHEGSim/"
        property string pDFD: "Djets/FDsubtraction/"
        property string pDunfold: "Djets/unfolding/"
        property string pDfinal: "Djets/finalSpectra/"
        property string pDsys: "Djets/systematics/"
        property string pDprelim: "Djets/Preliminaryplots/"


        files: [pDcuts+"makeTFile4CutsD0Jet_pp13TeV_BasepPb_normCounter.C",
                pDcuts+"var/makeTFile4CutsD0Jet_pp5TeV_root6LL1.C",
                pDcuts+"var/makeTFile4CutsD0Jet_pp5TeV_root6LL2.C",
                pDcuts+"var/makeTFile4CutsD0Jet_pp5TeV_root6LL3.C",
                pDcuts+"var/makeTFile4CutsD0Jet_pp5TeV_root6T3.C",

                "/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts/Default_AnalysisResults_Run2.root/RMconsistencyCheck.cxx",
                "Djets/configDzero.h",
                "Djets/configDzeroR4.h",
                "Djets/run.csh",
                "Djets/run_main.csh",
                "Djets/run_analysis.csh",
                "Djets/run_main_sys.csh",
            "Djets/verification.cxx",
            "Djets/treff.cxx",
                "/home/kvapil/work/analysis/pp_run2/D0jet/compare.cxx",
                "/home/kvapil/work/analysis/pp_run2/D0jet/compareZ.cxx",
                "/home/kvapil/work/analysis/pp_run2/D0jet/compareX.cxx",
                "/home/kvapil/work/analysis/pp_run2/D0jet/compareNima.cxx",
                "/home/kvapil/work/analysis/pp_run2/D0jet/toyMC.cxx",
                "/home/kvapil/work/analysis/pp_run2/D0jet/compare2.cxx",
                "/home/kvapil/work/analysis/pp_run2/D0jet/comparefinal.cxx",
                "/home/kvapil/work/analysis/pp_run2/D0jet/extractSign.cxx",
                "/home/kvapil/work/analysis/pp_run2/D0jet/prior.cxx",
            "/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts/KineComp.cxx",

                pDsignal+"getYields.csh",
                "/home/kvapil/work/analysis/pp_run2/D0jet/Effcomp/comp.cxx",

                pDeff+"DjetEfficiency.C",
                pDeff+"drawEff.C",
                pDeff+"DEfficiency.C",
            pDeff+"EstimateMCunc.C",

                pDsignal+"signalExtraction_SB.C",
                pDsignal+"signalExtraction_refl.C",
                pDsignal+"signalExtraction.h",
                pDsignal+"NimaCompare.C",

                pDResponse+"DetRM.C",
                pDResponse+"combineRM.C",

                pDFD+"subtractFD.C",

                pDsim+"plotSimSpectra.C",
                pDsim+"getSimSpectra.C",
                pDsim+"doGetSimOut.csh",

                pDunfold+"unfold_Bayes.C",
                pDunfold+"unfold_Bayes_2D.C",
                pDunfold+"unfold2Dexample.C",
                pDunfold+"unfold_SVD.C",
                pDunfold+"SparseToTree.C",

                pDfinal+"finalJetSpectra.C",
                pDfinal+"finalJetSpectraInv.C",

                pDsys+"rawYield_reflections.C",
                pDsys+"BkgSRangesComparison.C",
                pDsys+"sys_Bfeeddown.C",

                pDsys+"YieldExtraction/ExtractDJetRawYieldUncertainty.C",
                pDsys+"YieldExtraction/AliDJetRawYieldUncertaintyLocal.h",
                pDsys+"YieldExtraction/AliDJetRawYieldUncertaintyLocal.cxx",
                pDsys+"YieldExtraction/compareVariationAnalysis.C",
                pDsys+"YieldExtraction/multitrialJetPtYieldsDBins.C",
                pDsys+"unfRangesComparisonBayes.C",
                pDsys+"unfRangesComparisonSVD.C",
                pDsys+"unfRangesComparison2.C",
                pDsys+"unfPriorComparison2.C",
                pDsys+"cutsSystematics.C",
                pDsys+"JetSpectrumSys.C",
            pDsys+"doPlotForFinalSystMEoverD.C",

                pDsys+"SysRatio.cxx",
                pDsys+"SysRatio.h",
                pDsys+"runSysRatio.cxx",
            pDsys+"runSysRatioXsec.cxx",
            pDsys+"SysRatioCUT.cxx",
            pDsys+"runSysRatioCUT.cxx",

                pDQA+"runQAMC.cxx",
                pDQA+"OLoader.cxx",
                pDQA+"OLoader.h",
                pDQA+"OCanvas.cxx",
                pDQA+"OCanvas.h",

                pDprelim+"Performance/D0jet/drawSB.C",
                pDprelim+"Performance/D0jet/drawSB_jet.C",
                pDprelim+"efficiency/drawEff.C",
                pDprelim+"FD/drawFD.C",
                pDprelim+"Res/drawRes.C",

            pDprelim+"efficiency/drawEffZ.C",
            pDprelim+"FD/drawFDZ.C",
            pDprelim+"FD/drawFDZ2.C",
            pDprelim+"Performance/D0jet/drawSBZ.C",
            pDprelim+"Performance/D0jet/drawSB_jetZ.C",


                pPWGHF+"jetsHF/AliAnalysisTaskDJetCorrelations.cxx",
                pPWGHF+"jetsHF/macros/AddTaskDFilterCorrelations.C",
                pPWGHF+"jetsHF/AliAnalysisTaskDJetCorrelations.h",
                pPWGHF+"jetsHF/AliAnalysisTaskDmesonsFilterCJ.cxx",
                pPWGHF+"jetsHF/macros/AddTaskDmesonsFilterCJ.C",
                pPWGHF+"jetsHF/AliAnalysisTaskDmesonsFilterCJ.h",
                "~/alice/AliPhysics/PWGJE/EMCALJetTasks/macros/AddTaskEmcalJet.C",
                "~/alice/AliPhysics/PWGJE/EMCALJetTasks/AliEmcalJetTask.cxx",
                "~/alice/AliPhysics/PWGJE/EMCALJetTasks/AliEmcalJetTask.h",


        ]

        Group {     // Properties for the produced executable
            fileTagsFilter: "application"
            qbs.install: true
        }
    }
}
