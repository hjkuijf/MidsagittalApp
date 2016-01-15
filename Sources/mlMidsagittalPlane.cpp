#include "mlMidsagittalPlane.h"
#include "mlMidsagittalPlaneOutputImageHandler.h"

#include "functions.h"

#include "mlTools.h"

ML_START_NAMESPACE

ML_MODULE_CLASS_SOURCE( MidsagittalPlane, AlgorithmModule );

//------------------------------------------------------------------------------------
//! Macro used to implement the histogram calculation of a slice.
//------------------------------------------------------------------------------------
#define _VOLKAU_HISTOGRAM_CODE(ODTYPE, INDTYPE, OPNAME) \
{ \
  histogram(TSubImage<ODTYPE>(inMemoryImg.getImage()), sliceHistogram, (MLldouble)realMin, (MLldouble)realMax, bins); \
}

//------------------------------------------------------------------------------------
//! Macro used to implement the histogram calculation of a referenceSlice.
//------------------------------------------------------------------------------------
#define _VOLKAU_HISTOGRAM_APPEND_CODE(ODTYPE, INDTYPE, OPNAME) \
{ \
  histogram(TSubImage<ODTYPE>(inMemoryImg.getImage()), referenceSliceHistogram, (MLldouble)realMin, (MLldouble)realMax, bins, true); \
}



MidsagittalPlane::MidsagittalPlane() : AlgorithmModule( 1, 1 ) {
  handleNotificationOff();
  addInputFields();
  addOutputFields();
  handleNotificationOn();
}

void MidsagittalPlane::addInputFields() {
  _inBinsFld = addInt( "inBins", 64 );
}

void MidsagittalPlane::addOutputFields() {
  _outPlaneFld = addPlane( "outPlane", Plane() );
  _outCMSPxFld = addInt( "outCMSPx", 0 );
  _outLogFld = addString( "outLog", "" );
}

MidsagittalPlane::~MidsagittalPlane() {
}

void MidsagittalPlane::validateInput() {
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
  const MLint numInputImages = getNumInputImages();
  for ( MLint idx = 0; idx < numInputImages; ++idx ) {
    const PagedImage* img = getUpdatedInputImage( idx );
    if ( img == nullptr ) {
      const std::string msg = "At least one invalid input image detected.";
      throw AlgorithmModule::Error( AlgorithmModule::SC_ERROR_INPUT_OBJECT, msg );
    }
  }
}

