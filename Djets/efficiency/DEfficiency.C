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

using namespace std;

void setHistoDetails(TH1 *h, Color_t color, Style_t Mstyle, Size_t size = 0.9, Width_t width=2){

    h->SetMarkerStyle(Mstyle);
    h->SetMarkerColor(color);
    h->SetMarkerSize(size);
    h->SetLineColor(color);
    h->SetLineWidth(width);
    h->SetTitle(nullptr);
    h->GetXaxis()->SetTitle("p_{T,D^{0}}(GeV/c)");

    return;

}

void SaveCanvas(TCanvas *c, TString name = "tmp"){

    c->SaveAs(Form("%s.png",name.Data()));
    c->SaveAs(Form("%s.pdf",name.Data()));

}

void DEfficiencyRun(TString file_in,TString file_out, TString mode)

{

    gStyle->SetOptStat(0000); //Mean and RMS shown
    //gSystem->Exec(Form("mkdir -p %s",outDir.Data()));

    TString directoryName = "PWG3_D2H_CFtaskD0toKpi_"+mode;
    TString containerName = "CFHFccontainer0_"+mode;

    TFile* f = new TFile(file_in);
    //TFile* f = new TFile("/mnt/hgfs/vmware/data_141019/MC_FONLL/sub/AnalysisResults_FONLL_LHC16d_pass1.root");
    TDirectoryFile* d = (TDirectoryFile*)f->Get(directoryName.Data());
    AliCFContainer *data = (AliCFContainer*) (d->Get(containerName.Data()));


    Int_t ipt =0;

    Int_t stepNum = -1;
    Int_t stepDen = -1;

   AliCFEffGrid *eff = new AliCFEffGrid("eff"," The efficiency",*data);
   stepDen = 1; //(Int_t)(AliCFTaskVertexingHF::kStepAcceptance);
    stepNum = (Int_t)(AliCFTaskVertexingHF::kStepRecoPPR);
    //eff->CalculateEfficiency(stepNum,stepDen); //eff= step1/step0

    //TH1 *h = eff->Project(ipt);

    const Int_t nDim = 2;
    Int_t dim[nDim] = {0,1} ;

    THnSparse *hNum, *hDen, *ratio;

    AliCFGridSparse*  num = data->GetGrid(stepNum);
    AliCFGridSparse*  den = data->GetGrid(stepDen);

    hNum = num->GetGrid()->Projection(2,dim);
    hDen = den->GetGrid()->Projection(2,dim);

    TH2D *hNum2D = (TH2D*)hNum->Projection(0,1);
    TH2D *hDen2D = (TH2D*)hDen->Projection(0,1);

    hNum->GetAxis(1)->SetRangeUser(-0.9,0.9);
    hDen->GetAxis(1)->SetRangeUser(-0.9,0.9);

    TH1D *hNum2 = (TH1D*)hNum->Projection(0);
    TH1D *hDen2 = (TH1D*)hDen->Projection(0);


    const int     fptbinsDN = 12;
    double        fptbinsDA[fptbinsDN+1] = { 1,2,3,4,5,6,7,8,10,12,16,24,36 };

    TH1D *hNum2_reb = (TH1D*)hNum2->Rebin(fptbinsDN,"hNum2_reb",fptbinsDA);
    TH1D *hDen2_reb = (TH1D*)hDen2->Rebin(fptbinsDN,"hDen2_reb",fptbinsDA);


    //hNum2->Rebin(5);
    //hDen2->Rebin(5);

    TH1D *h = (TH1D*)hNum2->Clone("h");
    h->Divide(hDen2);

    TH1D *h_reb = (TH1D*)hNum2_reb->Clone("h_reb");
    h_reb->Divide(hDen2_reb);

    h_reb->SetMarkerColor(4);
    h_reb->SetMarkerStyle(20);

    /*
    ratio = (THnSparse*)hNum->Clone();
    ratio->Divide(hNum,hDen,1.,1.,"B");
    TH1 *h = ratio->Projection(0);
*/

    TCanvas *c = new TCanvas();
    h->Draw();

    //TFile *outFile = new TFile("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_PRELIM/FONLL/D0Eff_y05.root","RECREATE");
    TFile *outFile = new TFile(file_out,"RECREATE");
    outFile->cd();
    h->Write();
    h_reb->Write();
    hNum->Write();
    hDen->Write();
    hNum2D->Write();
    hDen2D->Write();

    outFile->Close();



}

void DEfficiency(){
    TString pathin = "/mnt/hgfs/vmware/data_141019/MC_FONLL/sub/";
    TString pathout ="/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_PRELIM/FONLL/";

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
        for (Int_t f = 0; f<32;f++){
            DEfficiencyRun(pathin+period[f]+".root",pathout+period[f]+"_"+mode[0]+".root", mode[0]);
        }

    //}




}



