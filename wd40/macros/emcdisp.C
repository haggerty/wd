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


Int_t emcdisp( TTree *W, TCanvas *cp, Int_t event, Double_t minadc = 1948.0, Double_t maxadc = 2148.0 )
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
    cp->cd( channel_to_canvas_odd(channel) );
    onepulse(W,channel,event,minadc,maxadc);
  }

  return 0;
}
