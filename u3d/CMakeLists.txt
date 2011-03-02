project(U3D)
cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED BUILD_SHARED_LIBS)
    option(BUILD_SHARED_LIBS "Build a shared library form of u3d" OFF)
endif()
if(NOT DEFINED U3D_SHARED)
    option(U3D_SHARED "Build a shared library form of u3d" OFF)
endif()

# add debug definitions
IF( CMAKE_GENERATOR STREQUAL "Unix Makefiles" )
SET(CMAKE_VERBOSE_MAKEFILE ON)
IF( CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo" )
  ADD_DEFINITIONS( -D_DEBUG )
ELSE( CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo" )
  ADD_DEFINITIONS( -DNDEBUG )
ENDIF( CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo" )
ENDIF( CMAKE_GENERATOR STREQUAL "Unix Makefiles" )

IF(APPLE)
# Tiger build
#  SET( CMAKE_OSX_ARCHITECTURES "ppc;i386" )
#  SET( CMAKE_OSX_SYSROOT /Developer/SDKs/MacOSX10.4u.sdk )
#  SET( CMAKE_OSX_DEPLOYMENT_TARGET 10.4 )
#----
# Leopard build
#  SET( CMAKE_OSX_ARCHITECTURES "ppc;i386" )
#  SET( CMAKE_OSX_SYSROOT /Developer/SDKs/MacOSX10.5.sdk )
#  SET( CMAKE_OSX_DEPLOYMENT_TARGET 10.5 )
#----
# Snow Leopard build
#  SET( CMAKE_OSX_ARCHITECTURES "ppc;i386" )
#  SET( CMAKE_OSX_SYSROOT /Developer/SDKs/MacOSX10.6.sdk )
#  SET( CMAKE_OSX_DEPLOYMENT_TARGET 10.6 )
#----
  ADD_DEFINITIONS( -DMAC32 -fno-strict-aliasing )
  SET( U3D_PLATFORM Mac32 )
  SET( CMAKE_CXX_FLAGS_DEBUG "-D_DEBUG" )
ENDIF(APPLE)

IF(WIN32)
  ADD_DEFINITIONS( -DUNICODE -D_UNICODE -D_CRT_SECURE_NO_DEPRECATE )
  SET( U3D_PLATFORM Win32 )
  LINK_LIBRARIES( winmm )
ENDIF(WIN32)

IF(UNIX AND NOT APPLE)
  ADD_DEFINITIONS( -DLIN32 -fno-strict-aliasing )
  SET( U3D_PLATFORM Lin32 )
  SET( CMAKE_CXX_FLAGS_DEBUG "-D_DEBUG" )
ENDIF(UNIX AND NOT APPLE)

IF(U3D_SHARED OR BUILD_SHARED_LIBS)
# check zlib availibility
find_package(ZLIB REQUIRED)
# include_directories(${ZLIB_INCLUDE_DIR})
# IF(NOT BUILD_SHARED_LIBS)
#   set(ADDITIONAL_LIBRARIES ${ZLIB_LIBRARIES})
# ENDIF(NOT BUILD_SHARED_LIBS)

# check png availibility
find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})
add_definitions(${PNG_DEFINITIONS})
set(ADDITIONAL_LIBRARIES ${ADDITIONAL_LIBRARIES} ${PNG_LIBRARIES})

# check jpeg availibility
find_package(JPEG REQUIRED)
include_directories(${JPEG_INCLUDE_DIR})
set(ADDITIONAL_LIBRARIES ${ADDITIONAL_LIBRARIES} ${JPEG_LIBRARIES})

ELSE(U3D_SHARED OR BUILD_SHARED_LIBS)
#============================================================================
# zlib
#============================================================================

include(CheckTypeSize)
include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckCSourceCompiles)

check_include_file(sys/types.h HAVE_SYS_TYPES_H)
check_include_file(stdint.h    HAVE_STDINT_H)
check_include_file(stddef.h    HAVE_STDDEF_H)

#
# Check to see if we have large file support
#
set(CMAKE_REQUIRED_DEFINITIONS -D_LARGEFILE64_SOURCE=1)
check_type_size(off64_t OFF64_T)
if(HAVE_OFF64_T)
   add_definitions(-D_LARGEFILE64_SOURCE=1)
endif()
set(CMAKE_REQUIRED_DEFINITIONS) # clear variable

#
# Check for fseeko
#
check_function_exists(fseeko HAVE_FSEEKO)
if(NOT HAVE_FSEEKO)
    add_definitions(-DNO_FSEEKO)
endif()

#
# Check for unistd.h
#
check_include_file(unistd.h Z_HAVE_UNISTD_H)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
endif()

SET(ZLIB_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/RTL/Dependencies/zlib)
configure_file(${ZLIB_SOURCE_DIR}/zconf.h.cmakein
               ${CMAKE_CURRENT_BINARY_DIR}/zconf.h @ONLY)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${ZLIB_SOURCE_DIR})


set(ZLIB_PUBLIC_HDRS
    ${CMAKE_CURRENT_BINARY_DIR}/zconf.h
    ${ZLIB_SOURCE_DIR}/zlib.h
)
set(ZLIB_PRIVATE_HDRS
    ${ZLIB_SOURCE_DIR}/crc32.h
    ${ZLIB_SOURCE_DIR}/deflate.h
    ${ZLIB_SOURCE_DIR}/gzguts.h
    ${ZLIB_SOURCE_DIR}/inffast.h
    ${ZLIB_SOURCE_DIR}/inffixed.h
    ${ZLIB_SOURCE_DIR}/inflate.h
    ${ZLIB_SOURCE_DIR}/inftrees.h
    ${ZLIB_SOURCE_DIR}/trees.h
    ${ZLIB_SOURCE_DIR}/zutil.h
)
set(ZLIB_SRCS
    ${ZLIB_SOURCE_DIR}/adler32.c
    ${ZLIB_SOURCE_DIR}/compress.c
    ${ZLIB_SOURCE_DIR}/crc32.c
    ${ZLIB_SOURCE_DIR}/deflate.c
    ${ZLIB_SOURCE_DIR}/gzclose.c
    ${ZLIB_SOURCE_DIR}/gzlib.c
    ${ZLIB_SOURCE_DIR}/gzread.c
    ${ZLIB_SOURCE_DIR}/gzwrite.c
    ${ZLIB_SOURCE_DIR}/inflate.c
    ${ZLIB_SOURCE_DIR}/infback.c
    ${ZLIB_SOURCE_DIR}/inftrees.c
    ${ZLIB_SOURCE_DIR}/inffast.c
    ${ZLIB_SOURCE_DIR}/trees.c
    ${ZLIB_SOURCE_DIR}/uncompr.c
    ${ZLIB_SOURCE_DIR}/zutil.c
)


#============================================================================
# png
#============================================================================

if(NOT WIN32)
  find_library(M_LIBRARY
    NAMES m
    PATHS /usr/lib /usr/local/lib
  )
  if(NOT M_LIBRARY)
    message(STATUS
      "math library 'libm' not found - floating point support disabled")
  endif()
else()
  # not needed on windows
  set(M_LIBRARY "")
endif()

# maybe needs improving, but currently I don't know when we can enable what :)
set(png_asm_tmp "OFF")
if(NOT WIN32)
  find_program(uname_executable NAMES uname PATHS /bin /usr/bin /usr/local/bin)
  if(uname_executable)
    exec_program(${uname_executable}
                 ARGS --machine OUTPUT_VARIABLE uname_output)
    if("uname_output" MATCHES "^.*i[1-9]86.*$")
      set(png_asm_tmp "ON")
    else("uname_output" MATCHES "^.*i[1-9]86.*$")
      set(png_asm_tmp "OFF")
    endif("uname_output" MATCHES "^.*i[1-9]86.*$")
  endif(uname_executable)
else()
  # this env var is normally only set on win64
  set(TEXT "ProgramFiles(x86)")
  if("$ENV{${TEXT}}" STREQUAL "")
    set(png_asm_tmp "ON")
  endif("$ENV{${TEXT}}" STREQUAL "")
endif()

