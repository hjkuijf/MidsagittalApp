#pragma once

#include "librarySystem.h"
#include <mlAlgorithmModule.h>

#include "mlXMarkerList.h"

#include "alglibSpline2DWrapper.h"
#include "functions.h"

// ALGLIB includes
#include "interpolation.h"

// dlib includes
#include "dlib/optimization.h"

ML_START_NAMESPACE

class ISIMIDSAGITTAL_EXPORT MidsagittalSurface : public AlgorithmModule {
  public:
    MidsagittalSurface();
    ~MidsagittalSurface();
    virtual CalculateOutputImageHandler* createCalculateOutputImageHandler( PagedImage* outputImage );
    
    //! Configures (in)validation handling of inputs which are not connected or up to date.
    virtual INPUT_HANDLE handleInput(int inputIndex, INPUT_STATE state) const;

  protected:
    virtual void validateInput();
    virtual void update();
    virtual void clear();
    virtual void setOutputImageProperties( int outputIndex, PagedImage* outputImage );
    
    //! Get a probability distribution of the grey-values in the reference-slices.
    //!
    //! The reference slices are located at 2 cm away from the cMSPx. The cMSPx is
    //! taken from the corresponding input-field on the Module and originates from
    //! the Volkau module.
    //! This way, the initial KL-measure is equal to the optimal KL-measure resulting
    //! from the Volkau module.
    /*!
      \param PagedImage * inputPagedImage Image to use
      \param const MLuint bins Number of bins in the distribution
      \param const MLldouble realMin Lowest grey-value in inputPagedImage
      \param const MLldouble realMax Highest grey-value in inputPagedImage
      \return const std::vector<MLdouble> Probability distribution of the reference-slices
    */
    const std::vector<MLdouble> getReferenceSliceProbability( PagedImage* inputPagedImage, PagedImage* inputPagedMaskImage, const MLuint bins, const MLldouble realMin, const MLldouble realMax ) const;

    //! Creates an axis of a regular grid. 
    //!
    //! Creates a range from [0 .. size] (so including 0 and size), with a total of numberOfPoints points and
    //! stores the result in axis
    /*!
      \param const MLuint numberOfPoints Number of points on the axis
      \param double * axis Output axis
      \param const MLint size Length of the axis
    */
    void createGridAxis( const MLuint numberOfPoints, double* axis, const MLint size ) const;
    //MLuint createGridAxisMm( double*& axis , const MLuint& imageExtent, const MLdouble& voxelSize ) const;
    std::tuple<MLuint, std::unique_ptr<double[]> > MidsagittalSurface::createGridAxisMm( const MLuint& imageExtent, const MLdouble& voxelSize ) const;

    //! Builds a spline with the shape of a plane, given the four corner-points of the plane on the image
    /*!
      \param const PagedImage * inputImage 
      \param const Plane & plane
      \return alglib::spline2dinterpolant Plane-shaped spline
    */
    alglib::spline2dinterpolant buildSplineFromCornerPoints( const PagedImage* inputImage, const Plane &plane ) const;

    //! Given a plane, it will compute four corner-points in the image, where the sides of the image intersect with the plane.
    //!
    //! 2D example:
    //! --\------  <= corner1
    //! |  \    |
    //! |   \   |
    //! |    \  |
    //! ------\--  <= corner2
    //!
    /*!
      \param const PagedImage * inputImage 
      \param const Plane & inputPlane 
      \param Vector3 & corner1 
      \param Vector3 & corner2 
      \param Vector3 & corner3 
      \param Vector3 & corner4 
    */
    void getCornerPoints( const PagedImage* inputImage, const Plane &inputPlane, Vector3 &corner1, Vector3 &corner2, Vector3 &corner3, Vector3 &corner4 ) const;
  
  
  private:
    void addInputFields();
    void addOutputFields();

    PlaneField* _inPlane1Fld;
    PlaneField* _inPlane2Fld;
    BoolField* _inUseSecondPlaneFld;
    IntField* _inCMSPxFld;
    IntField* _inBinsFld;
    DoubleField* _inGridSizeFld;
    IntField* _inMaxSizeFld;
    DoubleField* _inDeltaFld;
    DoubleField* _inEpsFld;
    EnumField* _inOptimizerFld;
    BaseField* _outXMarkerListFld;
    BaseField* _outSplineFld;
    
    //! Wrapper that is attached to the BaseField, holding the spline
    AlglibSpline2DWrapper _alglibSpline2DWrapper;

    XMarkerList _outXMarkerList;

  ML_MODULE_CLASS_HEADER( MidsagittalSurface )
};

ML_END_NAMESPACE