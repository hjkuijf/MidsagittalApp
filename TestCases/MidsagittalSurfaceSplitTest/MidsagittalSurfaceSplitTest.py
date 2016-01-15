# **InsertLicense** code

from mevis import *
from TestSupport import Base, Fields, Logging, ScreenShot
from TestSupport.Macros import *
from AlgorithmModuleTestSupport import Checks as AMChecks
from AlgorithmModuleTestSupport import Tests as AMTests
from AlgorithmModule.Definitions import StatusCode

#----------------------------------------------------------------------------------

MODULE_NAME = "MidsagittalSurfaceSplit"

class TestData( object ):
  """Contains a setting of input and corresponding reference output field values."""
  def __init__( self ):
    self.input0Path = None
    self.inMidsagittalSurfacePath = None
    self.inIntersectionMode = None
    self.inVolumeThreshold = None
    self.inUseGlobalSubsample = None
    self.inGlobalSubsample = None
    self.inXSubsample = None
    self.inYSubsample = None
    self.inZSubsample = None
    self.output0ReferencePath = None

def executeTestOfUpdate( testData ):
  __provideInput( testData )
  __processInput()
  __validateOutput( testData )

def __provideInput( testData ):
  __setUpNetworkWithInput( testData )

def __setUpNetworkWithInput( testData ):
  __setUpInputObjects( testData )
  __setUpInputParameters( testData )

def __setUpInputObjects( testData ):
  __loadFileWithLocalModule( "Load_input0", testData.input0Path )
  __loadFileWithLocalModule( "Load_inMidsagittalSurface", testData.inMidsagittalSurfacePath )

def __loadFileWithLocalModule( moduleName, filePath ):
  if ( filePath != None ):
    Fields.setValue( "{}.name".format( moduleName ), filePath )
    Fields.touch( "{}.load".format( moduleName ))
  else:
    Fields.touch( "{}.close".format( moduleName ))

def __setUpInputParameters( testData ):
  Fields.setValue( "{}.inIntersectionMode".format( MODULE_NAME ), testData.inIntersectionMode )
  Fields.setValue( "{}.inVolumeThreshold".format( MODULE_NAME ), testData.inVolumeThreshold )
  Fields.setValue( "{}.inUseGlobalSubsample".format( MODULE_NAME ), testData.inUseGlobalSubsample )
  Fields.setValue( "{}.inGlobalSubsample".format( MODULE_NAME ), testData.inGlobalSubsample )
  Fields.setValue( "{}.inXSubsample".format( MODULE_NAME ), testData.inXSubsample )
  Fields.setValue( "{}.inYSubsample".format( MODULE_NAME ), testData.inYSubsample )
  Fields.setValue( "{}.inZSubsample".format( MODULE_NAME ), testData.inZSubsample )

def __processInput():
  AMTests.testUpdate( MODULE_NAME )

def __validateOutput( testData ):
  __setUpNetworkWithOutputReference( testData )
  __executeChecksToValidateOutput( testData )

def __setUpNetworkWithOutputReference( testData ):
  __setUpOutputReferenceObjects( testData )

def __setUpOutputReferenceObjects( testData ):
  __loadFileWithLocalModule( "Load_output0_Reference", testData.output0ReferencePath )

def __executeChecksToValidateOutput( testData ):
  checkList = __getChecksToValidateOutputAfterUpdate( testData )
  for check in checkList:
    check.execute()

def __getChecksToValidateOutputAfterUpdate( testData ):
  checks = (
    AMChecks.ImageCompareCheck( "Validate_output0" ),
  )
  return checks

def __getDefaultTestData():
  """Returns a default and valid TestData object."""
  Logging.infoHTML( "<b>ToDo: Set valid values for default TestData object!</b>" )
  testData = TestData()
  testData.input0Path = None
  testData.inMidsagittalSurfacePath = None
  testData.inIntersectionMode = None
  testData.inVolumeThreshold = None
  testData.inUseGlobalSubsample = None
  testData.inGlobalSubsample = None
  testData.inXSubsample = None
  testData.inYSubsample = None
  testData.inZSubsample = None
  testData.output0ReferencePath = None
  return testData

