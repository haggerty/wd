Int_t emcesum( TString filename )
{

  gStyle->SetOptFit(111);

  TCanvas *c1 = new TCanvas("c1","Total energies",640,480);
  c1->Divide(2,2);

  TFile *f = new TFile( filename );

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

  W->SetAlias("electron","C2Icopy>60 || C1>4");

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
  cout << "EMCSUM = " << emctotal  << endl;

  c1->cd(1);
  W->Draw("EMCSUM");

  // IHCAL

  TString ihctotal = "TMath::Abs("; 
  for (Int_t i = 65; i < 96; i+=2 ) {
    ihctotal += "peak[";
    ihctotal += i;
    ihctotal += "]-pedestal[";
    ihctotal += i;
    if ( i != 95 ) {
      ihctotal += "]+";
    } else {
      ihctotal += "])";
    }
  }

  W->SetAlias("IHCSUM",ihctotal);
  cout << "IHCSUM = " << ihctotal  << endl;

  c1->cd(2);
  W->Draw("IHCSUM");

  // OHCAL

  TString ohctotal = "TMath::Abs("; 
  for (Int_t i = 113; i < 144; i+=2 ) {
    ohctotal += "peak[";
    ohctotal += i;
    ohctotal += "]-pedestal[";
    ohctotal += i;
    if ( i != 143 ) {
      ohctotal += "]+";
    } else {
      ohctotal += "])";
    }
  }

  W->SetAlias("OHCSUM",ohctotal);
  cout << "OHCSUM = " << ohctotal  << endl;

  c1->cd(3);
  W->Draw("OHCSUM");

  c1->cd(4);
  W->Draw("0.25*EMCSUM+0.4*IHCSUM+0.2*OHCSUM>>h");


  //  h->GetXaxis()->SetRange(50,200);
  Int_t binmax = h->GetMaximumBin();
  Double_t xmin = 0.8*h->GetXaxis()->GetBinCenter(binmax);
  
  h->Fit("gaus","","",xmin,1-000);

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
  sumfile.open("emcesum.txt", std::ofstream::app);
  std::cout << shortfilename << "," << res << "," << d_res << std::endl; 
  std::sumfile << shortfilename << "," 
               << mean << " " <<  gfit->GetParError(1) << " "
               << res << " " << d_res << std::endl; 
  sumfile.close();

  TSystem *sys = new TSystem();
  TString title = sys->BaseName( filename );
  title.ReplaceAll(".root","");
  TString pdffile = "emcesum_";
  pdffile += sys->BaseName( filename );
  pdffile.ReplaceAll(".root",".pdf");
  std::cout << pdffile << std:: endl;
  c1->Print(pdffile);

  return 0;

}
