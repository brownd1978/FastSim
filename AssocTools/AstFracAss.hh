//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AstFracAss.hh 436 2010-01-13 16:51:56Z stroili $
//
// Description:
//	Class AstFracAss.  Defines a fractional association between 2 objects
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

#ifndef ASTFRACASS_HH
#define ASTFRACASS_HH

#include "AssocTools/AstFracElem.hh"
#include "AssocTools/AstAbsAssociation.hh"

template <class A,class B> class AstSTLFracMap;

template <class A,class B> 
class AstFracAss : public AstFracElem<B>, 
		   public AstAbsAssociation<A,B> {

public:

// construct give the weight and the things being associated

  AstFracAss();
  AstFracAss(const A*,const B*,double weight);
  AstFracAss(const AstFracAss& other);

  virtual ~AstFracAss();

  // accessors
  virtual const A* firstArgument() const;
  virtual const B* secondArgument() const;


  // Assignment op
  AstFracAss& operator = (const AstFracAss& other);

  // allow arithmetic operations (on the weights!)
  AstFracAss& operator += (const AstFracAss&);
  AstFracAss& operator -= (const AstFracAss&);



  // operators needed for RW sorting

  bool operator < (const AstFracAss& other) const;
  bool operator > (const AstFracAss& other) const;
  bool operator == (const AstFracAss& other) const;
  bool operator != (const AstFracAss& other) const;

protected:

private:
  
  void rescale(double factor) { AstFracElem<B>::rescale(factor); }

  friend class AstSTLFracMap<A,B>;

  const A* _aelem; // association element

};

#ifdef    BABAR_COMP_INST
#include  "AssocTools/AstFracAss.icc"
#endif // BABAR_COMP_INST

#endif

