if(use_python)
	INCLUDE(${SWIG_USE_FILE})
	INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
	INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
	SET(CMAKE_SWIG_FLAGS "")

	SET_SOURCE_FILES_PROPERTIES(mgl.i PROPERTIES CPLUSPLUS ON)
#	SET_SOURCE_FILES_PROPERTIES(example.i PROPERTIES SWIG_FLAGS "-includeall")
	SWIG_ADD_MODULE(mathgl python mgl.i ../include/mgl/type.h ../include/mgl/data.h ../include/mgl/mgl.h)
	SWIG_LINK_LIBRARIES(mathgl ${PYTHON_LIBRARIES})

# 	add_custom_command(OUTPUT mgl_python.cpp mathgl.py
# 		COMMAND ${SWIG_EXECUTABLE} -python -c++ -I${CMAKE_SOURCE_DIR}/include -o mgl_python.cpp ${CMAKE_SOURCE_DIR}/lang/mgl.i
# 		MAIN_DEPENDENCY mgl.i
# 		IMPLICIT_DEPENDS CXX ../include/mgl/data.h ../include/mgl/mgl.h
# #		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/lang
# 	)
# 	add_custom_target(_mathgl.so ALL
# 		COMMAND CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR} ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/setup.py build
# #		COMMAND CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR} ${PYTHON_EXECUTABLE}
# 		DEPENDS ${CMAKE_BINARY_DIR}/lang/mathgl.py ${CMAKE_BINARY_DIR}/lang/mgl_python.cpp
# 		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
# 	)
# 	install(CODE "execute_process(COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/setup.py install --prefix=${CMAKE_INSTALL_PREFIX} WORKING_DIRECTORY ${CMAKE_BINARY_DIR} )")
endif(use_python)
