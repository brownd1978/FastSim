#ifndef GEVENT_HH
#define GEVENT_HH

#include <iostream>

#include "AbsEvent/AbsEvtObj.hh"

class GEvent : public AbsEvtObj {

public:
        // Default constructor for RW persistance
	GEvent();

        // operator== for RW
	virtual bool operator==( const GEvent &otherEvent ) const;

	void printAll ( std::ostream &o = std::cout ) const;

	friend std::ostream &operator<<(std::ostream &, const GEvent&);
	friend std::ostream &operator<<(std::ostream &, const GEvent*);
	
	int idrun() const { return _idrun; }
	void setIdrun(const int i) { _idrun = i; }
	
	int idevt() const { return _idevt; }
	void setIdevt(const int i) { _idevt = i; }
	
	int nrndm1() const { return _nrndm1; }
	void setNrndm1(const int i) { _nrndm1 = i; }
	
	int nrndm2() const { return _nrndm2; }
	void setNrndm2(const int i) { _nrndm2 = i; }
	
	int ntrack() const { return _ntrack; }
	void setNtrack(const int i) { _ntrack = i; }
	
	int nvertx() const { return _nvertx; }
	void setNvertx(const int i) { _nvertx = i; }
	
	int iday() const { return _iday; }
	void setIday(const int i) { _iday = i; }
	
	int itime() const { return _itime; }
	void setItime(const int i) { _itime = i; }
	
	float t0() const { return _t0; }
	void setT0(const float t) { _t0 = t; }
	
	int bx0() const { return _bx0; }
	void setBx0(const int i) { _bx0 = i; }
	
	int platform() const { return _platform; }
	void setPlatform(const int i) { _platform = i; }
	
	int partition() const { return _partition; }
	void setPartition(const int i) { _partition = i; }
	
	int bdbepoch() const { return _bdbepoch; }
	void setBdbepoch(const int i) { _bdbepoch = i; }
	
	int bdboffset() const { return _bdboffset; }
	void setBdboffset(const int i) { _bdboffset = i; }

	protected: 
	int    _idrun;
	int    _idevt;
	int    _nrndm1;
	int    _nrndm2;
	int    _ntrack;
	int    _nvertx;
	int    _iday;
	int    _itime;
	float  _t0;
	int    _bx0;
	int    _platform;
	int    _partition;
	int    _bdbepoch;
	int    _bdboffset;
};

#endif
