#----------------------------------------------------------------------------------
#! Macro module MidsagittalApp
#/*!
# \file    MidsagittalApp.py
# \author  Hugo
# \date    2014-07-30
#
# 
# */
#----------------------------------------------------------------------------------

from mevis import *
import os


class MidsagittalApp( object ):
  def __init__( self, ctx ):
    self._ctx = ctx
    
    # Disable MSP viewing on start. Because of dual-plane calculations, the user would see the various planes blinking during computation.
    self._ctx.field("SoTogglePlane.on").setBoolValue(False)
    
    # Set up field listeners
    self._ctx.addFieldListener( self._ctx.field("update"), self.do, False )
    self._ctx.addFieldListener( self._ctx.field("open"), self.openImage, False )
    self._ctx.addFieldListener( self._ctx.field("save"), self.save, False )
    
    
  def do( self ):
    self._ctx.field("Console.clear").touch()
    self._ctx.field("MidsagittalSurfaceMacro.update").touch()
    
    usedDualPlane = self._ctx.field("MidsagittalSurfaceMacro.outAngle").doubleValue() >= self._ctx.field("MidsagittalSurfaceMacro.inDualPlaneAngle").doubleValue()
    self._ctx.field("MSP.drawingOn").setBoolValue(not usedDualPlane)
    self._ctx.field("MSPAnterior.drawingOn").setBoolValue(usedDualPlane)
    self._ctx.field("MSPPosterior.drawingOn").setBoolValue(usedDualPlane)
    self._ctx.field("SoTogglePlane.on").setBoolValue(True)
    


  def openImage( self ):
    self._ctx.module("ImageLoader").showWindow("SingleImage")


  def save( self ):
    outputFilename = self._ctx.field("outputFilename").stringValue()
    
    if len(outputFilename) < 2:
      MLAB.showWarning("Output filename must be given.")
      return
    
    if not self._ctx.field("MidsagittalSurfaceMacro.outputCurvedSurfaceImage").isValid():
      MLAB.showWarning("There is no output.")
      return
    
    prefix = os.path.splitext(outputFilename)[0]
    
    self._ctx.field("PlaneSave.fileName").setStringValue(prefix + ".plane.nii")
    self._ctx.field("SurfaceSave.fileName").setStringValue(prefix + ".surface.nii")
    
    self._ctx.field("PlaneSave.save").touch()
    self._ctx.field("SurfaceSave.save").touch()
    
    # Save the plane equations
    with open(prefix + ".plane.txt", "wb") as planeFile:
      planeFile.write(self._ctx.field("MidsagittalSurfaceMacro.outMsp").stringValue()  + os.linesep)
      planeFile.write(self._ctx.field("MidsagittalSurfaceMacro.outMspA").stringValue() + os.linesep)
      planeFile.write(self._ctx.field("MidsagittalSurfaceMacro.outMspP").stringValue() + os.linesep)
      
    # Save the control points
    with open(prefix + ".points.txt", "wb") as pointsFile:
      points = self._ctx.field("MidsagittalSurfaceMacro.outputControlPoints").object().getMarkers()
      for point in points:
        pointsFile.write(str(point.pos[:3]) + os.linesep)
    
    
    MLAB.showInformation("Files saved.")


def shouldClose():  
  closeAllowed = True
  
  if MLAB.isStandaloneApplication() and ctx.field("MidsagittalSurfaceMacro.outputCurvedSurfaceImage").isValid():
    closeAllowed = MLAB.showQuestion("Are you sure you want to quit? Any unsaved data will be lost.", "Quit MidsagittalApp", ['Yes', 'No'], 1) == 0
    
  ctx.window().setCloseAllowed(closeAllowed)

  
