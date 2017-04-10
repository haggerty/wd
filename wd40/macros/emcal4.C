TTree *W;


Int_t emcal4( TString filename )
{
  gSystem->Load("onepulse_C.so");
  gStyle->SetTitleOffset(1.5,"y");
  gStyle->SetOptFit(111);
 
  TFile *f = new TFile(filename);
  f->GetObject("W",W);

  TString adc;
  TString name;
  for (Int_t i = 24; i < 32; i++ ) { 

    name = "EMC";
    name += i;

    adc = "TMath::Abs(peak[";
    adc += i;
    adc += "]-pedestal[";
    adc += i;
    adc += "])";

    W->SetAlias(name,adc);
    cout << name << " " << adc << endl;

  }

  TCanvas *c1 = new TCanvas();
  c1->Divide(2,2);

  TString drawstring;
  TString cutstring;
  TString vetocut = "EMC24<200 && EMC26<200 && EMC28<200 && EMC30<200";
  Int_t ican = 1;
  for (Int_t i = 25; i < 32; i+=2 ) {

    drawstring = "EMC";
    drawstring += i;
    drawstring += ">>h";
    drawstring += i;
    drawstring += "(100,0.0,300.0)";

    cutstring = vetocut;
    cutstring += " && EMC";
    cutstring += i;
    cutstring += ">50";

    c1->cd(ican++);
    W->Draw(drawstring,cutstring);
  }

  Int_t ican = 1;
  Int_t ifit = 0;
  Int_t binmax;
  Double_t xmin;
  TH1F *h[4];
  TF1 *fit;
  TString hname;
  Float_t mpv[64];
  Float_t dmpv[64];
  for (Int_t i = 25; i < 32; i+=2 ) {
    c1->cd(ican++);
    hname = "h";
    hname += i;
    f->GetObject(hname,h[ifit]);
    binmax = h25->GetMaximumBin();
    xmin = 0.90*h25->GetXaxis()->GetBinCenter(binmax);
    h[ifit]->Fit("landau","","",xmin,2000);
    fit = (TF1*) h[ifit]->GetFunction("landau");
    mpv[ifit] = fit->GetParameter(1);
    dmpv[ifit] = fit->GetParError(1);
    ifit++;
  }

  TSystem *sys = new TSystem();
  TString shortfilename = sys->BaseName( filename );
  std::cout << shortfilename << ",";
  for (Int_t i = 0; i < 4; i++ ) {
    std::cout << mpv[i] << "," << dmpv[i] << ",";
  }
  std::cout << std::endl;

  TSystem *sys = new TSystem();
  TString shortfilename = sys->BaseName( filename );
  TString pdffilename = "emcal4_";
  pdffilename += shortfilename;
  pdffilename.ReplaceAll(".root",".pdf");
  c1->Print(pdffilename,"pdf Portrait");

  return 0;
}
