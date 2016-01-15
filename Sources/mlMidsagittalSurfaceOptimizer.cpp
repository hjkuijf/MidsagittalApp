// Local includes
#include "mlMidsagittalSurfaceOptimizer.h"

#include "functions.h"

// STL includes
#include <vector>

ML_START_NAMESPACE

MidsagittalSurfaceOptimizer::MidsagittalSurfaceOptimizer( PagedImage* inputPagedImage, PagedImage* inputPagedMaskImage, const std::vector<MLdouble>& referenceSliceProbability, const alglib::real_1d_array& yArray, const alglib::real_1d_array& zArray, MLuint bins, MLldouble realMin, MLldouble realMax ) : 
  _inputDataType(inputPagedImage->getDataType()), 
  _inputImageExtent(inputPagedImage->getImageExtent()), 
  _inputMemoryImage(inputPagedImage->getMemoryImage()), 
  _hasMask(inputPagedMaskImage != NULL),
  _inputMemoryMaskImage(_hasMask ? inputPagedMaskImage->getMemoryImage() : inputPagedImage->getMemoryImage()), //Warning: null reference, DO NEVER USE !!!
  _referenceSliceProbability(referenceSliceProbability),
  _yArray(yArray), 
  _zArray(zArray),
  _bins(bins),
  _realMin(realMin),
  _realMax(realMax)
{
  _inputMemoryImage.update(inputPagedImage, inputPagedImage->getBoxFromImageExtent(), _inputDataType);
  
  if (_hasMask) {
    _inputMemoryMaskImage.update(inputPagedMaskImage, inputPagedMaskImage->getBoxFromImageExtent(), _inputDataType);
  }
}

double MidsagittalSurfaceOptimizer::operator()( const column_vector& v ) const
{
  ML_TRACE_IN("MidsagittalSurfaceOptimizer::operator() ()");

  const long vSize(v.size());
  const MLuint m(_yArray.length());
  const MLuint n(_zArray.length());

  // Create x-coordinates
  std::unique_ptr<double[]> xCoordinates(new double[vSize]);

  // Copy column_vector v to xCoordinates
  for (MLuint i(0); i < vSize; ++i) {
    xCoordinates[i] = v(i);
  }

  alglib::real_2d_array xArray; xArray.setcontent(n, m, xCoordinates.get());

  // Build the spline
  alglib::spline2dinterpolant spline;
  alglib::spline2dbuildbicubic(_yArray, _zArray, xArray, n, m, spline);

  // Computer the histogram for the current spline.
  std::vector<MLuint> sliceHistogram(_bins, 0);
  MLuint numberOfPixels(0);

#define _COMPUTE_SPLINE_HISTOGRAM_CODE(ODTYPE, INDTYPE, OPNAME) \
  { \
    if (_hasMask) { \
      TSubImage<ODTYPE> inputTSubImage(_inputMemoryImage.getImage()); \
      TSubImage<ODTYPE> inputTSubMaskImage(_inputMemoryMaskImage.getImage()); \
      for (MLint y(0); y < _inputImageExtent.y; ++y) { \
        for (MLint z(0); z < _inputImageExtent.z; ++z) { \
          const MLdouble imageX(alglib::spline2dcalc(spline, y, z)); \
          if (imageX < 0 || imageX > _inputImageExtent.x) return 0; \
          const MLdouble imageY(y); \
          const MLdouble imageZ(z); \
          \
          if (inputTSubMaskImage.getImageValue(floor(imageX), imageY, imageZ) != 0) { \
            const MLdouble pixelValue = \
            (1 - (imageX - floor(imageX)))     * inputTSubImage.getImageValue(floor(imageX),     imageY, imageZ) + \
            (1 - (floor(imageX + 1) - imageX)) * inputTSubImage.getImageValue(floor(imageX + 1), imageY, imageZ); \
            \
            sliceHistogram[(pixelValue - _realMin) * _bins / (_realMax - _realMin + ML_FLOAT_EPSILON)]++; \
            ++numberOfPixels; \
          } \
        } \
      } \
    } else { \
      numberOfPixels = _inputImageExtent.y * _inputImageExtent.z; \
      TSubImage<ODTYPE> inputTSubImage(_inputMemoryImage.getImage()); \
      for (MLint y(0); y < _inputImageExtent.y; ++y) { \
        for (MLint z(0); z < _inputImageExtent.z; ++z) { \
          const MLdouble imageX(alglib::spline2dcalc(spline, y, z)); \
          if (imageX < 0 || imageX > _inputImageExtent.x) return 0; \
          const MLdouble imageY(y); \
          const MLdouble imageZ(z); \
          \
          const MLdouble pixelValue = \
            (1 - (imageX - floor(imageX)))     * inputTSubImage.getImageValue(floor(imageX),     imageY, imageZ) + \
            (1 - (floor(imageX + 1) - imageX)) * inputTSubImage.getImageValue(floor(imageX + 1), imageY, imageZ); \
          \
          sliceHistogram[(pixelValue - _realMin) * _bins / (_realMax - _realMin + ML_FLOAT_EPSILON)]++; \
        } \
      } \
    } \
  }

  ML_IMPLEMENT_SCALAR_TYPE_CASES("", _COMPUTE_SPLINE_HISTOGRAM_CODE, _inputDataType, "VolkauCurveOptimizer::operator() ()", "");

#undef _COMPUTE_SPLINE_HISTOGRAM_CODE

  // Convert to probability
  std::vector<MLdouble> sliceProbability(_bins, 0);
  normalize(sliceHistogram, sliceProbability, numberOfPixels);

  // Compute KL measure
  const MLdouble kl(kullbackLeibler(sliceProbability, _referenceSliceProbability));
  
  return kl;
}

ML_END_NAMESPACE
