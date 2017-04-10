#include "TString.h"
#include <iostream>

#include "emc3map.C"

Int_t emc3led( TString filename )
{
  
  gROOT->SetStyle("Modern");
  gStyle->SetOptFit(111);

  TFile *f = new TFile( filename );
  TCanvas *c1 = new TCanvas("c1","EMCAL",640,480);
  c1->Divide(8,8);
  //  c1->Divide(2,2);
  
  W->SetAlias("SPILLmax","maxadc[160]");
  W->SetAlias("SPILLmin","minadc[160]");

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
  Float_t mean[64];
  Float_t dmean[64];
  Float_t sigma[64];
  Float_t dsigma[64];
  TH1F *hmean = new TH1F("mean","EMCAL LED Fit Mean",100,0.0,2000.0);
  TH2F *hmean2 = new TH2F("mean2","EMCAL LED Fit Mean",64,0.0,64.0,100,0.0,2000.0);
  for (Int_t i = 0; i < 64; i++ ) {
    hname = "h";
    hname += i;

    drawstring = "EMC";
    drawstring += i+1;
    drawstring += ">>";
    drawstring += hname;
    //    drawstring += "(100,0.0,200.0)";
    
    cutstring = "EMC";
    cutstring += i+1;
    cutstring += ">15.0";
    cutstring += " && SPILLmin>1848 && SPILLmin<2248";

    std::cout << i+1 << " Draw(" << drawstring << "," << cutstring << ")" << std::endl;
    
    // ADC order
    //    c1->cd( i+1 );

    // tower order
    c1->cd( channel_to_canvas(i) );

    W->Draw(drawstring,cutstring);

    W->Fit("gaus",drawstring,cutstring);
    f->GetObject(hname,h);
    fit = (TF1*) h->GetFunction("gaus");
    mean[i] = fit->GetParameter(1);
    dmean[i] = fit->GetParError(1);
    sigma[i] = fit->GetParameter(2);
    dsigma[i] = fit->GetParError(2);
    
    hmean->Fill(mean[i]);
    hmean2->Fill(float(i),mean[i]);

  }
  

  for (Int_t i = 0; i < 64; i++ ) {
    std::cout << i << " " << mean[i] << " " << dmean[i] << " "
	      << sigma[i] << " " << dsigma[i] << std::endl;
  }

  TSystem *sys = new TSystem();
  TString pdffilename = "emc3led_";
  pdffilename += sys->BaseName( filename );
  pdffilename.ReplaceAll(".root",".pdf");
  c1->Print(pdffilename,"pdf Portrait");

}
