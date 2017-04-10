Int_t scanemc( TTree *W, Int_t first_event, 
	       Double_t minadc = 2048.0-500.0, Double_t maxadc = 2048.0+100.0 )
{

  //  gSystem->Load("sumpulse.C");
  gSystem->Load("emcdisp_C.so");

  if ( first_event < 2 ) {
    cout << "First event must be > 2" << endl;
    return;
  }

  //  TFile *f = new TFile( filename );
  //  TTree *W = (TTree*)f->Get("W");
  //  W->AddFriend("Wf","rc-00436-0f.root");
  //  W->Print("all");

  TCanvas *cp = new TCanvas("cp", "EMCAL",0,0,640,640);  
  cp->Divide(8,8);

  for (Int_t i = first_event; ; i++) {
    
    //    emcdisp(W,cp,i,0.0,2100.0);
    emcdisp(W,cp,i,minadc,maxadc);
    cp->Update();
    cout << "Press enter for next event (q to exit)" << endl;
    //    if ( getchar() == (Int_t)'q' ) {
    if ( std::cin.get() == (Int_t)'q' ) {
      break; 
    }
    cout << "Event " << i << endl;
    
  }

}
