Int_t *emc3movie( TString filename, Int_t nevents = 10, Int_t first_event = 2  )
{

  //  gSystem->Load("onepulse_C.so");
  gSystem->Load("emc3disp_C.so");
  gStyle->SetTitleOffset(1.5,"y");
 
  TFile *f = new TFile(filename);
  TTree *W;
  f->GetObject("W",W);
  
  TCanvas *c1 = new TCanvas("c1","EMC3",640,640);
  c1->Divide(8,8);
  
  for (Int_t event = first_event; event < first_event + nevents; event++) {
    emc3disp(W,c1,event,2000,3500);
    filename  = "emc3movie_";
    filename += event;
    filename += ".gif";
    c1->Print(filename);
  }

  return 0;

}
