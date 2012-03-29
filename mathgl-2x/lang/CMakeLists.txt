if(MGL_HAVE_PYTHON)
	configure_file(${MathGL_SOURCE_DIR}/lang/mgl.i.in ${MathGL_BINARY_DIR}/lang/mgl.i COPYONLY)
	configure_file(${MathGL_SOURCE_DIR}/lang/numpy.i.in ${MathGL_BINARY_DIR}/lang/numpy.i COPYONLY)
	SET(dep_libs mgl)
#	if(MGL_HAVE_FLTK)
		get_property(path_to_mglwnd TARGET mgl-wnd PROPERTY LOCATION)
		file(TO_NATIVE_PATH ${path_to_mglwnd} MGL_LIB_WND)
		SET(dep_libs ${dep_libs} mgl-wnd)
#	endif(MGL_HAVE_FLTK)
	get_property(path_to_mgl TARGET mgl PROPERTY LOCATION)
	file(TO_NATIVE_PATH ${path_to_mgl} MGL_LIB)
	file(TO_NATIVE_PATH ${MathGL_BINARY_DIR}/lang/mgl.i MGL_I)
	file(TO_NATIVE_PATH ${MathGL_SOURCE_DIR}/include SRC_INC)
	file(TO_NATIVE_PATH ${MathGL_BINARY_DIR}/include BIN_INC)
	configure_file(${MathGL_SOURCE_DIR}/lang/setup.py.in ${MathGL_BINARY_DIR}/lang/setup.py)
#	file(COPY ${CMAKE_SOURCE_DIR}/lang/mgl.i ${CMAKE_SOURCE_DIR}/lang/numpy.i DESTINATION ${CMAKE_BINARY_DIR}/lang)
#	INCLUDE(${SWIG_USE_FILE})
#	INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
#	INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
#	SET(CMAKE_SWIG_FLAGS "-c++")
#	SET_SOURCE_FILES_PROPERTIES(mgl.i PROPERTIES CPLUSPLUS ON)
#	SWIG_ADD_MODULE(mathgl python mgl.i ../include/mgl/type.h ../include/mgl/data.h ../include/mgl/mgl.h)
#	SWIG_LINK_LIBRARIES(mathgl ${PYTHON_LIBRARIES})


# 	add_custom_command(OUTPUT mgl_python.cpp mathgl.py
# 		COMMAND ${SWIG_EXECUTABLE} -python -c++ -I${CMAKE_SOURCE_DIR}/include -o mgl_python.cpp ${CMAKE_SOURCE_DIR}/lang/mgl.i
# 		MAIN_DEPENDENCY mgl.i
# 		IMPLICIT_DEPENDS CXX ../include/mgl/type.h ../include/mgl/data.h ../include/mgl/mgl.h
# 	)
 	add_custom_command(OUTPUT _mathgl.so mathgl.py
		COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_BINARY_DIR}/lang/setup.py build
		DEPENDS ${dep_libs} ${CMAKE_BINARY_DIR}/lang/mgl.i ${CMAKE_BINARY_DIR}/lang/numpy.i
		IMPLICIT_DEPENDS CXX ${CMAKE_SOURCE_DIR}/include/mgl/type.h ${CMAKE_SOURCE_DIR}/include/mgl/data.h ${CMAKE_SOURCE_DIR}/include/mgl/mgl.h ${CMAKE_SOURCE_DIR}/include/mgl/window.h
 		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/lang
 	)
	add_custom_target(mgl_python_module ALL DEPENDS _mathgl.so mathgl.py)
	install(CODE "execute_process(COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_BINARY_DIR}/lang/setup.py install --prefix=${CMAKE_INSTALL_PREFIX} WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/lang )")
	set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "mgl.i;numpy.i;setup.py;mgl_wrap.cpp;build;mathgl.pyc")
endif(MGL_HAVE_PYTHON)
