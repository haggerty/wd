Int_t add21( TString filename )
{

  gStyle->SetOptFit(111);

  TFile *f = new TFile( filename );

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
  
  // 3x3 sum around Tower 21
  // as of 2017.02.06

  Int_t tower21_3x3[9] = { 10, 27, 26,
			   8, 25, 24,
			   14, 31, 30 };

  TString emc3x3 = "TMath::Abs("; 
  for (Int_t i = 0; i < 9; i++ ) {
    emc3x3 += "peak[";
    emc3x3 += tower21_3x3[i];
    emc3x3 += "]-pedestal[";
    emc3x3 += tower21_3x3[i];
    if ( i < 8 ) {
      emc3x3 += "]+";
    } else {
      emc3x3 += "])";
    }
  }

  W->SetAlias("EMC3X3",emc3x3);
  cout << "EMC3X3: " << emc3x3  << endl;

  // 5x5 sum around Tower 21

  Int_t tower21_5x5[25] = { 5, 4, 21, 20, 37,
			    11, 10, 27, 26, 43,
			    9, 8, 25, 24, 41,
			    15, 14, 31, 30, 47,
			    13, 12, 29, 28, 45 };

  TString emc5x5 = "TMath::Abs("; 
  for (Int_t i = 0; i < 25; i++ ) {
    emc5x5 += "peak[";
    emc5x5 += tower21_5x5[i];
    emc5x5 += "]-pedestal[";
    emc5x5 += tower21_5x5[i];
    if ( i < 24 ) {
      emc5x5 += "]+";
    } else {
      emc5x5 += "])";
    }
  }

  W->SetAlias("EMC5X5",emc5x5);
  cout << "EMC5X5: " << emc5x5  << endl;

  // Cerenkov counters

  W->SetAlias("C1","TMath::Abs(peak[168]-pedestal[168])");
  W->SetAlias("C2I","TMath::Abs(peak[169]-pedestal[169])");
  W->SetAlias("C2O","TMath::Abs(peak[170]-pedestal[170])");

  W->SetAlias("C2Icopy","TMath::Abs(peak[166]-pedestal[166])");
  W->SetAlias("C2Ocopy","TMath::Abs(peak[167]-pedestal[167])");

  TCanvas *cemc = new TCanvas();
  cemc->Divide(3,1);

  c1->cd(1);
  W->Draw("EMCSUM");

  c1->cd(2);
  W->Draw("EMCSUM","C2Ocopy>50");
  W->SetLineColor(kRed);
  W->Draw("EMC3X3>>h(200,0,0)","C2Ocopy>50","same");
  W->SetLineColor(kGreen);
  W->Draw("EMC5X5","C2Ocopy>50","same");

  // fit the 3x3

  c1->cd(3);

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
  sumfile.open("add21.txt", std::ofstream::app);
  std::cout << shortfilename << "," << res << "," << d_res << std::endl; 
  std::sumfile << shortfilename << "," 
	       << mean << " " <<  gfit->GetParError(1) << " "
	       << res << " " << d_res << std::endl; 
  sumfile.close();

  // end of fit stuff

  //  TSystem *sys = new TSystem();
  //  TString shortfilename = sys->BaseName( filename );
  TString pdffilename = "add21_";
  pdffilename += shortfilename;
  pdffilename.ReplaceAll(".root",".pdf");
  cemc->Print(pdffilename,"pdf Portrait");


}



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

  W->Draw("TMath::Abs(peak[171]-pedestal[171])>>h(200,0,0)","TMath::Abs(peak[170]-pedestal[170])>30&&anyx&&anyy");

  //  W->Draw("TMath::Abs(peak[144]-pedestal[144])>>h(200,0,0)","TMath::Abs(peak[166]-pedestal[166])>60&&anyx&&anyy");

  // W->Draw("TMath::Abs(peak[171]-pedestal[171])>>h(200,0,0)");

  TString pdffilename = "pbgl_";
  pdffilename += shortfilename;
  pdffilename.ReplaceAll(".root",".pdf");
  c1->Print(pdffilename,"pdf Portrait");


  return 0;
}
