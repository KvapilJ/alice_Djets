#include "SysRatio.h"

void runSysRatioLc(){
    gROOT->LoadMacro("SysRatio.cxx++g");

    TString zbin[5] = {": 5 < p_{T,jet} < 7 GeV/#it{c} (p_{T,D} > 3 GeV/#it{c})",
                       ": 7 < p_{T,jet} < 15 GeV/#it{c} (p_{T,D} > 3 GeV/#it{c})",
                       ": 15 < p_{T,jet} < 35 GeV/#it{c} (p_{T,D} > 3 GeV/#it{c})"};

    TString defaulfile = "Default_AnalysisResults_Run2w18b.root/unfolding_2D_5/unfoldedSpectrum_unfoldedJetSpectrum.root";
    TString path = "/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts/";
/*
    //B Feed-Down systematics
        SysRatio *UNFClosure = new SysRatio(1,"Unfolding Bayes Closure");
        UNFClosure->SetPath(path);
        UNFClosure->SetOutPath(path+"systematics");

        TString UNFClosurelegendlist[1] ={"True projection"};
        TString UNFClosurefilelist[1] = {"Default_AnalysisResults_Run2w18b.root/unfolding_2D_5/unfoldedSpectrum_unfoldedJetSpectrum.root"
                                 };

        UNFClosure->SetDefaultFile("Default_AnalysisResults_Run2w18b.root/unfolding_2D_5/unfoldedSpectrum_unfoldedJetSpectrum.root");
        UNFClosure->SetVarFilelist(UNFClosurefilelist);
        UNFClosure->SetVarLegendDesc(UNFClosurelegendlist);
        UNFClosure->SetMethod(SysRatio::Method::kMax);

        for(Int_t z = 1; z <= 3; z++){
            UNFClosure->SetOutFolder(Form("UNFClosure%d",z));
            UNFClosure->SetTitle("BFD variation up"+zbin[z-1]);
            UNFClosure->SetDefHistoName(Form("unfoldedSpectrumClosure%d",z));
            UNFClosure->SetHistoName(Form("TrueSpectrumClosure%d",z));
            UNFClosure->SetAxisTitle("z_{#parallel}","Yield");
            UNFClosure->SetRatioRangesY(0.8,1.2);
            UNFClosure->Run();
        }
        return;
*/
//B Feed-Down systematics
    SysRatio *BFDu = new SysRatio(1,"B meson Feed-Down up");
    BFDu->SetPath(path);
    BFDu->SetOutPath(path+"systematics");

    TString BFDfilelistu[1] = {"Default_AnalysisResults_Run2w18b.root/unfolding_2D_5_FDsysUp/unfoldedSpectrum_unfoldedJetSpectrum.root"};
    TString BFDlegendlistu[1] ={"FD up"};

    BFDu->SetDefaultFile(defaulfile);
    BFDu->SetVarFilelist(BFDfilelistu);
    BFDu->SetVarLegendDesc(BFDlegendlistu);
    BFDu->SetMethod(SysRatio::Method::kMax);

    for(Int_t z = 1; z <= 3; z++){
        BFDu->SetOutFolder(Form("BFDup%d",z));
        BFDu->SetTitle("BFD variation up"+zbin[z-1]);
        BFDu->SetHistoName(Form("unfoldedSpectrumKineEff%d",z));
        BFDu->SetAxisTitle("z_{#parallel}","Yield");
        BFDu->Run();
    }

//B Feed-Down systematics
    SysRatio *BFDd = new SysRatio(1,"B meson Feed-Down down");
    BFDd->SetPath(path);
    BFDd->SetOutPath(path+"systematics");

    TString BFDfilelistd[1] = {"Default_AnalysisResults_Run2w18b.root/unfolding_2D_5_FDsysDown/unfoldedSpectrum_unfoldedJetSpectrum.root"};
    TString BFDlegendlistd[1] ={"FD down"};

    BFDd->SetDefaultFile(defaulfile);
    BFDd->SetVarFilelist(BFDfilelistd);
    BFDd->SetVarLegendDesc(BFDlegendlistd);
    BFDd->SetMethod(SysRatio::Method::kMax);

    for(Int_t z = 1; z <= 3; z++){
        BFDd->SetOutFolder(Form("BFDdown%d",z));
        BFDd->SetTitle("BFD variation down"+zbin[z-1]);
        BFDd->SetHistoName(Form("unfoldedSpectrumKineEff%d",z));
        BFDd->SetAxisTitle("z_{#parallel}","Yield");
        BFDd->Run();
    }

//reflections
    SysRatio *REFL = new SysRatio(2,"Reflection variation");
    REFL->SetPath(path);
    REFL->SetOutPath(path+"systematics");

    TString REFLlegendlist[2] ={"Reflection -50%",
                                   "Reflection +50%"};


    for(Int_t z = 1; z <= 3; z++){
        TString REFLdefaulfile = Form("Default_AnalysisResults_Run2w18b.root/signalExtraction/JetPtSpectra_SB_eff%d.root",z);
        REFL->SetDefaultFile(REFLdefaulfile);
        TString REFLfilelist[2];
        REFLfilelist[0]= Form("ReflSys05_AnalysisResults_Run2w18b.root/signalExtraction/JetPtSpectra_SB_eff%d.root",z);
        REFLfilelist[1]= Form("ReflSys15_AnalysisResults_Run2w18b.root/signalExtraction/JetPtSpectra_SB_eff%d.root",z);
        REFL->SetVarFilelist(REFLfilelist);
        REFL->SetVarLegendDesc(REFLlegendlist);
        REFL->SetMethod(SysRatio::Method::kMax);
        REFL->SetOutFolder(Form("REFL%d",z));
        REFL->SetTitle("Reflection variation"+zbin[z-1]);
        REFL->SetAxisTitle("z_{#parallel}","Yield");
        REFL->SetHistoName("hjetptspectrumReb");
        REFL->Run();
    }

//SB range variation
    SysRatio *SBRANGE = new SysRatio(6,"Signal and Background range variation");
    SBRANGE->SetPath(path);
    SBRANGE->SetOutPath(path+"systematics");

    TString SBRANGElegendlist[6] ={"S:2#sigma, B:3-8#sigma",
                                   "S:2#sigma, B:4-10#sigma",
                                   "S:2#sigma, B:4-12#sigma",
                                   "S:2#sigma, B:4-15#sigma",
                                   "S:3#sigma, B:4-9#sigma",
                                   "S:3#sigma, B:4-12#sigma"};


    for(Int_t z = 1; z <= 3; z++){
        TString SBRANGEdefaulfile = Form("Default_AnalysisResults_Run2w18b.root/signalExtraction/JetPtSpectra_SB_eff%d.root",z);
        SBRANGE->SetDefaultFile(SBRANGEdefaulfile);
        TString SBRANGEfilelist[6];
        SBRANGEfilelist[0]= Form("S2SB38_AnalysisResults_Run2w18b.root/signalExtraction/JetPtSpectra_SB_eff%d.root",z);
        SBRANGEfilelist[1]= Form("S2SB410_AnalysisResults_Run2w18b.root/signalExtraction/JetPtSpectra_SB_eff%d.root",z);
        SBRANGEfilelist[2]= Form("S2SB412_AnalysisResults_Run2w18b.root/signalExtraction/JetPtSpectra_SB_eff%d.root",z);
        SBRANGEfilelist[3]= Form("S2SB415_AnalysisResults_Run2w18b.root/signalExtraction/JetPtSpectra_SB_eff%d.root",z);
        SBRANGEfilelist[4]= Form("S3SB49_AnalysisResults_Run2w18b.root/signalExtraction/JetPtSpectra_SB_eff%d.root",z);
        SBRANGEfilelist[5]= Form("S3SB412_AnalysisResults_Run2w18b.root/signalExtraction/JetPtSpectra_SB_eff%d.root",z);

        SBRANGE->SetVarFilelist(SBRANGEfilelist);
        SBRANGE->SetVarLegendDesc(SBRANGElegendlist);
        SBRANGE->SetOutFolder(Form("SBRANGE%d",z));
        SBRANGE->SetTitle("Signal and Background range variation"+zbin[z-1]);
        SBRANGE->SetAxisTitle("z_{#parallel}","Yield");
        SBRANGE->SetHistoName("hjetptspectrumReb");
        SBRANGE->Run();
    }

/*
    //JES
    SysRatio *JES = new SysRatio(1,"JES");
    JES->SetPath(path);
    JES->SetOutPath(path+"systematics");

    TString JESfilelist[1] = {"Default_AnalysisResults_Run2w18b.root96/unfolding_2D_5FDJES/unfoldedSpectrum_unfoldedJetSpectrum.root"};
    TString JESlegendlist[1] ={"inefficiency 4%"};

    JES->SetDefaultFile(defaulfile);
    JES->SetVarFilelist(JESfilelist);
    JES->SetVarLegendDesc(JESlegendlist);

    for(Int_t z = 1; z <= 5; z++){
        JES->SetOutFolder(Form("JESprior%d",z));
        JES->SetTitle("JES"+zbin[z-1]);
        JES->SetRatioRangesY(0.8,1.2);
        JES->SetAxisTitle("z_{#parallel}","Yield");
        JES->SetHistoName(Form("Prior%d",z-1));
        JES->Run();
    }*/

    //JES2
    SysRatio *JES = new SysRatio(1,"JES");
    JES->SetPath(path);
    JES->SetOutPath(path+"systematics");

    TString JESfilelist[1] = {"Default_AnalysisResults_Run2w18b.root96/unfolding_2D_5/unfoldedSpectrum_unfoldedJetSpectrum.root"};
    TString JESlegendlist[1] ={"inefficiency 4%"};

    JES->SetDefaultFile(defaulfile);
    JES->SetVarFilelist(JESfilelist);
    JES->SetVarLegendDesc(JESlegendlist);

    for(Int_t z = 1; z <= 3; z++){
        JES->SetOutFolder(Form("JES%d",z));
        JES->SetTitle("JES"+zbin[z-1]);
        JES->SetAxisTitle("z_{#parallel}","Yield");
        JES->SetRatioRangesY(0.8,1.2);
        JES->SetHistoName(Form("unfoldedSpectrumKineEff%d",z));
        JES->Run();
    }

 //CUTS

    SysRatio *CUTS = new SysRatio(4,"CUTS");
    CUTS->SetPath(path);
    CUTS->SetOutPath(path+"systematics");

    TString CUTSfilelist[4] = {"Cuts_L2/Default/unfolding_2D_5/unfoldedSpectrum_unfoldedJetSpectrum.root",
                               "Cuts_L3/Default/unfolding_2D_5/unfoldedSpectrum_unfoldedJetSpectrum.root",
                               "Cuts_T2/Default/unfolding_2D_5/unfoldedSpectrum_unfoldedJetSpectrum.root",
                               "Cuts_T3/Default/unfolding_2D_5/unfoldedSpectrum_unfoldedJetSpectrum.root"
                              };
    TString CUTSlegendlist[4] ={"Looser 1",
                                "Looser 2",
                                "Tighter 1",
                                "Tighter 2"
                               };

    CUTS->SetDefaultFile("Cuts_Def/Default_AnalysisResults_Run2w18b.root/unfolding_2D_5/unfoldedSpectrum_unfoldedJetSpectrum.root");
    CUTS->SetVarFilelist(CUTSfilelist);
    CUTS->SetVarLegendDesc(CUTSlegendlist);
    CUTS->SetMethod(SysRatio::Method::kError);

    for(Int_t z = 1; z <= 3; z++){
        CUTS->SetOutFolder(Form("CUTS%d",z));
        CUTS->SetTitle("CUTS"+zbin[z-1]);
        CUTS->SetAxisTitle("z_{#parallel}","Yield");
        CUTS->SetHistoName(Form("unfoldedSpectrumKineEff%d",z));
        CUTS->Run();
    }
/*
    //Unfolding Iterations
    SysRatio *UNFITER = new SysRatio(2,"Unfolding iterations");
    UNFITER->SetPath(path);
    UNFITER->SetOutPath(path+"systematics");

    TString UNFITERfilelist[2] = {"Default_AnalysisResults_Run2w18b.root/unfolding_2D_4/unfoldedSpectrum_unfoldedJetSpectrum.root",
                                  "Default_AnalysisResults_Run2w18b.root/unfolding_2D_6/unfoldedSpectrum_unfoldedJetSpectrum.root"};
    TString UNFITERlegendlist[2] ={"Bayes iter = 4",
                                   "Bayes iter = 6"};

    UNFITER->SetDefaultFile(defaulfile);
    UNFITER->SetVarFilelist(UNFITERfilelist);
    UNFITER->SetVarLegendDesc(UNFITERlegendlist);

    for(Int_t z = 1; z <= 5; z++){
        UNFITER->SetOutFolder(Form("UNFITER%d",z));
        UNFITER->SetTitle("Unfolding iterations"+zbin[z-1]);
        UNFITER->SetAxisTitle("z_{#parallel}","Yield");
        UNFITER->SetHistoName(Form("unfoldedSpectrumKineEff%d",z));
        UNFITER->Run();
    }
*/
    /*
 //Prior Variation
    SysRatio *PRIOR = new SysRatio(8,"Prior variation");
    PRIOR->SetPath(path);
    PRIOR->SetOutPath(path+"systematics");

    TString PRIORfilelist[8] = {"Default_AnalysisResults_Run2w18b.root/unfolding_2D_5prior0/unfoldedSpectrum_unfoldedJetSpectrum.root",
                                "Default_AnalysisResults_Run2w18b.root/unfolding_2D_5prior1/unfoldedSpectrum_unfoldedJetSpectrum.root",
                                "Default_AnalysisResults_Run2w18b.root/unfolding_2D_5prior2/unfoldedSpectrum_unfoldedJetSpectrum.root",
                                "Default_AnalysisResults_Run2w18b.root/unfolding_2D_5prior3/unfoldedSpectrum_unfoldedJetSpectrum.root",
                                "Default_AnalysisResults_Run2w18b.root/unfolding_2D_5prior4/unfoldedSpectrum_unfoldedJetSpectrum.root",
                                "Default_AnalysisResults_Run2w18b.root/unfolding_2D_5prior5/unfoldedSpectrum_unfoldedJetSpectrum.root",
                                "Default_AnalysisResults_Run2w18b.root/unfolding_2D_5prior6/unfoldedSpectrum_unfoldedJetSpectrum.root",
                                "Default_AnalysisResults_Run2w18b.root/unfolding_2D_5prior7/unfoldedSpectrum_unfoldedJetSpectrum.root"};
    TString PRIORlegendlist[8] ={"positive linear soft z_{#parallel} and soft p_{T,jet} modification",
                                 "negative linear soft z_{#parallel} and soft p_{T,jet} modification",
                                 "positive linear hard z_{#parallel} and soft p_{T,jet} modification",
                                 "negative linear hard z_{#parallel} and soft p_{T,jet} modification",
                                 "positive linear harder z_{#parallel} and soft p_{T,jet} modification",
                                 "negative linear harder z_{#parallel} and soft p_{T,jet} modification",
                                 "positive linear soft z_{#parallel} and hard p_{T,jet} modification",
                                 "negative linear soft z_{#parallel} and hard p_{T,jet} modification"
                               };

    PRIOR->SetDefaultFile(defaulfile);
    PRIOR->SetVarFilelist(PRIORfilelist);
    PRIOR->SetVarLegendDesc(PRIORlegendlist);

    for(Int_t z = 1; z <= 5; z++){
        PRIOR->SetOutFolder(Form("PRIOR%d",z));
        PRIOR->SetTitle("Prior variation"+zbin[z-1]);
        PRIOR->SetAxisTitle("z_{#parallel}","Yield");
        PRIOR->SetHistoName(Form("unfoldedSpectrumKineEff%d",z));
        PRIOR->Run();
    }

//Prior Variation
   SysRatio *PRIORSHAPE = new SysRatio(8,"Prior variation");
   PRIORSHAPE->SetPath(path);
   PRIORSHAPE->SetOutPath(path+"systematics");

   PRIORSHAPE->SetDefaultFile(defaulfile);
   PRIORSHAPE->SetVarFilelist(PRIORfilelist);
   PRIORSHAPE->SetVarLegendDesc(PRIORlegendlist);

   for(Int_t z = 0; z < 5; z++){
       PRIORSHAPE->SetOutFolder(Form("PRIORSHAPE%d",z));
       PRIORSHAPE->SetTitle("Prior shape"+zbin[z-1]);
       PRIORSHAPE->SetAxisTitle("z_{#parallel}","Yield");
       PRIORSHAPE->SetHistoName(Form("Prior%d",z));
       PRIORSHAPE->Run();
   }
*/
/*
    SysRatio *CUTS = new SysRatio(3,"CUTS");
    CUTS->SetPath("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts/");
    CUTS->SetOutPath("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts/Systematics");
    TString CUTSdefaulfile = "Cuts_VarDef/Default/unfolding_2D_5/unfoldedSpectrum_unfoldedJetSpectrum.root";

    TString CUTSfilelist[3] = {"Cuts_L2/Default/unfolding_2D_5/unfoldedSpectrum_unfoldedJetSpectrum.root",
                               "Cuts_L3/Default/unfolding_2D_5/unfoldedSpectrum_unfoldedJetSpectrum.root",
                               "Cuts_T2/Default/unfolding_2D_5/unfoldedSpectrum_unfoldedJetSpectrum.root",
                              };
    TString CUTSlegendlist[3] ={"Looser 1",
                                "Looser 2",
                                "Tighter 1",
                               };

    CUTS->SetDefaultFile(CUTSdefaulfile);
    CUTS->SetVarFilelist(CUTSfilelist);
    CUTS->SetVarLegendDesc(CUTSlegendlist);
    CUTS->SetMethod(SysRatio::Method::kError);

    for(Int_t z = 1; z <= 5; z++){
        CUTS->SetOutFolder(Form("CUTS%d",z));
        CUTS->SetTitle("CUTS"+zbin[z-1]);
        CUTS->SetHistoName(Form("unfoldedSpectrumKineEff%d",z));
        CUTS->Run();
    }
*//*
    SysRatio *CUTSFD = new SysRatio(3,"CUTSFD");
    CUTSFD->SetPath("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts/");
    CUTSFD->SetOutPath("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts/Systematics");
    //TString CUTSFDdefaulfile = "Cuts_VarDef/Default/unfolding_2D_5/unfoldedSpectrum_unfoldedJetSpectrum.root";


    TString CUTSFDlegendlist[3] ={"Looser 1",
                                "Looser 2",
                                "Tighter 1",
                               };



    for(Int_t z = 1; z <= 5; z++){
        TString CUTSFDdefaulfile = Form("Cuts_VarDef/Default/FDsubtraction/JetPtSpectrum_FDsub%d.root",z);
        CUTSFD->SetDefaultFile(CUTSFDdefaulfile);
        TString CUTSFDfilelist[3];
        CUTSFDfilelist[0]= Form("Cuts_L2/Default/FDsubtraction/JetPtSpectrum_FDsub%d.root",z);
        CUTSFDfilelist[1]= Form("Cuts_L3/Default/FDsubtraction/JetPtSpectrum_FDsub%d.root",z);
        CUTSFDfilelist[2]= Form("Cuts_T2/Default/FDsubtraction/JetPtSpectrum_FDsub%d.root",z);
        CUTSFD->SetVarFilelist(CUTSFDfilelist);
        CUTSFD->SetVarLegendDesc(CUTSFDlegendlist);
        CUTSFD->SetMethod(SysRatio::Method::kError);
        CUTSFD->SetOutFolder(Form("CUTSFD%d",z));
        CUTSFD->SetTitle("CUTSFD"+zbin[z-1]);
        CUTSFD->SetHistoName("hData_binned");
        CUTSFD->Run();
    }

    */

    //Int_t prompt = 1;

    SysRatio *CUTSEFF = new SysRatio(4,"CUTSEFF");
    CUTSEFF->SetPath(path);
    CUTSEFF->SetOutPath(path+"systematics");

    TString CUTSEFFlegendlist[4] ={"Looser 1",
                                "Looser 2",
                                "Tighter 1",
                                    "Tighter 2"
                               };

    const char *pt[6] = {"5.00","7.00","15.00","35.00"};
    const char *promp[2] = {"nonPrompt","prompt"};

    for(Int_t prompt = 0; prompt <=1; prompt++){
        for(Int_t z = 1; z <= 3; z++){
            TString CUTSEFFdefaulfile = Form("Default_AnalysisResults_Run2w18b.root/efficiency/DjetEff_%s_jetpt%s_%s.root",promp[prompt],pt[z-1],pt[z]);
            CUTSEFF->SetDefaultFile(CUTSEFFdefaulfile);
            TString CUTSEFFfilelist[4];
            CUTSEFFfilelist[0]= Form("Cuts_L2/Default/efficiency/DjetEff_%s_jetpt%s_%s.root",promp[prompt],pt[z-1],pt[z]);
            CUTSEFFfilelist[1]= Form("Cuts_L3/Default/efficiency/DjetEff_%s_jetpt%s_%s.root",promp[prompt],pt[z-1],pt[z]);
            CUTSEFFfilelist[2]= Form("Cuts_T2/Default/efficiency/DjetEff_%s_jetpt%s_%s.root",promp[prompt],pt[z-1],pt[z]);
            CUTSEFFfilelist[3]= Form("Cuts_T3/Default/efficiency/DjetEff_%s_jetpt%s_%s.root",promp[prompt],pt[z-1],pt[z]);
            CUTSEFF->SetVarFilelist(CUTSEFFfilelist);
            CUTSEFF->SetVarLegendDesc(CUTSEFFlegendlist);
            CUTSEFF->SetMethod(SysRatio::Method::kError);
            CUTSEFF->SetOutFolder(Form("CUTSEFF%s%d",promp[prompt],z));
            CUTSEFF->SetTitle(Form("CUTSEFF%s",promp[prompt])+zbin[z-1]);
            BFDu->SetAxisTitle("p_{D}","eff #times acc");
            CUTSEFF->SetHistoName("hEff_reb");
            CUTSEFF->Run();
        }
    }

  /*
    SysRatio *CUTSEFF = new SysRatio(1,"CUTSEFF");
    CUTSEFF->SetPath(path);
    CUTSEFF->SetOutPath(path+"systematics");

    TString CUTSEFFlegendlist[1] ={"open"};

    const char *pt[6] = {"2.00","5.00","7.00","10.00","15.00","50.00"};
    const char *promp[2] = {"nonPrompt","prompt"};

    for(Int_t prompt = 0; prompt <=1; prompt++){
        for(Int_t z = 1; z <= 5; z++){
            TString CUTSEFFdefaulfile = Form("Default_AnalysisResults_Run2w18b.root/efficiency_def/DjetEff_%s_jetpt%s_%s.root",promp[prompt],pt[z-1],pt[z]);
            CUTSEFF->SetDefaultFile(CUTSEFFdefaulfile);
            TString CUTSEFFfilelist[1];
            CUTSEFFfilelist[0]= Form("Default_AnalysisResults_Run2w18b.root/efficiency_open/DjetEff_%s_jetpt%s_%s.root",promp[prompt],pt[z-1],pt[z]);
            CUTSEFF->SetVarFilelist(CUTSEFFfilelist);
            CUTSEFF->SetVarLegendDesc(CUTSEFFlegendlist);
            CUTSEFF->SetMethod(SysRatio::Method::kError);
            CUTSEFF->SetOutFolder(Form("CUTSEFF%s%d",promp[prompt],z));
            CUTSEFF->SetTitle(Form("CUTSEFF%s",promp[prompt])+zbin[z-1]);
            CUTSEFF->SetHistoName("hEff_reb");
            CUTSEFF->Run();
        }
    }
*/
}
