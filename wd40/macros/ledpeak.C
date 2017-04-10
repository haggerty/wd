#define ledpeak_cxx
// The class definition in ledpeak.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// Root > T->Process("ledpeak.C")
// Root > T->Process("ledpeak.C","some options")
// Root > T->Process("ledpeak.C+")
//

#include "ledpeak.h"
#include <TH2.h>
#include <TStyle.h>
#include <TF1.h>
#include <iostream>

#include "TGraph.h"
#include "TAxis.h"
#include "TSQLServer.h"
#include "TSQLResult.h"
#include "TSQLRow.h"
#include "TString.h"
#include "TH1.h"

const int NEMCCHAN = 64;

TFile *f;
TTree *L;
TH1F *h[NEMCCHAN];
TF1 *fit;

Int_t event;
Int_t tower[NEMCCHAN];
Float_t mean[NEMCCHAN];
Float_t dmean[NEMCCHAN];
Float_t sigma[NEMCCHAN];
Float_t dsigma[NEMCCHAN];
Float_t temperature[NEMCCHAN];

TGraph *grt[NEMCCHAN];

Double_t eventtime;
Double_t beginruntime;
Int_t rn;

// --- timeseries.C

TGraph *timeseries( TString column, TString table, TString where = "", TString database = "/sphenix/data/data01/t1044-2016a/fnal/testbeam.db" )
{

  TSQLServer *serv = TSQLServer::Connect("sqlite://"+database, "", "");
  if ( !serv ) return 0;

  TSQLResult *res;
  TSQLRow *row;

  TString field;
  TString fieldname;

  TString sql;
  sql = "SELECT COUNT(*) AS nrows";
  TString from = " FROM ";
  from += table;
  sql += from;
  sql += " ";
  sql += where;

  std::cout << sql << std::endl;
  res = serv->Query(sql);
  if ( !res ) return 0;

  // Kluge to get the number of rows in this query

  row = res->Next();
  field = TString( row->GetField(0) );
  fieldname = TString( res->GetFieldName(0) );
  Int_t nrows_k = field.Atoi();
  //  std::cout << fieldname << " nrows_k: " << nrows_k << std::endl;

  // Extract the result of the query into vectors

  sql = "SELECT ";
  TString what = "UTIME,";
  what += column;
  sql += what;
  sql += from;
  sql += " ";
  sql += where;
  std::cout << sql << std::endl;
  res = serv->Query(sql);

  //  Int_t nrows = res->GetRowCount();
  Int_t nfields = res->GetFieldCount();
  std::cout << "rows: " << nrows_k << " columns: " << nfields << std::endl;

  Double_t ureadtime;
  Double_t value;
  std::vector<double> lr_time;
  std::vector<double> lr_value;

  for (Int_t i = 0; i < nrows_k; i++) {
    row = res->Next();
    for (Int_t j = 0; j < nfields; j++) {
      fieldname = TString( res->GetFieldName(j) );
      field = TString( row->GetField(j) );
      //      std::cout << "fieldname: " << fieldname << " field: " << field << std::endl;
      if ( fieldname == "UTIME" ) ureadtime = field.Atof();
      if ( fieldname == column ) value = field.Atof();
    }
    lr_time.push_back( ureadtime );
    lr_value.push_back( value );
  }

  TGraph *gr =  new TGraph( lr_time.size(), &lr_time[0], &lr_value[0] );

  return gr;
}


// --- timeseries.C


