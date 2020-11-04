#include <Rtypes.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TTree.h>
#include <TCut.h>
#include <TLegend.h>
#include <TString.h>
#include <TStyle.h>
#include <TProfile.h>
#include <stdio.h>
#include "Riostream.h"


Double_t splitgaus(Double_t *x, Double_t *par) {
  Double_t retval;
  Double_t core;
  Double_t tail;
  Float_t xval = x[0];
  if(xval < par[1]) {
    core = exp(-0.5*pow((xval-par[1])/par[2],2))/par[2];
    tail = par[4]*exp(-0.5*pow((xval-par[1])/par[5],2))/par[5];
  } else {
    core = exp(-0.5*pow((xval-par[1])/par[3],2))/par[3];
    tail = (1/par[2]-1/par[3]+par[4]/par[5])*exp(-0.5*pow((xval-par[1])/par[6],2));
  }
  retval = par[0]*0.398942*(core+tail);
// add a tail Gaussian
  return retval;
}

void fitLandau(TCanvas* can, TH1D* landau) {
  TF1* sgau = new TF1("sgau",splitgaus,-5.,5.,7);
  sgau->SetParName(0,"Norm");
  sgau->SetParName(1,"Mean");
  sgau->SetParName(2,"SigH");
  sgau->SetParName(3,"SigL");
  sgau->SetParName(4,"TFL");
  sgau->SetParName(5,"TSigH");
  sgau->SetParName(6,"TSigL");
  
  double integral = landau->GetEntries()*landau->GetBinWidth(1);
  sgau->SetParameters(integral,0.0,0.5*landau->GetRMS(),0.8*landau->GetRMS(),0.02,2.0*landau->GetRMS(),0.5*landau->GetRMS());
//  sgau->SetParLimits(5,1.0*landau->GetRMS()/2.0,10.0);
//  sgau->SetParLimits(6,1.0*landau->GetRMS()/2.0,10.0);
//  sgau->SetParLimits(4,0.0,0.49);
//  sgau->FixParameter(1,0.0);
  landau->Fit("sgau","L","",-5,0.5);
  
}