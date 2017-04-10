//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Jun  2 17:06:04 2016 by ROOT version 5.34/34
// from TTree L/LED
// found on file: ledpeak.root
//////////////////////////////////////////////////////////

#ifndef ledtempco_h
#define ledtempco_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class ledtempco : public TSelector {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

   // Declaration of leaf types
   Int_t           event;
   Int_t           tower[64];
   Float_t         mean[64];
   Float_t         dmean[64];
   Float_t         sigma[64];
   Float_t         dsigma[64];
   Float_t         temperature[64];

   // List of branches
   TBranch        *b_event;   //!
   TBranch        *b_tower;   //!
   TBranch        *b_mean;   //!
   TBranch        *b_dmean;   //!
   TBranch        *b_sigma;   //!
   TBranch        *b_dsigma;   //!
   TBranch        *b_temperature;   //!

   ledtempco(TTree * /*tree*/ =0) : fChain(0) { }
   virtual ~ledtempco() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(ledtempco,0);
};

#endif

#ifdef ledtempco_cxx
void ledtempco::Init(TTree *tree)
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
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("tower", tower, &b_tower);
   fChain->SetBranchAddress("mean", mean, &b_mean);
   fChain->SetBranchAddress("dmean", dmean, &b_dmean);
   fChain->SetBranchAddress("sigma", sigma, &b_sigma);
   fChain->SetBranchAddress("dsigma", dsigma, &b_dsigma);
   fChain->SetBranchAddress("temperature", temperature, &b_temperature);
}

Bool_t ledtempco::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef ledtempco_cxx
