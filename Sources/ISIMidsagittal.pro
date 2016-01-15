TEMPLATE = lib
TARGET = ISIMidsagittal

DESTDIR = ../../../lib
DLLDESTDIR = ../../../lib

# Set high warn level (warn 4 on MSVC)
WARN = MAXIMUM64

# Add used projects here (see included pri files below for available projects).
CONFIG += dll ML MLAlgorithmModule MLTools OpenMP alglib dlib MLBase inventor SoView2D MLOpenGL

MLAB_PACKAGES += FMEstable_ReleaseMeVis

MLAB_PACKAGES += ISI_Neuro \
                 ISI_ThirdParty \
                 MeVisLab_Standard 

# make sure that this file is included after CONFIG and MLAB_PACKAGES
include ($(MLAB_MeVis_Foundation)/Configuration/IncludePackages.pri)

DEFINES += ISIMIDSAGITTAL_EXPORT_SYMBOLS

# Disable deprecated API.
DEFINES += ML_DISABLE_DEPRECATED

HEADERS += \
    alglibSpline2DWrapper.h \
    librarySystem.h \
	functions.h \
    mlMidsagittalPlane.h \
    mlMidsagittalPlaneOutputImageHandler.h \
	mlMidsagittalPlaneSplit.h \
	mlMidsagittalPlaneSplitOutputImageHandler.h \
    mlMidsagittalSurface.h \
	mlMidsagittalSurfaceOptimizer.h \
    mlMidsagittalSurfaceOutputImageHandler.h \
	mlMidsagittalSurfaceSplit.h \
	mlMidsagittalSurfaceSplitOutputImageHandler.h \
	SoView2DMidsagittalSurface.h

SOURCES += \
    alglibSpline2DWrapper.cpp \
    libraryInit.cpp \
	functions.cpp \
    mlMidsagittalPlane.cpp \
    mlMidsagittalPlaneOutputImageHandler.cpp \
	mlMidsagittalPlaneSplit.cpp \
	mlMidsagittalPlaneSplitOutputImageHandler.cpp \
    mlMidsagittalSurface.cpp \
	mlMidsagittalSurfaceOptimizer.cpp \
    mlMidsagittalSurfaceOutputImageHandler.cpp \
	mlMidsagittalSurfaceSplit.cpp \
	mlMidsagittalSurfaceSplitOutputImageHandler.cpp \
	SoView2DMidsagittalSurface.cpp