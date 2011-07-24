if(use_python)
	add_custom_command(OUTPUT mgl_python.cpp mathgl.py
		COMMAND ${SWIG_EXECUTABLE} -python -c++ -I${CMAKE_SOURCE_DIR}/include -o mgl_python.cpp ${CMAKE_SOURCE_DIR}/lang/mgl.i
		MAIN_DEPENDENCY mgl.i
		IMPLICIT_DEPENDS CXX ../include/mgl/mgl_c.h ../include/mgl/mgl_data.h
#		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/lang
	)
	add_custom_target(_mathgl.so ALL
		COMMAND CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR} ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/setup.py build
		DEPENDS ${CMAKE_BINARY_DIR}/lang/mathgl.py ${CMAKE_BINARY_DIR}/lang/mgl_python.cpp
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	)
	install(CODE "execute_process(COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/setup.py install --prefix=${CMAKE_INSTALL_PREFIX} WORKING_DIRECTORY ${CMAKE_BINARY_DIR} )")
endif(use_python)
