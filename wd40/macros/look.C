TTree *W;


Int_t look( TString filename )
{
  gSystem->Load("onepulse_C.so");
  gStyle->SetTitleOffset(1.5,"y");
 
  TFile *f = new TFile(filename);
  f->GetObject("W",W);

  // PbGl, when it's there

  W->SetAlias("PBGL","TMath::Abs(peak[171]-pedestal[171])");
  W->SetAlias("PBGLt","time[171]");

  // the "warbler" (tags physics triggers)

  W->SetAlias("SPILLmax","maxadc[160]");
  W->SetAlias("SPILLmin","minadc[160]");

  // Cerenkov counters

  W->SetAlias("C1","TMath::Abs(peak[168]-pedestal[168])");
  W->SetAlias("C2I","TMath::Abs(peak[169]-pedestal[169])");
  W->SetAlias("C2O","TMath::Abs(peak[170]-pedestal[170])");

  W->SetAlias("C2Icopy","TMath::Abs(peak[166]-pedestal[166])");
  W->SetAlias("C2Ocopy","TMath::Abs(peak[167]-pedestal[167])");

  // scintillators

  W->SetAlias("SC3","TMath::Abs(peak[161]-pedestal[161])");
  W->SetAlias("SC3t","time[161]");

  W->SetAlias("SCMWPC4","TMath::Abs(peak[162]-pedestal[162])");
  W->SetAlias("SCMWPC4t","time[162]");

  W->SetAlias("HCALS1","TMath::Abs(peak[163]-pedestal[163])");
  W->SetAlias("HCALS1t","time[163]");

  W->SetAlias("HCALS2","TMath::Abs(peak[164]-pedestal[164])");
  W->SetAlias("HCALS2t","time[164]");

  W->SetAlias("HCALS3","TMath::Abs(peak[165]-pedestal[165])");
  W->SetAlias("HCALS3t","time[165]");

  W->SetAlias("VETOL","TMath::Abs(peak[173]-pedestal[173])");
  W->SetAlias("VETOLt","time[173]");

  W->SetAlias("VETOR","TMath::Abs(peak[175]-pedestal[175])");
  W->SetAlias("VETORt","time[175]");

  W->SetAlias("VETOT","TMath::Abs(peak[174]-pedestal[174])");
  W->SetAlias("VETOTt","time[174]");

  // changed from CH12 to CH3 in SETDC after run 3700
  //  W->SetAlias("VETOB","TMath::Abs(peak[172]-pedestal[172])");
  //  W->SetAlias("VETOBt","time[172]");
  W->SetAlias("VETOB","TMath::Abs(peak[163]-pedestal[163])");
  W->SetAlias("VETOBt","time[163]");

  // hodoscope

  TString adc;
  TString name;
  Int_t x = 96;
  Int_t y = 104;
  for (Int_t i = 0; i < 8; i++ ) { 

    // x (vertical fingers)

    name = "X";
    name += i;

    adc = "TMath::Abs(peak[";
    adc += x;
    adc += "]-pedestal[";
    adc += x;
    adc += "])";

    W->SetAlias(name,adc);
    cout << name << " " << adc << endl;
    x++;

    // y (horizontal fingers)

    name = "Y";
    name += i;

    adc = "TMath::Abs(peak[";
    adc += y;
    adc += "]-pedestal[";
    adc += y;
    adc += "])";

    W->SetAlias(name,adc);
    cout << name << " " << adc << endl;
    y++;

  }


  // Add up the whole damned EMCAL

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


  return 0;
}

Int_t plotafew( Int_t channel = 64, Int_t event = 2, Int_t nevents = 20, Double_t minadc = 1948.0, Double_t maxadc = 2148.0 )
{

  TString drawopt = "AP";
  for (Int_t i = event; i < event+nevents; i++ ) {
    onepulse(W,channel,i,minadc,maxadc,drawopt);
    drawopt = "same";
  }

}
