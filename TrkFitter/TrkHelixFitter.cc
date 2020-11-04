//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkHelixFitter.cc 104 2010-01-15 12:13:14Z stroili $
//
// Description:
//     Performs least-squares fit for any simple track representation -- 
// circle, line, helix.  Depending on the state of flags, it will also 
// drop hits and flip ambiguities (for hits that allow such action) while 
// fitting.  (Originally written in C, which shows in a few spots.)
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author(s): Steve Schaffner
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include <assert.h>
#include "TrkFitter/TrkHelixFitter.hh"
#include "TrkBase/TrkSimpTraj.hh"
#include "TrkBase/TrkParams.hh"
#include "TrkBase/TrkHitOnTrk.hh"
#include "CLHEP/Matrix/Vector.h" 
#include "CLHEP/Geometry/HepPoint.h"
#include "CLHEP/Matrix/Matrix.h"
#include "CLHEP/Matrix/SymMatrix.h"
#include "TrkBase/TrkEnums.hh"
#include "TrkBase/TrkErrCode.hh"
#include "TrkBase/TrkHotList.hh"
#include "ErrLogger/ErrLog.hh"
#include <vector>
using std::cout;
using std::endl;

//------------------------------------------------------------------------
TrkHelixFitter::~TrkHelixFitter() {}
//------------------------------------------------------------------------

//------------------------------------------------------------------------
TrkHelixFitter::TrkHelixFitter(bool allowFlips, bool allowDrops) 
//------------------------------------------------------------------------
{
  _allowFlips = allowFlips;
  _allowDrops = allowDrops;
  _lastChisq = -1.;
}

//------------------------------------------------------------------------
TrkHelixFitter::TrkHelixFitter(const TrkHelixFitter& right) 
//------------------------------------------------------------------------
{
  _allowFlips = right._allowFlips;
  _allowDrops = right._allowDrops;
  _lastChisq = -1.;
}

//------------------------------------------------------------------------
TrkHelixFitter& 
TrkHelixFitter::operator=(const TrkHelixFitter& right) 
//------------------------------------------------------------------------
{
  if (&right == this) return *this;
  _allowFlips = right._allowFlips;
  _allowDrops = right._allowDrops;
  _lastChisq =  right._lastChisq;

  return *this;
}

//------------------------------------------------------------------------
void 
TrkHelixFitter::setFittingPar(bool allowFlips, bool allowDrops) {
//------------------------------------------------------------------------
  _allowFlips = allowFlips;
  _allowDrops = allowDrops;
}

