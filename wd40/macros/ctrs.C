Int_t ctrs( TString filename )
{

  TFile *f =  new TFile( filename );

  TCanvas *c1 =  new TCanvas("c1","Counters",640,480);
  c1->Divide(4,2);
 
  W->SetAlias("S1","TMath::Abs(peak[163]-pedestal[163])");
  W->SetAlias("S2","TMath::Abs(peak[164]-pedestal[164])");
  W->SetAlias("S3","TMath::Abs(peak[165]-pedestal[165])");

  W->SetAlias("BOTTOM","TMath::Abs(peak[172]-pedestal[172])");
  W->SetAlias("TOP","TMath::Abs(peak[174]-pedestal[174])");
  W->SetAlias("LEFT","TMath::Abs(peak[173]-pedestal[173])");
  W->SetAlias("RIGHT","TMath::Abs(peak[175]-pedestal[175])");

  c1->cd(1);
  W->Draw("S1>>hs1(100,0,400)","S1>10");
  c1->cd(2);
  W->Draw("S2>>hs2(100,0,200)","S2>10");
  c1->cd(3);
  W->Draw("S3>>hs3(100,0,200)","S3>15");
  
  c1->cd(5);
  W->Draw("BOTTOM>>hsb(100,0,200)","BOTTOM>10");
  c1->cd(6);
  W->Draw("TOP>>hst(100,0,200)","TOP>10");
  c1->cd(7);
  W->Draw("LEFT>>hsl(100,0,400)","LEFT>10");
  c1->cd(8);
  W->Draw("RIGHT>>hsr(100,0,400)","RIGHT>10");

  return 0;


}
