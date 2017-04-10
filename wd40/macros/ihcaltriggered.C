#include "TString.h"
#include <iostream>

Int_t ihcaltriggered( TString filename )
{
  
  //     1   2  3  4
  //     5   6  7  8
  //     9  10 11 12
  //     13 14 15 16

  Int_t whichcan[] = { 13, 9, 5, 1, 
		       14, 10, 6, 2,
		       15, 11, 7, 3,
		       16, 12, 8, 4 };

  gROOT->SetStyle("Modern");
  gStyle->SetOptFit(111);

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
  TString hname;
  Int_t ican = 0;
  for (Int_t i = 0; i < 16; i++ ) {
  //  for (Int_t i = 8; i < 12; i++ ) {
    drawstring = "IL";
    drawstring += i+1;
    
    //    std::cout << drawstring << std::endl;

    /*
1 234 0 123
2 134 1 023
3 124 2 013
4 123 3 012
5 678 4 567
6 578 5 467
    */
    
  Int_t bottom = (i/4)*4;

    TString cutstring = "";
    for (Int_t j = bottom; j < bottom+4; j++ ) {
      if ( j == i ) continue;
      cutstring += "IL";
      cutstring += j+1;
      cutstring += ">6.0 && ";
    }
    cutstring += drawstring;
    cutstring += "<200.0";

    std::cout << i+1 << " Draw(" << drawstring << "," << cutstring << ")" << std::endl;
    
    c1->cd(whichcan[ican++]);
    //    W->Draw(drawstring,cutstring);
    
    cout << drawstring << "," << cutstring << endl;
    W->Fit("landau",drawstring,cutstring);

  }

  TSystem *sys = new TSystem();
  TString pdffile = "ihcaltriggered_";
  pdffile += sys->BaseName( filename );
  pdffile.ReplaceAll(".root",".pdf");
  std::cout << pdffile << std:: endl;
  c1->Print(pdffile);
  
  
}
