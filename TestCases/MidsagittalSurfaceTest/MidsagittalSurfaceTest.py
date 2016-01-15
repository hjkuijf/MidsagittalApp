# **InsertLicense** code

from mevis import *
from TestSupport import Base, Fields, Logging, ScreenShot
from TestSupport.Macros import *
from AlgorithmModuleTestSupport import Checks as AMChecks
from AlgorithmModuleTestSupport import Tests as AMTests
from AlgorithmModule.Definitions import StatusCode

#----------------------------------------------------------------------------------

MODULE_NAME = "MidsagittalSurface"

class TestData( object ):
  """Contains a setting of input and corresponding reference output field values."""
  def __init__( self ):
    self.input0Path = None
    self.input1Path = None
    self.inPlane1 = None
    self.inPlane2 = None
    self.inUseSecondPlane = None
    self.inCMSPx = None
    self.inBins = None
    self.inGridSize = None
    self.inMaxSize = None
    self.inDelta = None
    self.inEps = None
    self.inOptimizer = None
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
  __loadFileWithLocalModule( "Load_input1", testData.input1Path )

def __loadFileWithLocalModule( moduleName, filePath ):
  if ( filePath != None ):
    Fields.setValue( "{}.name".format( moduleName ), filePath )
    Fields.touch( "{}.load".format( moduleName ))
  else:
    Fields.touch( "{}.close".format( moduleName ))

def __setUpInputParameters( testData ):
  Fields.setValue( "{}.inPlane1".format( MODULE_NAME ), testData.inPlane1 )
  Fields.setValue( "{}.inPlane2".format( MODULE_NAME ), testData.inPlane2 )
  Fields.setValue( "{}.inUseSecondPlane".format( MODULE_NAME ), testData.inUseSecondPlane )
  Fields.setValue( "{}.inCMSPx".format( MODULE_NAME ), testData.inCMSPx )
  Fields.setValue( "{}.inBins".format( MODULE_NAME ), testData.inBins )
  Fields.setValue( "{}.inGridSize".format( MODULE_NAME ), testData.inGridSize )
  Fields.setValue( "{}.inMaxSize".format( MODULE_NAME ), testData.inMaxSize )
  Fields.setValue( "{}.inDelta".format( MODULE_NAME ), testData.inDelta )
  Fields.setValue( "{}.inEps".format( MODULE_NAME ), testData.inEps )
  Fields.setValue( "{}.inOptimizer".format( MODULE_NAME ), testData.inOptimizer )

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
  testData.input1Path = None
  testData.inPlane1 = None
  testData.inPlane2 = None
  testData.inUseSecondPlane = None
  testData.inCMSPx = None
  testData.inBins = None
  testData.inGridSize = None
  testData.inMaxSize = None
  testData.inDelta = None
  testData.inEps = None
  testData.inOptimizer = None
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

def ITERATIVETEST_Error_Of_Input_Image_input1():
  """Tests occurrence of an error because of an invalid image provided at field "input1"."""
  Logging.infoHTML( "<b>ToDo: Add more \"TestData\" objects to maximize variations!</b>" )
  testDataVariations = {
    "Missing_Object" : __getMissingObjectTestData_input1(),
  }
  return ( testDataVariations, testError_input1 )

def __getMissingObjectTestData_input1():
  testData = __getDefaultTestData()
  # Note: None as path value prevents loading.
  testData.input1Path = None
  return testData

def testError_input1( testData ):
  __provideInput( testData )
  checksToExecute = (
    AMChecks.EqualCheck( fieldName = "{}.statusCode".format( MODULE_NAME ),
                         value     = StatusCode.ERROR_INPUT_OBJECT ),
  )
  AMTests.testFailedUpdate( MODULE_NAME, checksToExecute )

#----------------------------------------------------------------------------------

def ITERATIVETEST_Error_Of_Input_Parameter_inPlane1():
  """Tests occurrence of an error because of an invalid input value provided at field "inPlane1"."""
  Logging.infoHTML( "<b>ToDo: Add more \"TestData\" objects to maximize variations!</b>" )
  testDataVariations = {
    "Invalid_Variation_1" : __getInvalidTestDataVariation1_inPlane1(),
    "Invalid_Variation_2" : __getInvalidTestDataVariation2_inPlane1(),
  }
  return ( testDataVariations, testError_inPlane1 )