#----------------------------------------------------------------------------------

def ITERATIVETEST_Error_Of_Input_Image_input0():
  """Tests occurrence of an error because of an invalid image provided at field "input0"."""
  Logging.infoHTML( "<b>ToDo: Add more \"TestData\" objects to maximize variations!</b>" )
  testDataVariations = {
    "Missing_Object" : __getMissingObjectTestData_input0(),
  }
  return ( testDataVariations, testError_input0 )

def __getMissingObjectTestData_input0():
  testData = __getDefaultTestData()
  # Note: None as path value prevents loading.
  testData.input0Path = None
  return testData

def testError_input0( testData ):
  __provideInput( testData )
  checksToExecute = (
    AMChecks.EqualCheck( fieldName = "{}.statusCode".format( MODULE_NAME ),
                         value     = StatusCode.ERROR_INPUT_OBJECT ),
  )
  AMTests.testFailedUpdate( MODULE_NAME, checksToExecute )

#----------------------------------------------------------------------------------

def ITERATIVETEST_Error_Of_Input_Base_inMidsagittalSurface():
  """Tests occurrence of an error because of an invalid Base object connected at field "inMidsagittalSurface"."""
  Logging.infoHTML( "<b>ToDo: Add more \"TestData\" objects to maximize variations!</b>" )
  testDataVariations = {
    "Missing_Object" : __getMissingObjectTestData_inMidsagittalSurface(),
    "Empty_Object" : __getEmptyObjectTestData_inMidsagittalSurface(),
  }
  return ( testDataVariations, testError_inMidsagittalSurface )

def __getMissingObjectTestData_inMidsagittalSurface():
  testData = __getDefaultTestData()
  # Note: None as path value prevents loading.
  testData.inMidsagittalSurfacePath = None
  return testData

def __getEmptyObjectTestData_inMidsagittalSurface():
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with a path pointing to an empty object!</b>" )
  testData.inMidsagittalSurfacePath = None
  return testData

def testError_inMidsagittalSurface( testData ):
  __provideInput( testData )
  checksToExecute = (
    AMChecks.EqualCheck( fieldName = "{}.statusCode".format( MODULE_NAME ),
                         value     = StatusCode.ERROR_INPUT_OBJECT ),
  )
  AMTests.testFailedUpdate( MODULE_NAME, checksToExecute )

#----------------------------------------------------------------------------------

def ITERATIVETEST_Error_Of_Input_Parameter_inIntersectionMode():
  """Tests occurrence of an error because of an invalid input value provided at field "inIntersectionMode"."""
  Logging.infoHTML( "<b>ToDo: Add more \"TestData\" objects to maximize variations!</b>" )
  testDataVariations = {
    "Invalid_Variation_1" : __getInvalidTestDataVariation1_inIntersectionMode(),
    "Invalid_Variation_2" : __getInvalidTestDataVariation2_inIntersectionMode(),
  }
  return ( testDataVariations, testError_inIntersectionMode )

def __getInvalidTestDataVariation1_inIntersectionMode():
  Logging.infoHTML( "<b>ToDo: Rename function that the name matches the intention of returned test data!</b>" )
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with a reliable invalid value of \"inIntersectionMode\"!</b>" )
  testData.inIntersectionMode = None
  return testData

def __getInvalidTestDataVariation2_inIntersectionMode():
  Logging.infoHTML( "<b>ToDo: Rename function that the name matches the intention of returned test data!</b>" )
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with a reliable invalid value of \"inIntersectionMode\"!</b>" )
  testData.inIntersectionMode = None
  return testData

def testError_inIntersectionMode( testData ):
  __provideInput( testData )
  checksToExecute = (
    AMChecks.EqualCheck( fieldName = "{}.statusCode".format( MODULE_NAME ),
                         value     = StatusCode.ERROR_INPUT_PARAMETER ),
  )
  AMTests.testFailedUpdate( MODULE_NAME, checksToExecute )

