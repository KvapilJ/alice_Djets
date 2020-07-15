#define tree_cxx
#include "tree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "config.h"

Double_t GetEfficiency(TH1 *hh, Double_t Dpt);
TH1* GetInputHist(TString inFile, TString histName,TH1 *hh){

    TFile *jetPtFile = new TFile(inFile,"read");
    hh = dynamic_cast<TH1*>(jetPtFile->Get(histName));

    //hh = (TH1F*)hh_tmp->Rebin(ptbinsJetN,"hh",ptbinsJet);
    return hh;
}
void tree::Loop()


{

  Int_t  dptN=fptbinsDN;
  Double_t jetEta = 0.9-fRpar;
//   In a ROOT session, you can do:
//      root> .L tree.C
//      root> tree t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;
//TH1I *hparton = new TH1I("hparton","hparton",10,0,10);
TH1D *hjetpt[dptN];
    TH1D *hjetz[dptN];
    for (Int_t j=0; j<dptN; j++) {
        hjetpt[j] = new TH1D(Form("hjetpt_%d",j),"hjetpt",100,0,100);
        hjetpt[j]->Sumw2();
      //  hjetz[j] = new TH1D(Form("hjetz_%d",j),"hjetz",fzbinsJetMeasN[fzBin-1],fzbinsJetMeasA[fzBin-1]);
      //  hjetz[j]->Sumw2();
    }
    Double_t effC, effB, eff;
    TH1D *hPt = new TH1D("hPt","hjetpt",100,0,100);

    TH1D *hPromptEff = nullptr;
    if(true) hPromptEff = dynamic_cast<TH1D*>(GetInputHist("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts/Default_AnalysisResults_Run2w18b.root/efficiency/DjetEff_prompt_jetpt5.00_50.00.root","hEff_reb",hPromptEff));
    TH1D *hNonPromptEff = nullptr;
    if(true) hNonPromptEff = dynamic_cast<TH1D*>(GetInputHist("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts/Default_AnalysisResults_Run2w18b.root/efficiency/DjetEff_nonPrompt_jetpt5.00_50.00.root","hEff_reb",hNonPromptEff));

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;



if(ientry<20)std::cout<<Dmesons_<<std::endl;

for (Int_t j=0; j<dptN; j++) {
    for(Int_t d=0 ; d< Dmesons_;d++){
if(Dmesons_fPartonType[d] != 4) continue;
if (Jet_AKTChargedR040_pt_scheme_fEta[d] < -jetEta || Jet_AKTChargedR040_pt_scheme_fEta[d] > jetEta) continue;
if ((Dmesons_fPt[d] < fptbinsDA[j] || Dmesons_fPt[d] >= fptbinsDA[j+1])) continue;
hjetpt[j]->Fill(Jet_AKTChargedR040_pt_scheme_fPt[d],fWeight);
}
}
    
   }
for (Int_t j=0; j<dptN; j++){
    Double_t pt = (fptbinsDA[j]+fptbinsDA[j+1])/2.;
    if(true)  {
        effC = GetEfficiency(hPromptEff,pt);
        effB = GetEfficiency(hNonPromptEff,pt);
        eff = effB / effC;
    }
    else eff = 1;
    if (!j){
        hPt = dynamic_cast<TH1D*>(hjetpt[j]->Clone("hPt"));
        hPt->Scale(eff);
    }
    else{
        hPt->Add(hjetpt[j],eff);
    }
  }
hPt->Draw();
}

Double_t GetEfficiency(TH1 *hh, Double_t Dpt){
    return hh->GetBinContent(hh->GetXaxis()->FindBin(Dpt));
}
