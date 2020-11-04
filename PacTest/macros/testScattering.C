#include "TMath.h"
#include <iostream>
#include <iomanip>
#include "TH1F.h"
#include "TF1.h"
#include "TFile.h"
#include "TTree.h"
#include "TCut.h"
#include "TCanvas.h"
#include "TLegend.h"
#include <vector>
#include <string>
void testScattering(TCanvas* can, std::vector<std::string>& files,double* trange=0);

void testScattering(TCanvas* can,std::vector<std::string>& files,double* trange){
  int colors[6] = {kRed,kBlue,kGreen,kMagenta,kCyan,kBlack};
  std::vector<TFile*> tfiles;
  std::vector<TTree*> trees;
  std::vector<TH1F*> histos;
  char cutstring[100];
  TCut tcut("effect==5");
  if(trange != 0){
    snprintf(cutstring,100,"thick>%f&&thick<%f",trange[0],trange[1]);
    tcut += TCut(cutstring);
  }
  for(unsigned ifile=0;ifile<files.size();ifile++){
    TFile* tfile = new TFile(files[ifile].c_str());
    if(tfile != 0){
      tfiles.push_back(tfile);
      TTree* tracks = (TTree*)tfile->Get("tracks");
      if(tracks != 0){
        trees.push_back(tracks);
        char name[20];
        snprintf(name,20,"h%i",ifile);
        TH1F* hist = new TH1F(name,"Normalized scattering angle",500,-15,15);
        histos.push_back(hist);
        tracks->Project(name,"pf_theta*33.333333*(pi_mag+pf_mag)/sqrt(thick)",tcut);
        snprintf(name,20,"h%i+",ifile);
        tracks->Project(name,"pf_phi*33.333333*(pi_mag+pf_mag)/sqrt(thick)",tcut);
      } else {
        std::cerr << "Can find tree 'tracks' in file " << files[ifile] << std::endl;
        return;
      }
    } else {
      std::cerr << "Can't find file " << files[ifile] << std::endl;
      return;
    }
  }
  can->Divide(1,1);
  can->cd(1);
  gPad->SetLogy(1);
  TLegend* leg = new TLegend(0.6,0.6,0.9,0.9);
  for(unsigned ifile=0;ifile<files.size();ifile++){
    histos[ifile]->SetStats(0);
    histos[ifile]->SetLineColor(colors[ifile]);
    if(ifile > 0)
      histos[ifile]->Draw("same");
    else
      histos[ifile]->Draw();

    leg->AddEntry(histos[ifile],files[ifile].c_str(),"L");
  }
  leg->Draw();
}
