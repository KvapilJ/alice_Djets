#include "SysRatio.h"

void runSysRatiochecks(){
    gROOT->LoadMacro("SysRatio.cxx++g");
    const Int_t nR = 2;
    TString R[nR] = {": R = 0.2, 2 < p_{T,jet} < 50 GeV/#it{c} (p_{T,D} > 2 GeV/#it{c})",
                     ": R = 0.6, 2 < p_{T,jet} < 50 GeV/#it{c} (p_{T,D} > 2 GeV/#it{c})"};

    TString defaulfileUnf = "Default_AnalysisResults_Run2w18b.root/unfolding_Bayes_5/unfoldedSpectrum_unfoldedJetSpectrum.root";
    TString defaulfileSignal = "Default_AnalysisResults_Run2w18b.root/signalExtraction/JetPtSpectra_SB_eff.root";
    TString path = "/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_TPC/";

//CUTS
    SysRatio *CUTS = new SysRatio(1,"CUTS");
    CUTS->SetPath("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_TPC/");
    CUTS->SetOutPath("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_TPC/systematics");

    TString CUTSfilelist[4] = {"Cuts_L2/Default/unfolding_Bayes_5/unfoldedSpectrum_unfoldedJetSpectrum.root",
//TString CUTSfilelist[4] = {"Default_AnalysisResults_18.root/unfolding_BayesClosure_5/unfoldedSpectrum_unfoldedJetSpectrum.root",
                              };
    TString CUTSlegendlist[4] ={"TPC crossed rows > 70",
                               };

    CUTS->SetDefaultFile("Default_AnalysisResults_18.root/unfolding_Bayes_5/unfoldedSpectrum_unfoldedJetSpectrum.root");
   // CUTS->SetDefaultFile("Cuts_L2/Default/unfolding_Bayes_5/unfoldedSpectrum_unfoldedJetSpectrum.root");
    CUTS->SetVarFilelist(CUTSfilelist);
    CUTS->SetVarLegendDesc(CUTSlegendlist);
    CUTS->SetAxisTitle("p_{T, ch. jet}","dN/dp_{T, ch. jet}");
    CUTS->SetRatioRangesY(0,2);
    CUTS->SetMethod(SysRatio::Method::kError);

    CUTS->SetOutFolder("TPC");
    CUTS->SetHistoName("unfoldedSpectrum");
    CUTS->Run();
return;
//CUTS
    SysRatio *CUTSFD = new SysRatio(3,"CUTSFD");
    CUTSFD->SetPath("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_FDcheck/");
    CUTSFD->SetOutPath("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_FDcheck/systematics");

    TString CUTSFDfilelist[4] = {"Default_AnalysisResults_Run2w18b.root_1/unfolding_Bayes_5/unfoldedSpectrum_unfoldedJetSpectrum.root",
"Cuts_L3/Default_1/unfolding_Bayes_5/unfoldedSpectrum_unfoldedJetSpectrum.root",
"Cuts_L3/Default_2/unfolding_Bayes_5/unfoldedSpectrum_unfoldedJetSpectrum.root",
                              };
    TString CUTSFDlegendlist[4] ={"DEFAULT N=1",
"L3 N=1",
"L3 N=2",
                               };

    CUTSFD->SetDefaultFile("Default_AnalysisResults_Run2w18b.root_2/unfolding_Bayes_5/unfoldedSpectrum_unfoldedJetSpectrum.root");
    CUTSFD->SetVarFilelist(CUTSFDfilelist);
    CUTSFD->SetVarLegendDesc(CUTSFDlegendlist);
    CUTSFD->SetAxisTitle("p_{T, ch. jet}","dN/dp_{T, ch. jet}");
    CUTSFD->SetRatioRangesY(0,2);
    CUTSFD->SetMethod(SysRatio::Method::kError);

    CUTSFD->SetOutFolder("FD");
    CUTSFD->SetHistoName("unfoldedSpectrum");
    CUTSFD->Run();


//CUT efficiency
    SysRatio *CUTSEFF = new SysRatio(3,"CUTSEFF");
    CUTSEFF->SetPath("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_FDcheck/");
    CUTSEFF->SetOutPath("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_FDcheck/systematics");

    TString CUTSEFFlegendlist[4] ={"DEFAULT N=1",
"L3 N=1",
"L3 N=2",
                               };

    const char *promp[2] = {"nonPrompt","prompt"};

    for(Int_t prompt = 0; prompt <=1; prompt++){
        TString CUTSEFFdefaulfile = Form("Default_AnalysisResults_Run2w18b.root_2/efficiency/DjetEff_%s_jetpt5.00_50.00.root",promp[prompt]);
        CUTSEFF->SetDefaultFile(CUTSEFFdefaulfile);
        TString CUTSEFFfilelist[4];
        CUTSEFFfilelist[0]= Form("Default_AnalysisResults_Run2w18b.root_1/efficiency/DjetEff_%s_jetpt5.00_50.00.root",promp[prompt]);
        CUTSEFFfilelist[1]= Form("Cuts_L3/Default_1/efficiency/DjetEff_%s_jetpt5.00_50.00.root",promp[prompt]);
        CUTSEFFfilelist[2]= Form("Cuts_L3/Default_2/efficiency/DjetEff_%s_jetpt5.00_50.00.root",promp[prompt]);
        CUTSEFF->SetVarFilelist(CUTSEFFfilelist);
        CUTSEFF->SetVarLegendDesc(CUTSEFFlegendlist);
        CUTSEFF->SetMethod(SysRatio::Method::kError);
        CUTSEFF->SetAxisTitle("p_{D}","eff #times acc");
        CUTSEFF->SetRatioRangesY(0.9,1.7);
        CUTSEFF->SetOutFolder(Form("CUTSEFF%s",promp[prompt]));
        CUTSEFF->SetHistoName("hEff_reb");
        CUTSEFF->Run();
    }

}
