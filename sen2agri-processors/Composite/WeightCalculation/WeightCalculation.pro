QT       += core

QT       -= gui

TARGET = TestWeightCalculation
CONFIG   += console
CONFIG   -= app_bundle

CONFIG += c++11

TEMPLATE = app

DEFINES += VCL_CAN_STATIC_CONST_INIT_FLOAT=0


SOURCES += \
    TotalWeight/testWeightCalculation.cpp \
    TotalWeight/totalweightcomputation.cpp \
    WeightAOT/weightonaot.cpp \
    WeightOnClouds/cloudmaskbinarization.cpp \
    WeightOnClouds/cloudsinterpolation.cpp \
    WeightOnClouds/cloudweightcomputation.cpp \
    WeightOnClouds/gaussianfilter.cpp \
    ../../MACCSMetadata/src/tinyxml_utils.cpp\
    ../../MACCSMetadata/src/FluentXML.cpp \
    ../../MACCSMetadata/src/MACCSMetadataReader.cpp \
    ../../MACCSMetadata/src/MACCSMetadataWriter.cpp \
    ../../MACCSMetadata/src/SPOT4MetadataReader.cpp \
    ../Common/MetadataHelper.cpp \
    ../Common/Spot4MetadataHelper.cpp \
    ../Common/MetadataHelperFactory.cpp \
    ../Common/MACCSMetadataHelper.cpp

INCLUDEPATH += -I /usr/include -I /usr/local/include -I /usr/local/include/OTB-5.0 -I /usr/local/include/ITK-4.7 -I TotalWeight -I WeightAOT -I WeightOnClouds -I ../../MACCSMetadata/include


LIBS += -L/usr/lib/x86_64-linux-gnu/ -ltinyxml  \
        -L/usr/local/lib -lfftw3f -lfftw3f_threads -lfftw3 -lfftw3_threads -lgdal -lgeos_c -lgeos -lgeotiff -lITKBiasCorrection-4.7 \
        -lITKCommon-4.7 -litkdouble-conversion-4.7 -lITKFFT-4.7 \
        -lITKIOImageBase-4.7 -lITKKLMRegionGrowing-4.7 -lITKLabelMap-4.7 \
        -lITKMesh-4.7 -lITKMetaIO-4.7 -litkNetlibSlatec-4.7 -lITKOptimizers-4.7 \
        -lITKOptimizersv4-4.7 -lITKPath-4.7 -lITKPolynomials-4.7                \
        -lITKQuadEdgeMesh-4.7 -lITKSpatialObjects-4.7 -lITKStatistics-4.7       \
        -litksys-4.7 -litkv3p_lsqr-4.7 -litkv3p_netlib-4.7 -litkvcl-4.7         \
        -litkvnl-4.7 -litkvnl_algo-4.7 -lITKVNLInstantiation-4.7                \
        -lITKWatersheds-4.7 -ljpeg -lkmlbase -lkmlconvenience -lkmldom -lkmlengine -lkmlregionator -lkmlxsd -llibsvm -lminizip -lmuparser       \
        -lmuparserx -lopencv_core -lopencv_ml -lopenjp2 -lOpenThreads -lossim       \
        -lotb6S-5.0 -lOTBApplicationEngine-5.0 -lOTBCarto-5.0                       \
        -lOTBCommandLine-5.0 -lOTBCommandLineParser-5.0 -lOTBCommon-5.0             \
        -lOTBCurlAdapters-5.0 -lOTBEdge-5.0 -lOTBExtendedFilename-5.0               \
        -lOTBFuzzy-5.0 -lOTBGdalAdapters-5.0 -lOTBImageBase-5.0 -lOTBImageIO-5.0    \
        -lOTBImageManipulation-5.0 -lOTBIOBSQ-5.0 -lOTBIOGDAL-5.0                   \
        -lOTBIOKML-5.0 -lOTBIOLUM-5.0 -lOTBIOMSTAR-5.0          \
        -lOTBIOMW-5.0 -lOTBIOONERA-5.0 -lOTBIORAD-5.0 -lOTBIOTileMap-5.0            \
        -lOTBMathParser-5.0 -lOTBMathParserX-5.0 -lOTBMetadata-5.0                  \
        -lOTBOpenThreadsAdapters-5.0 -lOTBOpticalCalibration-5.0                    \
        -lOTBOSSIMAdapters-5.0 -lotbossimplugins-5.0 -lOTBPolarimetry-5.0           \
        -lOTBProjection-5.0 -lOTBQtWidget-5.0 -lOTBRCC8-5.0 -lotbsiftfast-5.0       \
        -lOTBSimulation-5.0 -lOTBStreaming-5.0 -lOTBSupervised-5.0                  \
        -lOTBSVMLearning-5.0 -lOTBTestKernel-5.0 -lOTBTransform-5.0                 \
        -lOTBVectorDataBase-5.0 -lOTBVectorDataIO-5.0                               \
        -lOTBVectorDataRendering-5.0 -lOTBWavelet-5.0 -lproj -lsqlite3 -ltiff -ltiffxx


#-lOTBIOJPEG2000-5.0
#HEADERS +=

HEADERS += \
    TotalWeight/totalweightcomputation.h \
    WeightAOT/weightonaot.h \
    WeightOnClouds/cloudmaskbinarization.h \
    WeightOnClouds/cloudsinterpolation.h \
    WeightOnClouds/cloudweightcomputation.h \
    WeightOnClouds/gaussianfilter.h \
    ../../MACCSMetadata/include/FluentXML.hpp \
    ../../MACCSMetadata/include/MACCSMetadata.hpp \
    ../../MACCSMetadata/include/MACCSMetadataReader.hpp \
    ../../MACCSMetadata/include/MACCSMetadataWriter.hpp \
    ../../MACCSMetadata/include/SPOT4Metadata.hpp \
    ../../MACCSMetadata/include/SPOT4MetadataReader.hpp \
    ../../MACCSMetadata/include/tinyxml_utils.hpp \
    ../Common/MetadataHelper.h \
    ../Common/Spot4MetadataHelper.h \
    ../Common/MetadataHelperFactory.h \
    ../Common/MACCSMetadataHelper.h \
    TotalWeightCalculationFilter.h
