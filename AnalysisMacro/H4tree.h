//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Jul 24 18:09:45 2015 by ROOT version 5.34/30
// from TTree H4tree/H4 testbeam tree
// found on file: root://eoscms//eos/cms/store/group/dpg_ecal/alca_ecalcalib/TimingTB_H2_Jul2015/raw/DataTree/3314/1.root
//////////////////////////////////////////////////////////

#ifndef H4tree_h
#define H4tree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class H4tree {
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
   ULong64_t       evtTime[1];   //[nEvtTimes]
   UInt_t          evtTimeBoard[1];   //[nEvtTimes]
   UInt_t          nAdcChannels;
   UInt_t          adcBoard[32];   //[nAdcChannels]
   UInt_t          adcChannel[32];   //[nAdcChannels]
   UInt_t          adcData[32];   //[nAdcChannels]
   UInt_t          nTdcChannels;
   UInt_t          tdcBoard[22];   //[nTdcChannels]
   UInt_t          tdcChannel[22];   //[nTdcChannels]
   UInt_t          tdcData[22];   //[nTdcChannels]
   UInt_t          nDigiSamples;
   UInt_t          digiFrequency[27648];   //[nDigiSamples]
   UInt_t          digiGroup[27648];   //[nDigiSamples]
   UInt_t          digiChannel[27648];   //[nDigiSamples]
   UInt_t          digiSampleIndex[27648];   //[nDigiSamples]
   Float_t         digiSampleValue[27648];   //[nDigiSamples]
   UInt_t          digiBoard[27648];   //[nDigiSamples]
   UInt_t          nScalerWords;
   UInt_t          scalerWord[8];   //[nScalerWords]
   UInt_t          scalerBoard[8];   //[nScalerWords]
   UInt_t          nPatterns;
   UInt_t          pattern[1];   //[nPatterns]
   UInt_t          patternBoard[1];   //[nPatterns]
   UInt_t          patternChannel[1];   //[nPatterns]
   UInt_t          nTriggerWords;
   UInt_t          triggerWords[1];   //[nTriggerWords]
   UInt_t          triggerWordsBoard[1];   //[nTriggerWords]

   // List of branches
   TBranch        *b_runNumber;   //!
   TBranch        *b_spillNumber;   //!
   TBranch        *b_evtNumber;   //!
   TBranch        *b_evtTimeDist;   //!
   TBranch        *b_evtTimeStart;   //!
   TBranch        *b_nEvtTimes;   //!
   TBranch        *b_evtTime;   //!
   TBranch        *b_evtTimeBoard;   //!
   TBranch        *b_nAdcChannels;   //!
   TBranch        *b_adcBoard;   //!
   TBranch        *b_adcChannel;   //!
   TBranch        *b_adcData;   //!
   TBranch        *b_nTdcChannels;   //!
   TBranch        *b_tdcBoard;   //!
   TBranch        *b_tdcChannel;   //!
   TBranch        *b_tdcData;   //!
   TBranch        *b_nDigiSamples;   //!
   TBranch        *b_digiFrequency;   //!
   TBranch        *b_digiGroup;   //!
   TBranch        *b_digiChannel;   //!
   TBranch        *b_digiSampleIndex;   //!
   TBranch        *b_digiSampleValue;   //!
   TBranch        *b_digiBoard;   //!
   TBranch        *b_nScalerWords;   //!
   TBranch        *b_scalerWord;   //!
   TBranch        *b_scalerBoard;   //!
   TBranch        *b_nPatterns;   //!
   TBranch        *b_pattern;   //!
   TBranch        *b_patternBoard;   //!
   TBranch        *b_patternChannel;   //!
   TBranch        *b_nTriggerWords;   //!
   TBranch        *b_triggerWords;   //!
   TBranch        *b_triggerWordsBoard;   //!

   H4tree(TTree *tree=0);
   virtual ~H4tree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef H4tree_cxx
H4tree::H4tree(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("root://eoscms//eos/cms/store/group/dpg_ecal/alca_ecalcalib/TimingTB_H2_Jul2015/raw/DataTree/3314/1.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("root://eoscms//eos/cms/store/group/dpg_ecal/alca_ecalcalib/TimingTB_H2_Jul2015/raw/DataTree/3314/1.root");
      }
      f->GetObject("H4tree",tree);

   }
   Init(tree);
}

H4tree::~H4tree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t H4tree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t H4tree::LoadTree(Long64_t entry)
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

void H4tree::Init(TTree *tree)
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
   fChain->SetBranchAddress("evtTime", evtTime, &b_evtTime);
   fChain->SetBranchAddress("evtTimeBoard", evtTimeBoard, &b_evtTimeBoard);
   fChain->SetBranchAddress("nAdcChannels", &nAdcChannels, &b_nAdcChannels);
   fChain->SetBranchAddress("adcBoard", adcBoard, &b_adcBoard);
   fChain->SetBranchAddress("adcChannel", adcChannel, &b_adcChannel);
   fChain->SetBranchAddress("adcData", adcData, &b_adcData);
   fChain->SetBranchAddress("nTdcChannels", &nTdcChannels, &b_nTdcChannels);
   fChain->SetBranchAddress("tdcBoard", tdcBoard, &b_tdcBoard);
   fChain->SetBranchAddress("tdcChannel", tdcChannel, &b_tdcChannel);
   fChain->SetBranchAddress("tdcData", tdcData, &b_tdcData);
   fChain->SetBranchAddress("nDigiSamples", &nDigiSamples, &b_nDigiSamples);
   fChain->SetBranchAddress("digiFrequency", digiFrequency, &b_digiFrequency);
   fChain->SetBranchAddress("digiGroup", digiGroup, &b_digiGroup);
   fChain->SetBranchAddress("digiChannel", digiChannel, &b_digiChannel);
   fChain->SetBranchAddress("digiSampleIndex", digiSampleIndex, &b_digiSampleIndex);
   fChain->SetBranchAddress("digiSampleValue", digiSampleValue, &b_digiSampleValue);
   fChain->SetBranchAddress("digiBoard", digiBoard, &b_digiBoard);
   fChain->SetBranchAddress("nScalerWords", &nScalerWords, &b_nScalerWords);
   fChain->SetBranchAddress("scalerWord", scalerWord, &b_scalerWord);
   fChain->SetBranchAddress("scalerBoard", scalerBoard, &b_scalerBoard);
   fChain->SetBranchAddress("nPatterns", &nPatterns, &b_nPatterns);
   fChain->SetBranchAddress("pattern", &pattern, &b_pattern);
   fChain->SetBranchAddress("patternBoard", &patternBoard, &b_patternBoard);
   fChain->SetBranchAddress("patternChannel", &patternChannel, &b_patternChannel);
   fChain->SetBranchAddress("nTriggerWords", &nTriggerWords, &b_nTriggerWords);
   fChain->SetBranchAddress("triggerWords", triggerWords, &b_triggerWords);
   fChain->SetBranchAddress("triggerWordsBoard", triggerWordsBoard, &b_triggerWordsBoard);
   Notify();
}

Bool_t H4tree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void H4tree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t H4tree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef H4tree_cxx
