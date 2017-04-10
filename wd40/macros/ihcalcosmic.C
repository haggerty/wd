TCanvas *cosmic( TTree *W, Int_t event = 2 )
{

  gSystem->Load("onepulse_C.so");

  //  TFile *f = new TFile(filename);
  //  TTree *W = (TTree*)f->Get("W");
  
  TCanvas *c1 = new TCanvas("c1","SiPM",640,640);
  c1->Divide(4,4);
  
  Int_t tilechannel[] = {65, 67, 69, 71,
			 73, 75, 77, 79,
			 81, 83, 85, 87,
			 89, 91, 93, 95};

  Int_t minadc = 1948;
  Int_t maxadc = 2448;

  Int_t i = 0;
  Int_t ican = 1;
  for ( Int_t j = 0; j < sizeof(tilechannel)/sizeof(tilechannel[0]); j++ ) {
    c1->cd(ican++);
    onepulse(W,tilechannel[j],event,minadc,maxadc);
  };
  
  return c1;
}

Int_t ihcalcosmic(TTree *W, Int_t nevents = 20)
{
  gSystem->Load("onepulse_C.so");

  TString filename;
  TCanvas *c1;
  for (Int_t event = 2; event < nevents + 2; event++) {
    c1 = cosmic(W,event);
    filename  = "ihcalcosmicmovie";
    filename += event;
    filename += ".gif";
    c1->Print(filename);
  }
}
