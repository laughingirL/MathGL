
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
	RUNTIME DESTINATION bin
)

if(MGL_HAVE_QT)
	include(${QT_USE_FILE})
	add_executable(mglview mglview.cpp)
	target_link_libraries(mglview mgl-qt)
	set(MGL_HAVE_MGLVIEW 1)
else(MGL_HAVE_QT)
	if(MGL_HAVE_FLTK)
		add_executable(mglview mglview.cpp)
		target_link_libraries(mglview mgl-fltk)
		set(MGL_HAVE_MGLVIEW 1)
	else(MGL_HAVE_FLTK)
		if(MGL_HAVE_WX)
			add_executable(mglview mglview.cpp)
			target_link_libraries(mglview mgl-wx)
			set(MGL_HAVE_MGLVIEW 1)
		else(MGL_HAVE_WX)
			set(MGL_HAVE_MGLVIEW 0)
		endif(MGL_HAVE_WX)
	endif(MGL_HAVE_FLTK)
endif(MGL_HAVE_QT)

if(MGL_HAVE_MGLVIEW)
install(
	TARGETS mglview
	RUNTIME DESTINATION bin
)
endif(MGL_HAVE_MGLVIEW)
