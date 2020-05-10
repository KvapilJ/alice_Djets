//-----------------------------------------------------------------------
//  Author B.Trzeciak
//  Utrecht University
//  barbara.antonina.trzeciak@cern.ch
//-----------------------------------------------------------------------

#include "config.h"
#include "../Preliminaryplots/FD/style.C"

double jetEta = 0.9 - fRpar;
double dy = 2*jetEta;

double *sysCutVar, *systuncP;
double DTrackEff = 100;
double globalUnc = 100;
//int xAxisBins;
Double_t *xAxis;
double plotmin = 0.4, plotmax=1;

Color_t colors[] = {1,8,4,2,6,kOrange-1,kGray+2,kCyan+1,kMagenta+2,kViolet+5,kYellow+2};
Int_t markers[] = {20,21,22,23,24,25,26,27,28,29,30,32,33,34};
Style_t linestyle[] = {1,2,3,4,5,6,7,8,9,10,11,12,13};

TGraphAsymmErrors *grsystheory, *grsys, *grsysRatio, *grsystheoryratio;
TH1D *hData_binned=nullptr;
TH1D *hData_binned_ratio;
TH1D *hPrompt_central_binned, *hPrompt_up, *hPrompt_down;

void ScaleHist(TH1 *hh, int full = 0);
void setHistoDetails(TH1 *hh, Color_t color, Style_t Mstyle, Double_t Msize = 1.1, Width_t Lwidth = 2, Style_t Lstyle = 1);
void SaveCanvas(TCanvas *c, TString name = "tmp");
void getSystematics(TString inDir, TString outPlotDir);
TH1* GetUpSys(TH1D **hh, const int nFiles = 11, TH1D *hh_up = nullptr);
TH1* GetDownSys(TH1D **hh, const int nFiles = 11, TH1D *hh_down = nullptr);
TH1* GetInputHist(TString inFile, TString histName,TH1 *hh);
void ScaleHist(TH1 *hh, int full);
void drawFinal(TString outPlotDir);

TH1D *CentralPointsStatisticalUncertainty__4;
TH1D *GeneratorLevel_JetPtSpectrum__3;
Int_t fBin;
Int_t xAxisBins;

bool isSimSys = 1, isSys = 1, isSim = 1;
TString sysUncDir;

