//----------------------------------------------------------------------------------
//! The Inventor module class SoView2DMidsagittalSurface derived from SoView2DExtension
/*!
// \file    SoView2DMidsagittalSurface.h
// \author  Hugo
// \date    2015-08-17
//
// 
*/
//----------------------------------------------------------------------------------

#pragma once


// Local includes
#include "librarySystem.h"

// Wrap Inventor includes into XVEnterScope and XVLeaveScope to avoid
// collisions between OpenInventor and Windows headers.
#include <XVEnterScope.h>
#include <Inventor/nodes/SoSubNode.h>
#include <SoView2DExtension.h>
#include <View2DSlice.h>
#include <View2DSliceList.h>
// include used fields
#include <SoSFMLBase.h>
#include <XVLeaveScope.h>

#include <mlAPI.h>

// Include BaseWrapper
#include "alglibSpline2DWrapper.h"

//! 
class ISIMIDSAGITTAL_EXPORT SoView2DMidsagittalSurface : public SoView2DExtension
{
  //! Implements the runtime type system interface of this new node.
  SO_NODE_HEADER(SoView2DMidsagittalSurface);

public:
  //! Constructor
  SoView2DMidsagittalSurface();

  //! Initializes this class (called on DLL initialization).
  static void initClass();

  //@{! Fields
  //! Midsagittal surface
  SoSFMLBase inMidsagittalSurface;
  //@}


  //! Handling of events occurring in the viewer.
  //! \c view2d is the node calling
  //! this function to offer the possibility to handle this event. \c list
  //! is the list of all available slices potentially drawn or for which these
  //! events could be of interest.
  //! Note that this method is called only once when the event is occurring
  //! in the viewer; it is NOT called for each slice.
  //! \c event is the container describing the current event, e.g. whether it's
  //! a mouse or a keyboard event, whether a key is pressed, released or the
  //! current mouse button.
  //! \c phase describes whether the event starts (e.g. button press), is active
  //! (e.g. mouse motion) or is deactivated (e.g. button release).
  //! The return value should be true if the event is handled by the methods and
  //! false if not. However, currently the return value is not used any more.
  //! See \c SoView2DExample for an example
  virtual bool evalEvent(SoView2D* view2d, View2DSliceList* slicelist,
                         View2DEvent* eventContainer, View2DEventPhase eventPhase);

  //! Virtual method called by the \c SoView2D node when an image is rendered.
  //! It is called for each slices
  //! drawn in each viewer. \c list is the \c View2DSliceList, the list containing
  //! all information of all slices handled by the viewer. \c slice is the slice
  //! currently drawn and \c z the the number of the currently drawn slice.
  //! See \c SoView2DExample for an example
  virtual void draw(View2DSliceList* slicelist, View2DSlice* slice, int sliceZ);

protected:
  //! Protected destructor
  virtual ~SoView2DMidsagittalSurface();



private:

  // your own member variables...

  //! Helper variable to store last selected point where a cross shall be drawn
  //! during mouse interaction. Default is the invalid position (-1,-1,-1).
  SbVec3f _lastSelectedPosition;
};