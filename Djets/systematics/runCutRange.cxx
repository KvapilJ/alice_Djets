#include "SysRatio.h"

void runCutRange(){
    gROOT->LoadMacro("SysRatio.cxx++g");
    TString zbin[5] = {": 2 < p_{T,jet} < 5 GeV/#it{c} (p_{T,D} > 2 GeV/#it{c})",
                       ": 5 < p_{T,jet} < 7 GeV/#it{c} (p_{T,D} > 2 GeV/#it{c})",
                       ": 7 < p_{T,jet} < 10 GeV/#it{c} (p_{T,D} > 2 GeV/#it{c})",
                       ": 10 < p_{T,jet} < 15 GeV/#it{c} (p_{T,D} > 2 GeV/#it{c})",
                       ": 15 < p_{T,jet} < 50 GeV/#it{c} (p_{T,D} > 3 GeV/#it{c})"};

    TString defaulfile = "Cuts_VarDef/Default/unfolding_2D_5/unfoldedSpectrum_unfoldedJetSpectrum.root";
    TString path = "/home/kvapil/work/analysis/pp_run2/D0jet/CutVar_ranges_R02_fix/";


//SB range variation
    SysRatio *SBRANGE = new SysRatio(7,"Signal and Background range variation");
    SBRANGE->SetPath(path);
    SBRANGE->SetOutPath(path+"systematics");

    TString SBRANGElegendlist[7] ={"S:2#sigma, B:4-9#sigma",
                                    "S:2#sigma, B:3-8#sigma",
                                   "S:2#sigma, B:4-10#sigma",
                                   "S:2#sigma, B:4-12#sigma",
                                   "S:2#sigma, B:4-15#sigma",
                                   "S:3#sigma, B:4-9#sigma",
                                   "S:3#sigma, B:4-12#sigma"};

        TString cut = "T3";

        SBRANGE->SetDefaultFile(defaulfile);
        TString SBRANGEfilelist[7];
        SBRANGEfilelist[0]= "Cuts_"+cut+"/Default/unfolding_2D_5/unfoldedSpectrum_unfoldedJetSpectrum.root";
        SBRANGEfilelist[1]= "Cuts_"+cut+"/S2SB38/unfolding_2D_5/unfoldedSpectrum_unfoldedJetSpectrum.root";
        SBRANGEfilelist[2]= "Cuts_"+cut+"/S2SB410/unfolding_2D_5/unfoldedSpectrum_unfoldedJetSpectrum.root";
        SBRANGEfilelist[3]= "Cuts_"+cut+"/S2SB412/unfolding_2D_5/unfoldedSpectrum_unfoldedJetSpectrum.root";
        SBRANGEfilelist[4]= "Cuts_"+cut+"/S2SB415/unfolding_2D_5/unfoldedSpectrum_unfoldedJetSpectrum.root";
        SBRANGEfilelist[5]= "Cuts_"+cut+"/S3SB49/unfolding_2D_5/unfoldedSpectrum_unfoldedJetSpectrum.root";
        SBRANGEfilelist[6]= "Cuts_"+cut+"/S3SB412/unfolding_2D_5/unfoldedSpectrum_unfoldedJetSpectrum.root";
        SBRANGE->SetVarFilelist(SBRANGEfilelist);
        SBRANGE->SetVarLegendDesc(SBRANGElegendlist);

    for(Int_t z = 1; z <= 5; z++){

        SBRANGE->SetOutFolder("CUTRANGE"+cut+Form("%d",z));
        SBRANGE->SetTitle("CUT var range variation "+cut+" "+zbin[z-1]);
        SBRANGE->SetHistoName(Form("unfoldedSpectrumKineEff%d",z));
        SBRANGE->Run();
    }


}
