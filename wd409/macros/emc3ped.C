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

    0,1,16,17,32,33,48,49,
    2,3,18,19,34,35,50,51,
    4,5,20,21,36,37,52,53,
    6,7,22,23,38,39,54,55,
    8,9,24,25,40,41,56,57,
    10,11,26,27,42,43,58,59,
    12,13,28,29,44,45,60,61,
    14,15,30,31,46,47,62,63

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

Int_t emc3ped( TString filename, Int_t column = -1, Int_t adccut = 120, Int_t nbins = 100, Float_t minadc = 0.0, Float_t maxadc = 3000.0 )
{
  
  // column 0-7 left to right
  // column < 0 will draw all columns

  gROOT->SetStyle("Modern");
  //  gStyle->SetOptFit(111);
  gStyle->SetOptFit(0);

  TFile *f = new TFile( filename );
  TTree *W = (TTree *)f->Get("W");

  TCanvas *c1 = new TCanvas("c1","EMCAL",640,480);
  c1->Divide(8,8);
  //  c1->Divide(2,2);

  TString adc;
  TString name;
  Int_t tower_number = 0;
  for (Int_t i = 0; i < 64; i++ ) { 
    name = "EMC";
    name += tower_number;

    adc = "pedestal[";
    adc += i;
    adc += "]";

    W->SetAlias(name,adc);
    cout << name << " " << adc << endl;
    tower_number++;
  }

  TString drawstring;
  TString cutstring;
  TString hname;
  TH1F *h;
  for (Int_t i = 0; i < 64; i++ ) {
    hname = "h";
    hname += i;
    drawstring = "EMC";
    drawstring += i;
    drawstring += ">>";
    drawstring += hname;
    drawstring += "(";
    drawstring += nbins;
    drawstring += ",";;
    drawstring += minadc;
    drawstring += ",";
    drawstring += maxadc;
    drawstring += ")";
    
    cutstring = "EMC";
    cutstring += i;
    cutstring += ">";
    cutstring += adccut;
    
    std::cout << i << " Draw(" << drawstring << "," << cutstring << ")" << std::endl;
    
    c1->cd( channel_to_canvas(i) );
    if ( (channel_to_canvas(i)-1)%8 == column || column < 0 ) {
      gPad->SetLogy();
      W->Draw(drawstring,cutstring);
    }

  }
  
  TSystem *sys = new TSystem();
  TString pdffilename = "emc3ped_";
  pdffilename += sys->BaseName( filename );
  pdffilename.ReplaceAll(".root",".pdf");
  c1->Print(pdffilename,"pdf Portrait");

  return 0;

}