#----------------------------------------------------------------------------------

def ITERATIVETEST_Error_Of_Input_Parameter_inVolumeThreshold():
  """Tests occurrence of an error because of an invalid input value provided at field "inVolumeThreshold"."""
  Logging.infoHTML( "<b>ToDo: Add more \"TestData\" objects to maximize variations!</b>" )
  testDataVariations = {
    "Invalid_Variation_1" : __getInvalidTestDataVariation1_inVolumeThreshold(),
    "Invalid_Variation_2" : __getInvalidTestDataVariation2_inVolumeThreshold(),
  }
  return ( testDataVariations, testError_inVolumeThreshold )

def __getInvalidTestDataVariation1_inVolumeThreshold():
  Logging.infoHTML( "<b>ToDo: Rename function that the name matches the intention of returned test data!</b>" )
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with a reliable invalid value of \"inVolumeThreshold\"!</b>" )
  testData.inVolumeThreshold = None
  return testData

def __getInvalidTestDataVariation2_inVolumeThreshold():
  Logging.infoHTML( "<b>ToDo: Rename function that the name matches the intention of returned test data!</b>" )
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with a reliable invalid value of \"inVolumeThreshold\"!</b>" )
  testData.inVolumeThreshold = None
  return testData

def testError_inVolumeThreshold( testData ):
  __provideInput( testData )
  checksToExecute = (
    AMChecks.EqualCheck( fieldName = "{}.statusCode".format( MODULE_NAME ),
                         value     = StatusCode.ERROR_INPUT_PARAMETER ),
  )
  AMTests.testFailedUpdate( MODULE_NAME, checksToExecute )

#----------------------------------------------------------------------------------

def ITERATIVETEST_Error_Of_Input_Parameter_inUseGlobalSubsample():
  """Tests occurrence of an error because of an invalid input value provided at field "inUseGlobalSubsample"."""
  Logging.infoHTML( "<b>ToDo: Add more \"TestData\" objects to maximize variations!</b>" )
  testDataVariations = {
    "Invalid_Variation_1" : __getInvalidTestDataVariation1_inUseGlobalSubsample(),
    "Invalid_Variation_2" : __getInvalidTestDataVariation2_inUseGlobalSubsample(),
  }
  return ( testDataVariations, testError_inUseGlobalSubsample )

def __getInvalidTestDataVariation1_inUseGlobalSubsample():
  Logging.infoHTML( "<b>ToDo: Rename function that the name matches the intention of returned test data!</b>" )
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with a reliable invalid value of \"inUseGlobalSubsample\"!</b>" )
  testData.inUseGlobalSubsample = None
  return testData

def __getInvalidTestDataVariation2_inUseGlobalSubsample():
  Logging.infoHTML( "<b>ToDo: Rename function that the name matches the intention of returned test data!</b>" )
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with a reliable invalid value of \"inUseGlobalSubsample\"!</b>" )
  testData.inUseGlobalSubsample = None
  return testData

def testError_inUseGlobalSubsample( testData ):
  __provideInput( testData )
  checksToExecute = (
    AMChecks.EqualCheck( fieldName = "{}.statusCode".format( MODULE_NAME ),
                         value     = StatusCode.ERROR_INPUT_PARAMETER ),
  )
  AMTests.testFailedUpdate( MODULE_NAME, checksToExecute )

#----------------------------------------------------------------------------------

def ITERATIVETEST_Error_Of_Input_Parameter_inGlobalSubsample():
  """Tests occurrence of an error because of an invalid input value provided at field "inGlobalSubsample"."""
  Logging.infoHTML( "<b>ToDo: Add more \"TestData\" objects to maximize variations!</b>" )
  testDataVariations = {
    "Invalid_Variation_1" : __getInvalidTestDataVariation1_inGlobalSubsample(),
    "Invalid_Variation_2" : __getInvalidTestDataVariation2_inGlobalSubsample(),
  }
  return ( testDataVariations, testError_inGlobalSubsample )

