Int_t scintillators( TString filename )
{

  gROOT->LoadMacro("look.C");

  TString look = "look(\"";
  look += filename;
  look += "\")";
  std::cout << look << std::endl;
  gROOT->ProcessLine(look);

  TCanvas *c1 = new TCanvas();
  c1->Divide(3,3);

  Int_t can = 1;

  // facility paddles

  c1->cd(can++);
  W->Draw("SC3:SC3t","SC3>5");

  c1->cd(can++);
  W->Draw("SCMWPC4:SCMWPC4t","SCMWPC4>5");

  // veto counters

  c1->cd(can++);
  W->Draw("VETOL:VETOLt","VETOL>5");

  c1->cd(can++);
  W->Draw("VETOR:VETORt","VETOR>5");

  c1->cd(can++);
  W->Draw("VETOT:VETOTt","VETOT>5");

  c1->cd(can++);
  W->Draw("VETOB:VETOBt","VETOB>5");

  // HCAL scintillators

  // there is no HCALS1 in Jan-Feb 2017
  //  c1->cd(can++);
  //  W->Draw("HCALS1:HCALS1t","HCALS1>5");

  c1->cd(can++);
  W->Draw("HCALS2:HCALS2t","HCALS2>5");

  c1->cd(can++);
  W->Draw("HCALS3:HCALS3t","HCALS3>5");

  TSystem *sys = new TSystem();
  TString shortfilename = sys->BaseName( filename );
  TString pdffilename = "scintillators_";
  pdffilename += shortfilename;
  pdffilename.ReplaceAll(".root",".pdf");
  c1->Print(pdffilename,"pdf Portrait");

  return 0;
}
