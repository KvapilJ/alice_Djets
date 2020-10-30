#include "SysRatio.h"

void runSysRatioXsec(){
    gROOT->LoadMacro("SysRatio.cxx++g");
    const Int_t nR = 2;
    TString R[nR] = {": R = 0.2, 2 < p_{T,jet} < 50 GeV/#it{c} (p_{T,D} > 2 GeV/#it{c})",
                     ": R = 0.6, 2 < p_{T,jet} < 50 GeV/#it{c} (p_{T,D} > 2 GeV/#it{c})"};

    TString defaulfileUnf = "Default_AnalysisResults_Run2w18b.root/unfolding_Bayes_5/unfoldedSpectrum_unfoldedJetSpectrum.root";
    TString defaulfileSignal = "Default_AnalysisResults_Run2w18b.root/signalExtraction/JetPtSpectra_SB_eff.root";
    TString path = "/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts/";
/*
    SysRatio *TPC = new SysRatio(4,"TPC");
    TPC->SetPath(path);
    TPC->SetOutPath(path+"systematics");

    TString TPCfilelist[4] = {"Cuts_L2/Default/unfolding_Bayes_5/unfoldedSpectrum_unfoldedJetSpectrum.root",
                               "Cuts_L3/Default/unfolding_Bayes_5/unfoldedSpectrum_unfoldedJetSpectrum.root",
                               "Cuts_T2/Default/unfolding_Bayes_5/unfoldedSpectrum_unfoldedJetSpectrum.root",
                               "Cuts_T3/Default/unfolding_Bayes_5/unfoldedSpectrum_unfoldedJetSpectrum.root",
                              };
    TString TPClegendlist[4] ={"Rows > 70, Rows/Clusters > 0.8",
                                "Rows > 120, Rows/Clusters > 0.8",
                                "Clusters > 56",
                                "Clusters > 70",

                               };

    TPC->SetDefaultFile("Cuts_Def/Default_AnalysisResults_18.root/unfolding_Bayes_5/unfoldedSpectrum_unfoldedJetSpectrum.root");
    TPC->SetVarFilelist(TPCfilelist);
    TPC->SetVarLegendDesc(TPClegendlist);
    TPC->SetAxisTitle("p_{T, ch. jet}","dN/dp_{T, ch. jet}");
    TPC->SetRatioRangesY(0.8,1.2);
    //TPC->SetMethod(SysRatio::Method::kError);

    TPC->SetOutFolder("CUTS");
    TPC->SetHistoName("unfoldedSpectrum");
    TPC->Run();
return;
*/

    SysRatio *UNFClosure = new SysRatio(1,"Unfolding Bayes Closure");
    UNFClosure->SetPath(path);
    UNFClosure->SetOutPath(path+"systematics");

    TString UNFClosurefilelist[1] = {"Default_AnalysisResults_Run2w18b.root/unfolding_BayesClosure_5/unfoldedSpectrum_unfoldedJetSpectrum.root"
                             };
    TString UNFClosurelegendlist[1] ={"True projection"};

    UNFClosure->SetDefaultFile("Default_AnalysisResults_Run2w18b.root/ResponseMatrix/DetMatrixClosureData_prompt.root");
    UNFClosure->SetVarFilelist(UNFClosurefilelist);
    UNFClosure->SetVarLegendDesc(UNFClosurelegendlist);
    UNFClosure->SetMethod(SysRatio::Method::kMax);
    UNFClosure->SetOutFolder("UNFClosure");
    UNFClosure->SetHistoName("unfoldedSpectrum");
    UNFClosure->SetAxisTitle("p_{T, ch. jet}","dN/dp_{T, ch. jet}");
    UNFClosure->SetRatioRangesY(0.8,1.2);
    UNFClosure->Run();

    return;


//B Feed-Down systematics up
    SysRatio *BFDu = new SysRatio(1,"B meson Feed-Down up");
    BFDu->SetPath(path);
    BFDu->SetOutPath(path+"systematics");

    TString BFDufilelist[1] = {"Default_AnalysisResults_Run2w18b.root/unfolding_Bayes_5_FDsysUp/unfoldedSpectrum_unfoldedJetSpectrum.root"
                             };
    TString BFDulegendlist[1] ={"FD up"};

    BFDu->SetDefaultFile(defaulfileUnf);
    BFDu->SetVarFilelist(BFDufilelist);
    BFDu->SetVarLegendDesc(BFDulegendlist);
    BFDu->SetMethod(SysRatio::Method::kMax);

    BFDu->SetOutFolder("BFDu");
    BFDu->SetTitle("BFD variation up");
    BFDu->SetAxisTitle("p_{T, ch. jet}","dN/dp_{T, ch. jet}");
    BFDu->SetRatioRangesY(0.5,1.5);
    BFDu->SetHistoName("unfoldedSpectrum");
    BFDu->Run();

//B Feed-Down systematics down
    SysRatio *BFDd = new SysRatio(1,"B meson Feed-Down down");
    BFDd->SetPath(path);
    BFDd->SetOutPath(path+"systematics");

    TString BFDdfilelist[1] = {"Default_AnalysisResults_Run2w18b.root/unfolding_Bayes_5_FDsysDown/unfoldedSpectrum_unfoldedJetSpectrum.root"};
    TString BFDdlegendlist[1] ={
                               "FD down"};

    BFDd->SetDefaultFile(defaulfileUnf);
    BFDd->SetVarFilelist(BFDdfilelist);
    BFDd->SetVarLegendDesc(BFDdlegendlist);
    BFDd->SetMethod(SysRatio::Method::kMax);

    BFDd->SetOutFolder("BFDd");
    BFDd->SetTitle("BFD variation down");
    BFDd->SetAxisTitle("p_{T, ch. jet}","dN/dp_{T, ch. jet}");
    BFDd->SetRatioRangesY(0.5,1.5);
    BFDd->SetHistoName("unfoldedSpectrum");
    BFDd->Run();

//reflection
    SysRatio *REFL = new SysRatio(2,"Reflection variation");
    REFL->SetPath(path);
    REFL->SetOutPath(path+"systematics");

    TString REFLlegendlist[2] ={"Reflection -50%",
                                "Reflection +50%"};

    REFL->SetDefaultFile(defaulfileSignal);
    TString REFLfilelist[2] = {"ReflSys05_AnalysisResults_Run2w18b.root/signalExtraction/JetPtSpectra_SB_eff.root",
                               "ReflSys15_AnalysisResults_Run2w18b.root/signalExtraction/JetPtSpectra_SB_eff.root"};
    REFL->SetVarFilelist(REFLfilelist);
    REFL->SetVarLegendDesc(REFLlegendlist);
    REFL->SetMethod(SysRatio::Method::kMax);
    REFL->SetAxisTitle("p_{T, ch. jet}","dN/dp_{T, ch. jet}");
    REFL->SetRatioRangesY(0.95,1.05);
    REFL->SetOutFolder("REFL");
    REFL->SetHistoName("hjetptspectrumReb");
    REFL->Run();


//SB range variation
    SysRatio *SBRANGE = new SysRatio(6,"Signal and Background range variation");
    SBRANGE->SetPath(path);
    SBRANGE->SetOutPath(path+"systematics");

    TString SBRANGEfilelist[6] = {"S2SB38_AnalysisResults_Run2w18b.root/signalExtraction/JetPtSpectra_SB_eff.root",
                                  "S2SB410_AnalysisResults_Run2w18b.root/signalExtraction/JetPtSpectra_SB_eff.root",
                                  "S2SB412_AnalysisResults_Run2w18b.root/signalExtraction/JetPtSpectra_SB_eff.root",
                                  "S2SB415_AnalysisResults_Run2w18b.root/signalExtraction/JetPtSpectra_SB_eff.root",
                                  "S3SB49_AnalysisResults_Run2w18b.root/signalExtraction/JetPtSpectra_SB_eff.root",
                                  "S3SB412_AnalysisResults_Run2w18b.root/signalExtraction/JetPtSpectra_SB_eff.root"};

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
    SBRANGE->SetAxisTitle("p_{T, ch. jet}","dN/dp_{T, ch. jet}");
    SBRANGE->SetRatioRangesY(0.82,1.1);
    SBRANGE->SetOutFolder("SBRANGE");
    SBRANGE->SetHistoName("hjetptspectrumReb");
    SBRANGE->Run();


//JES
    SysRatio *JES = new SysRatio(1,"JES");
    JES->SetPath(path);
    JES->SetOutPath(path+"systematics");

    TString JESfilelist[1] = {"Default_AnalysisResults_Run2w18b.root96/unfolding_Bayes_5/unfoldedSpectrum_unfoldedJetSpectrum.root"};
    TString JESlegendlist[1] ={"inefficiency 4%"};

    JES->SetDefaultFile(defaulfileUnf);
    JES->SetVarFilelist(JESfilelist);
    JES->SetVarLegendDesc(JESlegendlist);
    JES->SetAxisTitle("p_{T, ch. jet}","dN/dp_{T, ch. jet}");
    JES->SetRatioRangesY(0.9,1.2);
    JES->SetOutFolder("JES");
    JES->SetHistoName("unfoldedSpectrum");
    JES->Run();



//CUTS
    SysRatio *CUTS = new SysRatio(4,"CUTS");
    CUTS->SetPath(path);
    CUTS->SetOutPath(path+"systematics");

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

    CUTS->SetDefaultFile("Cuts_Def/Default_AnalysisResults_Run2w18b.root/unfolding_Bayes_5/unfoldedSpectrum_unfoldedJetSpectrum.root");
    CUTS->SetVarFilelist(CUTSfilelist);
    CUTS->SetVarLegendDesc(CUTSlegendlist);
    CUTS->SetAxisTitle("p_{T, ch. jet}","dN/dp_{T, ch. jet}");
    CUTS->SetRatioRangesY(0,2);
    CUTS->SetMethod(SysRatio::Method::kError);

    CUTS->SetOutFolder("CUTS");
    CUTS->SetHistoName("unfoldedSpectrum");
    CUTS->Run();

//no L2

    SysRatio *CUTS2 = new SysRatio(3,"CUTS");
    CUTS2->SetPath(path);
    CUTS2->SetOutPath(path+"systematics");

    TString CUTS2filelist[3] = {"Cuts_L2/Default/unfolding_Bayes_5/unfoldedSpectrum_unfoldedJetSpectrum.root",
                               "Cuts_T2/Default/unfolding_Bayes_5/unfoldedSpectrum_unfoldedJetSpectrum.root",
                               "Cuts_T3/Default/unfolding_Bayes_5/unfoldedSpectrum_unfoldedJetSpectrum.root",
                              };
    TString CUTS2legendlist[3] ={"Looser 1",
                                "Tighter 1",
                                "Tighter 2",
                               };

    CUTS2->SetDefaultFile("Cuts_Def/Default_AnalysisResults_Run2w18b.root/unfolding_Bayes_5/unfoldedSpectrum_unfoldedJetSpectrum.root");
    CUTS2->SetVarFilelist(CUTS2filelist);
    CUTS2->SetVarLegendDesc(CUTS2legendlist);
    CUTS2->SetAxisTitle("p_{T, ch. jet}","dN/dp_{T, ch. jet}");
    CUTS2->SetRatioRangesY(0,2);
    CUTS2->SetMethod(SysRatio::Method::kError);

    CUTS2->SetOutFolder("CUTSnoL2");
    CUTS2->SetHistoName("unfoldedSpectrum");
    CUTS2->Run();

//CUT efficiency
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
        TString CUTSEFFdefaulfile = Form("Default_AnalysisResults_Run2w18b.root/efficiency/DjetEff_%s_jetpt5.00_50.00.root",promp[prompt]);
        CUTSEFF->SetDefaultFile(CUTSEFFdefaulfile);
        TString CUTSEFFfilelist[4];
        CUTSEFFfilelist[0]= Form("Cuts_L2/Default/efficiency/DjetEff_%s_jetpt5.00_50.00.root",promp[prompt]);
        CUTSEFFfilelist[1]= Form("Cuts_L3/Default/efficiency/DjetEff_%s_jetpt5.00_50.00.root",promp[prompt]);
        CUTSEFFfilelist[2]= Form("Cuts_T2/Default/efficiency/DjetEff_%s_jetpt5.00_50.00.root",promp[prompt]);
        CUTSEFFfilelist[3]= Form("Cuts_T3/Default/efficiency/DjetEff_%s_jetpt5.00_50.00.root",promp[prompt]);
        CUTSEFF->SetVarFilelist(CUTSEFFfilelist);
        CUTSEFF->SetVarLegendDesc(CUTSEFFlegendlist);
        CUTSEFF->SetMethod(SysRatio::Method::kError);
        CUTSEFF->SetAxisTitle("p_{D}","eff #times acc");
        CUTSEFF->SetRatioRangesY(0.5,1.3);
        CUTSEFF->SetOutFolder(Form("CUTSEFF%s",promp[prompt]));
        CUTSEFF->SetHistoName("hEff_reb");
        CUTSEFF->Run();
    }

