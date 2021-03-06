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


Int_t channel_to_canvas_even( Int_t channel )
{

  Int_t canmap[] = {
  // 1 ... 15
  11+48,10+48,9+48,8+48,15+48,14+48,13+48,12+48,
  // 9 ... 16
  3+48,2+48,1+48,0+48,7+48,6+48,5+48,4+48,

  // 17 ... 24
  11+32,10+32,9+32,8+32,15+32,14+32,13+32,12+32,
  // 25 ... 32
  3+32,2+32,1+32,0+32,7+32,6+32,5+32,4+32,

  // 33 ... 40
  11+16,10+16,9+16,8+16,15+16,14+16,13+16,12+16,
  // 41 42 43 44 45 46 47 48
  3+16,2+16,1+16,0+16,7+16,6+16,5+16,4+16,

  // 49 50 51 52 53 54 55 56
  11,10,9,8,15,14,13,12,
  // 57 58 59 60 61 62 63 64
  3,2,1,0,7,6,5,4
  };

  if ( channel < 0 || channel > 63 ) return 0;
  return canmap[channel]+1;

}


Int_t channel_to_canvas_odd( Int_t channel )
{

  Int_t canmap[] = {
  // 1 ... 15
  10+48,11+48,8+48,9+48,14+48,15+48,12+48,13+48,
  // 9 ... 16
  2+48,3+48,0+48,1+48,6+48,7+48,4+48,5+48,

  // 17 ... 24
  10+32,11+32,8+32,9+32,14+32,15+32,12+32,13+32,
  // 25 ... 32
  2+32,3+32,0+32,1+32,6+32,7+32,4+32,5+32,

  // 33 ... 40
  10+16,11+16,8+16,9+16,14+16,15+16,12+16,13+16,
  // 41 42 43 44 45 46 47 48
  2+16,3+16,0+16,1+16,6+16,7+16,4+16,5+16,

  // 49 50 51 52 53 54 55 56
  10,11,8,9,14,15,12,13,
  // 57 58 59 60 61 62 63 64
  2,3,0,1,6,7,4,5

  };

  if ( channel < 0 || channel > 63 ) return 0;
  return canmap[channel]+1;

}


Int_t emclego( TTree *W, Int_t event)
{
  
  // column 0-7 left to right
  // column < 0 will draw all columns

  gROOT->SetStyle("Modern");
  gStyle->SetOptFit(111);

  TCanvas *c1 = new TCanvas("c1","EMCAL",640,640);


  TString adc;
  TString name;
  Int_t tower_number = 1;
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
    drawstring += i+1;
    drawstring += ">>";
    drawstring += hname;
    drawstring += "(150,0.0,300.0)";
    
    cutstring = "EMC";
    cutstring += i+1;
    cutstring += ">15.0";
    
    std::cout << i+1 << " Draw(" << drawstring << "," << cutstring << ")" << std::endl;
    
    c1->cd( channel_to_canvas_odd(i) );
    column = (channel_to_canvas_odd(i)-1)%8;
    row = (channel_to_canvas_odd(i)-1)/8;

    if ( (channel_to_canvas_odd(i)-1)%8 == column || column < 0 ) {
      gPad->SetLogy();
      //    W->Draw(drawstring,cutstring);
      W->Fit("landau",drawstring,cutstring);
      
      f->GetObject(hname,h);
      fit = (TF1*) h->GetFunction("landau");
      mpv[i] = fit->GetParameter(1);
      dmpv[i] = fit->GetParError(1);
    }

  }
  
  TSystem *sys = new TSystem();
  TString pdffilename = "emclego_";
  pdffilename += sys->BaseName( filename );
  pdffilename.ReplaceAll(".root",".png");
  //  c1->Print(pdffilename,"pdf Portrait");
  c1->Print(pdffilename);

  return 0;

}
