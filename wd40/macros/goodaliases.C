Int_t goodaliases( TString filename )
{

  TFile *f = new TFile(filename);

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

  W->SetAlias("electron","C2Icopy>60 || C1>4");

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
  cout << "EMCSUM = " << emctotal  << endl;

  // IHCAL

  TString ihctotal = "TMath::Abs("; 
  for (Int_t i = 65; i < 96; i+=2 ) {
    ihctotal += "peak[";
    ihctotal += i;
    ihctotal += "]-pedestal[";
    ihctotal += i;
    if ( i != 95 ) {
      ihctotal += "]+";
    } else {
      ihctotal += "])";
    }
  }

  W->SetAlias("IHCSUM",ihctotal);
  cout << "IHCSUM = " << ihctotal  << endl;

  // OHCAL

  TString ohctotal = "TMath::Abs("; 
  for (Int_t i = 113; i < 144; i+=2 ) {
    ohctotal += "peak[";
    ohctotal += i;
    ohctotal += "]-pedestal[";
    ohctotal += i;
    if ( i != 143 ) {
      ohctotal += "]+";
    } else {
      ohctotal += "])";
    }
  }

  W->SetAlias("OHCSUM",ohctotal);
  cout << "OHCSUM = " << ohctotal  << endl;

  return 0;

}