/*
//Unfolding Iterations
    SysRatio *UNFITER = new SysRatio(11,"Unfolding Bayes iterations and ranges");
    UNFITER->SetPath(path);
    UNFITER->SetOutPath(path+"systematics");

    TString UNFITERfilelist[11] = {"Default_AnalysisResults_Run2w18b.root/unfolding_Bayes_4/finalSpectra/JetPtSpectrum_final.root",
                                  "Default_AnalysisResults_Run2w18b.root/unfolding_Bayes_6/finalSpectra/JetPtSpectrum_final.root",
                                  "T350M250_AnalysisResults_Run2w18b.root/unfolding_Bayes_5/finalSpectra/JetPtSpectrum_final.root",
                                  "T350M350_AnalysisResults_Run2w18b.root/unfolding_Bayes_5/finalSpectra/JetPtSpectrum_final.root",
                                  "T450M250_AnalysisResults_Run2w18b.root/unfolding_Bayes_5/finalSpectra/JetPtSpectrum_final.root",
                                  "T450M350_AnalysisResults_Run2w18b.root/unfolding_Bayes_5/finalSpectra/JetPtSpectrum_final.root",
                                  "T450M450_AnalysisResults_Run2w18b.root/unfolding_Bayes_5/finalSpectra/JetPtSpectrum_final.root",
                                  "T550M250_AnalysisResults_Run2w18b.root/unfolding_Bayes_5/finalSpectra/JetPtSpectrum_final.root",
                                  "T550M350_AnalysisResults_Run2w18b.root/unfolding_Bayes_5/finalSpectra/JetPtSpectrum_final.root",
                                  "T550M450_AnalysisResults_Run2w18b.root/unfolding_Bayes_5/finalSpectra/JetPtSpectrum_final.root",
                                  "T550M550_AnalysisResults_Run2w18b.root/unfolding_Bayes_5/finalSpectra/JetPtSpectrum_final.root"

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

    UNFITER->SetDefaultFile("Default_AnalysisResults_Run2w18b.root/unfolding_Bayes_5/finalSpectra/JetPtSpectrum_final.root");
    UNFITER->SetVarFilelist(UNFITERfilelist);
    UNFITER->SetVarLegendDesc(UNFITERlegendlist);
    UNFITER->SetOutFolder("UNFITERRANGE");
    UNFITER->SetHistoName("hData_binned");
    UNFITER->SetAxisTitle("p_{T, ch. jet}","dN/dp_{T, ch. jet}");
    UNFITER->SetRatioRangesY(0.8,1.1);
    UNFITER->Run();


//Prior Variation
    SysRatio *PRIOR = new SysRatio(9,"Prior variation");
    PRIOR->SetPath(path);
    PRIOR->SetOutPath(path+"systematics");

    TString PRIORfilelist[9] = {"Default_AnalysisResults_Run2w18b.root/unfolding_Bayes_5_priorType0/unfoldedSpectrum_unfoldedJetSpectrum.root",
                                "Default_AnalysisResults_Run2w18b.root/unfolding_Bayes_5_priorType1/unfoldedSpectrum_unfoldedJetSpectrum.root",
                                "Default_AnalysisResults_Run2w18b.root/unfolding_Bayes_5_priorType2/unfoldedSpectrum_unfoldedJetSpectrum.root",
                                "Default_AnalysisResults_Run2w18b.root/unfolding_Bayes_5_priorType3/unfoldedSpectrum_unfoldedJetSpectrum.root",
                                "Default_AnalysisResults_Run2w18b.root/unfolding_Bayes_5_priorType4/unfoldedSpectrum_unfoldedJetSpectrum.root",
                                "Default_AnalysisResults_Run2w18b.root/unfolding_Bayes_5_priorType5/unfoldedSpectrum_unfoldedJetSpectrum.root",
                                "Default_AnalysisResults_Run2w18b.root/unfolding_Bayes_5_priorType6/unfoldedSpectrum_unfoldedJetSpectrum.root",
                                "Default_AnalysisResults_Run2w18b.root/unfolding_Bayes_5_priorType7/unfoldedSpectrum_unfoldedJetSpectrum.root",
                                "Default_AnalysisResults_Run2w18b.root/unfolding_Bayes_5_priorType8/unfoldedSpectrum_unfoldedJetSpectrum.root"
                               };
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
    PRIOR->SetAxisTitle("p_{T, ch. jet}","dN/dp_{T, ch. jet}");
    PRIOR->SetRatioRangesY(0.8,1.1);
    PRIOR->Run();

//SVD Variation
    SysRatio *UNFSVD = new SysRatio(12,"SVD variation");
    UNFSVD->SetPath(path);
    UNFSVD->SetOutPath(path+"systematics");


    TString UNFSVDfilelist[12] = {"Default_AnalysisResults_Run2w18b.root/unfolding_SVD_5/finalSpectra/JetPtSpectrum_final.root",
                                  "Default_AnalysisResults_Run2w18b.root/unfolding_SVD_6/finalSpectra/JetPtSpectrum_final.root",
                                  "Default_AnalysisResults_Run2w18b.root/unfolding_SVD_7/finalSpectra/JetPtSpectrum_final.root",
                                  "T350M250_AnalysisResults_Run2w18b.root/unfolding_SVD_6/finalSpectra/JetPtSpectrum_final.root",
                                  "T350M350_AnalysisResults_Run2w18b.root/unfolding_SVD_6/finalSpectra/JetPtSpectrum_final.root",
                                  "T450M250_AnalysisResults_Run2w18b.root/unfolding_SVD_6/finalSpectra/JetPtSpectrum_final.root",
                                  "T450M350_AnalysisResults_Run2w18b.root/unfolding_SVD_6/finalSpectra/JetPtSpectrum_final.root",
                                  "T450M450_AnalysisResults_Run2w18b.root/unfolding_SVD_6/finalSpectra/JetPtSpectrum_final.root",
                                  "T550M250_AnalysisResults_Run2w18b.root/unfolding_SVD_6/finalSpectra/JetPtSpectrum_final.root",
                                  "T550M350_AnalysisResults_Run2w18b.root/unfolding_SVD_6/finalSpectra/JetPtSpectrum_final.root",
                                  "T550M450_AnalysisResults_Run2w18b.root/unfolding_SVD_6/finalSpectra/JetPtSpectrum_final.root",
                                  "T550M550_AnalysisResults_Run2w18b.root/unfolding_SVD_6/finalSpectra/JetPtSpectrum_final.root"

                                 };
    TString UNFSVDlegendlist[12] ={"meas: 2-50, true: 2-50, SVD reg=5",
                                   "meas: 2-50, true: 2-50, SVD reg=6",
                                   "meas: 2-50, true: 2-50, SVD reg=7",
                                    "meas: 2-50, true: 3-50, SVD reg=6",
                                    "meas: 3-50, true: 3-50, SVD reg=6",
                                    "meas: 2-50, true: 4-50, SVD reg=6",
                                    "meas: 3-50, true: 4-50, SVD reg=6",
                                    "meas: 4-50, true: 4-50, SVD reg=6",
                                    "meas: 2-50, true: 5-50, SVD reg=6",
                                    "meas: 3-50, true: 5-50, SVD reg=6",
                                    "meas: 4-50, true: 5-50, SVD reg=6",
                                    "meas: 5-50, true: 5-50, SVD reg=6",
                                   };

    UNFSVD->SetDefaultFile("Default_AnalysisResults_Run2w18b.root/unfolding_Bayes_5/finalSpectra/JetPtSpectrum_final.root");
    UNFSVD->SetVarFilelist(UNFSVDfilelist);
    UNFSVD->SetVarLegendDesc(UNFSVDlegendlist);
    UNFSVD->SetOutFolder("SVD");
    UNFSVD->SetHistoName("hData_binned");
    UNFSVD->SetAxisTitle("p_{T, ch. jet}","dN/dp_{T, ch. jet}");
    UNFSVD->SetRatioRangesY(0.4,1.6);
    UNFSVD->Run();

*/
/*
    SysRatio *T3SB = new SysRatio(6,"Signal and Background range variation");
    T3SB->SetPath("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_06cutSB/Cuts_L3/");
    T3SB->SetOutPath("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_06cutSB/Cuts_L3/systematics");

    TString T3SBfilelist[6] = {"S2SB38/signalExtraction/JetPtSpectra_SB_eff.root",
                                  "S2SB410/signalExtraction/JetPtSpectra_SB_eff.root",
                                  "S2SB412/signalExtraction/JetPtSpectra_SB_eff.root",
                                  "S2SB415/signalExtraction/JetPtSpectra_SB_eff.root",
                                  "S3SB49/signalExtraction/JetPtSpectra_SB_eff.root",
                                  "S3SB412/signalExtraction/JetPtSpectra_SB_eff.root"};

    TString T3SBlegendlist[6] ={"S:2#sigma, B:3-8#sigma",
                                   "S:2#sigma, B:4-10#sigma",
                                   "S:2#sigma, B:4-12#sigma",
                                   "S:2#sigma, B:4-15#sigma",
                                   "S:3#sigma, B:4-9#sigma",
                                   "S:3#sigma, B:4-12#sigma"};

    T3SB->SetDefaultLegend("S:2#sigma, B:4-9#sigma (Def)");
    T3SB->SetDefaultFile("Default/signalExtraction/JetPtSpectra_SB_eff.root");
    T3SB->SetVarFilelist(T3SBfilelist);
    T3SB->SetVarLegendDesc(T3SBlegendlist);
    T3SB->SetAxisTitle("p_{T, ch. jet}","dN/dp_{T, ch. jet}");
    T3SB->SetRatioRangesY(0.75,1.1);
    T3SB->SetOutFolder("T3SB");
    T3SB->SetHistoName("hjetptspectrumReb");
    T3SB->Run();
*/