Bool_t pdf = 0;
Int_t binMin = 1; //0.2
//Int_t binMax = 7; //0.2
void finalJetSpectraInv(
TString dataFile = "/home/basia/Work/alice/analysis/pPb_run2/DzeroR03_RefDPt3PythiaEff_BaseCuts/Default_jetMeas3_50_jetTrue3_50_ppbinning/unfolding_Bayes_3/unfoldedSpectrum_unfoldedJetSpectrum.root",
TString dataAnalysisFile = "/home/basia/Work/alice/analysis/pPb_run2/D0jet/outData/AnalysisResults_LHC16R03.root",
TString simDir = "/home/basia/Work/alice/analysis/pPb_run2/DzeroR03_RefDPt3PythiaEff_BaseCuts/Simulations/Prompt",
TString outSpectraDir = "/home/basia/Work/alice/analysis/pPb_run2/DzeroR03_RefDPt3PythiaEff_BaseCuts/Default_jetMeas3_50_jetTrue3_50_ppbinning/unfolding_Bayes_3/finalSpectra",
TString sysDir = "/home/basia/Work/alice/analysis/pPb_run2/DzeroR03_RefDPt3PythiaEff_BaseCuts/Default_jetMeas3_50_jetTrue3_50_ppbinning/systematics",
bool issys = 1,
bool issim = 1,
bool simsys = 1,
TString listName = "",
Int_t zBin = 0,
Bool_t is2D = 0,
bool oldCounter = 0,
TString histBase = "unfoldedSpectrum"

)
{

    fBin=zBin;
    isSimSys = simsys;
    isSys = issys;
    isSim = issim;
    sysUncDir = sysDir;
    TString outPlotDir = outSpectraDir;
    outPlotDir+="/plots";
    outPlotDir+=(zBin!=0)?Form("%d",zBin):"";
    gSystem->Exec(Form("mkdir %s",outSpectraDir.Data()));
    gSystem->Exec(Form("mkdir %s",outPlotDir.Data()));
    if(fObservable == kXsection){
      xAxisBins = fptbinsJetFinalN;
    }
    if(fObservable == kFragmentation){
      xAxisBins = fzbinsJetFinalN[fBin-1];
      if(pdf)xAxisBins = xAxisBins - binMin+1;
      if(is2D == true) histBase = Form("unfoldedSpectrumKineEff%d",fBin);
    }

    xAxis = new double[xAxisBins+1];
    if(fObservable == kXsection){
      for(int k=0; k<xAxisBins+1; k++) xAxis[k] = fptbinsJetFinalA[k];
    }
    if(fObservable == kFragmentation){
      for(int k=0; k<xAxisBins+1; k++){
          if(pdf) xAxis[k] = fzbinsJetFinalA[fBin-1][k+binMin-1];
          else xAxis[k] = fzbinsJetFinalA[fBin-1][k];
      }
    }

    systuncP = new double[xAxisBins];
    //r=0.2
 /*   systuncP[0] = 0.10;
    systuncP[1] = 0.10;
    systuncP[2] = 0.106;
    systuncP[3] = 0.112;
    systuncP[4] = 0.13;
    systuncP[5] = 0.148;
    systuncP[6] = 0.20;*/
    //r=0.6
  /*  systuncP[0] = 0.11;
    systuncP[1] = 0.117;
    systuncP[2] = 0.124;
    systuncP[3] = 0.134;
    systuncP[4] = 0.1658;
    systuncP[5] = 0.1911;
    systuncP[6] = 0.305;*/
   // systuncP[7] = 0.001;
   // systuncP[8] = 0.001;
//    for(int k=0; k<xAxisBins; k++) systuncP[k] = 0.15;
//    systuncErr_pp[ptbinsN] = {0.104599235179,0.0886904729946,0.114415907985,0.111763142404,0.15895282319,0.180252600536,0.199602104197};
//    sysUncErr_pp[ptbinsN] = {0.104599395179,0.0886906329946,0.114416067985,0.111763302404,0.15895298319,0.180252760536,0.199602264197};//with BR 0.04% incl
/*systuncP[0]=0.104599395179;
systuncP[1]=0.0886906329946;
systuncP[2]=0.114416067985;
systuncP[3]=0.111763302404;
systuncP[4]=0.15895298319;
systuncP[5]=0.180252760536;
systuncP[6]=0.199602264197;*/
 /*   systuncP[0] = 0.001;
    systuncP[1] = 0.001;
    systuncP[2] = 0.001;
    systuncP[3] = 0.001;
    systuncP[4] = 0.001;
    systuncP[5] = 0.001;
    systuncP[6] = 0.001;*/
  /*  systuncP[0] =0.1008563335;
    systuncP[1] =0.08368990381;
    systuncP[2] =0.0990403958;
    systuncP[3] =0.1092336944;
    systuncP[4] =0.1310496089;
    systuncP[5] =0.1497631463;
    systuncP[6] =0.2303215144;
    systuncP[7] =0.258096881;*/

  /*  systuncP[0] =0.1130840395;
    systuncP[1] =0.09808159868;
    systuncP[2] =0.111467484;
    systuncP[3] =0.1206150903;
    systuncP[4] =0.1406769349;
    systuncP[5] =0.1582561215;
    systuncP[6] =0.2359321936;
    systuncP[7] =0.263115944;*/

 /*   systuncP[0]=0.107713741;
    systuncP[1]=0.113120511;
    systuncP[2]=0.1250809738;
    systuncP[3]=0.1333913415;
    systuncP[4]=0.1512316105;
    systuncP[5]=0.1694609099;
    systuncP[6]=0.2517498759;
    systuncP[7]=0.48723403;*/
/*
    systuncP[0]=0.108010416;
    systuncP[1]=0.11375522;
    systuncP[2]=0.126100159;
    systuncP[3]=0.134882356;
    systuncP[4]=0.15328405;
    systuncP[5]=0.172644143;
    systuncP[6]=0.2566671;
    systuncP[7]=0.494086025;
*/
    systuncP[0]=0.1186433732;
    systuncP[1]=0.1233541649;
    systuncP[2]=0.1345037174;
    systuncP[3]=0.1418705396;
    systuncP[4]=0.1602310831;
    systuncP[5]=0.1768756625;
    systuncP[6]=0.2544523531;
    systuncP[7]=0.2759402109;
/*
if(fBin ==2){
        systuncP[0]=0.53;
        systuncP[1]=0.21;
        systuncP[2]=0.17;
        systuncP[3]=0.15;
        systuncP[4]=0.14;
        systuncP[5]=0.12;
        systuncP[6]=0.11;
}else if(fBin ==3){
    systuncP[0]=0.34;
    systuncP[1]=0.37;
    systuncP[2]=0.24;
    systuncP[3]=0.15;
    systuncP[4]=0.10;
    systuncP[5]=0.10;
    systuncP[6]=0.10;
} else if(fBin ==4){
    systuncP[0]=0.63;
    systuncP[1]=0.47;
    systuncP[2]=0.20;
    systuncP[3]=0.11;
    systuncP[4]=0.10;
    systuncP[5]=0.09;
    systuncP[6]=0.10;
}else if(fBin ==5){
    systuncP[0]=0.97;
    systuncP[1]=0.40;
    systuncP[2]=0.16;
    systuncP[3]=0.11;
    systuncP[4]=0.12;
    systuncP[5]=0.11;
    systuncP[6]=0.11;
}*/

    //R z prelim
/*
    if(!pdf){
if(fBin ==1){
        systuncP[0]=0.01;
        systuncP[1]=0.01;
        systuncP[2]=0.01;
        systuncP[3]=0.01;
        systuncP[4]=0.01;
        systuncP[5]=0.01;}
else if(fBin ==2){
        systuncP[0]=0.30;
        systuncP[1]=0.23;
        systuncP[2]=0.17;
        systuncP[3]=0.12;
        systuncP[4]=0.09;
        systuncP[5]=0.10;
        systuncP[6]=0.11;
}else if(fBin ==3){
    systuncP[0]=0.31;
    systuncP[1]=0.36;
    systuncP[2]=0.17;
    systuncP[3]=0.11;
    systuncP[4]=0.09;
    systuncP[5]=0.09;
    systuncP[6]=0.10;
} else if(fBin ==4){
    systuncP[0]=0.41;
    systuncP[1]=0.16;
    systuncP[2]=0.11;
    systuncP[3]=0.09;
    systuncP[4]=0.08;
    systuncP[5]=0.10;
}else if(fBin ==5){
    systuncP[0]=0.40;
    systuncP[1]=0.15;
    systuncP[2]=0.11;
    systuncP[3]=0.10;
    systuncP[4]=0.09;
    systuncP[5]=0.10;
}
    }
    else{
        if(fBin ==1){
                systuncP[0]=0.01;
                systuncP[1]=0.01;
                systuncP[2]=0.01;
                systuncP[3]=0.01;
                systuncP[4]=0.01;
                systuncP[5]=0.01;}
        else if(fBin ==2){
                systuncP[0]=0.29;
                systuncP[1]=0.22;
                systuncP[2]=0.16;
                systuncP[3]=0.10;
                systuncP[4]=0.08;
                systuncP[5]=0.08;
                systuncP[6]=0.09;
        }else if(fBin ==3){
            systuncP[0]=0.31;
            systuncP[1]=0.36;
            systuncP[2]=0.16;
            systuncP[3]=0.09;
            systuncP[4]=0.07;
            systuncP[5]=0.07;
            systuncP[6]=0.08;
        } else if(fBin ==4){
            systuncP[0]=0.41;
            systuncP[1]=0.15;
            systuncP[2]=0.09;
            systuncP[3]=0.07;
            systuncP[4]=0.06;
            systuncP[5]=0.09;
        }else if(fBin ==5){
            systuncP[0]=0.39;
            systuncP[1]=0.15;
            systuncP[2]=0.10;
            systuncP[3]=0.09;
            systuncP[4]=0.08;
            systuncP[5]=0.09;
        }
    }
*/
//prelim end here


//R=0.2
/*
    if(fBin ==2){
            systuncP[0]=0.22;
            systuncP[1]=0.175;
            systuncP[2]=0.095;
            systuncP[3]=0.089;
            systuncP[4]=0.099;
    }else if(fBin ==3){
        systuncP[0]=0.84;
        systuncP[1]=0.138;
        systuncP[2]=0.105;
        systuncP[3]=0.098;
        systuncP[4]=0.098;
    } else if(fBin ==4){
        systuncP[0]=0.53;
        systuncP[1]=0.13;
        systuncP[2]=0.096;
        systuncP[3]=0.098;
        systuncP[4]=0.098;
    }else if(fBin ==5){
        systuncP[0]=0.34;
        systuncP[1]=0.13;
        systuncP[2]=0.096;
        systuncP[3]=0.09;
        systuncP[4]=0.12;
    }
*/
    //R=0.6
     /*   if(fBin ==2){
                systuncP[0]=0.12;
                systuncP[1]=0.13;
                systuncP[2]=0.10;
                systuncP[3]=0.10;
                systuncP[4]=0.10;
        }else if(fBin ==3){
            systuncP[0]=0.13;
            systuncP[1]=0.10;
            systuncP[2]=0.09;
            systuncP[3]=0.094;
            systuncP[4]=0.10;
        } else if(fBin ==4){
            systuncP[0]=0.125;
            systuncP[1]=0.1;
            systuncP[2]=0.12;
            systuncP[3]=0.08;
            systuncP[4]=0.15;
        }else if(fBin ==5){
            systuncP[0]=0.147;
            systuncP[1]=0.105;
            systuncP[2]=0.1;
            systuncP[3]=0.10;
            systuncP[4]=0.109;
        }
*/

    //R=0.4
  /*      if(fBin ==2){
                systuncP[0]=0.186;
                systuncP[1]=0.15;
                systuncP[2]=0.138;
                systuncP[3]=0.116;
                systuncP[4]=0.105;
        }else if(fBin ==3){
            systuncP[0]=0.243;
            systuncP[1]=0.156;
            systuncP[2]=0.101;
            systuncP[3]=0.095;
            systuncP[4]=0.095;
        } else if(fBin ==4){
            systuncP[0]=0.18;
            systuncP[1]=0.105;
            systuncP[2]=0.092;
            systuncP[3]=0.085;
            systuncP[4]=0.096;
        }else if(fBin ==5){
            systuncP[0]=0.20;
            systuncP[1]=0.098;
            systuncP[2]=0.113;
            systuncP[3]=0.095;
            systuncP[4]=0.11;
        }*/



    sysCutVar = new double[xAxisBins];
    for(int k=0; k<xAxisBins; k++) sysCutVar[k] = 0.0;

    TFile *File = new TFile(dataAnalysisFile,"read");
    TDirectoryFile* dir;
    TList *histList;
    double nEv;
std::cout<<"A"<<std::endl;
    if(oldCounter) {
      dir = dynamic_cast<TDirectoryFile*>(File->Get("DmesonsForJetCorrelations"));
      if(fDmesonSpecie) histList = dynamic_cast<TList*>(dir->Get("histosDStarMBN0"));
      else histList = dynamic_cast<TList*>(dir->Get("histosD0MBN0"));
      TH1F* hEvents = dynamic_cast<TH1F*>(histList->FindObject("hstat"));
      double nEvSel = hEvents->GetBinContent(2);
      //double nEvAna = hEvents->GetBinContent(1);
      nEv = nEvScale*nEvSel;
    }
    else {
      //dir = (TDirectoryFile*)File->Get(Form("PWG3_D2H_DmesonsForJetCorrelations%sMBN0",listName.Data()));
      dir = dynamic_cast<TDirectoryFile*>(File->Get("PWG3_D2H_DmesonsForJetCorrelationsMBN0"));
      AliNormalizationCounter *c = dynamic_cast<AliNormalizationCounter*>(dir->Get("NormalizationCounter"));
      nEv = c->GetNEventsForNorm();
    }

    double dataLum = nEv/(sigma_in*1000) ;//Luminosity in mbar
    double simScaling = 1;
    if(fSystem) simScaling = APb/2.;
    else simScaling = 0.5;
    double dataScaling;
    if(fDmesonSpecie) dataScaling = 1. /(BRDstar * dataLum)/2.;
    else dataScaling = 1. /(BRDzero * dataLum)/2.;

    //if(isSys) getSystematics(sysDir,outPlotDir);

    // ----------------- prompt simulation ---------------------
    if(isSim){
      int simNr = 0; // 0 - central value
      int nFiles = fCsimN;
      TH1D *hPrompt[fCsimN];
      TH1D *hPrompt_binned[fCsimN];

      for (int nr=simNr; nr<fCsimN; nr++){
          TString file = simDir;
          file += "/JetPt_";
          file += fRunC[nr];
          if(fObservable == Observable::kXsection){
              file += "_Dpt"; file += fptbinsDA[0]; file += "_"; file += fptbinsDA[fptbinsDN];
          }
          else if(fObservable == Observable::kFragmentation) {
              file += "_Dpt"; file += fzptbinsDPromptA[zBin-1][0]; file += "_"; file += fzptbinsDPromptA[zBin-1][fzptbinsDPromptN[zBin-1]];
              file += "_Jetpt"; file += fzptJetTrueA[zBin-1]; file += "_"; file += fzptJetTrueA[zBin];
          }
          if(fDmesonSpecie) file += "_Dstar";
          else file += "_Dzero";
          file += ".root";
          TH1D *htmp = nullptr;                 
          if(fObservable == kXsection){
              htmp = dynamic_cast<TH1D*>(GetInputHist(file, "hPt", htmp));
              htmp->GetYaxis()->SetTitle("d#sigma/dp_{T} (mb)");
              hPrompt[nr] = dynamic_cast<TH1D*>(htmp->Clone(Form("hPrompt_%d",nr)));
              hPrompt_binned[nr] = dynamic_cast<TH1D*>(htmp->Rebin(xAxisBins,Form("hPrompt_binned_%d",nr),xAxis));
          }
          if(fObservable == kFragmentation){
              htmp = dynamic_cast<TH1D*>(GetInputHist(file, "hz", htmp));
              htmp->GetYaxis()->SetTitle("d#sigma/dz (mb)");
              //Int_t nBin = htmp->FindBin(1.0);
              //std::cout<<htmp->GetBinContent(nBin-1)<<" "<<htmp->GetBinContent(nBin)<<std::endl;
              //htmp->SetBinContent(nBin-1,htmp->GetBinContent(nBin-1)+htmp->GetBinContent(nBin));
              //htmp->SetBinContent(nBin,0);
              //std::cout<<htmp->GetBinContent(nBin-1)<<" "<<htmp->GetBinContent(nBin)<<std::endl;
              hPrompt[nr] = dynamic_cast<TH1D*>(htmp->Clone(Form("hFD_%d",nr)));
              hPrompt_binned[nr] = dynamic_cast<TH1D*>(htmp->Rebin(fzbinsJetFinalN[fBin-1],Form("hFD_binned_%d",nr),fzbinsJetFinalA[fBin-1]));
              if(pdf){
                  hPrompt_binned[nr]->Scale(1./hPrompt_binned[nr]->Integral());
                  hPrompt_binned[nr]->Scale(1,"width");
              }
          }
      }
std::cout<<"B"<<std::endl;
      TH1D *htmp = dynamic_cast<TH1D*>(hPrompt_binned[simNr]->Clone("htmp"));
      TH1D *hPrompt_central = dynamic_cast<TH1D*>(htmp->Clone("hPrompt_central"));
      hPrompt_central_binned = dynamic_cast<TH1D*>(htmp->Rebin(xAxisBins,"hPrompt_central_binned",xAxis));
std::cout<<"BA"<<std::endl;
      setHistoDetails(hPrompt_central,4,24);
      setHistoDetails(hPrompt_central_binned,4,24);
      //Double_t sim_central_int = hPrompt_central_binned->Integral();
/*if(pdf){
    hPrompt_central->Scale(1./sim_central_int);
    hPrompt_central_binned->Scale(1./sim_central_int);
    hPrompt_central_binned->Scale(1,"width");
}*/
if(!pdf){
      hPrompt_central->Scale(simScaling);
      hPrompt_central_binned->Scale(simScaling);
      hPrompt_central_binned->Scale(1,"width");
      hPrompt_central_binned->Scale(1./dy);//2*jetEta;
}
std::cout<<"BB"<<std::endl;
      if(isSimSys){
      // ----------------- prompt syst. (rebinned)---------------------
        // get up unc
        hPrompt_up = dynamic_cast<TH1D*>(hPrompt_central_binned->Clone("hPrompt_up"));
        hPrompt_up = dynamic_cast<TH1D*>(GetUpSys(hPrompt_binned,nFiles,hPrompt_up));
        setHistoDetails(hPrompt_up,4,24,0,2,2);
        //Double_t sim_up_int = hPrompt_up->Integral();
 /* if(pdf){
      hPrompt_up->Scale(1./sim_up_int);
      hPrompt_up->Scale(1,"width");
  }*/
  if(!pdf){
        hPrompt_up->Scale(simScaling);
        hPrompt_up->Scale(1,"width");
        hPrompt_up->Scale(1./dy);
  }
        // get down unc
        hPrompt_down = dynamic_cast<TH1D*>(hPrompt_central_binned->Clone("hPrompt_down"));
        hPrompt_down = dynamic_cast<TH1D*>(GetDownSys(hPrompt_binned,nFiles,hPrompt_down));
        setHistoDetails(hPrompt_down,4,24,0,2,2);
       // Double_t sim_down_int = hPrompt_down->Integral();
 /* if(pdf){
      hPrompt_down->Scale(1./sim_down_int);
      hPrompt_down->Scale(1,"width");
  }*/
  if(!pdf){
        hPrompt_down->Scale(simScaling);
        hPrompt_down->Scale(1,"width");
        hPrompt_down->Scale(1./dy);
  }
      }
      std::cout<<"BC"<<std::endl;
    }
std::cout<<"BD"<<std::endl;
    // ----------------- data ---------------------
    TH1D *hData_binned2 = nullptr;
    hData_binned2 = dynamic_cast<TH1D*>(GetInputHist(dataFile, histBase, hData_binned2));
    std::cout<<"getting "<<dataFile<<" "<<histBase<<std::endl;
    if(!hData_binned2)std::cout<<"not histo"<<std::endl;
    hData_binned = dynamic_cast<TH1D*>(hData_binned2->Rebin(xAxisBins,"hData_binned", xAxis));
    Double_t data_int = hData_binned->Integral();
    //std::cout<<hData_binned->GetXaxis()->GetBinLowEdge(binMin)<<" to "<<hData_binned->GetXaxis()->GetBinUpEdge(binMax)<<std::endl;

if(pdf){
  hData_binned->Scale(1./data_int);
  hData_binned->Scale(1,"width");
}else{
    hData_binned->Scale(1,"width");
    hData_binned->Scale(dataScaling);
    hData_binned->Scale(1./dy);
}
    hData_binned->SetTitle("");
    //hData_binned->SetMinimum(1);
    hData_binned->SetMaximum(hData_binned->GetMaximum()*2);
    if(fObservable == Observable::kXsection)hData_binned->GetYaxis()->SetTitle("d^{2}#sigma/dp_{T}d#it{#eta} (mb)");
    if(fObservable == Observable::kFragmentation)hData_binned->GetYaxis()->SetTitle("d^{2}#it{#sigma}/d#it{z}_{#parallel}d#it{#eta} (mb)");
    if(pdf)hData_binned->GetYaxis()->SetTitle("Probability Density");
std::cout<<"BE"<<std::endl;
    Double_t sysunc[xAxisBins];
    Double_t sysuncAbs[xAxisBins];
    Double_t statunc[xAxisBins];
    Double_t value[xAxisBins];
    Double_t ptval[xAxisBins];
    Double_t ptvalunc[xAxisBins];
    std::cout<<"BF"<<std::endl;
    if(isSys) {
      for(int j=0; j<xAxisBins; j++){
              ptval[j] = (xAxis[j]+xAxis[j+1]) / 2.;
              ptvalunc[j] = (xAxis[j+1]-xAxis[j]) / 2.;
              value[j] = hData_binned->GetBinContent(hData_binned->GetXaxis()->FindBin(ptval[j]));
              if(value[j]<1E-9)value[j]=1000;
              Double_t error = hData_binned->GetBinError(hData_binned->GetXaxis()->FindBin(ptval[j]));
              std::cout<<j<<" "<<ptval[j]<<" "<<ptvalunc[j]<<" "<<value[j]<<" "<<error<<std::endl;
              sysunc[j] =  systuncP[j];
              sysuncAbs[j] = value[j] * systuncP[j];
              if(value[j]>1E-9)statunc[j] = error/ value[j] *100;

            //  cout << j << " sys: " << sysunc[j] << "\t\t stat: " << statunc[j] << endl;
      }
      grsys = new TGraphAsymmErrors(xAxisBins,ptval,value,ptvalunc,ptvalunc,sysuncAbs,sysuncAbs);
    }

std::cout<<"C"<<std::endl;
    Double_t ptvaltheory[xAxisBins];
    if(isSim && isSimSys){
     // Double_t sysuncTheory[xAxisBins];
      Double_t ptvalunctheory[xAxisBins];
      Double_t valuetheory[xAxisBins];
      Double_t valuetheoryerrup[xAxisBins];
      Double_t valuetheoryerrdown[xAxisBins];
      for(int j=0; j<xAxisBins; j++){
              ptvaltheory[j] = (xAxis[j]+xAxis[j+1]) / 2.;
              ptvalunctheory[j] = (xAxis[j+1]-xAxis[j]) / 2.;
              valuetheory[j] = hPrompt_central_binned->GetBinContent(hPrompt_central_binned->GetXaxis()->FindBin(ptvaltheory[j]));
              valuetheoryerrup[j] = hPrompt_up->GetBinContent(hPrompt_up->GetXaxis()->FindBin(ptvaltheory[j])) - valuetheory[j];
              valuetheoryerrdown[j] = valuetheory[j] - hPrompt_down->GetBinContent(hPrompt_up->GetXaxis()->FindBin(ptvaltheory[j]));

      }
      grsystheory = new TGraphAsymmErrors(xAxisBins,ptvaltheory,valuetheory,ptvalunctheory,ptvalunctheory,valuetheoryerrdown,valuetheoryerrup);
    }

   //======= Ratio to powheg ======
    TH1D *hPrompt_central_binned_ratio = nullptr;
    TH1D *hPrompt_down_ratio = nullptr;
    TH1D *hPrompt_up_ratio = nullptr;
     if(isSim){    
       hPrompt_central_binned_ratio  = dynamic_cast<TH1D*>(hPrompt_central_binned->Clone("hPrompt_central_binned_ratio"));
       hPrompt_central_binned_ratio->Divide(hData_binned);
       if(isSimSys){
         hPrompt_up_ratio = dynamic_cast<TH1D*>(hPrompt_up->Clone("hPrompt_up_ratio"));
         hPrompt_down_ratio = dynamic_cast<TH1D*>(hPrompt_down->Clone("hPrompt_down_ratio"));
         hPrompt_up_ratio->Divide(hData_binned);
         hPrompt_down_ratio->Divide(hData_binned);
       }
     }
       hData_binned_ratio = dynamic_cast<TH1D*>(hData_binned->Clone("hData_binned_ratio"));
       if(isSys){
         double *sysuncRatio = new double[xAxisBins];
         double *valRatio = new double[xAxisBins];
         for(int j=0; j<xAxisBins; j++){
                 double pt = (xAxis[j]+xAxis[j+1]) / 2.;
                 double val = hData_binned->GetBinContent(hData_binned->GetXaxis()->FindBin(pt));
                 double valPred;
                 if(isSim) valPred = hData_binned->GetBinContent(hData_binned->GetXaxis()->FindBin(pt));
                 else valPred = hData_binned->GetBinContent(hData_binned->GetXaxis()->FindBin(pt));
                 if(val<1E-9)val=1000000;
                 if(valPred<1E-9)valPred=1000;
                 if(valPred>1E-9)valRatio[j] = val / valPred;
                 double err = 0;
                 if(valPred>1E-9) err=hData_binned->GetBinError(hData_binned->GetXaxis()->FindBin(pt)) / valPred;
                 //err = err * valRatio[j];
                 sysuncRatio[j] = sysunc[j]*valRatio[j];

                 hData_binned_ratio->SetBinContent(hData_binned_ratio->GetXaxis()->FindBin(pt),valRatio[j]);
                 hData_binned_ratio->SetBinError(hData_binned_ratio->GetXaxis()->FindBin(pt),err);
         }
         grsysRatio = new TGraphAsymmErrors(xAxisBins,ptval,valRatio,ptvalunc,ptvalunc,sysuncRatio,sysuncRatio);
       }
       //hData_binned_ratio->SetMaximum(10);
       if(zBin==2)hData_binned_ratio->SetMaximum(10);
       if(zBin==3)hData_binned_ratio->SetMaximum(5);
       if(zBin==4)hData_binned_ratio->SetMaximum(10);
       if(zBin==5)hData_binned_ratio->SetMaximum(4);
       if(zBin==2)hData_binned_ratio->GetYaxis()->SetRangeUser(0,10);
       if(zBin==3)hData_binned_ratio->GetYaxis()->SetRangeUser(0,5);
       if(zBin==4)hData_binned_ratio->GetYaxis()->SetRangeUser(0,10);
       if(zBin==5)hData_binned_ratio->GetYaxis()->SetRangeUser(0,5);

       if(isSimSys){
        //double *sysuncTheoryratio = new double[xAxisBins];
        double *ptvaltheoryratio = new double[xAxisBins];
        double *ptvalunctheoryratio = new double[xAxisBins];
        double *valuetheoryratio = new double[xAxisBins];
        double *valuetheoryerrupratio = new double[xAxisBins];
        double *valuetheoryerrdownratio = new  double[xAxisBins];
        for(int j=0; j<xAxisBins; j++){
              ptvaltheoryratio[j] = (xAxis[j]+xAxis[j+1]) / 2.;
              ptvalunctheoryratio[j] = (xAxis[j+1]-xAxis[j]) / 2.;
              valuetheoryratio[j] = hPrompt_central_binned_ratio->GetBinContent(hPrompt_central_binned_ratio->GetXaxis()->FindBin(ptvaltheory[j]));
              valuetheoryerrupratio[j] = hPrompt_up_ratio->GetBinContent(hPrompt_up_ratio->GetXaxis()->FindBin(ptvaltheory[j])) - valuetheoryratio[j];
              valuetheoryerrdownratio[j] = valuetheoryratio[j] - hPrompt_down_ratio->GetBinContent(hPrompt_down_ratio->GetXaxis()->FindBin(ptvaltheory[j]));
        }
        grsystheoryratio = new TGraphAsymmErrors(xAxisBins,ptvaltheoryratio,valuetheoryratio,ptvalunctheoryratio,ptvalunctheoryratio,valuetheoryerrdownratio,valuetheoryerrupratio);
      }

std::cout<<"D"<<std::endl;
drawFinal(outPlotDir);

TFile *ofile = new TFile(Form("%s/JetPtSpectrum_final%s.root",outSpectraDir.Data(),(zBin!=0)?Form("%d",zBin):""),"RECREATE");
hData_binned->Write();
hData_binned_ratio->Write();
if(isSim){
  hPrompt_central_binned->Write();
  if(isSimSys){
    hPrompt_up->Write();
    hPrompt_down->Write();
    grsystheory->Write();
    grsystheoryratio->Write();
  }
}
if(isSys) grsysRatio->Write();
if(isSys) grsys->Write();

ofile->Close();
std::cout<<"E"<<std::endl;
return;

}

