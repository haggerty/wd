#include "TStyle.h"
#include "TTree.h"
#include "TString.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TSpline.h"
#include "TSystem.h"
#include "TFile.h"

Int_t onepulse( TTree *W, Int_t channel = 0, Int_t event = 1, Double_t minadc = 1948.0, Double_t maxadc = 2148.0, TString drawopt = "AP" )
{
  const int NRSMP = 24;
  Double_t times[NRSMP];
  for ( Int_t i = 0; i < NRSMP; i++ ) { times[i] = (Double_t) i ; };

  TString cutstring;
  cutstring = "eventnumber == ";
  cutstring += event;

  TString plotstring = "adc[";
  plotstring += channel;
  plotstring += "]";
  W->Draw(plotstring,cutstring,"goff");
  TGraph *gr = new TGraph( W->GetSelectedRows(), times, W->GetV1() );
  gr->Draw(drawopt);
  gr->SetMarkerStyle(20);
  gr->SetMarkerSize( 0.4 );

  TString filename = gSystem->BaseName(W->GetCurrentFile()->GetName());
  TString grtitle = filename;
  grtitle += " " + plotstring + " " + cutstring + ";Sample;ADC";
  gr->SetTitle( grtitle );
  gr->GetYaxis()->SetRangeUser( minadc, maxadc );
  gr->Draw(drawopt);
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
