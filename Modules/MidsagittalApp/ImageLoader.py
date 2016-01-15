from mevis import *

def init():
  showOutputs()
  
def showStatus(status):
  ctx.field("status").setStringValue(status)
  
def setNumberOfImages(arg):
  numberOfImages = arg.intValue()
  
  contentString = "TabView { name = tabs "
  
  # Add any missing load modules
  for i in range(0, numberOfImages):
    addImageLoader(i)
    
    contentString += """
      TabViewItem Image"""+ str(i+1) +""" {
        tabSelectedCommand = tabSelected
        Field ImageLoad"""+ str(i) +""".filename {
          browseButton = True
          browseMode = open
          browseSelectedCommand = "*py: loadImage("""+ str(i) +""", args) *"
        }
      }
    """
  
  contentString += "}"
  
  ctx.control("loaders").setContentString(contentString)
  
  showOutputs()


def tabSelected():
  currentTab = ctx.control("tabs").currentTab()
  
  ctx.module("View2D").field("inImage").connectFrom(ctx.module("Switch" + str(currentTab)).field("output0"))

  
def loadImage(id, args):
  imageLoadModule = ctx.module("ImageLoad" + str(id))
  switchModule = ctx.module("Switch" + str(id))
  
  status = imageLoadModule.field("status").stringValue()
  if not "error" in status:
    # Load using ImageLoad
    switchModule.field("currentInput").setIntValue(0)
    showStatus("File " + str(id+1) +" loaded using ImageLoad.")
  else:
    # Try the itkImageFileReader
    itkImageFileReaderModule = ctx.module("itkImageFileReader" + str(id))    
    itkImageFileReaderModule.field("fileName").setStringValue(imageLoadModule.field("filename").stringValue())
    
    status = itkImageFileReaderModule.field("info").stringValue()
    
    if "okay" in status:
      # Load using itkImageFileReader
      # First, check the voxel type and component type. If the used voxel type differs from the component type, try to reload
      fileDataType = itkImageFileReaderModule.field("fileDataType").stringValue()
      componentType = itkImageFileReaderModule.field("componentType").stringValue()
      
      if not fileDataType == componentType:
        itkImageFileReaderModule.field("fileDataType").setStringValue(componentType)
        itkImageFileReaderModule.field("open").touch()
      
      switchModule.field("currentInput").setIntValue(1)
      showStatus("File loaded using itkImageFileReader.")
    else:
      # Loading failed
      showStatus("Loading failed. Filetype unknown.")
  

def addImageLoader(id):
  if ctx.hasModule("Switch" + str(id)):
    return
  
  imageLoadModule = ctx.addModule("ImageLoad")
  imageLoadModule.field("instanceName").setStringValue("ImageLoad" + str(id))
  
  itkImageFileReaderModule = ctx.addModule("itkImageFileReader")
  itkImageFileReaderModule.field("instanceName").setStringValue("itkImageFileReader" + str(id))
  itkImageFileReaderModule.field("correctSubVoxelShift").setBoolValue(True)
  
  switchModule = ctx.addModule("Switch")
  switchModule.field("instanceName").setStringValue("Switch" + str(id))
  switchModule.field("numberOfInputs").setIntValue(2)
  switchModule.field("input0").connectFrom(imageLoadModule.field("output0"))
  switchModule.field("input1").connectFrom(itkImageFileReaderModule.field("output0"))
  
  ctx.field("output" + str(id)).connectFrom(switchModule.field("output0"))


def showOutputs():  
  # Hide unused outputs
  for i in range(0, 16):
    ctx.field("output"+ str(i)).setHidden(i >= ctx.field("numberOfImages").intValue())
    
  ctx.updateLayout()