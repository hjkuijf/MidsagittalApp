#pragma once

#include <mlTypedHandlers.h>

ML_START_NAMESPACE

//! The MidsagittalPlaneOutputImageHandler handles the page calculation of an output image of MidsagittalPlane.
//! It should receive all required state from its caller in its
//! constructor and should not keep a reference to the output image nor the module itself.
//! This facilitates to implement a thread safe handler, which does not access non-thread safe shared data,
//! and which can be easily reused by other modules or within other contexts.
class MidsagittalPlaneOutputImageHandler : public TypedCalculateOutputImageHandler<MidsagittalPlaneOutputImageHandler,
    /* numInputs = */ 1,
    /* MLVariableType0 = */ ScalarTypes>
{

  public:
    //! Struct for processing parameters
    struct Parameters {

      Plane plane;
      SubImageBox inputBoxFromImageExtent;
    
      //! Default constructor ensuring that all variables are initialized.
      Parameters();
    };
  
    //! Template configuration of the output/input sub-images.
    enum {
    OutputSubImage_Type = MLVariableType0,
    InputSubImage0_Type = MLVariableType0,
    InputSubImage0_ReadOnly = true
  };

  private:
    //! Processing parameter set
    const Parameters _parameters;

  public:
    MidsagittalPlaneOutputImageHandler( const Parameters& parameters );
    
    //! Returns the input image region required to calculate a region of an output image.
    SubImageBox calculateInputSubImageBox( int inputIndex, const SubImageBox& outputSubImageBox );

    //! Templated output page calculation: compute output sub-image from input sub-image(s)
  template <typename OUTTYPE>
void typedCalculateOutputSubImage(TSubImage<OUTTYPE>& outputSubImage,
    const TSubImage<OUTTYPE>& inputSubImage0,
    UserThreadData* /*userThreadData*/);
};

ML_END_NAMESPACE