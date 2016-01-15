# **InsertLicense** code

from mevis import *
from TestSupport import Base, Fields, Logging, ScreenShot
from TestSupport.Macros import *
from AlgorithmModuleTestSupport import Checks as AMChecks
from AlgorithmModuleTestSupport import Tests as AMTests
from AlgorithmModule.Definitions import StatusCode

#----------------------------------------------------------------------------------

MODULE_NAME = "MidsagittalSurfaceMacro"

class TestData( object ):
  """Contains a setting of input and corresponding reference output field values."""
  def __init__( self ):
    pass

def executeTestOfUpdate( testData ):
  __provideInput( testData )
  __processInput()
  __validateOutput( testData )

def __provideInput( testData ):
  pass

def __processInput():
  AMTests.testUpdate( MODULE_NAME )

def __validateOutput( testData ):
  __setUpNetworkWithOutputReference( testData )
  __executeChecksToValidateOutput( testData )

def __setUpNetworkWithOutputReference( testData ):
  pass

def __executeChecksToValidateOutput( testData ):
  checkList = __getChecksToValidateOutputAfterUpdate( testData )
  for check in checkList:
    check.execute()

def __getChecksToValidateOutputAfterUpdate( testData ):
  checks = (
  )
  return checks

def __getDefaultTestData():
  """Returns a default and valid TestData object."""
  Logging.infoHTML( "<b>ToDo: Set valid values for default TestData object!</b>" )
  testData = TestData()
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