set(PNG_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/RTL/Dependencies/png)
# OUR SOURCES
set(libpng_sources
  ${PNG_SOURCE_DIR}/png.h
  ${PNG_SOURCE_DIR}/pngconf.h
 ${PNG_SOURCE_DIR}/pngpriv.h
  ${PNG_SOURCE_DIR}/png.c
  ${PNG_SOURCE_DIR}/pngerror.c
  ${PNG_SOURCE_DIR}/pngget.c
  ${PNG_SOURCE_DIR}/pngmem.c
  ${PNG_SOURCE_DIR}/pngpread.c
  ${PNG_SOURCE_DIR}/pngread.c
  ${PNG_SOURCE_DIR}/pngrio.c
  ${PNG_SOURCE_DIR}/pngrtran.c
  ${PNG_SOURCE_DIR}/pngrutil.c
  ${PNG_SOURCE_DIR}/pngset.c
  ${PNG_SOURCE_DIR}/pngtrans.c
  ${PNG_SOURCE_DIR}/pngwio.c
  ${PNG_SOURCE_DIR}/pngwrite.c
  ${PNG_SOURCE_DIR}/pngwtran.c
  ${PNG_SOURCE_DIR}/pngwutil.c
)
# SOME NEEDED DEFINITIONS

add_definitions(-DPNG_CONFIGURE_LIBPNG)

if(_AIX)
  add_definitions(-D_ALL_SOURCE)
endif(_AIX)

if(MSVC)
  add_definitions(-DPNG_NO_MODULEDEF -D_CRT_SECURE_NO_DEPRECATE)
endif(MSVC)

add_definitions(-DPNG_NO_CONSOLE_IO)

if(NOT M_LIBRARY AND NOT WIN32)
  add_definitions(-DPNG_NO_FLOATING_POINT_SUPPORTED)
endif()

set(ADDITIONAL_LIBRARIES ${ADDITIONAL_LIBRARIES} ${M_LIBRARY})
include_directories(${PNG_SOURCE_DIR})

CHECK_INCLUDE_FILE(stdlib.h HAVE_STDLIB_H)

set(JPEG_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/RTL/Dependencies/jpeg)
configure_file(${JPEG_SOURCE_DIR}/jconfig.h.cmake
               ${CMAKE_CURRENT_BINARY_DIR}/jconfig.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${JPEG_SOURCE_DIR})

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

set(JPEG_PUBLIC_HDRS
    ${JPEG_SOURCE_DIR}/jerror.h
    ${JPEG_SOURCE_DIR}/jmorecfg.h
    ${JPEG_SOURCE_DIR}/jpeglib.h
    ${CMAKE_CURRENT_BINARY_DIR}/jconfig.h
)
set(JPEG_PRIVATE_HDRS
    ${JPEG_SOURCE_DIR}/cderror.h
    ${JPEG_SOURCE_DIR}/jdct.h
    ${JPEG_SOURCE_DIR}/jinclude.h
    ${JPEG_SOURCE_DIR}/jmemsys.h
    ${JPEG_SOURCE_DIR}/jpegint.h
    ${JPEG_SOURCE_DIR}/jversion.h
    ${JPEG_SOURCE_DIR}/transupp.h
)

# memmgr back ends: compile only one of these into a working library
# (For now, let's use the mode that requires the image fit into memory.
# This is the recommended mode for Win32 anyway.)
SET(JPEG_systemdependent_SRCS ${JPEG_SOURCE_DIR}/jmemnobs.c)

set(JPEG_SRCS
    ${JPEG_SOURCE_DIR}/jaricom.c
    ${JPEG_SOURCE_DIR}/jcapimin.c
    ${JPEG_SOURCE_DIR}/jcapistd.c
    ${JPEG_SOURCE_DIR}/jcarith.c
    ${JPEG_SOURCE_DIR}/jccoefct.c
    ${JPEG_SOURCE_DIR}/jccolor.c
    ${JPEG_SOURCE_DIR}/jcdctmgr.c
    ${JPEG_SOURCE_DIR}/jchuff.c
    ${JPEG_SOURCE_DIR}/jcinit.c
    ${JPEG_SOURCE_DIR}/jcmainct.c
    ${JPEG_SOURCE_DIR}/jcmarker.c
    ${JPEG_SOURCE_DIR}/jcmaster.c
    ${JPEG_SOURCE_DIR}/jcomapi.c
    ${JPEG_SOURCE_DIR}/jcparam.c
    ${JPEG_SOURCE_DIR}/jcprepct.c
    ${JPEG_SOURCE_DIR}/jcsample.c
    ${JPEG_SOURCE_DIR}/jctrans.c
    ${JPEG_SOURCE_DIR}/jdapimin.c
    ${JPEG_SOURCE_DIR}/jdapistd.c
    ${JPEG_SOURCE_DIR}/jdarith.c
    ${JPEG_SOURCE_DIR}/jdatadst.c
    ${JPEG_SOURCE_DIR}/jdatasrc.c
    ${JPEG_SOURCE_DIR}/jdcoefct.c
    ${JPEG_SOURCE_DIR}/jdcolor.c
    ${JPEG_SOURCE_DIR}/jddctmgr.c
    ${JPEG_SOURCE_DIR}/jdhuff.c
    ${JPEG_SOURCE_DIR}/jdinput.c
    ${JPEG_SOURCE_DIR}/jdmainct.c
    ${JPEG_SOURCE_DIR}/jdmarker.c
    ${JPEG_SOURCE_DIR}/jdmaster.c
    ${JPEG_SOURCE_DIR}/jdmerge.c
    ${JPEG_SOURCE_DIR}/jdpostct.c
    ${JPEG_SOURCE_DIR}/jdsample.c
    ${JPEG_SOURCE_DIR}/jdtrans.c
    ${JPEG_SOURCE_DIR}/jerror.c
    ${JPEG_SOURCE_DIR}/jfdctflt.c
    ${JPEG_SOURCE_DIR}/jfdctfst.c
    ${JPEG_SOURCE_DIR}/jfdctint.c
    ${JPEG_SOURCE_DIR}/jidctflt.c
    ${JPEG_SOURCE_DIR}/jidctfst.c
    ${JPEG_SOURCE_DIR}/jidctint.c
    ${JPEG_SOURCE_DIR}/jquant1.c
    ${JPEG_SOURCE_DIR}/jquant2.c
    ${JPEG_SOURCE_DIR}/jutils.c
    ${JPEG_SOURCE_DIR}/jmemmgr.c)

SET(DEPENDENCIES_SRCS ${ZLIB_SRCS} ${ZLIB_PUBLIC_HDRS} ${ZLIB_PRIVATE_HDRS} ${libpng_sources} ${JPEG_systemdependent_SRCS} ${JPEG_SRCS} ${JPEG_PUBLIC_HDRS} ${JPEG_PRIVATE_HDRS})
ENDIF(U3D_SHARED OR BUILD_SHARED_LIBS)


SET_PROPERTY( SOURCE
	RTL/Platform/${U3D_PLATFORM}/Common/IFXOSLoader.cpp
	PROPERTY COMPILE_DEFINITIONS U3DPluginsPath="Plugins" U3DCorePath="." )
IF(STDIO_HACK)
SET_PROPERTY( SOURCE
	RTL/Component/Exporting/CIFXStdioWriteBufferX.cpp
	IDTF/ConverterDriver.cpp
	IDTF/File.cpp
	PROPERTY COMPILE_DEFINITIONS STDIO_HACK )
SET_PROPERTY( SOURCE
	RTL/Platform/${U3D_PLATFORM}/Common/IFXOSLoader.cpp
	PROPERTY COMPILE_DEFINITIONS U3DPluginsPath="Plugins" U3DCorePath="." STDIO_HACK )
ENDIF(STDIO_HACK)

SET( LIB_DESTINATION u3d )
SET( BIN_DESTINATION u3d )
SET( INCLUDE_DESTINATION u3d/include )
SET( PLUGIN_DESTINATION u3d )
SET( SAMPLE_DESTINATION u3d/samples )
SET( DOC_DESTINATION u3d/docs )

# SET( CMAKE_INSTALL_PREFIX Build )

INCLUDE_DIRECTORIES( RTL/Component/Include RTL/Kernel/Include RTL/Platform/Include
	RTL/Component/Base
	RTL/Component/Rendering
	RTL/Dependencies/WildCards )

