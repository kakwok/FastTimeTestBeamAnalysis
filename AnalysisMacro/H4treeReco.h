//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Jul 24 11:53:17 2015 by ROOT version 5.34/30
// from TTree H4treeReco/H4treeReco
// found on file: root://eoscms//eos/cms/store/cmst3/group/hgcal/TimingTB_H2_Jul2015/RECO/6b4cb13/RECO_3374.root
//////////////////////////////////////////////////////////

#ifndef H4treeReco_h
#define H4treeReco_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class H4treeReco {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   UInt_t          runNumber;
   UInt_t          spillNumber;
   UInt_t          evtNumber;
   UInt_t          evtTimeDist;
   UInt_t          evtTimeStart;
   UInt_t          nEvtTimes;
   UInt_t          nwc;
   Float_t         wc_recox[2];   //[nwc]
   Float_t         wc_recoy[2];   //[nwc]
   UInt_t          wc_xl_hits[2];   //[nwc]
   UInt_t          wc_xr_hits[2];   //[nwc]
   UInt_t          wc_yu_hits[2];   //[nwc]
   UInt_t          wc_yd_hits[2];   //[nwc]
   UInt_t          maxch;
   Float_t         group[4];   //[maxch]
   Float_t         ch[4];   //[maxch]
   Float_t         pedestal[4];   //[maxch]
   Float_t         pedestalRMS[4];   //[maxch]
   Float_t         wave_max[4];   //[maxch]
   Float_t         wave_max_aft[4];   //[maxch]
   Float_t         wave_aroundmax[4][50];   //[maxch]
   Float_t         time_aroundmax[4][50];   //[maxch]
   Float_t         charge_integ[4];   //[maxch]
   Float_t         charge_integ_max[4];   //[maxch]
   Float_t         t_max[4];   //[maxch]
   Float_t         t_max_frac30[4];   //[maxch]
   Float_t         t_max_frac50[4];   //[maxch]
   Float_t         t_at_threshold[4];   //[maxch]
   Float_t         t_over_threshold[4];   //[maxch]

   // List of branches
   TBranch        *b_runNumber;   //!
   TBranch        *b_spillNumber;   //!
   TBranch        *b_evtNumber;   //!
   TBranch        *b_evtTimeDist;   //!
   TBranch        *b_evtTimeStart;   //!
   TBranch        *b_nEvtTimes;   //!
   TBranch        *b_nwc;   //!
   TBranch        *b_wc_recox;   //!
   TBranch        *b_wc_recoy;   //!
   TBranch        *b_wc_xl_hits;   //!
   TBranch        *b_wc_xr_hits;   //!
   TBranch        *b_wc_yu_hits;   //!
   TBranch        *b_wc_yd_hits;   //!
   TBranch        *b_maxch;   //!
   TBranch        *b_group;   //!
   TBranch        *b_ch;   //!
   TBranch        *b_pedestal;   //!
   TBranch        *b_pedestalRMS;   //!
   TBranch        *b_wave_max;   //!
   TBranch        *b_wave_max_aft;   //!
   TBranch        *b_wave_aroundmax;   //!
   TBranch        *b_time_aroundmax;   //!
   TBranch        *b_charge_integ;   //!
   TBranch        *b_charge_integ_max;   //!
   TBranch        *b_t_max;   //!
   TBranch        *b_t_max_frac30;   //!
   TBranch        *b_t_max_frac50;   //!
   TBranch        *b_t_at_threshold;   //!
   TBranch        *b_t_over_threshold;   //!

   H4treeReco(TTree *tree=0);
   virtual ~H4treeReco();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef H4treeReco_cxx
H4treeReco::H4treeReco(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("root://eoscms//eos/cms/store/cmst3/group/hgcal/TimingTB_H2_Jul2015/RECO/6b4cb13/RECO_3374.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("root://eoscms//eos/cms/store/cmst3/group/hgcal/TimingTB_H2_Jul2015/RECO/6b4cb13/RECO_3374.root");
      }
      f->GetObject("H4treeReco",tree);

   }
   Init(tree);
}

H4treeReco::~H4treeReco()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t H4treeReco::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t H4treeReco::LoadTree(Long64_t entry)
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

void H4treeReco::Init(TTree *tree)
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

   fChain->SetBranchAddress("runNumber", &runNumber, &b_runNumber);
   fChain->SetBranchAddress("spillNumber", &spillNumber, &b_spillNumber);
   fChain->SetBranchAddress("evtNumber", &evtNumber, &b_evtNumber);
   fChain->SetBranchAddress("evtTimeDist", &evtTimeDist, &b_evtTimeDist);
   fChain->SetBranchAddress("evtTimeStart", &evtTimeStart, &b_evtTimeStart);
   fChain->SetBranchAddress("nEvtTimes", &nEvtTimes, &b_nEvtTimes);
   fChain->SetBranchAddress("nwc", &nwc, &b_nwc);
   fChain->SetBranchAddress("wc_recox", wc_recox, &b_wc_recox);
   fChain->SetBranchAddress("wc_recoy", wc_recoy, &b_wc_recoy);
   fChain->SetBranchAddress("wc_xl_hits", wc_xl_hits, &b_wc_xl_hits);
   fChain->SetBranchAddress("wc_xr_hits", wc_xr_hits, &b_wc_xr_hits);
   fChain->SetBranchAddress("wc_yu_hits", wc_yu_hits, &b_wc_yu_hits);
   fChain->SetBranchAddress("wc_yd_hits", wc_yd_hits, &b_wc_yd_hits);
   fChain->SetBranchAddress("maxch", &maxch, &b_maxch);
   fChain->SetBranchAddress("group", group, &b_group);
   fChain->SetBranchAddress("ch", ch, &b_ch);
   fChain->SetBranchAddress("pedestal", pedestal, &b_pedestal);
   fChain->SetBranchAddress("pedestalRMS", pedestalRMS, &b_pedestalRMS);
   fChain->SetBranchAddress("wave_max", wave_max, &b_wave_max);
   fChain->SetBranchAddress("wave_max_aft", wave_max_aft, &b_wave_max_aft);
   fChain->SetBranchAddress("wave_aroundmax", wave_aroundmax, &b_wave_aroundmax);
   fChain->SetBranchAddress("time_aroundmax", time_aroundmax, &b_time_aroundmax);
   fChain->SetBranchAddress("charge_integ", charge_integ, &b_charge_integ);
   fChain->SetBranchAddress("charge_integ_max", charge_integ_max, &b_charge_integ_max);
   fChain->SetBranchAddress("t_max", t_max, &b_t_max);
   fChain->SetBranchAddress("t_max_frac30", t_max_frac30, &b_t_max_frac30);
   fChain->SetBranchAddress("t_max_frac50", t_max_frac50, &b_t_max_frac50);
   fChain->SetBranchAddress("t_at_threshold", t_at_threshold, &b_t_at_threshold);
   fChain->SetBranchAddress("t_over_threshold", t_over_threshold, &b_t_over_threshold);
   Notify();
}

Bool_t H4treeReco::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void H4treeReco::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t H4treeReco::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef H4treeReco_cxx
