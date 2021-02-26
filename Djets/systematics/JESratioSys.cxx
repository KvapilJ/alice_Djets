
#include <string>
#include <sstream>
#include <iostream>

#include "config.h"


void JESratioSys(){
    //TString file[2][10];
    TFile * rfile[3][2];
    TH1D* h[3][2];
    TH1D* hR24_D;
    TH1D* hR24_96;
    TH1D* hR26_D;
    TH1D* hR26_96;

    for(Int_t i = 0; i<3 ;i++){
        rfile[i][0] = new TFile(Form("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_X0%d/Default_AnalysisResults_Run2w18b.root/unfolding_Bayes_5/unfoldedSpectrum_unfoldedJetSpectrum.root",(i+1)*2),"READ");
        rfile[i][1] = new TFile(Form("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_X0%d/Default_AnalysisResults_Run2w18b.root96/unfolding_Bayes_5/unfoldedSpectrum_unfoldedJetSpectrum.root",(i+1)*2),"READ");
        if(!rfile[i][0]) std::cout<<"Cannot open histo: "<<i<<" "<<"0"<<std::endl;
        if(!rfile[i][1]) std::cout<<"Cannot open histo: "<<i<<" "<<"1"<<std::endl;
    }


    for(Int_t i = 0; i<3 ;i++){
        for(Int_t j = 0; j< 2;j++){
        h[i][j] = dynamic_cast<TH1D*>(rfile[i][j]->Get("unfoldedSpectrum"));
        if(!h[i][j]) std::cout<<"Cannot open histo: "<<i<<" "<<j<<std::endl;
       // h[0][j]->SetMarkerStyle(20);
       // h[0][j]->SetTitle("division 96.5% RM, 3.5% data, nTrials = 10 ");
       // h[0][j]->GetXaxis()->SetRangeUser(5,50);
       // h[0][j]->GetYaxis()->SetRangeUser(0,20);
    }
    }

   /* hR24_D = (TH1D*)h[0][0]->Clone("hR24_D");
    hR24_D->Divide(h[1][0]);
    hR24_96 = (TH1D*)h[0][1]->Clone("hR24_96");
    hR24_96->Divide(h[1][1]);
    hR26_D = (TH1D*)h[0][0]->Clone("hR26_D");
    hR26_D->Divide(h[2][0]);
    hR26_96 = (TH1D*)h[0][1]->Clone("hR26_96");
    hR26_96->Divide(h[2][1]);*/

    hR24_D = (TH1D*)h[0][0]->Clone("hR24_D");
    hR24_D->Divide(h[0][1]);
    hR24_96 = (TH1D*)h[1][0]->Clone("hR24_96");
    hR24_96->Divide(h[1][1]);
    hR26_D = (TH1D*)h[0][0]->Clone("hR26_D");
    hR26_D->Divide(h[0][1]);
    hR26_96 = (TH1D*)h[2][0]->Clone("hR26_96");
    hR26_96->Divide(h[2][1]);

    TH1D *hR24_96_h24_D = (TH1D*)hR24_96->Clone("hR24_96_h24_D");
    hR24_96_h24_D->Divide(hR24_D);

    for(Int_t b = 1; b <= hR24_96_h24_D->GetNbinsX() ;b++){
        std::cout<<hR24_96_h24_D->GetBinCenter(b)<<" "<<TMath::Abs(1-hR24_96_h24_D->GetBinContent(b))<<std::endl;
    }

    TH1D *hR26_96_h26_D = (TH1D*)hR26_96->Clone("hR26_96_h26_D");
    hR26_96_h26_D->Divide(hR26_D);

    for(Int_t b = 1; b <= hR26_96_h26_D->GetNbinsX() ;b++){
        std::cout<<hR26_96_h26_D->GetBinCenter(b)<<" "<<TMath::Abs(1-hR26_96_h26_D->GetBinContent(b))<<std::endl;
    }

    TCanvas *c = new TCanvas("c","c",1600,800);
    c->Divide(3,2);
    c->cd(1);
    hR24_D->Draw();
    c->cd(2);
    hR24_96->Draw();
    c->cd(3);
    hR24_96_h24_D->Draw();
    c->cd(4);
    hR26_D->Draw();
    c->cd(5);
    hR26_96->Draw();
    c->cd(6);
    hR26_96_h26_D->Draw();




   /*

    hRMS[0] = (TH1D*)h[0][0]->Clone("RMS1");
   // hRMS[1] = (TH1D*)h[1][0]->Clone("RMS2");
    hRMS[0]->SetMarkerStyle(20);
    //hRMS[1]->SetMarkerStyle(20);
    hRMS[0]->GetXaxis()->SetRangeUser(5,50);
   // hRMS[1]->GetXaxis()->SetRangeUser(5,50);
    hRMS[0]->GetYaxis()->SetRangeUser(0,20);
   // hRMS[1]->GetYaxis()->SetRangeUser(0,20);
    hRMS[0]->GetYaxis()->SetTitle("RMS [%]");
   // hRMS[1]->GetYaxis()->SetTitle("RMS [%]");

    std::cout<<"A"<<std::endl;

    for(int bin=1; bin<=h[0][0]->GetNbinsX(); bin++){
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
    }

    for(int bin=1; bin<=h[1][0]->GetNbinsX(); bin++){
            Double_t rms_bin = 0;
            for (UInt_t file=0; file<10; file++){
                rms_bin = rms_bin + (h[1][file]->GetBinContent(bin)) * (h[1][file]->GetBinContent(bin));
            }
            rms_bin = TMath::Sqrt(rms_bin/10);

            hRMS[1]->SetBinContent(bin,rms_bin);
            hRMS[1]->SetBinError(bin,0);
    }*/
   // std::cout<<"B"<<std::endl;


   // c->SaveAs("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_X06/systematics/UnfTrials.png");



}
