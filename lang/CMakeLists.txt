if(use_python)
	add_custom_command(OUTPUT mgl_python.cpp mathgl.py
		COMMAND ${SWIG_EXECUTABLE} -python -c++ -o mgl_python.cpp mgl.i
		MAIN_DEPENDENCY mgl.i
		DEPENDS mgl_graph.i
		IMPLICIT_DEPENDS CXX ../include/mgl/mgl_c.h ../include/mgl/mgl_data.h
	)
	add_custom_target(_mathgl.so ALL
		COMMAND ${PYTHON_EXECUTABLE} setup.py build
		DEPENDS mathgl.py mgl_python.cpp
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	)
	install(CODE "execute_process(COMMAND ${PYTHON_EXECUTABLE} setup.py install --prefix=${CMAKE_INSTALL_PREFIX} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})")
endif(use_python)
