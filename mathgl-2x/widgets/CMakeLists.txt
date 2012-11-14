
if(MGL_HAVE_FLTK)
	include_directories(${FLTK_INCLUDE_DIR})
	add_library(mgl-fltk SHARED fltk.cpp ../include/mgl2/fltk.h)
	add_library(mgl-fltk-static STATIC fltk.cpp ../include/mgl2/fltk.h)
	set_target_properties(mgl-fltk-static PROPERTIES OUTPUT_NAME "mgl-fltk")
	set_target_properties(mgl-fltk PROPERTIES CLEAN_DIRECT_OUTPUT 1)
	set_target_properties(mgl-fltk-static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
	set_target_properties(mgl-fltk PROPERTIES SOVERSION 7.0.0)
	target_link_libraries(mgl-fltk mgl)
	target_link_libraries(mgl-fltk ${FLTK_LIBRARIES})

	install(
		TARGETS mgl-fltk mgl-fltk-static
		RUNTIME DESTINATION bin
		ARCHIVE DESTINATION lib
		LIBRARY DESTINATION lib
	)
endif(MGL_HAVE_FLTK)

if(MGL_HAVE_GLUT)
	include_directories(${GLUT_INCLUDE_DIR})
	add_library(mgl-glut SHARED glut.cpp ../include/mgl2/glut.h)
	add_library(mgl-glut-static STATIC glut.cpp ../include/mgl2/glut.h)
	set_target_properties(mgl-glut-static PROPERTIES OUTPUT_NAME "mgl-glut")
	set_target_properties(mgl-glut PROPERTIES CLEAN_DIRECT_OUTPUT 1)
	set_target_properties(mgl-glut-static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
	set_target_properties(mgl-glut PROPERTIES SOVERSION 7.0.0)
	target_link_libraries(mgl-glut mgl)
	target_link_libraries(mgl-glut ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})

	install(
		TARGETS mgl-glut mgl-glut-static
		RUNTIME DESTINATION bin
		ARCHIVE DESTINATION lib
		LIBRARY DESTINATION lib
	)
endif(MGL_HAVE_GLUT)

if(MGL_HAVE_WX)
	include(${wxWidgets_USE_FILE})
	add_library(mgl-wx SHARED wx.cpp ../include/mgl2/wx.h)
	add_library(mgl-wx-static STATIC wx.cpp ../include/mgl2/wx.h)
	set_target_properties(mgl-wx-static PROPERTIES OUTPUT_NAME "mgl-wx")
	set_target_properties(mgl-wx PROPERTIES CLEAN_DIRECT_OUTPUT 1)
	set_target_properties(mgl-wx-static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
	set_target_properties(mgl-wx PROPERTIES SOVERSION 7.0.0)
	target_link_libraries(mgl-wx mgl)
	target_link_libraries(mgl-wx ${wxWidgets_LIBRARIES})

	install(
		TARGETS mgl-wx mgl-wx-static
		RUNTIME DESTINATION bin
		ARCHIVE DESTINATION lib
		LIBRARY DESTINATION lib
	)
endif(MGL_HAVE_WX)

if(MGL_HAVE_QT)
	include(${QT_USE_FILE})
	qt4_wrap_cpp(MGL_MOC_FILES ../include/mgl2/qt.h)
	add_library(mgl-qt SHARED qt.cpp ${MGL_MOC_FILES} ../include/mgl2/qt.h)
	add_library(mgl-qt-static STATIC qt.cpp ${MGL_MOC_FILES} ../include/mgl2/qt.h)
	set_target_properties(mgl-qt-static PROPERTIES OUTPUT_NAME "mgl-qt")
	set_target_properties(mgl-qt PROPERTIES CLEAN_DIRECT_OUTPUT 1)
	set_target_properties(mgl-qt-static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
	set_target_properties(mgl-qt PROPERTIES SOVERSION 7.0.0)
	target_link_libraries(mgl-qt mgl)
	target_link_libraries(mgl-qt ${QT_LIBRARIES})

	install(
		TARGETS mgl-qt mgl-qt-static
		RUNTIME DESTINATION bin
		ARCHIVE DESTINATION lib
		LIBRARY DESTINATION lib
	)
endif(MGL_HAVE_QT)
