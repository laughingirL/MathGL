set(mgl_src 
mgl_1d.cpp     mgl_combi.cpp     mgl_def_font.cpp  mgl_fit.cpp   mgl_parse.cpp
mgl_2d.cpp     mgl_cont.cpp      mgl_eps.cpp       mgl_flow.cpp  mgl_pde.cpp
mgl_3d.cpp     mgl_crust.cpp     mgl_evalc.cpp     mgl_font.cpp  mgl_tex_table.cpp
mgl_ab.cpp     mgl_data_cf.cpp   mgl_eval.cpp      mgl_gl.cpp    mgl_vect.cpp
mgl_addon.cpp  mgl_data.cpp      mgl_exec.cpp      mgl_idtf.cpp  mgl_zb2.cpp
mgl_axis.cpp   mgl_data_io.cpp   mgl_export.cpp    mgl_zb.cpp    mgl_evalp.cpp
mgl_c.cpp      mgl_data_png.cpp  mgl_f.cpp         mgl_main.cpp
)

add_library(mgl SHARED ${mgl_src})
add_library(mgl-static STATIC ${mgl_src})
set_target_properties(mgl-static PROPERTIES OUTPUT_NAME "mgl")
set_target_properties(mgl PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(mgl-static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

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
add_definitions(-DMGL_FONT_PATH="${MGL_FONT_PATH_SYS}")

if(UNIX)
	add_definitions(-DNO_COLOR_ARRAY)
endif(UNIX)

set_target_properties(mgl PROPERTIES SOVERSION 5.0.0)
install(
	TARGETS mgl
	RUNTIME DESTINATION bin
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
)
