
#include "config.h"

std::tuple<Double_t, Int_t> GetX2(TH1D *O, TH1D *E);

void UnfToy(){
    TFile *fTreeSparse = new TFile("/mnt/hgfs/vmware/data_R04_190520_open/mc/AnalysisResults_Run2w18bTTree.root","READ");
    if(!fTreeSparse) std::cout<<"MC FILE not found"<<std::endl;
    Double_t jmatch[13];
    Double_t bincontent;
    TTree *tree_ = dynamic_cast<TTree*>(fTreeSparse->Get("ResponseMatrixSum_tree"));
    tree_->SetBranchAddress("coord",&jmatch);
    tree_->SetBranchAddress("bincontent",&bincontent);

    const Int_t n_full = 15;
    const Int_t n_lim1 = 14;
    const Int_t n_lim2 = 13;

    Double_t bin_full[n_full] = {0,1,2,3,4,5,7,9,11,13,15,20,25,30,50};
    Double_t bin_lim1[n_lim1] = {1,2,3,4,5,7,9,11,13,15,20,25,30,50};
    Double_t bin_lim2[n_lim2] = {2,3,4,5,7,9,11,13,15,20,25,30,50};

    TH1D* hMeas_full = new TH1D ("hMeas_full", "hMeas_full",n_full-1,bin_full);
    TH1D* hTrue_full = new TH1D ("hTrue_full", "hTrue_full",n_full-1,bin_full);

    TH1D* hTrue_unmatched = new TH1D ("hTrue_unmatched", "hTrue_unmatched",n_full-1,bin_full);

    TH1D* hMeas_lim = new TH1D ("hMeas_lim", "hMeas_lim",n_lim1-1,bin_lim1);
    TH1D* hTrue_lim = new TH1D ("hTrue_lim", "hTrue_lim",n_lim2-1,bin_lim2);

    TH1D* hMeas_limBoth = new TH1D ("hMeas_limBoth", "hMeas_limBoth",n_lim1-1,bin_lim1);
    TH1D* hTrue_limBoth = new TH1D ("hTrue_limBoth", "hTrue_limBoth",n_lim2-1,bin_lim2);

    RooUnfoldResponse *response_full = new RooUnfoldResponse();
    response_full->Setup(hMeas_full, hTrue_full);
    RooUnfoldResponse *response_limMeas = new RooUnfoldResponse();
    response_limMeas->Setup(hMeas_lim, hTrue_full);
    RooUnfoldResponse *response_limTrue = new RooUnfoldResponse();
    response_limTrue->Setup(hMeas_full, hTrue_lim);
    RooUnfoldResponse *response_limMeasWithMiss = new RooUnfoldResponse();
    response_limMeasWithMiss->Setup(hMeas_lim, hTrue_full);
    RooUnfoldResponse *response_limTrueWithFake = new RooUnfoldResponse();
    response_limTrueWithFake->Setup(hMeas_full, hTrue_lim);
    RooUnfoldResponse *response_limMeasTrueWithMissFake = new RooUnfoldResponse();
    response_limMeasTrueWithMissFake->Setup(hMeas_lim, hTrue_lim);
    RooUnfoldResponse *response_limMeasTrueWithMissFakeEff = new RooUnfoldResponse();
    response_limMeasTrueWithMissFakeEff->Setup(hMeas_lim, hTrue_lim);

    TH1D* hKine_limMeas = new TH1D ("hKine_limMeas", "hKine_limMeas",n_full-1,bin_full);
    TH1D* hKine_limMeas_den = new TH1D ("hKine_limMeas_den", "hKine_limMeas_den",n_full-1,bin_full);

    TH1D* hKine_limTrue = new TH1D ("hKine_limTrue", "hKine_limTrue",n_full-1,bin_full);
    TH1D* hKine_limTrue_den = new TH1D ("hKine_limTrue_den", "hKine_limTrue_den",n_full-1,bin_full);

    TH1D* hEff = new TH1D ("hEff", "hEff",n_lim2-1,bin_lim2);
    TH1D* hEff_den = new TH1D ("hEff_den", "hEff_den",n_lim2-1,bin_lim2);

    Double_t x;
    Double_t xt;
    Bool_t matched = false;

    TH1D* hsmear = new TH1D ("hsmear", "hsmear",100,0,2);

    // 5-50 M
    // 0-50 T
    //4 M -> 3 T
    TRandom3 *random = new TRandom3(0);

    std::cout<<"Looping over TTree to fill response matrix"<<std::endl;
    for(int i=0; i<tree_->GetEntries(); i++){
      tree_->GetEntry(i);
      Double_t smear = random->Gaus(1,0.35);
      if(smear < 0) smear = random->Gaus(1,0.35);
      if(smear < 0) smear = random->Gaus(1,0.35);
      if(smear < 0) std::cout<<"error"<<std::endl;

        hsmear->Fill(smear);
      x = jmatch[1]*smear;
      //std::cout<<jmatch[1]<<" "<<smear<<" "<<x<<std::endl;
      xt = jmatch[6];
      if(x > 0 && x < 50) matched = true;
      else matched = false;


      if(matched){
          if(xt > 2)hEff->Fill(xt,bincontent);
      }
      if(xt > 2)hEff_den->Fill(xt,bincontent);

      hTrue_unmatched->Fill(xt,bincontent);


      if(matched){
         response_full->Fill(x,xt,bincontent);
         hMeas_full->Fill(x,bincontent);
         hTrue_full->Fill(xt,bincontent);
      }

      if(matched){
          if(x > 1){
              response_limMeas->Fill(x,xt,bincontent);
              hMeas_lim->Fill(x,bincontent);
              hKine_limMeas->Fill(xt,bincontent);
          }
          hKine_limMeas_den->Fill(xt,bincontent);
      }

      if(matched){
          if(xt > 2){
              response_limTrue->Fill(x,xt,bincontent);
              hTrue_lim->Fill(xt,bincontent);
              hKine_limTrue->Fill(x,bincontent);
          }
          hKine_limTrue_den->Fill(x,bincontent);
      }

      if(matched){
          if(x > 1){
              response_limMeasWithMiss->Fill(x,xt,bincontent);
          }
          else{
              response_limMeasWithMiss->Miss(xt,bincontent);
          }
      }

      if(matched){
          if(xt > 2){
              response_limTrueWithFake->Fill(x,xt,bincontent);
          }
          else{
              response_limTrueWithFake->Fake(x,bincontent);
          }
      }

      if(matched){
          if(x > 1 && xt > 2){
              response_limMeasTrueWithMissFake->Fill(x,xt,bincontent);
              hMeas_limBoth->Fill(x,bincontent);
              hTrue_limBoth->Fill(xt,bincontent);
          }
          else if(x < 1 && xt > 2){
              response_limMeasTrueWithMissFake->Miss(xt,bincontent);
          }
          else if(xt < 2 && x > 1){
              response_limMeasTrueWithMissFake->Fake(x,bincontent);
          }
      }

      if(matched){
          if(x > 1 && xt > 2){
              response_limMeasTrueWithMissFakeEff->Fill(x,xt,bincontent);
          }
          else if(x < 1 && xt > 2){
              response_limMeasTrueWithMissFakeEff->Miss(xt,bincontent);
          }
          else if(xt < 2 && x > 1){
              response_limMeasTrueWithMissFakeEff->Fake(x,bincontent);
          }
      }
      else if(xt > 2){
          response_limMeasTrueWithMissFakeEff->Miss(xt,bincontent);
      }
    }

    for(Int_t bin = 1 ; bin < hMeas_full->GetNbinsX();bin++){
        hMeas_full->SetBinError(bin,TMath::Sqrt(hMeas_full->GetBinContent(bin)));
    }

    for(Int_t bin = 1 ; bin < hMeas_lim->GetNbinsX();bin++){
        hMeas_lim->SetBinError(bin,TMath::Sqrt(hMeas_lim->GetBinContent(bin)));
    }

    for(Int_t bin = 1 ; bin < hMeas_limBoth->GetNbinsX();bin++){
        hMeas_limBoth->SetBinError(bin,TMath::Sqrt(hMeas_limBoth->GetBinContent(bin)));
    }

    hEff->Divide(hEff,hEff_den,1,1,"B");

    hMeas_full->Sumw2();
    hMeas_full->SetLineWidth(2);
    hMeas_full->SetMarkerStyle(23);
    hMeas_full->SetMarkerColor(kGreen+2);
    hMeas_full->SetLineColor(kGreen+2);

    hTrue_full->Sumw2();
    hTrue_full->SetLineWidth(2);
    hTrue_full->SetMarkerStyle(22);


    hMeas_lim->Sumw2();
    hMeas_lim->SetLineWidth(2);
    hMeas_lim->SetMarkerStyle(23);
    hMeas_lim->SetMarkerColor(kGreen+2);
    hMeas_lim->SetLineColor(kGreen+2);

    hTrue_lim->Sumw2();
    hTrue_lim->SetLineWidth(2);
    hTrue_lim->SetMarkerStyle(22);
    hTrue_lim->SetMarkerColor(kBlue);
    hTrue_lim->SetLineColor(kBlue);

    hMeas_limBoth->Sumw2();
    hMeas_limBoth->SetLineWidth(2);
    hMeas_limBoth->SetMarkerStyle(23);
    hMeas_limBoth->SetMarkerColor(kGreen+2);
    hMeas_limBoth->SetLineColor(kGreen+2);

    hTrue_limBoth->Sumw2();
    hTrue_limBoth->SetLineWidth(2);
    hTrue_limBoth->SetMarkerStyle(22);
    hTrue_limBoth->SetMarkerColor(kBlue);
    hTrue_limBoth->SetLineColor(kBlue);

    hKine_limMeas_den->Sumw2();
    hKine_limMeas->Sumw2();


    hKine_limMeas->Divide(hKine_limMeas,hKine_limMeas_den,1,1,"B");
    hKine_limTrue->Divide(hKine_limTrue,hKine_limTrue_den,1,1,"B");
    hTrue_full->SetMarkerColor(kBlue);
    hTrue_lim->SetMarkerColor(kBlue);
    hTrue_limBoth->SetMarkerColor(kBlue);

    hTrue_full->SetLineColor(kBlue);
    hTrue_lim->SetLineColor(kBlue);
    hTrue_limBoth->SetLineColor(kBlue);




/*

  //  response_full->Setup(hMeas_full, hTrue_full);
  //  response_limMeas->Setup(hMeas_lim, hTrue_full);
    //  response_limMeasWithMiss->Setup(hMeas_lim, hTrue_full);




    response_limMeasTrueWithMissFake->Setup(hMeas_lim, hTrue_lim);
    response_limMeasTrueWithMissFakeEff->Setup(hMeas_lim, hTrue_lim);
*/


    RooUnfoldBayes unfold_full (response_full, hMeas_full, 5);
    TH1D *hUnf_full = dynamic_cast<TH1D*>(unfold_full.Hreco());
    hUnf_full->SetMarkerColor(kRed);
    hUnf_full->SetLineColor(kRed);
    hUnf_full->SetLineWidth(2);
    hUnf_full->SetMarkerStyle(21);

    RooUnfoldBayes unfold_limMeas (response_limMeas, hMeas_lim, 5);
    TH1D *hUnf_limMeas = dynamic_cast<TH1D*>(unfold_limMeas.Hreco());
    hUnf_limMeas->SetMarkerColor(kRed);
    hUnf_limMeas->SetLineColor(kRed);
    hUnf_limMeas->SetLineWidth(2);
    hUnf_limMeas->SetMarkerStyle(21);

    TH1D *hUnf_limMeas_wkine = dynamic_cast<TH1D*>(hUnf_limMeas->Clone("hUnf_limMeas_wkine"));
    hUnf_limMeas_wkine->Divide(hKine_limMeas);

    RooUnfoldBayes unfold_limMeasWithMiss (response_limMeasWithMiss, hMeas_lim, 5);
    TH1D *hUnf_limMeasWithMiss = dynamic_cast<TH1D*>(unfold_limMeasWithMiss.Hreco());
    hUnf_limMeasWithMiss->SetMarkerColor(kRed);
    hUnf_limMeasWithMiss->SetLineColor(kRed);
    hUnf_limMeasWithMiss->SetLineWidth(2);
    hUnf_limMeasWithMiss->SetMarkerStyle(21);

    //  response_limTrue->Setup(hMeas_full, hTrue_lim);
    //  response_limTrueWithFake->Setup(hMeas_full, hTrue_lim);

    RooUnfoldBayes unfold_limTrue (response_limTrue, hMeas_full, 5);
    TH1D *hUnf_limTrue = dynamic_cast<TH1D*>(unfold_limTrue.Hreco());
    hUnf_limTrue->SetMarkerColor(kRed);
    hUnf_limTrue->SetLineColor(kRed);
    hUnf_limTrue->SetLineWidth(2);
    hUnf_limTrue->SetMarkerStyle(21);

    TH1D *hMeas_limTrue_wKine = dynamic_cast<TH1D*>(hMeas_full->Clone("hMeas_limTrue_wKine"));
    hMeas_limTrue_wKine->Multiply(hKine_limTrue);

    RooUnfoldBayes unfold_limTrue_wKine (response_limTrue, hMeas_limTrue_wKine, 5);
    TH1D *hUnf_limTrue_wKine = dynamic_cast<TH1D*>(unfold_limTrue_wKine.Hreco());
    hUnf_limTrue_wKine->SetMarkerColor(kRed);
    hUnf_limTrue_wKine->SetLineColor(kRed);
    hUnf_limTrue_wKine->SetLineWidth(2);
    hUnf_limTrue_wKine->SetMarkerStyle(21);

    RooUnfoldBayes unfold_limTrueWithFake (response_limTrueWithFake, hMeas_full, 5);
    TH1D *hUnf_limTrueWithFake = dynamic_cast<TH1D*>(unfold_limTrueWithFake.Hreco());
    hUnf_limTrueWithFake->SetMarkerColor(kRed);
    hUnf_limTrueWithFake->SetLineColor(kRed);
    hUnf_limTrueWithFake->SetLineWidth(2);
    hUnf_limTrueWithFake->SetMarkerStyle(21);

    //response_limMeasTrueWithMissFake->Setup(hMeas_lim, hTrue_lim);
    RooUnfoldBayes unfold_limMeasTrueWithMissFake (response_limMeasTrueWithMissFake, hMeas_lim, 5);
    TH1D *hUnf_limMeasTrueWithMissFake = dynamic_cast<TH1D*>(unfold_limMeasTrueWithMissFake.Hreco());
    hUnf_limMeasTrueWithMissFake->SetMarkerColor(kRed);
    hUnf_limMeasTrueWithMissFake->SetLineColor(kRed);
    hUnf_limMeasTrueWithMissFake->SetLineWidth(2);
    hUnf_limMeasTrueWithMissFake->SetMarkerStyle(21);

    TH1D *hUnf_limMeasTrueWithMissFake_wEffcorr = dynamic_cast<TH1D*>(hUnf_limMeasTrueWithMissFake->Clone("hUnf_limMeasTrueWithMissFake_wEffcorr"));
    hUnf_limMeasTrueWithMissFake_wEffcorr->Divide(hEff);
    hUnf_limMeasTrueWithMissFake_wEffcorr->SetMarkerColor(kRed);
    hUnf_limMeasTrueWithMissFake_wEffcorr->SetLineColor(kRed);
    hUnf_limMeasTrueWithMissFake_wEffcorr->SetLineWidth(2);
    hUnf_limMeasTrueWithMissFake_wEffcorr->SetMarkerStyle(21);


    //response_limMeasTrueWithMissFakeEff->Setup(hMeas_lim, hTrue_lim);

    RooUnfoldBayes unfold_limMeasTrueWithMissFakeEff (response_limMeasTrueWithMissFakeEff, hMeas_lim, 5);
    TH1D *hUnf_limMeasTrueWithMissFakeEff = dynamic_cast<TH1D*>(unfold_limMeasTrueWithMissFakeEff.Hreco());
    hUnf_limMeasTrueWithMissFakeEff->SetMarkerColor(kRed);
    hUnf_limMeasTrueWithMissFakeEff->SetLineColor(kRed);
    hUnf_limMeasTrueWithMissFakeEff->SetLineWidth(2);
    hUnf_limMeasTrueWithMissFakeEff->SetMarkerStyle(21);



  //  hTrue_full->Chi2Test(hUnf_full,"UU P");
   // hTrue_full->Chi2Test(hUnf_limMeas,"UU P");
   // hTrue_full->Chi2Test(hUnf_limMeas_wkine,"UU P");
   // hTrue_full->Chi2Test(hUnf_limMeasWithMiss,"UU P");



    std::cout<<"M-T: full-full"<<std::endl;
    GetX2(hUnf_full,hTrue_full);
    std::cout<<"M-T: lim-full"<<std::endl;
    GetX2(hUnf_limMeas,hTrue_full);
    std::cout<<"M-T: lim-full kine"<<std::endl;
    GetX2(hUnf_limMeas_wkine,hTrue_full);
    std::cout<<"M-T: lim-full miss"<<std::endl;
    GetX2(hUnf_limMeasWithMiss,hTrue_full);

    std::cout<<"M-T: full-lim"<<std::endl;
    GetX2(hUnf_limTrue,hTrue_full);
    std::cout<<"M-T: full-lim kine"<<std::endl;
    GetX2(hUnf_limTrue_wKine,hTrue_full);
    std::cout<<"M-T: full-lim fake"<<std::endl;
    GetX2(hUnf_limTrueWithFake,hTrue_full);

    std::cout<<"M-T: lim-lim miss fake"<<std::endl;
    GetX2(hUnf_limMeasTrueWithMissFake,hTrue_full);

    std::cout<<"M-T: lim-lim miss fake manual eff"<<std::endl;
    GetX2(hUnf_limMeasTrueWithMissFake_wEffcorr,hTrue_unmatched);

    std::cout<<"M-T: lim-lim miss fake eff"<<std::endl;
    GetX2(hUnf_limMeasTrueWithMissFakeEff,hTrue_unmatched);

    Double_t x2[10]= {29.7943, 2.34468e+06, 29.7646, 29.7928, 610.332, 29.7943, 29.7943, 29.7928, 14.5455, 29.7928};
    Int_t ndf[10] = {13,13,13,13,11,11,11,11,11,11};
    TString p[10] = {".005042","<.00001",".005093",".005046","<.00001",".001707",".001707",".001708",".204289",".001708"};





    TCanvas *c[11];
    TLegend *leg[11];
    TPaveText *AnaInfo[11];
    for(Int_t i = 0 ; i< 11 ;i++){
        c[i] = new TCanvas (Form("c%d",i),Form("c%d",i),800,800);
        if(i<4)leg[i] = new TLegend(0.5,0.2,0.9,0.35);
        else leg[i] = new TLegend(0.5,0.2+0.2,0.9,0.35+0.2);
    }
    for(Int_t i = 0 ; i< 10 ;i++){
        if(i<4)AnaInfo[i] = new TPaveText(0.2,0.2,0.3,0.35,"brNDC");
        else AnaInfo[i] = new TPaveText(0.5,0.6,0.9,0.85,"brNDC");
        AnaInfo[i]->SetFillStyle(0);
        AnaInfo[i]->SetBorderSize(0);
        AnaInfo[i]->SetTextFont(42);
        AnaInfo[i]->SetTextSize(0.04f);
        AnaInfo[i]->SetTextAlign(11);
        AnaInfo[i]->AddText(Form("x2: %.4f",x2[i]));
        AnaInfo[i]->AddText(Form("ndf: %d",ndf[i]));
        AnaInfo[i]->AddText(Form("p-value: %s",p[i].Data()));
    }
    c[0]->cd();
    c[0]->cd()->SetLogy();
    leg[0]->AddEntry(hMeas_full,"Meas full range","lp");
    leg[0]->AddEntry(hUnf_full, "Unfolded","lp");
    leg[0]->AddEntry(hTrue_full,"True full range","lp");
    hUnf_full->SetTitle("Meas-True: full-full unfolding");
    hUnf_full->Draw();
    hMeas_full->Draw("same");
    hTrue_full->Draw("same");
    leg[0]->Draw();
    AnaInfo[0]->Draw();


    //std::cout<<"M-T: lim-full"<<std::endl;
    //GetX2(hUnf_limMeas,hTrue_full);
    c[1]->cd();
    c[1]->cd()->SetLogy();
    leg[1]->AddEntry(hMeas_lim,"Meas lim range","lp");
    leg[1]->AddEntry(hUnf_limMeas, "Unfolded","lp");
    leg[1]->AddEntry(hTrue_full,"True full range","lp");
    hUnf_limMeas->SetTitle("Meas-True: lim-full unfolding");
    hUnf_limMeas->Draw();
    hMeas_lim->Draw("same");
    hTrue_full->Draw("same");
    leg[1]->Draw();
    AnaInfo[1]->Draw();

    //std::cout<<"M-T: lim-full kine"<<std::endl;
    //GetX2(hUnf_limMeas_wkine,hTrue_full);
    c[2]->cd();
    c[2]->cd()->SetLogy();
    leg[2]->AddEntry(hMeas_lim,"Meas lim range","lp");
    leg[2]->AddEntry(hUnf_limMeas_wkine, "Unfolded with kine. eff.","lp");
    leg[2]->AddEntry(hTrue_full,"True full range","lp");
    hUnf_limMeas_wkine->SetTitle("Meas-True: lim-full unfolding with kine. eff.");
    hUnf_limMeas_wkine->Draw();
    hMeas_lim->Draw("same");
    hTrue_full->Draw("same");
    leg[2]->Draw();
    AnaInfo[2]->Draw();


    //std::cout<<"M-T: lim-full miss"<<std::endl;
    //GetX2(hUnf_limMeasWithMiss,hTrue_full);
    c[3]->cd();
    c[3]->cd()->SetLogy();
    leg[3]->AddEntry(hMeas_lim,"Meas lim range","lp");
    leg[3]->AddEntry(hUnf_limMeasWithMiss, "Unfolded with Miss()","lp");
    leg[3]->AddEntry(hTrue_full,"True full range","lp");
    hUnf_limMeasWithMiss->SetTitle("Meas-True: lim-full unfolding with Miss()");
    hUnf_limMeasWithMiss->Draw();
    hMeas_lim->Draw("same");
    hTrue_full->Draw("same");
    leg[3]->Draw();
    AnaInfo[3]->Draw();

    //std::cout<<"M-T: full-lim"<<std::endl;
    //GetX2(hUnf_limTrue,hTrue_full);
    c[4]->cd();
    c[4]->cd()->SetLogy();
    leg[4]->AddEntry(hMeas_full,"Meas full range","lp");
    leg[4]->AddEntry(hUnf_limTrue, "Unfolded","lp");
    leg[4]->AddEntry(hTrue_full,"True lim range","lp");
    hUnf_limTrue->SetTitle("Meas-True: full-lim unfolding");
    hUnf_limTrue->Draw();
    hMeas_lim->Draw("same");
    hTrue_full->Draw("same");
    leg[4]->Draw();
    AnaInfo[4]->Draw();


    //std::cout<<"M-T: full-lim kine"<<std::endl;
    //GetX2(hUnf_limTrue_wKine,hTrue_full);
    c[5]->cd();
    c[5]->cd()->SetLogy();
    leg[5]->AddEntry(hMeas_full,"Meas full range","lp");
    leg[5]->AddEntry(hUnf_limTrue_wKine, "Unfolded","lp");
    leg[5]->AddEntry(hTrue_full,"True lim range","lp");
    hUnf_limTrue_wKine->SetTitle("Meas-True: full-lim unfolding with kine. eff.");
    hUnf_limTrue_wKine->Draw();
    hMeas_lim->Draw("same");
    hTrue_full->Draw("same");
    leg[5]->Draw();
    AnaInfo[5]->Draw();


    //std::cout<<"M-T: full-lim fake"<<std::endl;
    //GetX2(hUnf_limTrueWithFake,hTrue_full);
    c[6]->cd();
    c[6]->cd()->SetLogy();
    leg[6]->AddEntry(hMeas_full,"Meas full range","lp");
    leg[6]->AddEntry(hUnf_limTrueWithFake, "Unfolded","lp");
    leg[6]->AddEntry(hTrue_full,"True lim range","lp");
    hUnf_limTrueWithFake->SetTitle("Meas-True: full-lim unfolding with Fake()");
    hUnf_limTrueWithFake->Draw();
    hMeas_lim->Draw("same");
    hTrue_full->Draw("same");
    leg[6]->Draw();
    AnaInfo[6]->Draw();

    //std::cout<<"M-T: lim-lim miss fake"<<std::endl;
    //GetX2(hUnf_limMeasTrueWithMissFake,hTrue_full);
    c[7]->cd();
    c[7]->cd()->SetLogy();
    leg[7]->AddEntry(hMeas_full,"Meas full range","lp");
    leg[7]->AddEntry(hUnf_limMeasTrueWithMissFake, "Unfolded","lp");
    leg[7]->AddEntry(hTrue_full,"True lim range","lp");
    hUnf_limMeasTrueWithMissFake->SetTitle("Meas-True: lim-lim unfolding with Miss() and Fake()");
    hUnf_limMeasTrueWithMissFake->Draw();
    hMeas_lim->Draw("same");
    hTrue_full->Draw("same");
    leg[7]->Draw();
    AnaInfo[7]->Draw();

    //std::cout<<"M-T: lim-lim miss fake manual eff"<<std::endl;
    //GetX2(hUnf_limMeasTrueWithMissFake_wEffcorr,hTrue_unmatched);
    c[8]->cd();
    c[8]->cd()->SetLogy();
    leg[8]->AddEntry(hMeas_full,"Meas full range","lp");
    leg[8]->AddEntry(hUnf_limMeasTrueWithMissFake_wEffcorr, "Unfolded","lp");
    leg[8]->AddEntry(hTrue_unmatched,"True lim range - unmatched","lp");
    hUnf_limMeasTrueWithMissFake_wEffcorr->SetTitle("Meas-True: lim-lim unfolding with Miss() and Fake() unmatched manual eff.");
    hUnf_limMeasTrueWithMissFake_wEffcorr->Draw();
    hMeas_lim->Draw("same");
    hTrue_unmatched->Draw("same");
    leg[8]->Draw();
    AnaInfo[8]->Draw();

    //std::cout<<"M-T: lim-lim miss fake eff"<<std::endl;
    //GetX2(hUnf_limMeasTrueWithMissFakeEff,hTrue_unmatched);
    c[9]->cd();
    c[9]->cd()->SetLogy();
    leg[9]->AddEntry(hMeas_full,"Meas full range","lp");
    leg[9]->AddEntry(hUnf_limMeasTrueWithMissFakeEff, "Unfolded","lp");
    leg[9]->AddEntry(hTrue_unmatched,"True lim range - unmatched","lp");
    hUnf_limMeasTrueWithMissFakeEff->SetTitle("Meas-True: lim-lim unfolding with Miss() and Fake() unmatched Miss() eff.");
    hUnf_limMeasTrueWithMissFakeEff->GetYaxis()->SetRangeUser(100,1E8);
    hUnf_limMeasTrueWithMissFakeEff->Draw();
    hMeas_lim->Draw("same");
    hTrue_unmatched->Draw("same");
    leg[9]->Draw();
    AnaInfo[9]->Draw();

    c[10]->cd();
    hsmear->Draw();
    for(Int_t i = 0 ; i< 11 ;i++){
        c[i]->SaveAs(Form("plotsUnfToy/spectra%d.png",i));
    }





    //        ' = new TCanvas ("c","c",800,800);
   /* c->Divide(3,1);

    c->cd(1);
    c->cd(1)->SetLogy();
    hTrue_full->Draw();
    hMeas_full->Draw("same");
    hUnf_limMeas->Draw("same");
    c->cd(2);
    hsmear->Draw();

     c->cd(2)->SetLogy();
    hTrue_full->Draw();
    hUnf_limTrue_wKine->Draw("same");
    c->cd(3);
     c->cd(3)->SetLogy();
    hTrue_full->Draw();
    hUnf_limTrueWithFake->Draw("same");
*/







}