def __getInvalidTestDataVariation1_inGlobalSubsample():
  Logging.infoHTML( "<b>ToDo: Rename function that the name matches the intention of returned test data!</b>" )
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with a reliable invalid value of \"inGlobalSubsample\"!</b>" )
  testData.inGlobalSubsample = None
  return testData

def __getInvalidTestDataVariation2_inGlobalSubsample():
  Logging.infoHTML( "<b>ToDo: Rename function that the name matches the intention of returned test data!</b>" )
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with a reliable invalid value of \"inGlobalSubsample\"!</b>" )
  testData.inGlobalSubsample = None
  return testData

def testError_inGlobalSubsample( testData ):
  __provideInput( testData )
  checksToExecute = (
    AMChecks.EqualCheck( fieldName = "{}.statusCode".format( MODULE_NAME ),
                         value     = StatusCode.ERROR_INPUT_PARAMETER ),
  )
  AMTests.testFailedUpdate( MODULE_NAME, checksToExecute )

#----------------------------------------------------------------------------------

def ITERATIVETEST_Error_Of_Input_Parameter_inXSubsample():
  """Tests occurrence of an error because of an invalid input value provided at field "inXSubsample"."""
  Logging.infoHTML( "<b>ToDo: Add more \"TestData\" objects to maximize variations!</b>" )
  testDataVariations = {
    "Invalid_Variation_1" : __getInvalidTestDataVariation1_inXSubsample(),
    "Invalid_Variation_2" : __getInvalidTestDataVariation2_inXSubsample(),
  }
  return ( testDataVariations, testError_inXSubsample )

def __getInvalidTestDataVariation1_inXSubsample():
  Logging.infoHTML( "<b>ToDo: Rename function that the name matches the intention of returned test data!</b>" )
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with a reliable invalid value of \"inXSubsample\"!</b>" )
  testData.inXSubsample = None
  return testData

def __getInvalidTestDataVariation2_inXSubsample():
  Logging.infoHTML( "<b>ToDo: Rename function that the name matches the intention of returned test data!</b>" )
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with a reliable invalid value of \"inXSubsample\"!</b>" )
  testData.inXSubsample = None
  return testData

def testError_inXSubsample( testData ):
  __provideInput( testData )
  checksToExecute = (
    AMChecks.EqualCheck( fieldName = "{}.statusCode".format( MODULE_NAME ),
                         value     = StatusCode.ERROR_INPUT_PARAMETER ),
  )
  AMTests.testFailedUpdate( MODULE_NAME, checksToExecute )

#----------------------------------------------------------------------------------

def ITERATIVETEST_Error_Of_Input_Parameter_inYSubsample():
  """Tests occurrence of an error because of an invalid input value provided at field "inYSubsample"."""
  Logging.infoHTML( "<b>ToDo: Add more \"TestData\" objects to maximize variations!</b>" )
  testDataVariations = {
    "Invalid_Variation_1" : __getInvalidTestDataVariation1_inYSubsample(),
    "Invalid_Variation_2" : __getInvalidTestDataVariation2_inYSubsample(),
  }
  return ( testDataVariations, testError_inYSubsample )

def __getInvalidTestDataVariation1_inYSubsample():
  Logging.infoHTML( "<b>ToDo: Rename function that the name matches the intention of returned test data!</b>" )
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with a reliable invalid value of \"inYSubsample\"!</b>" )
  testData.inYSubsample = None
  return testData

def __getInvalidTestDataVariation2_inYSubsample():
  Logging.infoHTML( "<b>ToDo: Rename function that the name matches the intention of returned test data!</b>" )
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with a reliable invalid value of \"inYSubsample\"!</b>" )
  testData.inYSubsample = None
  return testData

def testError_inYSubsample( testData ):
  __provideInput( testData )
  checksToExecute = (
    AMChecks.EqualCheck( fieldName = "{}.statusCode".format( MODULE_NAME ),
                         value     = StatusCode.ERROR_INPUT_PARAMETER ),
  )
  AMTests.testFailedUpdate( MODULE_NAME, checksToExecute )