def __getInvalidTestDataVariation1_inPlane1():
  Logging.infoHTML( "<b>ToDo: Rename function that the name matches the intention of returned test data!</b>" )
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with a reliable invalid value of \"inPlane1\"!</b>" )
  testData.inPlane1 = None
  return testData

def __getInvalidTestDataVariation2_inPlane1():
  Logging.infoHTML( "<b>ToDo: Rename function that the name matches the intention of returned test data!</b>" )
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with a reliable invalid value of \"inPlane1\"!</b>" )
  testData.inPlane1 = None
  return testData

def testError_inPlane1( testData ):
  __provideInput( testData )
  checksToExecute = (
    AMChecks.EqualCheck( fieldName = "{}.statusCode".format( MODULE_NAME ),
                         value     = StatusCode.ERROR_INPUT_PARAMETER ),
  )
  AMTests.testFailedUpdate( MODULE_NAME, checksToExecute )

#----------------------------------------------------------------------------------

def ITERATIVETEST_Error_Of_Input_Parameter_inPlane2():
  """Tests occurrence of an error because of an invalid input value provided at field "inPlane2"."""
  Logging.infoHTML( "<b>ToDo: Add more \"TestData\" objects to maximize variations!</b>" )
  testDataVariations = {
    "Invalid_Variation_1" : __getInvalidTestDataVariation1_inPlane2(),
    "Invalid_Variation_2" : __getInvalidTestDataVariation2_inPlane2(),
  }
  return ( testDataVariations, testError_inPlane2 )

def __getInvalidTestDataVariation1_inPlane2():
  Logging.infoHTML( "<b>ToDo: Rename function that the name matches the intention of returned test data!</b>" )
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with a reliable invalid value of \"inPlane2\"!</b>" )
  testData.inPlane2 = None
  return testData

def __getInvalidTestDataVariation2_inPlane2():
  Logging.infoHTML( "<b>ToDo: Rename function that the name matches the intention of returned test data!</b>" )
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with a reliable invalid value of \"inPlane2\"!</b>" )
  testData.inPlane2 = None
  return testData

def testError_inPlane2( testData ):
  __provideInput( testData )
  checksToExecute = (
    AMChecks.EqualCheck( fieldName = "{}.statusCode".format( MODULE_NAME ),
                         value     = StatusCode.ERROR_INPUT_PARAMETER ),
  )
  AMTests.testFailedUpdate( MODULE_NAME, checksToExecute )

#----------------------------------------------------------------------------------

def ITERATIVETEST_Error_Of_Input_Parameter_inUseSecondPlane():
  """Tests occurrence of an error because of an invalid input value provided at field "inUseSecondPlane"."""
  Logging.infoHTML( "<b>ToDo: Add more \"TestData\" objects to maximize variations!</b>" )
  testDataVariations = {
    "Invalid_Variation_1" : __getInvalidTestDataVariation1_inUseSecondPlane(),
    "Invalid_Variation_2" : __getInvalidTestDataVariation2_inUseSecondPlane(),
  }
  return ( testDataVariations, testError_inUseSecondPlane )

def __getInvalidTestDataVariation1_inUseSecondPlane():
  Logging.infoHTML( "<b>ToDo: Rename function that the name matches the intention of returned test data!</b>" )
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with a reliable invalid value of \"inUseSecondPlane\"!</b>" )
  testData.inUseSecondPlane = None
  return testData

def __getInvalidTestDataVariation2_inUseSecondPlane():
  Logging.infoHTML( "<b>ToDo: Rename function that the name matches the intention of returned test data!</b>" )
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with a reliable invalid value of \"inUseSecondPlane\"!</b>" )
  testData.inUseSecondPlane = None
  return testData

def testError_inUseSecondPlane( testData ):
  __provideInput( testData )
  checksToExecute = (
    AMChecks.EqualCheck( fieldName = "{}.statusCode".format( MODULE_NAME ),
                         value     = StatusCode.ERROR_INPUT_PARAMETER ),
  )
  AMTests.testFailedUpdate( MODULE_NAME, checksToExecute )

#----------------------------------------------------------------------------------

