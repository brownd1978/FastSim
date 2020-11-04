//-----------------------------------------------------------------------
// File and Version Information:
//   PacEmcClusterSplitter
//     Class for splitting a PacEmcCluster to several single bump
//     clusters if the original PacEmcCluster has multiple bumps.
//
// Author List:
//     Chih-hsiang Cheng       Caltech   (initial version 2009/02/03)
//
//-----------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------

#include "PacEmc/PacEmcClusterSplitter.hh"
#include "CLHEP/Geometry/HepPoint.h"
#include "ErrLogger/ErrLog.hh"

//---------------
// C++ Headers --
//---------------

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <math.h>

using namespace std;


bool compare(const PacEmcCluster* c1, const PacEmcCluster* c2);


//----------------
// Constructors --
//----------------

PacEmcClusterSplitter::PacEmcClusterSplitter() :
_rMo(3.7),  _maxIterations(10), _tolerance(0.1), _exponent(-2.5)
  ,_numIterations(0)
{
}

//--------------
// Destructor --
//--------------
PacEmcClusterSplitter::~PacEmcClusterSplitter()
{
}

void
PacEmcClusterSplitter::clear() {
  _numIterations=0;
}


void
PacEmcClusterSplitter::splitCluster(PacEmcCluster *cluster,std::vector< PacEmcCluster* >& splittedClusters) {

  static const unsigned maxnbumps(32);
  _numIterations=0;
  unsigned nbumps= cluster->nBumps();
  if ( nbumps <= 1 ) {
    // do nothing
    return;
  }
  if ( nbumps > maxnbumps ) {
    nbumps= maxnbumps;
    if ( ErrLogging( debugging ) ) {
      ErrMsg(warning) << "There are " << nbumps << " in this cluster, "
		      << "more than the limit " << maxnbumps
		      << ". Will only split up to the maximum clusters."
		      << endmsg;
      cluster->print(cout,true);
    }
  }

  double weights[maxnbumps];  // assume there won't be more than 8 bumps

  // A vector to store cluster centroids in previous iteration
  std::vector<HepPoint> centroids;

  //  cout << "before splitting " << endl;
  //  cluster->print(cout,1);

  for ( int k=0; k< nbumps; ++k) {
    PacEmcCluster *cl= new PacEmcCluster(cluster->centroidAlgo());
    HepPoint cento= (*cluster->localMaxima())[k]->where();

    //    clusterCentroidMap[cl]= cento;
    splittedClusters.push_back(cl);
    centroids.push_back(cento);
  }

  unsigned niterations=0;

  while ( niterations < _maxIterations ) {

    // loop over cluster digis
    PacEmcDigiConstIterator diter= cluster->pacEmcDigis()->begin();
    while ( diter!= cluster->pacEmcDigis()->end() ) {

      HepPoint rdigi= (*diter)->where();
      double sumweights=0;

      for ( int k=0; k<nbumps; ++k) {
        double ebump;
  // Take the energy of the original local maximum as the bump energy
  //  at the first iteration
        if ( niterations==0 ) ebump= ((*cluster->localMaxima())[k])->energy();
        else ebump= splittedClusters[k]->energy();

  // Distance between the digi in question and each bump.
        double distance= rdigi.distanceTo(centroids[k]);
  // Unnormalized weight for this digi
        weights[k] = weightingFunc(ebump, distance);
        sumweights+= weights[k];
      }

      // Assign a weight to this digi for each bump
      for ( int k=0; k<nbumps; ++k) {
        weights[k] /= sumweights;    // normalizing weight

        if ( niterations==0 ) {
    // First iteration, copy this digi and scale it by weight,
    // and add it to the new cluster
          PacEmcDigi adigi(*(*diter));
          adigi.scale(weights[k]);
          adigi.scalesim(weights[k]);
          splittedClusters[k]->addDigi(adigi);
        } else {
    // Scale the digi in the new cluster by weight
          splittedClusters[k]->setDigiEnergy(*(*diter)->tci(), (*diter)->energy() * weights[k], (*diter)->simenergy() * weights[k] );
        }
      }

      diter++;

    } // end looping original digis


    niterations++;
    bool stable= true;

    // check whether iteration is stable
    // copy centroids of splitted clusters to the vector centroids
    for (int k=0; k<nbumps; ++k) {
      if ( stable &&
      centroids[k].distanceTo(splittedClusters[k]->where() ) > _tolerance ) {
        stable = false;
      }
      centroids[k]= splittedClusters[k]->where();
    }


    if ( stable ) break;
  } // end iteration loop

  //  cout << " splitted clusters" << endl;
   
  for ( unsigned k=0; k<splittedClusters.size(); ++k) {
    PacEmcCluster *scl= splittedClusters[k];
    scl->removeNoise(PacEmcCluster::LowEnergy, 0.0001);
    if ( scl->numberOfDigis() > 0 ) {
      scl->searchLocalMax();
      scl->setSimTrkEnergyChange(cluster->simtrkEnergyChange() * 
				 scl->energy()/cluster->energy() );
      scl->setFirstSimhitTime(cluster->firstSimhitTime());
      scl->setRecordTime(cluster->recordTime());
      scl->setRecordTimeErr(cluster->recordTimeErr());
      //    scl->print(cout,1);

    } else {
      delete scl;
      splittedClusters[k]=0;
    }
  }

  // Sort the clusters from highest energy to lower energy
  //std::sort(splittedClusters.begin(), _splittedClusters.end(), compare);

  _numIterations= niterations;
}


double
PacEmcClusterSplitter::weightingFunc(const double energy, const double dR) {
  return energy * exp( _exponent * dR/ _rMo)  ;
}

bool
compare(const PacEmcCluster* c1, const PacEmcCluster* c2) {
  return c1->energy() > c2->energy();
}
