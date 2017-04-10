#include <iostream>
#include <iomanip>

#include "TString.h"
#include "TFile.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TF1.h"
#include "TTree.h"
#include "TSystem.h"
#include "TSpectrum.h"
#include "TMath.h"

Int_t ihcalhightriggeredpeak( TString filename )
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
  TCanvas *c1 = new TCanvas("c1","IHCAL High Gain",640,480);
  c1->Divide(4,4);
  //  c1->Divide(2,2);
  TTree *W  = (TTree*)f->Get("W");

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
  
  //  Float_t mpv[16] = {0.0};
  //  Float_t dmpv[16] = {0.0};

  TSpectrum *s[16];
  TH1F *h[16];
  Int_t npeaks[16];

  TString drawstring;
  TString hname;
  TString pname;
  Int_t ican = 0;
    for (Int_t i = 0; i < 16; i++ ) {
  //  for (Int_t i = 8; i < 12; i++ ) {
  //  for (Int_t i = 12; i < 16; i++ ) {
    drawstring = "IH";
    drawstring += i+1;
    
    std::cout << drawstring << std::endl;

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
      cutstring += "IH";
      cutstring += j+1;
      //      cutstring += ">10.0 && ";
      cutstring += ">50.0 && ";
    }
    cutstring += drawstring;
    cutstring += ">50.0 && ";
    cutstring += drawstring;
    cutstring += "<1900.0";

    std::cout << i+1 << " Draw(" << drawstring << "," << cutstring << ")" << std::endl;
    
    c1->cd(whichcan[ican++]);
    //    W->Draw(drawstring,cutstring);
    
    cout << drawstring << "," << cutstring << endl;
    hname = "h";
    hname += i;
    //    W->Fit("landau",drawstring,cutstring);
    //    W->Fit("landau",drawstring+">>"+hname,cutstring);
    W->Draw(drawstring+">>"+hname,cutstring,"goff");

    h[i] = (TH1F*) gROOT->FindObject( hname );

    s[i] = new TSpectrum(2);
    npeaks[i] = s[i]->Search(h[i]);
    s[i]->Print();
    
  }

  TSystem *sys = new TSystem();
  TString pdffile = "ihcalhightriggeredpeak_";
  pdffile += sys->BaseName( filename );
  pdffile.ReplaceAll(".root",".pdf");
  std::cout << pdffile << std:: endl;
  c1->Print(pdffile);
  
  Float_t *xpeaks;
  Float_t hg[16],lg[16];
  for (Int_t i = 0; i < 16; i++ ) {
    //    s[i]->Print();
    xpeaks = s[i]->GetPositionX();
    //    std::cout << i << " " << npeaks[i] << std::endl;
    for (Int_t ipeak = 0; ipeak < min(npeaks[i],1); ipeak++ ) {
      hg[i] = xpeaks[ipeak];
      lg[i] = xpeaks[ipeak]/32.0;
      std::cout << i << " " << ipeak 
		<< " HG: " << hg[i] << " LG: " << lg[i] << std::endl;
    }
  }
  
  std::cout << "High gain: " << TMath::Mean(16,hg) 
	    << " +/- " << TMath::RMS(16,hg) << std::endl; 

  return 0;
}