SET( IFXCoreStatic_SRCS
	RTL/IFXCoreStatic/IFXCoreStatic.cpp
	RTL/Platform/${U3D_PLATFORM}/Common/IFXOSUtilities.cpp
	RTL/Platform/${U3D_PLATFORM}/Common/IFXOSLoader.cpp
	RTL/Platform/${U3D_PLATFORM}/Common/IFXOSRenderWindow.cpp
	RTL/Component/Common/IFXDids.cpp
	RTL/Component/Base/IFXCoincidentVertexMap.cpp
	RTL/Component/Base/IFXCornerIter.cpp
	RTL/Component/Base/IFXEuler.cpp
	RTL/Component/Base/IFXFatCornerIter.cpp
	RTL/Component/Base/IFXTransform.cpp
	RTL/Component/Base/IFXVectorHasher.cpp
	RTL/Component/Base/IFXVertexMap.cpp
	RTL/Component/Base/IFXVertexMapGroup.cpp
	RTL/Kernel/DataTypes/IFXCoreArray.cpp
	RTL/Kernel/DataTypes/IFXCoreList.cpp
	RTL/Kernel/DataTypes/IFXFastAllocator.cpp
	RTL/Kernel/DataTypes/IFXListNode.cpp
	RTL/Kernel/DataTypes/IFXMatrix4x4.cpp
	RTL/Kernel/DataTypes/IFXQuaternion.cpp
	RTL/Kernel/DataTypes/IFXString.cpp
	RTL/Kernel/DataTypes/IFXUnitAllocator.cpp
	RTL/Kernel/DataTypes/IFXVector3.cpp
	RTL/Kernel/DataTypes/IFXVector4.cpp
	RTL/Dependencies/WildCards/wcmatch.cpp
	RTL/Kernel/Common/IFXDebug.cpp
	)
ADD_LIBRARY( IFXCoreStatic ${IFXCoreStatic_SRCS} )
install(
	TARGETS IFXCoreStatic
	ARCHIVE DESTINATION ${LIB_DESTINATION}
	LIBRARY DESTINATION ${LIB_DESTINATION}
)

# Something Windows-only 
# INCLUDE_DIRECTORIES( RTL/Component/Include RTL/Kernel/Include RTL/Platform/Include )
# 
# SET( IFXImportingStatic_SRCS
# 	RTL/Component/Base/IFXCoincidentVertexMap.cpp
# 	RTL/Component/Base/IFXCornerIter.cpp
# 	RTL/Component/Base/IFXVectorHasher.cpp
# 	RTL/Component/Base/IFXBoundingBox.cpp
# 	RTL/Component/Base/IFXVertexMap.cpp )

# ADD_LIBRARY( IFXImportingStatic ${IFXImportingStatic_SRCS} )

INCLUDE_DIRECTORIES( RTL/Component/Include RTL/Kernel/Include RTL/Platform/Include
	RTL/Component/Base
	RTL/Component/BitStream
	RTL/Component/Bones
	RTL/Component/BoundHierarchy
	RTL/Component/CLODAuthor
	RTL/Component/Common
	RTL/Component/Generators/CLOD
	RTL/Component/Generators/Glyph2D
	RTL/Component/Generators/LineSet
	RTL/Component/Generators/PointSet
	RTL/Component/Mesh
	RTL/Component/ModifierChain
	RTL/Component/Palette
	RTL/Component/Rendering
	RTL/Component/SceneGraph
	RTL/Component/Shaders
	RTL/Component/Subdiv
	RTL/Component/Texture
	RTL/Component/UVGenerator
	RTL/Kernel/IFXCom
	RTL/Kernel/Common
	RTL/Dependencies/FNVHash
	RTL/Dependencies/Predicates
	RTL/Dependencies/WildCards
	)

