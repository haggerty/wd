#include "TString.h"
#include <iostream>

Int_t ohcalallhigh( TString filename )
{
  
  gROOT->SetStyle("Modern");
  gStyle->SetOptFit(111);

  Int_t whichcan[] = { 13, 9, 5, 1, 
                       14, 10, 6, 2,
                       15, 11, 7, 3,
                       16, 12, 8, 4 };

  TFile *f = new TFile( filename );
  TCanvas *c1 = new TCanvas("c1","OHCAL Low",640,480);
  c1->Divide(4,4);

  TString adc;
  TString name;
  Int_t tower_number = 1;
  for (Int_t i = 112; i < 144; i+=2 ) { 
    name = "OH";
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
  Int_t ican = 0;
  for (Int_t i = 0; i < 16; i++ ) {
  //  for (Int_t i = 8; i < 12; i++ ) {
    drawstring = "OH";
    drawstring += i+1;
    
    cutstring = drawstring;
    cutstring += ">12.0";

    std::cout << i+1 << " Draw(" << drawstring << "," << cutstring << ")" << std::endl;
    
    c1->cd(whichcan[ican++]);
    gPad->SetLogy();
    //    W->Draw(drawstring,cutstring);
    //    W->Fit("landau",drawstring,cutstring);
    W->Fit("gaus",drawstring,cutstring);

  }
  
  TSystem *sys = new TSystem();
  TString pdffilename = "ohcalallhigh_";
  pdffilename += sys->BaseName( filename );
  pdffilename.ReplaceAll(".root",".pdf");
  c1->Print(pdffilename,"pdf Portrait");

}