#----------------------------------------------------------------------------------

def ITERATIVETEST_Error_Of_Input_Parameter_inZSubsample():
  """Tests occurrence of an error because of an invalid input value provided at field "inZSubsample"."""
  Logging.infoHTML( "<b>ToDo: Add more \"TestData\" objects to maximize variations!</b>" )
  testDataVariations = {
    "Invalid_Variation_1" : __getInvalidTestDataVariation1_inZSubsample(),
    "Invalid_Variation_2" : __getInvalidTestDataVariation2_inZSubsample(),
  }
  return ( testDataVariations, testError_inZSubsample )

def __getInvalidTestDataVariation1_inZSubsample():
  Logging.infoHTML( "<b>ToDo: Rename function that the name matches the intention of returned test data!</b>" )
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with a reliable invalid value of \"inZSubsample\"!</b>" )
  testData.inZSubsample = None
  return testData

def __getInvalidTestDataVariation2_inZSubsample():
  Logging.infoHTML( "<b>ToDo: Rename function that the name matches the intention of returned test data!</b>" )
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with a reliable invalid value of \"inZSubsample\"!</b>" )
  testData.inZSubsample = None
  return testData

def testError_inZSubsample( testData ):
  __provideInput( testData )
  checksToExecute = (
    AMChecks.EqualCheck( fieldName = "{}.statusCode".format( MODULE_NAME ),
                         value     = StatusCode.ERROR_INPUT_PARAMETER ),
  )
  AMTests.testFailedUpdate( MODULE_NAME, checksToExecute )

#----------------------------------------------------------------------------------

def ITERATIVETEST_Variation_Of_Input_Image_input0():
  """Tests variation of input image connected at field "input0"."""
  Logging.infoHTML( "<b>ToDo: Add more \"TestData\" objects to maximize variations!</b>" )
  testDataVariations = {
    "Variation_1" : __getTestDataVariation1_input0(),
  }
  return ( testDataVariations, executeTestOfUpdate )

def __getTestDataVariation1_input0():
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with reliable variation values!</b>" )
  testData.input0Path = None
  testData.output0ReferencePath = None
  return testData

#----------------------------------------------------------------------------------

def ITERATIVETEST_Variation_Of_Input_Base_inMidsagittalSurface():
  """Tests variation of input Base object connected at field "inMidsagittalSurface"."""
  Logging.infoHTML( "<b>ToDo: Add more \"TestData\" objects to maximize variations!</b>" )
  testDataVariations = {
    "Variation_1" : __getTestDataVariation1_inMidsagittalSurface(),
  }
  return ( testDataVariations, executeTestOfUpdate )

def __getTestDataVariation1_inMidsagittalSurface():
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with reliable variation values!</b>" )
  testData.inMidsagittalSurfacePath = None
  testData.output0ReferencePath = None
  return testData

#----------------------------------------------------------------------------------

def ITERATIVETEST_Variation_Of_Input_Parameter_inIntersectionMode():
  """Tests variation of input value at field "inIntersectionMode"."""
  Logging.infoHTML( "<b>ToDo: Add more \"TestData\" objects to maximize variations!</b>" )
  testDataVariations = {
    "Variation_1" : __getTestDataVariation1_inIntersectionMode(),
  }
  return ( testDataVariations, executeTestOfUpdate )

def __getTestDataVariation1_inIntersectionMode():
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with reliable variation values!</b>" )
  testData.inIntersectionMode = None
  testData.output0ReferencePath = None
  return testData

#----------------------------------------------------------------------------------

def ITERATIVETEST_Variation_Of_Input_Parameter_inVolumeThreshold():
  """Tests variation of input value at field "inVolumeThreshold"."""
  Logging.infoHTML( "<b>ToDo: Add more \"TestData\" objects to maximize variations!</b>" )
  testDataVariations = {
    "Variation_1" : __getTestDataVariation1_inVolumeThreshold(),
  }
  return ( testDataVariations, executeTestOfUpdate )

