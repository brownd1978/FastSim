//--------------------------------------------------------------------------
// Name:
//    PacShowerInfo
// Description:
//    Information struct for a piece of a simulated shower (EM or hadronic)
// Environment:
//    Software developed for PACRAT / SuperB
// Copyright Information:
//	  Copyright (C) 2008	Lawrence Berkeley Laboratory
// Author List:
//    Dave Brown 23 APR 2008
//------------------------------------------------------------------------

#ifndef PacShowerInfo_HH
#define PacShowerInfo_HH
class DetMaterial;
class PacShowerModel;

struct PacShowerInfo {
  PacShowerInfo(double eshower,double ein,double eout,double edep,const DetMaterial* mat,
  double din,double dout,
  double transx,double transy,double transcorr, const PacShowerModel* model) :
    _eshower(eshower),_ein(ein),_eout(eout),_edep(edep),_mat(mat),_din(din),_dout(dout),
    _transx(transx),_transy(transy),_transcorr(transcorr),_model(model)
    {}
  PacShowerInfo() : _eshower(0.0),_ein(0.0),_eout(0.0),_edep(0.0),_mat(0),
  _din(0.0),_dout(0.0),_transx(0.0),_transy(0.0),_transcorr(0.0),_model(0)
  {}
  PacShowerInfo(double eshower) : _eshower(eshower),_ein(eshower),_eout(eshower),_edep(0.0),_mat(0),
  _din(0.0),_dout(0.0),_transx(0.0),_transy(0.0),_transcorr(0.0),_model(0)
  {}
  
// accessors
  double showerEnergy() const { return _eshower;}
  double energyIn() const { return _ein;}
  double energyOut() const { return _eout;}
  double energyDeposit() const { return _edep;}
  const DetMaterial* material() const { return _mat;}
  double depthIn() const { return _din;}
  double depthOut() const { return _dout;}
  double depth() const { return _dout - _din;}
  double fractionIn() const { return (_eshower-_ein)/_eshower;}
  double fractionOut() const { return (_eshower-_eout)/_eshower;}
  double showerFraction() const { return (_ein-_eout)/_eshower;}
  const PacShowerModel* showerModel() const { return _model;}
// data members
  double _eshower; // total shower energy
  double _ein; // shower energy at start of element
  double _eout; // shower energy at end of element
  double _edep; // shower energy deposited in this element (including transition radiation energy)
  const DetMaterial* _mat; // material of this element
  double _din; // dimensionless longitudinal (z) position at the start of this shower piece
  double _dout; // dimensionless longitudinal (z) position at the end of this shower piece
  double _transx; // size (in cm) of the transverse spread of this shower piece in the 'x' direction
  double _transy; // size (in cm) of the transverse spread of this shower piece in the 'y' direction
  double _transcorr; // correlation between x and y spread
  const PacShowerModel* _model; // shower model used to generate this shower step
};

#endif
