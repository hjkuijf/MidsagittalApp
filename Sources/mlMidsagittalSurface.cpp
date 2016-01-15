#include "mlMidsagittalSurface.h"
#include "mlMidsagittalSurfaceOptimizer.h"
#include "mlMidsagittalSurfaceOutputImageHandler.h"

#include "functions.h"

#include "mlLine.h"
#include "mlTools.h"

ML_START_NAMESPACE

ML_MODULE_CLASS_SOURCE( MidsagittalSurface, AlgorithmModule );


//------------------------------------------------------------------------------------
//! Macro used to implement the histogram calculation of a referenceSlice.
//------------------------------------------------------------------------------------
#define _VOLKAU_HISTOGRAM_APPEND_CODE(ODTYPE, INDTYPE, OPNAME) \
{ \
  histogram(TSubImage<ODTYPE>(inputMemoryImage.getImage()), referenceSliceHistogram, (MLldouble)realMin, (MLldouble)realMax, bins, true); \
}

//------------------------------------------------------------------------------------
//! Macro used to implement the histogram calculation of a referenceSlice, using a mask.
//------------------------------------------------------------------------------------
#define _VOLKAU_HISTOGRAM_MASK_APPEND_CODE(ODTYPE, INDTYPE, OPNAME) \
{ \
  numberOfPixels += histogram(TSubImage<ODTYPE>(inputMemoryImage.getImage()), TSubImage<ODTYPE>(inputMemoryMaskImage.getImage()), referenceSliceHistogram, (MLldouble)realMin, (MLldouble)realMax, bins, true); \
}



MidsagittalSurface::MidsagittalSurface() : AlgorithmModule( 2, 1 ) {
  handleNotificationOff();
  addInputFields();
  addOutputFields();
  handleNotificationOn();
}

void MidsagittalSurface::addInputFields() {
  _inPlane1Fld = addPlane( "inPlane1", 1, 0, 0, 0 );
  _inPlane2Fld = addPlane( "inPlane2", 1, 0, 0, 0 );
  _inUseSecondPlaneFld = addBool( "inUseSecondPlane", false );
  _inCMSPxFld = addInt( "inCMSPx", 0 );
  _inBinsFld = addInt( "inBins", 64 );
  _inGridSizeFld = addDouble( "inGridSize", 30 );
  _inMaxSizeFld = addInt( "inMaxSize", 1000 );
  _inDeltaFld = addDouble( "inDelta", 0.00001 );
  _inEpsFld = addDouble( "inEps", 1 );
  static const char* const inOptimizerValues[] = { "bfgs", "cg", "lbfgs" };
  _inOptimizerFld = addEnum( "inOptimizer", inOptimizerValues, 3 );
  _inOptimizerFld->setEnumValue( 2 );
}

void MidsagittalSurface::addOutputFields() {
  _outXMarkerListFld = addBase( "outXMarkerList", nullptr );
  _outSplineFld = addBase( "outSpline", nullptr );
  
  _outSplineFld->setBaseValueAndAddAllowedType(&_alglibSpline2DWrapper);
  _outXMarkerListFld->setBaseValueAndAddAllowedType(&_outXMarkerList);
}

MidsagittalSurface::~MidsagittalSurface() {
}

void MidsagittalSurface::validateInput() {
  // Validate current values of module's input fields (object and parameter fields)
  // to prepare an update.
  //
  // If validation fails you have to throw an exception of type
  // AlgorithmModule::Error( const AlgorithmModule::EStatusCode code, const std::string& message )
  // with meaningful status code and message. Their values will be delegated to module's
  // status field interface.
  // To indicate an error with an input object (e.g. Image, Base)
  // use AlgorithmModule::SC_ERROR_INPUT_OBJECT as code.
  // To indicate an error with an input parameter use AlgorithmModule::SC_ERROR_INPUT_PARAMETER as code.
  //
  // Example for an invalid input parameter:
  // 
  // if ( thresholdFld.getIntValue() < 0 ) {
  //   const std::string msg = "Invalid input threshold value detected. A value less than 0 is not allowed.";
  //   throw AlgorithmModule::Error( AlgorithmModule::SC_ERROR_INPUT_PARAMETER, msg );
  // }
  //
  // Example for an invalid input object:
  // 
  // if ( getUpdatedInputImage( 0 ) == NULL ) {
  //   const std::string msg = "Invalid input image at input0 detected.";
  //   throw AlgorithmModule::Error( AlgorithmModule::SC_ERROR_INPUT_OBJECT, msg );
  // }
  //

  // 2 inputs: image and mask. Mask may be disconnected.
  const PagedImage* img = getUpdatedInputImage(0);
  if (img == nullptr) {
      const std::string msg = "Invalid input image detected.";
      throw AlgorithmModule::Error( AlgorithmModule::SC_ERROR_INPUT_OBJECT, msg );
  }
}

