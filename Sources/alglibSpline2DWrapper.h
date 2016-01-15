#pragma once

// Local includes
#include "librarySystem.h"

// ML includes
#include <mlBase.h>

// ALGLIB includes
#include "interpolation.h"


ML_START_NAMESPACE

class ISIMIDSAGITTAL_EXPORT AlglibSpline2DWrapper : public Base
{
public:
  // Constructor
  AlglibSpline2DWrapper() : isValid(false) {}
  virtual ~AlglibSpline2DWrapper() { }

  alglib::spline2dinterpolant spline;
  bool isValid;


private:

  ML_CLASS_HEADER(AlglibSpline2DWrapper);
};

ML_END_NAMESPACE
