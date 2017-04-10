{
  
  gSystem->Load("emc3peak_row_or_column_C.so");

  TString mipfile[8] = { "../root/beam_00004023-0000.root",
			 "../root/beam_00004022-0000.root",
			 "../root/beam_00004021-0000.root",
			 "../root/beam_00004020-0000.root",
			 "../root/beam_00004019-0000.root",
			 "../root/beam_00004018-0000.root",
			 "../root/beam_00004017-0000.root",
			 "../root/beam_00004016-0000.root" }

  TCanvas *c1 = new TCanvas();
  c1->Divide(4,2);

  Int_t ican = 1;
  for ( Int_t i = 0; i < 8; i++ ) {
    c1->cd(ican++);
    emc3peak_row_or_column( mipfile[i],i,-1 );
  }

}
