
#include <string>
#include <sstream>
#include <iostream>

#include "config.h"


void UnfTrialZ(){
    //TString file[2][10];
    TFile * rfile[5][10];
    TH1D* h[5][10];
    TH1D* hRMS[5];
    Int_t R = 6;

    for(Int_t i = 1; i<= 5;i++){
    for(Int_t j = 0; j< 10;j++){
        rfile[i-1][j] = new TFile(Form("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_z0%dF/systematics/UNFClosure%d%d/SystematicsOut.root",R,i,j+1),"READ");
        //rfile[i-1][j] = new TFile(Form("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts/systematics/UNFClosure%d%d/SystematicsOut.root",i,j+1),"READ");
    }
    }


        for(Int_t i = 0; i< 5;i++){
    for(Int_t j = 0; j< 10;j++){
       // h[i][j] = dynamic_cast<TH1D*>(rfile[i][j]->Get("hMaxVAR"));
        h[i][j] = dynamic_cast<TH1D*>(rfile[i][j]->Get("hMean"));
        if(!h[i][j]) std::cout<<"Cannot open histo: "<<i<<j<<std::endl;
        h[i][j]->SetMarkerStyle(20);
        h[i][j]->SetTitle(Form("Random MC division, nTrials = 10. z bin = %d, R = 0.%d",i+1,R));
       // h[i][j]->GetXaxis()->SetRangeUser(5,50);
        h[i][j]->GetYaxis()->SetRangeUser(0.8,1.2);
        if(i==0)h[i][j]->GetYaxis()->SetRangeUser(0.8,1.2);
    }
        }

         for(Int_t i = 0; i< 5;i++){
    hRMS[i] = (TH1D*)h[i][0]->Clone(Form("RMS%d",i+1));
    hRMS[i]->SetMarkerStyle(20);
   // hRMS[i]->GetXaxis()->SetRangeUser(5,50);
    hRMS[i]->GetYaxis()->SetRangeUser(0,7);
    if(i==0)hRMS[i]->GetYaxis()->SetRangeUser(0,14);
    hRMS[i]->GetYaxis()->SetTitle("mean [%]");
         }


    std::cout<<"A"<<std::endl;

    std::ofstream txtout;
    txtout.open (Form("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_z0%dF/systematics/UnfTrialSys.txt",R));


//rms
/*for(Int_t i = 0; i< 5;i++){
    txtout << "Zbin: "<<i<<"\n";
    for(int bin=1; bin<=h[i][0]->GetNbinsX(); bin++){
       // std::cout<<bin<<std::endl;
            Double_t rms_bin = 0;
            for (UInt_t file=0; file<10; file++){
                //std::cout<<file<<" "<<bin<<std::endl;
                rms_bin = rms_bin + (h[i][file]->GetBinContent(bin)) * (h[i][file]->GetBinContent(bin));
            }
           // std::cout<<rms_bin<<std::endl;
            rms_bin = TMath::Sqrt(rms_bin/10);
          //  std::cout<<rms_bin<<std::endl;

            hRMS[i]->SetBinContent(bin,rms_bin);
            hRMS[i]->SetBinError(bin,0);
            txtout<<rms_bin<<"\n";
            std::cout<<bin<<" "<<rms_bin<<std::endl;
    }
}*/

for(Int_t i = 0; i< 5;i++){
    txtout << "Zbin: "<<i<<"\n";
    for(int bin=1; bin<=h[i][0]->GetNbinsX(); bin++){
       // std::cout<<bin<<std::endl;
            Double_t mean_bin = 0;
            for (UInt_t file=0; file<10; file++){
                //std::cout<<file<<" "<<bin<<std::endl;
                mean_bin = mean_bin + h[i][file]->GetBinContent(bin);
            }
           // std::cout<<rms_bin<<std::endl;
            mean_bin = mean_bin/10;
          //  std::cout<<rms_bin<<std::endl;

            hRMS[i]->SetBinContent(bin,100*TMath::Abs(1-mean_bin));
            hRMS[i]->SetBinError(bin,0);
            txtout<<100*TMath::Abs(1-mean_bin)<<"\n";
            std::cout<<bin<<" "<<100*TMath::Abs(1-mean_bin)<<std::endl;
    }
}

txtout.close();

    std::cout<<"B"<<std::endl;

    TCanvas *c = new TCanvas("c","c",1600,1600);
    c->Divide(2,5);

for(Int_t i = 0; i< 5;i++){
    for(Int_t j = 0; j< 10;j++){
        c->cd(2*i+1);
        j==0?h[i][j]->Draw("P"):h[i][j]->Draw("sameP");
        c->cd(2*i+2);
        j==0?hRMS[i]->Draw("P"):hRMS[i]->Draw("sameP");
    }
}
    c->SaveAs(Form("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_z0%dF/systematics/UnfTrials.png",R));





}