SET( IFXCore_SRCS
	RTL/Platform/${U3D_PLATFORM}/IFXCore/IFXCoreDllMain.cpp
	RTL/IFXCorePluginStatic/IFXCorePluginStatic.cpp
	RTL/Platform/${U3D_PLATFORM}/Common/IFXOSLoader.cpp
	RTL/Platform/${U3D_PLATFORM}/Common/IFXOSRenderWindow.cpp
	RTL/Platform/${U3D_PLATFORM}/Common/IFXOSUtilities.cpp
	RTL/Component/Base/IFXCoincidentVertexMap.cpp
	RTL/Component/Base/IFXCornerIter.cpp
	RTL/Component/Base/IFXEuler.cpp
	RTL/Component/Base/IFXFatCornerIter.cpp
	RTL/Component/Base/IFXTransform.cpp
	RTL/Component/Base/IFXVectorHasher.cpp
	RTL/Component/Base/IFXVertexMap.cpp
	RTL/Component/Base/IFXVertexMapGroup.cpp
	RTL/Component/BitStream/CIFXBitStreamX.cpp
	RTL/Component/BitStream/CIFXDataBlockQueueX.cpp
	RTL/Component/BitStream/CIFXDataBlockX.cpp
	RTL/Component/BitStream/IFXHistogramDynamic.cpp
	RTL/Component/Bones/CIFXAnimationModifier.cpp
	RTL/Component/Bones/CIFXBoneWeightsModifier.cpp
	RTL/Component/Bones/CIFXSkeleton.cpp
	RTL/Component/Bones/IFXBoneCacheArray.cpp
	RTL/Component/Bones/IFXBoneNode.cpp
	RTL/Component/Bones/IFXBonesManagerImpl.cpp
	RTL/Component/Bones/IFXCharacter.cpp
	RTL/Component/Bones/IFXCoreNode.cpp
	RTL/Component/Bones/IFXKeyTrack.cpp
	RTL/Component/Bones/IFXMeshGroup_Character.cpp
	RTL/Component/Bones/IFXMixerQueue.cpp
	RTL/Component/Bones/IFXMixerQueueImpl.cpp
	RTL/Component/Bones/IFXMotion.cpp
	RTL/Component/Bones/IFXMotionManagerImpl.cpp
	RTL/Component/Bones/IFXMotionMixerImpl.cpp
	RTL/Component/Bones/IFXSkin.cpp
	RTL/Component/Bones/IFXSkin_p3.cpp
	RTL/Component/Bones/IFXVertexWeight.cpp
	RTL/Component/Bones/IFXVertexWeights.cpp
	RTL/Component/BoundHierarchy/CIFXAABBHierarchyBuilder.cpp
	RTL/Component/BoundHierarchy/CIFXAxisAlignedBBox.cpp
	RTL/Component/BoundHierarchy/CIFXBoundFace.cpp
	RTL/Component/BoundHierarchy/CIFXBoundHierarchy.cpp
	RTL/Component/BoundHierarchy/CIFXBoundUtil.cpp
	RTL/Component/BoundHierarchy/CIFXBTree.cpp
	RTL/Component/BoundHierarchy/CIFXBTreeNode.cpp
	RTL/Component/BoundHierarchy/CIFXCollisionResult.cpp
	RTL/Component/BoundHierarchy/CIFXPickObject.cpp
	RTL/Component/BoundHierarchy/CIFXPrimitiveOverlap.cpp
	RTL/Component/BoundHierarchy/CIFXResultAllocator.cpp
	RTL/Component/CLODAuthor/CIFXAuthorCLODGen.cpp
	RTL/Component/CLODAuthor/CIFXAuthorMeshMap.cpp
	RTL/Component/CLODAuthor/CIFXSetAdjacencyX.cpp
	RTL/Component/CLODAuthor/CIFXSetX.cpp
	RTL/Component/CLODAuthor/CLODGenerator.cpp
	RTL/Component/CLODAuthor/ContractionRecorder.cpp
	RTL/Component/CLODAuthor/Face.cpp
	RTL/Component/CLODAuthor/Matrix4x4.cpp
	RTL/Component/CLODAuthor/NormalMap.cpp
	RTL/Component/CLODAuthor/Pair.cpp
	RTL/Component/CLODAuthor/PairFinder.cpp
	RTL/Component/CLODAuthor/PairHash.cpp
	RTL/Component/CLODAuthor/PairHeap.cpp
	RTL/Component/CLODAuthor/Primitives.cpp
	RTL/Component/CLODAuthor/Vertex.cpp
	RTL/Component/CLODAuthor/VertexPairContractor.cpp
	RTL/Component/Common/CIFXCoreServices.cpp
	RTL/Component/Common/CIFXCoreServicesRef.cpp
	RTL/Component/Common/CIFXHashMap.cpp
	RTL/Component/Common/CIFXIDManager.cpp
	RTL/Component/Common/CIFXMetaData.cpp
	RTL/Component/Common/CIFXNameMap.cpp
	RTL/Component/Common/CIFXSimpleHash.cpp
	RTL/Component/Common/CIFXVoidWrapper.cpp
	RTL/Component/Common/IFXComponentDescriptorList.cpp
	RTL/Component/Common/IFXComponentIds.cpp
	RTL/Component/Common/IFXDids.cpp
	RTL/Component/Generators/CLOD/CIFXAuthorCLODResource.cpp
	RTL/Component/Generators/CLOD/CIFXAuthorMesh.cpp
	RTL/Component/Generators/CLOD/CIFXAuthorMeshScrub.cpp
	RTL/Component/Generators/CLOD/CIFXCLODModifier.cpp
	RTL/Component/Generators/CLOD/CIFXMeshCompiler.cpp
	RTL/Component/Generators/CLOD/CIFXMeshMap.cpp
	RTL/Component/Generators/CLOD/IFXCLODManager.cpp
	RTL/Component/Generators/CLOD/IFXNeighborResController.cpp
	RTL/Component/Generators/Glyph2D/CIFXContour.cpp
	RTL/Component/Generators/Glyph2D/CIFXContourExtruder.cpp
	RTL/Component/Generators/Glyph2D/CIFXContourGenerator.cpp
	RTL/Component/Generators/Glyph2D/CIFXContourTessellator.cpp
	RTL/Component/Generators/Glyph2D/CIFXGlyph2DCommands.cpp
	RTL/Component/Generators/Glyph2D/CIFXGlyph2DModifier.cpp
	RTL/Component/Generators/Glyph2D/CIFXGlyph3DGenerator.cpp
	RTL/Component/Generators/Glyph2D/CIFXGlyphCommandList.cpp
	RTL/Component/Generators/Glyph2D/CIFXQuadEdge.cpp
	RTL/Component/Generators/LineSet/CIFXAuthorLineSetAnalyzer.cpp
	RTL/Component/Generators/LineSet/CIFXAuthorLineSet.cpp
	RTL/Component/Generators/LineSet/CIFXAuthorLineSetResource.cpp
	RTL/Component/Generators/PointSet/CIFXAuthorPointSet.cpp
	RTL/Component/Generators/PointSet/CIFXAuthorPointSetResource.cpp
	RTL/Component/Mesh/CIFXInterleavedData.cpp
	RTL/Component/Mesh/CIFXMesh.cpp
	RTL/Component/Mesh/CIFXMeshGroup.cpp
	RTL/Component/Mesh/CIFXNeighborMesh.cpp
	RTL/Component/Mesh/CIFXRenderable.cpp
	RTL/Component/Mesh/IFXFaceLists.cpp
	RTL/Component/ModifierChain/CIFXDidRegistry.cpp
	RTL/Component/ModifierChain/CIFXModifierChain.cpp
	RTL/Component/ModifierChain/CIFXModifier.cpp
	RTL/Component/ModifierChain/CIFXModifierDataElementIter.cpp
	RTL/Component/ModifierChain/CIFXModifierDataPacket.cpp
	RTL/Component/ModifierChain/CIFXSubject.cpp
	RTL/Component/ModifierChain/IFXModifierChainState.cpp
	RTL/Component/Palette/CIFXPalette.cpp
	RTL/Component/Palette/CIFXSimpleObject.cpp
	RTL/Component/SceneGraph/CIFXBoundSphereDataElement.cpp
	RTL/Component/SceneGraph/CIFXDevice.cpp
	RTL/Component/SceneGraph/CIFXDummyModifier.cpp
	RTL/Component/SceneGraph/CIFXFileReference.cpp
	RTL/Component/SceneGraph/CIFXGroup.cpp
	RTL/Component/SceneGraph/CIFXLight.cpp
	RTL/Component/SceneGraph/CIFXLightResource.cpp
	RTL/Component/SceneGraph/CIFXLightSet.cpp
	RTL/Component/SceneGraph/CIFXMarker.cpp
	RTL/Component/SceneGraph/CIFXMaterialResource.cpp
	RTL/Component/SceneGraph/CIFXMixerConstruct.cpp
	RTL/Component/SceneGraph/CIFXModel.cpp
	RTL/Component/SceneGraph/CIFXMotionResource.cpp
	RTL/Component/SceneGraph/CIFXNode.cpp
	RTL/Component/SceneGraph/CIFXResourceClient.cpp
	RTL/Component/SceneGraph/CIFXSceneGraph.cpp
	RTL/Component/SceneGraph/CIFXShaderList.cpp
	RTL/Component/SceneGraph/CIFXSimpleCollection.cpp
	RTL/Component/SceneGraph/CIFXSimpleList.cpp
	RTL/Component/SceneGraph/CIFXView.cpp
	RTL/Component/SceneGraph/CIFXViewResource.cpp
	RTL/Component/SceneGraph/IFXRenderPass.cpp
	RTL/Component/Shaders/CIFXShader.cpp
	RTL/Component/Shaders/CIFXShaderLitTexture.cpp
	RTL/Component/Shaders/CIFXShadingModifier.cpp
	RTL/Component/Subdiv/CIFXSubdivModifier.cpp
	RTL/Component/Subdiv/IFXAttributeNeighborhood.cpp
	RTL/Component/Subdiv/IFXButterflyScheme.cpp
	RTL/Component/Subdiv/IFXScreenSpaceMetric.cpp
	RTL/Component/Subdiv/IFXSharedUnitAllocator.cpp
	RTL/Component/Subdiv/IFXSubdivisionManager.cpp
	RTL/Component/Subdiv/IFXTQTAddress.cpp
	RTL/Component/Subdiv/IFXTQTAttribute.cpp
	RTL/Component/Subdiv/IFXTQTBaseTriangle.cpp
	RTL/Component/Subdiv/IFXTQTTriangleAllocator.cpp
	RTL/Component/Subdiv/IFXTQTTriangle.cpp
	RTL/Component/Subdiv/IFXVertexAllocator.cpp
	RTL/Component/Texture/CIFXImageTools.cpp
	RTL/Component/Texture/CIFXTextureImageTools.cpp
	RTL/Component/Texture/CIFXTextureObject.cpp
	RTL/Component/UVGenerator/CIFXUVGenerator.cpp
	RTL/Component/UVGenerator/CIFXUVMapperCylindrical.cpp
	RTL/Component/UVGenerator/CIFXUVMapperNone.cpp
	RTL/Component/UVGenerator/CIFXUVMapperPlanar.cpp
	RTL/Component/UVGenerator/CIFXUVMapperReflection.cpp
	RTL/Component/UVGenerator/CIFXUVMapperSpherical.cpp
	RTL/Kernel/Common/CIFXConnector.cpp
	RTL/Kernel/Common/CIFXPerformanceTimer.cpp 
	RTL/Kernel/Common/IFXCheckX.cpp
	RTL/Kernel/IFXCom/CIFXComponentManager.cpp
	RTL/Kernel/IFXCom/CIFXGUIDHashMap.cpp
	RTL/Kernel/IFXCom/CIFXPluginProxy.cpp
	RTL/Kernel/IFXCom/IFXCom.cpp
	RTL/Kernel/Memory/IFXMemory.cpp
	RTL/Kernel/DataTypes/IFXCoreArray.cpp
	RTL/Kernel/DataTypes/IFXCoreList.cpp
	RTL/Kernel/DataTypes/IFXFastAllocator.cpp
	RTL/Kernel/DataTypes/IFXListNode.cpp
	RTL/Kernel/DataTypes/IFXMatrix4x4.cpp
	RTL/Kernel/DataTypes/IFXQuaternion.cpp
	RTL/Kernel/DataTypes/IFXString.cpp
	RTL/Kernel/DataTypes/IFXUnitAllocator.cpp
	RTL/Kernel/DataTypes/IFXVector3.cpp
	RTL/Kernel/DataTypes/IFXVector4.cpp
	RTL/Dependencies/WildCards/wcmatch.cpp
	RTL/Dependencies/Predicates/predicates.cpp
	RTL/Kernel/Common/IFXDebug.cpp
	)
IF(WIN32)
  ADD_LIBRARY( IFXCore SHARED ${IFXCore_SRCS} RTL/Platform/Win32/IFXCore/IFXCore.rc RTL/Platform/Win32/IFXCore/IFXCore.def ${DEPENDENCIES_SRCS} )
ENDIF(WIN32)
IF(APPLE)
  ADD_LIBRARY( IFXCore MODULE ${IFXCore_SRCS} ${DEPENDENCIES_SRCS} )
  set_target_properties( IFXCore PROPERTIES
	LINK_FLAGS "${MY_LINK_FLAGS} -exported_symbols_list ${CMAKE_CURRENT_SOURCE_DIR}/RTL/Platform/Mac32/IFXCore/IFXCore.def" )
ENDIF(APPLE)
IF(UNIX AND NOT APPLE)
  ADD_LIBRARY( IFXCore MODULE ${IFXCore_SRCS} ${DEPENDENCIES_SRCS} )
  set_target_properties( IFXCore PROPERTIES
	LINK_FLAGS "-Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/RTL/Platform/Lin32/IFXCore/IFXCore.list -Wl,--no-undefined" )
  target_link_libraries( IFXCore ${CMAKE_DL_LIBS} )
