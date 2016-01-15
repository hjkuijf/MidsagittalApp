# **InsertLicense** code

from mevis import *
from TestSupport import Base, Fields, Logging, ScreenShot
from TestSupport.Macros import *
from AlgorithmModuleTestSupport import Checks as AMChecks
from AlgorithmModuleTestSupport import Tests as AMTests
from AlgorithmModule.Definitions import StatusCode

#----------------------------------------------------------------------------------

MODULE_NAME = "MidsagittalPlane"

class TestData( object ):
  """Contains a setting of input and corresponding reference output field values."""
  def __init__( self ):
    self.input0Path = None
    self.inBins = None
    self.output0ReferencePath = None
    self.outPlane = None
    self.outCMSPx = None
    self.outLog = None

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

def __loadFileWithLocalModule( moduleName, filePath ):
  if ( filePath != None ):
    Fields.setValue( "{}.name".format( moduleName ), filePath )
    Fields.touch( "{}.load".format( moduleName ))
  else:
    Fields.touch( "{}.close".format( moduleName ))

def __setUpInputParameters( testData ):
  Fields.setValue( "{}.inBins".format( MODULE_NAME ), testData.inBins )

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
    AMChecks.FloatEqualCheck( fieldName = "{}.outPlane".format( MODULE_NAME ),
                              value = testData.outPlane,
                              epsilon = 0.0 ),
    AMChecks.EqualCheck( fieldName = "{}.outCMSPx".format( MODULE_NAME ),
                         value = testData.outCMSPx ),
    AMChecks.EqualCheck( fieldName = "{}.outLog".format( MODULE_NAME ),
                         value = testData.outLog ),
  )
  Logging.infoHTML( "<b>ToDo: Set up epsilon value of \"FloatEqualCheck\" objects!</b>" )
  return checks

def __getDefaultTestData():
  """Returns a default and valid TestData object."""
  Logging.infoHTML( "<b>ToDo: Set valid values for default TestData object!</b>" )
  testData = TestData()
  testData.input0Path = None
  testData.inBins = None
  testData.output0ReferencePath = None
  testData.outPlane = None
  testData.outCMSPx = None
  testData.outLog = None
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
  testData.outPlane = None
  testData.outCMSPx = None
  testData.outLog = None
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
  testData.outPlane = None
  testData.outCMSPx = None
  testData.outLog = None
  return testData

#----------------------------------------------------------------------------------

def TEST_Update_And_Clear():
  """Tests module's update and clear functionality."""
  executeTestOfUpdate( __getDefaultTestData())
  AMTests.testClear( MODULE_NAME, checkList = __getChecksToValidateOutputAfterClear())

def __getChecksToValidateOutputAfterClear():
  checks = (
    AMChecks.EqualCheck( fieldName = "{}.outPlane".format( MODULE_NAME ),
                         value = None ),
    AMChecks.EqualCheck( fieldName = "{}.outCMSPx".format( MODULE_NAME ),
                         value = None ),
    AMChecks.EqualCheck( fieldName = "{}.outLog".format( MODULE_NAME ),
                         value = None ),
  )
  return checks

#----------------------------------------------------------------------------------
