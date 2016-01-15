#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

// Local includes
#include "librarySystem.h"

#include <vector>

// ML includes
#include <mlAlgorithmModule.h>

//! Generate a histogram
template<class DATATYPE>
void histogram(const ml::TSubImage<DATATYPE>& image, std::vector<MLuint>& histogram, const MLldouble& min, const MLldouble& max, const MLuint& bins, const bool& append = false)
{
  if (!append) {
    histogram.clear();
    histogram.resize(bins, 0);
  }

  const ml::SubImageBox box(image.getBoxFromImageExtent());

  ml::ImageVector p = box.v1;
  for (p.z = box.v1.z; p.z <= box.v2.z; ++p.z) {
    p.y = box.v1.y;
    // Get pointer to the row start of the output subimage.
    const DATATYPE *inVoxel = image.getSubImagePointer(p);
    const MLint rowEnd  = box.v2.y;

    for ( ; p.y <= rowEnd; ++p.y, ++inVoxel) {
      histogram[MLuint(MLdouble(*inVoxel - min) * bins / MLdouble(max - min + ML_FLOAT_EPSILON))]++;
    }
  }
}

//! Generate a histogram
template<class DATATYPE>
MLuint histogram(const ml::TSubImage<DATATYPE>& image, const ml::TSubImage<DATATYPE>& mask, std::vector<MLuint>& histogram, const MLldouble& min, const MLldouble& max, const MLuint& bins, const bool& append = false)
{
  if (!append) {
    histogram.clear();
    histogram.resize(bins, 0);
  }

  const ml::SubImageBox box(image.getBoxFromImageExtent());
  MLuint numberOfPixels(0);

  ml::ImageVector p = box.v1;
  for (p.z = box.v1.z; p.z <= box.v2.z; ++p.z) {
    p.y = box.v1.y;
    // Get pointer to the row start of the output subimage.
    const DATATYPE *inVoxel = image.getSubImagePointer(p);
    const DATATYPE *maskVoxel = mask.getSubImagePointer(p);
    const MLint rowEnd  = box.v2.y;

    for ( ; p.y <= rowEnd; ++p.y, ++inVoxel, ++maskVoxel ) {
      if (*maskVoxel != 0) {
        histogram[MLuint(MLldouble(*inVoxel - min) * bins / MLldouble(max - min + ML_FLOAT_EPSILON))]++;
        ++numberOfPixels;
      }
    }
  }

  return numberOfPixels;
}

//! Normalize a histogram to become a probability distribution
void normalize(const std::vector<MLuint>& histogram, std::vector<MLdouble>& probability, const MLuint& numberOfPixels);

//! Compute the Kullback-Leibler distance between two probability distributions
MLdouble kullbackLeibler(const std::vector<MLdouble> &p, const std::vector<MLdouble> &q);

//! Transform a plane with the matrix
ml::Plane transformPlane(const ml::Plane& inputPlane, const ml::Matrix4& matrix);

#endif