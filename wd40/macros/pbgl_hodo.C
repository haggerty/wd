Int_t pbgl( TString filename )
{

  gROOT->SetStyle("Modern");
  gStyle->SetOptFit(111);

  TFile *f = new TFile( filename );

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

  W->SetAlias("PBGL","TMath::Abs(peak[171]-pedestal[171])");
  W->SetAlias("PBGLt","time[171]");

  TString anyx = "X0>20||X1>20||X2>20||X3>20||X4>20||X5>20||X6>20||X7>20";
  W->SetAlias("anyx",anyx);
  TString anyy = "Y0>20||Y1>20||Y2>20||Y3>20||Y4>20||Y5>20||Y6>20||Y7>20";
  W->SetAlias("anyy",anyy);

  TString middlex = "(X0<25&&X1<25&&X2<25&&X5<25&&X6<25&&X7<25)&&(X3>25||X4>25)";
  W->SetAlias("middlex",middlex);

  TString onefingerx0 = "X0>25&&X1<25&&X2<25&&X3<25&&X4<25&&X5<25&&X6<25&&X7<25";
  W->SetAlias("onefingerx0",onefingerx0);
  TString onefingerx1 = "X0<25&&X1>25&&X2<25&&X3<25&&X4<25&&X5<25&&X6<25&&X7<25";
  W->SetAlias("onefingerx1",onefingerx1);
  TString onefingerx2 = "X0<25&&X1<25&&X2>25&&X3<25&&X4<25&&X5<25&&X6<25&&X7<25";
  W->SetAlias("onefingerx2",onefingerx2);
  TString onefingerx3 = "X0<25&&X1<25&&X2<25&&X3>25&&X4<25&&X5<25&&X6<25&&X7<25";
  W->SetAlias("onefingerx3",onefingerx3);
  TString onefingerx4 = "X0<25&&X1<25&&X2<25&&X3<25&&X4>25&&X5<25&&X6<25&&X7<25";
  W->SetAlias("onefingerx4",onefingerx4);
  TString onefingerx5 = "X0<25&&X1<25&&X2<25&&X3<25&&X4<25&&X5>25&&X6<25&&X7<25";
  W->SetAlias("onefingerx5",onefingerx5);
  TString onefingerx6 = "X0<25&&X1<25&&X2<25&&X3<25&&X4<25&&X5<25&&X6>25&&X7<25";
  W->SetAlias("onefingerx6",onefingerx6);
  TString onefingerx7 = "X0<25&&X1<25&&X2<25&&X3<25&&X4<25&&X5<25&&X6<25&&X7>25";
  W->SetAlias("onefingerx7",onefingerx7);


  TString middley = "(Y0<25&&Y1<25&&Y2<25&&Y5<25&&Y6<25&&Y7<25)&&(Y3>25||Y4>25)";
  W->SetAlias("middley",middley);

  TString onefingery0 = "Y0>25&&Y1<25&&Y2<25&&Y3<25&&Y4<25&&Y5<25&&Y6<25&&Y7<25";
  W->SetAlias("onefingery0",onefingery0);
  TString onefingery1 = "Y0<25&&Y1>25&&Y2<25&&Y3<25&&Y4<25&&Y5<25&&Y6<25&&Y7<25";
  W->SetAlias("onefingery1",onefingery1);
  TString onefingery2 = "Y0<25&&Y1<25&&Y2>25&&Y3<25&&Y4<25&&Y5<25&&Y6<25&&Y7<25";
  W->SetAlias("onefingery2",onefingery2);
  TString onefingery3 = "Y0<25&&Y1<25&&Y2<25&&Y3>25&&Y4<25&&Y5<25&&Y6<25&&Y7<25";
  W->SetAlias("onefingery3",onefingery3);
  TString onefingery4 = "Y0<25&&Y1<25&&Y2<25&&Y3<25&&Y4>25&&Y5<25&&Y6<25&&Y7<25";
  W->SetAlias("onefingery4",onefingery4);
  TString onefingery5 = "Y0<25&&Y1<25&&Y2<25&&Y3<25&&Y4<25&&Y5>25&&Y6<25&&Y7<25";
  W->SetAlias("onefingery5",onefingery5);
  TString onefingery6 = "Y0<25&&Y1<25&&Y2<25&&Y3<25&&Y4<25&&Y5<25&&Y6>25&&Y7<25";
  W->SetAlias("onefingery6",onefingery6);
  TString onefingery7 = "Y0<25&&Y1<25&&Y2<25&&Y3<25&&Y4<25&&Y5<25&&Y6<25&&Y7>25";
  W->SetAlias("onefingery7",onefingery7);

  // PbGl cut on Cerenkov outer

  //  W->Draw("TMath::Abs(peak[171]-pedestal[171])>>h(200,0,0)","TMath::Abs(peak[170]-pedestal[170])>30&&anyx&&anyy");

  W->Draw("PBGL>>h(200,0,0)","TMath::Abs(peak[170]-pedestal[170])>30&&middlex&&middley");

  //  W->Draw("TMath::Abs(peak[144]-pedestal[144])>>h(200,0,0)","TMath::Abs(peak[166]-pedestal[166])>60&&anyx&&anyy");

  // W->Draw("TMath::Abs(peak[171]-pedestal[171])>>h(200,0,0)");

  h->GetXaxis()->SetRange(20,200);
  Int_t binmax = h->GetMaximumBin();
  Double_t xmin = 0.92*h->GetXaxis()->GetBinCenter(binmax);
  
  h->Fit("gaus","","",xmin,2000);

  TF1 *gfit = (TF1*) h->GetFunction("gaus");
  Double_t mean  = gfit->GetParameter(1);
  Double_t sigma = gfit->GetParameter(2);

  TSystem *sys = new TSystem();
  TString shortfilename = sys->BaseName( filename );

  std::cout << shortfilename << " "
	    << mean << " +/- " << gfit->GetParError(1) << " "
	    << sigma << " +/- " << gfit->GetParError(2)
	    << " sigma/mean " << sigma/mean
	    << std::endl; 
  
  Double_t res = sigma/mean;
  Double_t f_mean =  gfit->GetParError(1)/gfit->GetParameter(1);
  Double_t f_sigma = gfit->GetParError(2)/gfit->GetParameter(2);
  Double_t f_res = TMath::Sqrt(TMath::Power(f_mean,2)+TMath::Power(f_sigma,2));
  Double_t d_res = f_res*res;
  
  ofstream sumfile;
  sumfile.open("pbgl.txt", std::ofstream::app);
  std::cout << shortfilename << "," << res << "," << d_res << std::endl; 
  std::sumfile << shortfilename << "," 
	       << mean << " " <<  gfit->GetParError(1) << " "
	       << res << " " << d_res << std::endl; 
  sumfile.close();

  TString pdffilename = "pbgl_hodo_";
  pdffilename += shortfilename;
  pdffilename.ReplaceAll(".root",".pdf");
  c1->Print(pdffilename,"pdf Portrait");


  return 0;
}
