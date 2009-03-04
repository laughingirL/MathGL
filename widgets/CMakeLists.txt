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
	add_library(mgl-fltk SHARED mgl_fltk.cpp)
	target_link_libraries(mgl-fltk ${FLTK_LIBRARIES} mgl)
	set_target_properties(mgl-fltk PROPERTIES SOVERSION 5.0.0)
	install(
		TARGETS mgl-fltk
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
	)
endif(use_fltk)

if(use_glut)
	add_definitions(-DHAVE_GLUT)
	include_directories(${GLUT_INCLUDE_DIR})
	add_library(mgl-glut SHARED mgl_glut.cpp)
	target_link_libraries(mgl-glut ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} mgl)
	set_target_properties(mgl-glut PROPERTIES SOVERSION 5.0.0)
	install(
		TARGETS mgl-glut
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
	)
endif(use_glut)

if(use_wx)
	include(${wxWidgets_USE_FILE})
	add_definitions(-DHAVE_WX)
	add_library(mgl-wx SHARED mgl_wx.cpp)
	target_link_libraries(mgl-wx ${wxWidgets_LIBRARIES} mgl)
	set_target_properties(mgl-wx PROPERTIES SOVERSION 5.0.0)
	install(
		TARGETS mgl-wx
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
	)
endif(use_wx)

if(use_qt)
	include(${QT_USE_FILE})
	add_definitions(-DHAVE_QT)
	qt_wrap_cpp(mgl-qt MGL_MOC_FILES ../include/mgl/mgl_qt.h)
	add_library(mgl-qt SHARED mgl_qt.cpp ${MGL_MOC_FILES})
	target_link_libraries(mgl-qt ${QT_LIBRARIES} mgl)
	set_target_properties(mgl-qt PROPERTIES SOVERSION 5.0.0)
	install(
		TARGETS mgl-qt
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
	)
endif(use_qt)

