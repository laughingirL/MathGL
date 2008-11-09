if(use_fltk)
	add_definitions(-DHAVE_FLTK)
	include_directories(${FLTK_INCLUDE_DIR})
	add_library(mgl-fltk SHARED mgl_fltk.cpp)
	target_link_libraries(mgl-fltk ${FLTK_LIBRARIES} mgl)
	install(
		TARGETS mgl-fltk
		LIBRARY DESTINATION lib
	)
endif(use_fltk)

if(use_glut)
	add_definitions(-DHAVE_GLUT)
	include_directories(${GLUT_INCLUDE_DIR})
	add_library(mgl-glut SHARED mgl_glut.cpp)
	target_link_libraries(mgl-glut ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} mgl)
	install(
		TARGETS mgl-glut
		LIBRARY DESTINATION lib
	)
endif(use_glut)

if(use_wx)
	include(${wxWidgets_USE_FILE})
	add_definitions(-DHAVE_WX)
	add_library(mgl-wx SHARED mgl_wx.cpp)
	target_link_libraries(mgl-wx ${wxWidgets_LIBRARIES} mgl)
	install(
		TARGETS mgl-wx
		LIBRARY DESTINATION lib
	)
endif(use_wx)

if(use_qt)
	include(${QT_USE_FILE})
	add_definitions(-DHAVE_QT)
	qt_wrap_cpp(mgl-qt mgl_qt.moc.cpp ../include/mgl/mgl_qt.h)
	add_library(mgl-qt SHARED mgl_qt.cpp)
	target_link_libraries(mgl-qt ${QT_LIBRARIES} mgl)
	install(
		TARGETS mgl-qt
		LIBRARY DESTINATION lib
	)
endif(use_qt)

#set_target_properties(mgl PROPERTIES SOVERSION 4.2.1)
