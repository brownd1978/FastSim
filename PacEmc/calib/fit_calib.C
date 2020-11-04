#include <iostream>
#include "TROOT.h"
#include "TChain.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TString.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "RooAbsReal.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooArgSet.h"
#include "RooGaussian.h"
#include "RooCBShape.h"
#include "RooPlot.h"
#include "RooGlobalFunc.h"
#include "RooFitResult.h"
#include "RooCurve.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "TGraphErrors.h"
#include "TF1.h"

using namespace std;
using namespace RooFit;
using namespace RooFitShortHand;

enum region_t {FWD, BAR, BWD, nregions};
float thetalo[nregions]= { 0.28, 0.48, 2.67 };
float thetahi[nregions]= { 0.46, 2.44, 2.74 }; // FIXME. The last value is 157 degrees because of the shielding near IP, which should be relaxed to ~167 degrees

double interpolate(double y, double x1, double y1, double x2, double y2) {
  double m = (y2-y1)/(x2-x1);
  return x1+ (y-y1)/m;
}

void meanSigmaEstimator(RooDataSet &data, RooRealVar &var, double &m, double &s) {
  
  double mean = data.moment(var,1);
  double rms = sqrt(data.moment(var,2));

  TH1F *h= new TH1F("h", "h", 20, mean-2*rms, mean+2*rms);
  data.fillHistogram(h, var);
  //  h->Draw();

  int maxbin= h->GetMaximumBin();
  double maxo2= h->GetBinContent(maxbin)/2.0;
  
  int leftbin(maxbin-1); // find bin on the left just below max/2
  for ( int i= maxbin-1; i>1; i--) {
    leftbin= i;
    if ( h->GetBinContent(i) < maxo2 ) break;
  }
  int rightbin(maxbin+1); // find bin on the right just below max/2
  for ( int i= maxbin+1; i<= h->GetNbinsX(); i++) {
    rightbin= i;
    if ( h->GetBinContent(i) < maxo2 ) break;
  }
  double xlo= interpolate(maxo2, h->GetBinCenter(leftbin), h->GetBinContent(leftbin),  h->GetBinCenter(leftbin+1), h->GetBinContent(leftbin+1) );
  double xhi= interpolate(maxo2, h->GetBinCenter(rightbin-1), h->GetBinContent(rightbin-1),  h->GetBinCenter(rightbin), h->GetBinContent(rightbin) );
  
  m= h->GetBinCenter(maxbin);
  s= (xhi-xlo)/2.36;

}



RooFitResult* fitOne(TString filename, region_t region ) {

  TChain *tr= new TChain("emc");
  tr->Add(filename);
  float charge, ecal;
  TVector3 *clusterPos= new TVector3();
  TLorentzVector *ogtrackP4= new TLorentzVector();
  tr->SetBranchStatus("*", 0);
  tr->SetBranchStatus("charge", 1);
  tr->SetBranchStatus("ecal", 1);
  tr->SetBranchStatus("clusterPos", 1);
  tr->SetBranchStatus("ogtrackP4", 1);
  tr->SetBranchAddress("charge", &charge);
  tr->SetBranchAddress("ecal", &ecal);
  tr->SetBranchAddress("clusterPos", &clusterPos);
  tr->SetBranchAddress("ogtrackP4", &ogtrackP4);

  RooRealVar pull("pull", "pull", -1, 2);
  RooDataSet *data= new RooDataSet("data", "Ecal pull", RooArgSet(pull));

  for ( int i=0; i< tr->GetEntries(); i++) {
    tr->GetEntry(i);
    if ( charge != 0 ) continue;
    float angle = ogtrackP4->Theta();
    if ( angle < thetalo[region] ||  angle > thetahi[region] ) {
      continue;
    }
    pull = (ecal- ogtrackP4->E())/ogtrackP4->E();
//     if ( i < 10 ) {
//       cout << pull.getVal() << "  " << ecal << "   " <<  ogtrackP4->E() << endl;
//     }

    data->add(pull);
  }


  double mean, sigma;
  meanSigmaEstimator(*data, pull, mean, sigma);
  cout << "mean= " << mean << "    sigma = " << sigma << endl;

  pull.setRange(TMath::Max(mean-4*sigma,pull.getMin()),
		TMath::Min(mean+4*sigma,pull.getMax()));
  
  RooPlot *frame= pull.frame();
  data->plotOn(frame);

  // Gaussian
  RooRealVar m("m","mean", -0.1, -0.2, 0.0);
  RooRealVar s("s","sigma", sigma, 0.5*sigma, 2*sigma);
  RooGaussian gauss("gauss","gaussian", pull, m, s);
  double fitmin= TMath::Max(-0.5, mean-1.5*sigma);
  double fitmax= mean+ 1.5*sigma;
  gauss.fitTo(*data, Range(fitmin, fitmax));
  //gauss.plotOn(frame);
  //frame->Draw();

  cout << m.getVal() << "   " << s.getVal() << endl;

  // Crystal Ball
  RooRealVar alpha("alpha","alpha", 1, 0.5, 2);
  RooRealVar n("n","n", 5);
  RooCBShape cbshape("cbshape", "crystal ball", pull, m, s, alpha,n);
  RooFitResult *result= cbshape.fitTo(*data, Range(fitmin, fitmax), Save(1));
  cbshape.plotOn(frame);
  frame->Draw();
  return result;
}

