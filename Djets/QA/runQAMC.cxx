/*#include "TSystem.h"
#include "TFile.h"
#include "TString.h"
#include "TCollection.h"
#include <iostream>
#include <fstream>
#include <string>
#include "TObjString.h"
#include "TObjArray.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TKey.h"
#include "TH1D.h"
#include "TH1I.h"
#include "TMath.h"
#include "TPavesText.h"
#include "TLegend.h"
#include "TColor.h"
#include "TStyle.h"
#include "AliEmcalList.h"
#include "AliNormalizationCounter.h"*/
#include "OCanvas.h"
#include "OLoader.h"
#include "TROOT.h"
#include <fstream>

void runQAMC(){
    gROOT->LoadMacro("OLoader.cxx++g");
    gROOT->LoadMacro("OCanvas.cxx++g");
    const Int_t nHisto = 16;
    TString histoNamesLoad[nHisto]{"fhPhiJetPtJetMC","fPhiJetTrksPtJetMC","fhEtaJetPtJetMC","fhEtaJetTrksPtJetMC","fhAreaJetPtJetMC","fhJetTrksPtJetMC","fhPhiJetPtJetrec","fPhiJetTrksPtJetrec","fhEtaJetPtJetrec","fhEtaJetTrksPtJetrec","fhAreaJetPtJetrec","fhJetTrksPtJetrec","fhPhiJetPtJetD","fPhiJetTrksPtJetD","fhAreaJetPtJetD","fhJetTrksPtJetD"};
    TString scaling[nHisto]{"1","1","1","1","1","1"};
    const Int_t nYear = 1;
    const Int_t N_X = 1;
    enum years{LHC2016,LHC2017,LHC2018};
    TString Syears[nYear]{"2017"};
    Color_t colorWheel[13]{632,416,600,616,432,800,825,845,865,885,905,1,400};

  //  TString scale = "nevents";
    TString scale = "nentries";
   // //kWhite  = 0,   kBlack  = 1,   kGray    = 920,  kRed    = 632,  kGreen  = 416,
   // kBlue   = 600, kYellow = 400, kMagenta = 616,  kCyan   = 432,  kOrange = 800,
    //kSpring = 820, kTeal   = 840, kAzure   =  860, kViolet = 880,  kPink   = 900

    TString MCfileList = "MCFileList";

    std::ifstream fileList;
    fileList.open (MCfileList);
    std::string name;

    std::map<TString,OLoader*> MC;

    while ( std::getline(fileList, name)){
       TString p(name);
       p=((TObjString *)p.Tokenize("/")->Last())->String().ReplaceAll("AnalysisResults_","").ReplaceAll(".root","");
       MC.insert(std::make_pair(p,new OLoader(name,true)));
       MC.find(p)->second->OSetDataName(p);
    }
       fileList.close();

    TString DATAfileList = "DATAFileList";

    std::ifstream DfileList;
    DfileList.open (DATAfileList);
    std::string DATAname;

    std::map<TString,OLoader*> DATA;

    while ( std::getline(DfileList, DATAname)){
      TString p(DATAname);
      p=((TObjString *)p.Tokenize("/")->Last())->String().ReplaceAll("AnalysisResults_","").ReplaceAll(".root","");
      DATA.insert(std::make_pair(p,new OLoader(DATAname,true)));
      DATA.find(p)->second->OSetDataName(p);
    }
      DfileList.close();

    TString list2D[4]{"_incl_MC","_incl_Reco","_Djet_MC","_Djet_Reco"};
    TString list2DName[6]{"fhAreaJetPtJet","fhPhiJetPtJet","fhEtaJetPtJet","fhJetTrksPtJet","fhPhiJetTrksPtJet","fhEtaJetTrksPtJet"};
    TString listIntName[6]{"hPhiJetTrks","hEtaJetTrks","hPtJetTrks","hPhiJet","hEtaJet","hPtJet"};
    Double_t ptbins[8] = {3.,5.,10.,15.,20.,30.,50.,100.};
    Double_t ptint[8] = {5.,100.};


    OCanvas *c = new OCanvas();
    OCanvas *cproj[2];
    cproj[0]= new OCanvas();
    cproj[1]= new OCanvas();
    OCanvas *cproj2[2];
    cproj2[0]= new OCanvas();
    cproj2[1]= new OCanvas();
    //OLoader *o = new OLoader("MC/AnalysisResults_MC_LHC17c_hadronPID.root",true);
  /*  MC.insert(std::make_pair("MC_LHC17c_hadronPID",new OLoader("MC/AnalysisResults_MC_LHC17c_hadronPID.root",true)));
    MC.insert(std::make_pair("MC_LHC17e_hadronPID",new OLoader("MC/AnalysisResults_MC_LHC17e_hadronPID.root",true)));
    MC.find("MC_LHC17c_hadronPID")->second->OSetDataName("MC_LHC17c_hadronPID");
    MC.find("MC_LHC17e_hadronPID")->second->OSetDataName("MC_LHC17e_hadronPID");*/

    Int_t id = -1;
    TString year[2] = {"LHC17","LHC18"};
    for(auto mc : MC){
        id++;
        Int_t y = 0;
        if(mc.first.Contains("LHC17")) y = 0;
        if(mc.first.Contains("LHC18")) y = 1;
        for(Int_t i = 0; i< 4;i++){
           c->OCreate(mc.first+list2D[i],3,2);
           mc.second->OSetNameSpace("DmesonsForJetCorrelations.histosD0MBNXMCrec.");
           mc.second->OSetY("fhJetTrksPtJet"+list2D[i],1,15);
           mc.second->OSetY("fhEtaJetPtJet"+list2D[i],-1,1);
           mc.second->OSetY("fhEtaJetTrksPtJet"+list2D[i],-1,1);
           for(Int_t h = 0; h< 6;h++){
             mc.second->OSetX(list2DName[h]+list2D[i],0,100);
             c->OPlace(mc.first+list2D[i],mc.second->OGetHisto2(list2DName[h]+list2D[i]),h+1,"colzlogx");
             mc.second->ODo1DprojectionY(list2DName[h]+list2D[i],ptbins,sizeof(ptbins)/sizeof(ptbins[0]));
             mc.second->ODo1DprojectionY(list2DName[h]+list2D[i],ptint,2);

             if(mc.first.Contains("LHC17c")){
                 cproj[y]->OCreate(year[y]+"_"+list2DName[h]+list2D[i]+"_proj",3,3);
                 if(h==0)cproj2[y]->OCreate(year[y]+"_"+list2D[i]+"_proj",4,2);
             }
             if(mc.first.Contains("LHC18b")){
                 id=0;
                 cproj[y]->OCreate(year[y]+"_"+list2DName[h]+list2D[i]+"_proj",3,3);
                 if(h==0)cproj2[y]->OCreate(year[y]+"_"+list2D[i]+"_proj",4,2);
             }

             if(h==0 || h==3)mc.second->OSetProp(mc.second->OGetHisto1(list2DName[h]+list2D[i]+"_px_"+ptint[0]+"_"+ptint[1]), scale, 0);
             else mc.second->OSetProp(mc.second->OGetHisto1(list2DName[h]+list2D[i]+"_px_"+ptint[0]+"_"+ptint[1]), scale, 2);
             mc.second->OSetColor(list2DName[h]+list2D[i]+"_px_"+ptint[0]+"_"+ptint[1],colorWheel[id],2);
             if(h==0 || h==3)cproj2[y]->OPlace(year[y]+"_"+list2D[i]+"_proj",mc.second->OGetHisto1(list2DName[h]+list2D[i]+"_px_"+ptint[0]+"_"+ptint[1]),h<3?h+1:h+2,"logy","0");
             else cproj2[y]->OPlace(year[y]+"_"+list2D[i]+"_proj",mc.second->OGetHisto1(list2DName[h]+list2D[i]+"_px_"+ptint[0]+"_"+ptint[1]),h<3?h+1:h+2,"","0");


             if(i==0 && h==0)cproj[y]->OLegend("data",mc.second->OGetHisto1(list2DName[0]+list2D[0]+"_px_"+ptbins[0]+"_"+ptbins[1]),mc.first);
             for(UInt_t j = 0; j< sizeof(ptbins)/sizeof(ptbins[0])-1;j++){
                 if(h==0 || h==3)mc.second->OSetProp(mc.second->OGetHisto1(list2DName[h]+list2D[i]+"_px_"+ptbins[j]+"_"+ptbins[j+1]), scale, 0);
                 else if(j==0||j==1||j==2) mc.second->OSetProp(mc.second->OGetHisto1(list2DName[h]+list2D[i]+"_px_"+ptbins[j]+"_"+ptbins[j+1]), scale, 2);
                 else if(j==3||j==4||j==5) mc.second->OSetProp(mc.second->OGetHisto1(list2DName[h]+list2D[i]+"_px_"+ptbins[j]+"_"+ptbins[j+1]), scale, 4);
                 else mc.second->OSetProp(mc.second->OGetHisto1(list2DName[h]+list2D[i]+"_px_"+ptbins[j]+"_"+ptbins[j+1]), scale, 12);
                // std::cout<<"file: "<<mc.first<<std::endl;
                mc.second->OSetColor(list2DName[h]+list2D[i]+"_px_"+ptbins[j]+"_"+ptbins[j+1],colorWheel[id],2);
                if(h==0 || h==3)cproj[y]->OPlace(year[y]+"_"+list2DName[h]+list2D[i]+"_proj",mc.second->OGetHisto1(list2DName[h]+list2D[i]+"_px_"+ptbins[j]+"_"+ptbins[j+1]),j+1,"logy","1");
                else cproj[y]->OPlace(year[y]+"_"+list2DName[h]+list2D[i]+"_proj",mc.second->OGetHisto1(list2DName[h]+list2D[i]+"_px_"+ptbins[j]+"_"+ptbins[j+1]),j+1,"","1");

             }

             cproj[y]->OPlace(year[y]+"_"+list2DName[h]+list2D[i]+"_proj",cproj[y]->OGetLegend("data"),sizeof(ptbins)/sizeof(ptbins[0])+1,"");
             //cproj[y]->ODraw(year[y]+"_"+list2DName[h]+list2D[i]+"_proj");
           }
           cproj2[y]->OPlace(year[y]+"_"+list2D[i]+"_proj",cproj[y]->OGetLegend("data"),8,"");
        }
    }
if(scale.Contains("nevents")){
    c->OFlush("ResultsMC/2D","png");
    cproj[0]->OFlush("ResultsMC/2D_proj","png");
    cproj[1]->OFlush("ResultsMC/2D_proj","png");
    cproj2[0]->OFlush("ResultsMC/2D_INT","png");
    cproj2[1]->OFlush("ResultsMC/2D_INT","png");
    c->OFlush("ResultsMC/2D","svg");
    cproj[0]->OFlush("ResultsMC/2D_proj","svg");
    cproj[1]->OFlush("ResultsMC/2D_proj","svg");
    cproj2[0]->OFlush("ResultsMC/2D_INT","svg");
    cproj2[1]->OFlush("ResultsMC/2D_INT","svg");
    c->OFlush("ResultsMC/2D","pdf");
    cproj[0]->OFlush("ResultsMC/2D_proj","pdf");
    cproj[1]->OFlush("ResultsMC/2D_proj","pdf");
    cproj2[0]->OFlush("ResultsMC/2D_INT","pdf");
    cproj2[1]->OFlush("ResultsMC/2D_INT","pdf");
    c->OFlush("ResultsMC/2D","eps");
    cproj[0]->OFlush("ResultsMC/2D_proj","eps");
    cproj[1]->OFlush("ResultsMC/2D_proj","eps");
    cproj2[0]->OFlush("ResultsMC/2D_INT","eps");
    cproj2[1]->OFlush("ResultsMC/2D_INT","eps");
}
    //integrated plots
    OCanvas *cint = new OCanvas();

    id=-1;

    for(auto mc : MC){
        id++;
        if(mc.first.Contains("LHC18b")){
            id=0;
        }
        Int_t y = 0;

        if(mc.first.Contains("LHC17")) y = 0;
        if(mc.first.Contains("LHC18")) y = 1;
        if(id==0)cint->OCreate(year[y],4,2);

           cint->OCreate(mc.first,3,2);
           mc.second->OSetNameSpace("DmesonsForJetCorrelations.histosD0MBNXMCrec.");
           for(Int_t h = 0; h< 6;h++){
               if(h==2 || h==5){
                   mc.second->OSetX(listIntName[h],0,100);
               }

             mc.second->OSetProp(mc.second->OGetHisto1(listIntName[h]), scale, 0);
             mc.second->OSetColor(listIntName[h],colorWheel[id],2);
             if(h == 0)cint->OLegend(year[y],mc.second->OGetHisto1(listIntName[h]),mc.first);
             if(h==2||h==5){
                 cint->OPlace(mc.first,mc.second->OGetHisto1(listIntName[h]),h+1,"logy","1");
                 cint->OPlace(year[y],mc.second->OGetHisto1(listIntName[h]),h<3?h+1:h+2,"logy","0");
             }
             else{
                 cint->OPlace(mc.first,mc.second->OGetHisto1(listIntName[h]),h+1,"","1");
                 cint->OPlace(year[y],mc.second->OGetHisto1(listIntName[h]),h<3?h+1:h+2,"","0");
             }


           }
           cint->OForceStack(year[y],3,0,100);
           cint->OForceStack(year[y],7,0,100);
           if(id ==0)cint->OPlace(year[y],cint->OGetLegend(year[y]),8,"");
        }
if(scale.Contains("nevents")){
    cint->OFlush("ResultsMC/1D","png");
    cint->OFlush("ResultsMC/1D","svg");
    cint->OFlush("ResultsMC/1D","pdf");
    cint->OFlush("ResultsMC/1D","eps");
}

    OCanvas *csparse = new OCanvas();
    TString s[2] = {"p_T^{res}","z^{res}"};
    for(auto mc : MC){
        id++;
        Int_t y = 0;
        if(mc.first.Contains("LHC17")) y = 0;
       if(mc.first.Contains("LHC18")) y = 1;
         if(mc.first.Contains("LHC17c")) id = 0;
        if(mc.first.Contains("LHC18b")) id = 0;

        for(Int_t i = 0; i< 2;i++){
            if(id==0)csparse->OCreate(year[y]+s[i],3,3);
            csparse->OCreate(mc.first+s[i],3,3);
            mc.second->OSetNameSpace("DmesonsForJetCorrelations.histosD0MBNXMCrec.");
            for(UInt_t j = 0; j< sizeof(ptbins)/sizeof(ptbins[0])-1;j++){
                //std::cout<<"bin cut: "<<ptbins[j]<<" "<<ptbins[j+1]<<std::endl;
                mc.second->OCutSparse("ResponseMatrix",5,ptbins[j],ptbins[j+1]);
                mc.second->OProjectSparse("ResponseMatrix",22+i);
                mc.second->OSetColor("ResponseMatrix"+TString::Itoa(22+i,10)+TString::Itoa(j,10),colorWheel[id],2);
                if(j==0||j==1)mc.second->OSetProp(mc.second->OGetHisto1("ResponseMatrix"+TString::Itoa(22+i,10)+TString::Itoa(j,10)), scale, 0);
                if(j==2||j==3)mc.second->OSetProp(mc.second->OGetHisto1("ResponseMatrix"+TString::Itoa(22+i,10)+TString::Itoa(j,10)), scale, 2);
                if(j==4||j==5)mc.second->OSetProp(mc.second->OGetHisto1("ResponseMatrix"+TString::Itoa(22+i,10)+TString::Itoa(j,10)), scale, 4);
                if(j==6||j==7)mc.second->OSetProp(mc.second->OGetHisto1("ResponseMatrix"+TString::Itoa(22+i,10)+TString::Itoa(j,10)), scale, 10);
                mc.second->OGetHisto1("ResponseMatrix"+TString::Itoa(22+i,10)+TString::Itoa(j,10))->SetTitle(mc.first +" "+ s[i]+" "+ptbins[j]+" - "+ptbins[j+1]+" GeV/c");
                mc.second->OGetHisto1("ResponseMatrix"+TString::Itoa(22+i,10)+TString::Itoa(j,10))->GetXaxis()->SetTitle(s[i]);
                csparse->OPlace(mc.first+s[i],mc.second->OGetHisto1("ResponseMatrix"+TString::Itoa(22+i,10)+TString::Itoa(j,10)),j+1,"","1");
                csparse->OPlace(year[y]+s[i],mc.second->OGetHisto1("ResponseMatrix"+TString::Itoa(22+i,10)+TString::Itoa(j,10)),j+1,"logy","0");

                if(j==0 && i==0)csparse->OLegend(year[y],mc.second->OGetHisto1("ResponseMatrix"+TString::Itoa(22+i,10)+TString::Itoa(j,10)),mc.first);
            }
            mc.second->OCutSparse("ResponseMatrix",5,ptint[0],ptint[1]);
            mc.second->OProjectSparse("ResponseMatrix",22+i);
            mc.second->OSetColor("ResponseMatrix"+TString::Itoa(22+i,10)+TString::Itoa(sizeof(ptbins)/sizeof(ptbins[0])-1,10),colorWheel[id],2);
            mc.second->OGetHisto1("ResponseMatrix"+TString::Itoa(22+i,10)+TString::Itoa(sizeof(ptbins)/sizeof(ptbins[0])-1,10))->SetTitle(mc.first +" "+ s[i]+" "+ptint[0]+" - "+ptint[1]+" GeV/c");
            mc.second->OGetHisto1("ResponseMatrix"+TString::Itoa(22+i,10)+TString::Itoa(sizeof(ptbins)/sizeof(ptbins[0])-1,10))->GetXaxis()->SetTitle(s[i]);
            mc.second->OSetProp(mc.second->OGetHisto1("ResponseMatrix"+TString::Itoa(22+i,10)+TString::Itoa(sizeof(ptbins)/sizeof(ptbins[0])-1,10)), scale, 0);
            csparse->OPlace(year[y]+s[i],mc.second->OGetHisto1("ResponseMatrix"+TString::Itoa(22+i,10)+TString::Itoa(sizeof(ptbins)/sizeof(ptbins[0])-1,10)),8,"logy");
            csparse->OPlace(year[y]+s[i],csparse->OGetLegend(year[y]),9,"");
        }
    }
    if(scale.Contains("nevents")){
    csparse->OFlush("ResultsMC/SPARSE","png");
    csparse->OFlush("ResultsMC/SPARSE","svg");
    csparse->OFlush("ResultsMC/SPARSE","pdf");
    csparse->OFlush("ResultsMC/SPARSE","eps");
    }


    //******************************************************************
    //******************************************************************
    //***********************DATA***************************************
    //******************************************************************
    //******************************************************************
/*
    DATA.insert(std::make_pair("LHC17c_pass1",new OLoader("DATA/AnalysisResults_LHC17c_pass1.root",true)));
    DATA.insert(std::make_pair("LHC17e_pass1",new OLoader("DATA/AnalysisResults_LHC17e_pass1.root",true)));
    DATA.find("LHC17c_pass1")->second->OSetDataName("LHC17c_pass1");
    DATA.find("LHC17e_pass1")->second->OSetDataName("LHC17e_pass1");*/

    OCanvas *datac = new OCanvas();
    OCanvas *datacproj[2];
    datacproj[0]= new OCanvas();
    datacproj[1]= new OCanvas();
    OCanvas *datacproj2[2];
    datacproj2[0]= new OCanvas();
    datacproj2[1]= new OCanvas();
    id=-1;
    list2D[0] = "_incl_Reco";
    list2D[1] = "_Djet_Reco";
    for(auto data : DATA){
        id++;
        Int_t y = 0;
        if(data.first.Contains("LHC17")){ y = 0;}
        if(data.first.Contains("LHC18")){ y = 1;}
        if(data.first.Contains("LHC18b")){ id = 0;}
        for(Int_t i = 0; i< 2;i++){
           datac->OCreate(data.first+list2D[i],3,2);
           data.second->OSetNameSpace("DmesonsForJetCorrelations.histosD0MBNX.");
           data.second->OSetY("fhJetTrksPtJet"+list2D[i],1,15);
           data.second->OSetY("fhEtaJetPtJet"+list2D[i],-1,1);
           data.second->OSetY("fhEtaJetTrksPtJet"+list2D[i],-1,1);
           for(Int_t h = 0; h< 6;h++){
             data.second->OSetX(list2DName[h]+list2D[i],0,100);
             datac->OPlace(data.first+list2D[i],data.second->OGetHisto2(list2DName[h]+list2D[i]),h+1,"colzlogx");
             data.second->ODo1DprojectionY(list2DName[h]+list2D[i],ptbins,sizeof(ptbins)/sizeof(ptbins[0]));
             data.second->ODo1DprojectionY(list2DName[h]+list2D[i],ptint,2);
             if(!(datacproj[y]->OGetCanvas(year[y]+"_"+list2DName[h]+list2D[i]+"_proj"))){
                 datacproj[y]->OCreate(year[y]+"_"+list2DName[h]+list2D[i]+"_proj",3,3);
                 if(h==0)datacproj2[y]->OCreate(year[y]+"_"+list2D[i]+"_proj",4,2);
             }
             /*if(id==0){
                 datacproj[y]->OCreate(year[y]+"_"+list2DName[h]+list2D[i]+"_proj",3,3);
                 if(h==0)datacproj2[y]->OCreate(year[y]+"_"+list2D[i]+"_proj",4,2);
             }*/
             if(h==0 || h==3)data.second->OSetProp(data.second->OGetHisto1(list2DName[h]+list2D[i]+"_px_"+ptint[0]+"_"+ptint[1]), scale, 0);
             else data.second->OSetProp(data.second->OGetHisto1(list2DName[h]+list2D[i]+"_px_"+ptint[0]+"_"+ptint[1]), scale, 2);
             data.second->OSetColor(list2DName[h]+list2D[i]+"_px_"+ptint[0]+"_"+ptint[1],colorWheel[id],2);
             if(h==0 || h==3)datacproj2[y]->OPlace(year[y]+"_"+list2D[i]+"_proj",data.second->OGetHisto1(list2DName[h]+list2D[i]+"_px_"+ptint[0]+"_"+ptint[1]),h<3?h+1:h+2,"logy","0");
             else datacproj2[y]->OPlace(year[y]+"_"+list2D[i]+"_proj",data.second->OGetHisto1(list2DName[h]+list2D[i]+"_px_"+ptint[0]+"_"+ptint[1]),h<3?h+1:h+2,"","0");

             if(i==0 && h==0)datacproj[y]->OLegend("data",data.second->OGetHisto1(list2DName[0]+list2D[0]+"_px_"+ptbins[0]+"_"+ptbins[1]),data.first);
             for(UInt_t j = 0; j< sizeof(ptbins)/sizeof(ptbins[0])-1;j++){
                 std::cout<<"H"<<std::endl;
                 if(h==0 || h==3)data.second->OSetProp(data.second->OGetHisto1(list2DName[h]+list2D[i]+"_px_"+ptbins[j]+"_"+ptbins[j+1]), scale, 0);
                 else if(j==0||j==1||j==2) data.second->OSetProp(data.second->OGetHisto1(list2DName[h]+list2D[i]+"_px_"+ptbins[j]+"_"+ptbins[j+1]), scale, 2);
                 else if(j==3||j==4||j==5) data.second->OSetProp(data.second->OGetHisto1(list2DName[h]+list2D[i]+"_px_"+ptbins[j]+"_"+ptbins[j+1]), scale, 4);
                 else data.second->OSetProp(data.second->OGetHisto1(list2DName[h]+list2D[i]+"_px_"+ptbins[j]+"_"+ptbins[j+1]), scale, 12);
                // std::cout<<"file: "<<mc.first<<std::endl;
                 std::cout<<"HH"<<std::endl;
                data.second->OSetColor(list2DName[h]+list2D[i]+"_px_"+ptbins[j]+"_"+ptbins[j+1],colorWheel[id],2);
                if(h==0 || h==3)datacproj[y]->OPlace(year[y]+"_"+list2DName[h]+list2D[i]+"_proj",data.second->OGetHisto1(list2DName[h]+list2D[i]+"_px_"+ptbins[j]+"_"+ptbins[j+1]),j+1,"logy","1");
                else datacproj[y]->OPlace(year[y]+"_"+list2DName[h]+list2D[i]+"_proj",data.second->OGetHisto1(list2DName[h]+list2D[i]+"_px_"+ptbins[j]+"_"+ptbins[j+1]),j+1,"","1");
                std::cout<<"HHH"<<std::endl;
             }
             datacproj[y]->OPlace(year[y]+"_"+list2DName[h]+list2D[i]+"_proj",datacproj[y]->OGetLegend("data"),sizeof(ptbins)/sizeof(ptbins[0])+1,"");
             //cproj[y]->ODraw(year[y]+"_"+list2DName[h]+list2D[i]+"_proj");
           }
           std::cout<<"J"<<std::endl;
           datacproj2[y]->OPlace(year[y]+"_"+list2D[i]+"_proj",datacproj[y]->OGetLegend("data"),8,"");
        }
    }

if(scale.Contains("nevents")){
    datac->OFlush("ResultsDATA/2D","png");
    datacproj[0]->OFlush("ResultsDATA/2D_proj","png");
    datacproj[1]->OFlush("ResultsDATA/2D_proj","png");
    datacproj2[0]->OFlush("ResultsDATA/2D_INT","png");
    datacproj2[1]->OFlush("ResultsDATA/2D_INT","png");
    datac->OFlush("ResultsDATA/2D","svg");
    datacproj[0]->OFlush("ResultsDATA/2D_proj","svg");
    datacproj[1]->OFlush("ResultsDATA/2D_proj","svg");
    datacproj2[0]->OFlush("ResultsDATA/2D_INT","svg");
    datacproj2[1]->OFlush("ResultsDATA/2D_INT","svg");
    datac->OFlush("ResultsDATA/2D","pdf");
    datacproj[0]->OFlush("ResultsDATA/2D_proj","pdf");
    datacproj[1]->OFlush("ResultsDATA/2D_proj","pdf");
    datacproj2[0]->OFlush("ResultsDATA/2D_INT","pdf");
    datacproj2[1]->OFlush("ResultsDATA/2D_INT","pdf");
    datac->OFlush("ResultsDATA/2D","eps");
    datacproj[0]->OFlush("ResultsDATA/2D_proj","eps");
    datacproj[1]->OFlush("ResultsDATA/2D_proj","eps");
    datacproj2[0]->OFlush("ResultsDATA/2D_INT","eps");
    datacproj2[1]->OFlush("ResultsDATA/2D_INT","eps");
}

    //integrated plots
    OCanvas *datacint = new OCanvas();
    id=-1;

    for(auto data : DATA){
        id++;
        if(data.first.Contains("LHC18b")){
            id=0;
        }
        Int_t y = 0;
        //data.second->OLoadPrint();

        if(data.first.Contains("LHC17")) y = 0;
        if(data.first.Contains("LHC18")) y = 1;
        if(id==0)datacint->OCreate(year[y],4,2);

           datacint->OCreate(data.first,3,2);
           data.second->OSetNameSpace("DmesonsForJetCorrelations.histosD0MBNX.");
           for(Int_t h = 0; h< 6;h++){
               if(h==2 || h==5){
                   data.second->OSetX(listIntName[h],0,100);
               }

             data.second->OSetProp(data.second->OGetHisto1(listIntName[h]), scale, 0);
             data.second->OSetColor(listIntName[h],colorWheel[id],2);
             if(h == 0)datacint->OLegend(year[y],data.second->OGetHisto1(listIntName[h]),data.first);
             if(h==2||h==5){
                 datacint->OPlace(data.first,data.second->OGetHisto1(listIntName[h]),h+1,"logy","1");
                 datacint->OPlace(year[y],data.second->OGetHisto1(listIntName[h]),h<3?h+1:h+2,"logy","0");
             }
             else{
                 datacint->OPlace(data.first,data.second->OGetHisto1(listIntName[h]),h+1,"","1");
                 datacint->OPlace(year[y],data.second->OGetHisto1(listIntName[h]),h<3?h+1:h+2,"","0");
             }


           }
           datacint->OForceStack(year[y],3,0,100);
           datacint->OForceStack(year[y],7,0,100);
           if(id ==0)datacint->OPlace(year[y],datacint->OGetLegend(year[y]),8,"");
        }
if(scale.Contains("nevents")){
    datacint->OFlush("ResultsDATA/1D","png");
    datacint->OFlush("ResultsDATA/1D","svg");
    datacint->OFlush("ResultsDATA/1D","pdf");
    datacint->OFlush("ResultsDATA/1D","eps");
}

    //******************************************************************
    //******************************************************************
    //***********************MC-DATA COMPARISON*************************
    //******************************************************************
    //******************************************************************

//

    OCanvas *ccomp= new OCanvas();
    OCanvas *ccompINT= new OCanvas();
if(scale.Contains("nentries")){
    for(auto mc : MC){
        for(auto data : DATA){
            TString dname = data.first;
            dname.ReplaceAll("_pass1","");
            if(!mc.first.Contains(dname)) continue;
            Int_t y = 0;
            if(data.first.Contains("LHC17")){ y = 0;}
            if(data.first.Contains("LHC18")){ y = 1;}
            std::cout<<"Comparison"<<data.first<<" "<<mc.first<<std::endl;

            for(Int_t i = 0; i< 2;i++){
                if(!(ccompINT->OGetCanvas(data.first+"_"+list2D[i]+"_proj"))){
                    ccompINT->OCreate(data.first+"_"+list2D[i]+"_proj",4,2);
                }
                for(Int_t h = 0; h< 6;h++){
                    if(!(ccomp->OGetCanvas(data.first+"_"+list2DName[h]+list2D[i]+"_proj"))){
                        ccomp->OCreate(data.first+"_"+list2DName[h]+list2D[i]+"_proj",3,3);
                    }
                    for(UInt_t j = 0; j< sizeof(ptbins)/sizeof(ptbins[0])-1;j++){
                        if(scale.Contains("nentries"))mc.second->OSetColor(list2DName[h]+list2D[i]+"_px_"+ptbins[j]+"_"+ptbins[j+1],colorWheel[0],1);
                        if(scale.Contains("nentries"))data.second->OSetColor(list2DName[h]+list2D[i]+"_px_"+ptbins[j]+"_"+ptbins[j+1],colorWheel[1],1);
                        if(data.first.Contains("LHC17c") && i==0 && h ==0 && j==0)ccomp->OLegend("gen",data.second->OGetHisto1(list2DName[h]+list2D[i]+"_px_"+ptbins[j]+"_"+ptbins[j+1]),"DATA");
                        if(data.first.Contains("LHC17c") && i==0 && h ==0 && j==0)ccomp->OLegend("gen",mc.second->OGetHisto1(list2DName[h]+list2D[i]+"_px_"+ptbins[j]+"_"+ptbins[j+1]),"MC");

                        if(h==0 || h==3)ccomp->OPlaceRatio(data.first+"_"+list2DName[h]+list2D[i]+"_proj",data.second->OGetHisto1(list2DName[h]+list2D[i]+"_px_"+ptbins[j]+"_"+ptbins[j+1]),mc.second->OGetHisto1(list2DName[h]+list2D[i]+"_px_"+ptbins[j]+"_"+ptbins[j+1]),j+1,"logy");
                        else ccomp->OPlaceRatio(data.first+"_"+list2DName[h]+list2D[i]+"_proj",data.second->OGetHisto1(list2DName[h]+list2D[i]+"_px_"+ptbins[j]+"_"+ptbins[j+1]),mc.second->OGetHisto1(list2DName[h]+list2D[i]+"_px_"+ptbins[j]+"_"+ptbins[j+1]),j+1,"");

                    }
                    if(scale.Contains("nentries"))mc.second->OSetColor(list2DName[h]+list2D[i]+"_px_"+ptint[0]+"_"+ptint[1],colorWheel[0],1);
                    if(scale.Contains("nentries"))data.second->OSetColor(list2DName[h]+list2D[i]+"_px_"+ptint[0]+"_"+ptint[1],colorWheel[1],1);
                    if(h==0 || h==3)ccompINT->OPlaceRatio(data.first+"_"+list2D[i]+"_proj",data.second->OGetHisto1(list2DName[h]+list2D[i]+"_px_"+ptint[0]+"_"+ptint[1]),mc.second->OGetHisto1(list2DName[h]+list2D[i]+"_px_"+ptint[0]+"_"+ptint[1]),h<3?h+1:h+2,"logy");
                    else ccompINT->OPlaceRatio(data.first+"_"+list2D[i]+"_proj",data.second->OGetHisto1(list2DName[h]+list2D[i]+"_px_"+ptint[0]+"_"+ptint[1]),mc.second->OGetHisto1(list2DName[h]+list2D[i]+"_px_"+ptint[0]+"_"+ptint[1]),h<3?h+1:h+2,"");
                    ccompINT->OPlace(data.first+"_"+list2D[i]+"_proj",ccomp->OGetLegend("gen"),8);
                    ccomp->OPlace(data.first+"_"+list2DName[h]+list2D[i]+"_proj",ccomp->OGetLegend("gen"),sizeof(ptbins)/sizeof(ptbins[0])+1);
                }
            }
        }
    }

ccomp->OFlush("COMPARISON","png");
ccomp->OFlush("COMPARISON","svg");
ccompINT->OFlush("COMPARISON_INT","png");
ccompINT->OFlush("COMPARISON_INT","svg");
ccomp->OFlush("COMPARISON","pdf");
ccomp->OFlush("COMPARISON","eps");
ccompINT->OFlush("COMPARISON_INT","pdf");
ccompINT->OFlush("COMPARISON_INT","eps");
}


}
