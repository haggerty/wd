Long64_t dat2tree( Char_t *filename )
{
  TString rootfilename = filename;
  rootfilename.ReplaceAll(".dat",".root");

  cout << "root file: " << rootfilename << endl;

  TFile *f = new TFile(rootfilename,"RECREATE");
  TTree *T = new TTree("T","LED");

  Long64_t nlines = T->ReadFile(filename);
  T->Write();

  return nlines;
}