void MidsagittalSurface::update() {
  // Execute the algorithm implementation with validated input values.
  // Update values of module's output (object and parameter fields) afterwards with results.
  // Touch module's output images to inform attached modules and ML-Host.
  //
  // If update fails you have to throw an exception of type
  // AlgorithmModule::Error( const AlgorithmModule::EStatusCode code, const std::string& message )
  // with meaningful status code and message. Their values will be delegated to module's
  // status field interface.
  // To indicate an error during calculation (e.g. incorrect intermediate result, out of memory)
  // use AlgorithmModule::SC_ERROR_INTERNAL as code.
  //
  // Example:
  //
  // if ( goodnessOfFit < 0.9 ) {
  //   const std::string msg = "Fit algorithm failed: Goodness of fit smaller than allowed limit.";
  //   throw AlgorithmModule::Error( AlgorithmModule::SC_ERROR_INTERNAL, msg );
  // }
  //

  const MLuint bins(_inBinsFld->getIntValue());
  // Get input data
  PagedImage* inputPagedImage(getUpdatedInputImage(0));
  PagedImage* inputPagedMaskImage(getUpdatedInputImage(1));

  // Get min/max data
  MLdouble stdMin, stdMax, realMin, realMax;
  Tools::scanInput(this, 0, stdMin, stdMax, realMin, realMax);

  const Plane inputPlane1(_inPlane1Fld->getPlaneValue());
  const Plane inputPlane2(_inPlane2Fld->getPlaneValue());

  const bool useSecondPlane(_inUseSecondPlaneFld->getBoolValue());
    
  // Compute the reference-slice probability
  const std::vector<MLdouble> referenceSliceProbability(getReferenceSliceProbability(inputPagedImage, inputPagedMaskImage, bins, realMin, realMax));
    
  // Build a spline from these four corner-points
  auto spline1(buildSplineFromCornerPoints(inputPagedImage, inputPlane1));
  auto spline2(buildSplineFromCornerPoints(inputPagedImage, inputPlane2));

  // Construct a larger grid from spline c, of size m x n  
  std::unique_ptr<double[]> yCoordinates;
  std::unique_ptr<double[]> zCoordinates;

  const ImageVector inputImageExtent(inputPagedImage->getImageExtent());
  const Vector3 inputVoxelSize(inputPagedImage->getVoxelSize());

  MLuint m, n;
  std::tie(m, yCoordinates) = createGridAxisMm(inputImageExtent.y, inputVoxelSize.y);
  std::tie(n, zCoordinates) = createGridAxisMm(inputImageExtent.z, inputVoxelSize.z);

  const MLuint mn(m*n);  

  // Determine initial halfspace
  const bool initialHalfSpace(inputPlane2.isInHalfSpace(inputPagedImage->mapVoxelToWorld(Vector3(alglib::spline2dcalc(spline1, 0, inputImageExtent.z/2), 0, inputImageExtent.z/2))));
    
  // Create x-coordinates
  std::unique_ptr<double[]> xCoordinates(new double[mn]);
  for (MLuint i = 0; i < m; ++i) {
    for (MLuint j = 0; j < n; ++j) {        
      const MLdouble x(alglib::spline2dcalc(spline1, yCoordinates[i], zCoordinates[j]));
      const MLdouble x2(alglib::spline2dcalc(spline2, yCoordinates[i], zCoordinates[j]));

      const Vector3 world(inputPagedImage->mapVoxelToWorld(Vector3(x, yCoordinates[i], zCoordinates[j])));

      if (useSecondPlane && (inputPlane2.isInHalfSpace(world) != initialHalfSpace)) {
        xCoordinates[j*m + i] = x2;
      } else {
        xCoordinates[j*m + i] = x;
      }
    }
  }

  alglib::real_1d_array yArray; yArray.setcontent(m, yCoordinates.get());
  alglib::real_1d_array zArray; zArray.setcontent(n, zCoordinates.get());
  alglib::real_2d_array xArray; xArray.setcontent(n, m, xCoordinates.get());

  // Copy the xCoordinates into the starting_point
  column_vector solution;
  solution.set_size(mn);
  for (MLuint i(0); i < mn; ++i) {
    solution(i) = xCoordinates[i];
  }

  // Start the optimization
  switch (_inOptimizerFld->getEnumValue()) {
  case 0:
    dlib::find_max_using_approximate_derivatives(
      dlib::bfgs_search_strategy(),
      dlib::objective_delta_stop_strategy(_inDeltaFld->getDoubleValue()).be_verbose(),
      MidsagittalSurfaceOptimizer(inputPagedImage, inputPagedMaskImage, referenceSliceProbability, yArray, zArray, bins, realMin, realMax), 
      solution, 
      100, // Maximum possible KL value
      _inEpsFld->getDoubleValue() // Step-size in derivative (default: 1 pixel)
      );
    break;
  case 1:
    dlib::find_max_using_approximate_derivatives(
      dlib::cg_search_strategy(),
      dlib::objective_delta_stop_strategy(_inDeltaFld->getDoubleValue()).be_verbose(),
      MidsagittalSurfaceOptimizer(inputPagedImage, inputPagedMaskImage, referenceSliceProbability, yArray, zArray, bins, realMin, realMax), 
      solution, 
      100, // Maximum possible KL value
      _inEpsFld->getDoubleValue() // Step-size in derivative (default: 1 pixel)
      );
    break;
  case 2:
    dlib::find_max_using_approximate_derivatives(
      dlib::lbfgs_search_strategy(_inMaxSizeFld->getIntValue()), 
      dlib::objective_delta_stop_strategy(_inDeltaFld->getDoubleValue()).be_verbose(),
      MidsagittalSurfaceOptimizer(inputPagedImage, inputPagedMaskImage, referenceSliceProbability, yArray, zArray, bins, realMin, realMax), 
      solution, 
      100, // Maximum possible KL value
      _inEpsFld->getDoubleValue() // Step-size in derivative (default: 1 pixel)
      );
    break;
  //case 3:
  //  dlib::find_max_using_approximate_derivatives(
  //    dlib::newton_search_strategy(),
  //    dlib::objective_delta_stop_strategy(_deltaFld->getDoubleValue()), //.be_verbose(),
  //    VolkauCurveOptimizer(inputPagedImage, inputPagedMaskImage, referenceSliceProbability, yArray, zArray, bins, realMin, realMax), 
  //    solution, 
  //    100, // Maximum possible KL value
  //    _epsFld->getDoubleValue() // Step-size in derivative (default: 1 pixel)
  //    );
  //  break;
  default:
    mlError("VolkauCurve::handleNotification ()", ML_PROGRAMMING_ERROR) << "No optimizer selected.";
  };
    
  // Create NEW x-coordinates based on the optimized solution
  std::unique_ptr<double[]> xCoordinatesNew(new double[mn]);
  for (MLuint i(0); i < mn; ++i) {
    xCoordinatesNew[i] = solution(i);
  }

  alglib::real_2d_array xArrayNew; xArrayNew.setcontent(n, m, xCoordinatesNew.get());

  // Build the spline
  alglib::spline2dbuildbicubic(yArray, zArray, xArrayNew, n, m, _alglibSpline2DWrapper.spline);
  _alglibSpline2DWrapper.isValid = true;

  // Output control points
  _outXMarkerList.clearList();
  const Matrix4& voxelToWorldMatrix(inputPagedImage->getVoxelToWorldMatrix());
  for (MLuint i = 0; i < m; ++i) {
    for (MLuint j = 0; j < n; ++j) {
      const double xCoordinate(alglib::spline2dcalc(_alglibSpline2DWrapper.spline, yCoordinates[i], zCoordinates[j]));

      Vector3 position(xCoordinate, yCoordinates[i], zCoordinates[j]);
      position = voxelToWorldMatrix.transformPoint(position);

      XMarker marker;
      marker.pos[ML_VX] = position[ML_VX];
      marker.pos[ML_VY] = position[ML_VY];
      marker.pos[ML_VZ] = position[ML_VZ];

      _outXMarkerList.appendItem(marker);
    }
  }
  
  // Touch output images to notify attached modules.
  const MLint numOutputImages = getNumOutputImages();
  for ( MLint idx = 0; idx < numOutputImages; ++idx ) {
    getOutputImageField( idx )->touch();
  }
  _outSplineFld->touch();
  _outXMarkerListFld->touch();
}

