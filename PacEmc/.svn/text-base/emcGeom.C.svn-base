void drawBarrel(int nbar, double radin, double *thick, double th1, double th2, double tolerance=0, double *rs=0, double *z1s=0, double *z2s=0) {
  // radin = radius of inner surface
  cout << endl;

  double totalThick=0;
  for ( int i= 0; i<nbar; i++) {
    totalThick+= thick[i];
  }

  TString cyltype = " type=\"EmcCylinder\"";
  if ( tolerance > 0 ) cyltype="";


  double thisthick=0;
  double radius= radin;
  for ( int i= 0; i<nbar; i++) {
    thisthick+= thick[i];
    radius+= 0.5*thick[i];

    double x[5], y[5];

    //cout << radius << endl;

    double z1= radius/tan(th1);
    double z2= radius/tan(th2);

    if ( tolerance > 0 ) {
      if (rs) rs[i]= radius;

      // Fix z1, z2 so that the path length is close to targeted total thickness
      if ( thisthick > (totalThick+ tolerance)*sin(th1) ) {
	double tt= (totalThick+tolerance)/thisthick;
	z1= sqrt((tt*tt-1))* (radius+0.5*thick[i]);
      }

      if ( thisthick > (totalThick+ tolerance)*sin(th2) ) {
	double tt= (totalThick+tolerance)/thisthick;
	z2= -sqrt((tt*tt-1))* (radius+0.5*thick[i]);
      }

      if ( z1s ) z1s[i]=z1;
      if ( z2s ) z2s[i]=z2;
    }

    x[0]= z1;
    x[1]= x[0];
    x[2]= z2;
    x[3]= x[2];
    x[4]= x[0];

    y[0]= radius-0.5*thick[i];
    y[1]= y[0]+thick[i];
    y[2]= y[1];
    y[3]= y[0];
    y[4]= y[0];

    TPolyLine *pline= new TPolyLine(5,x,y);
    pline->SetFillColor(38);
    pline->SetLineColor(4);
    pline->SetLineWidth(1);
    pline->Draw("f");
    pline->Draw();

    TLine *line= new TLine(x[0],radius,x[2],radius);
    line->SetLineColor(2);
    line->SetLineWidth(1);
    line->Draw();

    cout << "<cyl" << cyltype << " name=\"EmcCsIBarrel\" id=\""
	 << i <<"\" zmin=\"" << x[2] << "\" zmax=\"" << x[0]
	 << "\" radius=\"" << radius << "\" thick=\"" << thick[i]
	 << "\" mat=\"emc-CsI\" meas=\"Emc\" />" << endl;


    radius+= 0.5*thick[i];

  }
}


void drawBarrelPathLen(int nbar, double *barRs, double *barTk, double *barZ1, double *barZ2, double barTh1, double barTh2) {

  TCanvas *cvp= new TCanvas("cvp","cvp",700,500);
  TCanvas *cvh= new TCanvas("cvh","cvh",700,500);
  cvp->SetMargin(0.12,0.03,0.12,0.03);
  cvh->SetMargin(0.12,0.03,0.12,0.03);

  TGraph *gr= new TGraph();
  TH1F *h1= new TH1F("h1","h1",40,25,35);
  int np=0;
  for (double cth= cos(barTh2); cth<= cos(barTh1); cth+= 0.01 ) {
    double pathlen=0;
    for (int i=0; i<nbar; i++) {
      double z=0;
      if ( cth != 0 ) {
	double tan= sqrt(1-cth*cth)/cth;
	z= barRs[i]/tan;
      }

      if ( (z-barZ1[i])*(z-barZ2[i]) < 0 ) {
	pathlen+= barTk[i]/sqrt(1-cth*cth);
      }
    }

    gr->SetPoint(np,cth,pathlen);
    h1->Fill(pathlen);
    np++;
  }

  cvp->cd();
  gr->GetXaxis()->SetTitle("cos#theta");
  gr->GetYaxis()->SetTitle("path length (cm)");
  gr->Draw("APL");

  cvh->cd();
  h1->SetXTitle("path length (cm)");
  h1->SetStats(true);
  gStyle->SetOptStat(1111);
  h1->Draw();

}


