//-----------------------------------------------------------------------
//  Author B.Trzeciak
//  Utrecht University
//  barbara.antonina.trzeciak@cern.ch
//-----------------------------------------------------------------------

#include <iostream>
#include "TString.h"
#include "TStyle.h"
#include "TMath.h"
#include "TFile.h"
#include "TDirectoryFile.h"
#include "TList.h"
#include "TCanvas.h"
#include "TPaveText.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TSystem.h"
#include "TRandom3.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TF1.h"
#include "THStack.h"
#include "TMatrixD.h"
#include "THnSparse.h"
#include "TDatabasePDG.h"
#include "TNtuple.h"

#include <AliCFContainer.h>
#include <AliCFEffGrid.h>
#include <AliCFTaskVertexingHF.h>



void EstimateMCunc(){
    TString pathin ="/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts/FONLL/";

    TString period[32] = {"AnalysisResults_FONLL_LHC17e_hadronPID",
                        "AnalysisResults_FONLL_LHC17f_hadronPID",
                        "AnalysisResults_FONLL_LHC17h_hadronPID",
                        "AnalysisResults_FONLL_LHC17i_hadronPID",
                        "AnalysisResults_FONLL_LHC17j_hadronPID",
                        "AnalysisResults_FONLL_LHC17k_hadronPID",
                        "AnalysisResults_FONLL_LHC17l_hadronPID",
                        "AnalysisResults_FONLL_LHC17m_hadronPID",
                        "AnalysisResults_FONLL_LHC17o_hadronPID",
                        "AnalysisResults_FONLL_LHC17r_hadronPID",
                        "AnalysisResults_FONLL_LHC16k_hadronPID",
                        "AnalysisResults_FONLL_LHC16l_hadronPID",
                        "AnalysisResults_FONLL_LHC16d_pass1",
                        "AnalysisResults_FONLL_LHC16e_pass1",
                        "AnalysisResults_FONLL_LHC16g_pass1",
                        "AnalysisResults_FONLL_LHC16h_pass1_positiveMagField",
                        "AnalysisResults_FONLL_LHC16h_pass1_negativeMagField",
                        "AnalysisResults_FONLL_LHC16j_pass1",
                        "AnalysisResults_FONLL_LHC16o_pass1_hadronPID",
                        "AnalysisResults_FONLL_LHC16p_pass1",
                        "AnalysisResults_FONLL_LHC18d_hadronPID",
                        "AnalysisResults_FONLL_LHC18e_hadronPID",
                        "AnalysisResults_FONLL_LHC18f_hadronPID",
                        "AnalysisResults_FONLL_LHC18g_hadronPID",
                        "AnalysisResults_FONLL_LHC18h_hadronPID",
                        "AnalysisResults_FONLL_LHC18i_hadronPID",
                        "AnalysisResults_FONLL_LHC18k_hadronPID",
                        "AnalysisResults_FONLL_LHC18l_hadronPID",
                        "AnalysisResults_FONLL_LHC18m_hadronPID",
                        "AnalysisResults_FONLL_LHC18n_hadronPID",
                        "AnalysisResults_FONLL_LHC18o_hadronPID",
                        "AnalysisResults_FONLL_LHC18p_hadronPID" };
    TString mode[2] ={"central","FONLL"};

    //for (Int_t m = 0; m<2;m++){
    //    for (Int_t f = 0; f<32;f++){
     //       DEfficiencyRun(pathin+period[f]+".root",pathout+period[f]+"_"+mode[0]+".root", mode[0]);
//        }

    //}
    TFile *file[32][2][2];
    TH1D *hist[32][2][2];
    TH1D *hratio[32][2];
    TH1D *unc[2];

    for (Int_t m = 0; m<2;m++){
        for (Int_t f = 0; f<32;f++){
            file[f][m][0] = new TFile(pathin+"y05/"+period[f] +"_"+ mode[m]+".root");
            file[f][m][1] = new TFile(pathin+"y09/"+period[f] +"_"+ mode[m]+".root");
            hist[f][m][0] = dynamic_cast<TH1D*>(file[f][m][0]->Get("h_reb"));
            hist[f][m][1] = dynamic_cast<TH1D*>(file[f][m][1]->Get("h_reb"));
            if(!hist[f][m][0])std::cout<<"hist not found"<<std::endl;
            if(!hist[f][m][1])std::cout<<"hist not found"<<std::endl;

        }
    }
    TCanvas *c = new TCanvas("c","c",1600,1600);
    c->Divide(2,1);

    unc[0] = dynamic_cast<TH1D*>(hist[0][0][0]->Clone("unc05"));
    unc[1] = dynamic_cast<TH1D*>(hist[0][0][0]->Clone("unc09"));
    unc[0]->SetTitle("MC pT shape uncertainty |y|<0.5");
    unc[1]->SetTitle("MC pT shape uncertainty |y|<0.9");
    unc[0]->GetYaxis()->SetTitle("unc. [%]");
    unc[1]->GetYaxis()->SetTitle("unc. [%]");

    for (Int_t f = 0; f<32;f++){
        hratio[f][0]= dynamic_cast<TH1D*>(hist[f][0][0]->Clone(Form("h_ratio05_%d",f)));
        hratio[f][1]= dynamic_cast<TH1D*>(hist[f][0][1]->Clone(Form("h_ratio09_%d",f)));
        /*hratio[f][0]->Divide(hist[f][1][0]);
        hratio[f][1]->Divide(hist[f][1][1]);*/
        hratio[f][0]->Divide(hist[f][0][0],hist[f][1][0],1,1,"B");
        hratio[f][1]->Divide(hist[f][0][1],hist[f][1][1],1,1,"B");

    }

    for(Int_t bin = 1; bin <=12;bin++){
        Double_t u05 = 0;
        Double_t u09 = 0;
        for (Int_t f = 0; f<32;f++){
            u05 += TMath::Abs(1-hratio[f][0]->GetBinContent(bin));
            u09 += TMath::Abs(1-hratio[f][1]->GetBinContent(bin));
        }
        unc[0]->SetBinContent(bin,u05/32*100);
        unc[1]->SetBinContent(bin,u09/32*100);
        unc[0]->SetBinError(bin,0);
        unc[1]->SetBinError(bin,0);
    }
    TString tmp="";
    hratio[0][0]->SetTitle(tmp+hratio[0][0]->GetTitle()+" y<05");
    hratio[0][1]->SetTitle(tmp+hratio[0][1]->GetTitle()+" y<09");

    for (Int_t f = 0; f<32;f++){
        c->cd(1);
        f==0?hratio[f][0]->Draw():hratio[f][0]->Draw("same");
       // c->cd(2);
        //f==0?hratio[f][1]->Draw():hratio[f][1]->Draw("same");
    }
    c->cd(2);
    unc[0]->Draw();
   // c->cd(4);
   // unc[1]->Draw();





}



