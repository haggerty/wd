#include "TString.h"
#include <iostream>

Int_t tilemapper( TString filename )
{
  
  gROOT->SetStyle("Modern");
  gStyle->SetOptFit(111);

  TFile *f = new TFile( filename );
  TCanvas *c1 = new TCanvas("c1","Tiletester high gain",640,480);
  c1->Divide(4,2);
  //  c1->Divide(2,2);

  TString adc;
  TString name;
  Int_t tower_number = 1;
  for (Int_t i = 176; i < 192; i+=2 ) { 
    name = "TH";
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
  for (Int_t i = 0; i < 8; i++ ) {
    hname = "tt";
    hname += i;

    drawstring = "TH";
    drawstring += i+1;
    drawstring += ">>";
    drawstring += hname;
    drawstring += "(100,0.0,500.0)";
    
    cutstring = "TH";
    cutstring += i+1;
    cutstring += ">25.0";

    //    cutstring += " && TH1>50 && TH8>100";

    std::cout << i+1 << " Draw(" << drawstring << "," << cutstring << ")" << std::endl;
    
    c1->cd(ican++);
    //    W->Draw(drawstring,cutstring);
    W->Fit("landau",drawstring,cutstring);

  }
  
  TSystem *sys = new TSystem();
  TString pdffilename = "tilemapper_";
  pdffilename += sys->BaseName( filename );
  pdffilename.ReplaceAll(".root",".pdf");
  c1->Print(pdffilename,"pdf Portrait");

}