void MidsagittalPlane::update() {
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

  

  PagedImage* inPagedImg = getUpdatedInputImage(0);
	//PagedImage* inPagedMask = getUpdatedInputImage(1);

  if (NULL == inPagedImg) {
    ML_PRINT_ERROR("Volkau::handleNotification ()", "No image input", "Aborting plane calculation");
    _outPlaneFld->setVector4Value(Vector4(0, 0, 0, 0));
  } else {

	  // Check if the mask is valid.

    // This function implements the method of Volkau et al.
    const MLuint bins(_inBinsFld->getIntValue());

	  std::stringstream log;

    /* FIRST STAGE */
    // Read the brain volume data
    const SubImageBox inBoxFromImgExt = inPagedImg->getBoxFromImageExtent();
    MemoryImage& inMemoryImg = inPagedImg->getMemoryImage();
    const MLDataType inDataType = inPagedImg->getDataType();
    MLdouble stdMin, stdMax, realMin, realMax;
    Tools::scanInput(this, 0, stdMin, stdMax, realMin, realMax);

    // Read the voxel-sizes (in mm)
    const Vector3 inVoxelSize = inPagedImg->getVoxelSize();

    // Generate sagittal slices
    //  This is not needed, as we can easily adjust the iterators of the PixelStorage

    // Identify the central slice in the volume along the sagittal direction
    //  That is: the middle slice along the x-axis
    const MLint centralSlice = inPagedImg->getImageExtent().x / 2;

    // Define a volume of interest (inMemoryImg) in the range of ± 2cm around the central
    // slice in the sagittal direction
    const MLint firstSliceOfVOI( centralSlice - 20 / inVoxelSize[0] );
    const MLint lastSliceOfVoi( centralSlice + 20 / inVoxelSize[0] );
    const SubImageBox voiBox(ImageVector(firstSliceOfVOI, 0, 0, 0, 0, 0), ImageVector(lastSliceOfVoi, inBoxFromImgExt.v2.y, inBoxFromImgExt.v2.z, 0, 0, 0));

    // Choose the first (and last) slice of the inMemoryImg as the reference slice
    const SubImageBox referenceSlice1Box(ImageVector(firstSliceOfVOI, 0, 0, 0, 0, 0), ImageVector(firstSliceOfVOI, inBoxFromImgExt.v2.y, inBoxFromImgExt.v2.z, 0, 0, 0));
    const SubImageBox referenceSlice2Box(ImageVector( lastSliceOfVoi, 0, 0, 0, 0, 0), ImageVector( lastSliceOfVoi, inBoxFromImgExt.v2.y, inBoxFromImgExt.v2.z, 0, 0, 0));

    // Compute the KL-measure for all slices in the inMemoryImg with respect to the reference slice
    std::vector<MLuint> referenceSliceHistogram(bins, 0);

    { // Reference Slice 1
      inMemoryImg.update(inPagedImg, referenceSlice1Box, inDataType);
      ML_IMPLEMENT_SCALAR_TYPE_CASES("", _VOLKAU_HISTOGRAM_APPEND_CODE, inDataType, "Volkau::handleNotification ()", "");
    }

    { // Reference Slice 2
      inMemoryImg.update(inPagedImg, referenceSlice2Box, inDataType);
      ML_IMPLEMENT_SCALAR_TYPE_CASES("", _VOLKAU_HISTOGRAM_APPEND_CODE, inDataType, "Volkau::handleNotification ()", "");
    }

    //  Create a probability distribution of the histogram
    const MLuint numberOfPixels((inBoxFromImgExt.v2.y + 1) * (inBoxFromImgExt.v2.z + 1));
    std::vector<MLdouble> referenceSliceProbability;
    normalize(referenceSliceHistogram, referenceSliceProbability, numberOfPixels * 2);

    MLdouble maxKLmeasure = 0;
    MLint maxSlice = 0;
      
    for (MLint x = 1; x <= lastSliceOfVoi - firstSliceOfVOI; ++x) {
      const SubImageBox sliceBox(ImageVector(firstSliceOfVOI + x, 0, 0, 0, 0, 0), ImageVector(firstSliceOfVOI + x, inBoxFromImgExt.v2.y, inBoxFromImgExt.v2.z, 0, 0, 0));
      inMemoryImg.update(inPagedImg, sliceBox, inDataType);

      std::vector<MLuint> sliceHistogram;
      std::vector<MLdouble> sliceProbability;
        
      ML_IMPLEMENT_SCALAR_TYPE_CASES("", _VOLKAU_HISTOGRAM_CODE, inDataType, "Volkau::handleNotification ()", "");

      normalize(sliceHistogram, sliceProbability, numberOfPixels);

      const MLdouble KLmeasure = kullbackLeibler(sliceProbability, referenceSliceProbability);

      if (KLmeasure > maxKLmeasure) {
        maxKLmeasure = KLmeasure;
        maxSlice = x;
      }
    }

    // Select the slice with the maximum KL-measure. This slice is referred as cMSP
    const SubImageBox cMSPBox(ImageVector(firstSliceOfVOI + maxSlice, 0, 0, 0, 0, 0), ImageVector(firstSliceOfVOI + maxSlice, inBoxFromImgExt.v2.y, inBoxFromImgExt.v2.z, 0, 0, 0));

    //  From here: cMSPx contains the x-value of the cMSP with respect to the
    //  whole volume
    const MLint cMSPx = firstSliceOfVOI + maxSlice;
    _outCMSPxFld->setIntValue(cMSPx);

	  log << "cMSP is at slice " << cMSPx << std::endl;
	  log << "KL value of cMSPx is " << maxKLmeasure << std::endl;

    /* SECOND STAGE */
    // Three corner points of cMSP are chosen and are represented by (cMSPx, Yi, Zi),
    // where i = 1,2,3. (Any three corner points may be used because they define
    // the same plane.)
    //const Vector3 cornerPoint1(cMSPx, 0, 0);
    //const Vector3 cornerPoint2(cMSPx, inBoxFromImgExt.v2.y, 0);
    //const Vector3 cornerPoint3(cMSPx, 0, inBoxFromImgExt.v2.z);

    // Define the reference slice at the distance of 2cm from the cMSP slice in the
    // sagittal direction. This reference slice remains the same for the whole
    // second stage.
    const SubImageBox newReferenceSlice1Box(ImageVector(cMSPx - 20 / inVoxelSize[0], 0, 0, 0, 0, 0), ImageVector(cMSPx - 20 / inVoxelSize[0], inBoxFromImgExt.v2.y, inBoxFromImgExt.v2.z, 0, 0, 0));
    const SubImageBox newReferenceSlice2Box(ImageVector(cMSPx + 20 / inVoxelSize[0], 0, 0, 0, 0, 0), ImageVector(cMSPx + 20 / inVoxelSize[0], inBoxFromImgExt.v2.y, inBoxFromImgExt.v2.z, 0, 0, 0));

    referenceSliceHistogram.clear();
    referenceSliceHistogram.resize(bins, 0);

    { // Reference Slice 1
      inMemoryImg.update(inPagedImg, newReferenceSlice1Box, inDataType);
      ML_IMPLEMENT_SCALAR_TYPE_CASES("", _VOLKAU_HISTOGRAM_APPEND_CODE, inDataType, "Volkau::handleNotification ()", "");
    }

    { // Reference Slice 2
      inMemoryImg.update(inPagedImg, newReferenceSlice2Box, inDataType);
      ML_IMPLEMENT_SCALAR_TYPE_CASES("", _VOLKAU_HISTOGRAM_APPEND_CODE, inDataType, "Volkau::handleNotification ()", "");
    }

    referenceSliceProbability.clear();

    normalize(referenceSliceHistogram, referenceSliceProbability, numberOfPixels * 2);

    // Let L be the number of sagittal slices in the VOI. A step size of
    // d0 = min( ceil( L/8 ), 8 ) is used for the initial search along the
    // sagittal direction. The search is conducted within the defined VOI only.
    MLuint L((lastSliceOfVoi - firstSliceOfVOI) / 2),
      stepSize(std::min(ceil(MLdouble(L / 8.)), MLdouble(8)));

    // Let the pertubed corner points of the plane defined at each step be
    // (a, Y1, Z1), (b, Y2, Z2), (c, Y3, Z3), where a,b,c from [s ± d0]. (We
    // choose three points which define a new plane on the image boundary with
    // y = const and z = const. To generate a new candidate plane only the x
    // coordinate is changed because it defines the position of a plane in the
    // volume along the sagittal direction.) Calculate the KL-measure for every
    // plane defined by the above points.
    MLint startA(cMSPx), 
      startB(cMSPx), 
      startC(cMSPx);

    // Create memory-image
    inMemoryImg.update(inPagedImg, inBoxFromImgExt, inDataType);
#define _VOLKAU_SECOND_STAGE_CODE(ODTYPE, INDTYPE, OPNAME) \
    { \
      const TSubImage<ODTYPE> VOI(inMemoryImg.getImage()); \
      /* Repeat while the step size is greater of equal to 1 pixel*/ \
      while (stepSize >= 1) { \
        MLint maxA(startA), maxB(startB), maxC(startC); \
        maxKLmeasure = 0; \
        \
        /*  Loop over all possible values of a, b and c. */ \
__pragma(omp parallel for) \
        for (MLint a = startA - L; a <= startA + L; a += stepSize) { \
          for (MLint b = startB - L; b <= startB + L; b += stepSize) { \
            for (MLint c = startC - L; c <= startC + L; c += stepSize) { \
              /* Generate a candidate slice, given a, b and c, and compute its histogram. */ \
              std::vector<MLuint> candidateSliceHistogram(bins, 0); \
              \
              for (MLint z = 0; z <= inBoxFromImgExt.v2.z; ++z) { \
                for (MLint y = 0; y <= inBoxFromImgExt.v2.y; ++y) { \
                  /* We want to find the pixel value for pixel (x, y, 1) in our plane, */ \
                  /* defined by cornerpoints (X1, Y1, a), (X2, Y2, b), (X3, Y3, c).    */ \
                  /*const Vector3 cornerPointA(a, cornerPoint1[1], cornerPoint1[2]);*/ \
                  /*const Vector3 cornerPointB(b, cornerPoint2[1], cornerPoint2[2]);*/ \
                  /*const Vector3 cornerPointC(c, cornerPoint3[1], cornerPoint3[2]);*/ \
                  \
                  /* First find the coordinates for the current (0, y, z) pixel in the           */\
                  /* image.                                                                      */ \
                  /* We must interpolate the x-coordinates between cornerPointA and cornerPointB */ \
                  /* We must interpolate the y-coordinates between cornerPointA and cornerPointC */ \
                  const MLdouble imageX((MLdouble)a + ((MLdouble)y / (MLdouble)inBoxFromImgExt.v2.y) * (MLdouble)(b - a) + ((MLdouble)z / (MLdouble)inBoxFromImgExt.v2.z) * (MLdouble)(c - a)); \
                  const MLdouble imageY(y); \
                  const MLdouble imageZ(z); \
                  \
                  /* We only apply linear interpolation along the z-axis, because that is */ \
                  /* the only axis that has to be interpolated. The values along x and y  */ \
                  /* map 1-to-1 with the pixels in those directions in the input image.   */ \
                  \
                  const MLdouble pixelValue = \
                    (1 - (imageX - floor(imageX)))     * VOI.getImageValue(floor(imageX),     imageY, imageZ) + \
                    (1 - (floor(imageX + 1) - imageX)) * VOI.getImageValue(floor(imageX + 1), imageY, imageZ); \
                  \
                  candidateSliceHistogram[(pixelValue - realMin) * bins / (realMax - realMin + ML_FLOAT_EPSILON)]++; \
                } \
              } \
              \
              std::vector<MLdouble> candidateSliceProbability(bins, 0); \
              normalize(candidateSliceHistogram, candidateSliceProbability, numberOfPixels); \
              \
              const MLdouble KLmeasure = kullbackLeibler(candidateSliceProbability, referenceSliceProbability); \
              \
__pragma(omp critical) \
              if (KLmeasure > maxKLmeasure) { \
                maxKLmeasure = KLmeasure; \
                maxA = a; \
                maxB = b; \
                maxC = c; \
              } \
            } \
          } \
        } \
        \
        startA = maxA; \
        startB = maxB; \
        startC = maxC; \
        \
        /* If the stepSize is lower than 1, we should terminate. However, because of  */ \
        /* the ceil operator, the stepSize will never be lower than 1:                */ \
        /* ceil (1 / 2) = ceil (0.5) = 1. So this will loop forever. Therefore we set */ \
        /* the stepSize to 0, if it is currently 1, causing the loop to stop.         */ \
        if (stepSize == 1) stepSize = 0; \
        \
        stepSize = ceil( MLdouble(stepSize / 2 )); \
        L = stepSize * 4; \
      } \
    } 

    ML_IMPLEMENT_SCALAR_TYPE_CASES("", _VOLKAU_SECOND_STAGE_CODE, inDataType, "Volkau::handleNotification ()", "");
#undef _VOLKAU_SECOND_STAGE_CODE

	  log << "Final KL measure is " << maxKLmeasure << std::endl;
      
    // The plane is now defined by the following coordinates:
    // (startA, 0, 0), (startB, yDim, 0), (startC, 0, zDim)

    // Corner points
    const Vector3 a(startA, 0, 0),
      b(startB, inBoxFromImgExt.v2.y, 0),
      c(startC, 0, inBoxFromImgExt.v2.z);

    // Transform the corner-points from voxel-coords to world-coords
    Vector3 aWorld, bWorld, cWorld;
    aWorld = inPagedImg->mapVoxelToWorld(a);
    bWorld = inPagedImg->mapVoxelToWorld(b);
    cWorld = inPagedImg->mapVoxelToWorld(c);

    // Set plane as new output
    Plane plane(aWorld, bWorld, cWorld);
    _outPlaneFld->setPlaneValue(plane);

	  _outLogFld->setStringValue(log.str());
  }




  
  // Touch output images to notify attached modules.
  const MLint numOutputImages = getNumOutputImages();
  for ( MLint idx = 0; idx < numOutputImages; ++idx ) {
    getOutputImageField( idx )->touch();
  }

}

