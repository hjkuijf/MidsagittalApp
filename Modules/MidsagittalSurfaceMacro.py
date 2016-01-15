from AlgorithmMacroModule.AlgorithmMacroModule import AlgorithmMacroModule, Error
from AlgorithmModule.Definitions import StatusCode

import numpy

class MidsagittalSurfaceMacro( AlgorithmMacroModule ):
  def _init( self ):
    self._ctx.field("MemoryCache.clear").touch()
    self._ctx.field("outMsp").setVectorValue((1,0,0,0))
    self._ctx.field("outMspA").setVectorValue((1,0,0,0))
    self._ctx.field("outMspP").setVectorValue((1,0,0,0))
    self._ctx.field("outAngle").setDoubleValue(0)
    
    self._ctx.field("progress").setDoubleValue(0)
    
  def _getPlane( self, imageField ):
    self._ctx.field("MidsagittalPlane.input0").connectFrom(ctx.field(imageField))
    self._ctx.field("MidsagittalPlane.update").touch()
    return self._ctx.field("MidsagittalPlane.outPlane").vectorValue()
  
  def __init__( self, ctx ):
    AlgorithmMacroModule.__init__( self, ctx )
    
    self._init()

  def _validateInput( self ):
    if not self._ctx.field("input0").isValid():
      raise Error( StatusCode.ERROR_INPUT_OBJECT, u"Input image not valid.")
    

  def _update( self ):
    self._init()
    
    self._ctx.field("progress").setDoubleValue(0.05)
    
    # Compute the midsagittal plane
    msp = self._getPlane("Bypass.output0")
    self._ctx.field("MemoryCache.update").touch()
    self._ctx.field("outMsp").setVectorValue(msp)
    
    # Set the cMSPx from the plane computed on the full image
    self._ctx.field("MidsagittalSurface.inCMSPx").setIntValue(self._ctx.field("MidsagittalPlane.outCMSPx").intValue())
    
    self._ctx.field("progress").setDoubleValue(0.1)
    
    # Do we want to use the dual-plane approach, to handle extreme asymmetric cases?
    if self._ctx.field("inUseDualPlane").boolValue():
      mspA = self._getPlane("Anterior.output0")
      self._ctx.field("progress").setDoubleValue(0.2)
      mspP = self._getPlane("Posterior.output0")
      self._ctx.field("progress").setDoubleValue(0.3)
      
      self._ctx.field("outMspA").setVectorValue(mspA)
      self._ctx.field("outMspP").setVectorValue(mspP)
      
      # Compute the in-plane angle between the two MSPs
      vA = numpy.array(mspA[:2])
      vP = numpy.array(mspP[:2])
      
      # Normalize to unit length
      vA = vA / numpy.linalg.norm(vA)
      vP = vP / numpy.linalg.norm(vP)
      
      # Compute angle. Special case if vectors have an angle of 0 or 180 degrees
      angle = numpy.arccos(numpy.dot(vA, vP))
      if numpy.isnan(angle):
        if (v1_u == v2_u).all():
          angle = 0.0
        else:
          angle = numpy.pi
      
      self._ctx.field("outAngle").setDoubleValue(numpy.degrees(angle))
      
      # Use dual plane ?
      if numpy.degrees(angle) > self._ctx.field("inDualPlaneAngle").doubleValue(): 
        self._ctx.field("MidsagittalSurface.inPlane1").setVectorValue(mspA) 
        self._ctx.field("MidsagittalSurface.inPlane2").setVectorValue(mspP)
        self._ctx.field("MidsagittalSurface.inUseSecondPlane").setBoolValue(True)
      else:
        self._ctx.field("MidsagittalSurface.inPlane1").setVectorValue(msp)
        self._ctx.field("MidsagittalSurface.inUseSecondPlane").setBoolValue(False)
      
    self._ctx.field("progress").setDoubleValue(0.4)
    
    self._ctx.field("MidsagittalSurface.update").touch()
    
    self._ctx.field("progress").setDoubleValue(1)

  def _clear( self ):
    self._ctx.field("MidsagittalSurface.clear").touch()
    
    self._init()
    