#pragma once

#include "librarySystem.h"
#include <mlAlgorithmModule.h>

#include "alglibSpline2DWrapper.h"

ML_START_NAMESPACE

class ISIMIDSAGITTAL_EXPORT MidsagittalSurfaceSplit : public AlgorithmModule {
  public:
    MidsagittalSurfaceSplit();
    ~MidsagittalSurfaceSplit();
    virtual CalculateOutputImageHandler* createCalculateOutputImageHandler( PagedImage* outputImage );

  protected:
    virtual void validateInput();
    virtual void update();
    virtual void clear();
    virtual void setOutputImageProperties( int outputIndex, PagedImage* outputImage );
  
  private:
    void addInputFields();
    BaseField* _inMidsagittalSurfaceFld;
    EnumField* _inIntersectionModeFld;
    DoubleField* _inVolumeThresholdFld;
    BoolField* _inUseGlobalSubsampleFld;
    IntField* _inGlobalSubsampleFld;
    IntField* _inXSubsampleFld;
    IntField* _inYSubsampleFld;
    IntField* _inZSubsampleFld;
    
    //! Wrapper that is attached to the BaseField, holding the spline
    AlglibSpline2DWrapper* _alglibSpline2DWrapper;

  ML_MODULE_CLASS_HEADER( MidsagittalSurfaceSplit )
};

ML_END_NAMESPACE