void MidsagittalPlane::clear() {
  // Clear values of module's output fields (parameters and objects) and reset with initial values.
  
  // Touch module's output images to inform attached modules and ML-Host.
  const MLint numOutputImages = getNumOutputImages();
  for ( MLint idx = 0; idx < numOutputImages; ++idx ) {
    if ( getOutputImage( idx )->isValid()) {
      clearOutputImage( idx );
      getOutputImageField( idx )->touch();
    }
  }
}

void MidsagittalPlane::setOutputImageProperties( int /*outputIndex*/, PagedImage* outputImage ) {
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
  MidsagittalPlaneOutputImageHandler::setupKnownProperties( outputImage );


  const PagedImage* inputImage(getUpdatedInputImage(0));

  // Image extent
  const ImageVector inputImageExtend(inputImage->getImageExtent());
  outputImage->setImageExtent(inputImageExtend.y, inputImageExtend.z);

  // Voxel size
  const Vector3 inputVoxelSize(inputImage->getVoxelSize());
  outputImage->setVoxelSize(inputVoxelSize.y, inputVoxelSize.z, 1);
  
  // Valid  
  outputImage->setValid(_outPlaneFld->getPlaneValue().getNormal().compSum() != 0);

  MidsagittalPlaneOutputImageHandler::verifyProperties( outputImage );
}

CalculateOutputImageHandler* MidsagittalPlane::createCalculateOutputImageHandler( PagedImage* ) {
  MidsagittalPlaneOutputImageHandler::Parameters processingParameters;

  processingParameters.plane = transformPlane( this->_outPlaneFld->getPlaneValue(), getUpdatedInputImage(0)->getWorldToVoxelMatrix() );
  processingParameters.inputBoxFromImageExtent = getUpdatedInputImage(0)->getBoxFromImageExtent();

  return new MidsagittalPlaneOutputImageHandler( processingParameters );
}

ML_END_NAMESPACE