void MidsagittalSurface::clear() {
  // Clear values of module's output fields (parameters and objects) and reset with initial values.
  
  // Touch module's output images to inform attached modules and ML-Host.
  const MLint numOutputImages = getNumOutputImages();
  for ( MLint idx = 0; idx < numOutputImages; ++idx ) {
    if ( getOutputImage( idx )->isValid()) {
      clearOutputImage( idx );
      getOutputImageField( idx )->touch();
    }
  }
  
  _alglibSpline2DWrapper.isValid = false;
  _outXMarkerList.clearList();
  _outSplineFld->touch();
  _outXMarkerListFld->touch();
}

void MidsagittalSurface::setOutputImageProperties( int /*outputIndex*/, PagedImage* outputImage ) {
  // If any error occurs you have to throw an exception of type
  // AlgorithmModule::Error( const AlgorithmModule::EStatusCode code, const std::string& message )
  // with meaningful status code and message. Their values will be delegated to module's
  // status field interface.
  // To indicate an error with input images (e.g. failed verification of output image handler,
  // data type, image extent) use AlgorithmModule::SC_ERROR_INPUT_OBJECT as code.
  // To indicate internal error (e.g. out of memory for intermediate volumes, ... )
  // use AlgorithmModule::SC_ERROR_INTERNAL as code.
  //
  // Example:
  //
  // outputImageHandler::verifyProperties( outputImage );
  // if ( !( outputImage->isValid())) {
  //  const std::string msg = "Properties verification of OutputImageHandler failed.";
  //  throw AlgorithmModule::Error( AlgorithmModule::SC_ERROR_INPUT_OBJECT, msg );
  // }
  //
  
  MidsagittalSurfaceOutputImageHandler::setupKnownProperties( outputImage );


  const PagedImage* inputImage(getUpdatedInputImage(0));

  // Image extent
  const ImageVector inputImageExtend(inputImage->getImageExtent());
  outputImage->setImageExtent(inputImageExtend.y, inputImageExtend.z);

  // Voxel size
  const Vector3 inputVoxelSize(inputImage->getVoxelSize());
  outputImage->setVoxelSize(inputVoxelSize.y, inputVoxelSize.z, 1);

  // Valid
  outputImage->setValid(_alglibSpline2DWrapper.isValid);
  

  MidsagittalSurfaceOutputImageHandler::verifyProperties( outputImage );
}


