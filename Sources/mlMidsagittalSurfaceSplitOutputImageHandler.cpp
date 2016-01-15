#include "mlMidsagittalSurfaceSplitOutputImageHandler.h"

ML_START_NAMESPACE

MidsagittalSurfaceSplitOutputImageHandler::Parameters::Parameters() {

  this->inIntersectionMode = 0;
  this->inVolumeThreshold = 0;
  this->inUseGlobalSubsample = true;
  this->inGlobalSubsample = 0;
  this->inXSubsample = 0;
  this->inYSubsample = 0;
  this->inZSubsample = 0;
  this->outputIndex = 0;
}

MidsagittalSurfaceSplitOutputImageHandler::MidsagittalSurfaceSplitOutputImageHandler( const Parameters& parameters )
: _parameters( parameters )
{
}

SubImageBox MidsagittalSurfaceSplitOutputImageHandler::calculateInputSubImageBox( int /*inputIndex*/, const SubImageBox& outputSubImageBox )
{
  // Return region of input image inputIndex needed to compute region outputSubImageBox.
  return outputSubImageBox;
}


template <typename OUTTYPE>
void MidsagittalSurfaceSplitOutputImageHandler::typedCalculateOutputSubImage(TSubImage<OUTTYPE>& outputSubImage,
    const TSubImage<OUTTYPE>& inputSubImage0,
    UserThreadData* /*userThreadData*/)
{
  // Clamp box of output image against image extent to avoid that unused areas are processed.
  const SubImageBox box = outputSubImage.getValidRegion();
  
  // Const variables used for volume or anti-aliasing.
  const MLuint nx(_parameters.inUseGlobalSubsample ? _parameters.inGlobalSubsample : _parameters.inXSubsample);
  const MLuint ny(_parameters.inUseGlobalSubsample ? _parameters.inGlobalSubsample : _parameters.inYSubsample);
  const MLuint nz(_parameters.inUseGlobalSubsample ? _parameters.inGlobalSubsample : _parameters.inZSubsample);
  const MLdouble subsampleSize(MLdouble(nx*ny*nz));
  const MLdouble xStep(1. / (2. * nx));
  const MLdouble yStep(1. / (2. * ny));
  const MLdouble zStep(1. / (2. * nz));

  // Process all voxels of the valid region of the output page.
  ImageVector p;
  for ( p.u = box.v1.u;  p.u <= box.v2.u;  ++p.u ) {
    for ( p.t = box.v1.t;  p.t <= box.v2.t;  ++p.t ) {
      for ( p.c = box.v1.c;  p.c <= box.v2.c;  ++p.c ) {
        for ( p.z = box.v1.z;  p.z <= box.v2.z;  ++p.z ) {
          for ( p.y = box.v1.y;  p.y <= box.v2.y;  ++p.y ) {
            p.x = box.v1.x;
            // Get pointers to row starts of input and output sub-images.
            const OUTTYPE* inVoxel0 = inputSubImage0.getImagePointer(p);
            OUTTYPE* outVoxel = outputSubImage.getImagePointer(p);

            const MLint rowEnd = box.v2.x;

            // Process all row voxels.
            for ( ; p.x <= rowEnd;  ++p.x, ++outVoxel, ++inVoxel0 ) {
              const MLdouble imageX(alglib::spline2dcalc(_parameters.spline, double(p.y), double(p.z)) - .5); // Correct with .5 for subvoxel shift 

              // For the anti-aliasing computation, it only makes sense if the current voxel is close to the spline. If the
              // distance is too large, the plane does not intersect the voxel. Then, no computation is needed anyway and
              // we fall back to midpoint (default).                
              MLint currentIntersectionMode = _parameters.inIntersectionMode;
              if (_parameters.inIntersectionMode == 1 || _parameters.inIntersectionMode == 2) { // Volume or midpoint
                if (mlAbs(p.x - imageX) > 5) { 
                  // No intersection possible, fall back to midpoint
                  currentIntersectionMode = 0;
                }
              }

              switch (currentIntersectionMode) {
              case 0: // Midpoint
                if (imageX < p.x) {
                  *outVoxel = _parameters.outputIndex == 0 ? OUTTYPE(0) : *inVoxel0;
                } else {
                  *outVoxel = _parameters.outputIndex == 0 ? *inVoxel0 : OUTTYPE(0);
                }
                break;
              case 1:
              case 2: 
                {
                  // Divide the voxel in n x n x n boxes and test each component individually against the plane.
                  MLuint count(0);

                  for (MLuint z(0); z < nz; ++z) {
                    for (MLuint y(0); y < ny; ++y) {
                      for (MLuint x(0); x < nx; ++x) {
                        const MLdouble newX = MLdouble(p.x) + MLdouble(x) / ny + xStep;
                        const MLdouble newY = MLdouble(p.y) + MLdouble(y) / ny + yStep;
                        const MLdouble newZ = MLdouble(p.z) + MLdouble(z) / nz + zStep;

                        const MLdouble imageXX(alglib::spline2dcalc(_parameters.spline, newY, newZ));

                        if (imageXX < newX) {
                          count += (_parameters.outputIndex == 0) ? 0 : 1;
                        } else {
                          count += (_parameters.outputIndex == 0) ? 1 : 0;
                        }
                      }
                    }
                  }

                  const MLdouble percentage(MLdouble(count) / subsampleSize); // [0..1]

                  if (currentIntersectionMode == 1) { // Volume
                    *outVoxel = (percentage >= _parameters.inVolumeThreshold) ? *inVoxel0 : 0;
                  }
                  if (currentIntersectionMode == 2) { // AntiAliasing
                    *outVoxel = OUTTYPE(*inVoxel0 * percentage);
                  }
                }
                break;
              default:
                // This should never occur
                mlFatalError("MidsagittalSurfaceSplitOutputImageHandler::typedCalculateOutputSubImage", ML_PROGRAMMING_ERROR);
              }
            }
          }
        }
      }
    }
  }
}

ML_END_NAMESPACE