#include "mlMidsagittalPlaneSplitOutputImageHandler.h"

ML_START_NAMESPACE

MidsagittalPlaneSplitOutputImageHandler::Parameters::Parameters() {

  this->inIntersectionMode = 0;
  this->inVolumeThreshold = 0;
  this->inUseGlobalSubsample = true;
  this->inGlobalSubsample = 0;
  this->inXSubsample = 0;
  this->inYSubsample = 0;
  this->inZSubsample = 0;
  this->outputIndex = 0;
  this->plane = Plane();
  this->inVoxelToWorldMatrix = Matrix4();
}

MidsagittalPlaneSplitOutputImageHandler::MidsagittalPlaneSplitOutputImageHandler( const Parameters& parameters )
: _parameters( parameters )
{
}

SubImageBox MidsagittalPlaneSplitOutputImageHandler::calculateInputSubImageBox( int /*inputIndex*/, const SubImageBox& outputSubImageBox )
{
  // Return region of input image inputIndex needed to compute region outputSubImageBox.
  return outputSubImageBox;
}


template <typename OUTTYPE>
void MidsagittalPlaneSplitOutputImageHandler::typedCalculateOutputSubImage(TSubImage<OUTTYPE>& outputSubImage,
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
              // For the anti-aliasing computation, it only makes sense if the current voxel is close to the plane. If the
              // distance is too large, the plane does not intersect the voxel. Then, no computation is needed anyway and
              // we fall back to midpoint (default).
              const Vector3 voxelCenter(_parameters.inVoxelToWorldMatrix.transformPoint(Vector3(MLdouble(p.x) + 0.5, MLdouble(p.y) + 0.5, MLdouble(p.z) + 0.5)));

              MLint currentIntersectionMode = _parameters.inIntersectionMode;
              if (_parameters.inIntersectionMode == 1 || _parameters.inIntersectionMode == 2) { // Volume or midpoint
                const MLdouble distance = mlAbs(_parameters.plane.getDistance(voxelCenter));

                // Check if the plane intersects the current voxel. If the distance from the voxelCenter to the plane is
                // smaller than the distance from voxelOrigin to voxelCenter, the plane is in the voxel.
                const Vector3 voxelOrigin(_parameters.inVoxelToWorldMatrix.transformPoint(Vector3(MLdouble(p.x), MLdouble(p.y), MLdouble(p.z))));
                if (distance > voxelOrigin.distance(voxelCenter)) { 
                  // No intersection, fall back to midpoint
                  currentIntersectionMode = 0;
                }
              }

              switch (currentIntersectionMode) {
              case 0: // Midpoint
                if (_parameters.plane.isInHalfSpace(voxelCenter)) {
                  *outVoxel = (_parameters.outputIndex == 0) ? *inVoxel0 : OUTTYPE(0);
                } else {
                  *outVoxel = (_parameters.outputIndex == 0) ? OUTTYPE(0) : *inVoxel0;
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
                        const MLdouble newX = MLdouble(p.x) + MLdouble(x) / nx + xStep;
                        const MLdouble newY = MLdouble(p.y) + MLdouble(y) / ny + yStep;
                        const MLdouble newZ = MLdouble(p.z) + MLdouble(z) / nz + zStep;

                        if (_parameters.plane.isInHalfSpace(_parameters.inVoxelToWorldMatrix.transformPoint(Vector3(newX, newY, newZ)))) {
                          count += (_parameters.outputIndex == 0) ? 1 : 0;
                        } else {
                          count += (_parameters.outputIndex == 0) ? 0 : 1;
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
                mlFatalError("MidsagittalPlaneSplitOutputImageHandler::typedCalculateOutputSubImage", ML_PROGRAMMING_ERROR);
              }
            }
          }
        }
      }
    }
  }
}

ML_END_NAMESPACE