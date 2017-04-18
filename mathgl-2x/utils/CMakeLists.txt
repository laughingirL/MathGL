add_executable(make_pas make_pas.cpp)

add_executable(mglconv mglconv.cpp)
if(MSVC)
set(link_type -static)
else(MSVC)
set(link_type)
endif(MSVC)
target_link_libraries(mglconv mgl${link_type} ${getopt_lib-static})
install(
	TARGETS mglconv
	EXPORT MathGLTargets
	RUNTIME DESTINATION ${MathGL_INSTALL_BIN_DIR}
)

add_executable(mgl.cgi mglcgi.cpp)
target_link_libraries(mgl.cgi mgl${link_type})
install(
	TARGETS mgl.cgi
	EXPORT MathGLTargets
# should be /usr/lib/cgi-bin/
	RUNTIME DESTINATION ${MGL_CGI_PATH}
)

set(po_files ${po_files} mglconv.cpp mglview.cpp PARENT_SCOPE)

if(QT_ENABLED)
	add_executable(mglview mglview.cpp)
	if(enable-qt5)
		include(../cmake-qt5.txt)
		target_link_libraries(mglview mgl-qt5${link_type} ${getopt_lib-static} ${MGL_QT5_LIBS})
	else(enable-qt5)
		include(../cmake-qt4.txt)
		target_link_libraries(mglview mgl-qt4${link_type} ${getopt_lib-static} ${MGL_QT4_LIBS})
	endif(enable-qt5)

	install(
		TARGETS mglview
		EXPORT MathGLTargets
		RUNTIME DESTINATION ${MathGL_INSTALL_BIN_DIR}
	)
endif(QT_ENABLED)
