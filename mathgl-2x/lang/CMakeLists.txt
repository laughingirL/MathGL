set(src_dep
${CMAKE_BINARY_DIR}/lang/mgl.i
${CMAKE_BINARY_DIR}/lang/numpy.i
)
set(src_imp_dep
${CMAKE_SOURCE_DIR}/include/mgl/type.h
${CMAKE_SOURCE_DIR}/include/mgl/data.h
${CMAKE_SOURCE_DIR}/include/mgl/mgl.h
${CMAKE_SOURCE_DIR}/include/mgl/mathgl.h
${CMAKE_SOURCE_DIR}/include/mgl/window.h
)
configure_file(${MathGL_SOURCE_DIR}/lang/mgl.i.in ${MathGL_BINARY_DIR}/lang/mgl.i COPYONLY)
configure_file(${MathGL_SOURCE_DIR}/lang/numpy.i.in ${MathGL_BINARY_DIR}/lang/numpy.i COPYONLY)
file(TO_NATIVE_PATH ${MathGL_BINARY_DIR}/lang/mgl.i MGL_I)

file(TO_NATIVE_PATH ${MathGL_SOURCE_DIR}/include SRC_INC)
file(TO_NATIVE_PATH ${MathGL_BINARY_DIR}/include BIN_INC)

get_directory_property(DEF_LIST COMPILE_DEFINITIONS)
foreach(def_i ${DEF_LIST})
	if(MGL_DEF)
		set(MGL_DEF ${MGL_DEF} -D${def_i})
	else(MGL_DEF)
		set(MGL_DEF -D${def_i})
	endif(MGL_DEF)
endforeach()
SET(dep_libs mgl-wnd mgl)
if(MGL_HAVE_PYTHON)
	configure_file(${MathGL_SOURCE_DIR}/lang/setup.py.in ${MathGL_BINARY_DIR}/lang/setup.py)
 	add_custom_command(OUTPUT _mathgl.so mathgl.py
		COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_BINARY_DIR}/lang/setup.py
		build -f build_ext 
		--include-dirs='${BIN_INC}:${SRC_INC}'
		--library-dirs='$<TARGET_LINKER_FILE_DIR:mgl>:$<TARGET_LINKER_FILE_DIR:mgl-wnd>'
		--libraries='mgl-wnd mgl'
		--swig ${SWIG_EXECUTABLE}
		--swig-opts='-c++ -threads -I${BIN_INC} -I${SRC_INC} ${MGL_DEF}'
		DEPENDS ${dep_libs} ${src_dep}
		IMPLICIT_DEPENDS CXX ${src_imp_dep}
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/lang
	)
	add_custom_target(mgl_python_module ALL DEPENDS _mathgl.so mathgl.py)
	install(CODE "execute_process(COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_BINARY_DIR}/lang/setup.py install --prefix=${CMAKE_INSTALL_PREFIX} WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/lang )")
	set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "mgl_wrap.cpp;build;mathgl.pyc")
endif(MGL_HAVE_PYTHON)

if(MGL_HAVE_OCTAVE)
	execute_process(COMMAND ${oct_prog} -p CANONICAL_HOST_TYPE
	OUTPUT_VARIABLE oct_host
	OUTPUT_STRIP_TRAILING_WHITESPACE)
	execute_process(COMMAND ${oct_prog} -p API_VERSION
	OUTPUT_VARIABLE oct_api
	OUTPUT_STRIP_TRAILING_WHITESPACE)
	execute_process(COMMAND ${oct_prog} -p OCTINCLUDEDIR
	OUTPUT_VARIABLE oct_inc
	OUTPUT_STRIP_TRAILING_WHITESPACE)
	set(oct_arch ${oct_host}-${oct_api})
	set(pkg_name mathgl)
	file(MAKE_DIRECTORY ${pkg_name}/inst/${oct_arch})

	configure_file(${MathGL_SOURCE_DIR}/COPYING ${MathGL_BINARY_DIR}/lang/${pkg_name} COPYONLY)
	configure_file(${MathGL_SOURCE_DIR}/lang/DESCRIPTION ${MathGL_BINARY_DIR}/lang/${pkg_name} COPYONLY)
	configure_file(${MathGL_SOURCE_DIR}/lang/INDEX ${MathGL_BINARY_DIR}/lang/${pkg_name} COPYONLY)
	configure_file(${MathGL_SOURCE_DIR}/lang/PKG_ADD_template ${MathGL_BINARY_DIR}/lang/${pkg_name}/PKG_ADD COPYONLY)

	add_custom_command(OUTPUT mgl_octave.cpp
		COMMAND ${SWIG_EXECUTABLE} -octave -c++ ${MGL_DEF}
		-I${BIN_INC} -I${SRC_INC} 
		-o mgl_octave.cpp ${MGL_I}
		DEPENDS ${src_dep}
		IMPLICIT_DEPENDS CXX ${src_imp_dep}
	)
	add_custom_command(OUTPUT ${pkg_name}/inst/${oct_arch}/mathgl.oct
		COMMAND ${oct_mk} -pthread ${MGL_DEF}
		-I${oct_inc} -I${BIN_INC} -I${SRC_INC} 
		-L$<TARGET_LINKER_FILE_DIR:mgl>
		-L$<TARGET_LINKER_FILE_DIR:mgl-wnd>
		-lmgl-wnd -lmgl
		-o ${pkg_name}/inst/${oct_arch}/mathgl.oct mgl_octave.cpp
		DEPENDS ${dep_libs} ${src_dep} mgl_octave.cpp
		IMPLICIT_DEPENDS CXX ${src_imp_dep}
	)
	add_custom_command(OUTPUT mathgl.tar.gz
		COMMAND ${oct_tar} cpzvf mathgl.tar.gz ${pkg_name}
		DEPENDS ${pkg_name}/inst/${oct_arch}/mathgl.oct
	)
	add_custom_target(mgl_octave_module ALL DEPENDS mathgl.tar.gz)
endif(MGL_HAVE_OCTAVE)
