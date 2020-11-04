
#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
#include <iostream>
#else                                                               // BABAR_IOSTREAMS_MIGRATION
#include <iostream.h>
#endif                                                              // BABAR_IOSTREAMS_MIGRATION

//**
// Some classes to put into the dictionary.  We're going to pull them
// out and call their virtual print() methods.  We sandwhich this between
// two other virtuals.  The placement in the vtab of prnt() is differnt
// for each class.  If we ever get one the wrong way we'll call the wrong
// fn and get an error message.
//
//**

class B {
public:
  B( int i ) : _id( i ) {}
  virtual ~B() {;}
#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
  virtual void print(void) const { std::cout << "B[" << _id << "] "; }
#else                                                               // BABAR_IOSTREAMS_MIGRATION
  virtual void print(void) const { cout << "B[" << _id << "] "; }
#endif                                                              // BABAR_IOSTREAMS_MIGRATION
#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
  virtual void err1( void) const { std::cout << "ERROR: B probably as C"; }
#else                                                               // BABAR_IOSTREAMS_MIGRATION
  virtual void err1( void) const { cout << "ERROR: B probably as C"; }
#endif                                                              // BABAR_IOSTREAMS_MIGRATION
#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
  virtual void err2( void) const { std::cout << "ERROR: B probably as D"; }
#else                                                               // BABAR_IOSTREAMS_MIGRATION
  virtual void err2( void) const { cout << "ERROR: B probably as D"; }
#endif                                                              // BABAR_IOSTREAMS_MIGRATION
private:
  int _id;
#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
  friend std::ostream& operator <<( std::ostream& o, const B& a);
#else                                                               // BABAR_IOSTREAMS_MIGRATION
  friend ostream& operator <<( ostream& o, const B& a);
#endif                                                              // BABAR_IOSTREAMS_MIGRATION
};

class C {
public:
  C( int i ) : _id( i ) {}
  virtual ~C() {;}
#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
  virtual void err1( void) const { std::cout << "ERROR: C probably as B"; }
#else                                                               // BABAR_IOSTREAMS_MIGRATION
  virtual void err1( void) const { cout << "ERROR: C probably as B"; }
#endif                                                              // BABAR_IOSTREAMS_MIGRATION
#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
  virtual void print(void) const { std::cout << "C[" << _id << "] "; }
#else                                                               // BABAR_IOSTREAMS_MIGRATION
  virtual void print(void) const { cout << "C[" << _id << "] "; }
#endif                                                              // BABAR_IOSTREAMS_MIGRATION
#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
  virtual void err2( void) const { std::cout << "ERROR: C probably as D"; }
#else                                                               // BABAR_IOSTREAMS_MIGRATION
  virtual void err2( void) const { cout << "ERROR: C probably as D"; }
#endif                                                              // BABAR_IOSTREAMS_MIGRATION
private:
  int _id;
#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
  friend std::ostream& operator <<( std::ostream& o, const C& a);
#else                                                               // BABAR_IOSTREAMS_MIGRATION
  friend ostream& operator <<( ostream& o, const C& a);
#endif                                                              // BABAR_IOSTREAMS_MIGRATION
};

class D {
public:
  D( int i ) : _id( i ) {}
  virtual ~D() {;}
#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
  virtual void err1( void) const { std::cout << "ERROR: D probably as B"; }
#else                                                               // BABAR_IOSTREAMS_MIGRATION
  virtual void err1( void) const { cout << "ERROR: D probably as B"; }
#endif                                                              // BABAR_IOSTREAMS_MIGRATION
#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
  virtual void err2( void) const { std::cout << "ERROR: D probably as C"; }
#else                                                               // BABAR_IOSTREAMS_MIGRATION
  virtual void err2( void) const { cout << "ERROR: D probably as C"; }
#endif                                                              // BABAR_IOSTREAMS_MIGRATION
#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
  virtual void print(void) const { std::cout << "D[" << _id << "] "; }
#else                                                               // BABAR_IOSTREAMS_MIGRATION
  virtual void print(void) const { cout << "D[" << _id << "] "; }
#endif                                                              // BABAR_IOSTREAMS_MIGRATION
private:
  int _id;
#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
  friend std::ostream& operator <<( std::ostream& o, const D& d);
#else                                                               // BABAR_IOSTREAMS_MIGRATION
  friend ostream& operator <<( ostream& o, const D& d);
#endif                                                              // BABAR_IOSTREAMS_MIGRATION
};

//
// some classes to use in the test
//
class boogie{};
class woogie{};

class ListA {
public:
  ListA() : _myID( _instanceScaler++ ) {}
  virtual ~ListA() { --_instanceScaler; }
  static void numInstances(void) {
#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
    std::cout << "Number of ListA objects=" <<  _instanceScaler;
#else                                                               // BABAR_IOSTREAMS_MIGRATION
    cout << "Number of ListA objects=" <<  _instanceScaler;
#endif                                                              // BABAR_IOSTREAMS_MIGRATION
  }
private:
  static int _instanceScaler;
  int _myID;
};


class NoIfd {
public:
  NoIfd( int id ) : _myID( id ) {}
  virtual ~NoIfd() {;}
#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
  virtual void print(void) const { std::cout << "instance " << _myID
#else                                                               // BABAR_IOSTREAMS_MIGRATION
  virtual void print(void) const { cout << "instance " << _myID
#endif                                                              // BABAR_IOSTREAMS_MIGRATION
				        <<" of NoIfd"; }
private:
  int _myID;
#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
  friend std::ostream& operator <<( std::ostream& o, const NoIfd& d);
#else                                                               // BABAR_IOSTREAMS_MIGRATION
  friend ostream& operator <<( ostream& o, const NoIfd& d);
#endif                                                              // BABAR_IOSTREAMS_MIGRATION
};