/*
    SysRatio *UNFSVD = new SysRatio(12,"SVD variation");
    UNFSVD->SetPath("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts/");
    UNFSVD->SetOutPath("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts/systematics");


    TString UNFSVDfilelist[12] = {"Default_AnalysisResults_Run2w18b.root/unfolding_SVD_5/finalSpectra/JetPtSpectrum_final.root",
                                  "Default_AnalysisResults_Run2w18b.root/unfolding_SVD_6/finalSpectra/JetPtSpectrum_final.root",
                                  "Default_AnalysisResults_Run2w18b.root/unfolding_SVD_7/finalSpectra/JetPtSpectrum_final.root",
                                  "T350M250_AnalysisResults_Run2w18b.root/unfolding_SVD_6/finalSpectra/JetPtSpectrum_final.root",
                                  "T350M350_AnalysisResults_Run2w18b.root/unfolding_SVD_6/finalSpectra/JetPtSpectrum_final.root",
                                  "T450M250_AnalysisResults_Run2w18b.root/unfolding_SVD_6/finalSpectra/JetPtSpectrum_final.root",
                                  "T450M350_AnalysisResults_Run2w18b.root/unfolding_SVD_6/finalSpectra/JetPtSpectrum_final.root",
                                  "T450M450_AnalysisResults_Run2w18b.root/unfolding_SVD_6/finalSpectra/JetPtSpectrum_final.root",
                                  "T550M250_AnalysisResults_Run2w18b.root/unfolding_SVD_6/finalSpectra/JetPtSpectrum_final.root",
                                  "T550M350_AnalysisResults_Run2w18b.root/unfolding_SVD_6/finalSpectra/JetPtSpectrum_final.root",
                                  "T550M450_AnalysisResults_Run2w18b.root/unfolding_SVD_6/finalSpectra/JetPtSpectrum_final.root",
                                  "T550M550_AnalysisResults_Run2w18b.root/unfolding_SVD_6/finalSpectra/JetPtSpectrum_final.root"

                                 };
    TString UNFSVDlegendlist[12] ={"meas: 2-50, true: 2-50, SVD reg=5",
                                   "meas: 2-50, true: 2-50, SVD reg=6",
                                   "meas: 2-50, true: 2-50, SVD reg=7",
                                    "meas: 2-50, true: 3-50, SVD reg=6",
                                    "meas: 3-50, true: 3-50, SVD reg=6",
                                    "meas: 2-50, true: 4-50, SVD reg=6",
                                    "meas: 3-50, true: 4-50, SVD reg=6",
                                    "meas: 4-50, true: 4-50, SVD reg=6",
                                    "meas: 2-50, true: 5-50, SVD reg=6",
                                    "meas: 3-50, true: 5-50, SVD reg=6",
                                    "meas: 4-50, true: 5-50, SVD reg=6",
                                    "meas: 5-50, true: 5-50, SVD reg=6",
                                   };

    UNFSVD->SetDefaultFile("Default_AnalysisResults_Run2w18b.root/unfolding_Bayes_5/finalSpectra/JetPtSpectrum_final.root");
    UNFSVD->SetVarFilelist(UNFSVDfilelist);
    UNFSVD->SetVarLegendDesc(UNFSVDlegendlist);
    UNFSVD->SetOutFolder("SVD");
    UNFSVD->SetHistoName("hData_binned");
    UNFSVD->SetAxisTitle("p_{T, ch. jet}","dN/dp_{T, ch. jet}");
    UNFSVD->SetRatioRangesY(0.4,1.6);
    UNFSVD->Run();
*/

