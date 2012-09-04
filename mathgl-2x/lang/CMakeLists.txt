set(src_dep
${CMAKE_BINARY_DIR}/lang/mgl.i
${CMAKE_BINARY_DIR}/lang/numpy.i
)
set(src_imp_dep
${CMAKE_SOURCE_DIR}/include/mgl2/type.h
${CMAKE_SOURCE_DIR}/include/mgl2/data.h
${CMAKE_SOURCE_DIR}/include/mgl2/mgl.h
${CMAKE_SOURCE_DIR}/include/mgl2/window.h
)
configure_file(${MathGL_SOURCE_DIR}/lang/mgl.i.in ${MathGL_BINARY_DIR}/lang/mgl.i COPYONLY)
configure_file(${MathGL_SOURCE_DIR}/lang/numpy.i.in ${MathGL_BINARY_DIR}/lang/numpy.i COPYONLY)
file(TO_NATIVE_PATH ${MathGL_BINARY_DIR}/lang/mgl.i MGL_I)

file(TO_NATIVE_PATH ${MathGL_SOURCE_DIR}/include SRC_INC)
file(TO_NATIVE_PATH ${MathGL_BINARY_DIR}/include BIN_INC)

file(TO_NATIVE_PATH ${MathGL_BINARY_DIR}/src MGL_LINK_DIR)
file(TO_NATIVE_PATH ${MathGL_BINARY_DIR}/widgets MGL_WND_LINK_DIR)

if(WIN32)
	set(MGL_ALL_INC ${BIN_INC};${SRC_INC})
	set(MGL_ALL_LINK ${MGL_LINK_DIR};${MGL_WND_LINK_DIR})
else(WIN32)
	set(MGL_ALL_INC ${BIN_INC}:${SRC_INC})
	set(MGL_ALL_LINK ${MGL_LINK_DIR}:${MGL_WND_LINK_DIR})
endif(WIN32)

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
		build build_ext 
		--include-dirs='${MGL_ALL_INC}'
#		--library-dirs='$<TARGET_LINKER_FILE_DIR:mgl>:$<TARGET_LINKER_FILE_DIR:mgl-wnd>' #commented out for compatibilty reason with old cmake
		--library-dirs='${MGL_ALL_LINK}'
#		--libraries='mgl-wnd'
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
message(STATUS "${oct_prog} ${oct_host} ${oct_api}")
	set(oct_arch ${oct_host}-${oct_api})
	set(pkg_name mathgl)
	file(MAKE_DIRECTORY ${MathGL_BINARY_DIR}/lang/${pkg_name}/inst/${oct_arch})

	configure_file(${MathGL_SOURCE_DIR}/lang/install.m.in ${MathGL_BINARY_DIR}/lang/install.m)
	configure_file(${MathGL_SOURCE_DIR}/lang/install-path.m.in ${MathGL_BINARY_DIR}/lang/install-path.m)
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
#		-L$<TARGET_LINKER_FILE_DIR:mgl>                      #commented out for compatibilty reason with old cmake
#		-L$<TARGET_LINKER_FILE_DIR:mgl-wnd>                  #commented out for compatibilty reason with old cmake
	add_custom_command(OUTPUT mgl_octave.o
		COMMAND ${oct_mk} -c ${MGL_DEF}
		-I${oct_inc} -I${BIN_INC} -I${SRC_INC}
		-o mgl_octave.o mgl_octave.cpp
		DEPENDS ${src_dep} mgl_octave.cpp
		IMPLICIT_DEPENDS CXX ${src_imp_dep}
	)
	add_custom_command(OUTPUT ${pkg_name}/inst/${oct_arch}/mathgl.oct
		COMMAND ${oct_mk}
		-L${MGL_LINK_DIR}
		-L${MGL_WND_LINK_DIR}
		-lmgl-wnd -lmgl
		-o ${pkg_name}/inst/${oct_arch}/mathgl.oct mgl_octave.o
		DEPENDS ${dep_libs} mgl_octave.o
	)
#	add_custom_command(OUTPUT ${pkg_name}/inst/${oct_arch}/mathgl.oct
#		COMMAND ${oct_mk} ${MGL_DEF}
#		-I${oct_inc} -I${BIN_INC} -I${SRC_INC}
#		-L${MGL_LINK_DIR}
#		-L${MGL_WND_LINK_DIR}
#		-lmgl-wnd -lmgl
#		-o ${pkg_name}/inst/${oct_arch}/mathgl.oct mgl_octave.cpp
#		DEPENDS ${dep_libs} ${src_dep} mgl_octave.cpp
#	)
	add_custom_command(OUTPUT mathgl.tar.gz
		COMMAND ${oct_tar} cpzvf mathgl.tar.gz ${pkg_name}
		DEPENDS ${pkg_name}/inst/${oct_arch}/mathgl.oct
	)
	add_custom_target(mgl_octave_module ALL DEPENDS mathgl.tar.gz)
	if(enable-octave-global OR enable-octave-local)
		install(CODE "execute_process(COMMAND ${oct_exec} ${CMAKE_BINARY_DIR}/lang/install.m WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/lang )")
	endif(enable-octave-global OR enable-octave-local)
	if(enable-octave-path)
		install(CODE "execute_process(COMMAND ${oct_exec} ${CMAKE_BINARY_DIR}/lang/install-path.m WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/lang )")
	endif(enable-octave-path)
endif(MGL_HAVE_OCTAVE)
