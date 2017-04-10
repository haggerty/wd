Int_t scintillators_mip( TString filename )
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
  W->Draw("SC3>>hsc3(100,0,500)","SC3>5");

  c1->cd(can++);
  W->Draw("SCMWPC4>>hscmwpc4(100,0,500)","SCMWPC4>5");

  // veto counters

  c1->cd(can++);
  W->Draw("VETOL>>hl(100,0,500)","VETOL>5");

  c1->cd(can++);
  W->Draw("VETOR>>hr(100,0,500)","VETOR>5");

  c1->cd(can++);
  W->Draw("VETOT>>ht(100,0,500)","VETOT>5");

  c1->cd(can++);
  W->Draw("VETOB>>hb(100,0,500)","VETOB>5");

  // HCAL scintillators

  // there is no HCALS1 in Jan-Feb 2017
  //  c1->cd(can++);
  //  W->Draw("HCALS1:HCALS1t","HCALS1>5");

  c1->cd(can++);
  W->Draw("HCALS2>>hcals2(100,0,500)","HCALS2>5");

  c1->cd(can++);
  W->Draw("HCALS3>>hcals3(100,0,500)","HCALS3>5");

  TSystem *sys = new TSystem();
  TString shortfilename = sys->BaseName( filename );
  TString pdffilename = "scintillators_mip_";
  pdffilename += shortfilename;
  pdffilename.ReplaceAll(".root",".pdf");
  c1->Print(pdffilename,"pdf Portrait");

  return 0;
}
