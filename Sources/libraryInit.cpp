#include <mlLibraryInitMacros.h>
#include "mlMidsagittalPlane.h"
#include "mlMidsagittalPlaneSplit.h"
#include "mlMidsagittalSurface.h"
#include "mlMidsagittalSurfaceSplit.h"
#include "SoView2DMidsagittalSurface.h"

// Include base wrapper objects
#include "alglibSpline2DWrapper.h"

ML_START_NAMESPACE

//! Calls init functions of all modules to add their types to the runtime type system of the ML.
int ISIMidsagittalInit() {
  AlglibSpline2DWrapper::initClass();

  MidsagittalPlane::initClass();
  MidsagittalPlaneSplit::initClass();
  MidsagittalSurface::initClass();
  MidsagittalSurfaceSplit::initClass();

  SoView2DMidsagittalSurface::initClass();

  return 1;
}

ML_END_NAMESPACE

ML_INIT_LIBRARY( ISIMidsagittalInit )