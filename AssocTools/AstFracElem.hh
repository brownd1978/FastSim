//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AstFracElem.hh 436 2010-01-13 16:51:56Z stroili $
//
// Description:
//	Class AstFracElem.  Defines an element in a fractional association
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//     David Brown
//
// Copyright Information:
//	Copyright (C) 2000		Lawrence Berkeley Laboratory
//
//------------------------------------------------------------------------

#ifndef ASTFRACELEM_HH
#define ASTFRACELEM_HH

template <class B> 
class AstFracElem {

public:
  // construct give the weight and the thing being associated
  AstFracElem(const B*,double weight);
  AstFracElem();
  AstFracElem(const AstFracElem& other);


  virtual ~AstFracElem();

  // accessors
  const B* element() const { return _elem; }

  double weight() const { return _weight; }

  AstFracElem& operator = (const AstFracElem& other);

  // operators needed for RW sorting; these use weights
  bool operator < (const AstFracElem<B>& other) const;
  bool operator > (const AstFracElem<B>& other) const;
  bool operator == (const AstFracElem<B>& other) const;
  bool operator != (const AstFracElem<B>& other) const;

  // element equivalence

  bool sameElement(const AstFracElem<B>& other) const;
  bool ltElement(const AstFracElem<B>& other) const;
  bool gtElement(const AstFracElem<B>& other) const;

protected:

  // allow incrementing

  AstFracElem& operator += (const AstFracElem<B>&);
  AstFracElem& operator -= (const AstFracElem<B>&);

  // allow rescaling (normalization)
  void rescale(double factor) { _weight *=factor; }

private:

  double _weight;  // 'weight' of this association
  const B* _elem; // association element

};


#ifdef    BABAR_COMP_INST
#include  "AssocTools/AstFracElem.icc"
#endif // BABAR_COMP_INST


#endif

