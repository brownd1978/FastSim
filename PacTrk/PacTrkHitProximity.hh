#ifndef PacTrkHitProximity_HH
#define PacTrkHitProximity_HH

class PacHitOnTrk;
// simple struct describing the proximity of 2 hits
struct PacTrkHitProximity {
  const PacHitOnTrk* _h1;
  const PacHitOnTrk* _h2;
  double _msep; // distance between hits in the measurement direction
  double _wsep; // distance between hits in the 'wire' or strip direction
};

#endif