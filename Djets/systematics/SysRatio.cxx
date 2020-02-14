#include "SysRatio.h"

SysRatio::SysRatio(UInt_t nFiles, TString name):nFiles_(nFiles),name_(name){
};

void SysRatio::SetVarFilelist(TString *filelist){
    for(UInt_t f = 0; f < nFiles_; f++){
        TFile_.push_back(new TFile(path_+filelist[f]));
    }
}

void SysRatio::SetVarLegendDesc(TString *legendlist){
    for(UInt_t f = 0; f < nFiles_; f++){
        legendVarDesc_.push_back(legendlist[f]);
    }
}

void SysRatio::SetDefaultFile(TString filename){
    TFileDefault_ = new TFile(path_ + filename);
};

void SysRatio::GetRMS(){
    if(taskInhibit)return;
    hRMS_ = dynamic_cast<TH1D*>(TH1DDefault_->Clone("hRMS"));
    hRMS_->SetTitle("Systematics: "+name_);
    if(nFiles_==1)hRMS_->GetYaxis()->SetTitle("Variation from default [%]");
    else hRMS_->GetYaxis()->SetTitle("RMS [%]");
    if(method_ == kMax)hRMS_->GetYaxis()->SetTitle("Max variation [%]");
  //  std::cout<<"size "<<hRatio_.size()<<std::endl;
    Double_t RMSmax = 0;
    for(int bin=1; bin<=TH1DDefault_->GetNbinsX(); bin++){
        Double_t rms_bin = 0;
        for (UInt_t file=0; file<nFiles_; file++){
            //if(TMath::Abs(1-hRatio_[file]->GetBinContent(bin))>0.9) continue;
            if(method_ == kMax){
              //  std::cout<<file<<" "<<rms_bin<<" "<<TMath::Abs(1-hRatio_[file]->GetBinContent(bin))<<std::endl;
                rms_bin = TMath::Abs(1-hRatio_[file]->GetBinContent(bin))>rms_bin?TMath::Abs(1-hRatio_[file]->GetBinContent(bin)):rms_bin;
            }
            else if(nFiles_==1){
                rms_bin = TMath::Abs((1 - hRatio_[file]->GetBinContent(bin)));
            }
            else{
                rms_bin = rms_bin + (1 - hRatio_[file]->GetBinContent(bin)) * (1-hRatio_[file]->GetBinContent(bin));
            }


        }
        //std::cout<<bin<<" "<<rms_bin<<std::endl;
        if(nFiles_>1 && method_ != kMax){ rms_bin = TMath::Sqrt(rms_bin/nFiles_);}
        std::cout<<bin<<" "<<rms_bin*100<<std::endl;
        RMSmax = RMSmax>rms_bin*100?RMSmax:rms_bin*100;
        hRMS_->SetBinContent(bin,rms_bin*100);
        hRMS_->SetBinError(bin,0);
    }
    hRMS_->SetMinimum(0);
    hRMS_->SetMaximum(1.1*RMSmax);
    hRMS_->SetLineColor(kRed+1);
    hRMS_->SetMarkerColor(kRed+1);
};

void SysRatio::GetMean(){
    if(taskInhibit)return;
    hMean_ = dynamic_cast<TH1D*>(TH1DDefault_->Clone("hMean"));
    hMean_->SetTitle("Systematics: "+name_);
    hMean_->GetYaxis()->SetTitle("mean");
    Double_t meanMin = 100;
    Double_t meanMax = 0;
    for(int bin=1; bin<=TH1DDefault_->GetNbinsX(); bin++){
        Double_t mean_bin = 0;
        for (UInt_t file=0; file<nFiles_; file++){
           mean_bin = mean_bin + hRatio_[file]->GetBinContent(bin);
        }
        mean_bin = mean_bin/nFiles_;
        hMean_->SetBinContent(bin,mean_bin);
        hMean_->SetBinError(bin,0);
        meanMax = meanMax>mean_bin?meanMax:mean_bin;
        meanMin = meanMin>mean_bin?mean_bin:meanMin;
    }
    hMean_->SetMinimum(0.9*meanMin);
    hMean_->SetMaximum(1.1*meanMax);
    hMean_->SetLineColor(kRed+1);
    hMean_->SetMarkerColor(kRed+1);
};

