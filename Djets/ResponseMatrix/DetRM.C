//-----------------------------------------------------------------------
//  Author B.Trzeciak
//  Utrecht University
//  barbara.antonina.trzeciak@cern.ch
//-----------------------------------------------------------------------

#include "config.h"

void SparseToTree(TString MCfile, Bool_t isPostfix, TString postfix);

void DetRM(bool isPrompt = 1, TString datafile = "../outMC/AnalysisResults_fast_D0MCPythia_SMQcorr2.root", TString outDir = "plots",
bool postfix = 0, TString listName = "FD", bool isprefix=0, Int_t zBin=0, Bool_t effScale = false, TString effFile = "", TString BFDsub = "")
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

  //  Int_t NDMC = 0;
  //  if(isPrompt) NDMC = 2;
  //  else NDMC = 1;
  //  std::cout<<"FD sim: "<<NDMC<<std::endl;

    TH1F *hMCpt;
      TH1F *hMCpt_reco;
    TH2F *hPtJet[NDMC];
    TH1F *hPtG[NDMC];
    TH1F *hPtR[NDMC];

//    TH2F *hPtJetEffClosure[NDMC];
//    TH1F *hPtGClosure[NDMC];
//    TH1F *hPtREffClosure[NDMC];
//    TH1F *hPtGClosureData[NDMC];
//    TH1F *hPtREffClosureData[NDMC];

    TH3F* sparseMCeff[NDMC];
  //  TH3F* sparseMCeffClosure6[NDMC];
  //  TH3F* sparseMCeffClosure4[NDMC];
  //  TH3F* sparseMCClosure4[NDMC];
    TRandom3 *random = new TRandom3(0);

      TList *histList[NDMC];
      THnSparseF *sparseMC[NDMC];
 //     THnSparseF *sparseMCopen[NDMC];


    TH2F *hPtJet2d;
    TH1F *hPtJetGen;
    TH1F *hPtJetRec;

    TH2D *hPtJet2dClosure;
    TH1D *hPtJetGenClosure;
    TH1D *hPtJetRecClosure;
    TH1D *hPtJetGenClosureData;
    TH1D *hPtJetRecClosureData;

    TH1F *hPtJetGenClosureDataReb;
std::cout<<"herea"<<std::endl;
    TFile *Feff = nullptr;
    TH1D *eff = nullptr;
    if(effScale && fObservable == kXsection){
        Feff = new TFile (effFile,"read");
        eff = dynamic_cast<TH1D*>(Feff->Get("hEff_reb"));
    }

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


        //sparseMC[i] = (THnSparseF*)histList[i]->FindObject("ResponseMatrix");

        THnSparseF* sMC = (THnSparseF*)histList[i]->FindObject("ResponseMatrix");
        sparseMC[i] = dynamic_cast<THnSparseF*>(sMC->Clone(Form("sparseMC_%d",i)));
  //      sparseMCopen[i] = dynamic_cast<THnSparseF*>(sMC->Clone(Form("sparseMCopen_%d",i)));

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

  //  sparseMCopen[i]->GetAxis(7)->SetRangeUser(Dptmin,Dptmax);
  //  sparseMCopen[i]->GetAxis(6)->SetRangeUser(jetmin,jetmax);
  //  sparseMCopen[i]->GetAxis(9)->SetRangeUser(-(0.9-fRpar),0.9-fRpar);

