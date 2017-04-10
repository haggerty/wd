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

  TString anyx = "X0>20||X1>20||X2>20||X3>20||X4>20||X5>20||X6>20||X7>20";
  W->SetAlias("anyx",anyx);
  TString anyy = "Y0>20||Y1>20||Y2>20||Y3>20||Y4>20||Y5>20||Y6>20||Y7>20";
  W->SetAlias("anyy",anyy);

  // PbGl cut on Cerenkov outer

  //  W->Draw("TMath::Abs(peak[171]-pedestal[171])>>h(200,0,0)","TMath::Abs(peak[170]-pedestal[170])>100");

  //    W->Draw("TMath::Abs(peak[171]-pedestal[171])>>h(200,0,0)","TMath::Abs(peak[170]-pedestal[170])>30&&anyx&&anyy");
    W->Draw("TMath::Abs(peak[171]-pedestal[171])>>h(200,0,0)","TMath::Abs(peak[166]-pedestal[166])>30&&anyx&&anyy");

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

  TString pdffilename = "pbgl_";
  pdffilename += shortfilename;
  pdffilename.ReplaceAll(".root",".pdf");
  c1->Print(pdffilename,"pdf Portrait");


  return 0;
}