void SysRatio::Write(){
    if(taskInhibit)return;
    if(!gSystem->OpenDirectory(outPath_))gSystem->Exec("mkdir "+outPath_);
    if(!gSystem->OpenDirectory(outPath_+"/"+outFolder_))gSystem->Exec("mkdir "+outPath_+"/"+outFolder_);
    TFile *out = new TFile(outPath_+"/"+outFolder_+"/SystematicsOut.root","RECREATE");
    out->cd();
    hStackMain_->Write();
    hStackRatio_->Write();
    if(method_ == kMax) hRMS_->Write("hMaxVAR");
    else if(nFiles_>1)hRMS_->Write();
    else hRMS_->Write("hVAR");
    hMean_->Write();
    out->Close();
    cStack_->SaveAs(outPath_+"/"+outFolder_+"/data.png");
    if(method_ == kMax)cRMS_->SaveAs(outPath_+"/"+outFolder_+"/MaxVar.png");
    else if(nFiles_>1)cRMS_->SaveAs(outPath_+"/"+outFolder_+"/RMS.png");
    else cRMS_->SaveAs(outPath_+"/"+outFolder_+"/VAR.png");
    cMean_->SaveAs(outPath_+"/"+outFolder_+"/Mean.png");
    //file
    std::ofstream txtout;
    txtout.open (outPath_+"/"+outFolder_+"/SystematicsOut.txt");
    if(method_ == kMax)txtout << "MaxVar:\n";
    else if(nFiles_>1)txtout << "RMS:\n";
    else txtout << "Var:\n";

    for (Int_t bin = 1;bin<=hRMS_->GetNbinsX();bin++){
        txtout<<hRMS_->GetBinContent(bin)<<"\n";
    }
    txtout << "hMean:\n";
    for (Int_t bin = 1;bin<=hMean_->GetNbinsX();bin++){
        txtout<<hMean_->GetBinContent(bin)<<"\n";
    }
    txtout.close();

};

void SysRatio::Run(){



    this->Load();
    this->GetRMS();
    this->GetMean();
    this->Draw();
    this->Write();
    this->ResetRun();
};
void SysRatio::Load(){
    hStackRatio_= new THStack("hStackRatio_","");
    hStackMain_= new THStack("hStackMain_","");
    //leg_ = new TLegend(0.55,0.2,0.9,0.5);
    leg_ = new TLegend(0.55,0.5,0.9,0.8);
    legRatio_ = new TLegend(0.5,0.2,0.85,0.5);
    leg_->SetFillStyle(0);
    leg_->SetBorderSize(0);
    legRatio_->SetFillStyle(0);
    legRatio_->SetBorderSize(0);
    std::cout<<"loading DEF hist: "<<histoName_<<" in "<<TFileDefault_->GetPath()<<std::endl;
    TH1DDefault_=dynamic_cast<TH1D*>(TFileDefault_->Get(histoName_));
    TH1DDefault_->SetLineColor(kBlack);
    TH1DDefault_->SetMarkerColor(kBlack);
    hStackMain_->Add(TH1DDefault_,"nostack");
    leg_->AddEntry(TH1DDefault_,legendDefDesc_);

    for(UInt_t f = 0; f < nFiles_; f++){
        std::cout<<"loading VAR hist: "<<histoName_<<" in "<<TFile_[f]->GetPath()<<std::endl;
        TH1D_.push_back(dynamic_cast<TH1D*>(TFile_[f]->Get(histoName_)));
        if(!TH1D_[f]){std::cout<<"olala"<<std::endl;taskInhibit=true;return;}
        TH1D_[f]->Sumw2();
        TH1D_[f]->SetTitle("");
        TH1D_[f]->SetLineColor(colors_[f%12]);
        TH1D_[f]->SetMarkerColor(colors_[f%12]);
        TH1D_[f]->SetMarkerStyle(markers_[f%14]);
        leg_->AddEntry(TH1D_[f],legendVarDesc_[f]);
        hStackMain_->Add(TH1D_[f],"nostack");
        //ratios
        //TH1D* tmp = dynamic_cast<TH1D*>(TH1D_[f]->Clone(Form("hratio_%d",f)));
        //tmp->Divide(TH1DDefault_);
        hRatio_.push_back(dynamic_cast<TH1D*>(TH1D_[f]->Clone(Form("hratio_%d",f))));
        hRatio_[f]->Divide(TH1DDefault_);
        hRatio_[f]->SetLineStyle(linestyle_[(f+1)%15]);
        hRatio_[f]->SetLineWidth(2);
        hRatio_[f]->GetYaxis()->SetTitle("ratio to central ("+legendDefDesc_+")");
        legRatio_->AddEntry(hRatio_[f],legendVarDesc_[f],"l");
        TString opt{"hist"};
        if(method_ == kError) opt = "nostack";
        hStackRatio_->Add(hRatio_[f],opt);
      //  TCanvas *ctmp = new TCanvas(+Form("ctmp%d",f),"ctmp",800,800);
    //    ctmp->cd();
        hRatio_[f]->SetMinimum(0);
        hRatio_[f]->SetMaximum(2);
   //     hRatio_[f]->Draw();
    //    ctmp->SaveAs(outPath_+"/"+outFolder_+Form("/ratio%d.png",f));
    }
}

