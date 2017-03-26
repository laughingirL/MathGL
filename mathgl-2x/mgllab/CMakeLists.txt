# cmake_minimum_required(VERSION 2.8.9)
# 
# project( MGLLAB )
# 
# include(FindFLTK)
# if(NOT FLTK_FOUND)
# 	message(SEND_ERROR "Couldn't find FLTK library.")
# endif(NOT FLTK_FOUND)
# 
# include_directories( ${MGLLAB_SOURCE_DIR} ${MGLLAB_BINARY_DIR})
# 
# find_package(MathGL2 REQUIRED FLTK)
# #find_package(MathGL REQUIRED FLTK PATHS /usr/local/lib/CMake/MathGL/)
# 
# find_package(OpenMP)
# if(OPENMP_FOUND)
# 	set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
# 	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
# else(OPENMP_FOUND)
# 	message(SEND_ERROR "Couldn't find OpenMP.")
# endif(OPENMP_FOUND)
# 
# include_directories(${FLTK_INCLUDE_DIR})
# set(mgl_lab_src animate.cpp editor.cpp help.cpp mathgl.cpp setup.cpp write.cpp data.cpp grid.cpp dialogs.cpp mgllab.cpp option.cpp table.cpp image.cpp)
# set(mgl_lab_hdr mgllab.h image.h)
# add_executable(mgllab ${mgl_lab_src} ${mgl_lab_hdr})
# target_link_libraries(mgllab ${MATHGL2_FLTK_LIBRARY} ${MATHGL2_LIBRARY} ${FLTK_LIBRARIES})
# install(
# 	TARGETS mgllab
# 	RUNTIME DESTINATION bin	)

if(MGL_HAVE_FLTK)
	include_directories(${FLTK_INCLUDE_DIR})
	set(mgl_lab_src animate.cpp editor.cpp help.cpp mathgl.cpp setup.cpp write.cpp data.cpp grid.cpp dialogs.cpp mgllab.cpp option.cpp table.cpp image.cpp)
	set(mgl_lab_hdr mgllab.h image.h)
	add_executable(mgllab ${mgl_lab_src} ${mgl_lab_hdr})
	target_link_libraries(mgllab mgl mgl-fltk ${FLTK_LIBRARIES})
	install(
		TARGETS mgllab
		RUNTIME DESTINATION bin	)
endif(MGL_HAVE_FLTK)
