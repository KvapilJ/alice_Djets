//-----------------------------------------------------------------------
//  Author B.Trzeciak
//  Utrecht University
//  barbara.antonina.trzeciak@cern.ch
//-----------------------------------------------------------------------
//
// Macro to extract jet pt spectra from simulation, prompt or non-prompt D
//

#include "config.h"
#include <fstream>
#include "TBranchElement.h"
//fDmesonSpecie = 1;
//fRpar = 0.4;
//Rpar = 4;

Double_t dptmin = 5, dptmax = 30; // for D pT spectra
Double_t jetptmin = 5, jetptmax = 30; // for D pT spectra
Double_t jetEta = 0.9-fRpar;
Int_t BFDsim;
Int_t dptN;
Int_t fzBin;

TH1* GetInputSimHistJet(TString inFile, TH1 *hPt, Bool_t isEff, TString effFilePrompt, TString effFileNonPrompt,Bool_t isDptcut, Int_t isNPrompt, Int_t SimNr,TH1D **hz);
TH1* GetInputSimHistD(TString inFile, TH1 *hPt, Bool_t isjetptcut);



//quark: 1 = beauty, 0 = charm
void getSimSpectraTree(
TString simFile = "./",
Int_t simNr = 0, Int_t quark = 1, Bool_t jet = 1,  Bool_t isDptcut = 1, Bool_t isEff = 0,
TString effFilePrompt = "$HOME/file.root",
TString effFileNonPrompt = "$HOME/file.root",
TString outFileDir = "outR03Test/",
Bool_t isjetptcut = 0, Double_t jetmin = 5, Double_t jetmax = 30, Int_t zBin=0 );

Double_t GetEfficiency(TH1 *hh, Double_t Dpt);
void setHistoDetails(TH1 *hh, Color_t color, Style_t Mstyle, Width_t width, TString name);
void SaveCanvas(TCanvas *c, TString name = "tmp");

//quark: 1 = beauty, 0 = charm
void getSimSpectraTree(TString simFile, Int_t simNr,
  Int_t quark, Bool_t jet, Bool_t isDptcut,
  Bool_t isEff, TString effFilePrompt, TString effFileNonPrompt,
  TString outFileDir,
  Bool_t isjetptcut, Double_t jetmin, Double_t jetmax, Int_t zBin )
{
std::cout<<"A"<<std::endl;
    gSystem->Exec(Form("mkdir -p %s",outFileDir.Data()));

    jetptmin = jetmin;
    jetptmax = jetmax;
    BFDsim = quark;
    dptmin=fptbinsDA[0];
    dptmax=fptbinsDA[fptbinsDN];
    dptN=fptbinsDN;

    if(fObservable == Observable::kFragmentation){

        if(quark == 0){
            jetptmin = fzptJetTrueA[zBin-1];
            jetptmax = fzptJetTrueA[zBin];
            dptmin=fzptbinsDPromptA[zBin-1][0];
            dptmax=fzptbinsDPromptA[zBin-1][fzptbinsDPromptN[zBin-1]];
            dptN=fzptbinsDPromptN[zBin-1];
        }
        else if(quark == 1){
            jetptmin = fzptJetMeasA[zBin-1];
            jetptmax = fzptJetMeasA[zBin];
            dptmin=fzptbinsDA[zBin-1][0];
            dptmax=fzptbinsDA[zBin-1][fzptbinsDN[zBin-1]];
            dptN=fzptbinsDN[zBin-1];
        }
            fzBin=zBin;
            isjetptcut=true;
    }

    simFile += "/";
    if(quark == 1) simFile += fRunB[simNr];
    else if(quark == 0) simFile += fRunC[simNr];
    simFile += ".root";
std::cout<<"B"<<std::endl;
    TH1D *hPt = nullptr;
    TH1D *hz = nullptr;
    if(jet) hPt = dynamic_cast<TH1D*>(GetInputSimHistJet(simFile.Data(),hPt, isEff, effFilePrompt.Data(), effFileNonPrompt.Data(),isDptcut,quark,simNr,&hz));
    else hPt = dynamic_cast<TH1D*>(GetInputSimHistD(simFile.Data(),hPt,isjetptcut));

    TString out = outFileDir;
    if(jet) out += "/JetPt_";
    else out += "/DPt_";
    if(quark == 1) out += fRunB[simNr];
    else if(quark == 0) out += fRunC[simNr];
std::cout<<"C"<<std::endl;
if(fObservable == Observable::kXsection){
    if(jet){
        if(isDptcut) { out += "_Dpt"; out +=  dptmin; out += "_"; out += dptmax;  }
    }
    else{
        if(isjetptcut){ out += "_Jetpt"; out +=  jetptmin; out += "_"; out += jetptmax; }
    }
}
if(fObservable == Observable::kFragmentation){
    out += "_Dpt"; out +=  dptmin; out += "_"; out += dptmax;
    out += "_Jetpt"; out +=  jetptmin; out += "_"; out += jetptmax;
}

    if(jet && isEff) out += "_effScaled";
    if(fDmesonSpecie) out += "_Dstar";
    else out += "_Dzero";
    out += ".root";
std::cout<<"D"<<std::endl;
    TFile *ofile = new TFile( out.Data() ,"RECREATE");
    hPt->Write();
    hz->Write();
    ofile->Close();

    return;
}