//----------------------------------------------------------------------------------
//! Configures (in)validation handling of inputs which are not connected or invalid.
//----------------------------------------------------------------------------------
Module::INPUT_HANDLE MidsagittalSurface::handleInput(int inputIndex, INPUT_STATE state) const
{
  ML_TRACE_IN("MidsagittalSurface::handleInput ()");

  if (inputIndex == 1 && state == DISCONNECTED) {
    return ALLOW_INVALID_INPUT;
  }

  // State is DISCONNECTED if no connection exists or CONNECTED_BUT_INVALID if a connection
  // exists with an image which cannot be validated.
  // Return INVALIDATE to stop image processing (the default).
  // Return ALLOW_INVALID_INPUT to continue image processing and handle the invalid input image.
  // For details see section "Handling Disconnected and Invalid Inputs by Overloading handleInput()"
  // in ML Programming Guide.
  return INVALIDATE;
}

CalculateOutputImageHandler* MidsagittalSurface::createCalculateOutputImageHandler( PagedImage* ) {
  MidsagittalSurfaceOutputImageHandler::Parameters processingParameters;
  
  processingParameters.spline = _alglibSpline2DWrapper.spline;
  processingParameters.inputBoxFromImageExtent = getUpdatedInputImage(0)->getBoxFromImageExtent();

  return new MidsagittalSurfaceOutputImageHandler( processingParameters );
}



