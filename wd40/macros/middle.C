#include "TString.h"
#include <iostream>

Int_t middle( TString filename )
{
  
  gROOT->SetStyle("Modern");
  
  TFile *f = new TFile( filename );

  TString adc;
  TString name;
  
  for (Int_t i = 0; i < 16; i++ ) { 

    name = "ADC";
    name += i;

    adc = "TMath::Abs(peak[";
    adc += 160 + i;
    adc += "]-pedestal[";
    adc += 160 + i;
    adc += "])";

    W->SetAlias(name,adc);
    cout << name << " " << adc << endl;

  }

  TCanvas *c = new TCanvas("c","Hodoscope",8*240,480);
  c->Divide(8,2);

  Int_t can = 1;
  TString drawstring;
  TString cutstring;
  TString hname;
  for (Int_t i = 0; i < 16; i++ ) {
    c->cd(can);
    drawstring = "ADC";
    drawstring += i;
    drawstring += ">>";
    hname = "h";
    hname += i;
    drawstring += hname;
    drawstring += "(100,0,500)";

    cutstring = "ADC";
    cutstring += i;
    cutstring += ">10";
    std::cout << drawstring << " " << cutstring << std::endl;
    //    std::cout << drawstring << std::endl;
    W->Draw(drawstring,cutstring);
    //    W->Draw(drawstring);

    can++;
  }

}