def ITERATIVETEST_Error_Of_Input_Parameter_inCMSPx():
  """Tests occurrence of an error because of an invalid input value provided at field "inCMSPx"."""
  Logging.infoHTML( "<b>ToDo: Add more \"TestData\" objects to maximize variations!</b>" )
  testDataVariations = {
    "Invalid_Variation_1" : __getInvalidTestDataVariation1_inCMSPx(),
    "Invalid_Variation_2" : __getInvalidTestDataVariation2_inCMSPx(),
  }
  return ( testDataVariations, testError_inCMSPx )

def __getInvalidTestDataVariation1_inCMSPx():
  Logging.infoHTML( "<b>ToDo: Rename function that the name matches the intention of returned test data!</b>" )
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with a reliable invalid value of \"inCMSPx\"!</b>" )
  testData.inCMSPx = None
  return testData

def __getInvalidTestDataVariation2_inCMSPx():
  Logging.infoHTML( "<b>ToDo: Rename function that the name matches the intention of returned test data!</b>" )
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with a reliable invalid value of \"inCMSPx\"!</b>" )
  testData.inCMSPx = None
  return testData

def testError_inCMSPx( testData ):
  __provideInput( testData )
  checksToExecute = (
    AMChecks.EqualCheck( fieldName = "{}.statusCode".format( MODULE_NAME ),
                         value     = StatusCode.ERROR_INPUT_PARAMETER ),
  )
  AMTests.testFailedUpdate( MODULE_NAME, checksToExecute )

#----------------------------------------------------------------------------------

def ITERATIVETEST_Error_Of_Input_Parameter_inBins():
  """Tests occurrence of an error because of an invalid input value provided at field "inBins"."""
  Logging.infoHTML( "<b>ToDo: Add more \"TestData\" objects to maximize variations!</b>" )
  testDataVariations = {
    "Invalid_Variation_1" : __getInvalidTestDataVariation1_inBins(),
    "Invalid_Variation_2" : __getInvalidTestDataVariation2_inBins(),
  }
  return ( testDataVariations, testError_inBins )

def __getInvalidTestDataVariation1_inBins():
  Logging.infoHTML( "<b>ToDo: Rename function that the name matches the intention of returned test data!</b>" )
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with a reliable invalid value of \"inBins\"!</b>" )
  testData.inBins = None
  return testData

def __getInvalidTestDataVariation2_inBins():
  Logging.infoHTML( "<b>ToDo: Rename function that the name matches the intention of returned test data!</b>" )
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with a reliable invalid value of \"inBins\"!</b>" )
  testData.inBins = None
  return testData

def testError_inBins( testData ):
  __provideInput( testData )
  checksToExecute = (
    AMChecks.EqualCheck( fieldName = "{}.statusCode".format( MODULE_NAME ),
                         value     = StatusCode.ERROR_INPUT_PARAMETER ),
  )
  AMTests.testFailedUpdate( MODULE_NAME, checksToExecute )

#----------------------------------------------------------------------------------

def ITERATIVETEST_Error_Of_Input_Parameter_inGridSize():
  """Tests occurrence of an error because of an invalid input value provided at field "inGridSize"."""
  Logging.infoHTML( "<b>ToDo: Add more \"TestData\" objects to maximize variations!</b>" )
  testDataVariations = {
    "Invalid_Variation_1" : __getInvalidTestDataVariation1_inGridSize(),
    "Invalid_Variation_2" : __getInvalidTestDataVariation2_inGridSize(),
  }
  return ( testDataVariations, testError_inGridSize )

def __getInvalidTestDataVariation1_inGridSize():
  Logging.infoHTML( "<b>ToDo: Rename function that the name matches the intention of returned test data!</b>" )
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with a reliable invalid value of \"inGridSize\"!</b>" )
  testData.inGridSize = None
  return testData

def __getInvalidTestDataVariation2_inGridSize():
  Logging.infoHTML( "<b>ToDo: Rename function that the name matches the intention of returned test data!</b>" )
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with a reliable invalid value of \"inGridSize\"!</b>" )
  testData.inGridSize = None
  return testData

