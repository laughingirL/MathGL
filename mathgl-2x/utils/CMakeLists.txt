if(use_gsl)
#	target_link_libraries(mgl ${GSL_LIB} ${GSL_CBLAS_LIB} )
	add_definitions(-DHAVE_GSL)
	include_directories(${GSL_INCLUDE_DIR})
else(use_gsl)
	add_definitions(-DNO_GSL)
endif(use_gsl)

if(use_fltk)
	add_definitions(-DHAVE_FLTK)
	include_directories(${FLTK_INCLUDE_DIR})
	set(mgl_wnd_src ${mgl_wnd_src} fltk.cpp)
endif(use_fltk)
if(use_wx)
	include(${wxWidgets_USE_FILE})
	add_definitions(-DHAVE_WX)
	set(mgl_wnd_src ${mgl_wnd_src} wx.cpp)
endif(use_wx)
if(use_qt)
	include(${QT_USE_FILE})
	add_definitions(-DHAVE_QT)
	qt_wrap_cpp(mgl-qt MGL_MOC_FILES ../include/mgl/mgl_qt.h)
	set(mgl_wnd_src ${mgl_wnd_src} qt.cpp ${MGL_MOC_FILES})
endif(use_qt)



add_executable(mglconv mglconv.cpp)
target_link_libraries(mglconv mgl)
install(
	TARGETS mglconv
	RUNTIME DESTINATION bin
)

if(use_fltk OR use_qt)
add_executable(mglview mglview.cpp)
target_link_libraries(mglview mgl-wnd)
install(
	TARGETS mglview
	RUNTIME DESTINATION bin
)
endif(use_fltk OR use_qt)

