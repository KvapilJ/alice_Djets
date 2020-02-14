//-----------------------------------------------------------------------
//  Author B.Trzeciak
//  Utrecht University
//  barbara.antonina.trzeciak@cern.ch
//-----------------------------------------------------------------------

#include "config.h"

using namespace std;
//forward declaration
void setHistoDetails(TH1 *h, Color_t color, Style_t Mstyle, Size_t size = 0.9f, Width_t width=2.0f);
void SaveCanvas(TCanvas *c, TString name = "tmp");

Double_t ptmin , ptmax;

void DjetEfficiency(Bool_t isPrompt = 1, TString effFile = "../outMC/AnalysisResults_fast_D0MCHijing_SMQcorr2.root", TString outDir = "SQMCorrcuts",
Double_t jetptmin = 2, Double_t jetptmax = 50, Bool_t recoPt = 0, Bool_t postfix = 0, TString listName = "", Bool_t isprefix = 0, Int_t zbin = 0 )
{


    if (fObservable == Observable::kXsection){
      ptmin = fptbinsDA[0];
      ptmax = fptbinsDA[fptbinsDN];
    }
    else if (fObservable == Observable::kFragmentation){
        ptmin = fzptbinsDA[zbin-1][0];
        ptmax = fzptbinsDA[zbin-1][fzptbinsDN[zbin-1]];
        jetptmin=fzptJetMeasA[zbin-1];
        jetptmax=fzptJetMeasA[zbin];
    }

    gStyle->SetOptStat(0000); //Mean and RMS shown
    gSystem->Exec(Form("mkdir %s",outDir.Data()));
    //jetptmin = 0; jetptmax = 200;

    // get analysis output file
    TFile *File = new TFile(effFile,"read");
    TDirectoryFile* dir=dynamic_cast<TDirectoryFile*>(File->Get("DmesonsForJetCorrelations"));

    TString histName;
    if(!isprefix){
        if(fDmesonSpecie) histName = "histosDStarMBN";
        else histName = "histosD0MBN";}
    else{
        if(fDmesonSpecie) histName = "histosDStarMBN";
        else histName = "histosD0";}

    TH1D *hMCpt = nullptr;
    TH1D *hMCpt_reco = nullptr;

    TH1D *hMC[NDMC];
    TH1D * hreco[NDMC];
    TList *histList[NDMC];
    THnSparseF *sparseMC[NDMC];
    THnSparseF *sparsereco[NDMC];

    for(int i=0; i<NDMC; i++){

        if(!isprefix){
            if(postfix) { histList[i] =  dynamic_cast<TList*>(dir->Get(Form("%s%d%sMCrec",histName.Data(),i,listName.Data()))); }
            else {
                if(isPrompt) histList[i] =  dynamic_cast<TList*>(dir->Get(Form("%s%dMCrec",histName.Data(),i)));
                else histList[i] =  dynamic_cast<TList*>(dir->Get(Form("%s%dFDMCrec",histName.Data(),i)));
            }
        }
        else{
            if(postfix) {
                if(isPrompt){ histList[i] =  dynamic_cast<TList*>(dir->Get(Form("%s%sMBN%dMCrec",histName.Data(),listName.Data(),i))); }
                else{ histList[i] =  dynamic_cast<TList*>(dir->Get(Form("%s%sMBN%dFDMCrec",histName.Data(),listName.Data(),i))); }
            }
            else { cout<<"-----postfix has to be true if prefix is true!! check again----------------"<<endl; return;	}
        }

        THnSparseF* sMC = dynamic_cast<THnSparseF*>(histList[i]->FindObject("ResponseMatrix"));
        sparseMC[i] = dynamic_cast<THnSparseF*>(sMC->Clone(Form("sparseMC_%d",i)));
        if(fDmesonSpecie){ sparseMC[i]->GetAxis(5)->SetRangeUser(jetptmin,jetptmax); // Dstar tmp
            std::cout<<"if fDmesonSpecie"<<std::endl;
        }
            else { 	sparseMC[i]->GetAxis(6)->SetRangeUser(jetptmin,jetptmax); // jet pT gen
            sparseMC[i]->GetAxis(9)->SetRangeUser(-(0.9-fRpar),0.9-fRpar); // MC jet eta
            sparseMC[i]->GetAxis(7)->SetRangeUser(2,36);
        //    sparseMC[i]->GetAxis(8)->SetRangeUser(-(0.9-fRpar),0.9-fRpar); // MC D y
            std::cout<<"1 else fDmesonSpecie"<<std::endl;
            std::cout<<"jet pT: "<<jetptmin<<" "<<jetptmax<<" eta "<<0.9-fRpar<<std::endl;
        }
            if(fDmesonSpecie) hMC[i] = dynamic_cast<TH1D*>(sparseMC[i]->Projection(6)); // Dstar tmp
            else hMC[i] = dynamic_cast<TH1D*>(sparseMC[i]->Projection(7)); // Dpt gen
        hMC[i]->SetName(Form("hMC_%d",i));

        THnSparseF* sreco = dynamic_cast<THnSparseF*>(histList[i]->FindObject("ResponseMatrix"));
                sparsereco[i] = dynamic_cast<THnSparseF*>(sreco->Clone(Form("sparsereco_%d",i)));
        if(recoPt) {
                sparsereco[i]->GetAxis(1)->SetRangeUser(ptmin,ptmax); // jet pT reco
            }
            else {
                if(fDmesonSpecie)sparsereco[i]->GetAxis(5)->SetRangeUser(jetptmin,jetptmax); // Dstar tmp
                else{ sparsereco[i]->GetAxis(6)->SetRangeUser(jetptmin,jetptmax); // jet pT gen
                    std::cout<<"2 else fDmesonSpecie"<<std::endl;
                    std::cout<<"jet pT: "<<jetptmin<<" "<<jetptmax<<std::endl;
                }
            sparsereco[i]->GetAxis(1)->SetRangeUser(5,50); // jet pT reco
            std::cout<<"jet reco pT: "<<"0 140"<<std::endl;
            }
        if(!fDmesonSpecie){ sparsereco[i]->GetAxis(9)->SetRangeUser(-(0.9-fRpar),0.9-fRpar); // reco jet eta //4reco 9 mc
            sparsereco[i]->GetAxis(4)->SetRangeUser(-(0.9-fRpar),0.9-fRpar);
            sparsereco[i]->GetAxis(2)->SetRangeUser(2,36);
            sparsereco[i]->GetAxis(7)->SetRangeUser(2,36);
        //                    sparsereco[i]->GetAxis(8)->SetRangeUser(-(0.9-fRpar),0.9-fRpar); // MC D y
            std::cout<<"3 else fDmesonSpecie"<<std::endl;
            std::cout<<" eta "<<0.9-fRpar<<std::endl;
        }

            if(fDmesonSpecie)hreco[i] = dynamic_cast<TH1D*>(sparsereco[i]->Projection(6)); // Dstar tmp
            else{ hreco[i] = dynamic_cast<TH1D*>(sparsereco[i]->Projection(7)); // Dpt gen
             std::cout<<"projecting 7"<<std::endl;
            }

        hreco[i]->SetName(Form("hreco_%d",i));
        if (!i){
            hMCpt = dynamic_cast<TH1D*>(hMC[0]->Clone("hMCpt"));
            hMCpt_reco = dynamic_cast<TH1D*>(hreco[0]->Clone("hMCpt_reco"));
        }
        else {
            hMCpt->Add(hMC[i]);
            hMCpt_reco->Add(hreco[i]);
        }
    }

    hMCpt->Sumw2();
    hMCpt_reco->Sumw2();
    hMCpt->SetLineColor(4);
    hMCpt->SetMarkerColor(4);
    hMCpt->SetMarkerStyle(20);

    hMCpt_reco->SetLineColor(2);
    hMCpt_reco->SetMarkerColor(2);
    hMCpt_reco->SetMarkerStyle(20);

    TH1D * hEff = dynamic_cast<TH1D*>(hMCpt_reco->Clone("hEff"));
    hEff -> Divide(hMCpt_reco,hMCpt,1,1,"b");
    //hEff->GetXaxis()->SetRangeUser(ptmin,ptmax);

    hMCpt->SetMinimum(10);
    TCanvas *cPt = new TCanvas();
    cPt->cd();
    hMCpt->Draw();
    hMCpt_reco->Draw("same");

    if(isPrompt) setHistoDetails(hEff,2,20,1.2f);
    else setHistoDetails(hEff,4,25,1.2f);

    TCanvas *cEff = new TCanvas();
    hEff->Draw("ep");
    //for (Int_t i = 0 ; i< fptbinsDN;i++) std::cout<<fptbinsDA[i]<<std::endl;

    TH1D *hpt_mc_reb = nullptr;
    TH1D *hpt_reco_reb = nullptr;

    if (fObservable == Observable::kXsection){
        hpt_mc_reb = dynamic_cast<TH1D*>(hMCpt->Rebin(fptbinsDN,"hpt_mc_reb",fptbinsDA));
        hpt_reco_reb = dynamic_cast<TH1D*>(hMCpt_reco->Rebin(fptbinsDN,"hpt_reco_reb",fptbinsDA));
    }
    else if (fObservable == Observable::kFragmentation){
        hpt_mc_reb = dynamic_cast<TH1D*>(hMCpt->Rebin(fzptbinsDN[zbin-1],"hpt_mc_reb",fzptbinsDA[zbin-1]));
        hpt_reco_reb = dynamic_cast<TH1D*>(hMCpt_reco->Rebin(fzptbinsDN[zbin-1],"hpt_reco_reb",fzptbinsDA[zbin-1]));
    }

    TH1D * hEff_reb = dynamic_cast<TH1D*>(hpt_reco_reb->Clone("hEff_reb"));
    hEff_reb -> Divide(hpt_reco_reb,hpt_mc_reb,1,1,"b");
    hEff_reb->GetXaxis()->SetRangeUser(ptmin,ptmax);
    hEff_reb->SetTitle(Form("|#eta_{jet}|<%.1f",0.9-fRpar));
    hEff_reb->GetXaxis()->SetTitle(Form("p_{T,%s} (GeV/$it{c})",fDmesonS.Data()));
    hEff_reb->GetYaxis()->SetTitle("Acc #times Eff");

    if(isPrompt) setHistoDetails(hEff_reb,2,20,1.2f);
    else setHistoDetails(hEff_reb,4,25,1.2f);

    TCanvas *cEffReb = new TCanvas();
    hEff_reb->Draw("ep");

    TString out = Form("%s/DjetEffRaw_%s_jetpt%d_%d", outDir.Data(), isPrompt ? "prompt" : "nonPrompt", static_cast<int>(jetptmin),static_cast<int>(jetptmax) );
    SaveCanvas(cEff,out);

    out = Form("%s/DjetEff_%s_jetpt%d_%d", outDir.Data(), isPrompt ? "prompt" : "nonPrompt", static_cast<int>(jetptmin),static_cast<int>(jetptmax) );
    SaveCanvas(cEffReb,out);

    TFile *outFile = new TFile(Form("%s/DjetEff_%s_jetpt%d_%d.root", outDir.Data(), isPrompt ? "prompt" : "nonPrompt", static_cast<int>(jetptmin), static_cast<int>(jetptmax) ),"RECREATE");
    hEff->Write();
    hEff_reb->Write();
    hMCpt->Write();
    hMCpt_reco->Write();
    hpt_mc_reb->Write();
    hpt_reco_reb->Write();
    outFile->Close();

return;

}

void setHistoDetails(TH1 *h, Color_t color, Style_t Mstyle, Size_t size, Width_t width){

    h->SetMarkerStyle(Mstyle);
    h->SetMarkerColor(color);
    h->SetMarkerSize(size);
    h->SetLineColor(color);
    h->SetLineWidth(width);
    h->SetTitle("");
    h->GetXaxis()->SetTitle("p_{T,D^{0}}(GeV/c)");

    return;

}

void SaveCanvas(TCanvas *c, TString name){

    c->SaveAs(Form("%s.png",name.Data()));
    c->SaveAs(Form("%s.pdf",name.Data()));
    c->SaveAs(Form("%s.svg",name.Data()));

}
