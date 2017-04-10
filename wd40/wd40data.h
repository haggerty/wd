#ifndef __WD40DATA_H__
#define __WD40DATA_H__

#include "TObject.h"

static const int NRMOD = 4;
static const int NMODBYXMIT[] = {3,1};

static const int NXMIT = sizeof(NMODBYXMIT)/sizeof(*NMODBYXMIT);
static const int PACKET[NXMIT] = {21101,21102};

static const int NCHANPERMOD = 48;
static const int NRSAM = 24;
static const int NPEDSAM = 3;

class wd40data
{

 public:
  int runnumber;
  int eventnumber;
  unsigned int scaledtrig;
  unsigned int crossctr;
  unsigned int eventtime;
  int nrmodules[NXMIT];
  int nrsamples[NXMIT];
  int parity[NXMIT];
  int trigger[NRMOD];
  int bclk[NRMOD];
  int moduleid[NRMOD];
  int adc[NRMOD*NCHANPERMOD][NRSAM];
  float mean[NRMOD*NCHANPERMOD];
  float rms[NRMOD*NCHANPERMOD];
  int minadc[NRMOD*NCHANPERMOD];
  int maxadc[NRMOD*NCHANPERMOD];
  float pedestal[NRMOD*NCHANPERMOD];
  float pedsig[NRMOD*NCHANPERMOD];
  float peak[NRMOD*NCHANPERMOD];
  float time[NRMOD*NCHANPERMOD];
  float integral[NRMOD*NCHANPERMOD];
  float integral2[NRMOD*NCHANPERMOD];
  int summary[NXMIT];

  wd40data() {
    runnumber = 0;

  };
  
  ~wd40data() {
  }; 

};

#endif /* __WD40DATA_H__ */
