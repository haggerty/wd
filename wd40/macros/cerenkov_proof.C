Int_t cerenkov_proof( TString filename )
{
  TFile *f = new TFile(filename);

  TCanvas *c1 = new TCanvas("c1","Cerenkov counters",640,480);
  c1->Divide(2,1);

  gStyle->SetOptFit(111);  
  gStyle->SetOptStat(1110);  

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

  c1->cd(1);
  gPad->SetLogy();
  W->SetLineColor(kBlue);
  W->Draw("PBGL>>pbgl2(200,0,2000)","PBGL>2");
  W->SetLineColor(kRed);
  W->Fit("gaus","PBGL","PBGL>2 && C2Icopy>60","sames");

  TSystem *sys = new TSystem();
  TString title = sys->BaseName( filename );
  title.ReplaceAll(".root","");
  title += " PbGl C2Icopy>60";
  pbgl2->SetTitle(title);
  pbgl2->GetXaxis()->SetTitle("Peak ADC counts");

  c1->cd(2);
  gPad->SetLogy();
  W->SetLineColor(kBlue);
  W->Draw("PBGL>>pbgl1(200,0,2000)","PBGL>2");
  W->SetLineColor(kRed);
  W->Fit("gaus","PBGL","PBGL>2 && C1>4","sames");

  title = sys->BaseName( filename );
  title.ReplaceAll(".root","");
  title += " PbGl C1>4";
  pbgl1->SetTitle(title);
  pbgl1->GetXaxis()->SetTitle("Peak ADC counts");

  TString pdffile = "pbgl_cerenkov_";
  TString pdffile = "pbgl_cerenkov_";
  pdffile += sys->BaseName( filename );
  pdffile.ReplaceAll(".root",".pdf");
  std::cout << pdffile << std:: endl;
  c1->Print(pdffile);

  return 0;

}
