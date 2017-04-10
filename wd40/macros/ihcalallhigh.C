#include "TString.h"
#include <iostream>

Int_t ihcalallhigh( TString filename )
{
  
  gROOT->SetStyle("Modern");
  gStyle->SetOptFit(111);

  TFile *f = new TFile( filename );
  TCanvas *c1 = new TCanvas("c1","IHCAL High",640,480);
  c1->Divide(4,4);
  //  c1->Divide(2,2);

  TString adc;
  TString name;
  Int_t tower_number = 1;
  for (Int_t i = 64; i < 96; i+=2 ) { 
    name = "IH";
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
  Int_t ican = 1;
  for (Int_t i = 0; i < 16; i++ ) {
  //  for (Int_t i = 8; i < 12; i++ ) {
    drawstring = "IH";
    drawstring += i+1;
    
    cutstring = drawstring;
    cutstring += ">6.0";

    std::cout << i+1 << " Draw(" << drawstring << "," << cutstring << ")" << std::endl;
    
    c1->cd(ican++);
    gPad->SetLogy();
    //    W->Draw(drawstring,cutstring);
    W->Fit("gaus",drawstring,cutstring);

  }
  
  
  TSystem *sys = new TSystem();
  TString pdffilename = "ihcalallhigh_";
  pdffilename += sys->BaseName( filename );
  pdffilename.ReplaceAll(".root",".pdf");
  c1->Print(pdffilename,"pdf Portrait");

}
