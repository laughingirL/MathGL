set(mgl_src 
	mgl_1d.cpp    mgl_combi.cpp    mgl_data_png.cpp  mgl_f.cpp     mgl_zb2.cpp
	mgl_2d.cpp    mgl_cont.cpp     mgl_def_font.cpp  mgl_fit.cpp   mgl_main.cpp       mgl_zb.cpp
	mgl_3d.cpp    mgl_crust.cpp    mgl_eps.cpp       mgl_flow.cpp  mgl_parse.cpp
	mgl_ab.cpp    mgl_data_cf.cpp  mgl_eval.cpp      mgl_font.cpp  mgl_pde.cpp
	mgl_axis.cpp  mgl_data.cpp     mgl_exec.cpp      mgl_gl.cpp    mgl_tex_table.cpp
	mgl_c.cpp     mgl_data_io.cpp  mgl_export.cpp    mgl_idtf.cpp  mgl_vect.cpp
)

add_library(mgl SHARED ${mgl_src})

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

if(use_jpeg)
	target_link_libraries(mgl ${JPEG_LIBRARIES})
	add_definitions(-DHAVE_JPEG)
	include_directories(${JPEG_INCLUDE_DIR})
endif(use_jpeg)

if(use_hdf5)
	target_link_libraries(mgl ${HDF5_LIB})
	add_definitions(-DHAVE_HDF5)
	include_directories(${HDF5_INCLUDE_DIR})
endif(use_hdf5)

target_link_libraries(mgl ${GSL_LIB} ${GSL_CBLAS_LIB} ${OPENGL_LIBRARIES} ${PNG_LIBRARIES} m)
include_directories(${GSL_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${PNG_INCLUDE_DIR})
file(TO_NATIVE_PATH ${MGL_FONT_PATH} MGL_FONT_PATH_SYS)
add_definitions(-DMGL_FONT_PATH="${MGL_FONT_PATH_SYS}")

if(UNIX)
	add_definitions(-DNO_COLOR_ARRAY)
endif(UNIX)

#set_target_properties(mgl PROPERTIES SOVERSION 4.2.1)
install(
	TARGETS mgl
	LIBRARY DESTINATION lib
)
