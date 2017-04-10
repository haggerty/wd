TTree *gettree( TString filename )
{
  TFile *f = new TFile( filename );
  TTree *W = f->Get("W");
  return W;

}
