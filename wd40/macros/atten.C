Int_t atten( TString filename = "column3_214x.txt" )
{

  std::map<int,float> runtoz;
  
  runtoz[2136] = 4.2;
  runtoz[2138] = 29.7;
  runtoz[2140] = 55.2;
  runtoz[2141] = 80.6;
  runtoz[2142] = 106.3;
  runtoz[2143] = 131.5;
  runtoz[2144] = 156.6;
  runtoz[2146] = 182.4;

  ifstream sumfile;
  sumfile.open( filename );
  
  int rn;
  int tower;
  float mpv;
  float dmpv;

  std::string line;
  int linenum = 0; 
  while (getline (sumfile, line)) { 
    std::cout << line << std::endl;
    std::istringstream iss(line); 
    std::sumfile 
      >> rn
      >> tower
      >> mpv
      >> dmpv;
    if ( iss.rdstate() & std::ios::failbit ) {
      std::cout << "ignore line: " << line << std::endl;
      continue;
    }

    cout << mpv << " " << dmpv << endl;
    linenum++; 
  }

}


