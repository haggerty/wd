#include "TString.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TTree.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TSystem.h"
#include "TFile.h"
#include "TROOT.h"
#include "TH1.h"
#include "TF1.h"

#include <iostream>

Int_t channel_to_canvas( Int_t channel )
{

  Int_t chanmap[] = {
    3,2,19,18,35,34,51,50,
    1,0,17,16,33,32,49,48,
    7,6,23,22,39,38,55,54,
    5,4,21,20,37,36,53,52,
    11,10,27,26,43,42,59,58,
    9,8,25,24,41,40,57,56,
    15,14,31,30,47,46,63,62,
    13,12,29,28,45,44,61,60

  /*
    // First attempt; found reversal or Meritec cable
    2,3,18,19,34,35,50,51,
    0,1,16,17,32,33,48,49,
    6,7,22,23,38,39,54,55,
    4,5,20,21,36,37,52,53,
    10,11,26,27,42,43,58,59,
    8,9,24,25,40,41,56,57,
    14,15,30,31,46,47,62,63,
    12,13,28,29,44,45,60,61
  */

  };


  if ( channel < 0 || channel > 63 ) return 0;
  
  Int_t index = 0;
  for (Int_t i = 0; i < 64; i++ ) {
    if ( channel == chanmap[i] ) {
      index = i;
    }
  }

  return index+1;

}

Int_t emc3peak_column( TString filename, Int_t column = -1 )
{
  
  // column 0-7 left to right
  // column < 0 will draw all columns

  gROOT->SetStyle("Modern");
  //  gStyle->SetOptFit(111);
  gStyle->SetOptFit(0);
  gStyle->SetOptStat(0);

  TFile *f = new TFile( filename );
  TTree *W = (TTree *)f->Get("W");

  TCanvas *c1 = new TCanvas("c1","EMCAL",640,480);
  //  c1->Divide(8,8);
  //  c1->Divide(2,2);

  TSystem *sys = new TSystem();

  TString adc;
  TString name;
  Int_t tower_number = 0;
  Int_t noverlay = 0;
  for (Int_t i = 0; i < 64; i++ ) { 
    name = "EMC";
    name += tower_number;

    adc = "TMath::Abs(peak[";
    adc += i;
    adc += "]-pedestal[";
    adc += i;
    adc += "])";

    W->SetAlias(name,adc);
    cout << name << " " << adc << endl;
    tower_number++;
  }

  TString drawstring;
  TString cutstring;
  TString hname;
  TH1F *h;
  TF1 *fit;
  Float_t mpv[64];
  Float_t dmpv[64];
  for (Int_t i = 0; i < 64; i++ ) {
    hname = "h";
    hname += i;
    drawstring = "EMC";
    drawstring += i;
    drawstring += ">>";
    drawstring += hname;
    drawstring += "(150,0.0,300.0)";
    
    cutstring = "EMC";
    cutstring += i;
    cutstring += ">15.0";
    
    //    c1->cd( channel_to_canvas(i) );
    if ( (channel_to_canvas(i)-1)%8 == column || column < 0 ) {
      gPad->SetLogy();
      W->SetLineColor(noverlay+1);
      cout << i << " Draw(" << drawstring << "," << cutstring << ")" << endl;
      if ( noverlay == 0 ) {
	W->Draw(drawstring,cutstring);
      } else {
	W->Draw(drawstring,cutstring,"same");
      }
      f->GetObject(hname,h);
      TString title = "EMCAL MIP in column ";
      title += column;
      title += " (";
      title += sys->BaseName( filename );
      title += ")";
      h->SetTitle(title);
      noverlay++;
    }
    //    color++;
  }
  
  // at top
  //  TSystem *sys = new TSystem();
  TString pdffilename = "emc3peak_column_";
  pdffilename += column;
  pdffilename += "_";
  pdffilename += sys->BaseName( filename );
  pdffilename.ReplaceAll(".root",".pdf");
  c1->Print(pdffilename,"pdf Portrait");

  return 0;

}
