Int_t checkrun( TString filename )
{
  TSystem *sys = new TSystem();

  TString shortfilename = sys->BaseName( filename );
  TString pngfilename;

  TString macro[] = {"cerenkov.C", "scintillators.C"};
  TString plotfileprefix[] = {"cerenkov_", "scintillators_"};

  for (Int_t i = 0; i < 2; i++ ) {
    std::cout << macro[i] << std::endl;
    gROOT->LoadMacro(macro[i]);
    cerenkov(filename);

    pngfilename = plotfileprefix[i];
    pngfilename += shortfilename;
    pngfilename.ReplaceAll(".root",".png");
    c1->Print(pngfilename);
  }    

  return 0;
}
