Int_t cerenkov( TString filename )
{

  TFile *f = new TFile(filename);

  TCanvas *c1 = new TCanvas("c1","Cerenkov counters",900,700);
  c1->Divide(3,4);

  W->SetAlias("PBGL","TMath::Abs(peak[144]-pedestal[144])");
  W->SetAlias("PBGLt","time[144]");

  W->SetAlias("C1","TMath::Abs(peak[168]-pedestal[168])");
  W->SetAlias("C2I","TMath::Abs(peak[169]-pedestal[169])");
  W->SetAlias("C2O","TMath::Abs(peak[170]-pedestal[170])");

  W->SetAlias("C1t","time[168]");
  W->SetAlias("C2It","time[169]");
  W->SetAlias("C2Ot","time[170]");

  W->SetAlias("C2Icopy","TMath::Abs(peak[166]-pedestal[166])");
  W->SetAlias("C2Ocopy","TMath::Abs(peak[167]-pedestal[167])");

  W->SetAlias("C2Itcopy","time[166]");
  W->SetAlias("C2Otcopy","time[167]");

  c1->cd(1);
  W->Draw("C1:C1t");
  c1->cd(2);
  W->Draw("C2I:C2It");
  c1->cd(3);
  W->Draw("C2O:C2Ot");
  
  c1->cd(4);
  gPad->SetLogy();
  W->Draw("C1","C1>2");
  c1->cd(5);
  gPad->SetLogy();
  W->Draw("C2I","C2I>2");
  c1->cd(6);
  gPad->SetLogy();
  W->Draw("C2O","C2O>2");

  c1->cd(8);
  W->Draw("C2Icopy:C2Itcopy");
  c1->cd(9);
  W->Draw("C2Ocopy:C2Otcopy");

  c1->cd(11);
  gPad->SetLogy();
  W->Draw("C2Icopy","C2Icopy>2");
  c1->cd(12);
  gPad->SetLogy();
  W->Draw("C2Ocopy","C2Ocopy>2");

  TSystem *sys = new TSystem();
  TString pdffile = "cerenkov_";
  pdffile += sys->BaseName( filename );
  pdffile.ReplaceAll(".root",".pdf");
  std::cout << pdffile << std:: endl;
  c1->Print(pdffile);
  pdffile.ReplaceAll(".pdf",".png");
  c1->Print(pdffile);

  return 0;

}