void SysRatio::Draw(){
    if(taskInhibit)return;
    line_ = new TLine(TH1DDefault_->GetBinLowEdge(1),1,1,1);
    line_->SetLineStyle(2);
    line_->SetLineWidth(2);
    Double_t max = 0;
    Double_t min = 1000000;
    for (auto hist : TH1D_){
        for (Int_t bin = 1; bin <= dynamic_cast<TH1D*>(hist)->GetNbinsX();bin++){
            Double_t val = dynamic_cast<TH1D*>(hist)->GetBinContent(bin);
            max = max>val?max:val;
            min = min>val?val:min;
        }
    }

    Double_t minR = 100;
    Double_t maxR = 0;
    for (auto hist : hRatio_){
        for (Int_t bin = 1; bin <= dynamic_cast<TH1D*>(hist)->GetNbinsX();bin++){
            Double_t val = dynamic_cast<TH1D*>(hist)->GetBinContent(bin);
   //         std::cout<<"bin "<<bin<<" val "<<val<<std::endl;
            minR = minR>val?val:minR;
            maxR = maxR>val?maxR:val;
        }
    }
    hStackMain_->SetTitle(name_);

    cStack_ = new TCanvas("cStack_","ratio "+name_,1600,800);
    cStack_->Divide(2,1);
    cStack_->cd(1);
    cStack_->cd(1)->SetLeftMargin(0.15f);
    cStack_->cd(1)->SetRightMargin(0.1f);
    cStack_->cd(1)->SetBottomMargin(0.15f);
    cStack_->cd(1)->SetLogy();
    hStackMain_->Draw("nostack");
    leg_->Draw("same");
 //   hStackMain_->GetYaxis()->SetTitle("dN/dz_{#parallel}");
 //   hStackMain_->GetXaxis()->SetTitle("z_{#parallel}");
 //   hStackMain_->SetMaximum(1.1*max);
 //   hStackMain_->SetMinimum(0);
    hStackMain_->GetYaxis()->SetTitle("dN/dp_{T, ch. jet}");
    hStackMain_->GetXaxis()->SetTitle("p_{T, ch. jet}");
    hStackMain_->SetMaximum(3*max);
    hStackMain_->SetMinimum(0.2*min);
    cStack_->cd(2);
    cStack_->cd(2)->SetLeftMargin(0.15f);
    cStack_->cd(2)->SetRightMargin(0.15f);
    cStack_->cd(2)->SetBottomMargin(0.15f);
    hStackRatio_->Draw("nostack");
    hStackRatio_->GetYaxis()->SetTitle("ratio to central ("+legendDefDesc_+")");
    hStackRatio_->SetMinimum(3*minR-2); //2*m-1
    hStackRatio_->SetMaximum(maxR*(1+0.3*(maxR-1)/maxR)); //2.1*m-1.1
//    hStackRatio_->GetXaxis()->SetTitle("z_{#parallel}");
    hStackRatio_->GetXaxis()->SetTitle("p_{T, ch. jet}");
    hStackRatio_->GetYaxis()->SetDecimals();
    legRatio_->Draw("same");
    line_->Draw("same");

    cRMS_ = new TCanvas("cRMS_","RMS: "+name_,800,800);
    //hRMS_->SetMaximum(1.1);
    cRMS_->cd();
    cRMS_->cd()->SetLeftMargin(0.15f);
    hRMS_->Draw();

    cMean_ = new TCanvas("cMean_","Mean: "+name_,800,800);
    cMean_->cd();
    cMean_->cd()->SetLeftMargin(0.2f);
    hMean_->Draw();
}

void SysRatio::ResetRun(){
    delete leg_;
    delete legRatio_;
    delete TH1DDefault_;
    delete hStackMain_;
    TH1D_.clear();
    hRatio_.clear();
    TFile_.clear();

    delete hStackRatio_;
    delete hMean_;
    delete hRMS_;
    delete cStack_;
    delete cRMS_;
    delete cMean_;
    if(taskInhibit)taskInhibit=false;
}
