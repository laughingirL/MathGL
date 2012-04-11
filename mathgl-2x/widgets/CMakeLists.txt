set(mgl_wnd_hdr ../include/mgl2/window.h ../include/mgl2/canvas_wnd.h)
set(mgl_wnd_src window.cpp)
#set(mgl_wnd_src )

if(MGL_HAVE_GSL)
#	target_link_libraries(mgl ${GSL_LIB} ${GSL_CBLAS_LIB} )
	include_directories(${GSL_INCLUDE_DIR})
endif(MGL_HAVE_GSL)

if(MGL_HAVE_FLTK)
	include_directories(${FLTK_INCLUDE_DIR})
	set(mgl_wnd_src ${mgl_wnd_src} fltk.cpp)
	set(mgl_wnd_hdr ${mgl_wnd_hdr} ../include/mgl2/fltk.h)
endif(MGL_HAVE_FLTK)

if(MGL_HAVE_GLUT)
	include_directories(${GLUT_INCLUDE_DIR})
	set(mgl_glut_src glut.cpp window.cpp)
	set(mgl_glut_hdr ../include/mgl2/glut.h ../include/mgl2/window.h)

	add_library(mgl-glut SHARED ${mgl_glut_src} ${mgl_glut_hdr})
	add_library(mgl-glut-static STATIC ${mgl_glut_src} ${mgl_glut_hdr})
	set_target_properties(mgl-glut-static PROPERTIES OUTPUT_NAME "mgl-glut")
	set_target_properties(mgl-glut PROPERTIES CLEAN_DIRECT_OUTPUT 1)
	set_target_properties(mgl-glut-static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
	set_target_properties(mgl-glut PROPERTIES SOVERSION 6.0.0)
	target_link_libraries(mgl-glut mgl)
	target_link_libraries(mgl-glut ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})

	if(MGL_HAVE_PTHREAD)
		target_link_libraries(mgl-glut ${CMAKE_THREAD_LIBS_INIT})
	endif(MGL_HAVE_PTHREAD)

	install(
		TARGETS mgl-glut mgl-glut-static
		RUNTIME DESTINATION bin
		ARCHIVE DESTINATION lib
		LIBRARY DESTINATION lib
	)
endif(MGL_HAVE_GLUT)

if(MGL_HAVE_WX)
	include(${wxWidgets_USE_FILE})
	set(mgl_wnd_src ${mgl_wnd_src} wx.cpp)
	set(mgl_wnd_hdr ${mgl_wnd_hdr} ../include/mgl2/fltk.h)
endif(MGL_HAVE_WX)

if(MGL_HAVE_QT)
	include(${QT_USE_FILE})
#	qt_wrap_cpp(mgl-wnd MGL_MOC_FILES ../include/mgl2/qt.h)
	qt4_wrap_cpp(MGL_MOC_FILES ../include/mgl2/qt.h)
	set(mgl_wnd_src ${mgl_wnd_src} qt.cpp ${MGL_MOC_FILES})
	set(mgl_wnd_hdr ${mgl_wnd_hdr} ../include/mgl2/qt.h)
endif(MGL_HAVE_QT)

add_library(mgl-wnd SHARED ${mgl_wnd_src} ${mgl_wnd_hdr})
add_library(mgl-wnd-static STATIC ${mgl_wnd_src} ${mgl_wnd_hdr})
set_target_properties(mgl-wnd-static PROPERTIES OUTPUT_NAME "mgl-wnd")
set_target_properties(mgl-wnd PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(mgl-wnd-static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(mgl-wnd PROPERTIES SOVERSION 6.0.0)
target_link_libraries(mgl-wnd mgl)


if(MGL_HAVE_PTHREAD)
	target_link_libraries(mgl-wnd ${CMAKE_THREAD_LIBS_INIT})
endif(MGL_HAVE_PTHREAD)

if(MGL_HAVE_FLTK)
	target_link_libraries(mgl-wnd ${FLTK_LIBRARIES})
endif(MGL_HAVE_FLTK)

if(MGL_HAVE_WX)
	target_link_libraries(mgl-wnd ${wxWidgets_LIBRARIES})
endif(MGL_HAVE_WX)

if(MGL_HAVE_QT)
	target_link_libraries(mgl-wnd ${QT_LIBRARIES})
endif(MGL_HAVE_QT)

install(
	TARGETS mgl-wnd mgl-wnd-static
	RUNTIME DESTINATION bin
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
)
