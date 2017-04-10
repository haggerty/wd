#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"

Int_t ohcaldisp( Char_t *filename, Int_t event, Double_t minadc = 0.0, Double_t maxadc = 2100.0 )
{

  gSystem->Load("onepulse_C.so");

  TFile *f = new TFile( filename );
  TTree *W;
  f->GetObject("W",W);

  TCanvas *cpeven = new TCanvas("cpeven", "High gain inner HCAL",0,0,640,640);
  cpeven->Divide(4,4);
  TCanvas *cpodd = new TCanvas("cpodd", "Low gain inner HCAL",0,0,640,640);
  cpodd->Divide(4,4);
  
  Int_t spot[] = {13,9,5,1,
		  14,10,6,2,
		  15,11,7,3,
		  16,12,8,4};
		  

  Int_t first_channel = 112;
  Int_t ican = 0;
  for ( Int_t channel = first_channel; channel < first_channel + 32; channel+=2 ) {
    cpeven->cd( spot[ican] );
    onepulse(W,channel,event,minadc,maxadc);
    cpodd->cd( spot[ican] );
    onepulse(W,channel+1,event,minadc,maxadc);
    ican++;
  }

  return 0;
}
