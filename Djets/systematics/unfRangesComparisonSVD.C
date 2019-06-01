
#include <string>
#include <sstream>
#include <iostream>

#include "sys.h"
#include "config.h"

Int_t colors2[] = {1,2,kGreen+3,kMagenta+2,4,6,kCyan+1,8,kOrange-1,kGray+1,kViolet+5,kYellow+2};
Int_t markers2[] = {20,21,22,23,24,25,26,27,28,29,30,32,33,34};
Int_t linestyle2[] = {1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

const int nFiles =4;

TString inDir[nFiles] = {
    "Default_AnalysisResults_Run2.root/unfolding_Bayes_5",
    "Default_AnalysisResults_Run2.root/unfolding_SVD_5",
    "Default_AnalysisResults_Run2.root/unfolding_SVD_6",
    "Default_AnalysisResults_Run2.root/unfolding_SVD_7"
};

/*
TString inDir[nFiles] = {
    "Default_AnalysisResults_Run2.root/unfolding_Bayes_5",
    "Default_AnalysisResults_Run2.root/unfolding_SVD_6",
    "T350M250_AnalysisResults_Run2.root/unfolding_SVD_6",
    "T350M350_AnalysisResults_Run2.root/unfolding_SVD_6",
    "T450M250_AnalysisResults_Run2.root/unfolding_SVD_6",
    "T450M350_AnalysisResults_Run2.root/unfolding_SVD_6",
    "T450M450_AnalysisResults_Run2.root/unfolding_SVD_6",
    "T550M250_AnalysisResults_Run2.root/unfolding_SVD_6",
    "T550M350_AnalysisResults_Run2.root/unfolding_SVD_6",
    "T550M450_AnalysisResults_Run2.root/unfolding_SVD_6",
    "T550M550_AnalysisResults_Run2.root/unfolding_SVD_6",
    "Default_AnalysisResults_Run2.root/unfolding_SVD_5",
    "T350M250_AnalysisResults_Run2.root/unfolding_SVD_5",
    "T350M350_AnalysisResults_Run2.root/unfolding_SVD_5",
    "T450M250_AnalysisResults_Run2.root/unfolding_SVD_5",
    "T450M350_AnalysisResults_Run2.root/unfolding_SVD_5",
    "T450M450_AnalysisResults_Run2.root/unfolding_SVD_5",
    "T550M250_AnalysisResults_Run2.root/unfolding_SVD_5",
    "T550M350_AnalysisResults_Run2.root/unfolding_SVD_5",
    "T550M450_AnalysisResults_Run2.root/unfolding_SVD_5",
    "T550M550_AnalysisResults_Run2.root/unfolding_SVD_5",
    "Default_AnalysisResults_Run2.root/unfolding_SVD_7",
    "T350M250_AnalysisResults_Run2.root/unfolding_SVD_7",
    "T350M350_AnalysisResults_Run2.root/unfolding_SVD_7",
    "T450M250_AnalysisResults_Run2.root/unfolding_SVD_7",
    "T450M350_AnalysisResults_Run2.root/unfolding_SVD_7",
    "T450M450_AnalysisResults_Run2.root/unfolding_SVD_7",
    "T550M250_AnalysisResults_Run2.root/unfolding_SVD_7",
    "T550M350_AnalysisResults_Run2.root/unfolding_SVD_7",
    "T550M450_AnalysisResults_Run2.root/unfolding_SVD_7",
    "T550M550_AnalysisResults_Run2.root/unfolding_SVD_7",
};*/

TString desc[nFiles] = {
  "meas: 2-50, true: 2-50, Bayes reg=5 (Def)",
  "meas: 2-50, true: 2-50, SVD reg=5",
  "meas: 2-50, true: 2-50, SVD reg=6",
  "meas: 2-50, true: 2-50, SVD reg=7"
};

/*
TString desc[nFiles] = {
  "meas: 2-50, true: 2-50, Bayes reg=5 (Def)",
  "meas: 2-50, true: 2-50, SVD reg=6",
  "meas: 2-50, true: 3-50, SVD reg=6",
  "meas: 3-50, true: 3-50, SVD reg=6",
  "meas: 2-50, true: 4-50, SVD reg=6",
  "meas: 3-50, true: 4-50, SVD reg=6",
  "meas: 4-50, true: 4-50, SVD reg=6",
  "meas: 2-50, true: 5-50, SVD reg=6",
  "meas: 3-50, true: 5-50, SVD reg=6",
  "meas: 4-50, true: 5-50, SVD reg=6",
  "meas: 5-50, true: 5-50, SVD reg=6",
    "meas: 2-50, true: 2-50, SVD reg=5",
    "meas: 2-50, true: 3-50, SVD reg=5",
    "meas: 3-50, true: 3-50, SVD reg=5"
    "meas: 2-50, true: 4-50, SVD reg=5",
    "meas: 3-50, true: 4-50, SVD reg=5",
    "meas: 4-50, true: 4-50, SVD reg=5",
    "meas: 2-50, true: 5-50, SVD reg=5",
    "meas: 3-50, true: 5-50, SVD reg=5",
    "meas: 4-50, true: 5-50, SVD reg=5",
    "meas: 5-50, true: 5-50, SVD reg=5",
    "meas: 2-50, true: 2-50, SVD reg=7",
    "meas: 2-50, true: 3-50, SVD reg=7",
    "meas: 3-50, true: 3-50, SVD reg=7",
    "meas: 2-50, true: 4-50, SVD reg=7",
    "meas: 3-50, true: 4-50, SVD reg=7",
    "meas: 4-50, true: 4-50, SVD reg=7",
    "meas: 2-50, true: 5-50, SVD reg=7",
    "meas: 3-50, true: 5-50, SVD reg=7",
    "meas: 4-50, true: 5-50, SVD reg=7",
    "meas: 5-50, true: 5-50, SVD reg=7",
};*/

//double plotmin = 5, plotmax = 50;
//const int ptbinsN = 7;
//double ptbinsA[ptbinsN+1] = { 5,6,8,10,14,20,30,50 };

//int nJetBins2 = 7; fptbinsJetFinalA
//double ptJetbins2[] = {5,6,8,10,14,20,30,50};

void compareRanges(TString inName, TString inDirBase, int measmin, int measmax, int truemin, int truemax);
void getRMS(const int nFiles, TH1F **hratio, TH1F *hmean, TH1F *hsys);

void unfRangesComparisonSVD(
int reg=4,
TString inDirBase = "/home/jackbauer/Work/alice/analysis/pp5TeV/D0jet/results/DzeroR03_pPbCuts",
TString inName = "systematics/unfolding/diff_ranges_Bayes",
bool isChain = 0,
int measmin=3,
int measmax=50,
int truemin=5,
int truemax=50)
{

  TString inputDir = inDirBase;
//  inDir += "/";
//  inDir += input;
  gSystem->Exec(Form("mkdir %s/%s",inputDir.Data(),inName.Data()));

 compareRanges(inName,inDirBase,measmin,measmax,truemin,truemax);


return;
}

void compareRanges(TString inName, TString inDirBase, int measmin, int measmax, int truemin, int truemax)
{

            TString out = inDirBase;
            out += "/";
            out += inName;
            gStyle->SetOptStat(0000); //Mean and RMS shown
            gSystem->Exec(Form("mkdir %s",out.Data()));

            TString dirName[nFiles];

            for (int i=0; i<nFiles; i++){
                dirName[i] = inDirBase;
                dirName[i] += "/";
                dirName[i] += inDir[i];
              //  dirName[i] += "/unfolding_Bayes_";
              //  dirName[i] += reg;
            }
        //    return;
          //  for(int i=0; i<nFiles; i++){
          //    desc[i] = "reg=";
          //    desc[i] += regList[i];

          //  }

            TFile *outFile = new TFile(Form("%s/UnfoldingRangesComparison.root",out.Data()),"RECREATE");

            TFile *fproj[nFiles];
            for(int i=0; i<nFiles; i++) {
              fproj[i] = new TFile(Form("%s/unfoldedSpectrum_unfoldedJetSpectrum.root",dirName[i].Data()),"READ");
              std::cout<<i<<" OK"<<std::endl;
              if(!fproj[i]) { std::cout << "NO FILE !!!!!" << std::endl; return; }
            }

            TCanvas *cspec = new TCanvas("cspec","cspec",800,600);
            cspec->SetLogy();

            TLegend *leg = new TLegend(0.5,0.45,0.85,0.85);
            leg->SetBorderSize(0);

            TH1F *spec[nFiles];
            TH1F *specReb[nFiles];
std::cout<<"B"<<std::endl;
            for(int i=0; i<nFiles; i++) {
                spec[i] = (TH1F*)fproj[i]->Get("unfoldedSpectrum");
                spec[i]->Sumw2();
                spec[i] -> Scale(1,"width");
                spec[i]->SetTitle("");
                spec[i]->SetLineColor(colors2[i%12]);
                spec[i]->SetMarkerColor(colors2[i%12]);
                spec[i]->SetMarkerStyle(markers2[i%12]);

                specReb[i] = new TH1F(Form("specReb%d",i),"specReb",fptbinsJetFinalN,fptbinsJetFinalA);
                for(int j=1;j<specReb[i]->GetNbinsX()+1;j++){
                    double pt = specReb[i]->GetBinCenter(j);
                    int bin = spec[i]->GetXaxis()->FindBin(pt);
                    double value = spec[i]->GetBinContent(bin);
                    double error = spec[i]->GetBinError(bin);
                    specReb[i]->SetBinContent(j,value);
                    specReb[i]->SetBinError(j,error);
                }

                specReb[i]->SetTitle("");
                specReb[i]->SetLineColor(colors2[i%12]);
                specReb[i]->SetMarkerColor(colors2[i%12]);
                specReb[i]->SetMarkerStyle(markers2[i%12]);

                spec[i]->GetXaxis()->SetRangeUser(fptbinsJetFinalA[0],fptbinsJetFinalA[fptbinsJetFinalN]);
                if(!i) spec[i]->Draw();
                else spec[i]->Draw("same");
                leg->AddEntry(spec[i],desc[i].Data());
                std::cout<<i<<" OK2"<<std::endl;
                //cspec->SaveAs(Form("%s/UnfoldingRangesComparison%u.png",out.Data(),i));
            }
            leg->Draw("same");
std::cout<<"C"<<std::endl;
          cspec->SaveAs(Form("%s/UnfoldingRangesComparison.pdf",out.Data()));
          cspec->SaveAs(Form("%s/UnfoldingRangesComparison.png",out.Data()));

            TLegend *leg2 = new TLegend(0.55,0.70,0.89,0.90);
            leg2->SetBorderSize(0);
            TCanvas *cspec2 = new TCanvas("cspec2","cspec2",1200,800);
            TH1F *hratio[nFiles-1];
            for(int i=0; i<nFiles-1; i++){
                hratio[i] = (TH1F*)specReb[i+1]->Clone( Form("hratio_%d",i));
                hratio[i]->Divide(specReb[0]);
                hratio[i]->SetLineStyle(linestyle2[i%12]);
                hratio[i]->SetLineWidth(2);
                hratio[i]->GetXaxis()->SetRangeUser(fptbinsJetFinalA[0],fptbinsJetFinalA[fptbinsJetFinalN]);
                hratio[i]->GetYaxis()->SetRangeUser(0.9,1.18);
                hratio[i]->GetYaxis()->SetTitle(Form("ratio to central (%s)",desc[0].Data()));
                if(!i) hratio[i]->Draw("hist");
                else hratio[i]->Draw("samehist");
                leg2->AddEntry(hratio[i],desc[i+1].Data(),"l");
                std::cout<<i<<" OK3"<<std::endl;
            }
            leg2->Draw("same");
std::cout<<"D"<<std::endl;
            TLine *line = new TLine(fptbinsJetFinalA[0],1,fptbinsJetFinalA[fptbinsJetFinalN],1);
            line->SetLineStyle(2);
            line->SetLineWidth(2);
            line->Draw("same");

            cspec2->SaveAs(Form("%s/UnfoldingRangesComparison_ratio.pdf",out.Data()));
            cspec2->SaveAs(Form("%s/UnfoldingRangesComparison_ratio.png",out.Data()));
std::cout<<"E"<<std::endl;
            TH1F *hsys = new TH1F("hsys","syst. rms; p_{T,ch jet};  RMS [%]",fptbinsJetFinalN,fptbinsJetFinalA);
            TH1F *hmean = (TH1F*)hsys->Clone("hmean");
            getRMS(nFiles,hratio,hmean,hsys);
std::cout<<"F"<<std::endl;
            hsys->GetYaxis()->SetRangeUser(0,10);
            hsys->SetLineColor(kViolet+2);
            TCanvas *cspecRMS = new TCanvas("cspecRMS","cspecRMS",800,400);
            hsys->Draw("hist");

            cspecRMS->SaveAs(Form("%s/UnfoldingRangesComparison_rms.pdf",out.Data()));
            cspecRMS->SaveAs(Form("%s/UnfoldingRangesComparison_rms.png",out.Data()));

            hmean->GetYaxis()->SetRangeUser(0.994,1.002);
            hmean->SetLineColor(kMagenta+1);
            TCanvas *cspecMean = new TCanvas("cspecMean","cspecMean",800,400);
            hmean->GetYaxis()->SetRangeUser(0,15);
            hmean->Draw("hist");
           // line->Draw("same");

            cspecMean->SaveAs(Form("%s/UnfoldingRangesComparison_mean.pdf",out.Data()));
            cspecMean->SaveAs(Form("%s/UnfoldingRangesComparison_mean.png",out.Data()));

            outFile->cd();
            cspec->Write();
            cspec2->Write();
            cspecMean->Write();
            cspecRMS->Write();
            hmean->Write();
            hsys->Write();

            outFile->Close();

            return;

}

void getRMS(const int nFiles, TH1F **hratio, TH1F *hmean, TH1F *hsys)
{

  //TH1D *hsys = new TH1D("hsys","syst. rms; p_{T,ch jet};  sys [%] (rms)",nJetBins2,ptJetbins2);
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

  //hmean = (TH1F*)hsys->Clone("hmean");
  hmean->GetYaxis()->SetTitle("abs(1-mean)[%]");
//  hmean->GetYaxis()->SetRangeUser(0.95,1.1);
  hmean->SetMarkerStyle(20);
  hmean->SetLineStyle(1);
  hmean->SetTitle("");

  double *rms = new double[fptbinsJetFinalN];
  double *mean = new double[fptbinsJetFinalN];
  for(int i=0; i<fptbinsJetFinalN; i++){
      rms[i] = 0;
       mean[i] = 0;
       for (int j=0; j<nFiles-1; j++){
         mean[i] = mean[i]+ ( hratio[j]->GetBinContent(hratio[j]->FindBin( (fptbinsJetFinalA[i]+fptbinsJetFinalA[i+1])/2. )) );
      //double m = ( hratios[j]->GetBinContent(hratios[j]->FindBin( (ptJetbins2[i]+ptJetbins2[i+1])/2. )) );
      //rms[i] = rms[i]+ ( 1-hratios[j]->GetBinContent(hratios[j]->FindBin( (ptJetbins2[i]+ptJetbins2[i+1])/2. )) ) * ( 1-hratios[j]->GetBinContent(hratios[j]->FindBin( (ptJetbins2[i]+ptJetbins2[i+1])/2. )) ) ;
      }
   mean[i] = mean[i]/(double)(nFiles-1);

   for (int j=0; j<nFiles-1; j++){
      //mean[i] = mean[i]+ ( hratios[j]->GetBinContent(hratios[j]->FindBin( (ptJetbins2[i]+ptJetbins2[i+1])/2. )) );
      //double m = ( hratios[j]->GetBinContent(hratios[j]->FindBin( (ptJetbins2[i]+ptJetbins2[i+1])/2. )) );
    //  rms[i] = rms[i]+ ( mean[i]-hratio[j]->GetBinContent(hratio[j]->FindBin( (ptJetbins2[i]+ptJetbins2[i+1])/2. )) ) * ( mean[i]-hratio[j]->GetBinContent(hratio[j]->FindBin( (ptJetbins2[i]+ptJetbins2[i+1])/2. )) ) ;
      rms[i] = rms[i]+ ( 1-hratio[j]->GetBinContent(hratio[j]->FindBin( (fptbinsJetFinalA[i]+fptbinsJetFinalA[i+1])/2. )) ) * ( 1-hratio[j]->GetBinContent(hratio[j]->FindBin( (fptbinsJetFinalA[i]+fptbinsJetFinalA[i+1])/2. )) ) ;


  }
      rms[i] = sqrt(rms[i]/(double)(nFiles-1));
      hsys->SetBinContent(i+1,rms[i]*100);

      hmean->SetBinContent(i+1,TMath::Abs(1-mean[i])*100);
      std::cout << "RMS pT " << (fptbinsJetFinalA[i]+fptbinsJetFinalA[i+1])/2. << " GeV/c:\t" << rms[i]*100 << std::endl;
      std::cout << "Mean pT " << (fptbinsJetFinalA[i]+fptbinsJetFinalA[i+1])/2. << " GeV/c:\t" << mean[i] << std::endl;
  }

}
