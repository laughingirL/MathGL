if(MGL_HAVE_GSL)
#	target_link_libraries(mgl ${GSL_LIB} ${GSL_CBLAS_LIB} )
	include_directories(${GSL_INCLUDE_DIR})
endif(MGL_HAVE_GSL)

if(MGL_HAVE_FLTK)
	include_directories(${FLTK_INCLUDE_DIR})
	set(mgl_wnd_src ${mgl_wnd_src} fltk.cpp)
endif(MGL_HAVE_FLTK)
if(MGL_HAVE_WX)
	include(${wxWidgets_USE_FILE})
	set(mgl_wnd_src ${mgl_wnd_src} wx.cpp)
endif(MGL_HAVE_WX)
if(MGL_HAVE_QT)
	include(${QT_USE_FILE})
	qt_wrap_cpp(mgl-qt MGL_MOC_FILES ../include/mgl2/qt.h)
	set(mgl_wnd_src ${mgl_wnd_src} qt.cpp ${MGL_MOC_FILES})
endif(MGL_HAVE_QT)



add_executable(mglconv mglconv.cpp)
target_link_libraries(mglconv mgl)
install(
	TARGETS mglconv
	RUNTIME DESTINATION bin
)

add_executable(mglcgi mglcgi.cpp)
target_link_libraries(mglcgi mgl)
install(
	TARGETS mglcgi
	RUNTIME DESTINATION bin
)

if(MGL_HAVE_FLTK OR MGL_HAVE_QT)
add_executable(mglview mglview.cpp)
target_link_libraries(mglview mgl-wnd)
install(
	TARGETS mglview
	RUNTIME DESTINATION bin
)
endif(MGL_HAVE_FLTK OR MGL_HAVE_QT)