def testError_inGridSize( testData ):
  __provideInput( testData )
  checksToExecute = (
    AMChecks.EqualCheck( fieldName = "{}.statusCode".format( MODULE_NAME ),
                         value     = StatusCode.ERROR_INPUT_PARAMETER ),
  )
  AMTests.testFailedUpdate( MODULE_NAME, checksToExecute )

#----------------------------------------------------------------------------------

def ITERATIVETEST_Error_Of_Input_Parameter_inMaxSize():
  """Tests occurrence of an error because of an invalid input value provided at field "inMaxSize"."""
  Logging.infoHTML( "<b>ToDo: Add more \"TestData\" objects to maximize variations!</b>" )
  testDataVariations = {
    "Invalid_Variation_1" : __getInvalidTestDataVariation1_inMaxSize(),
    "Invalid_Variation_2" : __getInvalidTestDataVariation2_inMaxSize(),
  }
  return ( testDataVariations, testError_inMaxSize )

def __getInvalidTestDataVariation1_inMaxSize():
  Logging.infoHTML( "<b>ToDo: Rename function that the name matches the intention of returned test data!</b>" )
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with a reliable invalid value of \"inMaxSize\"!</b>" )
  testData.inMaxSize = None
  return testData

def __getInvalidTestDataVariation2_inMaxSize():
  Logging.infoHTML( "<b>ToDo: Rename function that the name matches the intention of returned test data!</b>" )
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with a reliable invalid value of \"inMaxSize\"!</b>" )
  testData.inMaxSize = None
  return testData

def testError_inMaxSize( testData ):
  __provideInput( testData )
  checksToExecute = (
    AMChecks.EqualCheck( fieldName = "{}.statusCode".format( MODULE_NAME ),
                         value     = StatusCode.ERROR_INPUT_PARAMETER ),
  )
  AMTests.testFailedUpdate( MODULE_NAME, checksToExecute )

#----------------------------------------------------------------------------------

def ITERATIVETEST_Error_Of_Input_Parameter_inDelta():
  """Tests occurrence of an error because of an invalid input value provided at field "inDelta"."""
  Logging.infoHTML( "<b>ToDo: Add more \"TestData\" objects to maximize variations!</b>" )
  testDataVariations = {
    "Invalid_Variation_1" : __getInvalidTestDataVariation1_inDelta(),
    "Invalid_Variation_2" : __getInvalidTestDataVariation2_inDelta(),
  }
  return ( testDataVariations, testError_inDelta )

def __getInvalidTestDataVariation1_inDelta():
  Logging.infoHTML( "<b>ToDo: Rename function that the name matches the intention of returned test data!</b>" )
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with a reliable invalid value of \"inDelta\"!</b>" )
  testData.inDelta = None
  return testData

def __getInvalidTestDataVariation2_inDelta():
  Logging.infoHTML( "<b>ToDo: Rename function that the name matches the intention of returned test data!</b>" )
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with a reliable invalid value of \"inDelta\"!</b>" )
  testData.inDelta = None
  return testData

def testError_inDelta( testData ):
  __provideInput( testData )
  checksToExecute = (
    AMChecks.EqualCheck( fieldName = "{}.statusCode".format( MODULE_NAME ),
                         value     = StatusCode.ERROR_INPUT_PARAMETER ),
  )
  AMTests.testFailedUpdate( MODULE_NAME, checksToExecute )

#----------------------------------------------------------------------------------

def ITERATIVETEST_Error_Of_Input_Parameter_inEps():
  """Tests occurrence of an error because of an invalid input value provided at field "inEps"."""
  Logging.infoHTML( "<b>ToDo: Add more \"TestData\" objects to maximize variations!</b>" )
  testDataVariations = {
    "Invalid_Variation_1" : __getInvalidTestDataVariation1_inEps(),
    "Invalid_Variation_2" : __getInvalidTestDataVariation2_inEps(),
  }
  return ( testDataVariations, testError_inEps )

def __getInvalidTestDataVariation1_inEps():
  Logging.infoHTML( "<b>ToDo: Rename function that the name matches the intention of returned test data!</b>" )
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with a reliable invalid value of \"inEps\"!</b>" )
  testData.inEps = None
  return testData

def __getInvalidTestDataVariation2_inEps():
  Logging.infoHTML( "<b>ToDo: Rename function that the name matches the intention of returned test data!</b>" )
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with a reliable invalid value of \"inEps\"!</b>" )
  testData.inEps = None
  return testData

