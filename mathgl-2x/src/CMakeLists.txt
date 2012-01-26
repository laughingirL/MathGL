set(mgl_src
addon.cpp axis.cpp base_cf.cpp base.cpp canvas_cf.cpp canvas.cpp cont.cpp crust.cpp
data.cpp data_io.cpp data_new.cpp data_op.cpp data_png.cpp def_font.cpp
export_2d.cpp export_3d.cpp eval.cpp evalp.cpp exec.cpp export.cpp
fit.cpp font.cpp opengl.cpp other.cpp parser.cpp pde.cpp pixel.cpp
plot.cpp prim.cpp surf.cpp tex_table.cpp vect.cpp volume.cpp evalc.cpp #window.cpp
)

set(mgl_hdr
../include/mgl/base_cf.h    ../include/mgl/fit.h     ../include/mgl/plot.h
../include/mgl/base.h       ../include/mgl/prim.h    ../include/mgl/canvas_cf.h
../include/mgl/font.h       ../include/mgl/canvas.h  ../include/mgl/surf.h
../include/mgl/config.h     ../include/mgl/mgl_cf.h  ../include/mgl/type.h
../include/mgl/cont.h       ../include/mgl/mgl.h     ../include/mgl/vect.h
../include/mgl/data.h       ../include/mgl/opengl.h  ../include/mgl/volume.h
../include/mgl/define.h     ../include/mgl/other.h   ../include/mgl/eval.h
../include/mgl/parser.h     ../include/mgl/addon.h   ../include/mgl/evalc.h )

add_library(mgl SHARED ${mgl_src})
add_library(mgl-static STATIC ${mgl_src})
set_target_properties(mgl-static PROPERTIES OUTPUT_NAME "mgl")
set_target_properties(mgl PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(mgl-static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

if(use_ltdl)
	target_link_libraries(mgl ${LTDL_LIB})
	add_definitions(-DHAVE_LTDL)
	include_directories(${LTDL_INCLUDE_DIR})
endif(use_ltdl)

if(use_u3d)
#	include_directories( IDTFGen ${U3D_INCLUDE_DIR} )
	add_definitions(-DHAVE_U3D)
	target_link_libraries(mgl ${U3D_LIB})
	target_link_libraries(mgl-static ${U3D_LIB})
	include_directories(${U3D_INCLUDE_DIR})
	if(HPDF_INCLUDE_DIR AND HPDF_LIB)
		include_directories(${HPDF_INCLUDE_DIR})
		target_link_libraries(mgl ${HPDF_LIB})
		add_definitions(-DHAVE_HPDF_H)
	endif(HPDF_INCLUDE_DIR AND HPDF_LIB)
endif(use_u3d)

if(use_qt)
	add_definitions(-DHAVE_QT)
endif(use_qt)
if(use_wx)
	add_definitions(-DHAVE_WX)
endif(use_wx)
if(use_fltk)
	add_definitions(-DHAVE_FLTK)
endif(use_fltk)
if(use_glut)
	add_definitions(-DHAVE_GLUT)
endif(use_glut)

if(use_pthread)
	target_link_libraries(mgl ${CMAKE_THREAD_LIBS_INIT})
	add_definitions(-DHAVE_PTHREAD)
endif(use_pthread)

if(use_jpeg)
	target_link_libraries(mgl ${JPEG_LIBRARIES})
	add_definitions(-DHAVE_JPEG)
	include_directories(${JPEG_INCLUDE_DIR})
endif(use_jpeg)

if(use_gif)
	target_link_libraries(mgl ${GIF_LIBRARIES})
	add_definitions(-DHAVE_GIF)
	include_directories(${GIF_INCLUDE_DIR})
endif(use_gif)

if(use_hdf5 OR use_hdf5_18)
	target_link_libraries(mgl ${HDF5_LIB})
	add_definitions(-DHAVE_HDF5)
	if(use_hdf5)
		add_definitions(-DH5_USE_16_API)
	endif(use_hdf5)
	include_directories(${HDF5_INCLUDE_DIR})
endif(use_hdf5 OR use_hdf5_18)

if(use_hdf4)
	target_link_libraries(mgl ${HDF4MF_LIB} ${HDF4_LIB})
	add_definitions(-DHAVE_HDF4)
	include_directories(${HDF4_INCLUDE_DIR})
endif(use_hdf4)

if(use_gsl)
	target_link_libraries(mgl ${GSL_LIB} ${GSL_CBLAS_LIB} )
	add_definitions(-DHAVE_GSL)
	include_directories(${GSL_INCLUDE_DIR})
else(use_gsl)
	add_definitions(-DNO_GSL)
endif(use_gsl)

target_link_libraries(mgl ${OPENGL_LIBRARIES} ${PNG_LIBRARIES} m)
include_directories(${OPENGL_INCLUDE_DIR} ${PNG_INCLUDE_DIR})
file(TO_NATIVE_PATH $(MGL_FONT_PATH) MGL_FONT_PATH_SYS)
#add_definitions(-DMGL_FONT_PATH="${MGL_FONT_PATH_SYS}")

if(UNIX)
	add_definitions(-DNO_COLOR_ARRAY)
endif(UNIX)

set_target_properties(mgl PROPERTIES SOVERSION 6.0.0)
install(
	TARGETS mgl mgl-static
	RUNTIME DESTINATION bin
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
)
