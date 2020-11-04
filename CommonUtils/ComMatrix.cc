//--------------------------------------------------------------------------
//
// Description:
// 	class ComMatrix : see header file for description.
//
//------------------------------------------------------------------------

#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "CommonUtils/ComMatrix.hh"
//-------------
// C Headers --
//-------------
#include <assert.h>
//---------------
// C++ Headers --
//---------------
#include <math.h>
#include <vector>
#include <algorithm>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "ErrLogger/ErrLog.hh"
#include "BbrStdUtils/BbrStdDeleteObject.hh"
//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//----------------
// Constructors --
//----------------
template <class T>
ComMatrix<T>::ComMatrix(int nrow,int ncol) : _line()
{

  std::vector<T*> column;
  for (int j=0;j<ncol;j++) column.push_back( (T*)NULL);
  for (int i=0;i<nrow;i++) 
    _line.push_back(new std::vector<T*> (column));
}
//
template <class T>
ComMatrix<T>::ComMatrix(const ComMatrix<T> &o) : _line(o._line)
{
}
//--------------
// Destructor --
//--------------
template <class T>
ComMatrix<T>::~ComMatrix()
{
  std::for_each(_line.begin(), _line.end(), BbrStdDeleteObject());
  _line.clear();  
}
//-----------------
// Member functions --
//-----------------

template <class T> 
void ComMatrix<T>::clearAndDestroy()
{
  typename std::vector< std::vector<T*>* >::iterator iter;
  for (iter = _line.begin(); iter != _line.end(); iter++) {
    std::for_each((*iter)->begin(), (*iter)->end(), BbrStdDeleteObject());
    (*iter)->clear();
  }
}