void getSystematics(TString inDir, TString outPlotDir) {

  const int nfiles = 9;
  TString files[nfiles] = {
    "CutVariationSyst_reg0.root",
    "YieldExtraction.root",
    "RawYield_reflections.root",
    "SBRangesComparison.root",
    "FD_reg3.root",
    "JES_reg3.root",
    "PriorComparison_reg3.root",
    "UnfoldingRangesComparison.root",
    "BkgComparison_reg3.root"
  };
  TString histName[nfiles] = {
    "cutSysRMS",
    "sysUnc",
    "hsys",
    "hsys",
    "hDFUnc",
    "hratiof_0",
    "hsys",
    "hsys",
    "hsys"
  };

  TString desc[nfiles+1] = {
    "Cut variation",
    "Raw Yield Extraction",
    "Reflections",
    "SB,Signal ranges",
    "B Feed-down",
    "Track. Eff. (JES)",
    "Unfolding: priors",
    "Unfolding: ranges,SVD",
    "Bkg. Fluctuation Matrix",
    "Track. Eff. (D meson)"
  };

  TCanvas *cUnc = new TCanvas("cUnc","cUnc",1200,800);
  TH1F *hist[nfiles+1];
  double **sysUnc = new double*[xAxisBins];
  for(int i=0; i<xAxisBins; i++)  sysUnc[i] = new double[nfiles+1];

  hist[nfiles] = new TH1F("histUncN","Systematic uncertanties",xAxisBins, xAxis);
  hist[0] = new TH1F("histUnc0","Systematic uncertanties",xAxisBins, xAxis);
//  hist[1] = new TH1F("histUnc1","Systematic uncertanties",xAxisBins, fptbinsJetTrueFinalpp);

  for(int k=0; k<xAxisBins; k++) hist[nfiles]->SetBinContent(k+1,DTrackEff*100);
  hist[nfiles]->SetMarkerColor(colors[nfiles+1]);
  hist[nfiles]->SetLineColor(colors[nfiles+1]);
  hist[nfiles]->SetLineStyle(linestyle[nfiles]);

  for(int j=0; j<xAxisBins; j++) hist[0]->SetBinContent(j+1,sysCutVar[j]);
  //for(int j=0; j<xAxisBins; j++) hist[1]->SetBinContent(j+1,sysRawYield[j]);

  TLegend  *leg = new TLegend(0.45,0.55,0.85,0.85);
  for(int i=0; i<nfiles; i++){
    TFile *fileIn;
//    if(i != 0 && i != 1) { fileIn = new TFile(Form("%s/%s", inDir.Data(),files[i].Data())); hist[i] = (TH1F*)fileIn->Get(Form("%s",histName[i].Data())); }
    if(i != 0 ) { fileIn = new TFile(Form("%s/%s", inDir.Data(),files[i].Data())); hist[i] = dynamic_cast<TH1F*>(fileIn->Get(Form("%s",histName[i].Data()))); }

    //hist[i] = (TH1F*)fileIn->Get(Form("%s",histName[i].Data()));
    hist[i]->SetMarkerColor(colors[i+1]);
    hist[i]->SetLineColor(colors[i+1]);
    hist[i]->SetLineStyle(linestyle[i+1]);
    hist[i]->GetYaxis()->SetTitle("Syst. Uncertainty [%]");
    hist[i]->GetXaxis()->SetRangeUser(5,50);
    hist[i]->GetYaxis()->SetRangeUser(0,30);
    hist[i]->SetFillColor(0);
    hist[i]->SetFillStyle(0);
    if(!i) hist[i]->Draw("hist");
    else hist[i]->Draw("histsame");
    std::cout << "systematics: " << desc[i] << std::endl;
    for(int j=0; j<xAxisBins; j++){
      double pt = (xAxis[j] + xAxis[j+1])/2.;
      int bin = hist[i]->GetXaxis()->FindBin(pt);
      if(bin) sysUnc[j][i] = hist[i]->GetBinContent(bin)*0.01;
      else sysUnc[j][i] = 0;
      std::cout << "pt: " << pt << "\tsys: " << sysUnc[j][i]*100 << std::endl;
    }
    std::cout << "=============END" << std::endl;
      leg->AddEntry(hist[i],Form("%s",desc[i].Data()),"l");
  }
  leg->AddEntry(hist[nfiles],Form("%s",desc[nfiles].Data()),"l");
  hist[nfiles]->Draw("histsame");
  leg->Draw("same");

  cUnc->SaveAs(Form("%s/JetPtSpectra_allUnc.png",outPlotDir.Data()));
  cUnc->SaveAs(Form("%s/JetPtSpectra_allUnc.pdf",outPlotDir.Data()));
  cUnc->SaveAs(Form("%s/JetPtSpectra_allUnc.svg",outPlotDir.Data()));

  TCanvas *cUnc2 = new TCanvas("cUnc2","cUnc2",1200,800);
  TH1D *histUnc = new TH1D("histUnc","Systematic uncertanties",xAxisBins, xAxis);
  histUnc->SetLineColor(2);
  histUnc->SetLineWidth(2);
  histUnc->SetLineStyle(2);
  histUnc->SetTitle("");
  histUnc->GetYaxis()->SetTitle("Final systematic uncertanties");
  if(fObservable == Observable::kXsection)histUnc->GetXaxis()->SetTitle("p_{T}^{ch,jet} (GeV/c)");
  if(fObservable == Observable::kFragmentation)histUnc->GetXaxis()->SetTitle("#it{z}_{#parallel}^{ch,jet}");
  histUnc->GetXaxis()->SetRangeUser(plotmin,plotmax);
  for(int i=0; i<xAxisBins; i++){
    double value = 0;
    for(int j=0; j<nfiles+1; j++){
      value += sysUnc[i][j]*sysUnc[i][j];
    }
    value += DTrackEff*DTrackEff;
    systuncP[i] = TMath::Sqrt(value);
    histUnc->SetBinContent(i+1,systuncP[i]);
    value += globalUnc*globalUnc;
    systuncP[i] = TMath::Sqrt(value);
  }
  histUnc->GetYaxis()->SetRangeUser(0,0.4);
  histUnc->GetXaxis()->SetRangeUser(plotmin,plotmax);
  histUnc->Draw("hist");

  cUnc2->SaveAs(Form("%s/JetPtSpectra_finalUnc.png",outPlotDir.Data()));
  cUnc2->SaveAs(Form("%s/JetPtSpectra_finalUnc.pdf",outPlotDir.Data()));
  cUnc2->SaveAs(Form("%s/JetPtSpectra_finalUnc.svg",outPlotDir.Data()));
}


