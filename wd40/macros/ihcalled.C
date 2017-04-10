#include "TString.h"
#include <iostream>

Int_t ihcalled( TString filename )
{
  
  gROOT->SetStyle("Modern");
  gStyle->SetOptFit(111);

  //     1   2  3  4
  //     5   6  7  8
  //     9  10 11 12
  //     13 14 15 16                                                                  
  Int_t whichcan[] = { 13, 9, 5, 1,
                       14, 10, 6, 2,
                       15, 11, 7, 3,
                       16, 12, 8, 4 };

  TFile *f = new TFile( filename );
  TCanvas *c1 = new TCanvas("c1","IHCAL Low",640,480);
  c1->Divide(4,4);
  //  c1->Divide(2,2);

  TString adc;
  TString name;
  Int_t tower_number = 1;
  for (Int_t i = 65; i < 96; i+=2 ) { 
    name = "IL";
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
  for (Int_t i = 0; i < 16; i++ ) {
    drawstring = "IL";
    drawstring += i+1;
    
    cutstring = drawstring;
    cutstring += ">6.0";

    std::cout << i+1 << " Draw(" << drawstring << "," << cutstring << ")" << std::endl;
    
    c1->cd(whichcan[i]);
    W->Draw(drawstring,cutstring);
    //    W->Fit("gaus",drawstring,cutstring);

  }
  
  TSystem *sys = new TSystem();
  TString pdffile = "ihcalled_";
  pdffile += sys->BaseName( filename );
  pdffile.ReplaceAll(".root",".pdf");
  std::cout << pdffile << std:: endl;
  c1->Print(pdffile);

  
}