//std::tuple<Double_t, Int_t>
std::tuple<Double_t, Int_t> GetX2(TH1D *O, TH1D *E){
    Double_t X2 = 0;
    for (Int_t bin = 1; bin <= O->GetNbinsX(); bin++){
        Double_t ObinC = O->GetBinCenter(bin);
        Int_t Ebin = E->FindBin(ObinC);
        Double_t EbinC = E->GetBinCenter(Ebin);
        //std::cout<<((E->GetBinContent(Ebin) - O->GetBinContent(bin))*(E->GetBinContent(Ebin) - O->GetBinContent(bin))/E->GetBinContent(Ebin))<<std::endl;
        X2+= ((E->GetBinContent(Ebin) - O->GetBinContent(bin))*(E->GetBinContent(Ebin) - O->GetBinContent(bin))/(O->GetBinError(bin)*O->GetBinError(bin)));
        //std::cout<<bin<<" "<<ObinC<<" "<<Ebin<<" "<<EbinC<<" "<<O->GetBinContent(bin)<<" "<<E->GetBinContent(Ebin) << std::endl;
    }
    std::cout<<X2<<" "<<O->GetNbinsX()-1<<std::endl;
    return std::make_tuple(X2,O->GetNbinsX()-1);

}


/*



if(matched){
    if(inMeasRange && inTrueRange)
        response->Fill(x,xt);
    else if(!inMeasRange)
        response->Miss(xt); //kinematic correction 1
    else if(!inTrueRange)
        response->Fake(x);  //kinematic correction 2
}
else
    response->Miss(xt,bincontent); //reconstruction efficiency correction
*/
