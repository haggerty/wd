#define ledtempco_cxx
// The class definition in ledtempco.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// Root > T->Process("ledtempco.C")
// Root > T->Process("ledtempco.C","some options")
// Root > T->Process("ledtempco.C+")
//

#include "ledtempco.h"
#include <TH2.h>
#include <TStyle.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TProfile.h>
#include <TF1.h>
#include <TGraph.h>

TH2F *meanvst[64];
TH1F *hslope;

void ledtempco::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

   TString hname;
   TString name;
   for ( Int_t i = 0; i < 64; i++ ) {
     hname = "mvst";
     hname += i;
     name = "LED vs T ";
     name += i;
     meanvst[i] = new TH2F(hname,name,100,25.0,35.0,200,0.0,4000.0);
   }
   hslope = new TH1F("hslope","Fractional change in LED with temperature;(dADC(T)/dT)/ADC(30 #circC)",50,0.0,0.1);

}

void ledtempco::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

Bool_t ledtempco::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either ledtempco::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

  fChain->GetEntry( entry );

  for ( Int_t i = 0; i < 64; i++ ) {
    if ( mean[tower[i]] > 0.0 && mean[tower[i]] < 2000.0 ) {
      // wrong?
      //      meanvst[tower[i]]->Fill(temperature[tower[i]],mean[tower[i]]);
      // right?
      meanvst[tower[i]]->Fill(temperature[i],mean[tower[i]]);
    }
  }


   return kTRUE;
}

void ledtempco::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void ledtempco::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

  TCanvas *c1 = new TCanvas("c1","LED vs T",640,480);
  c1->Divide(8,8);

  Int_t can = 0;
  TProfile *p[64];
  for ( Int_t i = 0; i < 64; i++ ) {
    c1->cd(++can);
    meanvst[i]->Draw();
    p[i] = meanvst[i]->ProfileX();
  }

  TCanvas *c2 = new TCanvas("c2","LED vs T",640,480);
  c2->Divide(8,8);
  TF1 *fit;
  Float_t slope[64];
  Float_t dslope[64];
  Float_t intercept[64];
  Float_t dintercept[64];
  Float_t peak30;
  can = 0;
  for ( Int_t i = 0; i < 64; i++ ) {
    c2->cd(++can);
    p[i]->Draw();
    p[i]->Fit("pol1");
    fit = (TF1*) p[i]->GetFunction("pol1");
    if ( fit ) {
      intercept[i] = fit->GetParameter(0);
      dintercept[i] = fit->GetParError(0);
      slope[i] = fit->GetParameter(1);
      dslope[i] = fit->GetParError(1);
      peak30 = intercept[i] + 30.0*slope[i];
      hslope->Fill(slope[i]/peak30);
    }
  }

  TCanvas *c3 = new TCanvas("c3","Slopes",640,480);
  hslope->Draw();
  hslope->Fit("gaus");
  gStyle->SetOptFit(111);

  TString pdffile = "ledtempco.pdf";
  c1->Print(pdffile+"(","pdf Portrait");
  c2->Print(pdffile,"pdf Portrait");
  c3->Print(pdffile+")","pdf Portrait");

}
