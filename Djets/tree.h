//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Jul 13 11:17:22 2020 by ROOT version 6.16/00
// from TTree AliAnalysisTaskDmesonJets_D0_MCTruth/AliAnalysisTaskDmesonJets_D0_MCTruth
// found on file: /mnt/hgfs/vmware/DjetSim4R/AnalysisResults_FastSim_powheg+pythia6_dijet46.root
//////////////////////////////////////////////////////////

#ifndef tree_h
#define tree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "AliAnalysisTaskDmesonJets.h"
#include "vector"
#include "AliAnalysisTaskDmesonJets.h"
#include "AliAnalysisTaskDmesonJets.h"
#include "vector"
#include "AliAnalysisTaskDmesonJets.h"

class tree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.
   static constexpr Int_t kMaxDmesons = 8;
   static constexpr Int_t kMaxJet_AKTChargedR020_pt_scheme = 8;
   static constexpr Int_t kMaxJet_AKTChargedR030_pt_scheme = 8;
   static constexpr Int_t kMaxJet_AKTChargedR040_pt_scheme = 8;
   static constexpr Int_t kMaxJet_AKTChargedR060_pt_scheme = 8;

   // Declaration of leaf types
 //AliAnalysisTaskDmesonJets::AliEventInfoSummary *Event;
   Double32_t      fWeight;
   Double32_t      fPtHard;
   Int_t           Dmesons_;
   Double32_t      Dmesons_fPt[kMaxDmesons];   //[Dmesons_]
   Double32_t      Dmesons_fEta[kMaxDmesons];   //[Dmesons_]
   Double32_t      Dmesons_fPhi[kMaxDmesons];   //[Dmesons_]
   Double32_t      Dmesons_fPartonType[kMaxDmesons];   //[Dmesons_]
   Double32_t      Dmesons_fPartonPt[kMaxDmesons];   //[Dmesons_]
   UShort_t        Dmesons_fAncestorPDG[kMaxDmesons];   //[Dmesons_]
   Int_t           Jet_AKTChargedR020_pt_scheme_;
   Double32_t      Jet_AKTChargedR020_pt_scheme_fPt[kMaxJet_AKTChargedR020_pt_scheme];   //[Jet_AKTChargedR020_pt_scheme_]
   Double32_t      Jet_AKTChargedR020_pt_scheme_fEta[kMaxJet_AKTChargedR020_pt_scheme];   //[Jet_AKTChargedR020_pt_scheme_]
   Double32_t      Jet_AKTChargedR020_pt_scheme_fPhi[kMaxJet_AKTChargedR020_pt_scheme];   //[Jet_AKTChargedR020_pt_scheme_]
   Double32_t      Jet_AKTChargedR020_pt_scheme_fR[kMaxJet_AKTChargedR020_pt_scheme];   //[Jet_AKTChargedR020_pt_scheme_]
   Double32_t      Jet_AKTChargedR020_pt_scheme_fZ[kMaxJet_AKTChargedR020_pt_scheme];   //[Jet_AKTChargedR020_pt_scheme_]
   Double32_t      Jet_AKTChargedR020_pt_scheme_fN[kMaxJet_AKTChargedR020_pt_scheme];   //[Jet_AKTChargedR020_pt_scheme_]
   Int_t           Jet_AKTChargedR030_pt_scheme_;
   Double32_t      Jet_AKTChargedR030_pt_scheme_fPt[kMaxJet_AKTChargedR030_pt_scheme];   //[Jet_AKTChargedR030_pt_scheme_]
   Double32_t      Jet_AKTChargedR030_pt_scheme_fEta[kMaxJet_AKTChargedR030_pt_scheme];   //[Jet_AKTChargedR030_pt_scheme_]
   Double32_t      Jet_AKTChargedR030_pt_scheme_fPhi[kMaxJet_AKTChargedR030_pt_scheme];   //[Jet_AKTChargedR030_pt_scheme_]
   Double32_t      Jet_AKTChargedR030_pt_scheme_fR[kMaxJet_AKTChargedR030_pt_scheme];   //[Jet_AKTChargedR030_pt_scheme_]
   Double32_t      Jet_AKTChargedR030_pt_scheme_fZ[kMaxJet_AKTChargedR030_pt_scheme];   //[Jet_AKTChargedR030_pt_scheme_]
   Double32_t      Jet_AKTChargedR030_pt_scheme_fN[kMaxJet_AKTChargedR030_pt_scheme];   //[Jet_AKTChargedR030_pt_scheme_]
   Int_t           Jet_AKTChargedR040_pt_scheme_;
   Double32_t      Jet_AKTChargedR040_pt_scheme_fPt[kMaxJet_AKTChargedR040_pt_scheme];   //[Jet_AKTChargedR040_pt_scheme_]
   Double32_t      Jet_AKTChargedR040_pt_scheme_fEta[kMaxJet_AKTChargedR040_pt_scheme];   //[Jet_AKTChargedR040_pt_scheme_]
   Double32_t      Jet_AKTChargedR040_pt_scheme_fPhi[kMaxJet_AKTChargedR040_pt_scheme];   //[Jet_AKTChargedR040_pt_scheme_]
   Double32_t      Jet_AKTChargedR040_pt_scheme_fR[kMaxJet_AKTChargedR040_pt_scheme];   //[Jet_AKTChargedR040_pt_scheme_]
   Double32_t      Jet_AKTChargedR040_pt_scheme_fZ[kMaxJet_AKTChargedR040_pt_scheme];   //[Jet_AKTChargedR040_pt_scheme_]
   Double32_t      Jet_AKTChargedR040_pt_scheme_fN[kMaxJet_AKTChargedR040_pt_scheme];   //[Jet_AKTChargedR040_pt_scheme_]
   Int_t           Jet_AKTChargedR060_pt_scheme_;
   Double32_t      Jet_AKTChargedR060_pt_scheme_fPt[kMaxJet_AKTChargedR060_pt_scheme];   //[Jet_AKTChargedR060_pt_scheme_]
   Double32_t      Jet_AKTChargedR060_pt_scheme_fEta[kMaxJet_AKTChargedR060_pt_scheme];   //[Jet_AKTChargedR060_pt_scheme_]
   Double32_t      Jet_AKTChargedR060_pt_scheme_fPhi[kMaxJet_AKTChargedR060_pt_scheme];   //[Jet_AKTChargedR060_pt_scheme_]
   Double32_t      Jet_AKTChargedR060_pt_scheme_fR[kMaxJet_AKTChargedR060_pt_scheme];   //[Jet_AKTChargedR060_pt_scheme_]
   Double32_t      Jet_AKTChargedR060_pt_scheme_fZ[kMaxJet_AKTChargedR060_pt_scheme];   //[Jet_AKTChargedR060_pt_scheme_]
   Double32_t      Jet_AKTChargedR060_pt_scheme_fN[kMaxJet_AKTChargedR060_pt_scheme];   //[Jet_AKTChargedR060_pt_scheme_]

   // List of branches
   TBranch        *b_Event_fWeight;   //!
   TBranch        *b_Event_fPtHard;   //!
   TBranch        *b_Dmesons_;   //!
   TBranch        *b_Dmesons_fPt;   //!
   TBranch        *b_Dmesons_fEta;   //!
   TBranch        *b_Dmesons_fPhi;   //!
   TBranch        *b_Dmesons_fPartonType;   //!
   TBranch        *b_Dmesons_fPartonPt;   //!
   TBranch        *b_Dmesons_fAncestorPDG;   //!
   TBranch        *b_Jet_AKTChargedR020_pt_scheme_;   //!
   TBranch        *b_Jet_AKTChargedR020_pt_scheme_fPt;   //!
   TBranch        *b_Jet_AKTChargedR020_pt_scheme_fEta;   //!
   TBranch        *b_Jet_AKTChargedR020_pt_scheme_fPhi;   //!
   TBranch        *b_Jet_AKTChargedR020_pt_scheme_fR;   //!
   TBranch        *b_Jet_AKTChargedR020_pt_scheme_fZ;   //!
   TBranch        *b_Jet_AKTChargedR020_pt_scheme_fN;   //!
   TBranch        *b_Jet_AKTChargedR030_pt_scheme_;   //!
   TBranch        *b_Jet_AKTChargedR030_pt_scheme_fPt;   //!
   TBranch        *b_Jet_AKTChargedR030_pt_scheme_fEta;   //!
   TBranch        *b_Jet_AKTChargedR030_pt_scheme_fPhi;   //!
   TBranch        *b_Jet_AKTChargedR030_pt_scheme_fR;   //!
   TBranch        *b_Jet_AKTChargedR030_pt_scheme_fZ;   //!
   TBranch        *b_Jet_AKTChargedR030_pt_scheme_fN;   //!
   TBranch        *b_Jet_AKTChargedR040_pt_scheme_;   //!
   TBranch        *b_Jet_AKTChargedR040_pt_scheme_fPt;   //!
   TBranch        *b_Jet_AKTChargedR040_pt_scheme_fEta;   //!
   TBranch        *b_Jet_AKTChargedR040_pt_scheme_fPhi;   //!
   TBranch        *b_Jet_AKTChargedR040_pt_scheme_fR;   //!
   TBranch        *b_Jet_AKTChargedR040_pt_scheme_fZ;   //!
   TBranch        *b_Jet_AKTChargedR040_pt_scheme_fN;   //!
   TBranch        *b_Jet_AKTChargedR060_pt_scheme_;   //!
   TBranch        *b_Jet_AKTChargedR060_pt_scheme_fPt;   //!
   TBranch        *b_Jet_AKTChargedR060_pt_scheme_fEta;   //!
   TBranch        *b_Jet_AKTChargedR060_pt_scheme_fPhi;   //!
   TBranch        *b_Jet_AKTChargedR060_pt_scheme_fR;   //!
   TBranch        *b_Jet_AKTChargedR060_pt_scheme_fZ;   //!
   TBranch        *b_Jet_AKTChargedR060_pt_scheme_fN;   //!

   tree(TTree *tree=0);
   virtual ~tree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef tree_cxx
