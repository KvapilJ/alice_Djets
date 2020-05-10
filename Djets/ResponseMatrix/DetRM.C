//-----------------------------------------------------------------------
//  Author B.Trzeciak
//  Utrecht University
//  barbara.antonina.trzeciak@cern.ch
//-----------------------------------------------------------------------

#include "config.h"

void DetRM(bool isPrompt = 1, TString datafile = "../outMC/AnalysisResults_fast_D0MCPythia_SMQcorr2.root", TString outDir = "plots",
bool postfix = 0, TString listName = "FD", bool isprefix=0, Int_t zBin=0)
{

    gStyle->SetOptStat(0000); //Mean and RMS shown
    gStyle->SetPadRightMargin(0.1);
    gSystem->Exec(Form("mkdir %s",outDir.Data()));
    gSystem->Exec(Form("mkdir %s/plots%s",outDir.Data(),(zBin!=0)?Form("%d",zBin):""));

    TFile *File = new TFile(datafile,"read");
    TDirectoryFile* dir=(TDirectoryFile*)File->Get("DmesonsForJetCorrelations");
        TString histName;
        if(!isprefix){
                if(fDmesonSpecie) histName = "histosDStarMBN";
                else histName = "histosD0MBN";}
        else{
                if(fDmesonSpecie) histName = "histosDStarMBN";
                else histName = "histosD0";}


    float jetmin = 0, jetmax = 60;
    float Dptmin = fptbinsDA[0], Dptmax = fptbinsDA[fptbinsDN];
    if(fObservable == kFragmentation){
        Dptmin=fzptbinsDA[zBin-1][0];
        Dptmax=fzptbinsDA[zBin-1][fzptbinsDN[zBin-1]];
        jetmin = fzptJetMeasA[zBin-1];
        jetmax = fzptJetMeasA[zBin];
    }

    TPaveText *pv2 = new TPaveText(0.15,0.8,0.25,0.9,"brNDC");
    pv2->SetFillStyle(0);
    pv2->SetBorderSize(0);
    pv2->AddText(Form("R=0.%d",Rpar));

    TH1F *hMCpt;
      TH1F *hMCpt_reco;
    TH2F *hPtJet[NDMC];
    TH1F *hPtG[NDMC];
    TH1F *hPtR[NDMC];

    TH3F* sparseMCeff[NDMC];

      TList *histList[NDMC];
      THnSparseF *sparseMC[NDMC];


    TH2F *hPtJet2d;
    TH1F *hPtJetGen;
    TH1F *hPtJetRec;

   // TFile *Feff = new TFile ("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts/Default_AnalysisResults_Run2w18b.root/efficiency/DjetEff_nonPrompt_jetpt5.00_50.00.root");
  //  TH1D *eff = dynamic_cast<TH1D*>(Feff->Get("hEff_reb"));
    TH1D *eff =nullptr;


        for(int i=0; i<NDMC; i++){
           if(!isprefix){
                if(postfix) {
            histList[i] =  (TList*)dir->Get(Form("%s%d%sMCrec",histName.Data(),i,listName.Data())); }

                else {
                         if(isPrompt) histList[i] =  (TList*)dir->Get(Form("%s%dMCrec",histName.Data(),i));
                         else histList[i] =  (TList*)dir->Get(Form("%s%dFDMCrec",histName.Data(),i));
                }
           }
           else{
                if(postfix) {
                        if(isPrompt){ histList[i] =  (TList*)dir->Get(Form("%s%sMBN%dMCrec",histName.Data(),listName.Data(),i)); }
                        else{    histList[i] =  (TList*)dir->Get(Form("%s%sMBN%dFDMCrec",histName.Data(),listName.Data(),i)); }
                }
                else { std::cout<<"-----postfix has to be true if prefix is true!! check again----------------"<<std::endl; return;       }

           }


        sparseMC[i] = (THnSparseF*)histList[i]->FindObject("ResponseMatrix");

        sparseMC[i]->GetAxis(2)->SetRangeUser(Dptmin,Dptmax);
        sparseMC[i]->GetAxis(1)->SetRangeUser(jetmin,jetmax);

        if(fDmesonSpecie) sparseMC[i]->GetAxis(6)->SetRangeUser(Dptmin,Dptmax);
        else sparseMC[i]->GetAxis(7)->SetRangeUser(Dptmin,Dptmax);

        if(fDmesonSpecie) sparseMC[i]->GetAxis(5)->SetRangeUser(jetmin,jetmax); // Dstar tmp
        else sparseMC[i]->GetAxis(6)->SetRangeUser(jetmin,jetmax);

    if(!fDmesonSpecie) {
        sparseMC[i]->GetAxis(4)->SetRangeUser(-(0.9-fRpar),0.9-fRpar);
        sparseMC[i]->GetAxis(9)->SetRangeUser(-(0.9-fRpar),0.9-fRpar);
    }

        if(fObservable == kXsection){
            if(true){
            if(fDmesonSpecie) hPtJet[i] = (TH2F*)sparseMC[i]->Projection(5,1,"E"); //Dstar tmp
            else hPtJet[i] = (TH2F*)sparseMC[i]->Projection(6,1,"E");
            if(fDmesonSpecie) hPtG[i] = (TH1F*)sparseMC[i]->Projection(5); //Dstar tmp
            else   hPtG[i] = (TH1F*)sparseMC[i]->Projection(6);
            hPtR[i] = (TH1F*)sparseMC[i]->Projection(1);
            }else{
                sparseMCeff[i] = (TH3F*)sparseMC[i]->Projection(6,1,2,"E");
                for(Int_t jg = 1;jg <= sparseMCeff[i]->GetNbinsX();jg++){
                    for(Int_t jr = 1;jr <= sparseMCeff[i]->GetNbinsY();jr++){
                        for(Int_t dr = 1;dr <= sparseMCeff[i]->GetNbinsZ();dr++){
                           /* std::cout<<jg<<" "<<jr<<" "<<dr<<" jetG: "<<((TAxis*)sparseMCeff->GetXaxis())->GetBinCenter(jg)
                                    <<" jetR: "<<((TAxis*)sparseMCeff->GetYaxis())->GetBinCenter(jr)<<
                                      " DR: "<<((TAxis*)sparseMCeff->GetZaxis())->GetBinCenter(dr)
                                   <<" eff: "<<
                                      eff->GetBinContent(eff->FindBin(((TAxis*)sparseMCeff->GetZaxis())->GetBinCenter(dr)))<<std::endl;*/

                            sparseMCeff[i]->SetBinContent(jg,jr,dr,sparseMCeff[i]->GetBinContent(jg,jr,dr)/eff->GetBinContent(eff->FindBin(((TAxis*)sparseMCeff[i]->GetZaxis())->GetBinCenter(dr))));

                        }
                    }
                }

                hPtJet[i] = (TH2F*)sparseMCeff[i]->Project3D("xy");
                hPtG[i] = (TH1F*)sparseMCeff[i]->ProjectionX();
                hPtR[i] = (TH1F*)sparseMCeff[i]->ProjectionY();








            }

        }
        if(fObservable == kFragmentation){
            if(fDmesonSpecie) hPtJet[i] = (TH2F*)sparseMC[i]->Projection(4,0,"E"); //Dstar tmp
            else hPtJet[i] = (TH2F*)sparseMC[i]->Projection(5,0,"E");
            if(fDmesonSpecie) hPtG[i] = (TH1F*)sparseMC[i]->Projection(4); //Dstar tmp
            else   hPtG[i] = (TH1F*)sparseMC[i]->Projection(5);
            hPtR[i] = (TH1F*)sparseMC[i]->Projection(0);
        }

        hPtJet[i]->Sumw2();


        hPtG[i]->Sumw2();
        hPtR[i]->Sumw2();

        if(fObservable == kXsection){
            hPtJet[i]->SetName(Form("hPtJet_%d",i));
            hPtG[i]->SetName(Form("hPtG_%d",i));
            hPtR[i]->SetName(Form("hPtR_%d",i));
        }
        if(fObservable == kFragmentation){
            hPtJet[i]->SetName(Form("hZJet_%d",i));
            hPtG[i]->SetName(Form("hZG_%d",i));
            hPtR[i]->SetName(Form("hZR_%d",i));
        }

            if (!i){
                if(fObservable == kXsection){
                    hPtJet2d = (TH2F*)hPtJet[0]->Clone("hPtJet2d");
                    hPtJetGen = (TH1F*)hPtG[0]->Clone("hPtJetGen");
                    hPtJetRec = (TH1F*)hPtR[0]->Clone("hPtJetRec");
                }
                if(fObservable == kFragmentation){
                    hPtJet2d = (TH2F*)hPtJet[0]->Clone("hZJet2d");
                    hPtJetGen = (TH1F*)hPtG[0]->Clone("hZJetGen");
                    hPtJetRec = (TH1F*)hPtR[0]->Clone("hZJetRec");
                }

        }
        else {
            hPtJet2d->Add(hPtJet[i]);
                hPtJetGen->Add(hPtG[i]);
                hPtJetRec->Add(hPtR[i]);
        }

    }


    hPtJet2d->SetTitle("");
    if(fObservable == kXsection){
        hPtJet2d->SetName("hPtJet2d");
        hPtJet2d->GetXaxis()->SetTitle("p_{T,ch jet}^{rec.} (GeV/#it{c})");
        hPtJet2d->GetYaxis()->SetTitle("p_{T,ch jet}^{gen.} (GeV/#it{c})");
        hPtJetGen->SetName("hPtJetGen");
        hPtJetRec->SetName("hPtJetRec");
    }
    if(fObservable == kFragmentation){
        hPtJet2d->SetName("hZJet2d");
        hPtJet2d->GetXaxis()->SetTitle("z^{rec.}");
        hPtJet2d->GetYaxis()->SetTitle("z^{gen.}");
        hPtJet2d->GetXaxis()->SetRangeUser(fzbinsJetMeasA[zBin-1][0],fzbinsJetMeasA[zBin-1][fzbinsJetMeasN[zBin-1]]);
        if(isPrompt)hPtJet2d->GetYaxis()->SetRangeUser(fzbinsJetTrueAPrompt[zBin-1][0],fzbinsJetTrueAPrompt[zBin-1][fzbinsJetTrueN[zBin-1]]);
        else hPtJet2d->GetYaxis()->SetRangeUser(fzbinsJetMeasA[zBin-1][0],fzbinsJetMeasA[zBin-1][fzbinsJetMeasN[zBin-1]]);

        hPtJetGen->SetName("hZJetGen");
        hPtJetRec->SetName("hZJetRec");
        if(isPrompt){
            Int_t nBin = hPtJetGen->FindBin(1.0);
            hPtJetGen->SetBinContent(nBin-1,hPtJetGen->GetBinContent(nBin-1)+hPtJetGen->GetBinContent(nBin));
            hPtJetGen->SetBinContent(nBin,0);
            Int_t nBinY = hPtJet2d->GetYaxis()->FindBin(1.0);
            Int_t nBinX = hPtJet2d->GetNbinsX();
            for(Int_t bin = 1; bin<nBinX;bin++){
                hPtJet2d->SetBinContent(bin,nBinY-1,hPtJet2d->GetBinContent(bin,nBinY-1)+hPtJet2d->GetBinContent(bin,nBinY));
                hPtJet2d->SetBinContent(bin,nBinY,0);
            }
        }
    }

    hPtJetGen->SetLineColor(kBlue+2);
    hPtJetRec->SetLineColor(kRed+2);


    TCanvas *cjetPt = new TCanvas("cjetPt","cjetPt",800,600);
    cjetPt->SetLogy();
    hPtJetGen->Draw();
    hPtJetRec->Draw("same");
    //cjetPt->SaveAs(Form("%s/pTdist_Dpt%d_%d.png",outDir, (int)Dptmin, (int)Dptmax));

    TCanvas *cjetPt2d = new TCanvas("cjetPt2d","cjetPt2d",800,600);
    cjetPt2d->SetLogz();
    hPtJet2d->Draw("colz");
    pv2->Draw("same");


    cjetPt2d->SaveAs(Form("%s/plots%s/DetMatrix_%s_Dpt%d_%d.png",outDir.Data(),(zBin!=0)?Form("%d",zBin):"", isPrompt ? "prompt" : "nonPrompt", (int)Dptmin, (int)Dptmax));

    //TFile *ofile = new TFile(Form("%s/DetMatrix_Dpt%d_%d.root",outDir, (int)Dptmin, (int)Dptmax),"RECREATE");
    TFile *ofile = new TFile(Form("%s/DetMatrix_%s%s.root",outDir.Data(), isPrompt ? "prompt" : "nonPrompt", (zBin!=0)?Form("%d",zBin):"" ),"RECREATE");

    hPtJetGen->Write();
    hPtJetRec->Write();
    hPtJet2d->Write();
    ofile->Close();

   return;

/*
    TH1D *proj[nJetBins];
    for(int i=0; i<nJetBins; i++){
            proj[i] = (TH1D*)hPtJet2d->ProjectionX(Form("proj_%d",i),hPtJet2d->GetYaxis()->FindBin(ptJetbins[i]), hPtJet2d->GetYaxis()->FindBin(ptJetbins[i+1]) -1);
            proj[i]->Scale(1./proj[i]->Integral());
            //proj[i-1]->SetMarkerStyle(20);
            proj[i]->SetMarkerColor(2);
            proj[i]->SetLineColor(2);
    }
*/

}
