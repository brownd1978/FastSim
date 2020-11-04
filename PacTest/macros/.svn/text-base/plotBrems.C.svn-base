void plotBrems(TCanvas* can, TTree* tree) {
  TH1F* de = new TH1F("de","Energy loss",500,0,105);
  TH1F* deb = new TH1F("deb","Energy loss",500,0,105);
  TH1F* dei = new TH1F("dei","Energy loss",500,0,105);
  de->GetXaxis()->SetTitle("MeV");
  deb->GetXaxis()->SetTitle("MeV");
  dei->GetXaxis()->SetTitle("MeV");
  de->SetLineColor(kBlack);
  deb->SetLineColor(kRed);
  dei->SetLineColor(kBlue);
  tree->Project("de","1000*(pi_mag-pf_mag)");
  tree->Project("deb","1000*(pi_mag-pf_mag)","effect==3");
  tree->Project("dei","1000*(pi_mag-pf_mag)","effect==5");
  can->Clear();
  can->Divide(1,1);
  can->cd(1);
  gPad->SetLogy();
  de->Draw();
  deb->Draw("same");
  dei->Draw("same");
  
  TLegend* leg = new TLegend(0.5,0.7,0.8,0.9);
  leg->AddEntry(de,"All","L");
  leg->AddEntry(deb,"High-E Brems","L");
  leg->AddEntry(dei,"Ionization + low-E Brems","L");
  leg->Draw();
}