TH1* GetInputHist(TString inFile, TString histName,TH1 *hh){

    TFile *jetPtFile = new TFile(inFile,"read");
    hh = dynamic_cast<TH1*>(jetPtFile->Get(histName));

    //hh = (TH1F*)hh_tmp->Rebin(ptbinsJetN,"hh",ptbinsJet);
    return hh;
}

TH1* GetInputSimHistJet(TString inFile, TH1 *hPt, Bool_t isEff, TString effFilePrompt, TString effFileNonPrompt,Bool_t isDptcut, Int_t isNPrompt, Int_t SimNr,TH1D **hz){
std::cout<<"BA"<<std::endl;
    TFile *fileInput = new TFile(inFile,"read");
    if(!fileInput){
      std::cout << "File " << fileInput << " cannot be opened! check your file path!" << std::endl; return nullptr;
    }

    TList* dir=dynamic_cast<TList*>(fileInput->Get("AliAnalysisTaskDmesonJets_histos"));
    if(!dir) {
      std::cout << "Error in getting dir! Exiting..." << std::endl;
      return nullptr;
    }


std::cout<<"BB"<<std::endl;
    TProfile *hxsection;
    TProfile *hxsection2;
    if(!isNPrompt)	hxsection = dynamic_cast<TProfile*>(dir->FindObject("fHistXsection"));
    else { 
          if(SimNr == 11)  hxsection = dynamic_cast<TProfile*>(dir->FindObject("fHistXsectionDistribution"));
          else hxsection = dynamic_cast<TProfile*>(dir->FindObject("fHistXsection"));
          //else hxsection = (TH1D*)dir->FindObject("fHistXsection");
    }
    hxsection2 = dynamic_cast<TProfile*>(dir->FindObject("fHistXsectionNoSel"));
    TH1F *trials1 = dynamic_cast<TH1F*>(dir->FindObject("fHistTrialsVsPtHard"));
    TH1F *trials2 = dynamic_cast<TH1F*>(dir->FindObject("fHistTrialsVsPtHardNoSel"));
    TH1F *evWeight = dynamic_cast<TH1F*>(dir->FindObject("fHistEventWeights"));
 std::cout<<"BC"<<std::endl;
    if(!hxsection) {
      std::cout << "Error in getting x-section hist! Exiting..." << std::endl;
      return nullptr;
    }
    Double_t xsection = hxsection->GetMean(2);
    Double_t events = hxsection->GetEntries();
    //Double_t scaling = xsection/events;

    Double_t xsection2 = hxsection2->GetMean(2);
    Double_t events2 = hxsection2->GetEntries();

    Double_t trial1 = trials1->Integral();
    Double_t trial2 = trials2->Integral();
    Double_t evw = evWeight->Integral();
Double_t scaling = 1./(trial1);
//scaling = scaling/60;

std::cout<<"x section: "<< xsection<<" events: "<<events<<" trials: "<<trial1<<" scaling: "<<scaling<<" "<<xsection/trial1<<std::endl;
std::cout<<"x section nosel: "<< xsection2<<" events: "<<events2<<" trials: "<<trial2<<" scaling: "<<xsection2/events2<<" "<<xsection2/trial2<<std::endl;
std::cout<<"BD"<<std::endl;
    TTree *tree;
    if(!fDmesonSpecie) tree = dynamic_cast<TTree*>(fileInput->Get("AliAnalysisTaskDmesonJets_D0_MCTruth"));
    else tree = dynamic_cast<TTree*>(fileInput->Get("AliAnalysisTaskDmesonJets_DStar_MCTruth"));

std::cout<<"BE"<<std::endl;
    if(!tree) {
      std::cout << "Error in setting the tree/branch names! Exiting..." << std::endl;
      return nullptr;
    }

std::cout<<"BF"<<std::endl;
    TH1D *hPromptEff = nullptr;
    if(isEff) hPromptEff = dynamic_cast<TH1D*>(GetInputHist(effFilePrompt,"hEff_reb",hPromptEff));
    TH1D *hNonPromptEff = nullptr;
    if(isEff) hNonPromptEff = dynamic_cast<TH1D*>(GetInputHist(effFileNonPrompt,"hEff_reb",hNonPromptEff));

std::cout<<"BG"<<std::endl;
    TH1D *hjetpt[dptN];
    TH1D *hjetz[dptN];
    for (Int_t j=0; j<dptN; j++) {
        hjetpt[j] = new TH1D(Form("hjetpt_%d",j),"hjetpt",100,0,100);
        hjetpt[j]->Sumw2();
        hjetz[j] = new TH1D(Form("hjetz_%d",j),"hjetz",fzbinsJetMeasN[fzBin-1],fzbinsJetMeasA[fzBin-1]);
        hjetz[j]->Sumw2();
    }
    Double_t effC, effB, eff;
    hPt = new TH1D("hPt","hjetpt",100,0,100);
    *hz = new TH1D("hz","hz",fzbinsJetMeasN[fzBin-1],fzbinsJetMeasA[fzBin-1]);

    TH1D *hweight = new TH1D("hweight","hweight",60000,0,600);
    tree->Draw("Event.fWeight>>hweight","Event.fWeight >=0","goff");
    Double_t wint = hweight->Integral();

std::cout<<"BH: "<<wint<<std::endl;
//scaling=scaling/wint;

//Jet_AKTChargedR040_pt_scheme>>hjetpt[0](100,0,100) Dmesons.fPartonType == 4&&Jet_AKTChargedR040_pt_scheme.fEta < 0.5 && -0.5 < Jet_AKTChargedR040_pt_scheme.fEta&&Dmesons.fPt < 3 && 2 < Dmesons.fPt
//tree->Draw("Jet_AKTChargedR040_pt_scheme.fPt>>h(100,0,100)","Dmesons.fPartonType == 4 && Jet_AKTChargedR040_pt_scheme.fEta < 0.5 && -0.5 < Jet_AKTChargedR040_pt_scheme.fEta &&Dmesons.fPt > 2 && Dmesons.fPt < 36");

TCanvas *ctest = new TCanvas("ctest","ctest",2000,2000);
ctest->Divide(4,3);
//tree->SetWeight("Event.fWeight");

    if(isDptcut){
      for (Int_t j=0; j<dptN; j++) {
          TString projection = "";
          //projection+=Form("Jet_AKTChargedR0%d0_pt_scheme.fPt",Rpar);
          projection+=Form("Jet_AKTChargedR0%d0_pt_scheme.fPt",Rpar);
          projection+=Form(">>hjetpt_%d",j);
          TString cuts = "(";
          cuts+=Form("Dmesons.fPartonType == %d",BFDsim?5:4);//DmesonJet
          cuts+="&&";
          cuts+=Form("Jet_AKTChargedR040_pt_scheme.fEta <= 0.%d && -0.%d <= Jet_AKTChargedR040_pt_scheme.fEta",9-Rpar,9-Rpar);
          cuts+="&&";
          cuts+=Form("Dmesons.fPt < %.0f && %.0f <= Dmesons.fPt",fptbinsDA[j+1],fptbinsDA[j]);
          cuts+="&&Event.fWeight >=0)*Event.fWeight";
        //if ((brD->fPt < fptbinsDA[j] || brD->fPt >= fptbinsDA[j+1])&&(fObservable == Observable::kXsection)) continue;
        //if ((brD->fPt < fzptbinsDA[fzBin-1][j] || brD->fPt >= fzptbinsDA[fzBin-1][j+1])&&(fObservable == Observable::kFragmentation)) continue;
        //hjetpt[j]->Fill(brJet->fPt);
        //if (brJet->fPt >= fzptJetMeasA[fzBin-1] && brJet->fPt < fzptJetMeasA[fzBin]){ hjetz[j]->Fill(brJet->fZ);
        //myfile<<k<<" "<<brD->fPt<<" "<<brJet->fPt<<" "<<brJet->fZ<<std::endl;
          //std::cout<<j<< " "<<fptbinsDA[j]<<"-"<<fptbinsDA[j+1]<<std::endl;
          //std::cout<<projection<<" "<<cuts<<std::endl;
         tree->Draw(projection,cuts,"goff");
         // hjetpt[j]->Fill(j);
         //hjetpt[j] = (TH1D*)gPad->GetPrimitive("htemp");


       // }
      }//end of D-meson pT bin loop
    }
    //else hPt->Fill(brJet->fPt);
/*
   for (Int_t j=0; j<dptN; j++)  {
    ctest->cd(j+1);
    hjetpt[j]->Draw();
}*/
ctest->SaveAs("cTest.png");
//myfile.close();
std::cout<<"BI"<<std::endl;
if(isDptcut){
  for (Int_t j=0; j<dptN; j++){
    Double_t pt = (fptbinsDA[j]+fptbinsDA[j+1])/2.;
    if(fObservable == Observable::kFragmentation)pt = (fzptbinsDA[fzBin-1][j]+fzptbinsDA[fzBin-1][j+1])/2.;
    if(isEff)  {
        effC = GetEfficiency(hPromptEff,pt);
        effB = GetEfficiency(hNonPromptEff,pt);
        eff = effB / effC;
    }
    else eff = 1;
    if (!j){
        hPt = dynamic_cast<TH1D*>(hjetpt[j]->Clone("hPt"));
        hPt->Scale(eff);
        *hz = dynamic_cast<TH1D*>(hjetz[j]->Clone("hz"));
        (*hz)->Scale(eff);
    }
    else{
        hPt->Add(hjetpt[j],eff);
        (*hz)->Add(hjetz[j],eff);
    }
  }
}
std::cout<<"BJ"<<std::endl;
hPt->Scale(scaling);
(*hz)->Scale(scaling);

 if(!hPt) {
   std::cout << "Error in extracting the mass plot! Exiting..." << std::endl;
   return nullptr;
 }

return hPt;

}