void MidsagittalSurface::getCornerPoints( const PagedImage* image, const Plane &inputPlane, Vector3 &corner1, Vector3 &corner2, Vector3 &corner3, Vector3 &corner4 ) const
{
  const ImageVector inputImageExtent(image->getImageExtent());

  // Compute the intersection between the plane and the sides of the image. This is done by creating a line between two corner-points of the image, and
  // see if the plane intersects this line.
  // Throw an error when the plane is parallel to the sides of the image. This should never happen, though.
  ML_CHECK(inputPlane.intersect(Line(image->mapVoxelToWorld(Vector3(0, 0, 0)), image->mapVoxelToWorld(Vector3(inputImageExtent.x, 0, 0))), corner1));
  ML_CHECK(inputPlane.intersect(Line(image->mapVoxelToWorld(Vector3(0, inputImageExtent.y, 0)), image->mapVoxelToWorld(Vector3(inputImageExtent.x, inputImageExtent.y, 0))), corner2));
  ML_CHECK(inputPlane.intersect(Line(image->mapVoxelToWorld(Vector3(0, 0, inputImageExtent.z)), image->mapVoxelToWorld(Vector3(inputImageExtent.x, 0, inputImageExtent.z))), corner3));
  ML_CHECK(inputPlane.intersect(Line(image->mapVoxelToWorld(Vector3(0, inputImageExtent.y, inputImageExtent.z)), image->mapVoxelToWorld(Vector3(inputImageExtent.x, inputImageExtent.y, inputImageExtent.z))), corner4));
  
  // Transform them to pixel coordinates
  corner1 = image->mapWorldToVoxel(corner1);
  corner2 = image->mapWorldToVoxel(corner2);
  corner3 = image->mapWorldToVoxel(corner3);
  corner4 = image->mapWorldToVoxel(corner4);
}

alglib::spline2dinterpolant MidsagittalSurface::buildSplineFromCornerPoints( const PagedImage* inputImage, const Plane &plane ) const
{
  Vector3 corner1, corner2, corner3, corner4;
  getCornerPoints(inputImage, plane, corner1, corner2, corner3, corner4);

  const ImageVector inputImageExtent = inputImage->getImageExtent();

  // Create a 2D grid with the four corner-points
  const double yCoordinates[] = {0, inputImageExtent.y};
  const double zCoordinates[] = {0, inputImageExtent.z};
  const double xCoordinates[] = {corner1.x, corner2.x, corner3.x, corner4.x};

  alglib::real_1d_array y; y.setcontent(2, yCoordinates);
  alglib::real_1d_array z; z.setcontent(2, zCoordinates);
  alglib::real_2d_array x; x.setcontent(2, 2, xCoordinates);    

  // This builds a 2D spline with the yz-plane as a basis and the x-values as height (values to be interpolated)
  alglib::spline2dinterpolant c;
  alglib::spline2dbuildbilinear(y, z, x, 2, 2, c);

  return c;
}

void MidsagittalSurface::createGridAxis( const MLuint numberOfPoints, double* axis, const MLint size ) const
{
  // Create a range from [0 .. size] (so including 0 and size), with a total of numberOfPoints points
  for (MLuint i(0); i < numberOfPoints; ++i) { axis[i] = MLdouble(i * size) / MLdouble(numberOfPoints - 1); }
}

