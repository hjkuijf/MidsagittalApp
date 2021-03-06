#include "mlMidsagittalSurfaceSplit.h"
#include "mlMidsagittalSurfaceSplitOutputImageHandler.h"

ML_START_NAMESPACE

ML_MODULE_CLASS_SOURCE( MidsagittalSurfaceSplit, AlgorithmModule );

MidsagittalSurfaceSplit::MidsagittalSurfaceSplit() : AlgorithmModule( 1, 2 ) {
  handleNotificationOff();
  addInputFields();
  handleNotificationOn();
}

void MidsagittalSurfaceSplit::addInputFields() {
  _alglibSpline2DWrapper = nullptr;

  static const char* const inIntersectionModeValues[] = { "midpoint", "volume", "antialiasing" };
  _inIntersectionModeFld = addEnum( "inIntersectionMode", inIntersectionModeValues, 3 );
  _inIntersectionModeFld->setEnumValue( 0 );
  _inVolumeThresholdFld = addDouble( "inVolumeThreshold", 0.5 );
  _inUseGlobalSubsampleFld = addBool( "inUseGlobalSubsample", true );
  _inGlobalSubsampleFld = addInt( "inGlobalSubsample", 4 );
  _inXSubsampleFld = addInt( "inXSubsample", 4 );
  _inYSubsampleFld = addInt( "inYSubsample", 4 );
  _inZSubsampleFld = addInt( "inZSubsample", 4 );
  _inMidsagittalSurfaceFld = addBase( "inMidsagittalSurface", nullptr );
  _inMidsagittalSurfaceFld->setBaseValueAndAddAllowedType(_alglibSpline2DWrapper);
}

MidsagittalSurfaceSplit::~MidsagittalSurfaceSplit() {
}

void MidsagittalSurfaceSplit::validateInput() {
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

  Base* baseValue = _inMidsagittalSurfaceFld->getBaseValue();
  _alglibSpline2DWrapper = mlbase_cast<AlglibSpline2DWrapper*>(baseValue);
  if (_alglibSpline2DWrapper == nullptr) {
      const std::string msg = "No midsagittal surface connected.";
      throw AlgorithmModule::Error( AlgorithmModule::SC_ERROR_INPUT_OBJECT, msg );
  } else {
    if (!_alglibSpline2DWrapper->isValid) {
      const std::string msg = "No valid midsagittal surface.";
      throw AlgorithmModule::Error( AlgorithmModule::SC_ERROR_INPUT_OBJECT, msg );
    }
  }


}

void MidsagittalSurfaceSplit::update() {
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
  
  // Touch output images to notify attached modules.
  const MLint numOutputImages = getNumOutputImages();
  for ( MLint idx = 0; idx < numOutputImages; ++idx ) {
    getOutputImageField( idx )->touch();
  }
}

void MidsagittalSurfaceSplit::clear() {
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

void MidsagittalSurfaceSplit::setOutputImageProperties( int /*outputIndex*/, PagedImage* outputImage ) {
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
  MidsagittalSurfaceSplitOutputImageHandler::setupKnownProperties( outputImage );
  MidsagittalSurfaceSplitOutputImageHandler::verifyProperties( outputImage );
}

CalculateOutputImageHandler* MidsagittalSurfaceSplit::createCalculateOutputImageHandler( PagedImage* image ) {
  MidsagittalSurfaceSplitOutputImageHandler::Parameters processingParameters;

  processingParameters.inIntersectionMode = this->_inIntersectionModeFld->getEnumValue();
  processingParameters.inVolumeThreshold = this->_inVolumeThresholdFld->getDoubleValue();
  processingParameters.inUseGlobalSubsample = this->_inUseGlobalSubsampleFld->getBoolValue();
  processingParameters.inGlobalSubsample = this->_inGlobalSubsampleFld->getIntValue();
  processingParameters.inXSubsample = this->_inXSubsampleFld->getIntValue();
  processingParameters.inYSubsample = this->_inYSubsampleFld->getIntValue();
  processingParameters.inZSubsample = this->_inZSubsampleFld->getIntValue();
  processingParameters.spline = this->_alglibSpline2DWrapper->spline;
  processingParameters.outputIndex = image->getOutputIndex();

  // PASS OUTPUT INDEX

  return new MidsagittalSurfaceSplitOutputImageHandler( processingParameters );
}

ML_END_NAMESPACE