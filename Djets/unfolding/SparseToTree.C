#include "config.h"
#include "TLeaf.h"

void SparseToTree()
{

// Read the THnSparse and plot projection
    TFile *File = new TFile("/mnt/hgfs/vmware/data_200519/MC/AnalysisResults_LHC16kl.root","read");
    TDirectoryFile* dir=(TDirectoryFile*)File->Get("DmesonsForJetCorrelations");
    TString histName;
    if(fDmesonSpecie) histName = "histosDStarMBN";
                    else histName = "histosD0MBN";
    TList *histList[NDMC];
    THnSparseF *sparseMC[NDMC];
    THnSparseF *h = nullptr;

    for(int i=0; i<2; i++){
        histList[i] =  (TList*)dir->Get(Form("%s%dMCrec",histName.Data(),i));
        sparseMC[i] = (THnSparseF*)histList[i]->FindObject("ResponseMatrix");
        if(!i)h = dynamic_cast<THnSparseF*>(sparseMC[0]->Clone("ResponseMatrixSum"));
        else h->Add(sparseMC[i]);
    }
   /* h->GetAxis(5)->SetRangeUser(0,1.2);
    h->GetAxis(6)->SetRangeUser(0,100);
    h->GetAxis(7)->SetRangeUser(0,40);
    h->GetAxis(8)->SetRangeUser(-1.2,1.2);
    h->GetAxis(9)->SetRangeUser(-1.2,1.2);*/
    TH1D* hsparse = dynamic_cast<TH1D*>(h->Projection(9));

//copy from here: https://root.cern.ch/root/html/tutorials/tree/drawsparse.C.html
// Creates a TTree and fills it with the coordinates of all
// filled bins. The tree will have one branch for each dimension,
// and one for the bin content.
   TFile *File2 = new TFile("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts/Default_AnalysisResults_Run2.root/RM.root","RECREATE");
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

   Int_t *bins = new Int_t[dim];
   for (Long64_t i = 0; i < h->GetNbins(); ++i) {
      x[dim] = h->GetBinContent(i, bins);
      for (Int_t d = 0; d < dim; ++d) {
         x[d] = h->GetAxis(d)->GetBinCenter(bins[d]);
      }

      tree->Fill();

   }
   std::cout<<"tree entries "<< tree->GetEntries()<<std::endl;
   File2->Write(nullptr,TObject::kOverwrite);
   //File2->Write();
   File2->Close();

   Double_t jmatch[13];
   Double_t bincontent;

// Read the TTree and plot histogram
   TH1D* htree = new TH1D("htree","htree",150,-1.5,1.5);
   TFile *treefile = new TFile("/home/kvapil/work/analysis/pp_run2/D0jet/BaseCuts/Default_AnalysisResults_Run2.root/RM.root","READ");
   TTree *tree_ = dynamic_cast<TTree*>(treefile->Get("ResponseMatrixSum_tree"));
   tree_->SetBranchAddress("coord",&jmatch);
   tree_->SetBranchAddress("bincontent",&bincontent);
//   TBranch *br_coord = tree_->GetBranch("coord");
//   TLeaf * leaf_axis9 = br_coord->GetLeaf("axis9");
//   TLeaf * leaf_bincontent = tree_->GetLeaf("bincontent");
   htree->Sumw2();
   for(int i=0; i<tree_->GetEntries(); i++){
     tree_->GetEntry(i);
     //Float_t lv = leaf_bincontent->GetValue();
     //for(Int_t l = 1; l <= lv;l++)
     //htree->Fill(leaf_axis9->GetValue(),lv);
     if(bincontent < 1) std::cout<<bincontent<<std::endl;
     htree->Fill(jmatch[9],bincontent);
   }

   for(Int_t bin = 1;bin<htree->GetXaxis()->GetNbins();bin++)
       htree->SetBinError(bin,TMath::Sqrt(htree->GetBinContent(bin)));

   htree->SetLineColor(kRed);
   TLegend *l = new TLegend(0.5,0.2,0.9,0.5);
   l->AddEntry(hsparse,"sparse","lp");
   l->AddEntry(htree,"tree","lp");

//compare the 2 histograms

   htree->Draw();
   hsparse->Draw("same");
   l->Draw();

   delete [] bins;
   //delete [] x;
}
