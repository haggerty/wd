#include <cstdlib>
#include <cmath>
#include <iostream>
#include <fstream>

// Event library

#include "Event.h"
#include "Eventiterator.h"
#include "fileEventiterator.h"
#include "packet_hbd_fpgashort.h"

// ROOT

#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"

// Interpolator class and root of derivative

#include "Math/Interpolator.h"
#include "Math/Functor.h"
#include "Math/RootFinderAlgorithms.h" 

// data structures for this 

#include "wd40data.h"

using namespace std;

int wd40( const char *filename, const char *outputfile )
{

  // John Haggerty, BNL 2013.02.27

  int status;
  cerr << "file: " << filename << endl;
  
  Eventiterator *it =  new fileEventiterator (filename, status);
  if (status) {
    cerr << "Couldn't open input file " << filename << endl;
    delete it;
    exit(1);
  }
  
  Event *evt;
  
  // Get the root tree ready
  
  // first make a nice filename by making use of sthe string class

  TString rootfilename = outputfile;
  rootfilename.ReplaceAll(".PRDFF",".root");
  rootfilename.ReplaceAll(".prdf",".root");
  rootfilename = gSystem->BaseName( rootfilename.Data() ) ;

  cout << "creating root file: " << rootfilename << endl;

  // now make the root file

  TFile f(rootfilename,"recreate");
  TTree W("W","Waveform digitizer raw data");
  W.SetMaxTreeSize(50000000000LL);

  wd40data *wd;
  wd = new wd40data();

  W.Branch("runnumber",&wd->runnumber,"runnumber/I"); 
  W.Branch("eventnumber",&wd->eventnumber,"eventnumber/I"); 

  TString branchstring = "nrmodules[";
  branchstring += NXMIT;
  branchstring += "]/I";
  W.Branch("nrmodules",&wd->nrmodules,branchstring); 

  branchstring = "nrsamples[";
  branchstring += NXMIT;
  branchstring += "]/I";
  W.Branch("nrsamples",&wd->nrsamples,branchstring); 

  branchstring = "parity[";
  branchstring += NXMIT;
  branchstring += "]/I";
  W.Branch("parity",&wd->parity,branchstring); 

  branchstring = "trigger[";
  branchstring += NRMOD;
  branchstring += "]/I";
  W.Branch("trigger",wd->trigger,branchstring); 
  
  branchstring = "bclk[";
  branchstring += NRMOD;
  branchstring += "]/I";
  W.Branch("bclk",wd->bclk,branchstring); 

  branchstring = "moduleid[";
  branchstring += NRMOD;
  branchstring += "]/I";
  W.Branch("moduleid",wd->moduleid,branchstring); 
  // adc
  branchstring = "adc[";
  branchstring += NRMOD*NCHANPERMOD;
  branchstring += "][";
  branchstring += NRSAM;
  branchstring += "]/I";
  W.Branch("adc",wd->adc,branchstring); 
  // mean
  branchstring = "mean[";
  branchstring += NRMOD*NCHANPERMOD;
  branchstring += "]/F";
  W.Branch("mean",wd->mean,branchstring); 
  // rms
  branchstring = "rms[";
  branchstring += NRMOD*NCHANPERMOD;
  branchstring += "]/F";
  W.Branch("rms",wd->rms,branchstring); 
  // minadc
  branchstring = "minadc[";
  branchstring += NRMOD*NCHANPERMOD;
  branchstring += "]/I";
  W.Branch("minadc",wd->minadc,branchstring); 
  // maxadc
  branchstring = "maxadc[";
  branchstring += NRMOD*NCHANPERMOD;
  branchstring += "]/I";
  W.Branch("maxadc",wd->maxadc,branchstring); 
  // pedestal
  branchstring = "pedestal[";
  branchstring += NRMOD*NCHANPERMOD;
  branchstring += "]/F";
  W.Branch("pedestal",wd->pedestal,branchstring); 
  // pedsig
  branchstring = "pedsig[";
  branchstring += NRMOD*NCHANPERMOD;
  branchstring += "]/F";
  W.Branch("pedsig",wd->pedsig,branchstring); 
  // peak
  branchstring = "peak[";
  branchstring += NRMOD*NCHANPERMOD;
  branchstring += "]/F";
  W.Branch("peak",wd->peak,branchstring); 
  // time
  branchstring = "time[";
  branchstring += NRMOD*NCHANPERMOD;
  branchstring += "]/F";
  W.Branch("time",wd->time,branchstring); 
  // integral (+/- 1)
  branchstring = "integral[";
  branchstring += NRMOD*NCHANPERMOD;
  branchstring += "]/F";
  W.Branch("integral",wd->integral,branchstring);
  // integral2 (+/- 2)
  branchstring = "integral2[";
  branchstring += NRMOD*NCHANPERMOD;
  branchstring += "]/F";
  W.Branch("integral2",wd->integral2,branchstring);
  // summary
  branchstring = "summary[";
  branchstring += NXMIT;
  branchstring += "]/I";
  W.Branch("summary",wd->summary,branchstring); 

  // Arrays for Interpolator to work with
  Double_t t[NRSAM], a[NRSAM];
  for ( Int_t i = 0; i < NRSAM; i++ ) {
    t[i] = (Double_t) i;
  }
  
  int n_recs = 0;
  while ( (evt = it->getNextEvent()) ) {
    if ( evt->getEvtType() != 1 ) continue;
    
    int ibd = 0;
    int ich = 0;
    for ( Int_t ixmit = 0; ixmit < NXMIT; ixmit++ ) {
      Packet *p;
      p = evt->getPacket( PACKET[ixmit] );
      if ( p ) {
      
	Packet_hbd_fpgashort *hbd_p = dynamic_cast<Packet_hbd_fpgashort*>( p );
	hbd_p->setNumSamples( NRSAM );

	wd->runnumber = evt->getRunNumber();
	wd->eventnumber = evt->getEvtSequence();
	
	wd->nrmodules[ixmit] = p->iValue(0,"NRMODULES");
	// It's not reported correctly in the packet anyway... just do what's right
	//	wd->nrsamples[ixmit] = p->iValue(0,"NRSAMPLES");
	wd->nrsamples[ixmit] = NRSAM;
	wd->parity[ixmit] = p->iValue(0,"PARITY");
	
	int i,j;
	for (i = 0; i < NMODBYXMIT[ixmit]; i++) {
	  wd->trigger[ibd] = p->iValue(i,"TRIGGER");
	  wd->bclk[ibd] = p->iValue(i,"BCLK");
	  wd->moduleid[ibd] = p->iValue(i,"MODULEID");
	  ibd++;
	}
	
	for (i = 0; i < NMODBYXMIT[ixmit]*NCHANPERMOD ; i++) {
	  Int_t adcmped;
	  Int_t adcmpedmax = 0;
	  Int_t jmaxsamp = 0;
	  // mean and sigma of all NRSAM samples
	  wd->mean[ich] = 0.0;
	  wd->rms[ich] = 0.0;
	  // minimum and maximum of all NRSAM samples
	  wd->minadc[ich] = 4096;
	  wd->maxadc[ich] = -1;
	  Float_t delta = 0.0;
	  Float_t m2 = 0.0;
	  // mean and sigma of first NPEDSAM
	  wd->pedestal[ich] = 0.0;
	  wd->pedsig[ich] = 0.0;
	  Float_t deltaped = 0.0;
	  Float_t m2ped = 0.0;
	  for ( j = 0; j < NRSAM; j++) {
	    wd->adc[ich][j] = p->iValue(i,j);
	    if ( wd->adc[ich][j] < wd->minadc[ich] ) {
	      wd->minadc[ich] = wd->adc[ich][j];
	    }
	    if ( wd->adc[ich][j] > wd->maxadc[ich] ) {
	      wd->maxadc[ich] = wd->adc[ich][j];
	    }
	    // array to pass to Interpolator
	    a[j] = (Float_t) wd->adc[ich][j];
	    // calculate mean and rms of all samples ala Knuth
	    delta = a[j] - wd->mean[ich];
	    wd->mean[ich] += delta/( (Float_t) (j+1) );
	    m2 += delta*( a[j] - wd->mean[ich] );
	    if ( j < NPEDSAM ) {
	      deltaped = a[j] - wd->pedestal[ich];
	      wd->pedestal[ich] += deltaped/( (Float_t) (j+1) );
	      m2ped += deltaped*( a[j] - wd->pedestal[ich] );
	    }
	    // save sample furthest from 2048
	    adcmped = TMath::Abs( wd->adc[ich][j] - 2048 );
	    if ( adcmped > adcmpedmax ) {
	      jmaxsamp = j;
	      adcmpedmax = adcmped;
	    }	
	  }
	  wd->rms[ich] = m2/( (Float_t) (NRSAM-1) );
	  wd->pedsig[ich] = m2ped/( (Float_t) (NPEDSAM-1) );
	  
	  // begin interpolation around maximum adc
	  ROOT::Math::Interpolator inter( NRSAM, ROOT::Math::Interpolation::kCSPLINE );
	  inter.SetData( NRSAM, t, a );
	  
	  ROOT::Math::Functor1D f1d(&inter,&ROOT::Math::Interpolator::Deriv);
	  
	  // interpolate +/- sampwindow around maximum adc value
	  const Int_t sampwindow = 2;
	  Int_t jminint,jmaxint;
	  jminint = TMath::Max(0,jmaxsamp-sampwindow);
	  jmaxint = TMath::Min(NRSAM-1,jmaxsamp+sampwindow);
	  
	  // the derivative has to cross 0 or you can't find a root
	  if ( TMath::Sign(1.0,inter.Deriv(jminint)) == -TMath::Sign(1.0,inter.Deriv(jmaxint)) ) {
	    Double_t tmax;
	    ROOT::Math::Roots::Brent brf;
	    brf.SetFunction( f1d, (Float_t) jminint, (Float_t) jmaxint);
	    Bool_t statr = brf.Solve();
	    if ( !statr ) {
	      cout << "eventnumber,i: "
		   << wd->eventnumber << ","
		   << ich << endl;
	      cout << "statr: " << statr << endl;
	      cout << "adcmpedmax: " 
		   << adcmpedmax << endl;
	      cout << "jminint,jmaxsamp,jmaxint: " 
		   << jminint << ","
		   << jmaxsamp << ","
		   << jmaxint << endl;
	      cout << "tmax: " << tmax << endl;
	    }
	    tmax = brf.Root();
	    wd->time[ich] = (Float_t) tmax;
	    Double_t lower = TMath::Max(0.0,tmax - 1.0);
	    Double_t upper = TMath::Min( (Double_t) (NRSAM - 1),tmax + 1.0);
	    wd->integral[ich] = (Float_t) (inter.Integ(lower,upper)/2.0);
	    lower = TMath::Max(0.0,tmax - 2.0);
	    upper = TMath::Min( (Double_t) (NRSAM - 1),tmax + 2.0);
	    wd->integral2[ich] = (Float_t) (inter.Integ(lower,upper)/4.0);
	    wd->peak[ich] = (Float_t) (inter.Eval( tmax ));
	    // end interpolation around maximum adc
	  } else {
	    //	  cout << "No maximum found for eventnumber,ich: "
	    //	       << wd->eventnumber << ","
	    //	       << ich << endl;
	    wd->time[ich] = 0.0;
	    wd->peak[ich] = wd->pedestal[ich];
	    wd->integral[ich] = 0.0;
	    wd->integral2[ich] = 0.0;
	  }
	  ich++;
	}
	
	wd->summary[ixmit] = p->iValue(0,"SUMMARY");
	
	delete p;
      }
    }
    W.Fill();
    n_recs++;
    delete evt;
  }
  
  W.BuildIndex("runnumber", "eventnumber");
  W.Write();
  f.Close();

  cout << "wd40: final n_recs: " << n_recs << endl;

  delete wd;
  delete it;
  return 0;
  
}
