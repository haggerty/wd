void emcfit_graph( TString filename = "emcfit.txt")
{
  gStyle->SetOptFit(111);

  TGraphErrors *gr = new TGraphErrors(filename,"%*s %lg %lg %lg");
  gr->SetTitle("EMCAL Temperature Compensation ON");
  gr->GetXaxis()->SetTitle("Average temperature from thermistors (#circC)");
  gr->GetYaxis()->SetTitle("EMCSUM Peak");
  gr->Draw("AP");
  gr->Fit("pol1");

}