Int_t channel_to_canvas_odd( Int_t channel )
{

  Int_t canmap[] = {
    // 1 ... 15
    10+48,11+48,8+48,9+48,14+48,15+48,12+48,13+48,
    // 9 ... 16
    2+48,3+48,0+48,1+48,6+48,7+48,4+48,5+48,

    // 17 ... 24
    10+32,11+32,8+32,9+32,14+32,15+32,12+32,13+32,
    // 25 ... 32
    2+32,3+32,0+32,1+32,6+32,7+32,4+32,5+32,

    // 33 ... 40
    10+16,11+16,8+16,9+16,14+16,15+16,12+16,13+16,
    // 41 42 43 44 45 46 47 48
    2+16,3+16,0+16,1+16,6+16,7+16,4+16,5+16,

    // 49 50 51 52 53 54 55 56
    10,11,8,9,14,15,12,13,
    // 57 58 59 60 61 62 63 64
    2,3,0,1,6,7,4,5
  };

  if ( channel < 0 || channel > 63 ) return 0;
  return canmap[channel]+1;

}

void ledpeak::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
   std::cout << "option: " << option << std::endl;

   rn = option.Atoi();
   std::cout << "asked for run: " << rn << std::endl;

   TString hname;
   TString name = "EMCAL LED Run ";
   name += rn;
   for (Int_t i = 0; i < NEMCCHAN; i++ ) {
     hname = "h";
     hname += i;
     h[i] = new TH1F(hname,name,2048,0.0,4096.0);
   }

   TString rootfilename = "ledpeak_";
   rootfilename += rn;
   rootfilename += ".root";
   std::cout << "creating root file: " << rootfilename << std::endl;

   f = new TFile(rootfilename,"recreate");
   L = new TTree("L","LED");
   TString branchname;

   L->Branch("event",&event,"event/I"); 

   branchname = "tower["; branchname += NEMCCHAN; branchname += "]/I";
   std::cout << "branchname: " << branchname << std::endl;
   L->Branch("tower",tower,branchname); 

   branchname = "mean["; branchname += NEMCCHAN; branchname += "]/F";
   L->Branch("mean",mean,branchname); 

   branchname = "dmean["; branchname += NEMCCHAN; branchname += "]/F";
   L->Branch("dmean",dmean,branchname); 

   branchname = "sigma["; branchname += NEMCCHAN; branchname += "]/F";
   L->Branch("sigma",sigma,branchname); 

   branchname = "dsigma["; branchname += NEMCCHAN; branchname += "]/F";
   L->Branch("dsigma",dsigma,branchname); 

   branchname = "temperature["; branchname += NEMCCHAN; branchname += "]/F";
   L->Branch("temperature",temperature,branchname); 

   // get the temperature from the database

   TString where;
   for ( Int_t i = 0; i < NEMCCHAN; i++ ) {
     where = "WHERE RUNNUMBER=";
     where += rn;
     where += " AND DETID=";
     where += i;
     where += " ORDER BY UTIME ASC";
     grt[i] = timeseries("T0","EMCAL",where);
   }

   beginruntime = grt[0]->GetX()[0];
   std::cout << "begin run time: " << beginruntime << std::endl;

}

void ledpeak::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

Bool_t ledpeak::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either ledpeak::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

  fChain->GetEntry(entry);
  for (Int_t i = 0; i < NEMCCHAN; i++ ) { 
    h[i]->Fill( TMath::Abs(peak[i]-pedestal[i]) );
  }
  
  // Every 150 events, write out the mean and sigma

  if ( eventnumber%150 == 0 ) {

    event = eventnumber;
    for (Int_t i = 0; i < NEMCCHAN; i++ ) { 
      h[i]->Fit("gaus");
      fit = (TF1*) h[i]->GetFunction("gaus");
      mean[i] = fit->GetParameter(1);
      dmean[i] = fit->GetParError(1);
      sigma[i] = fit->GetParameter(2);
      dsigma[i] = fit->GetParError(2);
      
      // can write out histograms to check if you like
      //      h[i]->Write();

      tower[i] = 64 - channel_to_canvas_odd(i);
      h[i]->Reset();

      eventtime = beginruntime + event/2.45;
      temperature[i] = grt[i]->Eval(eventtime);

    }
    
    L->Fill();
  }
  
   return kTRUE;
}

void ledpeak::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void ledpeak::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

  L->Write();
  f->Close();

}