//------------------------------------------------------------------------
TrkErrCode 
TrkHelixFitter::fit(TrkHotList& hitlist,
                    TrkSimpTraj& theTraj) {
//------------------------------------------------------------------------
  // Assumes that weight matrix is diagonal. */
  /* Least-squares fit; the measured 
     quantity is the residual.  The fit is accomplished by linearizing 
     the equation, using the derivatives of the residual w/r/t the 
     track parameters; because of this approximation, the fit may be iterated. 
     The fitted parameters are given by:
     delta-param() = Vparam * Atran * Vyinv * delChi
     where Vyinv = covariance matrix for the measurements
     Atran = transpose of A
     A = matrix of derivatives of delChi wrt trk params 
                                     (size = no. of params x no. of hits)
     Vparam = covariance (error) matrix of the fitted parameters
     = (Atran * Vyinv * A)**-1
     */

  bool permitFlips = _allowFlips;
  bool lPickHits = _allowDrops;
  // permitFlips = 1 => permit state changes like ambiguity flips
  // lPickHits = 1 => choose the best set of active hits on each iteration     
  int i;
  TrkErrCode status(TrkErrCode::succeed);
  int lPicked = 0;   // = 1 => have either picked up or dropped an active hit
                                              // on this iteration
   double chisqold;
  double chisqnew, chichange;
  double chitest = 0.01; //delta(chi2) < chitest => fit has converged
  int nZ = 0, nXY = 0;    //  # active hits in each view
  int nActive = 0;

  // vparam = Vparam defined above ( = symmetric matrix)
  // diffsum = Atran * Vyinv * delChi defined above (column vector)
  // iter = iteration loop index
  // itermax = max number of iterations
  // delpar = change in parameters during this iteration
  // chisqold, chisqnew = chisq before and after latest iteration

  /***************************************************************************/
  setLastChisq(-1.);
  bool shiftRef = false;
  //  HepPoint storePoint;

  // Change reference point of trajectory to be at first hit -- reduces 
  //   numerical problems
  //  double oldT0 = hitlist[0]->parentTrack()->trackT0();
  if (shiftRef) {
    //    double firstFlight = hitlist[0]->fltLen();
    //    double newTime = hitlist[0]->parentTrack()->fitResult()->arrivalTime(firstFlight);
    //    hitlist[0]->parentTrack()->resetT0(newTime);
    //    HepPoint here = theTraj.position(firstFlight);
    //
    //    storePoint = here;
    //    DifPoint dfPos;
    //    DifVector dfDir, dfDelDir;
    //    theTraj.getDFInfo(firstFlight, dfPos, dfDir, dfDelDir);
    //
    //    theTraj.changePoint(here, fltOffset);
  }    

  //*** Things that don't change with each iteration 
  int nhits = hitlist.nHit();
  std::vector<double> delChi(nhits,0);
  std::vector<std::vector<double> > deriv(nhits);

  TrkParams &params = *(theTraj.parameters());
  int npar = params.nPar();
  // Decide minimum numbers of hits required.  This could turn out to be wrong
  //     someday.
  bool l3d = (npar > 3);  // I hope always true
  const int minZ = l3d ? 2 : 0;
  const int minXY = npar - minZ;
  const int minAct = minZ + minXY;

  HepSymMatrix vparam(npar,0);
  HepVector diffsum(npar);
  HepVector delpar(npar);

  std::vector<std::vector<double> >::iterator ideriv = deriv.begin();
  std::vector<double>::iterator idelChi = delChi.begin();
  assert(deriv.size()==(hitlist.end()-hitlist.begin()));
  for (TrkHotList::nc_hot_iterator ihit = hitlist.begin(); ihit != hitlist.end(); ++ihit,++ideriv,++idelChi) {
    ideriv->resize(npar);
    if (ihit->isActive()) {
      nActive++;
      if (ihit->whatView() == TrkEnums::xyView) nXY++;
      else if (ihit->whatView() == TrkEnums::zView) nZ++;
      else if (ihit->whatView() == TrkEnums::bothView) {
        nZ++;
        nXY++;
      }
    }

    // Update the Hots to reflect new reference point
    if (shiftRef) {
      //      ihit->setFltLen( ihit->fltLen() - fltOffset );
    }
  }  //end loop over hits

  if (nXY < minXY || nZ < minZ || nActive < minAct) {
    ErrMsg(warning) << "Not enough hits " 
                    << nXY << " " << nZ << " " << nActive << endmsg;
    status.setFailure(11,"Not enough hits in TrkHelixFitter");
    return status;
  }


  if (shiftRef) {
    //    double firstFlight = hitlist[0]->fltLen();
    //    HepPoint here = theTraj.position(firstFlight);
    //    DifPoint dfPos;
    //    DifVector dfDir, dfDelDir;
    //    theTraj.getDFInfo(firstFlight, dfPos, dfDir, dfDelDir);
    //    double dummy = 0.; 
    //    hitlist[0]->updateFitStuff(dummy, 0, !permitFlips);
    //    hitlist[0]->updateMeasurement();
    //    hitlist[0]->updateFitStuff(dummy, 0, !permitFlips);
   }
 
  HepVector derivs(npar);
  TrkErrCode calcResult;
  //**** Iterate fit. 
  size_t itermax = 12;
  for (size_t iter = 1; iter <= itermax; iter++) {
    bool mustIterate(false);   // flag to force another iteration
    chisqold = 0.0;
    for (i = 0; i < npar; i++) diffsum[i] = 0.0;
    vparam *= 0.0;  // dumb way of clearing matrix

    /* Loop over hits, accumulate sums, calculate chisq for current params. */
    std::vector<std::vector<double> >::iterator ideriv = deriv.begin();
    std::vector<double>::iterator idelChi = delChi.begin();
    assert(deriv.size()==(hitlist.end()-hitlist.begin()));
    for (TrkHotList::nc_hot_iterator ihit = hitlist.begin(); ihit != hitlist.end(); ++ihit,++ideriv,++idelChi) {

      // Ask the hit to do the calculations
      calcResult = updateMeasurement(*ihit,0,!permitFlips);
      double deltaChiNew;
      if (calcResult.success()) {
        if (iter < 2) { // FIXME? only update derivatives at first iteration...
          calcResult = ihit->getFitStuff(derivs, deltaChiNew);
          for (i=0; i<npar; ++i) (*ideriv)[i] = derivs[i];
#if 0
          cout << "deriv: ";
          for (i=0;i<npar;++i) cout << (*ideriv)[i] << " " ;
          cout << endl;
#endif
        } else  {
          calcResult = ihit->getFitStuff(deltaChiNew);
        }
      }
      if (calcResult.failure()) {
        ErrMsg(warning) << "unable to getFitStuff for hit " << *ihit << endmsg;
        ihit->setUsability(false);  // something bombed
        continue;
      }
      mustIterate = (mustIterate || (calcResult.success() != 1));
      *idelChi = deltaChiNew;
#if 0
            cout << (ihit-hitlist.begin()) << " dChi " << *idelChi
                 << " amb " << ihit->ambig() 
                 << " resid " << ihit->resid() 
                 << " rms " << ihit->hitRms() 
                 << " hitlen " << ihit->hitLen() 
                 << " fltlen " << ihit->fltLen()  << endl;
#endif
      if (ihit->isActive() == false) continue;
      chisqold += deltaChiNew * deltaChiNew;

      for (i = 0; i < npar; ++i) {
        diffsum[i] += (*ideriv)[i] * deltaChiNew;
        for (int j = 0; j < i+1; ++j) {
           vparam.fast(i+1,j+1) += (*ideriv)[i] * (*ideriv)[j];
        }
      }
    }  // end loop over hits


    //****  Calculate new paramters
    int ierr;
    vparam.invert(ierr);
    if (ierr) {
      ErrMsg(warning) << "Matrix inversion failed " << endmsg;
      status.setFailure(12, "Matrix inversion failed in TrkHelixFitter");
      break;
    }
    delpar = vparam * (-diffsum);
#if 0
    cout << delpar << endl;
#endif
    // The following test relies on having a fixed location for phi0 in 
    //   all simple params; it should be made robust somehow!!!
    if (fabs(delpar[1]) > 1.) {
      ErrMsg(warning) << "Pathological  fit " << endmsg;
      status.setFailure(13, "Pathological fit in TrkHelixFitter.");
      break;
    }

    for (i = 0; i < npar; ++i) params.parameter()[i] += delpar[i];
#if 0
    cout << params.parameter() << endl;
#endif
    
    //***** Loop through the hits again, calculating the approx change 
    //   in residuals and chisq., and picking which hits should be active 
    //      for next iteration.

    chisqnew = 0.0;
    lPicked = 0;
    double bigDelChi = 0.0;
    TrkHotList::nc_hot_iterator bigHit = hitlist.end();

    mustIterate = (mustIterate || (iter <= 2 && lPickHits));   // iterate until hit-dropping allowed
    ideriv = deriv.begin();
    idelChi = delChi.begin();
    for (TrkHotList::nc_hot_iterator ihit = hitlist.begin(); ihit != hitlist.end(); ++ihit,++ideriv,++idelChi) {
      if(!ihit->isUsable()) continue;
      //double weight = ihit->weight(); // FIXME: why isn't weight used???
      for (i = 0; i < npar; i++) {
              *idelChi += (*ideriv)[i] * delpar[i];
      }
      if (ihit->isActive()) chisqnew += *idelChi * *idelChi;

      // Hit-picking
      if (!mustIterate && lPickHits) {
        double abDelChi = fabs(*idelChi);
        if (abDelChi <= 3.0) {
          if (ihit->isActive() == 0) {
            ihit->setActivity(1);  // reactivate hit
            //            cout << "Added " << endl;
            lPicked = 1;
            nActive++;
            if (ihit->whatView() == TrkEnums::xyView) nXY++;
            else if (ihit->whatView() == TrkEnums::zView) nZ++;
            else if (ihit->whatView() == TrkEnums::bothView) {
              nZ++;
              nXY++;
            }
          }
        } else {
          if (ihit->isActive()) {
            if (abDelChi > bigDelChi) {
              bigDelChi = abDelChi;
              bigHit = ihit;
            }
          }
        }
      } // end if iter > 2 (hit-picking)
    } //end loop over hits

    // Drop hit with worst residual
    if (lPickHits) {
      int lDrop = 0;
      if (bigHit != hitlist.end() && (nActive > minAct)) {
        if ( bigHit->whatView() == TrkEnums::xyView && nXY > minXY)  {
          nXY--;
          lDrop = 1;
        } else if ( bigHit->whatView() == TrkEnums::zView && nZ > minZ)  {
          nZ--;
          lDrop = 1;
        } else if ( bigHit->whatView() == TrkEnums::bothView && nZ > minZ && 
                    nXY > minXY) {
          nZ--;
          nXY--;
          lDrop = 1;
        }
        if (lDrop == 1) {
          lPicked = 1;
          nActive--;
          bigHit->setActivity(0);    // deactivate hit
        }
      } 
    }// end if lPickHits

    /* Test for convergence. */
    chichange = chisqold - chisqnew;
#if 0
    cout << chisqold << " -> " << chisqnew << endl;
#endif
    if (chichange < -0.5 && !mustIterate && lPicked == 0) {
      ErrMsg(warning) << " blowing up: " << chichange << endmsg;
      /* It's blowing up. */
      setLastChisq(chisqnew);
      status.setFailure(1);
      break;
    } else if (chichange < chitest && !mustIterate && lPicked ==0){
      // We converged.
      // ErrMsg(warning) << "converged" << endmsg;
      status.setSuccess(1); 
      setLastChisq(chisqnew);
      break;
    }

    if (iter == itermax) {
      setLastChisq(chisqnew);
      // ErrMsg(warning) << "converged (itermax)" << endmsg;
      status.setSuccess(2);
    }
  }    /* end iteration loop */

  // store the error matrix  
  params.covariance() = vparam;

  // Attempt to calculate deltaChisq for this hit (compared to leaving hit 
  //     out of the fit).
  /*  chisqnew = 0;
  if (status.success()) {
    HepVector deltaAlpha(npar);
    for (ihit = 0; ihit < nhits; ihit++) {
      thisHot = hitlist(ihit);
      if (!thisHot->isActive()) continue;
      HepVector derivs(npar, 0);
      for (i = 0; i < npar; i++) {
        derivs[i] = deriv[ihit][i];
      }
      double weight = thisHot->weight();
      double resid = thisHot->resid();
      deltaAlpha = vparam * derivs;
      deltaAlpha *= (resid * weight);
      //      cout  << resid * resid * weight
      //           << "  " << resid * weight * dot(derivs, deltaAlpha) << endl;
      //      thisHot->chi2Contrib = -dot(deltaAlpha, temp) + resid * resid * weight
      //        + 2. * resid * weight * dot(derivs, deltaAlpha);
    }
  }
      */

  // Change reference point back to origin
  if (shiftRef) {
    //    HepPoint home(0.,0.,0.);
    //    theTraj.changePoint(home, fltOffset);
    //    hitlist[0]->parentTrack()->resetT0(oldT0);
    //for (ihit = 0; ihit < nhits; ihit++) {
      //      thisHot = hitlist[ihit];
      //      thisHot->setFltLen( thisHot->fltLen() - fltOffset );
    // }
  }
  return status;
}
