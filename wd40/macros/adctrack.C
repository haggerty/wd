#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TCanvas.h"
#include "TH2.h"

#include <iostream>

Int_t adctrack( Int_t runnumber )
{
  // exact correspondence between triggers
  //  TString mwpc_filename = "/sphenix/user/haggerty/t1044/mwpc/tracking/mwpc_00003463-0000.root";
  //  TString adc_filename = "/sphenix/data/data01/t1044-2016a/wd40/root/beam_00003463-0000.root";

  // off by 2.. screwy 
  //  TString mwpc_filename = "/sphenix/user/haggerty/t1044/mwpc/tracking/mwpc_00003514-0000.root";
  //  TString adc_filename = "/sphenix/data/data01/t1044-2016a/wd40/root/beam_00003514-0000.root";

  //  TString mwpc_filename = "/sphenix/user/haggerty/t1044/mwpc/tracking/mwpc_00003515-0000.root";
  //  TString adc_filename = "/sphenix/data/data01/t1044-2016a/wd40/root/beam_00003515-0000.root";

  TString mwpc_filename = "/sphenix/user/haggerty/t1044/mwpc/tracking/mwpc_0000";
  mwpc_filename += runnumber;
  mwpc_filename += "-0000.root";

  TString adc_filename = "/sphenix/data/data01/t1044-2016a/wd40/root/beam_0000";
  adc_filename += runnumber;
  adc_filename += "-0000.root";

  TFile *file_adc = new TFile(adc_filename);
  TTree *W;
  file_adc->GetObject("W",W);

  W->SetAlias("PBGL","TMath::Abs(peak[171]-pedestal[171])");
  W->SetAlias("PBGLt","time[171]");

  W->SetAlias("SCMWPC4","TMath::Abs(peak[162]-pedestal[162])");
  W->SetAlias("SCMWPC4t","time[162]");

  W->SetAlias("SPILLmax","maxadc[160]");
  W->SetAlias("SPILLmin","minadc[160]");

  W->SetAlias("SC3","TMath::Abs(peak[161]-pedestal[161])");
  W->SetAlias("SC3t","time[161]");

  W->SetAlias("C1","TMath::Abs(peak[168]-pedestal[168])");
  W->SetAlias("C2I","TMath::Abs(peak[169]-pedestal[169])");
  W->SetAlias("C2O","TMath::Abs(peak[170]-pedestal[170])");

  W->SetAlias("C2Icopy","TMath::Abs(peak[166]-pedestal[166])");
  W->SetAlias("C2Ocopy","TMath::Abs(peak[167]-pedestal[167])");

  // hodoscope

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
    std::cout << name << " " << adc << std::endl;
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
    std::cout << name << " " << adc << std::endl;
    y++;

  }

  TFile *file_mwpc = new TFile(mwpc_filename);
  TTree *track;
  file_mwpc->GetObject("track",track);
  W->AddFriend(track);

  // 
  TString cutstring = "track.num_xtracks==1 && track.num_ytracks==1";
  TString fingerdraw = ""; 
  TString gstring;
  
  TCanvas *ch = new TCanvas();
  ch->Divide(2,1);


  TString h_or_v[2] = {"X","Y"};
  //  TString drawstring[2] = { ":track.xtrack_inter[0]+track.xtrack_slope[0]*9552.0",
  //			    ":track.ytrack_inter[0]+track.ytrack_slope[0]*9552.0" };
  TString drawstring[2] = { ":track.xtrack_inter[0]+track.xtrack_slope[0]*14950.0",
			    ":track.ytrack_inter[0]+track.ytrack_slope[0]*14950.0" };
  
  /*
  TString hname[2] = { "hx", "hy" };
  TH2F *hx =  new TH2F("hx","MWPC vs Hodoscope ADC;Extrapolated x position from MWPC (mm);Hodoscope ADC",
		       200,-100,100,100,0,250);
  TH2F *hy =  new TH2F("hy","MWPC vs Hodoscope ADC;Extrapolated y position from MWPC (mm);Hodoscope ADC",
		       200,-100,100,100,0,250);
  */

  Int_t can = 1;
  for (Int_t j = 0; j < 2; j++ ) {
    ch->cd(can++);
    Int_t fingercolor = 1;
    for ( Int_t fingernumber = 0; fingernumber < 8; fingernumber++ ) {
      fingerdraw = h_or_v[j];
      fingerdraw += fingernumber;
      fingerdraw += drawstring[j];
      fingerdraw += ">>(200,-100,100,100,0,250)";
      //      fingerdraw += ">>";
      //      fingerdraw += hname[j];
      std::cout << fingerdraw << std::endl;
      W->SetMarkerColor(fingercolor++);
      if ( fingernumber == 0 ) {
	gstring = "";
      } else {
	gstring = "same";
      }
      W->Draw(fingerdraw,cutstring,gstring);
    }
  }  
  
  TString pdffilename = "adctrack_";
  pdffilename += runnumber;
  pdffilename += ".pdf";
  ch->Print(pdffilename);
  TString pngfilename = "adctrack_";
  pngfilename += runnumber;
  pngfilename += ".png";
  ch->Print(pngfilename);

  return 0;
}
