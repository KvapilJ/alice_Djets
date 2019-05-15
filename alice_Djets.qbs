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

        files: [pDcuts+"makeTFile4CutsD0Jet_pp13TeV_BasepPb_normCounter.C",

                "Djets/configDzero.h",
                "Djets/run.csh",
                "Djets/run_main.csh",
                "Djets/run_analysis.csh",
                "/home/kvapil/work/analysis/pp_run2/D0jet/compare.cxx",
                "/home/kvapil/work/analysis/pp_run2/D0jet/compare2.cxx",
                "/home/kvapil/work/analysis/pp_run2/D0jet/comparefinal.cxx",
                "/home/kvapil/work/analysis/pp_run2/D0jet/extractSign.cxx",
                "/home/kvapil/work/analysis/pp_run2/D0jet/prior.cxx",
                pDsignal+"getYields.csh",

                pDeff+"DjetEfficiency.C",
                pDeff+"drawEff.C",

                pDsignal+"signalExtraction_SB.C",
                pDsignal+"signalExtraction_refl.C",
                pDsignal+"signalExtraction.h",

                pDResponse+"DetRM.C",
                pDResponse+"combineRM.C",

                pDFD+"subtractFD.C",

                pDsim+"plotSimSpectra.C",
                pDsim+"getSimSpectra.C",
                pDsim+"doGetSimOut.csh",

                pDunfold+"unfold_Bayes.C",
                pDunfold+"unfold_SVD.C",

                pDfinal+"finalJetSpectra.C",

                pDsys+"rawYield_reflections.C",
                pDsys+"BkgSRangesComparison.C",
                pDsys+"sys_Bfeeddown.C",

                pDsys+"YieldExtraction/ExtractDJetRawYieldUncertainty.C",
                pDsys+"YieldExtraction/AliDJetRawYieldUncertaintyLocal.h",
                pDsys+"YieldExtraction/AliDJetRawYieldUncertaintyLocal.cxx",
                pDsys+"YieldExtraction/compareVariationAnalysis.C",
                pDsys+"YieldExtraction/multitrialJetPtYieldsDBins.C",
                pDsys+"unfPriorComparison2.C",

                pDQA+"runQAMC.cxx",
                pDQA+"OLoader.cxx",
                pDQA+"OLoader.h",
                pDQA+"OCanvas.cxx",
                pDQA+"OCanvas.h",



                pPWGHF+"jetsHF/AliAnalysisTaskDJetCorrelationsQA.cxx",
                pPWGHF+"jetsHF/macros/AddTaskDJetCorrelationsQA.C",
                pPWGHF+"jetsHF/AliAnalysisTaskDJetCorrelationsQA.h",


        ]

        Group {     // Properties for the produced executable
            fileTagsFilter: "application"
            qbs.install: true
        }
    }
}
