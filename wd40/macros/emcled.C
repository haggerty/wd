#include "TString.h"
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


Int_t emcled( TString filename )
{
  
  gROOT->SetStyle("Modern");
  gStyle->SetOptFit(111);

  TFile *f = new TFile( filename );
  TCanvas *c1 = new TCanvas("c1","EMCAL",640,480);
  c1->Divide(8,8);
  //  c1->Divide(2,2);
  
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

    std::cout << i+1 << " Draw(" << drawstring << "," << cutstring << ")" << std::endl;
    
    //    c1->cd( channel_to_canvas_odd(i) );
    c1->cd( i+1 );
    W->Draw(drawstring,cutstring);
    /*
    W->Fit("gaus",drawstring,cutstring);
    f->GetObject(hname,h);
    fit = (TF1*) h->GetFunction("gaus");
    mean[i] = fit->GetParameter(1);
    dmean[i] = fit->GetParError(1);
    sigma[i] = fit->GetParameter(2);
    dsigma[i] = fit->GetParError(2);
    
    hmean->Fill(mean[i]);
    hmean2->Fill(float(i),mean[i]);
    */
  }
  
  /*
  for (Int_t i = 0; i < 64; i++ ) {
    std::cout << i << " " << mean[i] << " " << dmean[i] << " "
	      << sigma[i] << " " << dsigma[i] << std::endl;
  }
  */

  TSystem *sys = new TSystem();
  TString pdffilename = "emcpeak_led_";
  pdffilename += sys->BaseName( filename );
  pdffilename.ReplaceAll(".root",".pdf");
  c1->Print(pdffilename,"pdf Portrait");

}
