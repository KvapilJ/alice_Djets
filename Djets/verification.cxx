
#include "Preliminaryplots/efficiency/style.C"
#include "TPaveText.h"

void verification(){
    style();

TFile *f5 = new TFile("/home/kvapil/Desktop/cristina/HFPtSpectrum_D0fine_final_10012019.root");
TFile *f13 = new TFile("/home/kvapil/Desktop/cristina/HFPtSpectrum_central_topo.root");
TH1F *h5t = (TH1F*)f5->Get("histoSigmaCorr");

Double_t fptbinsDA[13] = {1,2,3,4,5,6,7,8,10,12,16,24,36};
TH1F *h5 = (TH1F*)(h5t->Rebin(12,"h5",fptbinsDA));
TH1F *hw = (TH1F*)(h5t->Rebin(12,"hw",fptbinsDA));
TH1F *h13 = (TH1F*)f13->Get("histoSigmaCorr");
std::cout<<h13->Integral()<<" "<<h5->Integral()<<std::endl;
h13->SetMarkerColor(kRed);
h5->SetMarkerColor(kBlue);
h13->SetLineColor(kRed);
h5->SetLineColor(kBlue);
hw->SetBinContent(1,2);
hw->SetBinContent(2,2);
hw->SetBinContent(3,2);
hw->SetBinContent(4,2);
hw->SetBinContent(5,2);
hw->SetBinContent(6,2);
hw->SetBinContent(7,2);
hw->SetBinContent(8,2);
hw->SetBinContent(9,1);
hw->SetBinContent(10,1);
hw->SetBinContent(11,1);
hw->SetBinContent(12,1);
hw->SetBinError(1,0);
hw->SetBinError(2,0);
hw->SetBinError(3,0);
hw->SetBinError(4,0);
hw->SetBinError(5,0);
hw->SetBinError(6,0);
hw->SetBinError(7,0);
hw->SetBinError(8,0);
hw->SetBinError(9,0);
hw->SetBinError(10,0);
hw->SetBinError(11,0);
hw->SetBinError(12,0);
h5->Divide(hw);
h13->Divide(h5);

TFile *f13jet = new TFile("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts_PRELIMINARY/Default_AnalysisResults_Run2.root/unfolding_Bayes_5/finalSpectra/JetPtSpectrum_final.root");
TFile *f5jet = new TFile("/home/kvapil/Desktop/cristina/JetPtSpectrum_final.root");
TH1F *h13jettmp = (TH1F*)f13jet->Get("hData_binned");
TH1F *h5jet = (TH1F*)f5jet->Get("hData_binned");
std::cout<<h13jettmp->Integral(1,8)<<" "<<h5jet->Integral(1,7)<<std::endl;
Double_t fptbinsDA2[8] = {5,6,8,10,14,20,30,50};
//TH1F *h13jetsystmp = (TH1F*)h13jettmp->Clone("13systmp");
TH1F *h5jetsys = (TH1F*)h5jet->Clone("5sys");

Double_t sys13[7]={0.119,0.123,0.135,0.15,0.177,0.254,0.276};
Double_t sys5[7]={0.10,0.09,0.11,0.11,0.16,0.18,0.20};



//TH1F *h13jetsys = (TH1F*)(h13jetsystmp->Rebin(7,"h13jetsys",fptbinsDA2));
TH1F *h13jet = (TH1F*)(h13jettmp->Rebin(7,"h13jet",fptbinsDA2));
TH1F *hw2 = (TH1F*)(h13jettmp->Rebin(7,"hw2",fptbinsDA2));
hw2->SetBinContent(1,1);
hw2->SetBinContent(2,1);
hw2->SetBinContent(3,1);
hw2->SetBinContent(4,2);
hw2->SetBinContent(5,1);
hw2->SetBinContent(6,1);
hw2->SetBinContent(7,1);
hw2->SetBinError(1,0);
hw2->SetBinError(2,0);
hw2->SetBinError(3,0);
hw2->SetBinError(4,0);
hw2->SetBinError(5,0);
hw2->SetBinError(6,0);
hw2->SetBinError(7,0);
h13jet->Divide(hw2);
//h13jetsys->Divide(hw2);

TH1F *h13jetsys = (TH1F*)h13jet->Clone("h13jetsys");
for(int i = 1; i<8;i++){
    h13jetsys->SetBinError(i,sys13[i-1]*h13jetsys->GetBinContent(i));
    std::cout<<i<<" "<<sys13[i-1]<<std::endl;
}
for(Int_t i = 1; i<=7;i++){
    h5jetsys->SetBinError(i,sys5[i-1]*h5jetsys->GetBinContent(i));
    std::cout<<i<<std::endl;
}

h13jet->SetMarkerColor(kRed);
//h5->SetMarkerColor(kRed);
h13jet->Divide(h5jet);
h13jetsys->Divide(h5jetsys);
//h2->Draw();
for(int i = 1; i<8;i++){
    //h13jetsystmp->SetBinError(i,sys13[i-1]*h13jetsystmp->GetBinContent(i));
    std::cout<<i<<" "<<h13jetsys->GetBinError(i)/h13jetsys->GetBinContent(i)<<std::endl;
}
//h2->Draw();
//h5->Draw("same");

TFile *sim[2][9];
TFile *simJ[2][9];
TH1D *hsimD[2][9];
TH1D *hsimDjet[2][9];
TCanvas *c = new TCanvas("c","c",1000,800);
//c->Divide(2,1);
TH1D *hDmax;
TH1D *hDmin;
TH1D *hDmaxjet;
TH1D *hDminjet;

simJ[0][0] = new TFile("/home/kvapil/Desktop/cristina/Prompt13/central.root","read");
simJ[0][1] = new TFile("/home/kvapil/Desktop/cristina/Prompt13/F05R05.root","read");
simJ[0][2] = new TFile("/home/kvapil/Desktop/cristina/Prompt13/F05R1.root","read");
simJ[0][3] = new TFile("/home/kvapil/Desktop/cristina/Prompt13/F1R05.root","read");
simJ[0][4] = new TFile("/home/kvapil/Desktop/cristina/Prompt13/F1R2.root","read");
simJ[0][5] = new TFile("/home/kvapil/Desktop/cristina/Prompt13/F2R1.root","read");
simJ[0][6] = new TFile("/home/kvapil/Desktop/cristina/Prompt13/F2R2.root","read");
simJ[0][7] = new TFile("/home/kvapil/Desktop/cristina/Prompt13/m13.root","read");
simJ[0][8] = new TFile("/home/kvapil/Desktop/cristina/Prompt13/m17.root","read");

simJ[1][0] = new TFile("/home/kvapil/Desktop/cristina/Prompt5/central.root","read");
simJ[1][1] = new TFile("/home/kvapil/Desktop/cristina/Prompt5/F05R05.root","read");
simJ[1][2] = new TFile("/home/kvapil/Desktop/cristina/Prompt5/F05R1.root","read");
simJ[1][3] = new TFile("/home/kvapil/Desktop/cristina/Prompt5/F1R05.root","read");
simJ[1][4] = new TFile("/home/kvapil/Desktop/cristina/Prompt5/F1R2.root","read");
simJ[1][5] = new TFile("/home/kvapil/Desktop/cristina/Prompt5/F2R1.root","read");
simJ[1][6] = new TFile("/home/kvapil/Desktop/cristina/Prompt5/F2R2.root","read");
simJ[1][7] = new TFile("/home/kvapil/Desktop/cristina/Prompt5/m13.root","read");
simJ[1][8] = new TFile("/home/kvapil/Desktop/cristina/Prompt5/m17.root","read");


for(Int_t i = 0; i<9;i++){
   sim[0][i] = new TFile(Form("/home/kvapil/Desktop/cristina/13/%d.root",i),"read");
    sim[1][i] = new TFile(Form("/home/kvapil/Desktop/cristina/5/%d.root",i),"read");
    hsimD[0][i] = dynamic_cast<TH1D*>(sim[0][i]->Get("hPt"));
    hsimD[1][i] = dynamic_cast<TH1D*>(sim[1][i]->Get("hPt"));
    TH1D *tm1 = dynamic_cast<TH1D*>(simJ[0][i]->Get("hPt"));
    TH1D *tm2 = dynamic_cast<TH1D*>(simJ[1][i]->Get("hPt"));
    hsimDjet[0][i] = (TH1D*)(tm1->Rebin(7,Form("13%d",i),fptbinsDA2));
    hsimDjet[1][i] = (TH1D*)(tm2->Rebin(7,Form("5%d",i),fptbinsDA2));
    hsimD[0][i]->Divide(hsimD[1][i]);
    hsimDjet[0][i]->Divide(hsimDjet[1][i]);
    if(i==0){
       hDmax =  dynamic_cast<TH1D*>(hsimD[0][0]->Clone("hDmax"));
       hDmin =  dynamic_cast<TH1D*>(hsimD[0][0]->Clone("hDmin"));
       hDmaxjet =  dynamic_cast<TH1D*>(hsimDjet[0][0]->Clone("hDmaxjet"));
       hDminjet =  dynamic_cast<TH1D*>(hsimDjet[0][0]->Clone("hDminjet"));
    }else{
        for(Int_t bin = 1; bin <= hsimD[0][i]->GetXaxis()->GetNbins();bin++){
            if(hsimD[0][i]->GetBinContent(bin) > hDmax->GetBinContent(bin))
                hDmax->SetBinContent(bin,hsimD[0][i]->GetBinContent(bin));
            if(hsimD[0][i]->GetBinContent(bin) < hDmin->GetBinContent(bin))
                hDmin->SetBinContent(bin,hsimD[0][i]->GetBinContent(bin));
        }
        for(Int_t bin = 1; bin <= hsimDjet[0][i]->GetXaxis()->GetNbins();bin++){
            if(hsimDjet[0][i]->GetBinContent(bin) > hDmaxjet->GetBinContent(bin))
                hDmaxjet->SetBinContent(bin,hsimDjet[0][i]->GetBinContent(bin));
            if(hsimDjet[0][i]->GetBinContent(bin) < hDminjet->GetBinContent(bin))
                hDminjet->SetBinContent(bin,hsimDjet[0][i]->GetBinContent(bin));
        }
    }
    //c->cd(1);
    //if(i==0)hsimD[0][i]->Draw();
    //else hsimD[0][i]->Draw("same");
}/*
hsimD[0][0]->SetMarkerColor(kBlue);
hsimD[0][0]->SetMarkerStyle(22);
c->cd(1);
hsimD[0][0]->Draw("P");
hDmax->Draw("sameL");
hDmin->Draw("sameL");*/

hsimD[0][0]->SetTitle("D13/D5");
hsimDjet[0][0]->SetTitle("Djet13/Djet5");
hsimD[0][0]->GetYaxis()->SetRangeUser(1,15);
hsimD[0][0]->SetMarkerStyle(21);
hsimD[0][0]->SetLineColor(kWhite);
hsimD[0][0]->SetMarkerColor(kBlue+1);
//c->cd(1);
/*
hsimD[0][0]->Draw("");
hDmax->Draw("same hist");
hDmin->Draw("same hist");
hsimD[0][0]->Draw("Psame");
h13->Draw("same");*/
//h13jetsys->Draw();
//h5jetsys->Draw("same");
h13jetsys->GetXaxis()->SetTitle("#it{p}_{T,jet}^{ch} (GeV/#it{c})");
c->cd();
h13jet->SetLineColor(kRed+1);
h13jet->SetMarkerColor(kRed+1);
hsimDjet[0][0]->SetMarkerStyle(4);
hsimDjet[0][0]->SetLineWidth(2);
hsimDjet[0][0]->SetMarkerColor(kBlue+1);
hsimDjet[0][0]->SetLineColorAlpha(kWhite,0);
hsimDjet[0][0]->GetYaxis()->SetRangeUser(1,10);
h13jetsys->GetYaxis()->SetRangeUser(1,15);
h13jetsys->GetYaxis()->SetTitle("Ratio 13 TeV/5 TeV");
h13jetsys->GetZaxis()->SetTitleOffset(1.7);
//h13jetsys->SetFillStyle(3344);
h13jetsys->SetFillColor(TColor::GetColor("#cccccc"));
h13jetsys->SetLineColor(TColor::GetColor("#cccccc"));
h13jetsys->Draw("E2");
hDminjet->SetLineWidth(2);
hDmaxjet->SetLineWidth(2);
//hsimDjet[0][0]->Draw("Psame");
hDmaxjet->Draw("same hist");
hDminjet->Draw("same hist");
hsimDjet[0][0]->Draw("hist P same");
h13jet->Draw("same");

TPaveText *pt = nullptr;
pt = new TPaveText(0.15,0.7,0.8,0.9,"NB NDC");
pt->SetBorderSize(0);
pt->SetFillStyle(0);
pt->SetTextAlign(13);
pt->SetTextFont(43);
pt->SetTextSize(22);

TPaveText *pt2 = nullptr;
pt2 = new TPaveText(0.15,0.87,0.8,0.97,"NB NDC");
pt2->SetBorderSize(0);
pt2->SetFillStyle(0);
pt2->SetTextAlign(13);
pt2->SetTextFont(43);
pt2->SetTextSize(30);

TText *text = new TText;
text = pt2->AddText("ALICE Preliminary");
//text = pt->AddText("pp, #sqrt{#it{s}} = 13 TeV and #sqrt{#it{s}} = 5.02 TeV");
text = pt->AddText(Form("charged jets, anti-#it{k}_{T}, #it{R} = 0.%d, |#it{#eta}_{lab}^{jet}| < 0.%d",4,9-4));
text = pt->AddText(Form ("pp, #sqrt{#it{s}} = 13 TeV with D^{0}, %d < #it{p}_{T,D^{0}} < %d GeV/#it{c}",2,36));
text = pt->AddText(Form ("pp, #sqrt{#it{s}} = 5.02 TeV with D^{0}, %d < #it{p}_{T,D^{0}} < %d GeV/#it{c}",3,36));
pt->Draw();
pt2->Draw();

TLegend *leg = nullptr;
 leg = new TLegend(0.15,0.43,0.7,0.67,nullptr,"NB NDC");
leg->SetBorderSize(0);
leg->SetTextFont(43);
leg->SetTextSize(23);
leg->SetLineColor(1);
leg->SetLineStyle(1);
leg->SetLineWidth(1);
leg->SetFillColor(0);
leg->SetFillStyle(0);
leg->AddEntry(h13jet,"13 TeV / 5 TeV","P");
leg->AddEntry(h13jetsys,"Syst. unc. (data)","f");
leg->AddEntry(hsimDjet[0][0],"POWHEG+PYTHIA6","P");
leg->AddEntry(hDmaxjet,"Syst. unc. (theory)","f");

leg->Draw("same");


c->SaveAs("plot.png");
/*
TFile *cu = new TFile("/home/kvapil/Desktop/cristina/D0toKpiCutsNewCut_D0kint7_finerPtBins.root","read");
AliRDHFCuts * cuts = dynamic_cast<AliRDHFCuts*>(cu->Get("D0toKpiCuts"));*/
/*Double_t bi[7]={1,2,4,6,8,12,24};

TH1F *h = new TH1F("h","h",6,bi);
TH1F *hTrEff = new TH1F("hTrEff","hTrEff",6,bi);
Double_t trco[6] = {0.5,0.5,0.5,1,1,0.5};
Double_t meco[6] = {1.8,2.7,2.3,2.4,3,2.9};

for(Int_t i = 1; i<=6;i++){
    h->SetBinContent(i,meco[i-1]);
    hTrEff->SetBinContent(i,trco[i-1]);
}
TFile *fil = new TFile("D0pp13TeVME.root","recreate");
fil->cd();
h->Write("h");
TFile *fil2 = new TFile("D0pp13TeVTrkEff.root","recreate");
fil2->cd();
hTrEff->Write("hTrEff");//hTrEff
fil->Close();
fil2->Close();
*/
/*
std::cout<<"A"<<std::endl;
AliAnalysisTaskTrackingSysPropagation *prop = new AliAnalysisTaskTrackingSysPropagation(AliAnalysisTaskTrackingSysPropagation::DecChannel::kD0toKpi,cuts,me,tr);
std::cout<<"B"<<std::endl;
prop->SetMaximumPt(24);
prop->SetDebugLevel(1);
std::cout<<"C"<<std::endl;
prop->UserCreateOutputObjects();
std::cout<<"D"<<std::endl;
prop->Init();
std::cout<<"E"<<std::endl;
prop->UserExec(nullptr);
std::cout<<"F"<<std::endl;
prop->Terminate(nullptr);
std::cout<<"G"<<std::endl;


*/



}
