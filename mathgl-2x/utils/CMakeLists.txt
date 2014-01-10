
add_executable(mglconv mglconv.cpp)
target_link_libraries(mglconv mgl)
install(
	TARGETS mglconv
	RUNTIME DESTINATION bin
)

add_executable(mgl.cgi mglcgi.cpp)
target_link_libraries(mgl.cgi mgl)
install(
	TARGETS mgl.cgi
# should be /usr/lib/cgi-bin/
	RUNTIME DESTINATION ${MGL_CGI_PATH}
)

if(MGL_HAVE_QT4 OR MGL_HAVE_QT5)
	add_executable(mglview mglview.cpp)
	if(MGL_HAVE_QT4)
		include(${QT_USE_FILE})
		target_link_libraries(mglview mgl-qt)
	else(MGL_HAVE_QT4)
		target_link_libraries(mglview mgl-qt5)
	endif(MGL_HAVE_QT4)
	install(
		TARGETS mglview
		RUNTIME DESTINATION bin
	)
endif(MGL_HAVE_QT4 OR MGL_HAVE_QT5)
