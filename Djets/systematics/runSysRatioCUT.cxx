#include "SysRatio.h"

void runSysRatioCUT(){
    gROOT->LoadMacro("SysRatioCUT.cxx++g");
    TString zbin[5] = {": 2 < p_{T,jet} < 5 GeV/#it{c} (p_{T,D} > 2 GeV/#it{c})",
                       ": 5 < p_{T,jet} < 7 GeV/#it{c} (p_{T,D} > 2 GeV/#it{c})",
                       ": 7 < p_{T,jet} < 10 GeV/#it{c} (p_{T,D} > 2 GeV/#it{c})",
                       ": 10 < p_{T,jet} < 15 GeV/#it{c} (p_{T,D} > 2 GeV/#it{c})",
                       ": 15 < p_{T,jet} < 50 GeV/#it{c} (p_{T,D} > 3 GeV/#it{c})"};

    TString defaulfile = "Default_AnalysisResults_Run2.root/unfolding_2D_5/unfoldedSpectrum_unfoldedJetSpectrum.root";

    //CUTS
    SysRatio *CUTS = new SysRatio(4,"CUTS");
    CUTS->SetPath("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_PRELIM/");
    CUTS->SetOutPath("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_PRELIM/SystematicsCUT");
    TString CUTSdefaulfile = "Cuts_VarDefforLLandT3/Default/unfolding_2D_5/unfoldedSpectrum_unfoldedJetSpectrum.root";

    TString CUTSfilelist[4] = {"Cuts_LL1/Default/unfolding_2D_5/unfoldedSpectrum_unfoldedJetSpectrum.root",
                               "Cuts_LL2/Default/unfolding_2D_5/unfoldedSpectrum_unfoldedJetSpectrum.root",
                               "Cuts_LL3/Default/unfolding_2D_5/unfoldedSpectrum_unfoldedJetSpectrum.root",
                               "Cuts_T3/Default/unfolding_2D_5/unfoldedSpectrum_unfoldedJetSpectrum.root"
                              };
    TString CUTSlegendlist[4] ={"LL1",
                                "LL2",
                                "LL3",
                                "T3"
                               };

    Double_t stat[4][7] = {{1.25,1.14,1.10,1.06,1.06,1.05,1.03},{1.17,1.19,1.09,1.07,1.05,1.05,1.04},{1.38,1.22,1.11,1.07,1.07,1.07,1.06},{1.38,1.25,1.15,1.10,1.10,1.10,1.09}};
    Double_t statDown[4][7] = {{0.75,0.86,0.84,0.94,0.94,0.95,0.97},{0.83,0.81,0.91,0.93,0.95,0.95,0.96},{0.62,0.78,0.89,0.93,0.93,0.93,0.94},{0.62,0.75,0.85,0.9,0.9,0.9,0.91}};

    Double_t binning[8] = {0.2,0.4,0.5,0.6,0.7,0.8,0.9,1.0};
    TH1D *hstat[4];
    TH1D *hstatDown[4];
    for (Int_t i = 0; i< 4;i++){
        hstat[i] = new TH1D(Form("stat%d",i),Form("stat%d",i),7,binning);
        hstatDown[i] = new TH1D(Form("statD%d",i),Form("statD%d",i),7,binning);
        for (Int_t bin = 1;bin<8;bin++){
            hstat[i]->SetBinContent(bin,stat[i][bin-1]);
            hstatDown[i]->SetBinContent(bin,statDown[i][bin-1]);
        }
    }



    TCanvas *ctm = new TCanvas("ctm","ctm",1600,1600);
    ctm->Divide(2,2);
    ctm->cd(1);
    hstat[0]->Draw();
    ctm->cd(2);
    hstat[1]->Draw();
    ctm->cd(3);
    hstat[2]->Draw();
    ctm->cd(4);
    hstat[3]->Draw();
    ctm->SaveAs("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_PRELIM/SystematicsCUT/stat.png");

    CUTS->SetDefaultFile(CUTSdefaulfile);
    CUTS->SetVarFilelist(CUTSfilelist);
    CUTS->SetVarLegendDesc(CUTSlegendlist);
    CUTS->SetMethod(SysRatio::Method::kError);

    for(Int_t z = 1; z <= 5; z++){
        TFile *f = new TFile("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_PRELIM/Cuts_VarDefT2f/Default/unfolding_2D_5/unfoldedSpectrum_unfoldedJetSpectrum.root","read");
        TH1D *h = dynamic_cast<TH1D*>(f->Get(Form("unfoldedSpectrumKineEff%d",z)));
        TFile *fv = new TFile("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_PRELIM/Cuts_T2f/Default/unfolding_2D_5/unfoldedSpectrum_unfoldedJetSpectrum.root","read");
        TH1D *hv = dynamic_cast<TH1D*>(fv->Get(Form("unfoldedSpectrumKineEff%d",z)));
        TH1D *r = dynamic_cast<TH1D*>(hv->Clone("r"));
        r->Divide(h);
        CUTS->SetOutFolder(Form("CUTS%d",z));
        CUTS->SetTitle("CUTS"+zbin[z-1]);
        CUTS->SetHistoName(Form("unfoldedSpectrumKineEff%d",z));
        //CUTS->Run();
        CUTS->Load();
        if(z>1)CUTS->SetStatUp(hstat[z-2]);
        if(z>1)CUTS->SetStatDown(hstatDown[z-2]);
        CUTS->PushMain(hv,"T2");
        CUTS->PushRatio(r,"T2");
        //CUTS->SetNfiles(5);
        CUTS->GetRMS();
        CUTS->GetMean();
        CUTS->Draw();
        CUTS->Write();
        CUTS->ResetRun();

    }

    SysRatio *CUTSEFF = new SysRatio(4,"CUTSEFF");
    CUTSEFF->SetPath("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_PRELIM/");
    CUTSEFF->SetOutPath("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_PRELIM/SystematicsCUT");

    const char *pt[6] = {"2.00","5.00","7.00","10.00","15.00","50.00"};
    const char *promp[2] = {"nonPrompt","prompt"};

    for(Int_t prompt = 0; prompt <=1; prompt++){
        for(Int_t z = 1; z <= 5; z++){
            TFile *f = new TFile(Form("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_PRELIM/Cuts_VarDefT2f/Default/efficiency/DjetEff_%s_jetpt%s_%s.root",promp[prompt],pt[z-1],pt[z]),"read");
            TH1D *h = dynamic_cast<TH1D*>(f->Get("hEff_reb"));
            TFile *fv = new TFile(Form("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_PRELIM/Cuts_T2f/Default/efficiency/DjetEff_%s_jetpt%s_%s.root",promp[prompt],pt[z-1],pt[z]),"read");
            TH1D *hv = dynamic_cast<TH1D*>(fv->Get("hEff_reb"));
            TH1D *r = dynamic_cast<TH1D*>(hv->Clone("r"));
            r->Divide(h);
            TString CUTSEFFdefaulfile = Form("Cuts_VarDefforLLandT3/Default/efficiency/DjetEff_%s_jetpt%s_%s.root",promp[prompt],pt[z-1],pt[z]);
            CUTSEFF->SetDefaultFile(CUTSEFFdefaulfile);
            TString CUTSEFFfilelist[4];
            CUTSEFFfilelist[0]= Form("Cuts_LL1/Default/efficiency/DjetEff_%s_jetpt%s_%s.root",promp[prompt],pt[z-1],pt[z]);
            CUTSEFFfilelist[1]= Form("Cuts_LL2/Default/efficiency/DjetEff_%s_jetpt%s_%s.root",promp[prompt],pt[z-1],pt[z]);
            CUTSEFFfilelist[2]= Form("Cuts_LL3/Default/efficiency/DjetEff_%s_jetpt%s_%s.root",promp[prompt],pt[z-1],pt[z]);
            CUTSEFFfilelist[3]= Form("Cuts_T3/Default/efficiency/DjetEff_%s_jetpt%s_%s.root",promp[prompt],pt[z-1],pt[z]);
            CUTSEFF->SetVarFilelist(CUTSEFFfilelist);
            CUTSEFF->SetVarLegendDesc(CUTSlegendlist);
            CUTSEFF->SetMethod(SysRatio::Method::kError);
            CUTSEFF->SetOutFolder(Form("CUTSEFF%s%d",promp[prompt],z));
            CUTSEFF->SetTitle(Form("CUTSEFF%s",promp[prompt])+zbin[z-1]);
            CUTSEFF->SetHistoName("hEff_reb");
            CUTSEFF->Load();
            CUTSEFF->PushMain(hv,"T2");
            CUTSEFF->PushRatio(r,"T2");
            //CUTS->SetNfiles(5);
            CUTSEFF->GetRMS();
            CUTSEFF->GetMean();
            CUTSEFF->Draw();
            CUTSEFF->Write();
            CUTSEFF->ResetRun();
        }
    }


}

