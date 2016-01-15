#include "mlMidsagittalPlaneOutputImageHandler.h"

ML_START_NAMESPACE

MidsagittalPlaneOutputImageHandler::Parameters::Parameters() {

  this->plane = Plane();
}

MidsagittalPlaneOutputImageHandler::MidsagittalPlaneOutputImageHandler( const Parameters& parameters )
: _parameters( parameters )
{
}

SubImageBox MidsagittalPlaneOutputImageHandler::calculateInputSubImageBox( int /*inputIndex*/, const SubImageBox& /*outputSubImageBox*/ )
{
  // Return region of input image inputIndex needed to compute region outputSubImageBox.
  return _parameters.inputBoxFromImageExtent;
}


template <typename OUTTYPE>
void MidsagittalPlaneOutputImageHandler::typedCalculateOutputSubImage(TSubImage<OUTTYPE>& outputSubImage,
    const TSubImage<OUTTYPE>& inputSubImage0,
    UserThreadData* /*userThreadData*/)
{
  // Clamp box of output image against image extent to avoid that unused areas are processed.
  const SubImageBox box = outputSubImage.getValidRegion();
  
  const Vector3 normal(_parameters.plane.getNormal());
  const MLdouble distanceFromOrigin(_parameters.plane.getDistanceFromOrigin());

  // Process all voxels of the valid region of the output page.
  ImageVector p;
  for ( p.y = box.v1.y;  p.y <= box.v2.y;  ++p.y ) {
    p.x = box.v1.x;
    // Get pointers to row starts of input and output sub-images.
    OUTTYPE* outVoxel = outputSubImage.getImagePointer(p);

    const MLint rowEnd = box.v2.x;

    // Process all row voxels.
    for ( ; p.x <= rowEnd;  ++p.x, ++outVoxel ) {
        const MLdouble imageX( (normal.y * p.x + normal.z * p.y - distanceFromOrigin) / -normal.x );
        const MLdouble imageY(p.x); 
        const MLdouble imageZ(p.y);

        const MLdouble pixelValue = 
          (1 - (imageX - floor(imageX)))     * inputSubImage0.getImageValue(floor(imageX),     imageY, imageZ) + 
          (1 - (floor(imageX + 1) - imageX)) * inputSubImage0.getImageValue(floor(imageX + 1), imageY, imageZ);

        *outVoxel = (OUTTYPE)pixelValue;
    }
  }
}

ML_END_NAMESPACE