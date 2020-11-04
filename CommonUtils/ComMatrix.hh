//--------------------------------------------------------------------------
//
// Description:
// 	class ComMatrix:
//
//     A pointer based class for Matrices of objects
//     usage:
//           ComMatrix<myClass> matrix(3,3);     
// 
//            myClass* myclass11= new Myclass();
//
//            matrix[1][1]=myClass  //lvalue
//            myClass* another=matrix[1][1] //rvalue
//            ...
//            matrix.clearAndDestroy();
//          as usual [] indexing starts from 0 to n-1
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	S.Plaszczynski
//
// History (add to end):
//	S.Plaszczynski  Fri May 21 16:25:14 MET DST 1999 (creation)
//
//------------------------------------------------------------------------

#ifndef ComMatrix_hh
#define ComMatrix_hh

#include <vector>

//forward declarations

template <class T>
class ComMatrix
{
public:

  //constructors
  ComMatrix(int nrow=0,int ncol=0 );

  //copy ctor (warning: shallow copy)
  ComMatrix(const ComMatrix<T> &);
  

  // destructor
  ~ComMatrix();

  //member functions
  inline std::vector<T*>& operator[](size_t i) {return *_line[i]; };
  
  
  //clear and delete all T elements of the matrix
  void clearAndDestroy();
  
  
private:
  std::vector< std::vector<T*>* > _line;
  
};
#endif
