//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue May 31 14:56:29 2016 by ROOT version 5.30/03
// from TTree W/Waveform digitizer raw data
// found on file: ../root/led_00002946-0000.root
//////////////////////////////////////////////////////////

#ifndef ledpeak_h
#define ledpeak_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>

class ledpeak : public TSelector {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

   // Declaration of leaf types
   Int_t           runnumber;
   Int_t           eventnumber;
   Int_t           nrmodules[2];
   Int_t           nrsamples[2];
   Int_t           parity[2];
   Int_t           trigger[4];
   Int_t           bclk[4];
   Int_t           moduleid[4];
   Int_t           adc[192][24];
   Float_t         mean[192];
   Float_t         rms[192];
   Float_t         pedestal[192];
   Float_t         pedsig[192];
   Float_t         peak[192];
   Float_t         time[192];
   Float_t         integral[192];
   Float_t         integral2[192];
   Int_t           summary[2];

   // List of branches
   TBranch        *b_runnumber;   //!
   TBranch        *b_eventnumber;   //!
   TBranch        *b_nrmodules;   //!
   TBranch        *b_nrsamples;   //!
   TBranch        *b_parity;   //!
   TBranch        *b_trigger;   //!
   TBranch        *b_bclk;   //!
   TBranch        *b_moduleid;   //!
   TBranch        *b_adc;   //!
   TBranch        *b_mean;   //!
   TBranch        *b_rms;   //!
   TBranch        *b_pedestal;   //!
   TBranch        *b_pedsig;   //!
   TBranch        *b_peak;   //!
   TBranch        *b_time;   //!
   TBranch        *b_integral;   //!
   TBranch        *b_integral2;   //!
   TBranch        *b_summary;   //!

   ledpeak(TTree * /*tree*/ =0) { }
   virtual ~ledpeak() { }
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

   ClassDef(ledpeak,0);
};

#endif

#ifdef ledpeak_cxx
void ledpeak::Init(TTree *tree)
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

   fChain->SetBranchAddress("runnumber", &runnumber, &b_runnumber);
   fChain->SetBranchAddress("eventnumber", &eventnumber, &b_eventnumber);
   fChain->SetBranchAddress("nrmodules", nrmodules, &b_nrmodules);
   fChain->SetBranchAddress("nrsamples", nrsamples, &b_nrsamples);
   fChain->SetBranchAddress("parity", parity, &b_parity);
   fChain->SetBranchAddress("trigger", trigger, &b_trigger);
   fChain->SetBranchAddress("bclk", bclk, &b_bclk);
   fChain->SetBranchAddress("moduleid", moduleid, &b_moduleid);
   fChain->SetBranchAddress("adc", adc, &b_adc);
   fChain->SetBranchAddress("mean", mean, &b_mean);
   fChain->SetBranchAddress("rms", rms, &b_rms);
   fChain->SetBranchAddress("pedestal", pedestal, &b_pedestal);
   fChain->SetBranchAddress("pedsig", pedsig, &b_pedsig);
   fChain->SetBranchAddress("peak", peak, &b_peak);
   fChain->SetBranchAddress("time", time, &b_time);
   fChain->SetBranchAddress("integral", integral, &b_integral);
   fChain->SetBranchAddress("integral2", integral2, &b_integral2);
   fChain->SetBranchAddress("summary", summary, &b_summary);
}

Bool_t ledpeak::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef ledpeak_cxx
