{

  TGraphErrors *gr1 = new TGraphErrors("emcal4.dat","%lg %*s %lg %lg");
  TGraphErrors *gr2 = new TGraphErrors("emcal4.dat","%lg %*s %*lg %*lg %lg %lg");
  TGraphErrors *gr3 = new TGraphErrors("emcal4.dat","%lg %*s %*lg %*lg %*lg %*lg %lg %lg");
  TGraphErrors *gr4 = new TGraphErrors("emcal4.dat","%lg %*s %*lg %*lg %*lg %*lg %*lg %*lg %lg %lg");

  gr2->SetMarkerColor(2);  
  gr2->SetLineColor(2);
  gr3->SetMarkerColor(3);  
  gr3->SetLineColor(3);
  gr4->SetMarkerColor(4);  
  gr4->SetLineColor(4);

  TMultiGraph *gr = new TMultiGraph();

  gr->Add(gr1);
  gr->Add(gr2);
  gr->Add(gr3);
  gr->Add(gr4);

  gr->SetTitle("Longitudinal scan of St. Gobain towers;Position (cm);MPV ADC");

  gr->Draw("AP");
  gr->GetYaxis()->SetRangeUser(100.0,150.0);

  c1->Print("emcal4atten.pdf","pdf portrait");

}