void drawFwdEC(const char* name, const char* mat, int nfec, double *thick, double rho1, double th1, double th2, double thv) {
  // rho1 : outer radius of the first layer's center 
  // th1, th2:  the range of the polar angle of the fan 
  // thv : the cone vertex half angle

  double radius= rho1;
  double theta= thv- TMath::PiOver2();

  cout << endl;

  for ( int i=0; i<nfec; i++) {

    double dt= thick[i]/2.;
    double x[5], y[5];


    double y2= radius;
    double x2= y2/tan(th2);
    double x1= (y2-x2*tan(thv))/(tan(th1)-tan(thv));
    double y1= x1*tan(th1);

    x[0]= x2-dt*cos(theta);
    y[0]= y2-dt*sin(theta);
    x[1]= x2+dt*cos(theta);
    y[1]= y2+dt*sin(theta);
    x[2]= x1+dt*cos(theta);
    y[2]= y1+dt*sin(theta);
    x[3]= x1-dt*cos(theta);
    y[3]= y1-dt*sin(theta);
    x[4]= x[0];
    y[4]= y[0];

    TPolyLine *pline= new TPolyLine(5,x,y);
    pline->SetFillColor(79);
    pline->SetFillStyle(3002);
    pline->SetLineColor(103);
    pline->SetLineWidth(1);
    pline->Draw("f");
    pline->Draw();


    TLine *line= new TLine(x1,y1,x2,y2);
    line->SetLineColor(2);
    line->SetLineWidth(1);
    line->Draw();

    cout << "<cone name=\"" << name << "\" id=\"" << i << "\""
	 << " rho1=\"" << y1 << "\""
	 << " z1=\"" << x1 << "\""
	 << " rho2=\"" << y2 << "\""
	 << " z2=\"" << x2 << "\""
	 << " thick=\"" << thick[i]
	 << "\" mat=\"" << mat << "\" meas=\"Emc\" />" << endl;
    
    if ( i < nfec-1 ) {
      double dr= (dt+ thick[i+1]/2.)/cos(th2-theta);
      radius+= dr*sin(th2);
    }

  }
}


void drawBwdEC(const char* name, const char* mat, int nbec, double *thick, double rho1, double rho2, double z0) {

  cout << endl;

  double total= 0;
  for (int i=0; i< nbec; i++) {
    total+= thick[i];
  }

  double z=z0 + total/2.-thick[0]/2.;

  for (int i=0; i< nbec; i++) {

    double x[5], y[5];
    double dt= thick[i]/2.;

    y[0]= rho1;
    y[1]= rho2;
    y[2]= rho2;
    y[3]= rho1;
    y[4]= rho1;
    
    x[0]= z+dt;
    x[1]= z+dt;
    x[2]= z-dt;
    x[3]= z-dt;
    x[4]= z+dt;

    TPolyLine *pline= new TPolyLine(5,x,y);
    pline->SetFillColor(5);
    pline->SetFillStyle(3002);
    pline->SetLineColor(95);
    pline->SetLineWidth(1);
    pline->Draw("f");
    pline->Draw();


    TLine *line= new TLine(z,rho1,z,rho2);
    line->SetLineColor(2);
    line->SetLineWidth(1);
    line->Draw();

    cout << "<ring name=\"" << name << "\" id=\"" << i << "\""
	 << " z=\"" << z << "\""
	 << " lowradius=\"" << rho1 << "\""
	 << " hiradius=\"" << rho2 << "\""
	 << " thick=\"" << thick[i]
	 << "\" mat=\"" << mat << "\" meas=\"Emc\" />" << endl;


    if ( i<nbec-1){
      z-= dt+thick[i+1]/2;
    }


  }


}


