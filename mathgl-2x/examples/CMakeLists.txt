if(use_gsl)
#	target_link_libraries(mgl ${GSL_LIB} ${GSL_CBLAS_LIB} )
	add_definitions(-DHAVE_GSL)
	include_directories(${GSL_INCLUDE_DIR})
else(use_gsl)
	add_definitions(-DNO_GSL)
endif(use_gsl)

add_executable(mgl_example wnd_samples.cpp full_test.cpp)
target_link_libraries(mgl_example mgl)
install(
	TARGETS mgl_example
	RUNTIME DESTINATION bin
)

if(use_fltk)
	add_executable(mgl_fltk_example wnd_samples.cpp fltk_example.cpp)
	target_link_libraries(mgl_fltk_example mgl-wnd)
	install(
		TARGETS mgl_fltk_example
		RUNTIME DESTINATION bin
	)
endif(use_fltk)

if(use_glut)
	add_executable(mgl_glut_example wnd_samples.cpp glut_example.cpp)
	target_link_libraries(mgl_glut_example mgl-glut)
	install(
		TARGETS mgl_glut_example
		RUNTIME DESTINATION bin
	)
endif(use_glut)

if(use_wx)
	include(${wxWidgets_USE_FILE})
	add_executable(mgl_wx_example wnd_samples.cpp wx_example.cpp)
	target_link_libraries(mgl_wx_example ${wxWidgets_LIBRARIES} mgl-wnd)
	install(
		TARGETS mgl_wx_example
		RUNTIME DESTINATION bin
	)
endif(use_wx)

if(use_qt)
	include(${QT_USE_FILE})
	qt_wrap_cpp(mgl_qt_example MGL_MOC_EX_FILES ../include/mgl/mgl_qt.h)
	add_executable(mgl_qt_example wnd_samples.cpp qt_example.cpp ${MGL_MOC_EX_FILES})
	target_link_libraries(mgl_qt_example ${QT_LIBRARIES} mgl-wnd)
	install(
		TARGETS mgl_qt_example
		RUNTIME DESTINATION bin
	)
endif(use_qt)
