
#include <string>
#include <sstream>
#include <iostream>

#include "config.h"


void UnfTrial(){
    //TString file[2][10];
    TFile * rfile[1][10];
    TH1D* h[1][10];
    TH1D* hRMS[1];

    for(Int_t j = 0; j< 10;j++){
        rfile[0][j] = new TFile(Form("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts/systematics/UNFClosure%d/SystematicsOut.root",j+1),"READ");
      //  rfile[1][j] = new TFile(Form("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts/systematics/9010/UNFClosure%d/SystematicsOut.root",j+1),"READ");
    }


    for(Int_t j = 0; j< 10;j++){
        h[0][j] = dynamic_cast<TH1D*>(rfile[0][j]->Get("hMean"));
       // h[1][j] = dynamic_cast<TH1D*>(rfile[1][j]->Get("hMaxVAR"));
        if(!h[0][j]) std::cout<<"Cannot open histo: "<<"0 "<<j<<std::endl;
       // if(!h[1][j]) std::cout<<"Cannot open histo: "<<"1 "<<j<<std::endl;
        h[0][j]->SetMarkerStyle(20);
       // h[1][j]->SetMarkerStyle(20);
        h[0][j]->SetTitle("division 96.5% RM, 3.5% data, nTrials = 10 ");
       // h[1][j]->SetTitle("division 90% RM, 10% data, nTrials = 10 ");
        h[0][j]->GetXaxis()->SetRangeUser(5,50);
       // h[1][j]->GetXaxis()->SetRangeUser(5,50);
        h[0][j]->GetYaxis()->SetRangeUser(0.85,1.15);
       // h[1][j]->GetYaxis()->SetRangeUser(0,20);
    }

    hRMS[0] = (TH1D*)h[0][0]->Clone("RMS1");
   // hRMS[1] = (TH1D*)h[1][0]->Clone("RMS2");
    hRMS[0]->SetMarkerStyle(20);
    //hRMS[1]->SetMarkerStyle(20);
    hRMS[0]->GetXaxis()->SetRangeUser(5,50);
   // hRMS[1]->GetXaxis()->SetRangeUser(5,50);
    hRMS[0]->GetYaxis()->SetRangeUser(0,5);
   // hRMS[1]->GetYaxis()->SetRangeUser(0,20);
    hRMS[0]->GetYaxis()->SetTitle("mean [%]");
   // hRMS[1]->GetYaxis()->SetTitle("RMS [%]");

    std::cout<<"A"<<std::endl;

   /* for(int bin=1; bin<=h[0][0]->GetNbinsX(); bin++){
       // std::cout<<bin<<std::endl;
            Double_t rms_bin = 0;
            for (UInt_t file=0; file<10; file++){
                //std::cout<<file<<" "<<bin<<std::endl;
                rms_bin = rms_bin + (h[0][file]->GetBinContent(bin)) * (h[0][file]->GetBinContent(bin));
            }
           // std::cout<<rms_bin<<std::endl;
            rms_bin = TMath::Sqrt(rms_bin/10);
          //  std::cout<<rms_bin<<std::endl;

            hRMS[0]->SetBinContent(bin,rms_bin);
            hRMS[0]->SetBinError(bin,0);
            std::cout<<bin<<" "<<rms_bin<<std::endl;
    }*/

    for(int bin=1; bin<=h[0][0]->GetNbinsX(); bin++){
       // std::cout<<bin<<std::endl;
            Double_t mean_bin = 0;
            for (UInt_t file=0; file<10; file++){
                //std::cout<<file<<" "<<bin<<std::endl;
                mean_bin = mean_bin + h[0][file]->GetBinContent(bin);
            }
           // std::cout<<rms_bin<<std::endl;
            mean_bin = mean_bin/10;
          //  std::cout<<rms_bin<<std::endl;

            hRMS[0]->SetBinContent(bin,100*TMath::Abs(1-mean_bin));
            hRMS[0]->SetBinError(bin,0);
            std::cout<<bin<<" "<<mean_bin<<std::endl;
    }

/*
    for(int bin=1; bin<=h[1][0]->GetNbinsX(); bin++){
            Double_t rms_bin = 0;
            for (UInt_t file=0; file<10; file++){
                rms_bin = rms_bin + (h[1][file]->GetBinContent(bin)) * (h[1][file]->GetBinContent(bin));
            }
            rms_bin = TMath::Sqrt(rms_bin/10);

            hRMS[1]->SetBinContent(bin,rms_bin);
            hRMS[1]->SetBinError(bin,0);
    }*/
    std::cout<<"B"<<std::endl;

    TCanvas *c = new TCanvas("c","c",1600,800);
    c->Divide(1,2);


    for(Int_t j = 0; j< 10;j++){
        c->cd(1);
        j==0?h[0][j]->Draw("P"):h[0][j]->Draw("sameP");
       // c->cd(2);
       // j==0?h[1][j]->Draw("P"):h[1][j]->Draw("sameP");
        c->cd(2);
        j==0?hRMS[0]->Draw("P"):hRMS[0]->Draw("sameP");
      //  c->cd(4);
      //  j==0?hRMS[1]->Draw("P"):hRMS[1]->Draw("sameP");
    }
    c->SaveAs("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts/systematics/UnfTrials.png");



}
