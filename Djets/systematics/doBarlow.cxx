#include "SysRatio.h"

void doBarlow(){
    TString zbin[5] = {": 2 < p_{T,jet} < 5 GeV/#it{c} (p_{T,D} > 2 GeV/#it{c})",
                       ": 5 < p_{T,jet} < 7 GeV/#it{c} (p_{T,D} > 2 GeV/#it{c})",
                       ": 7 < p_{T,jet} < 10 GeV/#it{c} (p_{T,D} > 2 GeV/#it{c})",
                       ": 10 < p_{T,jet} < 15 GeV/#it{c} (p_{T,D} > 2 GeV/#it{c})",
                       ": 15 < p_{T,jet} < 50 GeV/#it{c} (p_{T,D} > 3 GeV/#it{c})"};

 //   TString defaulfile = "Default_AnalysisResults_Run2.root/unfolding_2D_5/finalSpectra3/JetPtSpectrum_final3.root";
    TString path = "/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_finalX06/";
/*
 TString path = "/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_R02_PF/";
    TString CUTSdefaulfile = "Cuts_VarDef/Default/unfolding_2D_5/finalSpectra5/JetPtSpectrum_final5.root";

    TString CUTSfilelist[4] = {"Cuts_L2/Default/unfolding_2D_5/finalSpectra5/JetPtSpectrum_final5.root",
                               "Cuts_L3/Default/unfolding_2D_5/finalSpectra5/JetPtSpectrum_final5.root",
                               "Cuts_T2/Default/unfolding_2D_5/finalSpectra5/JetPtSpectrum_final5.root",
                               "Cuts_T3/Default/unfolding_2D_5/finalSpectra5/JetPtSpectrum_final5.root"
                              };
    TString CUTSlegendlist[4] ={"Looser 1",
                                "Looser 2",
                                "Tighter 1",
                                "Tighter 2"
                               };
                               /mnt/hgfs/vmware/nenimisto/BaseCuts_R02/Cuts_VarDef/Default/unfolding_Bayes_5/finalSpectra
*/

    TString CUTSdefaulfile = "Default_AnalysisResults_Run2w18b.root/unfolding_Bayes_5/finalSpectra/JetPtSpectrum_final.root";

    TString CUTSfilelist[4] = {"Cuts_L2/Default/unfolding_Bayes_5/finalSpectra/JetPtSpectrum_final.root",
                               "Cuts_L3/Default/unfolding_Bayes_5/finalSpectra/JetPtSpectrum_final.root",
                               "Cuts_T2/Default/unfolding_Bayes_5/finalSpectra/JetPtSpectrum_final.root",
                               "Cuts_T3/Default/unfolding_Bayes_5/finalSpectra/JetPtSpectrum_final.root"
                              };
    TString CUTSlegendlist[4] ={"Looser 1",
                                "Looser 2",
                                "Tighter 1",
                                "Tighter 2"
                               };


    TFile *fv[4];
    TFile *fd = new TFile(path+CUTSdefaulfile);
    TH1D* hv[4];
    TH1D* hd = (TH1D*)fd->Get("hData_binned");


    for (Int_t i = 0;i<4;i++){
        fv[i] = new TFile(path+CUTSfilelist[i]);
        hv[i] = (TH1D*)fv[i]->Get("hData_binned");
    }

    Double_t significance[4][hd->GetNbinsX()];

    for (Int_t i = 0;i<4;i++){
        for(Int_t bin = 1;bin<=hd->GetNbinsX();bin++){
            std::cout<<"bin "<<hd->GetBinCenter(bin)<<" "<<hv[i]->GetBinCenter(bin)<<" "<<hd->GetBinContent(bin)<<" "<<hv[i]->GetBinContent(bin)<<" "<<hd->GetBinError(bin)<<" "<<hv[i]->GetBinError(bin)<<std::endl;
            significance[i][bin-1] = TMath::Abs(hd->GetBinContent(bin)-hv[i]->GetBinContent(bin))/TMath::Sqrt(TMath::Abs(hd->GetBinError(bin)*hd->GetBinError(bin)-hv[i]->GetBinError(bin)*hv[i]->GetBinError(bin)));
        }
    }

    for (Int_t i = 0;i<4;i++){
        std::cout<<CUTSlegendlist[i]<<std::endl;
        for(Int_t bin = 1;bin<=hd->GetNbinsX();bin++){

            std::cout<<significance[i][bin-1]<<" ";
        }
        std::cout<<std::endl;

    }




}
