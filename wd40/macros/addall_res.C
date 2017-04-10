Double_t calresq( Double_t *x, Double_t *par )
{

  // add two components in quadrature
  return TMath::Sqrt( TMath::Power(par[0]/TMath::Sqrt(x[0]),2) + TMath::Power(par[1],2) );

}

Double_t calresa( Double_t *x, Double_t *par )
{

  // add two componenets algebraically
  return ( par[0]/TMath::Sqrt(x[0]) ) + par[1];

}

Int_t addall_res( TString filename = "addall.dat", TString fitfun = "pol2" )
{
  gROOT->SetStyle("Modern");
  gStyle->SetOptFit(111);

  gStyle->SetStatX(0.5);
  gStyle->SetStatY(0.85);

  TCanvas *cl = new TCanvas("cl","ETOT",640,480);

  TGraphErrors *linear = new TGraphErrors( filename, "%*s %lg %lg %lg" );
  linear->SetMarkerStyle(20);
  linear->SetMarkerSize(0.8);
  linear->SetTitle("Total energy linearity");
  linear->GetXaxis()->SetTitle("Beam energy (GeV)");
  linear->GetYaxis()->SetTitle("Peak ADC (counts)");
  linear->GetYaxis()->SetTitleOffset(1.4);
  linear->Draw("AP");
  linear->Fit( fitfun );
  
  //  TPaveStats *st = (TPaveStats*)linear->FindObject("stats");
  //  st->SetX1NDC(0.8); //new x start position
  //  st->SetX2NDC(0.8); //new x end position

  gStyle->SetOptFit(0);
  TCanvas *cr = new TCanvas("cr","sPHENIX",640,480);

  TLegend *leg = new TLegend(0.5597484,0.75,0.8899371,0.8915929,NULL,"brNDC");

  leg->SetFillColor(0);


  TGraphErrors *res = new TGraphErrors( filename, "%*s %lg %*lg %*lg %lg %lg" );
  res->SetMarkerStyle(20);
  res->SetMarkerSize(0.4);
  res->GetYaxis()->SetRangeUser(0.0,1.0);
  res->SetTitle("ETOT resolution");
  res->GetXaxis()->SetTitle("Beam energy (GeV)");
  res->GetYaxis()->SetTitle("#sigma(peak)/mean(peak)");
  res->GetYaxis()->SetTitleOffset(1.4);
  res->Draw("AP");

  /*
  TF1 *fitfunq = new TF1( "fitfunq", calresq, 0.0, 50.0 , 2 );
  res->Fit("fitfunq","R");

  // put the fit results on the plot

  Double_t a = fitfunq->GetParameter(0);
  Double_t b = fitfunq->GetParameter(1);
  cout << "Fit results: a: " << a << " b: " << b << endl;
  Char_t fitleg[80];
  sprintf(fitleg,"Fit: %.2f%%/#sqrt{E} #oplus %.2f%%",100.0*a,100.0*b);
  leg->AddEntry(fitfunq,fitleg,"L");
  
  TF1 *fitfuna = new TF1( "fitfuna", calresa, 0.0, 50.0, 2 );
  fitfuna->SetLineColor( kGreen );
  res->Fit("fitfuna","R+");
  
  // put the fit results on the plot

  a = fitfuna->GetParameter(0);
  b = fitfuna->GetParameter(1);
  cout << "Fit results: a: " << a << " b: " << b << endl;
  sprintf(fitleg,"Fit: %.2f%%/#sqrt{E} + %.2f%%",100.0*a,100.0*b);
  leg->AddEntry(fitfuna,fitleg,"L");
  
  leg->Draw();
  */

  TString pdffilename = "linear_";
  pdffilename += filename;
  pdffilename.ReplaceAll(".dat",".pdf");
  cl->Print(pdffilename,"Portrait pdf");

  pdffilename = "resolution_";
  pdffilename += filename;
  pdffilename.ReplaceAll(".dat",".pdf");
  cr->Print(pdffilename,"Portrait pdf");

  return 0;
}