TH1* GetInputSimHistD(TString inFile, TH1 *hPt, Bool_t isjetptcut){

    TFile *fileInput = new TFile(inFile,"read");
    if(!fileInput){
      std::cout << "File " << fileInput << " cannot be opened! check your file path!" << std::endl; return nullptr;
    }

    TList* dir=dynamic_cast<TList*>(fileInput->Get("AliAnalysisTaskDmesonJets_histos"));
    if(!dir){
      std::cout << "Error in getting dir! Exiting..." << std::endl;
      return nullptr;
    }

    TProfile *hxsection = dynamic_cast<TProfile*>(dir->FindObject("fHistXsectionVsPtHard"));
    if(!hxsection){
      std::cout << "Error in getting x-section hist! Exiting..." << std::endl;
      return nullptr;
    }
    Double_t xsection = hxsection->GetMean(2);
    Double_t events = hxsection->GetEntries();
    Double_t scaling = xsection/events;



    TTree *tree;
    if(!fDmesonSpecie) tree = dynamic_cast<TTree*>(fileInput->Get("AliAnalysisTaskDmesonJets_D0_MCTruth"));
    else tree = dynamic_cast<TTree*>(fileInput->Get("AliAnalysisTaskDmesonJets_DStar_MCTruth"));
    AliAnalysisTaskDmesonJets::AliDmesonInfoSummary *brD = nullptr;

    AliAnalysisTaskDmesonJets::AliJetInfoSummary *brJet = nullptr;
    tree->SetBranchAddress("DmesonJet",&brD);

    tree->SetBranchAddress(Form("Jet_AKTChargedR0%d0_pt_scheme",Rpar),&brJet);

    if(!tree || !brD || !brJet) {
      std::cout << "Error in setting the tree/branch names! Exiting..." << std::endl;
      return nullptr;
    }

    hPt = new TH1D("hPt","hDpt",100,0,100);
    // hPt = new TH1D("hPt","hDpt",20,-2,2);
    for (Int_t k=0; k<tree->GetEntries(); k++) {
    tree->GetEntry(k);
    //if(isjetptcut) { if (brJet->fEta < -jetEta || brJet->fEta > jetEta) continue; }
    //if (brJet->fEta < -jetEta || brJet->fEta > jetEta) continue;
    if(isjetptcut) { if (brJet->fPt < jetptmin || brJet->fPt >= jetptmax) continue; }
    if (brD->fEta < -1 || brD->fEta > 1) continue;
      hPt->Fill(brD->fPt);
      //hPt->Fill(brD->fEta);
    }

    hPt->Scale(scaling);

    if(!hPt) {
      std::cout << "Error in extracting the mass plot! Exiting..." << std::endl;
      return nullptr;
    }

    return hPt;
}

Double_t GetEfficiency(TH1 *hh, Double_t Dpt){
    return hh->GetBinContent(hh->GetXaxis()->FindBin(Dpt));
}

void setHistoDetails(TH1 *hh, Color_t color, Style_t Mstyle, Width_t width, TString name){
    hh->SetMarkerColor(color);
    hh->SetMarkerStyle(Mstyle);;
    hh->SetLineColor(color);
    hh->SetLineWidth(2);
    hh->SetMarkerSize(1.1);
    hh->SetName(name);
    hh->SetTitle("");
    hh->GetXaxis()->SetTitle("p_{T}^{ch,jet} (GeV/c)");
}

void SaveCanvas(TCanvas *c, TString name){
    c->SaveAs(name+".png");
    c->SaveAs(name+".pdf");
    c->SaveAs(name+".svg");
}