ENDIF(UNIX AND NOT APPLE)
TARGET_LINK_LIBRARIES( IFXCore ${ADDITIONAL_LIBRARIES} )
install(
	TARGETS IFXCore
	ARCHIVE DESTINATION ${LIB_DESTINATION}
	LIBRARY DESTINATION ${LIB_DESTINATION}
)


INCLUDE_DIRECTORIES( RTL/Component/Include RTL/Kernel/Include RTL/Platform/Include
	RTL/Component/Exporting
	RTL/Dependencies/WildCards )
SET( IFXExporting_SRCS
	RTL/Platform/${U3D_PLATFORM}/IFXExporting/IFXExportingDllMain.cpp
	RTL/Component/Exporting/CIFXAnimationModifierEncoder.cpp
	RTL/Component/Exporting/CIFXAuthorCLODEncoderX.cpp
	RTL/Component/Exporting/CIFXAuthorCLODEncoderX_P.cpp
	RTL/Component/Exporting/CIFXAuthorCLODEncoderX_S.cpp
	RTL/Component/Exporting/CIFXAuthorGeomCompiler.cpp
	RTL/Component/Exporting/CIFXBlockPriorityQueueX.cpp
	RTL/Component/Exporting/CIFXBlockWriterX.cpp
	RTL/Component/Exporting/CIFXBoneWeightsModifierEncoder.cpp
	RTL/Component/Exporting/CIFXCLODModifierEncoder.cpp
	RTL/Component/Exporting/CIFXDummyModifierEncoder.cpp
	RTL/Component/Exporting/CIFXFileReferenceEncoder.cpp
	RTL/Component/Exporting/CIFXGlyphModifierEncoder.cpp
	RTL/Component/Exporting/CIFXGroupNodeEncoder.cpp
	RTL/Component/Exporting/CIFXLightNodeEncoder.cpp
	RTL/Component/Exporting/CIFXLightResourceEncoder.cpp
	RTL/Component/Exporting/CIFXLineSetEncoder.cpp
	RTL/Component/Exporting/CIFXMaterialResourceEncoder.cpp
	RTL/Component/Exporting/CIFXModelNodeEncoder.cpp
	RTL/Component/Exporting/CIFXMotionResourceEncoder.cpp
	RTL/Component/Exporting/CIFXNodeBaseEncoder.cpp
	RTL/Component/Exporting/CIFXPointSetEncoder.cpp
	RTL/Component/Exporting/CIFXShaderLitTextureEncoder.cpp
	RTL/Component/Exporting/CIFXShadingModifierEncoder.cpp
	RTL/Component/Exporting/CIFXStdioWriteBufferX.cpp
	RTL/Component/Exporting/CIFXSubdivisionModifierEncoder.cpp
	RTL/Component/Exporting/CIFXViewNodeEncoder.cpp
	RTL/Component/Exporting/CIFXViewResourceEncoder.cpp
	RTL/Component/Exporting/CIFXWriteManager.cpp
	RTL/Component/Exporting/IFXExporting.cpp
	RTL/Component/Exporting/IFXExportingGuids.cpp
	RTL/IFXCorePluginStatic/IFXCorePluginStatic.cpp
	RTL/Platform/${U3D_PLATFORM}/Common/IFXOSUtilities.cpp
	RTL/Platform/${U3D_PLATFORM}/Common/IFXOSLoader.cpp
	RTL/Component/Base/IFXVertexMap.cpp
	RTL/Kernel/DataTypes/IFXCoreArray.cpp
	RTL/Kernel/DataTypes/IFXCoreList.cpp
	RTL/Kernel/DataTypes/IFXFastAllocator.cpp
	RTL/Kernel/DataTypes/IFXListNode.cpp
	RTL/Kernel/DataTypes/IFXMatrix4x4.cpp
	RTL/Kernel/DataTypes/IFXQuaternion.cpp
	RTL/Kernel/DataTypes/IFXString.cpp
	RTL/Kernel/DataTypes/IFXUnitAllocator.cpp
	RTL/Kernel/DataTypes/IFXVector3.cpp
	RTL/Kernel/DataTypes/IFXVector4.cpp
	RTL/Dependencies/WildCards/wcmatch.cpp
	RTL/Kernel/Common/IFXDebug.cpp
	)
IF(WIN32)
  ADD_LIBRARY( IFXExporting  MODULE ${IFXExporting_SRCS} RTL/Platform/Win32/IFXExporting/IFXExporting.rc RTL/Platform/Win32/IFXExporting/IFXExporting.def )
  TARGET_LINK_LIBRARIES( IFXExporting IFXCore )
ENDIF(WIN32)
IF(APPLE)
  ADD_LIBRARY( IFXExporting  MODULE ${IFXExporting_SRCS} )
  set_target_properties( IFXExporting  PROPERTIES
	LINK_FLAGS "${MY_LINK_FLAGS} -exported_symbols_list ${CMAKE_CURRENT_SOURCE_DIR}/RTL/Platform/Mac32/IFXExporting/IFXExporting.def   -undefined dynamic_lookup" )
ENDIF(APPLE)
IF(UNIX AND NOT APPLE)
  ADD_LIBRARY( IFXExporting  MODULE ${IFXExporting_SRCS} )
  set_target_properties( IFXExporting  PROPERTIES
	LINK_FLAGS "-Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/RTL/Platform/Lin32/IFXExporting/IFXExporting.list" )
ENDIF(UNIX AND NOT APPLE)
install(
	TARGETS IFXExporting
	LIBRARY DESTINATION ${PLUGIN_DESTINATION}
)

INCLUDE_DIRECTORIES( RTL/Component/Include RTL/Kernel/Include RTL/Platform/Include
	RTL/Component/Importing
	RTL/Dependencies/WildCards )
SET( IFXImporting_SRCS
	RTL/Platform/${U3D_PLATFORM}/IFXImporting/IFXImportingDllMain.cpp
	RTL/Component/Importing/CIFXAnimationModifierDecoder.cpp
	RTL/Component/Importing/CIFXAuthorCLODDecoder.cpp
	RTL/Component/Importing/CIFXAuthorCLODDecoder_P.cpp
	RTL/Component/Importing/CIFXAuthorCLODDecoder_S.cpp
	RTL/Component/Importing/CIFXBlockReaderX.cpp
	RTL/Component/Importing/CIFXBoneWeightsModifierDecoder.cpp
	RTL/Component/Importing/CIFXBTTHash.cpp
	RTL/Component/Importing/CIFXCLODModifierDecoder.cpp
	RTL/Component/Importing/CIFXDecoderChainX.cpp
	RTL/Component/Importing/CIFXDummyModifierDecoder.cpp
	RTL/Component/Importing/CIFXGlyphModifierDecoder.cpp
	RTL/Component/Importing/CIFXGroupDecoder.cpp
	RTL/Component/Importing/CIFXInternetReadBufferX.cpp
	RTL/Component/Importing/CIFXLightDecoder.cpp
	RTL/Component/Importing/CIFXLightResourceDecoder.cpp
	RTL/Component/Importing/CIFXLineSetDecoder.cpp
	RTL/Component/Importing/CIFXLoadManager.cpp
	RTL/Component/Importing/CIFXMaterialDecoder.cpp
	RTL/Component/Importing/CIFXModelDecoder.cpp
	RTL/Component/Importing/CIFXMotionDecoder.cpp
	RTL/Component/Importing/CIFXNodeDecoder.cpp
	RTL/Component/Importing/CIFXPointSetDecoder.cpp
	RTL/Component/Importing/CIFXShaderLitTextureDecoder.cpp
	RTL/Component/Importing/CIFXShadingModifierDecoder.cpp
	RTL/Component/Importing/CIFXStdioReadBufferX.cpp
	RTL/Component/Importing/CIFXSubdivisionModifierDecoder.cpp
	RTL/Component/Importing/CIFXTextureDecoder.cpp
	RTL/Component/Importing/CIFXViewDecoder.cpp
	RTL/Component/Importing/CIFXViewResourceDecoder.cpp
	RTL/Component/Importing/IFXImporting.cpp
	RTL/Component/Importing/IFXImportingGuids.cpp
	RTL/Component/Importing/IFXInternetConnectionX.cpp
	RTL/Component/Importing/IFXInternetSessionX.cpp
	RTL/Component/Importing/IFXModifierBaseDecoder.cpp
	RTL/Component/Importing/IFXSocketStream.cpp
	RTL/IFXCorePluginStatic/IFXCorePluginStatic.cpp
	RTL/Platform/${U3D_PLATFORM}/Common/IFXOSUtilities.cpp
	RTL/Platform/${U3D_PLATFORM}/Common/IFXOSLoader.cpp
	RTL/Platform/${U3D_PLATFORM}/Common/IFXOSSocket.cpp
	RTL/Kernel/DataTypes/IFXCoreArray.cpp
	RTL/Kernel/DataTypes/IFXCoreList.cpp
	RTL/Kernel/DataTypes/IFXFastAllocator.cpp
	RTL/Kernel/DataTypes/IFXListNode.cpp
	RTL/Kernel/DataTypes/IFXMatrix4x4.cpp
	RTL/Kernel/DataTypes/IFXQuaternion.cpp
	RTL/Kernel/DataTypes/IFXString.cpp
	RTL/Kernel/DataTypes/IFXUnitAllocator.cpp
	RTL/Kernel/DataTypes/IFXVector3.cpp
	RTL/Kernel/DataTypes/IFXVector4.cpp
	RTL/Dependencies/WildCards/wcmatch.cpp
	RTL/Kernel/Common/IFXDebug.cpp
	)
