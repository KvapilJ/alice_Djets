import qbs

Project {
    minimumQbsVersion: "1.7.1"

    CppApplication {
        property string rootPath: "~/alice/sw/ubuntu1804_x86-64/ROOT/latest"
        property string AlirootPath: "~/alice/sw/ubuntu1804_x86-64/AliRoot/latest"
        property string AliPhysics: "~/alice/sw/ubuntu1804_x86-64/AliPhysics/latest"
        property string MAliEMCAL: "~/alice/sw/ubuntu1804_x86-64/AliPhysics/latest/PWG/EMCAL/macros"
        cpp.includePaths: [rootPath + "/include",AlirootPath + "/include",AliPhysics+"/include",MAliEMCAL+"/",AlirootPath,AliPhysics]
        cpp.libraryPaths: [rootPath + "/lib",AlirootPath + "/lib",AliPhysics+"/lib"]
        cpp.staticLibraries: ["Gui","Core","Cling","RIO","Net","Hist","Graf","Graf3d","Gpad","Tree","Rint","Postscript","Rint","Matrix","Physics","MathCore","Thread","pthread","m","dl","tbb","Imt","ANALYSISaliceBase","ANALYSIS","STEERBase","XMLParser","EG","Geom","Minuit","VMC","ANALYSISalice","AOD","ESD","TreePlayer","XMLIO","STAT","MultiProc","ANALYSIS"]


        consoleApplication: true
        property string pDcuts: "Djets/cuts_pp/Dzero/"
        property string pDeff: "Djets/efficiency/"
        property string pDsignal: "Djets/DsignalExtraction/"
        property string pDQA: "Djets/QA/"

        files: [pDcuts+"makeTFile4CutsD0Jet_pp13TeV_BasepPb_normCounter.C",
                "Djets/configDzero_pp_JK.h",
                pDeff+"DjetEfficiency.C",
                pDeff+"drawEff.C",
                pDsignal+"signalExtraction_SB.C",
                pDsignal+"signalExtraction.h",
                pDsignal+"config.h",
                "Djets/ResponseMatrix/DetRM.C",

                //pDQA+"AddTaskDJetCorrelationsQAloc.C",
                pDQA+"AddTaskSEDmesonsFilterCJQA.C",
                pDQA+"AddTasksFlavourJetMCQA.C",
                "/home/kvapil/alice/AliPhysics/PWGHF/jetsHF/AliAnalysisTaskDJetCorrelationsQA.cxx",
                "/home/kvapil/alice/AliPhysics/PWGHF/jetsHF/macros/AddTaskDJetCorrelationsQA.C",
                "/home/kvapil/alice/AliPhysics/PWGHF/jetsHF/AliAnalysisTaskDJetCorrelationsQA.h",
                //pDQA+"AliAnalysisTaskDJetCorrelationsQAloc.cxx",
                //pDQA+"AliAnalysisTaskDJetCorrelationsQAloc.h",
                pDQA+"AliAnalysisTaskSEDmesonsFilterCJQA.cxx",
                pDQA+"AliAnalysisTaskSEDmesonsFilterCJQA.h",
                pDQA+"AnalysisTrainCorrJetsLocalQA.C",
                pDQA+"runOnGrid.C",
                "/home/kvapil/work/analysis/pp_run2/D0jet/SignalComparisonM/comparePeriods.cxx",
                "/home/kvapil/work/analysis/pp_run2/D0jet/SignalComparison/comparePeriods.cxx",
                "/home/kvapil/work/analysis/pp_run2/D0jet/data/basicQA.cxx",
                "/home/kvapil/work/analysis/pp_run2/D0jet/MC/basicQAMC.cxx",

                pDQA+"runQAMC.cxx",
                pDQA+"OLoader.cxx",
                pDQA+"OLoader.h",
                pDQA+"OCanvas.cxx",
                pDQA+"OCanvas.h",



        ]

        Group {     // Properties for the produced executable
            fileTagsFilter: "application"
            qbs.install: true
        }
    }
}
