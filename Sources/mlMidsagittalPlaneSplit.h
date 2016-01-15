#pragma once

#include "librarySystem.h"
#include <mlAlgorithmModule.h>

ML_START_NAMESPACE

class ISIMIDSAGITTAL_EXPORT MidsagittalPlaneSplit : public AlgorithmModule {
  public:
    MidsagittalPlaneSplit();
    ~MidsagittalPlaneSplit();
    virtual CalculateOutputImageHandler* createCalculateOutputImageHandler( PagedImage* outputImage );

  protected:
    virtual void validateInput();
    virtual void update();
    virtual void clear();
    virtual void setOutputImageProperties( int outputIndex, PagedImage* outputImage );
  
  private:
    void addInputFields();
    PlaneField* _inPlaneFld;
    EnumField* _inIntersectionModeFld;
    DoubleField* _inVolumeThresholdFld;
    BoolField* _inUseGlobalSubsampleFld;
    IntField* _inGlobalSubsampleFld;
    IntField* _inXSubsampleFld;
    IntField* _inYSubsampleFld;
    IntField* _inZSubsampleFld;
    

  ML_MODULE_CLASS_HEADER( MidsagittalPlaneSplit )
};

ML_END_NAMESPACE