IF(WIN32)
  ADD_LIBRARY( IFXImporting  MODULE ${IFXImporting_SRCS} RTL/Platform/Win32/IFXImporting/IFXImporting.rc RTL/Platform/Win32/IFXImporting/IFXImporting.def )
  TARGET_LINK_LIBRARIES( IFXImporting IFXCore ws2_32 )
ENDIF(WIN32)
IF(APPLE)
  ADD_LIBRARY( IFXImporting  MODULE ${IFXImporting_SRCS} )
  set_target_properties( IFXImporting  PROPERTIES
	LINK_FLAGS "${MY_LINK_FLAGS} -exported_symbols_list ${CMAKE_CURRENT_SOURCE_DIR}/RTL/Platform/Mac32/IFXImporting/IFXImporting.def   -undefined dynamic_lookup" )
ENDIF(APPLE)
IF(UNIX AND NOT APPLE)
  ADD_LIBRARY( IFXImporting  MODULE ${IFXImporting_SRCS} )
  set_target_properties( IFXImporting  PROPERTIES
	LINK_FLAGS "-Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/RTL/Platform/Lin32/IFXImporting/IFXImporting.list" )
ENDIF(UNIX AND NOT APPLE)
install(
	TARGETS IFXImporting
	LIBRARY DESTINATION ${PLUGIN_DESTINATION}
)

INCLUDE_DIRECTORIES( RTL/Component/Include RTL/Kernel/Include RTL/Platform/Include
	RTL/Component/ModifierChain
	RTL/Component/SceneGraph
	RTL/Component/Scheduling
	RTL/Dependencies/WildCards )
SET( IFXScheduling_SRCS
	RTL/Platform/${U3D_PLATFORM}/IFXScheduling/IFXSchedulingDllMain.cpp
	RTL/Component/Scheduling/CIFXClock.cpp
	RTL/Component/Scheduling/CIFXErrorInfo.cpp
	RTL/Component/Scheduling/CIFXNotificationInfo.cpp
	RTL/Component/Scheduling/CIFXNotificationManager.cpp
	RTL/Component/Scheduling/CIFXScheduler.cpp
	RTL/Component/Scheduling/CIFXSchedulerInfo.cpp
	RTL/Component/Scheduling/CIFXSimulationInfo.cpp
	RTL/Component/Scheduling/CIFXSimulationManager.cpp
	RTL/Component/Scheduling/CIFXSystemManager.cpp
	RTL/Component/Scheduling/CIFXTaskCallback.cpp
	RTL/Component/Scheduling/CIFXTaskData.cpp
	RTL/Component/Scheduling/CIFXTaskManager.cpp
	RTL/Component/Scheduling/CIFXTaskManagerNode.cpp
	RTL/Component/Scheduling/CIFXTaskManagerView.cpp
	RTL/Component/Scheduling/CIFXTimeManager.cpp
	RTL/Component/Scheduling/IFXScheduling.cpp
	RTL/Component/Scheduling/IFXSchedulingGuids.cpp
	RTL/IFXCorePluginStatic/IFXCorePluginStatic.cpp
	RTL/Platform/${U3D_PLATFORM}/Common/IFXOSUtilities.cpp
	RTL/Component/ModifierChain/CIFXSubject.cpp
	RTL/Component/ModifierChain/CIFXModifier.cpp
	RTL/Component/SceneGraph/CIFXMarker.cpp
	RTL/Kernel/DataTypes/IFXCoreArray.cpp
	RTL/Kernel/DataTypes/IFXCoreList.cpp
	RTL/Kernel/DataTypes/IFXFastAllocator.cpp
	RTL/Kernel/DataTypes/IFXListNode.cpp
	RTL/Kernel/DataTypes/IFXMatrix4x4.cpp
	RTL/Kernel/DataTypes/IFXQuaternion.cpp
	RTL/Kernel/DataTypes/IFXString.cpp
	RTL/Kernel/DataTypes/IFXUnitAllocator.cpp
	RTL/Kernel/DataTypes/IFXVector3.cpp
	RTL/Kernel/DataTypes/IFXVector4.cpp
	RTL/Dependencies/WildCards/wcmatch.cpp
	RTL/Kernel/Common/IFXDebug.cpp
	)
IF(WIN32)
  ADD_LIBRARY( IFXScheduling  MODULE ${IFXScheduling_SRCS} RTL/Platform/Win32/IFXScheduling/IFXScheduling.rc RTL/Platform/Win32/IFXScheduling/IFXScheduling.def )
  TARGET_LINK_LIBRARIES( IFXScheduling IFXCore )
ENDIF(WIN32)
IF(APPLE)
  ADD_LIBRARY( IFXScheduling  MODULE ${IFXScheduling_SRCS} )
  set_target_properties( IFXScheduling  PROPERTIES
	LINK_FLAGS "${MY_LINK_FLAGS} -exported_symbols_list ${CMAKE_CURRENT_SOURCE_DIR}/RTL/Platform/Mac32/IFXScheduling/IFXScheduling.def   -undefined dynamic_lookup" )
ENDIF(APPLE)
IF(UNIX AND NOT APPLE)
  ADD_LIBRARY( IFXScheduling  MODULE ${IFXScheduling_SRCS} )
  set_target_properties( IFXScheduling  PROPERTIES
	LINK_FLAGS "-Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/RTL/Platform/Lin32/IFXScheduling/IFXScheduling.list" )
ENDIF(UNIX AND NOT APPLE)
install(
	TARGETS IFXScheduling
	LIBRARY DESTINATION ${PLUGIN_DESTINATION}
)

INCLUDE_DIRECTORIES( RTL/Component/Include RTL/Kernel/Include RTL/Platform/Include
	IDTF
	IDTF/Include
	IDTF/Common )