def __getTestDataVariation1_inVolumeThreshold():
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with reliable variation values!</b>" )
  testData.inVolumeThreshold = None
  testData.output0ReferencePath = None
  return testData

#----------------------------------------------------------------------------------

def ITERATIVETEST_Variation_Of_Input_Parameter_inUseGlobalSubsample():
  """Tests variation of input value at field "inUseGlobalSubsample"."""
  Logging.infoHTML( "<b>ToDo: Add more \"TestData\" objects to maximize variations!</b>" )
  testDataVariations = {
    "Variation_1" : __getTestDataVariation1_inUseGlobalSubsample(),
  }
  return ( testDataVariations, executeTestOfUpdate )

def __getTestDataVariation1_inUseGlobalSubsample():
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with reliable variation values!</b>" )
  testData.inUseGlobalSubsample = None
  testData.output0ReferencePath = None
  return testData

#----------------------------------------------------------------------------------

def ITERATIVETEST_Variation_Of_Input_Parameter_inGlobalSubsample():
  """Tests variation of input value at field "inGlobalSubsample"."""
  Logging.infoHTML( "<b>ToDo: Add more \"TestData\" objects to maximize variations!</b>" )
  testDataVariations = {
    "Variation_1" : __getTestDataVariation1_inGlobalSubsample(),
  }
  return ( testDataVariations, executeTestOfUpdate )

def __getTestDataVariation1_inGlobalSubsample():
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with reliable variation values!</b>" )
  testData.inGlobalSubsample = None
  testData.output0ReferencePath = None
  return testData

#----------------------------------------------------------------------------------

def ITERATIVETEST_Variation_Of_Input_Parameter_inXSubsample():
  """Tests variation of input value at field "inXSubsample"."""
  Logging.infoHTML( "<b>ToDo: Add more \"TestData\" objects to maximize variations!</b>" )
  testDataVariations = {
    "Variation_1" : __getTestDataVariation1_inXSubsample(),
  }
  return ( testDataVariations, executeTestOfUpdate )

def __getTestDataVariation1_inXSubsample():
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with reliable variation values!</b>" )
  testData.inXSubsample = None
  testData.output0ReferencePath = None
  return testData

#----------------------------------------------------------------------------------

def ITERATIVETEST_Variation_Of_Input_Parameter_inYSubsample():
  """Tests variation of input value at field "inYSubsample"."""
  Logging.infoHTML( "<b>ToDo: Add more \"TestData\" objects to maximize variations!</b>" )
  testDataVariations = {
    "Variation_1" : __getTestDataVariation1_inYSubsample(),
  }
  return ( testDataVariations, executeTestOfUpdate )

def __getTestDataVariation1_inYSubsample():
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with reliable variation values!</b>" )
  testData.inYSubsample = None
  testData.output0ReferencePath = None
  return testData

#----------------------------------------------------------------------------------

def ITERATIVETEST_Variation_Of_Input_Parameter_inZSubsample():
  """Tests variation of input value at field "inZSubsample"."""
  Logging.infoHTML( "<b>ToDo: Add more \"TestData\" objects to maximize variations!</b>" )
  testDataVariations = {
    "Variation_1" : __getTestDataVariation1_inZSubsample(),
  }
  return ( testDataVariations, executeTestOfUpdate )

def __getTestDataVariation1_inZSubsample():
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with reliable variation values!</b>" )
  testData.inZSubsample = None
  testData.output0ReferencePath = None
  return testData

#----------------------------------------------------------------------------------

def TEST_Update_And_Clear():
  """Tests module's update and clear functionality."""
  executeTestOfUpdate( __getDefaultTestData())
  AMTests.testClear( MODULE_NAME, checkList = __getChecksToValidateOutputAfterClear())

def __getChecksToValidateOutputAfterClear():
  checks = (
  )
  return checks

#----------------------------------------------------------------------------------
