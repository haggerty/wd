# wd (Waveform Digitizer)

## wd40

wd40 is code that builds in the sPHENIX environment to take a "prdf" (PHENIX Raw Data Format) file and convert it to a ROOT TTree (W) 
which contains a copy of the raw ADC data from the Columbia University "HBD digitizer" in the array adc[192][24] and an interpolated maximum (or minimum) ADC value in peak[192]
for each channel.

The macros directory contains various analysis code that was used to analyze the W TTree's in T-1044 in 2017.

This code is not particularly flexible, and expects prdf data in two packets, 21101 (144 channels) and 21102 (48 channels) and
many of the macros rely on hard-coded channel locations in the data.

## wd409

wd409 is code the builds in the sPHENIX environment to take a "prdf" (PHENIX Raw Data Format) file and convert it to a ROOT TTree (W) 
for the "new" ADC system being developed at Columbia University for the sPHENIX electromagnetic and hadronic calorimeters

The macros directory contains analysis code that was to analyze W TTree's taken in the HCAL lab at BNL.