def testError_inEps( testData ):
  __provideInput( testData )
  checksToExecute = (
    AMChecks.EqualCheck( fieldName = "{}.statusCode".format( MODULE_NAME ),
                         value     = StatusCode.ERROR_INPUT_PARAMETER ),
  )
  AMTests.testFailedUpdate( MODULE_NAME, checksToExecute )

#----------------------------------------------------------------------------------

def ITERATIVETEST_Error_Of_Input_Parameter_inOptimizer():
  """Tests occurrence of an error because of an invalid input value provided at field "inOptimizer"."""
  Logging.infoHTML( "<b>ToDo: Add more \"TestData\" objects to maximize variations!</b>" )
  testDataVariations = {
    "Invalid_Variation_1" : __getInvalidTestDataVariation1_inOptimizer(),
    "Invalid_Variation_2" : __getInvalidTestDataVariation2_inOptimizer(),
  }
  return ( testDataVariations, testError_inOptimizer )

def __getInvalidTestDataVariation1_inOptimizer():
  Logging.infoHTML( "<b>ToDo: Rename function that the name matches the intention of returned test data!</b>" )
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with a reliable invalid value of \"inOptimizer\"!</b>" )
  testData.inOptimizer = None
  return testData

def __getInvalidTestDataVariation2_inOptimizer():
  Logging.infoHTML( "<b>ToDo: Rename function that the name matches the intention of returned test data!</b>" )
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with a reliable invalid value of \"inOptimizer\"!</b>" )
  testData.inOptimizer = None
  return testData

def testError_inOptimizer( testData ):
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

def ITERATIVETEST_Variation_Of_Input_Image_input1():
  """Tests variation of input image connected at field "input1"."""
  Logging.infoHTML( "<b>ToDo: Add more \"TestData\" objects to maximize variations!</b>" )
  testDataVariations = {
    "Variation_1" : __getTestDataVariation1_input1(),
  }
  return ( testDataVariations, executeTestOfUpdate )

def __getTestDataVariation1_input1():
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with reliable variation values!</b>" )
  testData.input1Path = None
  testData.output0ReferencePath = None
  return testData

#----------------------------------------------------------------------------------

def ITERATIVETEST_Variation_Of_Input_Parameter_inPlane1():
  """Tests variation of input value at field "inPlane1"."""
  Logging.infoHTML( "<b>ToDo: Add more \"TestData\" objects to maximize variations!</b>" )
  testDataVariations = {
    "Variation_1" : __getTestDataVariation1_inPlane1(),
  }
  return ( testDataVariations, executeTestOfUpdate )

def __getTestDataVariation1_inPlane1():
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with reliable variation values!</b>" )
  testData.inPlane1 = None
  testData.output0ReferencePath = None
  return testData

#----------------------------------------------------------------------------------

def ITERATIVETEST_Variation_Of_Input_Parameter_inPlane2():
  """Tests variation of input value at field "inPlane2"."""
  Logging.infoHTML( "<b>ToDo: Add more \"TestData\" objects to maximize variations!</b>" )
  testDataVariations = {
    "Variation_1" : __getTestDataVariation1_inPlane2(),
  }
  return ( testDataVariations, executeTestOfUpdate )

def __getTestDataVariation1_inPlane2():
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with reliable variation values!</b>" )
  testData.inPlane2 = None
  testData.output0ReferencePath = None
  return testData

#----------------------------------------------------------------------------------

def ITERATIVETEST_Variation_Of_Input_Parameter_inUseSecondPlane():
  """Tests variation of input value at field "inUseSecondPlane"."""
  Logging.infoHTML( "<b>ToDo: Add more \"TestData\" objects to maximize variations!</b>" )
  testDataVariations = {
    "Variation_1" : __getTestDataVariation1_inUseSecondPlane(),
  }
  return ( testDataVariations, executeTestOfUpdate )

def __getTestDataVariation1_inUseSecondPlane():
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with reliable variation values!</b>" )
  testData.inUseSecondPlane = None
  testData.output0ReferencePath = None
  return testData

#----------------------------------------------------------------------------------

