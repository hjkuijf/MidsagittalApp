#pragma once

#include "librarySystem.h"
#include <mlAlgorithmModule.h>

//#include "mlXMarkerList.h"

// ALGLIB includes
#include "interpolation.h"

// dlib includes
#include "dlib/optimization.h"

ML_START_NAMESPACE
  
typedef dlib::matrix<MLdouble, 0, 1> column_vector;

class MidsagittalSurfaceOptimizer
{
public:
  //! Function object VolkauCurveOptimizer, which is used to compute the KL-measure of a
  //! solution during optimization of the curve.
  /*!
    \param PagedImage * inputPagedImage 
    \param const std::vector<MLdouble> & referenceSliceProbability 
    \param const alglib::real_1d_array & yArray 
    \param const alglib::real_1d_array & zArray 
    \param MLuint bins 
    \param MLldouble realMin 
    \param MLldouble realMax
  */
  MidsagittalSurfaceOptimizer(PagedImage* inputPagedImage, PagedImage* inputPagedMaskImage, const std::vector<MLdouble>& referenceSliceProbability, const alglib::real_1d_array& yArray, const alglib::real_1d_array& zArray, MLuint bins, MLldouble realMin, MLldouble realMax);

        
  //! Actual computation of the KL-measure given the current best solution
  /*!
    \param const column_vector & v Current solution
    \return double KL-measure
  */
  double operator () (const column_vector& v) const;
  
private:
  const MLDataType _inputDataType;
  const ImageVector _inputImageExtent;
  MemoryImage& _inputMemoryImage;
  MemoryImage& _inputMemoryMaskImage;
  bool _hasMask;
  const std::vector<MLdouble>& _referenceSliceProbability;
  const alglib::real_1d_array& _yArray;
  const alglib::real_1d_array& _zArray;
  const MLuint _bins;
  const MLldouble _realMin, _realMax;
};

ML_END_NAMESPACE