/*
    SysRatio *CUTS2 = new SysRatio(4,"CUTSbFD");
    CUTS2->SetPath(path);
    CUTS2->SetOutPath(path+"systematics");

    TString CUTSfilelist2[4] = {"Cuts_L2/Default/FDsubtraction/JetPtSpectrum_FDsub.root",
                               "Cuts_L3/Default/FDsubtraction/JetPtSpectrum_FDsub.root",
                               "Cuts_T2/Default/FDsubtraction/JetPtSpectrum_FDsub.root",
                               "Cuts_T3/Default/FDsubtraction/JetPtSpectrum_FDsub.root",
                              };
    TString CUTSlegendlist2[4] ={"Looser 1",
                                "Looser 2",
                                "Tighter 1",
                                "Tighter 2",
                               };

    CUTS2->SetDefaultFile("Default_AnalysisResults_Run2w18b.root/FDsubtraction/JetPtSpectrum_FDsub.root");
    CUTS2->SetVarFilelist(CUTSfilelist2);
    CUTS2->SetVarLegendDesc(CUTSlegendlist2);
    CUTS2->SetAxisTitle("p_{T, ch. jet}","dN/dp_{T, ch. jet}");
    CUTS2->SetRatioRangesY(0,2);
    CUTS2->SetMethod(SysRatio::Method::kError);

    CUTS2->SetOutFolder("CUTSbFD");
    CUTS2->SetHistoName("hData_binned");
    CUTS2->Run();
*/

    /*SysRatio *UNFSVD = new SysRatio(1,"SVD variation");
    UNFSVD->SetPath("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts/");
    UNFSVD->SetOutPath("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts/systematics");


    TString UNFSVDfilelist[12] = {"Default_AnalysisResults_Run2w18b.root/unfolding_SVD_6/finalSpectra/JetPtSpectrum_final.root"

                                 };
    TString UNFSVDlegendlist[12] ={
                                   "meas: 2-50, true: 2-50, SVD reg=6"
                                   };

    UNFSVD->SetDefaultFile("Default_AnalysisResults_Run2w18b.root/unfolding_Bayes_5/finalSpectra/JetPtSpectrum_final.root");
    UNFSVD->SetVarFilelist(UNFSVDfilelist);
    UNFSVD->SetVarLegendDesc(UNFSVDlegendlist);
    UNFSVD->SetOutFolder("SVD");
    UNFSVD->SetHistoName("hData_binned");
    UNFSVD->SetAxisTitle("p_{T, ch. jet}","dN/dp_{T, ch. jet}");
    UNFSVD->SetRatioRangesY(0.4,1.6);
    UNFSVD->Run();
*/
}