tree::tree(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/mnt/hgfs/vmware/DjetSim4R/AnalysisResults_FastSim_powheg+pythia6_dijet46.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/mnt/hgfs/vmware/DjetSim4R/AnalysisResults_FastSim_powheg+pythia6_dijet46.root");
      }
      f->GetObject("AliAnalysisTaskDmesonJets_D0_MCTruth",tree);

   }
   Init(tree);
}

tree::~tree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t tree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t tree::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void tree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("fWeight", &fWeight, &b_Event_fWeight);
   fChain->SetBranchAddress("fPtHard", &fPtHard, &b_Event_fPtHard);
   fChain->SetBranchAddress("Dmesons", &Dmesons_, &b_Dmesons_);
   fChain->SetBranchAddress("Dmesons.fPt", Dmesons_fPt, &b_Dmesons_fPt);
   fChain->SetBranchAddress("Dmesons.fEta", Dmesons_fEta, &b_Dmesons_fEta);
   fChain->SetBranchAddress("Dmesons.fPhi", Dmesons_fPhi, &b_Dmesons_fPhi);
   fChain->SetBranchAddress("Dmesons.fPartonType", Dmesons_fPartonType, &b_Dmesons_fPartonType);
   fChain->SetBranchAddress("Dmesons.fPartonPt", Dmesons_fPartonPt, &b_Dmesons_fPartonPt);
   fChain->SetBranchAddress("Dmesons.fAncestorPDG", Dmesons_fAncestorPDG, &b_Dmesons_fAncestorPDG);
   fChain->SetBranchAddress("Jet_AKTChargedR020_pt_scheme", &Jet_AKTChargedR020_pt_scheme_, &b_Jet_AKTChargedR020_pt_scheme_);
   fChain->SetBranchAddress("Jet_AKTChargedR020_pt_scheme.fPt", Jet_AKTChargedR020_pt_scheme_fPt, &b_Jet_AKTChargedR020_pt_scheme_fPt);
   fChain->SetBranchAddress("Jet_AKTChargedR020_pt_scheme.fEta", Jet_AKTChargedR020_pt_scheme_fEta, &b_Jet_AKTChargedR020_pt_scheme_fEta);
   fChain->SetBranchAddress("Jet_AKTChargedR020_pt_scheme.fPhi", Jet_AKTChargedR020_pt_scheme_fPhi, &b_Jet_AKTChargedR020_pt_scheme_fPhi);
   fChain->SetBranchAddress("Jet_AKTChargedR020_pt_scheme.fR", Jet_AKTChargedR020_pt_scheme_fR, &b_Jet_AKTChargedR020_pt_scheme_fR);
   fChain->SetBranchAddress("Jet_AKTChargedR020_pt_scheme.fZ", Jet_AKTChargedR020_pt_scheme_fZ, &b_Jet_AKTChargedR020_pt_scheme_fZ);
   fChain->SetBranchAddress("Jet_AKTChargedR020_pt_scheme.fN", Jet_AKTChargedR020_pt_scheme_fN, &b_Jet_AKTChargedR020_pt_scheme_fN);
   fChain->SetBranchAddress("Jet_AKTChargedR030_pt_scheme", &Jet_AKTChargedR030_pt_scheme_, &b_Jet_AKTChargedR030_pt_scheme_);
   fChain->SetBranchAddress("Jet_AKTChargedR030_pt_scheme.fPt", Jet_AKTChargedR030_pt_scheme_fPt, &b_Jet_AKTChargedR030_pt_scheme_fPt);
   fChain->SetBranchAddress("Jet_AKTChargedR030_pt_scheme.fEta", Jet_AKTChargedR030_pt_scheme_fEta, &b_Jet_AKTChargedR030_pt_scheme_fEta);
   fChain->SetBranchAddress("Jet_AKTChargedR030_pt_scheme.fPhi", Jet_AKTChargedR030_pt_scheme_fPhi, &b_Jet_AKTChargedR030_pt_scheme_fPhi);
   fChain->SetBranchAddress("Jet_AKTChargedR030_pt_scheme.fR", Jet_AKTChargedR030_pt_scheme_fR, &b_Jet_AKTChargedR030_pt_scheme_fR);
   fChain->SetBranchAddress("Jet_AKTChargedR030_pt_scheme.fZ", Jet_AKTChargedR030_pt_scheme_fZ, &b_Jet_AKTChargedR030_pt_scheme_fZ);
   fChain->SetBranchAddress("Jet_AKTChargedR030_pt_scheme.fN", Jet_AKTChargedR030_pt_scheme_fN, &b_Jet_AKTChargedR030_pt_scheme_fN);
   fChain->SetBranchAddress("Jet_AKTChargedR040_pt_scheme", &Jet_AKTChargedR040_pt_scheme_, &b_Jet_AKTChargedR040_pt_scheme_);
   fChain->SetBranchAddress("Jet_AKTChargedR040_pt_scheme.fPt", Jet_AKTChargedR040_pt_scheme_fPt, &b_Jet_AKTChargedR040_pt_scheme_fPt);
   fChain->SetBranchAddress("Jet_AKTChargedR040_pt_scheme.fEta", Jet_AKTChargedR040_pt_scheme_fEta, &b_Jet_AKTChargedR040_pt_scheme_fEta);
   fChain->SetBranchAddress("Jet_AKTChargedR040_pt_scheme.fPhi", Jet_AKTChargedR040_pt_scheme_fPhi, &b_Jet_AKTChargedR040_pt_scheme_fPhi);
   fChain->SetBranchAddress("Jet_AKTChargedR040_pt_scheme.fR", Jet_AKTChargedR040_pt_scheme_fR, &b_Jet_AKTChargedR040_pt_scheme_fR);
   fChain->SetBranchAddress("Jet_AKTChargedR040_pt_scheme.fZ", Jet_AKTChargedR040_pt_scheme_fZ, &b_Jet_AKTChargedR040_pt_scheme_fZ);
   fChain->SetBranchAddress("Jet_AKTChargedR040_pt_scheme.fN", Jet_AKTChargedR040_pt_scheme_fN, &b_Jet_AKTChargedR040_pt_scheme_fN);
   fChain->SetBranchAddress("Jet_AKTChargedR060_pt_scheme", &Jet_AKTChargedR060_pt_scheme_, &b_Jet_AKTChargedR060_pt_scheme_);
   fChain->SetBranchAddress("Jet_AKTChargedR060_pt_scheme.fPt", Jet_AKTChargedR060_pt_scheme_fPt, &b_Jet_AKTChargedR060_pt_scheme_fPt);
   fChain->SetBranchAddress("Jet_AKTChargedR060_pt_scheme.fEta", Jet_AKTChargedR060_pt_scheme_fEta, &b_Jet_AKTChargedR060_pt_scheme_fEta);
   fChain->SetBranchAddress("Jet_AKTChargedR060_pt_scheme.fPhi", Jet_AKTChargedR060_pt_scheme_fPhi, &b_Jet_AKTChargedR060_pt_scheme_fPhi);
   fChain->SetBranchAddress("Jet_AKTChargedR060_pt_scheme.fR", Jet_AKTChargedR060_pt_scheme_fR, &b_Jet_AKTChargedR060_pt_scheme_fR);
   fChain->SetBranchAddress("Jet_AKTChargedR060_pt_scheme.fZ", Jet_AKTChargedR060_pt_scheme_fZ, &b_Jet_AKTChargedR060_pt_scheme_fZ);
   fChain->SetBranchAddress("Jet_AKTChargedR060_pt_scheme.fN", Jet_AKTChargedR060_pt_scheme_fN, &b_Jet_AKTChargedR060_pt_scheme_fN);
   Notify();
}

Bool_t tree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void tree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t tree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef tree_cxx
