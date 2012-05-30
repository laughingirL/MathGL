if(MGL_HAVE_GSL)
	include_directories(${GSL_INCLUDE_DIR})
endif(MGL_HAVE_GSL)

add_executable(mgl_example wnd_samples.cpp full_test.cpp)
target_link_libraries(mgl_example mgl)
# install(
# 	TARGETS mgl_example
# 	RUNTIME DESTINATION bin
# )

if(MGL_HAVE_FLTK)
	add_executable(mgl_fltk_example wnd_samples.cpp fltk_example.cpp)
	target_link_libraries(mgl_fltk_example mgl-wnd)
# 	install(
# 		TARGETS mgl_fltk_example
# 		RUNTIME DESTINATION bin
# 	)
endif(MGL_HAVE_FLTK)

if(MGL_HAVE_GLUT)
	add_executable(mgl_glut_example wnd_samples.cpp glut_example.cpp)
	target_link_libraries(mgl_glut_example mgl-glut)
# 	install(
# 		TARGETS mgl_glut_example
# 		RUNTIME DESTINATION bin
# 	)
endif(MGL_HAVE_GLUT)

if(MGL_HAVE_WX)
	include(${wxWidgets_USE_FILE})
	add_executable(mgl_wx_example wnd_samples.cpp wx_example.cpp)
	target_link_libraries(mgl_wx_example ${wxWidgets_LIBRARIES} mgl-wnd)
# 	install(
# 		TARGETS mgl_wx_example
# 		RUNTIME DESTINATION bin
# 	)
endif(MGL_HAVE_WX)

if(MGL_HAVE_QT)
	include(${QT_USE_FILE})
	qt_wrap_cpp(mgl_qt_example MGL_MOC_EX_FILES ../include/mgl2/qt.h)
	add_executable(mgl_qt_example wnd_samples.cpp qt_example.cpp ${MGL_MOC_EX_FILES})
	target_link_libraries(mgl_qt_example mgl-wnd ${QT_LIBRARIES})
# 	install(
# 		TARGETS mgl_qt_example
# 		RUNTIME DESTINATION bin
# 	)
endif(MGL_HAVE_QT)
