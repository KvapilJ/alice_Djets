#include "SysRatio.h"

void runSysRatioXsec(){
    gROOT->LoadMacro("SysRatio.cxx++g");
    const Int_t nR = 2;
    TString R[nR] = {": R = 0.2, 2 < p_{T,jet} < 50 GeV/#it{c} (p_{T,D} > 2 GeV/#it{c})",
                     ": R = 0.6, 2 < p_{T,jet} < 50 GeV/#it{c} (p_{T,D} > 2 GeV/#it{c})"};

    TString defaulfileUnf = "Default_AnalysisResults_Run2.root/unfolding_Bayes_5/unfoldedSpectrum_unfoldedJetSpectrum.root";
    TString defaulfileSignal = "Default_AnalysisResults_Run2.root/signalExtraction/JetPtSpectra_SB_eff.root";
    TString path = "/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_R06/";

//B Feed-Down systematics
    SysRatio *BFD = new SysRatio(2,"B meson Feed-Down");
    BFD->SetPath(path);
    BFD->SetOutPath(path+"systematics");

    TString BFDfilelist[2] = {"Default_AnalysisResults_Run2.root/unfolding_Bayes_5_FDsysUp/unfoldedSpectrum_unfoldedJetSpectrum.root",
                              "Default_AnalysisResults_Run2.root/unfolding_Bayes_5_FDsysDown/unfoldedSpectrum_unfoldedJetSpectrum.root"};
    TString BFDlegendlist[2] ={"FD up",
                               "FD down"};

    BFD->SetDefaultFile(defaulfileUnf);
    BFD->SetVarFilelist(BFDfilelist);
    BFD->SetVarLegendDesc(BFDlegendlist);
    BFD->SetMethod(SysRatio::Method::kMax);

    BFD->SetOutFolder("BFD");
    BFD->SetTitle("BFD variation");
    BFD->SetHistoName("unfoldedSpectrum");
    BFD->Run();

//reflection
    SysRatio *REFL = new SysRatio(2,"Reflection variation");
    REFL->SetPath(path);
    REFL->SetOutPath(path+"systematics");

    TString REFLlegendlist[2] ={"Reflection -50%",
                                "Reflection +50%"};

    REFL->SetDefaultFile(defaulfileSignal);
    TString REFLfilelist[2] = {"ReflSys05_AnalysisResults_Run2.root/signalExtraction/JetPtSpectra_SB_eff.root",
                               "ReflSys15_AnalysisResults_Run2.root/signalExtraction/JetPtSpectra_SB_eff.root"};
    REFL->SetVarFilelist(REFLfilelist);
    REFL->SetVarLegendDesc(REFLlegendlist);
    REFL->SetMethod(SysRatio::Method::kMax);
    REFL->SetOutFolder("REFL");
    REFL->SetHistoName("hjetptspectrumReb");
    REFL->Run();


//SB range variation
    SysRatio *SBRANGE = new SysRatio(6,"Signal and Background range variation");
    SBRANGE->SetPath(path);
    SBRANGE->SetOutPath(path+"systematics");

    TString SBRANGEfilelist[6] = {"S2SB38_AnalysisResults_Run2.root/signalExtraction/JetPtSpectra_SB_eff.root",
                                  "S2SB410_AnalysisResults_Run2.root/signalExtraction/JetPtSpectra_SB_eff.root",
                                  "S2SB412_AnalysisResults_Run2.root/signalExtraction/JetPtSpectra_SB_eff.root",
                                  "S2SB415_AnalysisResults_Run2.root/signalExtraction/JetPtSpectra_SB_eff.root",
                                  "S3SB49_AnalysisResults_Run2.root/signalExtraction/JetPtSpectra_SB_eff.root",
                                  "S3SB412_AnalysisResults_Run2.root/signalExtraction/JetPtSpectra_SB_eff.root"};

    TString SBRANGElegendlist[6] ={"S:2#sigma, B:3-8#sigma",
                                   "S:2#sigma, B:4-10#sigma",
                                   "S:2#sigma, B:4-12#sigma",
                                   "S:2#sigma, B:4-15#sigma",
                                   "S:3#sigma, B:4-9#sigma",
                                   "S:3#sigma, B:4-12#sigma"};

    SBRANGE->SetDefaultLegend("S:2#sigma, B:4-9#sigma (Def)");
    SBRANGE->SetDefaultFile(defaulfileSignal);
    SBRANGE->SetVarFilelist(SBRANGEfilelist);
    SBRANGE->SetVarLegendDesc(SBRANGElegendlist);
    SBRANGE->SetOutFolder("SBRANGE");
    SBRANGE->SetHistoName("hjetptspectrumReb");
    SBRANGE->Run();


    //JES
    /*
    SysRatio *JES = new SysRatio(1,"JES");
    JES->SetPath("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts/");
    JES->SetOutPath("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts/Systematics");

    TString JESfilelist[1] = {"Default_AnalysisResults_Run2.root96/unfolding_2D_5/unfoldedSpectrum_unfoldedJetSpectrum.root"};
    TString JESlegendlist[1] ={"inefficiency 4%"};

 //   JES->SetDefaultFile(defaulfile);
    JES->SetVarFilelist(JESfilelist);
    JES->SetVarLegendDesc(JESlegendlist);

    for(Int_t z = 1; z <= 5; z++){
        JES->SetOutFolder(Form("JES%d",z));
   //     JES->SetTitle("JES"+zbin[z-1]);
        JES->SetHistoName(Form("unfoldedSpectrumKineEff%d",z));
        JES->Run();
    }
*/
//CUTS
    SysRatio *CUTS = new SysRatio(4,"CUTS");
    CUTS->SetPath(path);
    CUTS->SetOutPath(path+"systematics");
    TString CUTSdefaulfile = "Cuts_VarDef/Default/unfolding_Bayes_5/unfoldedSpectrum_unfoldedJetSpectrum.root";

    TString CUTSfilelist[4] = {"Cuts_L2/Default/unfolding_Bayes_5/unfoldedSpectrum_unfoldedJetSpectrum.root",
                               "Cuts_L3/Default/unfolding_Bayes_5/unfoldedSpectrum_unfoldedJetSpectrum.root",
                               "Cuts_T2/Default/unfolding_Bayes_5/unfoldedSpectrum_unfoldedJetSpectrum.root",
                               "Cuts_T3/Default/unfolding_Bayes_5/unfoldedSpectrum_unfoldedJetSpectrum.root",
                              };
    TString CUTSlegendlist[4] ={"Looser 1",
                                "Looser 2",
                                "Tighter 1",
                                "Tighter 2",
                               };

    CUTS->SetDefaultFile(CUTSdefaulfile);
    CUTS->SetVarFilelist(CUTSfilelist);
    CUTS->SetVarLegendDesc(CUTSlegendlist);
    CUTS->SetMethod(SysRatio::Method::kError);

    CUTS->SetOutFolder("CUTS");
    CUTS->SetHistoName("unfoldedSpectrum");
    CUTS->Run();
//CUS efficiency
    SysRatio *CUTSEFF = new SysRatio(4,"CUTSEFF");
    CUTSEFF->SetPath(path);
    CUTSEFF->SetOutPath(path+"systematics");

    TString CUTSEFFlegendlist[4] ={"Looser 1",
                                   "Looser 2",
                                   "Tighter 1",
                                   "Tighter 2",
                                  };

    const char *promp[2] = {"nonPrompt","prompt"};

    for(Int_t prompt = 0; prompt <=1; prompt++){
        TString CUTSEFFdefaulfile = Form("Cuts_VarDef/Default/efficiency/DjetEff_%s_jetpt5.00_50.00.root",promp[prompt]);
        CUTSEFF->SetDefaultFile(CUTSEFFdefaulfile);
        TString CUTSEFFfilelist[4];
        CUTSEFFfilelist[0]= Form("Cuts_L2/Default/efficiency/DjetEff_%s_jetpt5.00_50.00.root",promp[prompt]);
        CUTSEFFfilelist[1]= Form("Cuts_L3/Default/efficiency/DjetEff_%s_jetpt5.00_50.00.root",promp[prompt]);
        CUTSEFFfilelist[2]= Form("Cuts_T2/Default/efficiency/DjetEff_%s_jetpt5.00_50.00.root",promp[prompt]);
        CUTSEFFfilelist[3]= Form("Cuts_T3/Default/efficiency/DjetEff_%s_jetpt5.00_50.00.root",promp[prompt]);
        CUTSEFF->SetVarFilelist(CUTSEFFfilelist);
        CUTSEFF->SetVarLegendDesc(CUTSEFFlegendlist);
        CUTSEFF->SetMethod(SysRatio::Method::kError);
        CUTSEFF->SetOutFolder(Form("CUTSEFF%s",promp[prompt]));
        CUTSEFF->SetHistoName("hEff_reb");
        CUTSEFF->Run();
    }


//Unfolding Iterations
    SysRatio *UNFITER = new SysRatio(11,"Unfolding Bayes iterations and ranges");
    UNFITER->SetPath(path);
    UNFITER->SetOutPath(path+"systematics");

    TString UNFITERfilelist[11] = {"Default_AnalysisResults_Run2.root/unfolding_Bayes_4/finalSpectra/JetPtSpectrum_final.root",
                                  "Default_AnalysisResults_Run2.root/unfolding_Bayes_6/finalSpectra/JetPtSpectrum_final.root",
                                  "T350M250_AnalysisResults_Run2.root/unfolding_Bayes_5/finalSpectra/JetPtSpectrum_final.root",
                                  "T350M350_AnalysisResults_Run2.root/unfolding_Bayes_5/finalSpectra/JetPtSpectrum_final.root",
                                  "T450M250_AnalysisResults_Run2.root/unfolding_Bayes_5/finalSpectra/JetPtSpectrum_final.root",
                                  "T450M350_AnalysisResults_Run2.root/unfolding_Bayes_5/finalSpectra/JetPtSpectrum_final.root",
                                  "T450M450_AnalysisResults_Run2.root/unfolding_Bayes_5/finalSpectra/JetPtSpectrum_final.root",
                                  "T550M250_AnalysisResults_Run2.root/unfolding_Bayes_5/finalSpectra/JetPtSpectrum_final.root",
                                  "T550M350_AnalysisResults_Run2.root/unfolding_Bayes_5/finalSpectra/JetPtSpectrum_final.root",
                                  "T550M450_AnalysisResults_Run2.root/unfolding_Bayes_5/finalSpectra/JetPtSpectrum_final.root",
                                  "T550M550_AnalysisResults_Run2.root/unfolding_Bayes_5/finalSpectra/JetPtSpectrum_final.root"



                                 };
    TString UNFITERlegendlist[11] ={"meas: 2-50, true: 2-50, Bayes reg=4",
                                    "meas: 2-50, true: 2-50, Bayes reg=6",
                                    "meas: 2-50, true: 3-50, Bayes reg=5",
                                    "meas: 3-50, true: 3-50, Bayes reg=5",
                                    "meas: 2-50, true: 4-50, Bayes reg=5",
                                    "meas: 3-50, true: 4-50, Bayes reg=5",
                                    "meas: 4-50, true: 4-50, Bayes reg=5",
                                    "meas: 2-50, true: 5-50, Bayes reg=5",
                                    "meas: 3-50, true: 5-50, Bayes reg=5",
                                    "meas: 4-50, true: 5-50, Bayes reg=5",
                                    "meas: 5-50, true: 5-50, Bayes reg=5",
                                   };

    UNFITER->SetDefaultFile("Default_AnalysisResults_Run2.root/unfolding_Bayes_5/finalSpectra/JetPtSpectrum_final.root");
    UNFITER->SetVarFilelist(UNFITERfilelist);
    UNFITER->SetVarLegendDesc(UNFITERlegendlist);
    UNFITER->SetOutFolder("UNFITERRANGE");
    UNFITER->SetHistoName("hData_binned");
    UNFITER->Run();


//Prior Variation
    SysRatio *PRIOR = new SysRatio(9,"Prior variation");
    PRIOR->SetPath(path);
    PRIOR->SetOutPath(path+"systematics");

    TString PRIORfilelist[9] = {"Default_AnalysisResults_Run2.root/unfolding_Bayes_5_priorType0/unfoldedSpectrum_unfoldedJetSpectrum.root",
                                "Default_AnalysisResults_Run2.root/unfolding_Bayes_5_priorType1/unfoldedSpectrum_unfoldedJetSpectrum.root",
                                "Default_AnalysisResults_Run2.root/unfolding_Bayes_5_priorType2/unfoldedSpectrum_unfoldedJetSpectrum.root",
                                "Default_AnalysisResults_Run2.root/unfolding_Bayes_5_priorType3/unfoldedSpectrum_unfoldedJetSpectrum.root",
                                "Default_AnalysisResults_Run2.root/unfolding_Bayes_5_priorType4/unfoldedSpectrum_unfoldedJetSpectrum.root",
                                "Default_AnalysisResults_Run2.root/unfolding_Bayes_5_priorType5/unfoldedSpectrum_unfoldedJetSpectrum.root",
                                "Default_AnalysisResults_Run2.root/unfolding_Bayes_5_priorType6/unfoldedSpectrum_unfoldedJetSpectrum.root",
                                "Default_AnalysisResults_Run2.root/unfolding_Bayes_5_priorType7/unfoldedSpectrum_unfoldedJetSpectrum.root",
                                "Default_AnalysisResults_Run2.root/unfolding_Bayes_5_priorType8/unfoldedSpectrum_unfoldedJetSpectrum.root"};
    TString PRIORlegendlist[9] ={"prior0",
                                 "prior1",
                                 "prior2",
                                 "prior3",
                                 "prior4",
                                 "prior5",
                                 "prior6",
                                 "prior7",
                                 "prior8"
                               };

    PRIOR->SetDefaultFile(defaulfileUnf);
    PRIOR->SetVarFilelist(PRIORfilelist);
    PRIOR->SetVarLegendDesc(PRIORlegendlist);
    PRIOR->SetOutFolder("PRIOR");
    PRIOR->SetHistoName("unfoldedSpectrum");
    PRIOR->Run();

//SVD Variation
    SysRatio *UNFSVD = new SysRatio(3,"SVD variation");
    UNFSVD->SetPath(path);
    UNFSVD->SetOutPath(path+"systematics");

    TString UNFSVDfilelist[9] = {"Default_AnalysisResults_Run2.root/unfolding_SVD_5/unfoldedSpectrum_unfoldedJetSpectrum.root",
                                "Default_AnalysisResults_Run2.root/unfolding_SVD_6/unfoldedSpectrum_unfoldedJetSpectrum.root",
                                "Default_AnalysisResults_Run2.root/unfolding_SVD_7/unfoldedSpectrum_unfoldedJetSpectrum.root",
                                };
    TString UNFSVDlegendlist[9] ={"SVD iter=5",
                                  "SVD iter=6",
                                  "SVD iter=7",
                               };

    UNFSVD->SetDefaultFile(defaulfileUnf);
    UNFSVD->SetVarFilelist(UNFSVDfilelist);
    UNFSVD->SetVarLegendDesc(UNFSVDlegendlist);
    UNFSVD->SetOutFolder("SVD");
    UNFSVD->SetHistoName("unfoldedSpectrum");
    UNFSVD->Run();


}