TH1* GetUpSys(TH1D **hh, const int nFiles, TH1D *hh_up){

        //double bin = 0, binerr = 0;
        double max = 0, maxerr = 0;
        for(int j=1; j<xAxisBins+1; j++ ){
            max = hh[0]->GetBinContent(j);
            for(int i=1;i<nFiles;i++){
                if(hh[i]->GetBinContent(j) > max){
                        max = hh[i]->GetBinContent(j);
                        maxerr = hh[i]->GetBinError(j);
                }
            }
            hh_up->SetBinContent(j,max);
            hh_up->SetBinError(j,0);
        }

    return hh_up;
}

TH1* GetDownSys(TH1D **hh, const int nFiles, TH1D *hh_down){
        //double bin = 0, binerr = 0;
        double max = 0, maxerr = 0;

        for(int j=1; j<xAxisBins+1; j++ ){
      //for(int j=1; j<hh[0]->GetNbinsX()+1; j++ ){
            max = hh[0]->GetBinContent(j);
            for(int i=1;i<nFiles;i++){
                if(hh[i]->GetBinContent(j) < max){
                        max = hh[i]->GetBinContent(j);
                        maxerr = hh[i]->GetBinError(j);
                }
            }
            hh_down->SetBinContent(j,max);
            hh_down->SetBinError(j,0);
        }

    return hh_down;
}



TH1* GetInputHist(TString inFile, TString histName,TH1 *hh){

	TFile *jetPtFile = new TFile(inFile,"read");
  hh = dynamic_cast<TH1*>(jetPtFile->Get(histName.Data()));

  return hh;
}



void ScaleHist(TH1 *hh, int full){
    if(full){
        //hh->Scale(1,"width");
        //hh->Scale(pPbscaling);
        //hh->Scale(scalingF);
         hh->Scale(1,"width");
        hh->GetYaxis()->SetTitle("d^{2}#it{#sigma}/d#it{#eta}d#it{p}_{T} (mb #it{c}/GeV)");
    }
    else {
        hh->Scale(1,"width");
        hh->GetYaxis()->SetTitle("dN/dp_{T} (#it{c}/GeV)");
    }
}

