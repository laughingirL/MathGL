if(MGL_HAVE_GSL)
	include_directories(${GSL_INCLUDE_DIR})
endif(MGL_HAVE_GSL)

add_executable(mgl_example wnd_samples.cpp full_test.cpp samples.cpp)
target_link_libraries(mgl_example mgl)

if(MGL_HAVE_FLTK)
	add_executable(mgl_fltk_example wnd_samples.cpp fltk_example.cpp)
	target_include_directories(mgl_fltk_example SYSTEM PUBLIC ${FLTK_INCLUDE_DIR})
	target_link_libraries(mgl_fltk_example mgl-fltk)
endif(MGL_HAVE_FLTK)

if(MGL_HAVE_GLUT)
	add_executable(mgl_glut_example wnd_samples.cpp glut_example.cpp)
	target_link_libraries(mgl_glut_example mgl-glut)
endif(MGL_HAVE_GLUT)

if(MGL_HAVE_WX)
	include(${wxWidgets_USE_FILE})
	add_executable(mgl_wx_example wnd_samples.cpp wx_example.cpp)
	target_link_libraries(mgl_wx_example mgl-wx)
endif(MGL_HAVE_WX)

if(enable-qt)
	set(CMAKE_AUTOMOC ON)
	set(CMAKE_INCLUDE_CURRENT_DIR ON)
	add_executable(mgl_qt_example wnd_samples.cpp qt_example.cpp)
	if(enable-qt5)
		target_link_libraries(mgl_qt_example mgl-qt5 ${MGL_QT5_LIBS} Qt5::OpenGL)
	else(enable-qt5)
		target_link_libraries(mgl_qt_example mgl-qt4 ${MGL_QT4_LIBS} Qt4::QtOpenGL)
	endif(enable-qt5)
	add_executable(mgl_qgl_example wnd_samples.cpp qgl_example.cpp qgl_example.h)
	if(enable-qt5)
		target_link_libraries(mgl_qgl_example mgl ${MGL_QT5_LIBS} Qt5::OpenGL)
	else(enable-qt5)
		target_link_libraries(mgl_qgl_example mgl ${MGL_QT4_LIBS} Qt4::QtOpenGL)
	endif(enable-qt5)

endif(enable-qt)

if(MGL_HAVE_LTDL)
	include_directories(${LTDL_INCLUDE_DIR})
	add_library(mgl_module MODULE mgl_module.cpp)
	target_link_libraries(mgl_module mgl)	# for compatibility with win32
endif(MGL_HAVE_LTDL)

