#pragma once

#include "librarySystem.h"
#include <mlAlgorithmModule.h>

ML_START_NAMESPACE

class ISIMIDSAGITTAL_EXPORT MidsagittalPlane : public AlgorithmModule {
  public:
    MidsagittalPlane();
    ~MidsagittalPlane();
    virtual CalculateOutputImageHandler* createCalculateOutputImageHandler( PagedImage* outputImage );

  protected:
    virtual void validateInput();
    virtual void update();
    virtual void clear();
    virtual void setOutputImageProperties( int outputIndex, PagedImage* outputImage );
  
  private:
    void addInputFields();
    void addOutputFields();
    PlaneField* _outPlaneFld;
    IntField* _outCMSPxFld;
    IntField* _inBinsFld;
    StringField* _outLogFld;

  ML_MODULE_CLASS_HEADER( MidsagittalPlane )
};

ML_END_NAMESPACE