void setHistoDetails(TH1 *hh, Color_t color, Style_t Mstyle, Double_t Msize, Width_t Lwidth, Style_t Lstyle){
    hh->SetMarkerColor(color);
    hh->SetMarkerStyle(Mstyle);;
    hh->SetLineColor(color);
    hh->SetLineWidth(Lwidth);
    hh->SetMarkerSize(Msize);
    hh->SetLineStyle(Lstyle);
   // hh->SetName(name.c_str());
    hh->SetTitle("");
    if(fObservable == Observable::kXsection)hh->GetXaxis()->SetTitle("p_{T}^{ch,jet} (GeV/c)");
    if(fObservable == Observable::kFragmentation)hh->GetXaxis()->SetTitle("#it{z}_{#parallel}^{ch,jet}");
}

void SaveCanvas(TCanvas *c, TString name){
    c->SaveAs(Form("%s.png",name.Data()));
    c->SaveAs(Form("%s.pdf",name.Data()));
    c->SaveAs(Form("%s.svg",name.Data()));
    //c->SaveAs(Form("%s.png",name.c_str()));
    //c->SaveAs(Form("%s.pdf",name.c_str()));
}

void drawFinal(TString outPlotDir){

  //=========Macro generated from canvas: FinalSpectrum/FinalSpectrum
//=========  (Mon Jun 12 17:00:30 2017) by ROOT version5.34/30
   style();
   TCanvas *FinalSpectrum = new TCanvas("FinalSpectrum", "FinalSpectrum",0,45,700,700);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   FinalSpectrum->SetHighLightColor(2);
   FinalSpectrum->Range(0,0,1,1);
   FinalSpectrum->SetFillColor(0);
   FinalSpectrum->SetBorderMode(0);
   FinalSpectrum->SetBorderSize(2);
   FinalSpectrum->SetFrameBorderMode(0);

// ------------>Primitives in pad: FinalSpectrum_1
   TPad *FinalSpectrum_1 = new TPad("FinalSpectrum_1", "FinalSpectrum_1",0,0.35,1,1);
   FinalSpectrum_1->Draw();
   FinalSpectrum_1->cd();
   FinalSpectrum_1->Range(-1.986821e-07,-4.69897,33.33333,0.3499945);
   FinalSpectrum_1->SetFillColor(0);
   FinalSpectrum_1->SetBorderMode(0);
   FinalSpectrum_1->SetBorderSize(2);
   FinalSpectrum_1->SetLogy();
   FinalSpectrum_1->SetTickx(1);
   FinalSpectrum_1->SetTicky(1);
   FinalSpectrum_1->SetLeftMargin(0.15f);
   FinalSpectrum_1->SetBottomMargin(0);
   FinalSpectrum_1->SetFrameBorderMode(0);
   FinalSpectrum_1->SetFrameBorderMode(0);

   TH1D *CentralPointsStatisticalUncertainty__1 = new TH1D("CentralPointsStatisticalUncertainty__1","Central Values",xAxisBins, xAxis);
   //TH1D *CentralPointsStatisticalUncertainty__1 = new TH1D("CentralPointsStatisticalUncertainty__1","Central Values",xAxisBins2, xAxis2);
   if(fSystem){
     CentralPointsStatisticalUncertainty__1->SetMinimum(2.e-04);
     CentralPointsStatisticalUncertainty__1->SetMaximum(500);

   }
   else{
     CentralPointsStatisticalUncertainty__1->SetMinimum(2.e-06);
     CentralPointsStatisticalUncertainty__1->SetMaximum(0.5);
  }
   if(fObservable == Observable::kFragmentation)CentralPointsStatisticalUncertainty__1->SetMaximum(5);
    if(fObservable == Observable::kFragmentation)CentralPointsStatisticalUncertainty__1->SetMinimum(2.e-05);
   CentralPointsStatisticalUncertainty__1->SetEntries(8);
   CentralPointsStatisticalUncertainty__1->SetDirectory(nullptr);
   CentralPointsStatisticalUncertainty__1->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#990000");
   CentralPointsStatisticalUncertainty__1->SetLineColor(ci);

   ci = TColor::GetColor("#990000");
   CentralPointsStatisticalUncertainty__1->SetMarkerColor(ci);
   CentralPointsStatisticalUncertainty__1->SetMarkerStyle(20);
   CentralPointsStatisticalUncertainty__1->SetMarkerSize(0.9f);
   if(fObservable == Observable::kXsection)CentralPointsStatisticalUncertainty__1->GetXaxis()->SetTitle("#it{p}_{T,ch jet} (GeV/#it{c})");
   if(fObservable == Observable::kFragmentation)CentralPointsStatisticalUncertainty__1->GetXaxis()->SetTitle("#it{z}_{#parallel}^{ch,jet}");
   CentralPointsStatisticalUncertainty__1->GetXaxis()->SetLabelFont(42);
   CentralPointsStatisticalUncertainty__1->GetXaxis()->SetLabelSize(0.035f);
   CentralPointsStatisticalUncertainty__1->GetXaxis()->SetTitleSize(0.035f);
   CentralPointsStatisticalUncertainty__1->GetXaxis()->SetTitleFont(42);
   //CentralPointsStatisticalUncertainty__1->GetYaxis()->SetTitle("#frac{d^{2}#sigma}{d#it{p}_{T}d#it{#eta}} [mb (GeV/#it{c})^{-1}]");
   if(fObservable == Observable::kXsection)CentralPointsStatisticalUncertainty__1->GetYaxis()->SetTitle("#frac{d^{2}#sigma}{d#it{p}_{T}d#it{#eta}} mb (GeV/#it{c})^{-1}");
   if(fObservable == Observable::kFragmentation)CentralPointsStatisticalUncertainty__1->GetYaxis()->SetTitle("#frac{d^{2}#it{#sigma}}{d#it{z}_{#parallel}d#it{#eta}} (mb)");
   if(pdf)CentralPointsStatisticalUncertainty__1->GetYaxis()->SetTitle("Probability Density");
   CentralPointsStatisticalUncertainty__1->GetYaxis()->SetLabelFont(43);
   CentralPointsStatisticalUncertainty__1->GetYaxis()->SetLabelSize(22);
   CentralPointsStatisticalUncertainty__1->GetYaxis()->SetTitleSize(26);
   CentralPointsStatisticalUncertainty__1->GetYaxis()->SetTitleOffset(1.6f);
   CentralPointsStatisticalUncertainty__1->GetYaxis()->SetTitleFont(43);
   CentralPointsStatisticalUncertainty__1->GetZaxis()->SetLabelFont(42);
   CentralPointsStatisticalUncertainty__1->GetZaxis()->SetLabelSize(0.035f);
   CentralPointsStatisticalUncertainty__1->GetZaxis()->SetTitleSize(0.035f);
   CentralPointsStatisticalUncertainty__1->GetZaxis()->SetTitleFont(42);
   CentralPointsStatisticalUncertainty__1->Draw("axis");

   // dat syst. unc.
   TGraphAsymmErrors *grae = dynamic_cast<TGraphAsymmErrors*>(grsys->Clone("grae"));
if(isSys){
    // = new TGraphAsymmErrors(6);
   grae->SetName("CentralPointsSystematicUncertainty_copy");
   grae->SetTitle("Bayes, iter=4, prior=ResponseTruth Systematics");

   ci = TColor::GetColor("#cccccc");
   grae->SetFillColor(ci);
   grae->SetLineColor(ci);

   //=== data uncertantity from grae
   TH1F *Graph_central_syst_unc1 = new TH1F("Graph_central_syst_unc1","Bayes, iter=4, prior=ResponseTruth Systematics",100,2.5,52.5);
   Graph_central_syst_unc1->SetMinimum(4.779682e-05);
   Graph_central_syst_unc1->SetMaximum(0.02142993);
   Graph_central_syst_unc1->SetDirectory(nullptr);
   Graph_central_syst_unc1->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_central_syst_unc1->SetLineColor(ci);
   if(fObservable == Observable::kXsection)Graph_central_syst_unc1->GetXaxis()->SetTitle("#it{p}_{T,ch jet} (GeV/#it{c})");
   if(fObservable == Observable::kFragmentation)Graph_central_syst_unc1->GetXaxis()->SetTitle("#it{z}_{#parallel}^{ch,jet}");
   Graph_central_syst_unc1->GetXaxis()->SetLabelFont(42);
   Graph_central_syst_unc1->GetXaxis()->SetLabelSize(0.035f);
   Graph_central_syst_unc1->GetXaxis()->SetTitleSize(0.035f);
   Graph_central_syst_unc1->GetXaxis()->SetTitleFont(42);
   Graph_central_syst_unc1->GetYaxis()->SetTitle("#frac{d^{2}#sigma}{d#it{p}_{T}d#it{#eta}} [mb (GeV/#it{c})^{-1}]");
   Graph_central_syst_unc1->GetYaxis()->SetLabelFont(42);
   Graph_central_syst_unc1->GetYaxis()->SetLabelSize(0.035f);
   Graph_central_syst_unc1->GetYaxis()->SetTitleSize(0.035f);
   Graph_central_syst_unc1->GetYaxis()->SetTitleFont(42);
   Graph_central_syst_unc1->GetZaxis()->SetLabelFont(42);
   Graph_central_syst_unc1->GetZaxis()->SetLabelSize(0.035f);
   Graph_central_syst_unc1->GetZaxis()->SetTitleSize(0.035f);
   Graph_central_syst_unc1->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_central_syst_unc1);
   grae->Draw("2");
 }

   // Central data
   TH1D *CentralPointsStatisticalUncertainty__2 = dynamic_cast<TH1D*>(hData_binned->Clone("CentralPointsStatisticalUncertainty__2")); // = new
   CentralPointsStatisticalUncertainty__2->SetMinimum(2e-05);
   CentralPointsStatisticalUncertainty__2->SetMaximum(4);
   CentralPointsStatisticalUncertainty__2->SetEntries(8);
   CentralPointsStatisticalUncertainty__2->SetDirectory(nullptr);
   CentralPointsStatisticalUncertainty__2->SetStats(0);

   ci = TColor::GetColor("#990000");
   CentralPointsStatisticalUncertainty__2->SetLineColor(ci);
   CentralPointsStatisticalUncertainty__2->SetMarkerColor(ci);
   CentralPointsStatisticalUncertainty__2->SetMarkerStyle(20);
   CentralPointsStatisticalUncertainty__2->SetMarkerSize(0.9f);
   if(fObservable == Observable::kXsection)CentralPointsStatisticalUncertainty__2->GetXaxis()->SetTitle("#it{p}_{T,ch jet} (GeV/#it{c})");
   if(fObservable == Observable::kFragmentation)CentralPointsStatisticalUncertainty__2->GetXaxis()->SetTitle("#it{z}_{#parallel}^{ch,jet}");
   CentralPointsStatisticalUncertainty__2->GetXaxis()->SetLabelFont(42);
   CentralPointsStatisticalUncertainty__2->GetXaxis()->SetLabelSize(0.035f);
   CentralPointsStatisticalUncertainty__2->GetXaxis()->SetTitleSize(0.035f);
   CentralPointsStatisticalUncertainty__2->GetXaxis()->SetTitleFont(42);
   CentralPointsStatisticalUncertainty__2->GetYaxis()->SetTitle("#frac{d^{2}#sigma}{d#it{p}_{T}d#it{#eta}} [mb (GeV/#it{c})^{-1}]");
   CentralPointsStatisticalUncertainty__2->GetYaxis()->SetLabelFont(42);
   CentralPointsStatisticalUncertainty__2->GetYaxis()->SetLabelSize(0.035f);
   CentralPointsStatisticalUncertainty__2->GetYaxis()->SetTitleSize(0.035f);
   CentralPointsStatisticalUncertainty__2->GetYaxis()->SetTitleOffset(1.4f);
   CentralPointsStatisticalUncertainty__2->GetYaxis()->SetTitleFont(42);
   CentralPointsStatisticalUncertainty__2->GetZaxis()->SetLabelFont(42);
   CentralPointsStatisticalUncertainty__2->GetZaxis()->SetLabelSize(0.035f);
   CentralPointsStatisticalUncertainty__2->GetZaxis()->SetTitleSize(0.035f);
   CentralPointsStatisticalUncertainty__2->GetZaxis()->SetTitleFont(42);
   CentralPointsStatisticalUncertainty__2->Draw("same p e0 x0");

   // central theory
   TH1D *GeneratorLevel_JetPtSpectrum__3 =nullptr;