std::cout<<"hereb"<<std::endl;
        if(fObservable == kXsection){
            if(effScale == false){
                if(fDmesonSpecie) hPtJet[i] = (TH2F*)sparseMC[i]->Projection(5,1,"E"); //Dstar tmp
                else hPtJet[i] = (TH2F*)sparseMC[i]->Projection(6,1,"E");
                if(fDmesonSpecie) hPtG[i] = (TH1F*)sparseMC[i]->Projection(5); //Dstar tmp
                else   hPtG[i] = (TH1F*)sparseMC[i]->Projection(6);
                hPtR[i] = (TH1F*)sparseMC[i]->Projection(1);
            }else{
                sparseMCeff[i] = (TH3F*)sparseMC[i]->Projection(6,1,2,"E");
             //   sparseMCeffClosure6[i] = (TH3F*)sparseMC[i]->Projection(6,1,2,"E");
             //   sparseMCeffClosure4[i] = (TH3F*)sparseMC[i]->Projection(6,1,2,"E");
              //  sparseMCClosure4[i] = (TH3F*)sparseMCopen[i]->Projection(6,1,2,"E");

                for(Int_t jg = 1;jg <= sparseMCeff[i]->GetNbinsX();jg++){
                    for(Int_t jr = 1;jr <= sparseMCeff[i]->GetNbinsY();jr++){
                        for(Int_t dr = 1;dr <= sparseMCeff[i]->GetNbinsZ();dr++){
                           /* std::cout<<jg<<" "<<jr<<" "<<dr<<" jetG: "<<((TAxis*)sparseMCeff->GetXaxis())->GetBinCenter(jg)
                                    <<" jetR: "<<((TAxis*)sparseMCeff->GetYaxis())->GetBinCenter(jr)<<
                                      " DR: "<<((TAxis*)sparseMCeff->GetZaxis())->GetBinCenter(dr)
                                   <<" eff: "<<
                                      eff->GetBinContent(eff->FindBin(((TAxis*)sparseMCeff->GetZaxis())->GetBinCenter(dr)))<<std::endl;*/

                            sparseMCeff[i]->SetBinContent(jg,jr,dr,sparseMCeff[i]->GetBinContent(jg,jr,dr)/eff->GetBinContent(eff->FindBin(((TAxis*)sparseMCeff[i]->GetZaxis())->GetBinCenter(dr))));

                      //      sparseMCeffClosure6[i]->SetBinContent(jg,jr,dr,sparseMCeffClosure6[i]->GetBinContent(jg,jr,dr)/eff->GetBinContent(eff->FindBin(((TAxis*)sparseMCeffClosure6[i]->GetZaxis())->GetBinCenter(dr))));

                      //      sparseMCeffClosure4[i]->SetBinContent(jg,jr,dr,sparseMCeffClosure4[i]->GetBinContent(jg,jr,dr)/eff->GetBinContent(eff->FindBin(((TAxis*)sparseMCeffClosure4[i]->GetZaxis())->GetBinCenter(dr))));

                      //      sparseMCClosure4[i]->SetBinContent(jg,jr,dr,sparseMCClosure4[i]->GetBinContent(jg,jr,dr));

                        }
                    }
                }


              //  sparseMCeffClosure6[i]->Sumw2();
              //  sparseMCeffClosure6[i]->Scale(0.6);

              //  sparseMCeffClosure4[i]->Sumw2();
              //  sparseMCeffClosure4[i]->Scale(0.4);
              //  sparseMCClosure4[i]->Sumw2();
              //  sparseMCClosure4[i]->Scale(0.4);

               // std::cout<<sparseMCeff[i]->GetBinContent(4,4,4)<<std::endl;
              //  std::cout<<sparseMCeffClosure6[i]->GetBinContent(4,4,4)<<std::endl;

                hPtJet[i] = (TH2F*)sparseMCeff[i]->Project3D("xy"); //x (vertical) versus y (horisontal)
                hPtG[i] = (TH1F*)sparseMCeff[i]->ProjectionX();
                hPtR[i] = (TH1F*)sparseMCeff[i]->ProjectionY();
         //       hPtJetEffClosure[i]= (TH2F*)sparseMCeffClosure6[i]->Project3D("xy2");
          //      hPtGClosure[i]= (TH1F*)sparseMCeffClosure6[i]->ProjectionX("tag1");
         //       hPtREffClosure[i] = (TH1F*)sparseMCeffClosure6[i]->ProjectionY("tag2");

          //      hPtGClosureData[i]= (TH1F*)sparseMCClosure4[i]->ProjectionX("tag3");
          //      hPtREffClosureData[i] = (TH1F*)sparseMCeffClosure4[i]->ProjectionY("tag4");

          //      std::cout<<hPtJet[i]->GetBinContent(4,4)<<std::endl;
          //      std::cout<<hPtJetEffClosure[i]->GetBinContent(4,4)<<std::endl;
           //     std::cout<<hPtG[i]->GetBinContent(4,4)<<std::endl;
           //     std::cout<<hPtGClosure[i]->GetBinContent(4,4)<<std::endl;
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

std::cout<<"herec"<<std::endl;
        hPtG[i]->Sumw2();
        hPtR[i]->Sumw2();

        if(fObservable == kXsection){
            hPtJet[i]->SetName(Form("hPtJet_%d",i));
            hPtG[i]->SetName(Form("hPtG_%d",i));
            hPtR[i]->SetName(Form("hPtR_%d",i));
        //    hPtJetEffClosure[i]->SetName(Form("hPtJetClosure_%d",i));
        //    hPtGClosure[i]->SetName(Form("hPtGClosure_%d",i));
        //    hPtREffClosure[i]->SetName(Form("hPtRClosure_%d",i));
        //    hPtGClosureData[i]->SetName(Form("hPtGClosureData_%d",i));
        //    hPtREffClosureData[i]->SetName(Form("hPtRClosureData_%d",i));
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
        //            hPtJet2dClosure = (TH2F*)hPtJetEffClosure[0]->Clone("hPtJet2d2");
        //            hPtJetGenClosure = (TH1F*)hPtGClosure[0]->Clone("hPtJetGen2");
        //            hPtJetRecClosure = (TH1F*)hPtREffClosure[0]->Clone("hPtJetRec2");
        //            hPtJetGenClosureData = (TH1F*)hPtGClosureData[0]->Clone("hPtJetGen2D");
        //            hPtJetRecClosureData = (TH1F*)hPtREffClosureData[0]->Clone("hPtJetRec2D");
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
         //       hPtJet2dClosure->Add(hPtJetEffClosure[i]);
         //           hPtJetGenClosure->Add(hPtGClosure[i]);
          //          hPtJetRecClosure->Add(hPtREffClosure[i]);
           //         hPtJetGenClosureData->Add(hPtGClosureData[i]);
            //        hPtJetRecClosureData->Add(hPtREffClosureData[i]);
        }

    }
         if(fObservable == kXsection){
        hPtJetGenClosureData = new TH1D("hPtJetGen2D","hPtJetGen2D",60,jetmin,jetmax);
        hPtJetRecClosureData = new TH1D("hPtJetRec2D","hPtJetRec2D",60,jetmin,jetmax);
        hPtJet2dClosure = new TH2D("hPtJet2d2","hPtJet2d2",60,jetmin,jetmax,60,jetmin,jetmax);
         }
 if(fObservable == kXsection){
        //get TTree...if it does not exist, create it
        TFile *fTreeSparse = nullptr;
        Double_t jmatch[13];
        Double_t bincontent;
        TTree *tree_ = dynamic_cast<TTree*>(File->Get("ResponseMatrixSum_tree"));
        if(!tree_){
            TString tmp(datafile);
            TString treepost = "";
            treepost += "TTree";
            if(postfix) treepost+=listName;
            treepost += ".root";
            tmp.Remove(datafile.Length()-5,5).Append(treepost);
            std::cout<<"MC file do not contain TTree, trying to open TTree sparse in default path!"<<std::endl;
            std::cout<<"opening: "<<tmp<<std::endl;
            fTreeSparse = new TFile(tmp,"READ");
            if(!(fTreeSparse->IsOpen())){
                std::cout<<"TTree file not found, converting MC THnSparse to TTree now!"<<std::endl;
                SparseToTree(datafile, postfix,listName);
                std::cout<<"reading TTree: "<<tmp<<std::endl;
                fTreeSparse = new TFile(tmp,"READ");
                tree_ = dynamic_cast<TTree*>(fTreeSparse->Get("ResponseMatrixSum_tree"));
            }
            else
                tree_ = dynamic_cast<TTree*>(fTreeSparse->Get("ResponseMatrixSum_tree"));
        }

        tree_->SetBranchAddress("coord",&jmatch);
        tree_->SetBranchAddress("bincontent",&bincontent);

    //    TRandom3 *random = new TRandom3(0);

        //get number of jets in data
        Double_t datajets = 0;
        if(isPrompt && fObservable == kXsection){
            TFile *f  = TFile::Open(BFDsub);
            if (!f) { Error("LoadRawSpectrum","Raw spectrum file %s not found.",BFDsub.Data());	return; }
            TH1D *spectrum = dynamic_cast<TH1D*>(f->Get("hData_binned_sub"));
            datajets = spectrum->Integral();
        }

    //get number of eff. scaled jets in MC
        Double_t ttreejets = 0;
        for(int i=0; i<tree_->GetEntries(); i++){
          tree_->GetEntry(i);
          if(jetmin <= jmatch[6] && jmatch[6] <= jetmax){ //jet mc
            if(Dptmin <= jmatch[7] && jmatch[7] <= Dptmax){ //D mc
            if(-(0.9-fRpar) <= jmatch[9] && jmatch[9] <= 0.9-fRpar){ //eta mc
            if(jetmin <= jmatch[1] && jmatch[1] <= jetmax){ //jet det
            if(Dptmin <= jmatch[2] && jmatch[2] <= Dptmax){ //D det
            if(-(0.9-fRpar) <= jmatch[4] && jmatch[4] <= 0.9-fRpar){ //eta det
                ttreejets +=(bincontent/eff->GetBinContent(eff->FindBin(jmatch[2])));
            }
            }
            }
            }
            }
          }

        }
        //get the scaling for prompt, 0.1 for non-prompt is used
        Double_t RMscaling = 0.8;
        //if(isPrompt)RMscaling = 1-(datajets/ttreejets);
        std::cout<<"Data jets: "<<datajets<<", MC jets: "<<ttreejets<<", MC division data:RM "<<RMscaling<<std::endl;


    //loop over tree
        for(int i=0; i<tree_->GetEntries(); i++){
          tree_->GetEntry(i);
          Double_t RMw = 0;
          Double_t SPw = 0;
          for(Int_t iw = 0; iw < bincontent; iw++){
              if(random->Uniform(1) <= RMscaling) RMw++;
              else SPw++;
          }
          if(RMw+SPw != bincontent) std::cout<<"WARNING CLOSURE WEIGHT VIOLATION!!! "<<RMw+SPw<<"!="<<bincontent<<std::endl;

          if(jetmin <= jmatch[6] && jmatch[6] <= jetmax){ //jet mc
                if(Dptmin <= jmatch[7] && jmatch[7] <= Dptmax){ //D mc
                    if(-(0.9-fRpar) <= jmatch[9] && jmatch[9] <= 0.9-fRpar){ //eta mc
                        //pure true cuts
                        hPtJetGenClosureData->Fill(jmatch[6],SPw); //true spectra
                        if(jetmin <= jmatch[1] && jmatch[1] <= jetmax){ //jet det
                              if(Dptmin <= jmatch[2] && jmatch[2] <= Dptmax){ //D det
                                  if(-(0.9-fRpar) <= jmatch[4] && jmatch[4] <= 0.9-fRpar){ //eta det
                                      hPtJetRecClosureData->Fill(jmatch[1],SPw/eff->GetBinContent(eff->FindBin(jmatch[2]))); //reco spectra
                                      hPtJet2dClosure->Fill(jmatch[1],jmatch[6],RMw/eff->GetBinContent(eff->FindBin(jmatch[2]))); //RM
                                  }
                              }
                        }

                    }
                }
          }
          //  sparseMCopen[i]->GetAxis(7)->SetRangeUser(Dptmin,Dptmax);
          //  sparseMCopen[i]->GetAxis(6)->SetRangeUser(jetmin,jetmax);
          //  sparseMCopen[i]->GetAxis(9)->SetRangeUser(-(0.9-fRpar),0.9-fRpar);


        }
        hPtJet2dClosure->Sumw2();

        hPtJetGenClosure = (TH1D*)hPtJet2dClosure->ProjectionY("hPtJetGen2");
        hPtJetRecClosure = (TH1D*)hPtJet2dClosure->ProjectionX("hPtJetRec2");
}

    hPtJet2d->SetTitle("");
    if(fObservable == kXsection){
        hPtJet2d->SetName("hPtJet2d");
        hPtJet2d->GetXaxis()->SetTitle("p_{T,ch jet}^{rec.} (GeV/#it{c})");
        hPtJet2d->GetYaxis()->SetTitle("p_{T,ch jet}^{gen.} (GeV/#it{c})");
        hPtJetGen->SetName("hPtJetGen");
        hPtJetRec->SetName("hPtJetRec");
std::cout<<"here"<<std::endl;
        hPtJet2dClosure->SetName("hPtJet2d");
        hPtJet2dClosure->GetXaxis()->SetTitle("p_{T,ch jet}^{rec.} (GeV/#it{c})");
        hPtJet2dClosure->GetYaxis()->SetTitle("p_{T,ch jet}^{gen.} (GeV/#it{c})");
        hPtJetGenClosure->SetName("hPtJetGen");
        hPtJetRecClosure->SetName("hPtJetRec");
        hPtJetGenClosureData->SetName("hPtJetGen");
        hPtJetRecClosureData->SetName("hData_binned_sub");
        hPtJetGenClosure->SetTitle("hPtJetGen");
        hPtJetRecClosure->SetTitle("hPtJetRec");
        hPtJetGenClosureData->SetTitle("hPtJetGen");
        hPtJetRecClosureData->SetTitle("hData_binned_sub");
        std::cout<<"here2"<<std::endl;
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
 if(fObservable == kXsection){
    //hPtJetGenClosureDataReb = dynamic_cast<TH1F*>(hPtJetGenClosureData->Rebin(fptbinsJetTrueN,"mcreb",fptbinsJetTrueA));
    hPtJetGenClosureDataReb = reinterpret_cast<TH1F*>(hPtJetGenClosureData->Rebin(fptbinsJetTrueN,"mcreb",fptbinsJetTrueA));
    hPtJetGenClosureDataReb->Draw();
}

    cjetPt2d->SaveAs(Form("%s/plots%s/DetMatrix_%s_Dpt%d_%d.png",outDir.Data(),(zBin!=0)?Form("%d",zBin):"", isPrompt ? "prompt" : "nonPrompt", (int)Dptmin, (int)Dptmax));

    //TFile *ofile = new TFile(Form("%s/DetMatrix_Dpt%d_%d.root",outDir, (int)Dptmin, (int)Dptmax),"RECREATE");
    TFile *ofile = new TFile(Form("%s/DetMatrix_%s%s.root",outDir.Data(), isPrompt ? "prompt" : "nonPrompt", (zBin!=0)?Form("%d",zBin):"" ),"RECREATE");

    hPtJetGen->Write();
    hPtJetRec->Write();
    hPtJet2d->Write();
    ofile->Close();
 if(fObservable == kXsection){
    TFile *ofileClosure = new TFile(Form("%s/DetMatrixClosure_%s%s.root",outDir.Data(), isPrompt ? "prompt" : "nonPrompt", (zBin!=0)?Form("%d",zBin):"" ),"RECREATE");
    hPtJetGenClosure->Write("hPtJetGen");
    hPtJetRecClosure->Write("hPtJetRec");
    hPtJet2dClosure->Write("hPtJet2d");
    ofileClosure->Close();

    TFile *ofileClosureData = new TFile(Form("%s/DetMatrixClosureData_%s%s.root",outDir.Data(), isPrompt ? "prompt" : "nonPrompt", (zBin!=0)?Form("%d",zBin):"" ),"RECREATE");
    hPtJetGenClosureDataReb->Write("unfoldedSpectrum");
    hPtJetRecClosureData->Write("hData_binned_sub");
    ofileClosureData->Close();
}
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


void SparseToTree(TString MCfile, Bool_t isPostfix, TString postfix)
{
// Read the THnSparse
    TFile *File = new TFile(MCfile,"read");
    TDirectoryFile* dir=(TDirectoryFile*)File->Get("DmesonsForJetCorrelations");
    TString histName;
    if(fDmesonSpecie) histName = "histosDStarMBN";
                    else histName = "histosD0MBN";
    TList *histList[NDMC];
    THnSparseF *sparseMC[NDMC];
    THnSparseF *h = nullptr;

    for(int i=0; i<2; i++){
        histList[i] =  (TList*)dir->Get(Form("%s%d%sMCrec",histName.Data(),i,isPostfix?postfix.Data():""));
        sparseMC[i] = (THnSparseF*)histList[i]->FindObject("ResponseMatrix");
        std::cout<<sparseMC[i]->GetNbins()<<std::endl;
        if(!i)h = dynamic_cast<THnSparseF*>(sparseMC[0]->Clone("ResponseMatrixSum"));
        else h->Add(sparseMC[i]);
    }
    //copy from here: https://root.cern.ch/root/html/tutorials/tree/drawsparse.C.html
    // Creates a TTree and fills it with the coordinates of all
    // filled bins. The tree will have one branch for each dimension,
    // and one for the bin content.
    TString outName(MCfile);
    outName.Remove(outName.Length()-5,5);
    outName+="TTree";
    if(isPostfix)outName+=postfix;
    outName+=".root";
    std::cout<<"Converting THnSparse: "<<MCfile<<std::endl;
    std::cout<<"Into TTree: "<<outName<<std::endl;
   TFile *File2 = new TFile(outName,"RECREATE");
   Int_t dim = h->GetNdimensions();
   TString name(h->GetName()); name += "_tree";
   TString title(h->GetTitle()); title += " tree";

   TTree* tree = new TTree(name, title);
   Double_t* x = new Double_t[dim + 1];
   memset(x, 0, sizeof(Double_t) * (dim + 1));

   TString branchname;
   for (Int_t d = 0; d < dim; ++d) {
      if (branchname.Length())
         branchname += ":";
      TAxis* axis = h->GetAxis(d);
      branchname += axis->GetName();
      branchname += "/D";
   }
   tree->Branch("coord", x, branchname);
   tree->Branch("bincontent", &x[dim], "bincontent/D");

   const int percentPrint = 1;
   int totalWindows = h->GetNbins();
   int percent;
   int step = totalWindows / (100/percentPrint);
   int nextPrint = step;

   Int_t *bins = new Int_t[dim];
   for (Long64_t i = 0; i < h->GetNbins(); ++i) {
      if (i >= nextPrint)
       {
           percent = (100 * i) / totalWindows;
           std::cout << "\r" << std::string(percent/percentPrint , '|') << percent << "%";
           std::cout.flush();
           nextPrint += step;
       }
      x[dim] = h->GetBinContent(i, bins);
      for (Int_t d = 0; d < dim; ++d) {
         x[d] = h->GetAxis(d)->GetBinCenter(bins[d]);
      }

      tree->Fill();

   }
   std::cout<<std::endl<<"tree entries "<< tree->GetEntries()<<std::endl;
   File2->Write(nullptr,TObject::kOverwrite);
   File2->Close();
}

