set(mgl_src
	addon.cpp axis.cpp base_cf.cpp base.cpp canvas_cf.cpp canvas.cpp cont.cpp crust.cpp
	data.cpp data_io.cpp data_new.cpp data_png.cpp def_font.cpp
	export_2d.cpp export_3d.cpp eval.cpp evalp.cpp exec.cpp export.cpp
	fit.cpp font.cpp other.cpp parser.cpp pde.cpp pixel.cpp
	plot.cpp prim.cpp surf.cpp tex_table.cpp vect.cpp volume.cpp evalc.cpp
	s_hull/s_hull_pro.cpp
)

set(mgl_hdr
	../include/mgl2/base_cf.h	../include/mgl2/fit.h		../include/mgl2/plot.h
	../include/mgl2/base.h		../include/mgl2/prim.h		../include/mgl2/canvas_cf.h
	../include/mgl2/font.h		../include/mgl2/canvas.h	../include/mgl2/surf.h
	../include/mgl2/mgl_cf.h	../include/mgl2/type.h		${MathGL_BINARY_DIR}/include/mgl2/config.h
	../include/mgl2/cont.h		../include/mgl2/mgl.h		../include/mgl2/vect.h
	../include/mgl2/data.h		../include/mgl2/volume.h	../include/mgl2/data_cf.h
	../include/mgl2/define.h	../include/mgl2/other.h		../include/mgl2/eval.h
	../include/mgl2/parser.h	../include/mgl2/addon.h		../include/mgl2/evalc.h
	s_hull/s_hull_pro.h
)

add_definitions(-DMGL_SRC)

if(MGL_HAVE_PNG)
	set(prc_src prc/PRCbitStream.cc prc/PRCdouble.cc prc/oPRCFile.cc prc/writePRC.cc prc.cpp )
	set(prc_hdr prc/PRC.h prc/PRCbitStream.h prc/PRCdouble.h prc/oPRCFile.h prc/writePRC.h )

	set(mgl_src ${mgl_src} ${prc_src} )
	set(mgl_hdr ${mgl_hdr} ${prc_hdr} )
	include_directories(prc)
endif(MGL_HAVE_PNG)

if(MGL_HAVE_OPENGL)
	set(mgl_src ${mgl_src} opengl.cpp )
	set(mgl_hdr ${mgl_hdr} ../include/mgl2/opengl.h )
endif(MGL_HAVE_OPENGL)

add_library(mgl SHARED ${mgl_src} ${mgl_hdr})
add_library(mgl-static STATIC ${mgl_src} ${mgl_hdr})
set_target_properties(mgl-static PROPERTIES OUTPUT_NAME "mgl")
set_target_properties(mgl PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(mgl-static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

if(MGL_HAVE_LTDL)
	target_link_libraries(mgl ${LTDL_LIB})
	include_directories(${LTDL_INCLUDE_DIR})
endif(MGL_HAVE_LTDL)

if(MGL_HAVE_U3D)
#	include_directories( IDTFGen ${U3D_INCLUDE_DIR} )
	target_link_libraries(mgl ${U3D_LIB})
	target_link_libraries(mgl-static ${U3D_LIB})
	include_directories(${U3D_INCLUDE_DIR})
endif(MGL_HAVE_U3D)

if(MGL_HAVE_PDF)
	include_directories(${HPDF_INCLUDE_DIR})
	target_link_libraries(mgl ${HPDF_LIB})
endif(MGL_HAVE_PDF)

if(MGL_HAVE_PTHREAD)
	target_link_libraries(mgl ${CMAKE_THREAD_LIBS_INIT})
endif(MGL_HAVE_PTHREAD)

if(MGL_HAVE_JPEG)
	target_link_libraries(mgl ${JPEG_LIBRARIES})
	include_directories(${JPEG_INCLUDE_DIR})
endif(MGL_HAVE_JPEG)

if(MGL_HAVE_GIF)
	target_link_libraries(mgl ${GIF_LIBRARIES})
	include_directories(${GIF_INCLUDE_DIR})
endif(MGL_HAVE_GIF)

if(MGL_HAVE_HDF5)
	target_link_libraries(mgl ${HDF5_LIBRARIES})
#	if(NOT use_hdf5_18)
#		add_definitions(-DH5_USE_16_API)
#	endif(NOT use_hdf5_18)
	include_directories(${HDF5_INCLUDE_DIR})
endif(MGL_HAVE_HDF5)

if(MGL_HAVE_HDF4)
	target_link_libraries(mgl ${HDF4MF_LIB} ${HDF4_LIB})
	include_directories(${HDF4_INCLUDE_DIR})
endif(MGL_HAVE_HDF4)

if(MGL_HAVE_GSL)
	target_link_libraries(mgl ${GSL_LIB} ${GSL_CBLAS_LIB} )
	include_directories(${GSL_INCLUDE_DIR})
endif(MGL_HAVE_GSL)

if(MGL_HAVE_OPENGL)
	target_link_libraries(mgl ${OPENGL_LIBRARIES} )
	include_directories(${OPENGL_INCLUDE_DIR} )
endif(MGL_HAVE_OPENGL)

if(MGL_HAVE_PNG)
	target_link_libraries(mgl ${PNG_LIBRARIES} )
	include_directories(${PNG_INCLUDE_DIR})
endif(MGL_HAVE_PNG)

if(MGL_HAVE_ZLIB)
	target_link_libraries(mgl ${ZLIB_LIBRARIES} )
	include_directories(${ZLIB_INCLUDE_DIR})
endif(MGL_HAVE_ZLIB)

if(MGL_HAVE_MPI)
	target_link_libraries(mgl ${MPI_LIBRARIES} )
#	include_directories(${MPI_C_INCLUDE_PATH})
	include_directories(${MPI_CXX_INCLUDE_PATH})
endif(MGL_HAVE_MPI)

target_link_libraries(mgl ${M_LIB})
file(TO_NATIVE_PATH $(MGL_FONT_PATH) MGL_FONT_PATH_SYS)
#add_definitions(-DMGL_FONT_PATH="${MGL_FONT_PATH_SYS}")

set_target_properties(mgl PROPERTIES SOVERSION 6.0.0)
install(
	TARGETS mgl mgl-static
	RUNTIME DESTINATION bin
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
)
