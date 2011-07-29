set(mgl_wnd_src window.cpp)

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

if(use_glut)
	add_definitions(-DHAVE_GLUT)
	include_directories(${GLUT_INCLUDE_DIR})
	set(mgl_wnd_src ${mgl_wnd_src} glut.cpp)
endif(use_glut)

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



add_library(mgl-wnd SHARED ${mgl_wnd_src})
add_library(mgl-wnd-static STATIC ${mgl_wnd_src})
set_target_properties(mgl-wnd-static PROPERTIES OUTPUT_NAME "mgl-wnd")
set_target_properties(mgl-wnd PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(mgl-wnd-static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(mgl-wnd PROPERTIES SOVERSION 6.0.0)
target_link_libraries(mgl-wnd mgl)


if(use_fltk)
	target_link_libraries(mgl-wnd ${FLTK_LIBRARIES})
endif(use_fltk)

if(use_glut)
	target_link_libraries(mgl-wnd ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})
endif(use_glut)

if(use_wx)
	target_link_libraries(mgl-wnd ${wxWidgets_LIBRARIES})
endif(use_wx)

if(use_qt)
	target_link_libraries(mgl-wnd ${QT_LIBRARIES})
endif(use_qt)

install(
	TARGETS mgl-wnd mgl-wnd-static
	RUNTIME DESTINATION bin
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
)
