//----------------------------------------------------------------------------------
//! The Inventor module class SoView2DMidsagittalSurface
/*!
// \file    SoView2DMidsagittalSurface.cpp
// \author  Hugo
// \date    2015-08-17
//
// 
*/
//----------------------------------------------------------------------------------

// Local includes
#include "SoView2DMidsagittalSurface.h"

// Wrap inventor includes into XVEnterScope and XVLeaveScope to avoid
// collisions between OpenInventor and Windows headers.
#include <XVEnterScope.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/actions/SoGLRenderAction.h>
// TODO: include more inventor things here
#include <XVLeaveScope.h>

SO_NODE_SOURCE(SoView2DMidsagittalSurface)



void SoView2DMidsagittalSurface::initClass()
{
  SO_NODE_INIT_CLASS(SoView2DMidsagittalSurface, SoView2DExtension, "SoView2DExtension");
}


SoView2DMidsagittalSurface::SoView2DMidsagittalSurface()
{
  // Execute Inventor internal code for node construction.
  SO_NODE_CONSTRUCTOR(SoView2DMidsagittalSurface);

  //! Set inherited fields to a default state using the default string setting
  //! mechanism available in OpenInventor. E.g. in this case the extension
  //! shall be activated by button 1 and it must be over a valid voxel position
  //! to be activated.
  set("button1 PRESSED button2 IGNORED button3 IGNORED needsValidVoxel TRUE");
  SO_NODE_ADD_FIELD(inMidsagittalSurface, (NULL));
}

SoView2DMidsagittalSurface::~SoView2DMidsagittalSurface()
{
}




bool SoView2DMidsagittalSurface::evalEvent(SoView2D* /*view2d*/, View2DSliceList* /*slicelist*/,
                             View2DEvent* /*eventContainer*/, View2DEventPhase /*eventPhase*/)
{
  return FALSE;
}


void SoView2DMidsagittalSurface::draw(View2DSliceList* slicelist, View2DSlice* slice, int sliceZ)
{
  auto baseValue = inMidsagittalSurface.getValue();
  if (baseValue == nullptr) return; // No valid Base

  const auto alglibSpline2DWrapper = mlbase_cast<ml::AlglibSpline2DWrapper*>(baseValue);
  if (alglibSpline2DWrapper == nullptr) return; // Base value is not of type AlglibSpline2DWrapper
  if (!alglibSpline2DWrapper->isValid) return; // Empty spline

  // Drawing field enabled, i.e. are we supposed to draw?
  if (drawingOn.getValue()){
    // Yes, get size of connected image from the slice list. Note that this is the
    // list of slices connected to the SoView2D. Note that there is usually no need
    // for an additional image connected to this SoView2DExtension since that one of
    // the SoView2D accessable by dsl->getInputImage() is the relevant one.
    int x, y, z, c, t;
    slicelist->getInputImage()->getSize(x, y, z, c, t);

    //! Get the 2d devices (pixel) coordinates of the origin and the opposite corner of the
    //! 2D region in which the slice is drawn. Note that these coordinates have nothing to do
    //! with voxel coordinates. These coordinates are directly drawable device coordinates.
    float dx1, dy1;
    float dx2, dy2;
    slice->getVisibleDeviceRect(dx1, dy1, dx2, dy2);

    float sx, sy;
    slice->getDrawSize(sx, sy);


    // Drawing the midsagittal surface

    int yStart(0), yEnd(y);
    // Loop to the first voxel that has a device-position >= 0
    for ( ; slice->mapVoxelToDevice(0, yStart, 0, dx1, dy1), dy1 < 0; ++yStart ) {}
    // Similar, find the last voxel that is still withing the DrawSize
    for ( ; slice->mapVoxelToDevice(0, yEnd, 0, dx1, dy1), dy1 > sy; --yEnd ) {}

    glColor3fv(_rgbColor);
    for (int i(0); i < sy; ++i ) {
      const double y1 = double(yStart) + double(i) * (yEnd - yStart) / sy;
      const double y2 = double(yStart) + double(i + 1) * (yEnd - yStart) / sy;

      const double x1 = alglib::spline2dcalc(alglibSpline2DWrapper->spline, y1, sliceZ);
      const double x2 = alglib::spline2dcalc(alglibSpline2DWrapper->spline, y2, sliceZ);

      slice->mapVoxelToDevice(x1, y1, sliceZ, dx1, dy1);
      slice->mapVoxelToDevice(x2, y2, sliceZ, dx2, dy2);

      glBegin(GL_LINES);
      glVertex2f(dx1, dy1);
      glVertex2f(dx2, dy2);
      glEnd();
    }
  }
}