TF1* fitOneSet(TString path, region_t region) {

  const int nenergies= 8;
  const char* regnames[nregions] = {"FWD", "BAR", "BWD" } ;
  const char* substr[nenergies] = {"0.05","0.1","0.2","0.5","1.0","2.0","3.0","4.0"};

  TCanvas *cv= new TCanvas(Form("res-%s", regnames[region]), regnames[region], 1000, 600);
  cv->Divide(4,2);

  TGraphErrors *gmean= new TGraphErrors(nenergies);
  gmean->SetTitle("Pull;Measured E(GeV);Mean of (Ecal-Egen)/Egen)");
  TGraphErrors *gsigma= new TGraphErrors(nenergies);
  gsigma->SetTitle("Resolution;Generated E(GeV);Sigma of (Ecal-Egen)/Egen)");
  for (int i=0; i< nenergies; i++) {
    TString file= path + "/" + regnames[region] + "-" + substr[i] + ".root";
    cout << file << endl;
    cv->cd(i+1);
    RooFitResult* result= fitOne(file, region);
    cv->Update();
    
    RooRealVar *m = (RooRealVar*)result->floatParsFinal().find("m");
    RooRealVar *s = (RooRealVar*)result->floatParsFinal().find("s");
    double etrue= atof(substr[i]);
    double emeas= etrue*(1+m->getVal());
    gmean->SetPoint(i, emeas, m->getVal());
    gmean->SetPointError(i, 0, m->getError());
    
    gsigma->SetPoint(i, etrue, s->getVal());
    gsigma->SetPointError(i, 0, s->getError());

  }

  // Mean as a function of measured energy
  TCanvas *cv2= new TCanvas(Form("gmean-%s",regnames[region]), regnames[region]);
  cv2->cd();
  TF1 *func= new TF1("func","[0]+[1]*(log([2]*x)+[3])*exp(-[2]*x)", 0.05, 4.0);
  func->SetParameters(-0.1, 0.01, 1, 1);
  gmean->Fit(func);
  gmean->Draw("APL");
  cv2->Update();

  // Sigma as a function of true energy
  TCanvas *cv3= new TCanvas(Form("gsigma-%s",regnames[region]), regnames[region]);
  cv3->cd();
  TF1 *rfunc= new TF1("rfunc", "sqrt([0]*[0]/pow(x,2*[1])+[2]*[2])", 0.05, 4.00);
  rfunc->SetParameters(0.05, 0.5, 0.01);
  gsigma->Fit(rfunc);
  gsigma->Draw("APL");

  cv3->Update();

  return func;
}


void fit_calib(TString path, int region= -1) {

  gROOT->SetStyle("BABAR");
  gStyle->SetNdivisions(505,"xyz");
  
  double pars[nregions][4];

  for (int i=0; i<nregions; i++) {
    if ( region>=0 && i!=region) continue;
    TF1* func= fitOneSet(path, (region_t)i);
    for ( int j=0; j<4; j++) {
      pars[i][j]= func->GetParameter(j);
    }
  }

  cout << "== Pull mean summary ==" << endl;
  for (int i=0; i<nregions; i++) {
    if ( region>=0 && i!=region) continue;
     for ( int j=0; j<4; j++) {
       cout << pars[i][j] << " , ";
     }
     cout << endl;
  }

}