SET( IDTFConverter_SRCS
	IDTF/FileParser.cpp
	IDTF/SceneConverter.cpp
	IDTF/PointSetResourceParser.cpp
	IDTF/UrlListParser.cpp
	IDTF/NodeParser.cpp
	IDTF/ModifierParser.cpp
	IDTF/PointSetConverter.cpp
	IDTF/MaterialParser.cpp
	IDTF/MetaDataConverter.cpp
	IDTF/MeshResourceParser.cpp
	IDTF/ResourceConverter.cpp
	IDTF/TextureConverter.cpp
	IDTF/ResourceListParser.cpp
	IDTF/File.cpp
	IDTF/LineSetConverter.cpp
	IDTF/ConverterDriver.cpp
	IDTF/ModelConverter.cpp
	IDTF/TextureParser.cpp
	IDTF/NodeConverter.cpp
	IDTF/MeshConverter.cpp
	IDTF/BlockParser.cpp
	IDTF/ModelResourceParser.cpp
	IDTF/FileReferenceConverter.cpp
	IDTF/ShaderParser.cpp
	IDTF/FileScanner.cpp
	IDTF/FileReferenceParser.cpp
	IDTF/ModifierConverter.cpp
	IDTF/MetaDataParser.cpp
	IDTF/LineSetResourceParser.cpp
		IDTF/BlockParser.h
		IDTF/DefaultSettings.h
		IDTF/File.h
		IDTF/FileParser.h
		IDTF/FileReferenceConverter.h
		IDTF/FileReferenceParser.h
		IDTF/FileScanner.h
		IDTF/IConverter.h
		IDTF/IFXResource.h
		IDTF/LineSetConverter.h
		IDTF/LineSetResourceParser.h
		IDTF/MaterialParser.h
		IDTF/MeshConverter.h
		IDTF/MeshResourceParser.h
		IDTF/MetaDataConverter.h
		IDTF/MetaDataParser.h
		IDTF/ModelConverter.h
		IDTF/ModelResourceParser.h
		IDTF/ModifierConverter.h
		IDTF/ModifierParser.h
		IDTF/NodeConverter.h
		IDTF/NodeParser.h
		IDTF/PointSetConverter.h
		IDTF/PointSetResourceParser.h
		IDTF/ResourceConverter.h
		IDTF/ResourceListParser.h
		IDTF/SceneConverter.h
		IDTF/ShaderParser.h
		IDTF/TextureConverter.h
		IDTF/TextureParser.h
		IDTF/UrlListParser.h
		IDTF/Include/ConverterHelpers.h
		IDTF/Include/ConverterOptions.h
		IDTF/Include/ConverterResult.h
		IDTF/Include/DebugInfo.h
		IDTF/Include/SceneUtilities.h
		IDTF/Include/TGAImage.h
		IDTF/Include/U3DHeaders.h
	IDTF/Helpers/MiscUtilities.cpp
	IDTF/Helpers/TGAImage.cpp
	IDTF/Helpers/ModifierUtilities.cpp
	IDTF/Helpers/ConverterHelpers.cpp
	IDTF/Helpers/SceneUtilities.cpp
	IDTF/Helpers/DebugInfo.cpp
	IDTF/Helpers/Guids.cpp
	IDTF/Common/GlyphModifier.cpp
	IDTF/Common/ModelResource.cpp
	IDTF/Common/ModifierList.cpp
	IDTF/Common/NodeList.cpp
	IDTF/Common/FileReference.cpp
	IDTF/Common/ResourceList.cpp
	IDTF/Common/SceneResources.cpp
	IDTF/Common/ModelResourceList.cpp
	IDTF/Common/MetaDataList.cpp
	IDTF/Common/ParentList.cpp
	IDTF/Common/GlyphCommandList.cpp
		IDTF/Common/AnimationModifier.h
		IDTF/Common/BoneWeightModifier.h
		IDTF/Common/CLODModifier.h
		IDTF/Common/Color.h
		IDTF/Common/FileReference.h
		IDTF/Common/GlyphCommandList.h
		IDTF/Common/GlyphCommands.h
		IDTF/Common/GlyphModifier.h
		IDTF/Common/INode.h
		IDTF/Common/Int2.h
		IDTF/Common/Int3.h
		IDTF/Common/IResource.h
		IDTF/Common/LightNode.h
		IDTF/Common/LightResource.h
		IDTF/Common/LightResourceList.h
		IDTF/Common/LineSetResource.h
		IDTF/Common/MaterialResource.h
		IDTF/Common/MaterialResourceList.h
		IDTF/Common/MeshResource.h
		IDTF/Common/MetaDataList.h
		IDTF/Common/ModelNode.h
		IDTF/Common/ModelResource.h
		IDTF/Common/ModelResourceList.h
		IDTF/Common/ModelSkeleton.h
		IDTF/Common/Modifier.h
		IDTF/Common/ModifierList.h
		IDTF/Common/MotionResource.h
		IDTF/Common/MotionResourceList.h
		IDTF/Common/Node.h
		IDTF/Common/NodeList.h
		IDTF/Common/ParentData.h
		IDTF/Common/ParentList.h
		IDTF/Common/Point.h
		IDTF/Common/PointSetResource.h
		IDTF/Common/Quat.h
		IDTF/Common/Resource.h
		IDTF/Common/ResourceList.h
		IDTF/Common/SceneData.h
		IDTF/Common/SceneResources.h
		IDTF/Common/ShaderResource.h
		IDTF/Common/ShaderResourceList.h
		IDTF/Common/ShadingDescription.h
		IDTF/Common/ShadingDescriptionList.h
		IDTF/Common/ShadingModifier.h
		IDTF/Common/SubdivisionModifier.h
		IDTF/Common/TextureResource.h
		IDTF/Common/TextureResourceList.h
		IDTF/Common/Tokens.h
		IDTF/Common/UrlList.h
		IDTF/Common/ViewNodeData.h
		IDTF/Common/ViewNode.h
		IDTF/Common/ViewResource.h
		IDTF/Common/ViewResourceList.h
	)
IF(WIN32)
  ADD_EXECUTABLE( IDTFConverter ${IDTFConverter_SRCS} IDTF/IDTFConverter.rc )
ENDIF(WIN32)
IF(APPLE)
  ADD_EXECUTABLE( IDTFConverter ${IDTFConverter_SRCS} )
    set_target_properties( IDTFConverter  PROPERTIES
	LINK_FLAGS "${MY_LINK_FLAGS} -exported_symbols_list /dev/null" )
ENDIF(APPLE)
IF(UNIX AND NOT APPLE)
  ADD_EXECUTABLE( IDTFConverter ${IDTFConverter_SRCS} )
ENDIF(UNIX AND NOT APPLE)
target_link_libraries( IDTFConverter  IFXCoreStatic ${CMAKE_DL_LIBS} )
add_dependencies( IDTFConverter  IFXCoreStatic )
install(
	TARGETS IDTFConverter 
	DESTINATION ${BIN_DESTINATION}
)

INCLUDE_DIRECTORIES( RTL/Component/Include RTL/Kernel/Include RTL/Platform/Include
	IDTF
	IDTF/Include
	IDTF/Common 
        libIDTF )
SET( libIDTF_SRCS
	libIDTF/SceneConverterLib.cpp
	libIDTF/SceneConverterLib.h
	libIDTF/Writer.cpp
	libIDTF/Writer.h
	libIDTF/IFXOSLoader.cpp
	libIDTF/IFXOSUtilities.cpp
	IDTF/PointSetConverter.cpp
	IDTF/MetaDataConverter.cpp
	IDTF/ResourceConverter.cpp
	IDTF/TextureConverter.cpp
	IDTF/File.cpp
	IDTF/LineSetConverter.cpp
	IDTF/ModelConverter.cpp
	IDTF/NodeConverter.cpp
	IDTF/MeshConverter.cpp
	IDTF/FileReferenceConverter.cpp
	IDTF/ModifierConverter.cpp
		IDTF/DefaultSettings.h
		IDTF/File.h
		IDTF/FileReferenceConverter.h
		IDTF/IConverter.h
		IDTF/LineSetConverter.h
		IDTF/MeshConverter.h
		IDTF/MetaDataConverter.h
		IDTF/ModelConverter.h
		IDTF/ModifierConverter.h
		IDTF/NodeConverter.h
		IDTF/PointSetConverter.h
		IDTF/ResourceConverter.h
		IDTF/TextureConverter.h
		IDTF/Include/ConverterOptions.h
		IDTF/Include/ConverterResult.h
		IDTF/Include/DebugInfo.h
		IDTF/Include/SceneUtilities.h
		IDTF/Include/TGAImage.h
		IDTF/Include/U3DHeaders.h
	IDTF/Helpers/MiscUtilities.cpp
	IDTF/Helpers/TGAImage.cpp
	IDTF/Helpers/ModifierUtilities.cpp
	IDTF/Helpers/SceneUtilities.cpp
	IDTF/Helpers/DebugInfo.cpp
	IDTF/Helpers/Guids.cpp
	IDTF/Common/GlyphModifier.cpp
	IDTF/Common/ModelResource.cpp
	IDTF/Common/ModifierList.cpp
	IDTF/Common/NodeList.cpp
	IDTF/Common/FileReference.cpp
	IDTF/Common/ResourceList.cpp
	IDTF/Common/SceneResources.cpp
	IDTF/Common/ModelResourceList.cpp
	IDTF/Common/MetaDataList.cpp
	IDTF/Common/ParentList.cpp
	IDTF/Common/GlyphCommandList.cpp
		IDTF/Common/AnimationModifier.h
		IDTF/Common/BoneWeightModifier.h
		IDTF/Common/CLODModifier.h
		IDTF/Common/Color.h
		IDTF/Common/FileReference.h
		IDTF/Common/GlyphCommandList.h
		IDTF/Common/GlyphCommands.h
		IDTF/Common/GlyphModifier.h
		IDTF/Common/INode.h
		IDTF/Common/Int2.h
		IDTF/Common/Int3.h
		IDTF/Common/IResource.h
		IDTF/Common/LightNode.h
		IDTF/Common/LightResource.h
		IDTF/Common/LightResourceList.h
		IDTF/Common/LineSetResource.h
		IDTF/Common/MaterialResource.h
		IDTF/Common/MaterialResourceList.h
		IDTF/Common/MeshResource.h
		IDTF/Common/MetaDataList.h
		IDTF/Common/ModelNode.h
		IDTF/Common/ModelResource.h
		IDTF/Common/ModelResourceList.h
		IDTF/Common/ModelSkeleton.h
		IDTF/Common/Modifier.h
		IDTF/Common/ModifierList.h
		IDTF/Common/MotionResource.h
		IDTF/Common/MotionResourceList.h
		IDTF/Common/Node.h
		IDTF/Common/NodeList.h
		IDTF/Common/ParentData.h
		IDTF/Common/ParentList.h
		IDTF/Common/Point.h
		IDTF/Common/PointSetResource.h
		IDTF/Common/Quat.h
		IDTF/Common/Resource.h
		IDTF/Common/ResourceList.h
		IDTF/Common/SceneData.h
		IDTF/Common/SceneResources.h
		IDTF/Common/ShaderResource.h
		IDTF/Common/ShaderResourceList.h
		IDTF/Common/ShadingDescription.h
		IDTF/Common/ShadingDescriptionList.h
		IDTF/Common/ShadingModifier.h
		IDTF/Common/SubdivisionModifier.h
		IDTF/Common/TextureResource.h
		IDTF/Common/TextureResourceList.h
		IDTF/Common/Tokens.h
		IDTF/Common/UrlList.h
		IDTF/Common/ViewNodeData.h
		IDTF/Common/ViewNode.h
		IDTF/Common/ViewResource.h
		IDTF/Common/ViewResourceList.h
	RTL/Dependencies/WildCards/wcmatch.cpp
	RTL/Dependencies/WildCards/wcmatch.h
		RTL/Component/Base/IFXCoincidentVertexMap.cpp
		RTL/Component/Base/IFXFatCornerIter.cpp
		RTL/Component/Base/IFXVectorHasher.h
		RTL/Component/Base/IFXCornerIter.cpp
		RTL/Component/Base/IFXTransform.cpp
		RTL/Component/Base/IFXVertexMap.cpp
		RTL/Component/Base/IFXEuler.cpp
		RTL/Component/Base/IFXVectorHasher.cpp
		RTL/Component/Base/IFXVertexMapGroup.cpp
	RTL/Kernel/DataTypes/IFXCoreArray.cpp
	RTL/Kernel/DataTypes/IFXFastAllocator.cpp
	RTL/Kernel/DataTypes/IFXMatrix4x4.cpp
	RTL/Kernel/DataTypes/IFXString.cpp
	RTL/Kernel/DataTypes/IFXVector3.cpp
	RTL/Kernel/DataTypes/IFXCoreList.cpp
	RTL/Kernel/DataTypes/IFXListNode.cpp
	RTL/Kernel/DataTypes/IFXQuaternion.cpp
	RTL/Kernel/DataTypes/IFXUnitAllocator.cpp
	RTL/Kernel/DataTypes/IFXVector4.cpp
		RTL/Component/Common/IFXDids.cpp
	RTL/Kernel/Common/IFXDebug.cpp
		RTL/IFXCoreStatic/IFXCoreStatic.cpp
	)
