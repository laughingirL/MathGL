
add_executable(make_pas make_pas.cpp)

add_executable(make_bin make_bin.cpp)
target_link_libraries(make_bin mgl)

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

if(enable-qt)
	add_executable(mglview mglview.cpp)
	if(enable-qt5)
		target_link_libraries(mglview mgl-qt5 ${MGL_QT5_LIBS})
	else(enable-qt5)
		target_link_libraries(mglview mgl-qt4 ${MGL_QT4_LIBS})
	endif(enable-qt5)

	install(
		TARGETS mglview
		RUNTIME DESTINATION bin
	)
endif(enable-qt)
