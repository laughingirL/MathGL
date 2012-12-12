set(src_imp_dep
${CMAKE_SOURCE_DIR}/include/mgl2/type.h
${CMAKE_SOURCE_DIR}/include/mgl2/data.h
${CMAKE_SOURCE_DIR}/include/mgl2/mgl.h
${CMAKE_SOURCE_DIR}/include/mgl2/window.h
)

SET_SOURCE_FILES_PROPERTIES(mgl.i numpy.i PROPERTIES CPLUSPLUS ON)

if(MGL_HAVE_PYTHON)
	include_directories(${PYTHON_INCLUDE_DIR})
	execute_process(
		COMMAND ${PYTHON_EXECUTABLE} -c "from distutils.sysconfig import get_python_lib; print get_python_lib(prefix='${CMAKE_INSTALL_PREFIX}')"
		OUTPUT_VARIABLE MGL_PYTHON_SITE_PACKAGES
		OUTPUT_STRIP_TRAILING_WHITESPACE)
	SWIG_ADD_MODULE(mathgl python mgl.i)
	SWIG_LINK_LIBRARIES(mathgl mgl ${PYTHON_LIBRARIES})
	SET(SWIG_MODULE_mathgl_EXTRA_DEPS numpy.i ${src_imp_dep})
	set_target_properties(${SWIG_MODULE_${mathgl}_REAL_NAME} PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
	add_custom_target(mgl_python_module ALL DEPENDS ${SWIG_MODULE_${mathgl}_REAL_NAME})

	install(FILES ${MathGL_BINARY_DIR}/lang/mathgl.py ${MathGL_BINARY_DIR}/lang/mathgl.pyc DESTINATION ${MGL_PYTHON_SITE_PACKAGES})
	install (TARGETS ${SWIG_MODULE_${mathgl}_REAL_NAME} LIBRARY DESTINATION ${MGL_PYTHON_SITE_PACKAGES})
	set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "mathgl.py mathgl.pyc")
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
	set(mgl_pkg_dir ${MathGL_BINARY_DIR}/lang/${pkg_name}/inst/${oct_arch})

	include_directories(${oct_inc} ${oct_inc}/..)
	SWIG_ADD_MODULE(mgl-oct octave mgl.i)
	SWIG_LINK_LIBRARIES(mgl-oct mgl)
	SET(SWIG_MODULE_mgl-oct_EXTRA_DEPS numpy.i ${src_imp_dep})
	set_target_properties(mgl-oct PROPERTIES OUTPUT_NAME mathgl PREFIX "" SUFFIX ".oct")

	add_custom_target(mgl_octave_module ALL DEPENDS mgl-oct)

	install(CODE "
		file(MAKE_DIRECTORY ${mgl_pkg_dir})
		file(COPY ${MathGL_SOURCE_DIR}/COPYING ${MathGL_SOURCE_DIR}/lang/DESCRIPTION ${MathGL_SOURCE_DIR}/lang/INDEX ${MathGL_SOURCE_DIR}/lang/PKG_ADD_template DESTINATION ${MathGL_BINARY_DIR}/lang/${pkg_name})")
	install(TARGETS mgl-oct LIBRARY DESTINATION ${mgl_pkg_dir})
	install(CODE "
		execute_process(COMMAND ${oct_tar} cpzvf mathgl.tar.gz ${pkg_name} WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/lang)")
	install(FILES ${CMAKE_BINARY_DIR}/lang/mathgl.tar.gz DESTINATION ${MGL_DATA_PATH})
	if(enable-octave-install)
		install(CODE "execute_process(
			COMMAND ${oct_exec} -q --eval \"pkg install mathgl.tar.gz\"
			WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/lang )")
	endif(enable-octave-install)
	set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "mathgl mathgl.tar.gz")
endif(MGL_HAVE_OCTAVE)
