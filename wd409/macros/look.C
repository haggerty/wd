TTree *W;

Int_t look( TString filename )
{
  gSystem->Load("onepulse_C.so");
  gStyle->SetTitleOffset(1.5,"y");
 
  TFile *f = new TFile(filename);
  f->GetObject("W",W);

  TString adc;
  TString name;
  for (Int_t i = 0; i < 64; i++ ) { 
    name = "ADC";
    name += i;
      
    adc = "peak[";
    adc += i;
    adc += "]-pedestal[";
    adc += i;
    adc += "]";
    
    W->SetAlias(name,adc);
    //    cout << name << " " << adc << endl;
  }
  cout << "creating aliases ADC{0-63}" << endl;

  return 0;
}

Int_t plotafew( Int_t channel = 64, Int_t event = 2, Int_t nevents = 20, Double_t minadc = 0.0, Double_t maxadc = 16000.0 )
{

  TString drawopt = "AP";
  for (Int_t i = event; i < event+nevents; i++ ) {
    onepulse(W,channel,i,minadc,maxadc,drawopt);
    drawopt = "same";
  }

}
