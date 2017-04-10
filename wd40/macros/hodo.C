#include "TString.h"
#include <iostream>

Int_t hodo( TString filename )
{
  
  gROOT->SetStyle("Modern");
  
  TFile *f = new TFile( filename );

  TString adc;
  TString name;
  Int_t x = 96;
  Int_t y = 104;
  for (Int_t i = 0; i < 8; i++ ) { 

    // x (vertical fingers)

    name = "X";
    name += i;

    adc = "TMath::Abs(peak[";
    adc += x;
    adc += "]-pedestal[";
    adc += x;
    adc += "])";

    W->SetAlias(name,adc);
    cout << name << " " << adc << endl;
    x++;

    // y (horizontal fingers)

    name = "Y";
    name += i;

    adc = "TMath::Abs(peak[";
    adc += y;
    adc += "]-pedestal[";
    adc += y;
    adc += "])";

    W->SetAlias(name,adc);
    cout << name << " " << adc << endl;
    y++;

  }

  TString anyx = "X0>20||X1>20||X2>20||X3>20||X4>20||X5>20||X6>20||X7>20";
  W->SetAlias("anyx",anyx);
  TString anyy = "Y0>20||Y1>20||Y2>20||Y3>20||Y4>20||Y5>20||Y6>20||Y7>20";
  W->SetAlias("anyy",anyy);

  TCanvas *c = new TCanvas("c","Hodoscope",8*240,480);
  c->Divide(8,2);

  Int_t can = 1;
  TString drawstring;
  TString cutstring;
  for (Int_t i = 0; i < 8; i++ ) {
    c->cd(can);
    drawstring = "X";
    drawstring += i;
    cutstring = "X";
    cutstring += i;
    cutstring += ">25 && X";
    cutstring += i;
    cutstring += "<200";
    std::cout << drawstring << " " << cutstring << std::endl;
    W->Draw(drawstring,cutstring);

    c->cd(can+8);
    drawstring = "Y";
    drawstring += i;
    cutstring = drawstring;
    cutstring += ">25 && Y";
    cutstring += i;
    cutstring += "<200";
    std::cout << drawstring << " " << cutstring << std::endl;
    W->Draw(drawstring,cutstring);
    
    can++;
  }

  TSystem *sys = new TSystem();
  TString shortfilename = sys->BaseName( filename );
  TString pdffilename = "hodo_";
  pdffilename += shortfilename;
  pdffilename.ReplaceAll(".root",".pdf");
  c->Print(pdffilename,"pdf Portrait");


}