const std::vector<MLdouble> MidsagittalSurface::getReferenceSliceProbability( PagedImage* inputPagedImage, PagedImage* inputPagedMaskImage, const MLuint bins, const MLldouble realMin, const MLldouble realMax ) const
{
  const ImageVector inputImageExtent(inputPagedImage->getImageExtent());
  const MLDataType inputDataType(inputPagedImage->getDataType());

  const Vector3 inputVoxelSize(inputPagedImage->getVoxelSize());
  const MLuint cMSPx(_inCMSPxFld->getIntValue());
  MLuint numberOfPixels(0);
  const SubImageBox inputBoxFromImageExtent(inputPagedImage->getBoxFromImageExtent());
  // Define the reference slice at the distance of 2cm from the cMSP slice in the
  // sagittal direction. This reference slice remains the same for the whole
  // second stage.
  const SubImageBox newReferenceSlice1Box(ImageVector(cMSPx - 20 / inputVoxelSize[0], 0, 0, 0, 0, 0), ImageVector(cMSPx - 20 / inputVoxelSize[0], inputBoxFromImageExtent.v2.y, inputBoxFromImageExtent.v2.z, 0, 0, 0));
  const SubImageBox newReferenceSlice2Box(ImageVector(cMSPx + 20 / inputVoxelSize[0], 0, 0, 0, 0, 0), ImageVector(cMSPx + 20 / inputVoxelSize[0], inputBoxFromImageExtent.v2.y, inputBoxFromImageExtent.v2.z, 0, 0, 0));

  std::vector<MLuint> referenceSliceHistogram(bins, 0);
  referenceSliceHistogram.clear();
  referenceSliceHistogram.resize(bins, 0);

  MemoryImage& inputMemoryImage(inputPagedImage->getMemoryImage());
  if (inputPagedMaskImage == NULL) {
    numberOfPixels = inputImageExtent.y * inputImageExtent.z * 2;
    { // Reference Slice 1
      inputMemoryImage.update(inputPagedImage, newReferenceSlice1Box, inputDataType);
      ML_IMPLEMENT_SCALAR_TYPE_CASES("", _VOLKAU_HISTOGRAM_APPEND_CODE, inputDataType, "Volkau::handleNotification ()", "");
    }

    { // Reference Slice 2
      inputMemoryImage.update(inputPagedImage, newReferenceSlice2Box, inputDataType);
      ML_IMPLEMENT_SCALAR_TYPE_CASES("", _VOLKAU_HISTOGRAM_APPEND_CODE, inputDataType, "Volkau::handleNotification ()", "");
    }
  } else {
    // numberOfPixels is update in _VOLKAU_HISTOGRAM_MASK_APPEND_CODE
    MemoryImage& inputMemoryMaskImage(inputPagedMaskImage->getMemoryImage());
    { // Reference Slice 1
      inputMemoryImage.update(inputPagedImage, newReferenceSlice1Box, inputDataType);
      inputMemoryMaskImage.update(inputPagedMaskImage, newReferenceSlice1Box, inputDataType);
      ML_IMPLEMENT_SCALAR_TYPE_CASES("", _VOLKAU_HISTOGRAM_MASK_APPEND_CODE, inputDataType, "Volkau::handleNotification ()", "");
    }

    { // Reference Slice 2
      inputMemoryImage.update(inputPagedImage, newReferenceSlice2Box, inputDataType);
      inputMemoryMaskImage.update(inputPagedMaskImage, newReferenceSlice1Box, inputDataType);
      ML_IMPLEMENT_SCALAR_TYPE_CASES("", _VOLKAU_HISTOGRAM_MASK_APPEND_CODE, inputDataType, "Volkau::handleNotification ()", "");
    }    
  }

  std::vector<MLdouble> referenceSliceProbability;
  referenceSliceProbability.clear();

  normalize(referenceSliceHistogram, referenceSliceProbability, numberOfPixels);

  return referenceSliceProbability;
}

std::tuple<MLuint, std::unique_ptr<double[]> > MidsagittalSurface::createGridAxisMm( const MLuint& imageExtent, const MLdouble& voxelSize ) const
{
	// Compute grid in mm
	const MLdouble gridSize(_inGridSizeFld->getDoubleValue());
	const MLdouble numGridPointsD = (voxelSize * imageExtent) / gridSize + 1;// Include the last control point! 9 grids = 10 points
	const MLuint   numGridPointsI = MLuint(numGridPointsD);

	const MLdouble residue = numGridPointsD - numGridPointsI;
	
  std::unique_ptr<double[]> axis(new double[numGridPointsI]);
	
	for (MLuint i(0); i < numGridPointsI; ++i) { 
		axis[i] = (gridSize * (i + residue/2)) / voxelSize;
	}

	return std::make_tuple(numGridPointsI, std::move(axis));
}

ML_END_NAMESPACE