void emcGeom(int opt=1) {

  gROOT->SetStyle("BABAR");
  gStyle->SetPadLeftMargin(0.05);
  gStyle->SetPadRightMargin(0.01);
  gStyle->SetPadBottomMargin(0.05);
  gStyle->SetPadTopMargin(0.01);
  gStyle->SetNdivisions(505,"xy");
 
  double barTh1 = atan(920./1801.);
  double barTh2 = TMath::Pi()-atan(920./1127.);
  double fecTh1 = atan(558./1979.);
  double fecTh2 = atan((920.-1.)/1801.);  // assume ~1mm gap
  double fecThv = (90+22.7) * TMath::Pi()/180.;

  cout << "FEC coverage: " << fecTh1 << "  --  " << fecTh2 << endl;
  cout << "Barrel coverage: " << barTh1 << "  --  " << barTh2 << endl;

  double xlo= -160; // double xlo= 160;
  double xhi= +250;
  double ylo=    0;
  double yhi= +150;
  double zoom= 2;
  TH2F *h2= new TH2F("h2","h2",1,xlo,xhi,1,ylo,yhi);
  TCanvas *cv= new TCanvas("cv","cv",zoom*(xhi-xlo),zoom*(yhi-ylo));
  h2->Draw();

  if ( opt ==1 ) { // =========================================

    const int nbar= 1;
    double barR1= 92;   // radius of the _front face_ of the first layer
    double barTk[nbar] = {30};
    
    drawBarrel(nbar, barR1, barTk, barTh1, barTh2);
    
    const int nfwd= 1;
    double fecR1 = 107;  // outer radius of the _center_ of the first layer
    double fecTk[nfwd] = {30};
    const char* fname= "EmcCsiFwd";
    const char* mname= "emc-CsI";
    drawFwdEC(fname, mname, nfwd, fecTk, fecR1, fecTh1, fecTh2, fecThv);

  } else if ( opt == 2 ) { // =========================================

    const int nbar= 1;
    double barR1= 92;
    double barTk[nbar] = {30};
    
    drawBarrel(nbar, barR1, barTk, barTh1, barTh2);
    
    const int nfwd= 1;
    double fecR1 = 107;
    double fecTk[nfwd] = {20};
    const char* fname= "EmcLSOFwd";
    const char* mname= "emc-LSO";
    drawFwdEC(fname, mname, nfwd, fecTk, fecR1, fecTh1, fecTh2, fecThv);
   
    const int nbwd=1;
    double becR1= 31;
    double becR2= 75;
    double becZ0= -120;  // center of the whole
    double becTk[nbwd]= {14};

    const char* fname= "EmcPbScintBwd";
    const char* mname= "emc-PbScint";
    drawBwdEC(fname, mname, nbwd, becTk, becR1, becR2, becZ0);


  } else if ( opt == 3 ) { // =========================================

    const int nbar= 6;
    double barR1= 92;
    double barTk[nbar] = {2,3,3,5,7,10};

    drawBarrel(nbar, barR1, barTk, barTh1, barTh2);
    
    const int nfwd= 6;
    double fecR1 = 93;
    double fecTk[nfwd] = {2,3,3,5,7,10};
    const char* fname= "EmcCsiFwd";
    const char* mname= "emc-CsI";
    drawFwdEC(fname, mname, nfwd, fecTk, fecR1, fecTh1, fecTh2, fecThv);

  } else if ( opt == 4 ) { // =========================================

    const int nbar= 6;
    double barR1= 92;
    double barTk[nbar] = {2,3,3,5,7,10};

    drawBarrel(nbar, barR1, barTk, barTh1, barTh2);
    
    const int nfwd= 6;
    double fecR1 = 93;
    double fecTk[nfwd] = {1,1,3,4,5,6};
    const char* fname= "EmcLSOFwd";
    const char* mname= "emc-LSO";
    drawFwdEC(fname, mname, nfwd, fecTk, fecR1, fecTh1, fecTh2, fecThv);


    const int nbwd=4;
    double becR1= 31;
    double becR2= 75;
    double becZ0= -120;  // center of the whole
    double becTk[nbwd]= {3.5,3.5,3.5,3.5};

    const char* fname= "EmcPbScintBwd";
    const char* mname= "emc-PbScint";
    drawBwdEC(fname, mname, nbwd, becTk, becR1, becR2, becZ0);

  } else if ( opt == 5 ) { // ===Default SuperB ?===================================

    const int nbar= 10;
    double barR1= 92;
    //double barTk[nbar] = {1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2};// {2,2,3,4,3,3,3,3,3,4};
    double barTk[nbar] = {2,2,3,3.9,3,3,3,3,3.9,3};
    double barRs[nbar];
    double barZ1[nbar];
    double barZ2[nbar];

    drawBarrel(nbar, barR1, barTk, barTh1, barTh2, 2, barRs, barZ1, barZ2);

    const int nfwd= 5;
    double fecR1 = 93;
    double fecTk[nfwd] = {1,2,4,6,7};
    const char* fname= "EmcLSOFwd";
    const char* mname= "emc-LSO";
    drawFwdEC(fname, mname, nfwd, fecTk, fecR1, fecTh1, fecTh2, fecThv);

    const int nbwd=4;
    double becR1= 31;
    double becR2= 75;
    double becZ0= -139;  // center of the whole
    double becTk[nbwd]= {3.5,3.5,3.5,3.5};

    const char* fname= "EmcPbScintBwd";
    const char* mname= "emc-PbScint";
    drawBwdEC(fname, mname, nbwd, becTk, becR1, becR2, becZ0);

    drawBarrelPathLen(nbar,barRs, barTk, barZ1, barZ2, barTh1, barTh2);

  } else if ( opt == 6 ) { // ===Default SuperB, FWD back 10 cm

    const int nbar= 10;
    double barR1= 92;
    //double barTk[nbar] = {1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2};// {2,2,3,4,3,3,3,3,3,4};
    double barTk[nbar] = {2,2,3,3.9,3,3,3,3,3.9,3};
    double barRs[nbar];
    double barZ1[nbar];
    double barZ2[nbar];

    drawBarrel(nbar, barR1, barTk, barTh1, barTh2, 2, barRs, barZ1, barZ2);

    const int nfwd= 5;
    double fecR1 = 93 + 10*sin(fecTh2);
    double fecTk[nfwd] = {1,2,4,6,7};
    const char* fname= "EmcLSOFwd";
    const char* mname= "emc-LSO";
    drawFwdEC(fname, mname, nfwd, fecTk, fecR1, fecTh1, fecTh2, fecThv);

    const int nbwd=4;
    double becR1= 31;
    double becR2= 75;
    double becZ0= -120;  // center of the whole
    double becTk[nbwd]= {3.5,3.5,3.5,3.5};

    const char* fname= "EmcPbScintBwd";
    const char* mname= "emc-PbScint";
    drawBwdEC(fname, mname, nbwd, becTk, becR1, becR2, becZ0);

    //drawBarrelPathLen(nbar,barRs, barTk, barZ1, barZ2, barTh1, barTh2);

  } else if ( opt==7 ) { // === Default Babar === ?

    const int nbar= 10;
    double barR1= 92;
    //double barTk[nbar] = {1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2};// {2,2,3,4,3,3,3,3,3,4};
    double barTk[nbar] = {2,2,3,3.9,3,3,3,3,3.9,3};
    double barRs[nbar];
    double barZ1[nbar];
    double barZ2[nbar];

    drawBarrel(nbar, barR1, barTk, barTh1, barTh2, 2, barRs, barZ1, barZ2);

    const int nfwd= 5;
    double fecR1 = 93;
    double fecTk[nfwd] = {1,3,6,9,11};
    const char* fname= "EmcCsiFwd";
    const char* mname= "emc-CsI";
    drawFwdEC(fname, mname, nfwd, fecTk, fecR1, fecTh1, fecTh2, fecThv);

    drawBarrelPathLen(nbar,barRs, barTk, barZ1, barZ2, barTh1, barTh2);

  }


  cv->cd();
  TLine *ln1= new TLine(0,0,250,250*tan(fecTh1));
  ln1->SetLineStyle(kDashed);
  ln1->Draw();

  TLine *ln2= new TLine(0,0,250,250*tan(fecTh2));
  ln2->SetLineStyle(kDashed);
  ln2->Draw();

  TLine *ln3= new TLine(0,0,250,250*tan(barTh1));
  ln3->SetLineStyle(kDashed);
  ln3->Draw();

  TLine *ln4= new TLine(0,0,-250,-250*tan(barTh2));
  ln4->SetLineStyle(kDashed);
  ln4->Draw();

}