if(isSim){
   GeneratorLevel_JetPtSpectrum__3 = dynamic_cast<TH1D*>(hPrompt_central_binned->Clone("GeneratorLevel_JetPtSpectrum__3")); //  new
   GeneratorLevel_JetPtSpectrum__3->SetEntries(316731);
   GeneratorLevel_JetPtSpectrum__3->SetDirectory(nullptr);
   GeneratorLevel_JetPtSpectrum__3->SetStats(0);

   ci = TColor::GetColor("#000099");
   GeneratorLevel_JetPtSpectrum__3->SetLineColor(ci);

   ci = TColor::GetColor("#000099");
   GeneratorLevel_JetPtSpectrum__3->SetMarkerColor(ci);
   GeneratorLevel_JetPtSpectrum__3->SetMarkerStyle(24);
   GeneratorLevel_JetPtSpectrum__3->SetMarkerSize(1.2f);
   if(fObservable == Observable::kXsection)GeneratorLevel_JetPtSpectrum__3->GetXaxis()->SetTitle("#it{p}_{T,ch jet} (GeV/#it{c})");
   if(fObservable == Observable::kFragmentation)GeneratorLevel_JetPtSpectrum__3->GetXaxis()->SetTitle("#it{z}_{#parallel}^{ch,jet}");
   GeneratorLevel_JetPtSpectrum__3->GetXaxis()->SetLabelFont(42);
   GeneratorLevel_JetPtSpectrum__3->GetXaxis()->SetLabelSize(0.035f);
   GeneratorLevel_JetPtSpectrum__3->GetXaxis()->SetTitleSize(0.035f);
   GeneratorLevel_JetPtSpectrum__3->GetXaxis()->SetTitleFont(42);
   GeneratorLevel_JetPtSpectrum__3->GetYaxis()->SetTitle("#frac{d#sigma}{d#it{p}_{T}} #times #Delta#it{p}_{T} (mb)");
   GeneratorLevel_JetPtSpectrum__3->GetYaxis()->SetLabelFont(42);
   GeneratorLevel_JetPtSpectrum__3->GetYaxis()->SetLabelSize(0.035f);
   GeneratorLevel_JetPtSpectrum__3->GetYaxis()->SetTitleSize(0.035f);
   GeneratorLevel_JetPtSpectrum__3->GetYaxis()->SetTitleFont(42);
   GeneratorLevel_JetPtSpectrum__3->GetZaxis()->SetLabelFont(42);
   GeneratorLevel_JetPtSpectrum__3->GetZaxis()->SetLabelSize(0.035f);
   GeneratorLevel_JetPtSpectrum__3->GetZaxis()->SetTitleSize(0.035f);
   GeneratorLevel_JetPtSpectrum__3->GetZaxis()->SetTitleFont(42);
   GeneratorLevel_JetPtSpectrum__3->Draw("same p e0 x0");

   // theory syst unc
   if(isSimSys){
     grae = dynamic_cast<TGraphAsymmErrors*>(grsystheory->Clone("grae")); //  new TGraphAsymmErrors(6);
     grae->SetName("theorySyst_copy");
     grae->SetTitle("Graph");
     grae->SetFillColor(1);
     grae->SetFillStyle(0);

     ci = TColor::GetColor("#000099");
     grae->SetLineColor(ci);
     grae->SetLineWidth(2);
     TH1F *Graph_theorySyst_copy2 = new TH1F("Graph_theorySyst_copy2","Graph",100,2.5,52.5);
     Graph_theorySyst_copy2->SetMinimum(2.864129e-05);
     Graph_theorySyst_copy2->SetMaximum(0.01825038);
     Graph_theorySyst_copy2->SetDirectory(nullptr);
     Graph_theorySyst_copy2->SetStats(0);

     ci = TColor::GetColor("#000099");
     Graph_theorySyst_copy2->SetLineColor(ci);
     Graph_theorySyst_copy2->GetXaxis()->SetLabelFont(42);
     Graph_theorySyst_copy2->GetXaxis()->SetLabelSize(0.035f);
     Graph_theorySyst_copy2->GetXaxis()->SetTitleSize(0.035f);
     Graph_theorySyst_copy2->GetXaxis()->SetTitleFont(42);
     Graph_theorySyst_copy2->GetYaxis()->SetLabelFont(42);
     Graph_theorySyst_copy2->GetYaxis()->SetLabelSize(0.035f);
     Graph_theorySyst_copy2->GetYaxis()->SetTitleSize(0.035f);
     Graph_theorySyst_copy2->GetYaxis()->SetTitleFont(42);
     Graph_theorySyst_copy2->GetZaxis()->SetLabelFont(42);
     Graph_theorySyst_copy2->GetZaxis()->SetLabelSize(0.035f);
     Graph_theorySyst_copy2->GetZaxis()->SetTitleSize(0.035f);
     Graph_theorySyst_copy2->GetZaxis()->SetTitleFont(42);
     grae->SetHistogram(Graph_theorySyst_copy2);
     grae->Draw("2");
  }
}
   TLegend *leg = nullptr;
   if(fObservable==Observable::kXsection) leg = new TLegend(0.5,0.4,0.8,0.65,nullptr,"NB NDC");
   if(fObservable==Observable::kFragmentation) leg = new TLegend(0.5,0.1,0.8,0.35,nullptr,"NB NDC");
   leg->SetBorderSize(0);
   leg->SetTextFont(43);
   leg->SetTextSize(23);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(0);
   TLegendEntry *entry=leg->AddEntry("CentralPointsStatisticalUncertainty","Data","p");
   ci = TColor::GetColor("#990000");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(20);
   entry->SetMarkerSize(0.9f);
   entry->SetTextFont(43);

   if(isSys){

     entry=leg->AddEntry("CentralPointsSystematicUncertainty_copy","Syst. unc. (data)","f");
     ci = TColor::GetColor("#cccccc");
     entry->SetFillColor(ci);
     entry->SetFillStyle(1001);
  }

  if(isSim){
      ci = TColor::GetColor("#000099");
     entry->SetLineColor(ci);
     entry->SetLineStyle(1);
     entry->SetLineWidth(1);
     entry->SetMarkerColor(1);
     entry->SetMarkerStyle(21);
     entry->SetMarkerSize(1);
     entry->SetTextFont(43);
     if(fSystem) entry=leg->AddEntry("GeneratorLevel_JetPtSpectrum","POWHEG+PYTHIA6 #times A","p");
     else entry=leg->AddEntry("GeneratorLevel_JetPtSpectrum","POWHEG+PYTHIA6","p");
     entry->SetLineColor(1);
     entry->SetLineStyle(1);
     entry->SetLineWidth(1);
     ci = TColor::GetColor("#000099");
     entry->SetMarkerColor(ci);
     entry->SetMarkerStyle(24);
     entry->SetMarkerSize(1.2f);
     entry->SetTextFont(43);
     if(isSimSys) {
       entry=leg->AddEntry("theorySyst_copy","Syst. unc. (theory)","f");
       entry->SetFillColor(1);
       entry->SetLineColor(ci);
       entry->SetLineStyle(1);
       entry->SetLineWidth(2);
       entry->SetMarkerColor(1);
       entry->SetMarkerStyle(21);
       entry->SetMarkerSize(1);
       entry->SetTextFont(43);
    }
 }
   leg->Draw();

   TPaveText *pt = nullptr;
   TPaveText *pt2 = nullptr;
   if(fObservable==Observable::kXsection) pt = new TPaveText(0.2,0.75,0.85,0.95,"NB NDC");
   if(fObservable==Observable::kFragmentation) pt = new TPaveText(0.2,0.8,0.85,0.95,"NB NDC");
   if(fObservable==Observable::kXsection) pt2 = new TPaveText(0.2,0.6,0.85,0.75,"NB NDC");
   if(fObservable==Observable::kFragmentation) pt2 = new TPaveText(0.2,0.61,0.85,0.79,"NB NDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   pt->SetTextAlign(13);
   pt->SetTextFont(43);
   pt->SetTextSize(22);
   pt2->SetBorderSize(0);
   pt2->SetFillStyle(0);
   pt2->SetTextAlign(13);
   pt2->SetTextFont(43);
   pt2->SetTextSize(22);
   //TText *text = pt->AddText("ALICE Preliminary");
   TText *text = new TText;
   text = pt->AddText("ALICE Preliminary"); //uncomment
   if(fSystem) text = pt->AddText("p-Pb, #sqrt{#it{s}_{NN}} = 13 TeV");
   else text = pt->AddText("pp, #sqrt{#it{s}} = 13 TeV");
   text = pt->AddText(Form("charged jets, anti-#it{k}_{T}, #it{R} = 0.%d, |#it{#eta}_{lab}^{jet}| < 0.%d",Rpar,9-Rpar));
   if(fObservable==Observable::kXsection)text = pt2->AddText(Form ("with D^{0}, %d < #it{p}_{T,D^{0}} < %d GeV/#it{c}",static_cast<int>(fptbinsDA[0]),static_cast<int>(fptbinsDA[fptbinsDN])));
   if(fObservable==Observable::kFragmentation)text = pt2->AddText(Form ("with D^{0}, %d < #it{p}_{T,D^{0}} < %d GeV/#it{c}",static_cast<int>(fzptbinsDPromptA[fBin-1][0]),static_cast<int>(fzptbinsDPromptA[fBin-1][fzptbinsDPromptN[fBin-1]])));
   if(fObservable==Observable::kFragmentation)text = pt2->AddText(Form("%d < #it{p}_{T,ch. jet} < %d GeV/#it{c}",static_cast<Int_t>(fzptJetMeasA[fBin-1]),static_cast<Int_t>(fzptJetMeasA[fBin])));
   pt->Draw();
   pt2->Draw();

   // does nothing
   TH1D *CentralPointsStatisticalUncertainty__4 = new TH1D("CentralPointsStatisticalUncertainty__4","Central Values",xAxisBins, xAxis);
   CentralPointsStatisticalUncertainty__4->SetMinimum(2e-05);
   CentralPointsStatisticalUncertainty__4->SetMaximum(0.7);
   CentralPointsStatisticalUncertainty__4->SetEntries(8);
   CentralPointsStatisticalUncertainty__4->SetDirectory(nullptr);
   CentralPointsStatisticalUncertainty__4->SetStats(0);

   ci = TColor::GetColor("#990000");
   CentralPointsStatisticalUncertainty__4->SetLineColor(ci);

   ci = TColor::GetColor("#990000");
   CentralPointsStatisticalUncertainty__4->SetMarkerColor(ci);
   CentralPointsStatisticalUncertainty__4->SetMarkerStyle(20);
   CentralPointsStatisticalUncertainty__4->SetMarkerSize(0.9f);
   if(fObservable == Observable::kXsection)CentralPointsStatisticalUncertainty__4->GetXaxis()->SetTitle("#it{p}_{T,ch jet} (GeV/#it{c})");
   if(fObservable == Observable::kFragmentation)CentralPointsStatisticalUncertainty__4->GetXaxis()->SetTitle("#it{z}_{#parallel}^{ch,jet}");
   //if(pdf)CentralPointsStatisticalUncertainty__4->GetXaxis()->SetRangeUser(hData_binned->GetXaxis()->GetBinLowEdge(binMin),hData_binned->GetXaxis()->GetBinUpEdge(binMax));
   CentralPointsStatisticalUncertainty__4->GetXaxis()->SetLabelFont(42);
   CentralPointsStatisticalUncertainty__4->GetXaxis()->SetLabelSize(0.035f);
   CentralPointsStatisticalUncertainty__4->GetXaxis()->SetTitleSize(0.035f);
   CentralPointsStatisticalUncertainty__4->GetXaxis()->SetTitleFont(42);
   CentralPointsStatisticalUncertainty__4->GetYaxis()->SetTitle("#frac{d^{2}#sigma}{d#it{p}_{T}d#it{#eta}} [mb (GeV/#it{c})^{-1}]");
   CentralPointsStatisticalUncertainty__4->GetYaxis()->SetLabelFont(43);
   CentralPointsStatisticalUncertainty__4->GetYaxis()->SetLabelSize(22);
   CentralPointsStatisticalUncertainty__4->GetYaxis()->SetTitleSize(26);
   CentralPointsStatisticalUncertainty__4->GetYaxis()->SetTitleOffset(1.6f);
   CentralPointsStatisticalUncertainty__4->GetYaxis()->SetTitleFont(43);
   CentralPointsStatisticalUncertainty__4->GetZaxis()->SetLabelFont(42);
   CentralPointsStatisticalUncertainty__4->GetZaxis()->SetLabelSize(0.035f);
   CentralPointsStatisticalUncertainty__4->GetZaxis()->SetTitleSize(0.035f);
   CentralPointsStatisticalUncertainty__4->GetZaxis()->SetTitleFont(42);
   CentralPointsStatisticalUncertainty__4->Draw("sameaxis");
   FinalSpectrum_1->Modified();
   FinalSpectrum->cd();

// ------------>Primitives in pad: FinalSpectrum_2
   TPad* FinalSpectrum_2 = new TPad("FinalSpectrum_2", "FinalSpectrum_2",0,0,1,0.35);
   FinalSpectrum_2->Draw();
   FinalSpectrum_2->cd();
   FinalSpectrum_2->Range(-1.986821e-07,-0.9209589,33.33333,2.49);
   FinalSpectrum_2->SetFillColor(0);
   FinalSpectrum_2->SetBorderMode(0);
   FinalSpectrum_2->SetBorderSize(2);
   FinalSpectrum_2->SetGridy();
   FinalSpectrum_2->SetTickx(1);
   FinalSpectrum_2->SetTicky(1);
   FinalSpectrum_2->SetLeftMargin(0.15f);
   FinalSpectrum_2->SetTopMargin(0);
   FinalSpectrum_2->SetBottomMargin(0.27f);
   FinalSpectrum_2->SetFrameBorderMode(0);
   FinalSpectrum_2->SetFrameBorderMode(0);

   // central points data (values don't really needed)
   TH1D *CentralPointsStatisticalUncertainty__5 = new TH1D("CentralPointsStatisticalUncertainty__5","Central Values",xAxisBins, xAxis);
   CentralPointsStatisticalUncertainty__5->SetMinimum(0);
   CentralPointsStatisticalUncertainty__5->SetMaximum(3.3);
   CentralPointsStatisticalUncertainty__5->SetEntries(8);
   CentralPointsStatisticalUncertainty__5->SetDirectory(nullptr);
   CentralPointsStatisticalUncertainty__5->SetStats(0);

   ci = TColor::GetColor("#990000");
   CentralPointsStatisticalUncertainty__5->SetLineColor(ci);

   ci = TColor::GetColor("#990000");
   CentralPointsStatisticalUncertainty__5->SetMarkerColor(ci);
   CentralPointsStatisticalUncertainty__5->SetMarkerStyle(20);
   CentralPointsStatisticalUncertainty__5->SetMarkerSize(0.9f);
   if(fObservable == Observable::kXsection)CentralPointsStatisticalUncertainty__5->GetXaxis()->SetTitle("#it{p}_{T,ch jet} (GeV/#it{c})");
   if(fObservable == Observable::kFragmentation)CentralPointsStatisticalUncertainty__5->GetXaxis()->SetTitle("#it{z}_{#parallel}^{ch,jet}");
   CentralPointsStatisticalUncertainty__5->GetXaxis()->SetLabelFont(43);
   CentralPointsStatisticalUncertainty__5->GetXaxis()->SetLabelSize(22);
   CentralPointsStatisticalUncertainty__5->GetXaxis()->SetTitleSize(26);
   CentralPointsStatisticalUncertainty__5->GetXaxis()->SetTitleOffset(2.9f);
   CentralPointsStatisticalUncertainty__5->GetXaxis()->SetTitleFont(43);
   CentralPointsStatisticalUncertainty__5->GetYaxis()->SetTitle("theory / data");
   CentralPointsStatisticalUncertainty__5->GetYaxis()->SetNdivisions(509);
   CentralPointsStatisticalUncertainty__5->GetYaxis()->SetLabelFont(43);
   CentralPointsStatisticalUncertainty__5->GetYaxis()->SetLabelSize(22);
   CentralPointsStatisticalUncertainty__5->GetYaxis()->SetTitleSize(26);
   CentralPointsStatisticalUncertainty__5->GetYaxis()->SetTitleOffset(1.4f);
   CentralPointsStatisticalUncertainty__5->GetYaxis()->SetTitleFont(43);
   CentralPointsStatisticalUncertainty__5->GetZaxis()->SetLabelFont(42);
   CentralPointsStatisticalUncertainty__5->GetZaxis()->SetLabelSize(0.035f);
   CentralPointsStatisticalUncertainty__5->GetZaxis()->SetTitleSize(0.035f);
   CentralPointsStatisticalUncertainty__5->GetZaxis()->SetTitleFont(42);
   if(fBin==1 || fBin==2)CentralPointsStatisticalUncertainty__5->SetMaximum(7);
   CentralPointsStatisticalUncertainty__5->Draw("axis");

if(isSys) {
   // data syst unc ratio
   grae =  dynamic_cast<TGraphAsymmErrors*>(grsysRatio->Clone("grae")); //   new TGraphAsymmErrors(6);
   grae->SetName("ratioSyst");
   grae->SetTitle("Bayes, iter=4, prior=ResponseTruth Systematics");

   ci = TColor::GetColor("#cccccc");
   grae->SetFillColor(ci);
   grae->SetLineColor(ci);
   grae->SetMarkerColor(ci);

   TH1F *Graph_ratioSyst3 = new TH1F("Graph_ratioSyst3","Bayes, iter=4, prior=ResponseTruth Systematics",100,2.5,52.5);
   Graph_ratioSyst3->SetMinimum(0.5839274);
   Graph_ratioSyst3->SetMaximum(2.28012);
   Graph_ratioSyst3->SetDirectory(nullptr);
   Graph_ratioSyst3->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_ratioSyst3->SetLineColor(ci);
   Graph_ratioSyst3->GetXaxis()->SetLabelFont(42);
   Graph_ratioSyst3->GetXaxis()->SetLabelSize(0.035f);
   Graph_ratioSyst3->GetXaxis()->SetTitleSize(0.035f);
   Graph_ratioSyst3->GetXaxis()->SetTitleFont(42);
   Graph_ratioSyst3->GetYaxis()->SetLabelFont(42);
   Graph_ratioSyst3->GetYaxis()->SetLabelSize(0.035f);
   Graph_ratioSyst3->GetYaxis()->SetTitleSize(0.035f);
   Graph_ratioSyst3->GetYaxis()->SetTitleFont(42);
   Graph_ratioSyst3->GetZaxis()->SetLabelFont(42);
   Graph_ratioSyst3->GetZaxis()->SetLabelSize(0.035f);
   Graph_ratioSyst3->GetZaxis()->SetTitleSize(0.035f);
   Graph_ratioSyst3->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_ratioSyst3);

   grae->Draw("2");
}

   // data central ratio
   TH1D *CentralPointsStatisticalUncertainty__6 =  dynamic_cast<TH1D*>(hData_binned_ratio->Clone("CentralPointsStatisticalUncertainty__6"));
   CentralPointsStatisticalUncertainty__6->SetMinimum(1.122659e-05);
   CentralPointsStatisticalUncertainty__6->SetMaximum(0.06069752);
   CentralPointsStatisticalUncertainty__6->SetEntries(14);
   CentralPointsStatisticalUncertainty__6->SetDirectory(nullptr);
   CentralPointsStatisticalUncertainty__6->SetStats(0);

   ci = TColor::GetColor("#990000");
   CentralPointsStatisticalUncertainty__6->SetLineColor(ci);
   CentralPointsStatisticalUncertainty__6->SetMarkerColor(ci);
   CentralPointsStatisticalUncertainty__6->SetMarkerStyle(20);
   CentralPointsStatisticalUncertainty__6->SetMarkerSize(0.9f);
   if(fObservable == Observable::kXsection)CentralPointsStatisticalUncertainty__6->GetXaxis()->SetTitle("#it{p}_{T,ch jet} (GeV/#it{c})");
   if(fObservable == Observable::kFragmentation)CentralPointsStatisticalUncertainty__6->GetXaxis()->SetTitle("#it{z}_{#parallel}^{ch,jet}");

   CentralPointsStatisticalUncertainty__6->GetXaxis()->SetLabelFont(42);
   CentralPointsStatisticalUncertainty__6->GetXaxis()->SetLabelSize(0.035f);
   CentralPointsStatisticalUncertainty__6->GetXaxis()->SetTitleSize(0.035f);
   CentralPointsStatisticalUncertainty__6->GetXaxis()->SetTitleFont(42);
   CentralPointsStatisticalUncertainty__6->GetYaxis()->SetTitle("#frac{d^{2}#sigma}{d#it{p}_{T}d#it{#eta}} [mb (GeV/#it{c})^{-1}]");
   CentralPointsStatisticalUncertainty__6->GetYaxis()->SetLabelFont(42);
   CentralPointsStatisticalUncertainty__6->GetYaxis()->SetLabelSize(0.035f);
   CentralPointsStatisticalUncertainty__6->GetYaxis()->SetTitleSize(0.035f);
   CentralPointsStatisticalUncertainty__6->GetYaxis()->SetTitleOffset(1.4f);
   CentralPointsStatisticalUncertainty__6->GetYaxis()->SetTitleFont(42);
   CentralPointsStatisticalUncertainty__6->GetZaxis()->SetLabelFont(42);
   CentralPointsStatisticalUncertainty__6->GetZaxis()->SetLabelSize(0.035f);
   CentralPointsStatisticalUncertainty__6->GetZaxis()->SetTitleSize(0.035f);
   CentralPointsStatisticalUncertainty__6->GetZaxis()->SetTitleFont(42);
   CentralPointsStatisticalUncertainty__6->Draw("same p e0 x0");

   // theory syst ratio
   if(isSim && isSimSys) {
     grae = dynamic_cast<TGraphAsymmErrors*>(grsystheoryratio->Clone("grae")); // new TGraphAsymmErrors(6);
     grae->SetName("ratioTheorySyst");
     grae->SetTitle("Graph");
     grae->SetFillColor(1);
     grae->SetFillStyle(0);

     ci = TColor::GetColor("#000099");
     grae->SetLineColor(ci);
     grae->SetLineWidth(2);
     grae->SetMarkerStyle(24);
     //grae->SetMarkerSize(2);
     grae->SetMarkerColor(kBlue+1);

     TH1F *Graph_ratioTheorySyst4 = new TH1F("Graph_ratioTheorySyst4","Graph",100,2.5,32.5);
     Graph_ratioTheorySyst4->SetMinimum(0.2058133);
     Graph_ratioTheorySyst4->SetMaximum(1.968171);
     Graph_ratioTheorySyst4->SetDirectory(nullptr);
     Graph_ratioTheorySyst4->SetStats(0);

     ci = TColor::GetColor("#000099");
     Graph_ratioTheorySyst4->SetLineColor(ci);
     Graph_ratioTheorySyst4->GetXaxis()->SetLabelFont(42);
     Graph_ratioTheorySyst4->GetXaxis()->SetLabelSize(0.035f);
     Graph_ratioTheorySyst4->GetXaxis()->SetTitleSize(0.035f);
     Graph_ratioTheorySyst4->GetXaxis()->SetTitleFont(42);
     Graph_ratioTheorySyst4->GetYaxis()->SetLabelFont(42);
     Graph_ratioTheorySyst4->GetYaxis()->SetLabelSize(0.035f);
     Graph_ratioTheorySyst4->GetYaxis()->SetTitleSize(0.035f);
     Graph_ratioTheorySyst4->GetYaxis()->SetTitleFont(42);
     Graph_ratioTheorySyst4->GetZaxis()->SetLabelFont(42);
     Graph_ratioTheorySyst4->GetZaxis()->SetLabelSize(0.035f);
     Graph_ratioTheorySyst4->GetZaxis()->SetTitleSize(0.035f);
     Graph_ratioTheorySyst4->GetZaxis()->SetTitleFont(42);
     grae->SetHistogram(Graph_ratioTheorySyst4);
     grae->Draw("P2");
  }
   // just for graphics, data not really needed
   TH1D *CentralPointsStatisticalUncertainty__7 = new TH1D("CentralPointsStatisticalUncertainty__7","Central Values",xAxisBins, xAxis);
   CentralPointsStatisticalUncertainty__7->SetMinimum(0);
   CentralPointsStatisticalUncertainty__7->SetMaximum(2.49);
   CentralPointsStatisticalUncertainty__7->SetEntries(8);
   CentralPointsStatisticalUncertainty__7->SetDirectory(nullptr);
   CentralPointsStatisticalUncertainty__7->SetStats(0);

   ci = TColor::GetColor("#990000");
   CentralPointsStatisticalUncertainty__7->SetLineColor(ci);
   CentralPointsStatisticalUncertainty__7->SetMarkerColor(ci);
   CentralPointsStatisticalUncertainty__7->SetMarkerStyle(20);
   CentralPointsStatisticalUncertainty__7->SetMarkerSize(0.9f);
   if(fObservable == Observable::kXsection)CentralPointsStatisticalUncertainty__7->GetXaxis()->SetTitle("#it{p}_{T,ch jet} (GeV/#it{c})");
   if(fObservable == Observable::kFragmentation)CentralPointsStatisticalUncertainty__7->GetXaxis()->SetTitle("#it{z}_{#parallel}^{ch,jet}");
   CentralPointsStatisticalUncertainty__7->GetXaxis()->SetLabelFont(43);
   CentralPointsStatisticalUncertainty__7->GetXaxis()->SetLabelSize(22);
   CentralPointsStatisticalUncertainty__7->GetXaxis()->SetTitleSize(26);
   CentralPointsStatisticalUncertainty__7->GetXaxis()->SetTitleOffset(2.9f);
   CentralPointsStatisticalUncertainty__7->GetXaxis()->SetTitleFont(43);
   CentralPointsStatisticalUncertainty__7->GetYaxis()->SetTitle("theory / data");
   CentralPointsStatisticalUncertainty__7->GetYaxis()->SetRangeUser(0,3.5);//added
   CentralPointsStatisticalUncertainty__7->GetYaxis()->SetNdivisions(509);
   CentralPointsStatisticalUncertainty__7->GetYaxis()->SetLabelFont(43);
   CentralPointsStatisticalUncertainty__7->GetYaxis()->SetLabelSize(22);
   CentralPointsStatisticalUncertainty__7->GetYaxis()->SetTitleSize(26);
   CentralPointsStatisticalUncertainty__7->GetYaxis()->SetTitleOffset(1.4f);
   CentralPointsStatisticalUncertainty__7->GetYaxis()->SetTitleFont(43);
   CentralPointsStatisticalUncertainty__7->GetZaxis()->SetLabelFont(42);
   CentralPointsStatisticalUncertainty__7->GetZaxis()->SetLabelSize(0.035f);
   CentralPointsStatisticalUncertainty__7->GetZaxis()->SetTitleSize(0.035f);
   CentralPointsStatisticalUncertainty__7->GetZaxis()->SetTitleFont(42);
   CentralPointsStatisticalUncertainty__7->Draw("sameaxig");




   FinalSpectrum_2->Modified();
   FinalSpectrum->cd();
   FinalSpectrum->Modified();
   FinalSpectrum->cd();
   FinalSpectrum->SetSelected(FinalSpectrum);

   FinalSpectrum->SaveAs(Form("%s/JetPtSpectra_final.png",outPlotDir.Data()));
   FinalSpectrum->SaveAs(Form("%s/JetPtSpectra_final.pdf",outPlotDir.Data()));
   FinalSpectrum->SaveAs(Form("%s/JetPtSpectra_final.eps",outPlotDir.Data()));
   FinalSpectrum->SaveAs(Form("%s/JetPtSpectra_final.svg",outPlotDir.Data()));

}