def ITERATIVETEST_Variation_Of_Input_Parameter_inCMSPx():
  """Tests variation of input value at field "inCMSPx"."""
  Logging.infoHTML( "<b>ToDo: Add more \"TestData\" objects to maximize variations!</b>" )
  testDataVariations = {
    "Variation_1" : __getTestDataVariation1_inCMSPx(),
  }
  return ( testDataVariations, executeTestOfUpdate )

def __getTestDataVariation1_inCMSPx():
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with reliable variation values!</b>" )
  testData.inCMSPx = None
  testData.output0ReferencePath = None
  return testData

#----------------------------------------------------------------------------------

def ITERATIVETEST_Variation_Of_Input_Parameter_inBins():
  """Tests variation of input value at field "inBins"."""
  Logging.infoHTML( "<b>ToDo: Add more \"TestData\" objects to maximize variations!</b>" )
  testDataVariations = {
    "Variation_1" : __getTestDataVariation1_inBins(),
  }
  return ( testDataVariations, executeTestOfUpdate )

def __getTestDataVariation1_inBins():
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with reliable variation values!</b>" )
  testData.inBins = None
  testData.output0ReferencePath = None
  return testData

#----------------------------------------------------------------------------------

def ITERATIVETEST_Variation_Of_Input_Parameter_inGridSize():
  """Tests variation of input value at field "inGridSize"."""
  Logging.infoHTML( "<b>ToDo: Add more \"TestData\" objects to maximize variations!</b>" )
  testDataVariations = {
    "Variation_1" : __getTestDataVariation1_inGridSize(),
  }
  return ( testDataVariations, executeTestOfUpdate )

def __getTestDataVariation1_inGridSize():
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with reliable variation values!</b>" )
  testData.inGridSize = None
  testData.output0ReferencePath = None
  return testData

#----------------------------------------------------------------------------------

def ITERATIVETEST_Variation_Of_Input_Parameter_inMaxSize():
  """Tests variation of input value at field "inMaxSize"."""
  Logging.infoHTML( "<b>ToDo: Add more \"TestData\" objects to maximize variations!</b>" )
  testDataVariations = {
    "Variation_1" : __getTestDataVariation1_inMaxSize(),
  }
  return ( testDataVariations, executeTestOfUpdate )

def __getTestDataVariation1_inMaxSize():
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with reliable variation values!</b>" )
  testData.inMaxSize = None
  testData.output0ReferencePath = None
  return testData

#----------------------------------------------------------------------------------

def ITERATIVETEST_Variation_Of_Input_Parameter_inDelta():
  """Tests variation of input value at field "inDelta"."""
  Logging.infoHTML( "<b>ToDo: Add more \"TestData\" objects to maximize variations!</b>" )
  testDataVariations = {
    "Variation_1" : __getTestDataVariation1_inDelta(),
  }
  return ( testDataVariations, executeTestOfUpdate )

def __getTestDataVariation1_inDelta():
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with reliable variation values!</b>" )
  testData.inDelta = None
  testData.output0ReferencePath = None
  return testData

#----------------------------------------------------------------------------------

def ITERATIVETEST_Variation_Of_Input_Parameter_inEps():
  """Tests variation of input value at field "inEps"."""
  Logging.infoHTML( "<b>ToDo: Add more \"TestData\" objects to maximize variations!</b>" )
  testDataVariations = {
    "Variation_1" : __getTestDataVariation1_inEps(),
  }
  return ( testDataVariations, executeTestOfUpdate )

def __getTestDataVariation1_inEps():
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with reliable variation values!</b>" )
  testData.inEps = None
  testData.output0ReferencePath = None
  return testData

#----------------------------------------------------------------------------------

def ITERATIVETEST_Variation_Of_Input_Parameter_inOptimizer():
  """Tests variation of input value at field "inOptimizer"."""
  Logging.infoHTML( "<b>ToDo: Add more \"TestData\" objects to maximize variations!</b>" )
  testDataVariations = {
    "Variation_1" : __getTestDataVariation1_inOptimizer(),
  }
  return ( testDataVariations, executeTestOfUpdate )

def __getTestDataVariation1_inOptimizer():
  testData = __getDefaultTestData()
  Logging.infoHTML( "<b>ToDo: Replace \"None\" with reliable variation values!</b>" )
  testData.inOptimizer = None
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
