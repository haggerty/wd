#include "TCanvas.h"
#include "TStyle.h"
#include "TTree.h"
#include "TString.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TSpline.h"
#include "TSystem.h"
#include "TFile.h"

#include <iostream>

Int_t channel_to_canvas( Int_t channel )
{

  Int_t chanmap[] = {
    3,2,19,18,35,34,51,50,
    1,0,17,16,33,32,49,48,
    7,6,23,22,39,38,55,54,
    5,4,21,20,37,36,53,52,
    11,10,27,26,43,42,59,58,
    9,8,25,24,41,40,57,56,
    15,14,31,30,47,46,63,62,
    13,12,29,28,45,44,61,60
    // First attempt before checking reveral on Meritec cable
    /*
    2,3,18,19,34,35,50,51,
    0,1,16,17,32,33,48,49,
    6,7,22,23,38,39,54,55,
    4,5,20,21,36,37,52,53,
    10,11,26,27,42,43,58,59,
    8,9,24,25,40,41,56,57,
    14,15,30,31,46,47,62,63,
    12,13,28,29,44,45,60,61
    */
  };

  if ( channel < 0 || channel > 63 ) return 0;
  
  Int_t index = 0;
  for (Int_t i = 0; i < 64; i++ ) {
    if ( channel == chanmap[i] ) {
      index = i;
    }
  }

  return index+1;

}

Int_t onepulse( TTree *W, Int_t channel = 0, Int_t event = 1, Double_t minadc = 1948.0, Double_t maxadc = 2148.0 )
{
  const int NRSMP = 24;
  Double_t times[NRSMP];
  for ( Int_t i = 0; i < NRSMP; i++ ) { times[i] = (Double_t) i ; };
  TString cutstring;
  if ( event == 0 ) {
    cutstring = "";
  } else {
    cutstring = "eventnumber == ";
    cutstring += event;
  }
  TString plotstring = "adc[";
  plotstring += channel;
  plotstring += "]";
  W->Draw(plotstring,cutstring);
  TGraph *gr = new TGraph( W->GetSelectedRows(), times, W->GetV1() );
  gr->Draw("AP");
  gr->SetMarkerStyle(20);
  gr->SetMarkerSize( 0.4 );

  TString filename = gSystem->BaseName(W->GetCurrentFile()->GetName());
  TString grtitle = filename;
  grtitle += " " + plotstring + " " + cutstring + ";Sample;ADC";
  gr->SetTitle( grtitle );
  gr->GetYaxis()->SetRangeUser( minadc, maxadc );
  gr->Draw("AP");
  TSpline3 *sp = new TSpline3( "sp", gr );
  sp->Draw("same");

  plotstring = "peak[";
  plotstring += channel;
  plotstring += "]:time[";
  plotstring += channel;
  plotstring += "]";

  W->Draw(plotstring,cutstring,"goff");
  TGraph *grmax = new TGraph( W->GetSelectedRows(), W->GetV2(), W->GetV1() );
  grmax->SetMarkerColor( kRed );
  grmax->SetMarkerStyle( 20 );
  grmax->SetMarkerSize( 0.4 );
  grmax->Draw("psame");

  plotstring = "pedestal[";
  plotstring += channel;
  plotstring += "]";

  W->Draw(plotstring,cutstring,"goff");
  TGraph *grped = new TGraph( W->GetSelectedRows(), times, W->GetV1() );
  grped->SetMarkerColor( kGreen );
  grped->SetMarkerStyle( 20 );
  grped->SetMarkerSize( 0.4 );
  grped->Draw("psame");

  return 0;
}


Int_t emc3disp( TTree *W, TCanvas *cp, Int_t event, Double_t minadc = 1948.0, Double_t maxadc = 2148.0 )
{

  //  gSystem->Load("onepulse_C.so");

  //  TFile *f = new TFile( filename );
  //  TTree *W;
  //  f->GetObject("W",W);

  //  TCanvas *cp = new TCanvas("cp", "EMCAL",0,0,640,640);
  //  cp->Divide(8,8);
  
  Int_t first_channel = 0;
  for ( Int_t channel = first_channel; channel < first_channel + 64; channel++ ) {
    //    cp->cd( channel - first_channel + 1 );
    cp->cd( channel_to_canvas(channel) );
    onepulse(W,channel,event,minadc,maxadc);
  }

  return 0;
}
