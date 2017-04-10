Int_t addemc( TString filename )
{

  TFile *f = new TFile( filename );

  TString emctotal = "TMath::Abs("; 
  for (Int_t i = 0; i < 64; i++ ) {
    emctotal += "peak[";
    emctotal += i;
    emctotal += "]-pedestal[";
    emctotal += i;
    if ( i != 63 ) {
      emctotal += "]+";
    } else {
      emctotal += "])";
    }
  }

  W->SetAlias("EMCSUM",emctotal);
  cout << emctotal  << endl;

  // Cerenkov counters

  W->SetAlias("C1","TMath::Abs(peak[168]-pedestal[168])");
  W->SetAlias("C2I","TMath::Abs(peak[169]-pedestal[169])");
  W->SetAlias("C2O","TMath::Abs(peak[170]-pedestal[170])");

  W->SetAlias("C2Icopy","TMath::Abs(peak[166]-pedestal[166])");
  W->SetAlias("C2Ocopy","TMath::Abs(peak[167]-pedestal[167])");

  TCanvas *cemc = new TCanvas();
  cemc->Divide(2,1);

  c1->cd(1);
  W->Draw("EMCSUM");

  c1->cd(2);
  W->Draw("EMCSUM","C2Ocopy>200");

  TSystem *sys = new TSystem();
  TString shortfilename = sys->BaseName( filename );
  TString pdffilename = "addemc_";
  pdffilename += shortfilename;
  pdffilename.ReplaceAll(".root",".pdf");
  c1->Print(pdffilename,"pdf Portrait");


}