SET( libIDTF_HDRS
	libIDTF/SceneConverterLib.h
	libIDTF/Writer.h
		IDTF/DefaultSettings.h
		IDTF/File.h
		IDTF/FileReferenceConverter.h
		IDTF/IConverter.h
		IDTF/LineSetConverter.h
		IDTF/MeshConverter.h
		IDTF/MetaDataConverter.h
		IDTF/ModelConverter.h
		IDTF/ModifierConverter.h
		IDTF/NodeConverter.h
		IDTF/PointSetConverter.h
		IDTF/ResourceConverter.h
		IDTF/TextureConverter.h
		IDTF/Include/ConverterOptions.h
		IDTF/Include/ConverterResult.h
		IDTF/Include/DebugInfo.h
		IDTF/Include/SceneUtilities.h
		IDTF/Include/TGAImage.h
		IDTF/Include/U3DHeaders.h
		IDTF/Common/AnimationModifier.h
		IDTF/Common/BoneWeightModifier.h
		IDTF/Common/CLODModifier.h
		IDTF/Common/Color.h
		IDTF/Common/FileReference.h
		IDTF/Common/GlyphCommandList.h
		IDTF/Common/GlyphCommands.h
		IDTF/Common/GlyphModifier.h
		IDTF/Common/INode.h
		IDTF/Common/Int2.h
		IDTF/Common/Int3.h
		IDTF/Common/IResource.h
		IDTF/Common/LightNode.h
		IDTF/Common/LightResource.h
		IDTF/Common/LightResourceList.h
		IDTF/Common/LineSetResource.h
		IDTF/Common/MaterialResource.h
		IDTF/Common/MaterialResourceList.h
		IDTF/Common/MeshResource.h
		IDTF/Common/MetaDataList.h
		IDTF/Common/ModelNode.h
		IDTF/Common/ModelResource.h
		IDTF/Common/ModelResourceList.h
		IDTF/Common/ModelSkeleton.h
		IDTF/Common/Modifier.h
		IDTF/Common/ModifierList.h
		IDTF/Common/MotionResource.h
		IDTF/Common/MotionResourceList.h
		IDTF/Common/Node.h
		IDTF/Common/NodeList.h
		IDTF/Common/ParentData.h
		IDTF/Common/ParentList.h
		IDTF/Common/Point.h
		IDTF/Common/PointSetResource.h
		IDTF/Common/Quat.h
		IDTF/Common/Resource.h
		IDTF/Common/ResourceList.h
		IDTF/Common/SceneData.h
		IDTF/Common/SceneResources.h
		IDTF/Common/ShaderResource.h
		IDTF/Common/ShaderResourceList.h
		IDTF/Common/ShadingDescription.h
		IDTF/Common/ShadingDescriptionList.h
		IDTF/Common/ShadingModifier.h
		IDTF/Common/SubdivisionModifier.h
		IDTF/Common/TextureResource.h
		IDTF/Common/TextureResourceList.h
		IDTF/Common/Tokens.h
		IDTF/Common/UrlList.h
		IDTF/Common/ViewNodeData.h
		IDTF/Common/ViewNode.h
		IDTF/Common/ViewResource.h
		IDTF/Common/ViewResourceList.h
	RTL/Dependencies/WildCards/wcmatch.h
		RTL/Component/Base/IFXVectorHasher.h
	)
SET_PROPERTY( SOURCE
	libIDTF/IFXOSLoader.cpp
	PROPERTY COMPILE_DEFINITIONS U3DPluginsPath="Plugins" U3DCorePath="." )
ADD_LIBRARY( IDTF ${libIDTF_SRCS} )
SET_PROPERTY(
	TARGET IDTF
	PROPERTY COMPILE_DEFINITIONS LIBIDTF )
install(
	TARGETS IDTF
	ARCHIVE DESTINATION ${LIB_DESTINATION}
	LIBRARY DESTINATION ${LIB_DESTINATION}
)
ADD_EXECUTABLE( IDTFGen Samples/SampleCode/IDTFGen.cpp )
target_link_libraries( IDTFGen  IDTF ${CMAKE_DL_LIBS} )
add_dependencies( IDTFGen  IDTF )

install(
	TARGETS IDTFGen 
	DESTINATION ${BIN_DESTINATION}
)


#INCLUDE_DIRECTORIES( RTL/Component/Include RTL/Kernel/Include RTL/Platform/Include )
ADD_EXECUTABLE( HelloWorld Samples/SampleCode/HelloWorld.cpp )
target_link_libraries( HelloWorld  IFXCoreStatic ${CMAKE_DL_LIBS} )
add_dependencies( HelloWorld  IFXCoreStatic )

install(
	TARGETS HelloWorld 
	DESTINATION ${BIN_DESTINATION}
)

INSTALL( DIRECTORY RTL/Component/Include/ RTL/Kernel/Include/ RTL/Platform/Include/
	DESTINATION ${INCLUDE_DESTINATION}
	FILES_MATCHING PATTERN "*.h"
)
INSTALL( FILES RTL/Platform/${U3D_PLATFORM}/IFXRendering/OpenGL/IFXOpenGLOS.h
	DESTINATION ${INCLUDE_DESTINATION}
)
INSTALL( FILES ${libIDTF_HDRS}
	DESTINATION ${INCLUDE_DESTINATION}
)
INSTALL( FILES Samples/SampleCode/HelloWorld.cpp Samples/SampleCode/IDTFGen.cpp Samples/SampleCode/Makefile.sample
	DESTINATION ${SAMPLE_DESTINATION}/SampleCode
)
INSTALL( DIRECTORY Samples/TestScenes
	DESTINATION ${SAMPLE_DESTINATION}
	PATTERN "Makef*" EXCLUDE
)
INSTALL( DIRECTORY Docs/
	DESTINATION ${DOC_DESTINATION}
	PATTERN "Makef*" EXCLUDE
)
