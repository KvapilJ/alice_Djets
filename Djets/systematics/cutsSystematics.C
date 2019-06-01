
#include <string>
#include <sstream>
#include <iostream>

#include "config.h"
//#include "sys.h"

int nJetBins;
double *ptJetbins;
double ptbins[] = {2,3,4,5,6,8,10,12,14,20,30,50};
double ptbinstrue[] = {2,3,4,5,6,8,10,12,14,20,30,50};
/*
Int_t colors2[] = {1,2,4,6,kOrange-1,kGray+1,kCyan+1,kMagenta+2,kGreen+3,kViolet+5,kYellow+2,8};
Int_t markers2[] = {20,21,22,23,24,25,26,27,28,29,30,32,33,34};
Int_t linestyle2[] = {1,2,3,4,5,6,7,8,9,10,11,12,13};*/
Color_t colors2[12] = {1,2,kGreen+3,kMagenta+2,4,6,kCyan+1,8,kOrange-1,kGray+1,kViolet+5,kYellow+2};
Style_t markers2[14] = {20,21,22,23,24,25,26,27,28,29,30,32,33,34};
Style_t linestyle2[16] = {1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

  TPaveText *pvEn;
    TPaveText *pvJet;
    TPaveText *pvD;
    TPaveText *pvEta;
    TPaveText *pv3;

    TPaveText *pvJet2;
    TPaveText *pvD2;
    TPaveText *pvEta2;
    TPaveText *pvJetPt2;


 //   Int_t colors2[] = {2,4,6,1,kOrange-1,kGray+1,kCyan+1,kMagenta+2,kGreen+3,kViolet+5,kYellow+2,8};
 //   Int_t markers2[] = {20,21,22,23,24,25,26,27,28,29,30,32,33,34};
 //   Int_t linestyle2[] = {1,2,3,4,5,6,7,8,9,10,11,12,13};

    const int nFiles = 4;
    TString desc[nFiles] = { "central", "looser 1", "looser 2", "tighter 1"};//, "tighter 2" };

    //const int nFiles = 5;
    //TString desc[nFiles] = { "central", "looser 1", "looser 2", "tighter 1", "tighter 2" }

    TString dirBase;
    TString dirSignal[nFiles] = { "Cuts_VarDef","Cuts_L2","Cuts_L3","Cuts_T2"};//,"Cuts_T3"};
  //  TString dirSignal[nFiles] = { "CutVarBase","CutL0","CutL1","CutT0","CutT2" };
    TString inDir[nFiles];
    TString outDir;
    TFile *outFile;

TString outPlotName = "CutVariationSyst_";
bool barlow = 0;
void compareEfficiencies(int isprompt=1);
void compareRawSpectra();
void compareCorrSpectra(int reg, int unfold);
void compareFD();
void getRMS(const int nFiles, TH1D **hratio, TH1D *hmean, TH1D *hsys);
void getRMSBarlow(const int nFiles,  TH1D **hspectra, TH1D **hratio, TH1D *hmean, TH1D *hsys);


void cutsSystematics(int reg = 3, TString indirbase = "/home/basia/Work/alice/analysis/pPb_run2/DzeroR03_RefDPt3PythiaEff_", TString input = "Default_jetMeas3_50_jetTrue3_50_ppbinning", bool isRaw = 0, bool unf = 0, bool isChain = 0, int measmin=3, int measmax=50, int truemin=3, int truemax=50 )
{
  dirBase = indirbase;
  if(!unf) reg=0;

  outDir = dirBase;
  outDir += "BaseCuts/";
  outDir += "systematics/CutSys";

  if(barlow) outPlotName = "CutVariationSystBarlow_";
  outPlotName += "reg";
  outPlotName += reg;

  gSystem->Exec(Form("mkdir %s",outDir.Data()));

    nJetBins = fptbinsJetTrueN;
    ptJetbins = new double[nJetBins+1];
    for(int i=0;i<nJetBins+1;i++) ptJetbins[i] = fptbinsJetTrueA[i];

  for(int i=0; i<nFiles; i++) {
    inDir[i] = dirBase;
    inDir[i] += dirSignal[i];
    inDir[i] += "/";
    inDir[i] += input;

  }


  pvEn = new TPaveText(0.25,0.80,0.8,0.85,"brNDC");
  pvEn->SetFillStyle(0);
  pvEn->SetBorderSize(0);
  pvEn->SetTextFont(42);
  pvEn->SetTextSize(0.045f);
  pvEn->SetTextAlign(11);
  pvEn->AddText(Form("%s",fSystemS.Data()));

  double shift = 0.35;
  double dshift = 0.05;
  pvJet = new TPaveText(0.12,0.65-shift,0.9,0.7-shift,"brNDC");
  pvJet->SetFillStyle(0);
  pvJet->SetBorderSize(0);
  pvJet->SetTextFont(42);
  pvJet->SetTextSize(0.03f);
  pvJet->SetTextAlign(11);
  pvJet->AddText(Form("Charged Jets, Anti-#it{k}_{T}, #it{R} = 0.%d",Rpar));

  shift+=dshift;
  pvD = new TPaveText(0.12,0.65-shift,0.9,0.7-shift,"brNDC");
  pvD->SetFillStyle(0);
  pvD->SetBorderSize(0);
  pvD->SetTextFont(42);
  pvD->SetTextSize(0.03f);
  pvD->SetTextAlign(11);
  if(fDmesonSpecie) pvD->AddText("with D^{*+} #rightarrow D^{0}#pi^{+} and charge conj.");
  else pvD->AddText("with D^{0} #rightarrow K^{-}#pi^{+} and charge conj.");

  shift+=dshift;
  pvEta = new TPaveText(0.12,0.65-shift,0.9,0.7-shift,"brNDC");
  pvEta->SetFillStyle(0);
  pvEta->SetBorderSize(0);
  pvEta->SetTextFont(42);
  pvEta->SetTextSize(0.03f);
  pvEta->SetTextAlign(11);
  pvEta->AddText(Form("|#it{#eta}_{jet}| < 0.%d",9-Rpar));

  shift+=dshift;
  pv3 = new TPaveText(0.12,0.65-shift,0.9,0.7-shift,"brNDC");
  pv3->SetFillStyle(0);
  pv3->SetBorderSize(0);
  pv3->SetTextFont(42);
  pv3->SetTextSize(0.03f);
  pv3->SetTextAlign(11);
  pv3->AddText(Form("%d < p_{T,%s} < %d GeV/#it{c}",(Int_t)fptbinsDA[0],fDmesonS.Data(),(Int_t)fptbinsDA[fptbinsDN]));

  TPaveText *pvEn= new TPaveText(0.15,0.80,0.8,0.85,"brNDC");
  pvEn->SetFillStyle(0);
  pvEn->SetBorderSize(0);
  pvEn->SetTextFont(42);
  pvEn->SetTextSize(0.045f);
  pvEn->SetTextAlign(11);
  pvEn->AddText(Form("%s",fSystemS.Data()));
 // pvEn->AddText("PYTHIA6+HIJING, p-Pb, #sqrt{#it{s}_{NN}} = 5.02 TeV");

  shift = 0.3;
  pvD2 = new TPaveText(0.2,0.66-shift,0.9,0.7-shift,"brNDC");
  pvD2->SetFillStyle(0);
  pvD2->SetBorderSize(0);
  pvD2->SetTextFont(42);
  pvD2->SetTextSize(0.03f);
  pvD2->SetTextAlign(11);
  if(fDmesonSpecie) pvD2->AddText("D^{*+} #rightarrow D^{0}#pi^{+} and charge conj.");
  else pvD2->AddText("D^{0} #rightarrow K^{-}#pi^{+} and charge conj.");

  pvJet2 = new TPaveText(0.2,0.61-shift,0.9,0.65-shift,"brNDC");
  pvJet2->SetFillStyle(0);
  pvJet2->SetBorderSize(0);
  pvJet2->SetTextFont(42);
  pvJet2->SetTextSize(0.03f);
  pvJet2->SetTextAlign(11);
  pvJet2->AddText(Form("in Charged Jets, Anti-#it{k}_{T}, #it{R} = 0.%d",Rpar));

  pvEta2 = new TPaveText(0.2,0.56-shift,0.8,0.6-shift,"brNDC");
  pvEta2->SetFillStyle(0);
  pvEta2->SetBorderSize(0);
  pvEta2->SetTextFont(42);
  pvEta2->SetTextSize(0.03f);
  pvEta2->SetTextAlign(11);
  pvEta2->AddText(Form("|#it{#eta}_{jet}| < 0.%d",9-Rpar));

  pvJetPt2 = new TPaveText(0.2,0.48-shift,0.8,0.52-shift,"brNDC");
  pvJetPt2->SetFillStyle(0);
  pvJetPt2->SetBorderSize(0);
  pvJetPt2->SetTextFont(42);
  pvJetPt2->SetTextSize(0.03f);
  pvJetPt2->SetTextAlign(11);
  pvJetPt2->AddText(Form("%.0f < p_{T.ch jet} < %.0f GeV/#it{c}",5.,50.));

  TString outFileName = "CutVariationSyst_";
  outFileName += "reg";
  outFileName += reg;

  outFile = new TFile(Form("%s/%s.root",outDir.Data(),outFileName.Data()),"RECREATE");
  compareEfficiencies(1);
  compareEfficiencies(0);
  if(isRaw) compareRawSpectra();
  compareCorrSpectra(reg,unf);
  compareFD();
  outFile->Close();

return;

}

void compareEfficiencies(int isprompt)
{
    gStyle->SetOptStat(000);

    TString fileName;
    if(isprompt) fileName = "DjetEff_prompt_jetpt5_50.root";
    else fileName = "DjetEff_nonPrompt_jetpt5_50.root";
    TH1D *hSpectrum[nFiles];

    TLegend  *leg;
    if(isprompt) leg = new TLegend(0.6,0.15,0.85,0.55,"Prompt efficiency");
    else leg = new TLegend(0.6,0.15,0.85,0.55,"Non-Prompt efficiency");
    TCanvas *cJetPt = new TCanvas("cJetPt","cJetPt",800,600);
    cJetPt->SetLogy();
    for(int i=0;i<nFiles;i++){
        TFile *fileIn = new TFile(Form("%s/efficiency/%s", inDir[i].Data(),fileName.Data()) );
        hSpectrum[i] = (TH1D*)fileIn->Get("hEff_reb");
        hSpectrum[i]->SetLineColor(colors2[i]);
        hSpectrum[i]->SetMarkerColor(colors2[i]);
        hSpectrum[i]->SetMarkerSize(0.9);
        hSpectrum[i]->SetMarkerStyle(markers2[i]);
hSpectrum[i]->GetYaxis()->SetRangeUser(1E-2,1);
        hSpectrum[i]->GetXaxis()->SetRangeUser(3,36);
        //hSpectrum[i]->GetYaxis()->SetTitle("efficiency");
        if(!i) hSpectrum[i]->Draw();
        else hSpectrum[i]->Draw("same");
        leg->AddEntry(hSpectrum[i],Form("%s",desc[i].Data()),"p");
    }
     leg->Draw("same");
     pvEn->Draw("same");
     pvJet2->Draw("same");
     pvD2->Draw("same");
     pvEta2->Draw("same");
     pvJetPt2->Draw("same");


     if (isprompt){
          cJetPt->SaveAs(Form("%s/%s_PromptEfficiencies.pdf",outDir.Data(),outPlotName.Data()));
          cJetPt->SaveAs(Form("%s/%s_PromptEfficiencies.png",outDir.Data(),outPlotName.Data()));
    }
    else {
          cJetPt->SaveAs(Form("%s/%s_NonPromptEfficiencies.pdf",outDir.Data(),outPlotName.Data()));
          cJetPt->SaveAs(Form("%s/%s_NonPromptEfficiencies.png",outDir.Data(),outPlotName.Data()));
    }

     TCanvas *cRatio = new TCanvas("cRatio","cRatio",800,400);

     TH1D *hcentral = (TH1D*) hSpectrum[0]->Clone("hcentral");
     TH1D *hratios[nFiles-1];
     //TF1 *fline;
     TF1 *fline[nFiles-1];

     for(int i=0; i<nFiles-1; i++){
            hratios[i] = (TH1D*)hSpectrum[i+1]->Clone(Form("hratios_%d",i));
            hratios[i]->Divide(hcentral);
            hratios[i]->GetYaxis()->SetTitle("ratio");
            hratios[i]->GetYaxis()->SetRangeUser(0.3,2.5);
            if(!i)hratios[i]->Draw();
            else hratios[i]->Draw("same");

    }
    TLine *l=new TLine(3,1,36,1);
    l->SetLineStyle(2);
    l->SetLineWidth(2);
    l->Draw();

    if (isprompt){
        cRatio->SaveAs(Form("%s/%s_PromptEfficiencies_ratio.pdf",outDir.Data(),outPlotName.Data()));
        cRatio->SaveAs(Form("%s/%s_PromptEfficiencies_ratio.png",outDir.Data(),outPlotName.Data()));
   }
   else {
        cRatio->SaveAs(Form("%s/%s_NonPromptEfficiencies_ratio.pdf",outDir.Data(),outPlotName.Data()));
        cRatio->SaveAs(Form("%s/%s_NonPromptEfficiencies_ratio.png",outDir.Data(),outPlotName.Data()));
   }

}


void compareRawSpectra()
{
    gStyle->SetOptStat(000);

    TString fileName;
    fileName = "JetPtSpectra_SB_noEff.root";
    TH1D *hSpectrum[nFiles];
    double events = 3.87988E8;


    TLegend  *leg = new TLegend(0.65,0.45,0.85,0.85,"raw yields");
    TCanvas *cJetPt = new TCanvas("cJetPt","cJetPt",800,600);
    cJetPt->SetLogy();

    for(int i=0;i<nFiles;i++){
        TFile *fileIn = new TFile(Form("%s/signalExtraction/%s", inDir[i].Data(),fileName.Data()) );
        hSpectrum[i] = (TH1D*)fileIn->Get("hjetptspectrumReb");
        hSpectrum[i]->SetName(Form("rawSpectrum_%d",i));
        hSpectrum[i]->Scale(1./events);
        hSpectrum[i]->Scale(1,"width");

        hSpectrum[i]->SetLineColor(colors2[i]);
        hSpectrum[i]->SetMarkerColor(colors2[i]);
        hSpectrum[i]->SetMarkerSize(0.9);
        hSpectrum[i]->SetMarkerStyle(markers2[i]);
        //hSpectrum[i]->GetXaxis()->SetTitle("#it{p}_{T,D*} (GeV/#it{c})");
        hSpectrum[i]->GetYaxis()->SetTitle("dN/dp_{T} / events");
        hSpectrum[i]->GetYaxis()->SetRangeUser(1E-10,1E-4);
        if(!i) hSpectrum[i]->Draw();
        else hSpectrum[i]->Draw("same");
        leg->AddEntry(hSpectrum[i],Form("%s",desc[i].Data()),"p");

        outFile->cd();
        hSpectrum[i]->Write();
    }
     leg->Draw("same");
     pv3->Draw("same");
     pvEn->Draw("same");
     pvD->Draw("same");
     pvJet->Draw("same");
     pvEta->Draw("same");

          cJetPt->SaveAs(Form("%s/%s_RawSpectra.pdf",outDir.Data(),outPlotName.Data()));
          cJetPt->SaveAs(Form("%s/%s_RawSpectra.png",outDir.Data(),outPlotName.Data()));

     TCanvas *cRatio = new TCanvas("cRatio","cRatio",800,400);

     TH1D *hcentral = (TH1D*) hSpectrum[0]->Clone("hcentral");
     TH1D *hratios[nFiles-1];
     //TF1 *fline;
     TF1 *fline[nFiles-1];

     for(int i=0; i<nFiles-1; i++){
            hratios[i] = (TH1D*)hSpectrum[i+1]->Clone(Form("hratios_%d",i));
            hratios[i]->Divide(hcentral);
            hratios[i]->SetName(Form("rawSpectrumRatio_%d",i));
            hratios[i]->GetYaxis()->SetTitle("ratio");
            hratios[i]->GetYaxis()->SetRangeUser(0.3,2.5);
            if(!i)hratios[i]->Draw();
            else hratios[i]->Draw("same");
            hratios[i]->Write();

    }
    TLine *l=new TLine(3,1,36,1);
    l->SetLineStyle(2);
    l->SetLineWidth(2);
    l->Draw();


        cRatio->SaveAs(Form("%s/%s_RawSpectra_ratio.pdf",outDir.Data(),outPlotName.Data()));
        cRatio->SaveAs(Form("%s/%s_RawSpectra_ratio.png",outDir.Data(),outPlotName.Data()));
}

void compareCorrSpectra(int reg, int unfold)
{
    gStyle->SetOptStat(000);

    TString fileName;
    if(unfold) fileName = "unfoldedSpectrum_unfoldedJetSpectrum.root";
    else fileName = "JetPtSpectrum_FDsub.root";

    TH1D *hSpectrum[nFiles];
    double events = 4.57988E8;

    TLegend  *leg = new TLegend(0.65,0.45,0.85,0.85, "corrected yields");
    TCanvas *cJetPt = new TCanvas("cJetPt","cJetPt",800,600);
    cJetPt->SetLogy();

    for(int i=0;i<nFiles;i++){
        TFile *fileIn;
        if(unfold) {
          std::cout<<"loading: "<<Form("%s/unfolding_Bayes_%d/%s", inDir[i].Data(),reg,fileName.Data())<<"unfoldedSpectrum"<<std::endl;
          fileIn = new TFile(Form("%s/unfolding_Bayes_%d/%s", inDir[i].Data(),reg,fileName.Data()) );
          hSpectrum[i] = (TH1D*)fileIn->Get("unfoldedSpectrum");
          if(!hSpectrum[i]) std::cout<<"not found: "<<Form("%s/unfolding_Bayes_%d/%s", inDir[i].Data(),reg,fileName.Data())<<"unfoldedSpectrum"<<std::endl;
          //hSpectrum[i]->Draw();
        }
        else {
          fileIn = new TFile(Form("%s/FDsubtraction/%s", inDir[i].Data(),fileName.Data()) );
          hSpectrum[i] = (TH1D*)fileIn->Get("hData_binned_sub");
        }
        hSpectrum[i]->SetName(Form("corrSpectrum_%d",i));
        hSpectrum[i]->Scale(1./events);
        hSpectrum[i]->Scale(1,"width");
        hSpectrum[i]->SetTitle("");
std::cout<<"color is: "<<colors2[i]<<std::endl;
        hSpectrum[i]->SetLineColor(colors2[i]);
        hSpectrum[i]->SetMarkerColor(colors2[i]);
        hSpectrum[i]->SetMarkerSize(1.0);
        hSpectrum[i]->SetMarkerStyle(22+i);
        hSpectrum[i]->GetYaxis()->SetTitle("dN/dp_{T} / events");
        cJetPt->cd();
        if(!i) hSpectrum[i]->Draw();
        else hSpectrum[i]->Draw("same");
        leg->AddEntry(hSpectrum[i],Form("%s",desc[i].Data()),"p");
        outFile->cd();
        hSpectrum[i]->Write();
    }
     leg->Draw("same");
     pv3->Draw("same");
     pvEn->Draw("same");
     pvD->Draw("same");
     pvJet->Draw("same");
     pvEta->Draw("same");


          cJetPt->SaveAs(Form("%s/%s_CorrectedSpectra.pdf",outDir.Data(),outPlotName.Data()));
          cJetPt->SaveAs(Form("%s/%s_CorrectedSpectra.png",outDir.Data(),outPlotName.Data()));


     TCanvas *cRatio = new TCanvas("cRatio","cRatio",800,400);

     TH1D *hcentral = (TH1D*) hSpectrum[0]->Clone("hcentral");
     TH1D *hratios[nFiles-1];
     //TF1 *fline;
     TF1 *fline[nFiles-1];

     for(int i=0; i<nFiles-1; i++){
            hratios[i] = (TH1D*)hSpectrum[i+1]->Clone(Form("hratios_%d",i));
            hratios[i]->Divide(hcentral);
            hratios[i]->SetName(Form("corrSpectrumRatio_%d",i));
            hratios[i]->GetYaxis()->SetTitle("ratio");
            hratios[i]->GetYaxis()->SetRangeUser(0.7,1.8);

    }

    TH1D *hsys = new TH1D("hsys","syst. rms; p_{T,ch jet};  sys [%] (rms)",nJetBins,ptJetbins);
    TH1D *hmean = (TH1D*)hsys->Clone("hmean");
    if(barlow) getRMSBarlow(nFiles,hSpectrum,hratios,hmean,hsys);
    else getRMS(nFiles,hratios,hmean,hsys);

    for(int i=0; i<nFiles-1; i++){
           if(!i)hratios[i]->Draw();
           else hratios[i]->Draw("same");
           hratios[i]->Write();
   }

    TLine *l=new TLine(3,1,50,1);
    l->SetLineStyle(2);
    l->SetLineWidth(2);
    l->Draw();

        cRatio->SaveAs(Form("%s/%s_CorrectedSpectra_ratio.pdf",outDir.Data(),outPlotName.Data()));
        cRatio->SaveAs(Form("%s/%s_CorrectedSpectra_ratio.png",outDir.Data(),outPlotName.Data()));

        hsys->SetName("cutSysRMS");
        hmean->SetName("cutSysMean");

        hsys->GetYaxis()->SetRangeUser(0,45);
        hsys->GetXaxis()->SetRangeUser(5,50);
        hsys->SetLineColor(kViolet+2);
        TCanvas *cspecRMS = new TCanvas("cspecRMS","cspecRMS",800,400);
        hsys->Draw("hist");

        cspecRMS->SaveAs(Form("%s/%s_CorrectedSpectra_rms.pdf",outDir.Data(),outPlotName.Data()));
        cspecRMS->SaveAs(Form("%s/%s_CorrectedSpectra_rms.png",outDir.Data(),outPlotName.Data()));

        hmean->GetYaxis()->SetRangeUser(0.93,1.2);
        hmean->SetLineColor(kMagenta+1);
        TCanvas *cspecMean = new TCanvas("cspecMean","cspecMean",800,400);
        hmean->Draw("hist");
        l->Draw("same");

        cspecMean->SaveAs(Form("%s/%s_CorrectedSpectra_mean.pdf",outDir.Data(),outPlotName.Data()));
        cspecMean->SaveAs(Form("%s/%s_CorrectedSpectra_mean.png",outDir.Data(),outPlotName.Data()));

        hsys->Write();
        hmean->Write();

}

void compareFD()
{
    gStyle->SetOptStat(000);

    TString fileName;
    fileName = "JetPtSpectrum_FDsub.root";
    TH1D *hSpectrum[nFiles];
    double events = 3.87988E8;

    TLegend  *leg = new TLegend(0.6,0.12,0.85,0.45);
    TCanvas *cJetPt = new TCanvas("cJetPt","cJetPt",800,600);
    //cJetPt->SetLogy();
    for(int i=0;i<nFiles;i++){
        TFile *fileIn = new TFile(Form("%s/FDsubtraction/%s", inDir[i].Data(),fileName.Data()) );
        hSpectrum[i] = (TH1D*)fileIn->Get("hFD_ratio");
        //hSpectrum[i]->Scale(1./events);
        //hSpectrum[i]->Scale(1,"width");
        hSpectrum[i]->SetName(Form("FDfraction_%d",i));
        hSpectrum[i]->SetLineColor(colors2[i]);
        hSpectrum[i]->SetMarkerColor(colors2[i]);
        hSpectrum[i]->SetMarkerSize(0.9);
        hSpectrum[i]->SetMarkerStyle(markers2[i]);
        //hSpectrum[i]->GetXaxis()->SetTitle("#it{p}_{T,D*} (GeV/#it{c})");
        hSpectrum[i]->GetYaxis()->SetTitle("FD ratio");
        hSpectrum[i]->GetYaxis()->SetRangeUser(0,0.5);
        if(!i) hSpectrum[i]->Draw();
        else hSpectrum[i]->Draw("same");
        leg->AddEntry(hSpectrum[i],Form("%s",desc[i].Data()),"p");
        outFile->cd();
        hSpectrum[i]->Write();
    }
     leg->Draw("same");


          cJetPt->SaveAs(Form("%s/%s_FDFraction.pdf",outDir.Data(),outPlotName.Data()));
          cJetPt->SaveAs(Form("%s/%s_FDFraction.png",outDir.Data(),outPlotName.Data()));


     TCanvas *cRatio = new TCanvas("cRatio","cRatio",800,400);

     TH1D *hcentral = (TH1D*) hSpectrum[0]->Clone("hcentral");
     TH1D *hratios[nFiles-1];
     //TF1 *fline;
     TF1 *fline[nFiles-1];

     for(int i=0; i<nFiles-1; i++){
            hratios[i] = (TH1D*)hSpectrum[i+1]->Clone(Form("hratios_%d",i));
            hratios[i]->Divide(hcentral);
            hratios[i]->SetName(Form("FDfractionRatio_%d",i));
            hratios[i]->GetYaxis()->SetTitle("ratio");
            hratios[i]->GetYaxis()->SetRangeUser(0.7,1.5);
            if(!i)hratios[i]->Draw();
            else hratios[i]->Draw("same");
            hratios[i]->Write();

    }
    TLine *l=new TLine(3,1,36,1);
    l->SetLineStyle(2);
    l->SetLineWidth(2);
    l->Draw();

        cRatio->SaveAs(Form("%s/%s_FDFraction_ratio.pdf",outDir.Data(),outPlotName.Data()));
        cRatio->SaveAs(Form("%s/%s_FDFraction_ratio.png",outDir.Data(),outPlotName.Data()));

}


void getRMS(const int nFiles, TH1D **hratio, TH1D *hmean, TH1D *hsys)
{
  //TH1D *hsys = new TH1D("hsys","syst. rms; p_{T,ch jet};  sys [%] (rms)",nJetBins,ptJetbins);
  hsys->SetTitle("");
  hsys->SetLineColor(1);
  hsys->SetLineWidth(2);
  hsys->SetLineStyle(2);
  hsys->SetMaximum(0.12);
  hsys->GetXaxis()->SetLabelSize(0.05);
  hsys->GetXaxis()->SetTitleSize(0.05);
  hsys->GetYaxis()->SetTitleSize(0.05);
  hsys->GetYaxis()->SetLabelSize(0.05);
  hsys->GetYaxis()->SetTitleOffset(0.8);

  hmean->GetYaxis()->SetTitle("mean");
  hmean->SetMarkerStyle(20);
  hmean->SetLineStyle(1);
  hmean->SetTitle("");

  double *rms = new double[nJetBins];
  double *mean = new double[nJetBins];
  for(int i=0; i<nJetBins; i++){
      rms[i] = 0;
      mean[i] = 0;
      for (int j=0; j<nFiles-1; j++){
        mean[i] = mean[i]+ ( hratio[j]->GetBinContent(hratio[j]->FindBin( (ptJetbins[i]+ptJetbins[i+1])/2. )) );
      }
      mean[i] = mean[i]/(double)(nFiles-1);

      for (int j=0; j<nFiles-1; j++){
        rms[i] = rms[i]+ ( 1-hratio[j]->GetBinContent(hratio[j]->FindBin( (ptJetbins[i]+ptJetbins[i+1])/2. )) ) * ( 1-hratio[j]->GetBinContent(hratio[j]->FindBin( (ptJetbins[i]+ptJetbins[i+1])/2. )) ) ;
      }
      rms[i] = sqrt(rms[i]/(double)(nFiles-1));
      hsys->SetBinContent(i+1,rms[i]*100);

      hmean->SetBinContent(i+1,mean[i]);
      std::cout << "RMS pT " << (ptJetbins[i]+ptJetbins[i+1])/2. << " GeV/c:\t" << rms[i]*100 << std::endl;
      std::cout << "Mean pT " << (ptJetbins[i]+ptJetbins[i+1])/2. << " GeV/c:\t" << mean[i] << std::endl;
  }

}


void getRMSBarlow(const int nFiles,  TH1D **hspectra, TH1D **hratio, TH1D *hmean, TH1D *hsys)
{
  //TH1D *hsys = new TH1D("hsys","syst. rms; p_{T,ch jet};  sys [%] (rms)",nJetBins,ptJetbins);
  hsys->SetTitle("");
  hsys->SetLineColor(1);
  hsys->SetLineWidth(2);
  hsys->SetLineStyle(2);
  hsys->SetMaximum(0.12);
  hsys->GetXaxis()->SetLabelSize(0.05);
  hsys->GetXaxis()->SetTitleSize(0.05);
  hsys->GetYaxis()->SetTitleSize(0.05);
  hsys->GetYaxis()->SetLabelSize(0.05);
  hsys->GetYaxis()->SetTitleOffset(0.8);

  hmean->GetYaxis()->SetTitle("mean");
  hmean->SetMarkerStyle(20);
  hmean->SetLineStyle(1);
  hmean->SetTitle("");

  double *rms = new double[nJetBins];
  double *mean = new double[nJetBins];
  for(int i=0; i<nJetBins; i++){
      rms[i] = 0;
      mean[i] = 0;
      double errBase = hspectra[0]->GetBinError(hspectra[0]->FindBin( (ptJetbins[i]+ptJetbins[i+1])/2. ));
      int files = 0;
      for (int j=0; j<nFiles-1; j++){
        double err = hspectra[j+1]->GetBinError(hspectra[j]->FindBin( (ptJetbins[i]+ptJetbins[i+1])/2. ));
        double errSigma = TMath::Sqrt( TMath::Abs(errBase*errBase - err*err) );
        double diff = TMath::Abs( hspectra[j+1]->GetBinError(hspectra[j]->FindBin( (ptJetbins[i]+ptJetbins[i+1])/2. )) - hspectra[0]->GetBinError(hspectra[0]->FindBin( (ptJetbins[i]+ptJetbins[i+1])/2. )) );
        std::cout << "jetbin: " << i << "\t file: " << j << "\t dif: " << diff << "\t errSigma: " << 2*errSigma << std::endl;
        if ( diff < 2*errSigma ) {
          hratio[j]->SetBinContent(hratio[j]->FindBin( (ptJetbins[i]+ptJetbins[i+1])/2. ),1);
          hratio[j]->SetBinError(hratio[j]->FindBin( (ptJetbins[i]+ptJetbins[i+1])/2. ),0);
            continue;
        }
        std::cout << "!! significant deviation !!" << std::endl;
        files++;
        mean[i] = mean[i]+ ( hratio[j]->GetBinContent(hratio[j]->FindBin( (ptJetbins[i]+ptJetbins[i+1])/2. )) );
        rms[i] = rms[i]+ ( 1-hratio[j]->GetBinContent(hratio[j]->FindBin( (ptJetbins[i]+ptJetbins[i+1])/2. )) ) * ( 1-hratio[j]->GetBinContent(hratio[j]->FindBin( (ptJetbins[i]+ptJetbins[i+1])/2. )) ) ;
      }
      if(files){
        mean[i] = mean[i]/(double)(files);
        rms[i] = sqrt(rms[i]/(double)(files));
      }
      else {
        mean[i] = 0;
        rms[i] = 0;
      }
      hsys->SetBinContent(i+1,rms[i]*100);
      hmean->SetBinContent(i+1,mean[i]);

      std::cout << "RMS pT " << (ptJetbins[i]+ptJetbins[i+1])/2. << " GeV/c:\t" << rms[i]*100 << std::endl;
      std::cout << "Mean pT " << (ptJetbins[i]+ptJetbins[i+1])/2. << " GeV/c:\t" << mean[i] << std::endl;
  }

}
