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
	add_library(mgl-fltk SHARED fltk.cpp)
	add_library(mgl-fltk-static STATIC fltk.cpp)
	set_target_properties(mgl-fltk-static PROPERTIES OUTPUT_NAME "mgl-fltk")
	set_target_properties(mgl-fltk PROPERTIES CLEAN_DIRECT_OUTPUT 1)
	set_target_properties(mgl-fltk-static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
	target_link_libraries(mgl-fltk ${FLTK_LIBRARIES} mgl)
	set_target_properties(mgl-fltk PROPERTIES SOVERSION 5.0.0)
	install(
		TARGETS mgl-fltk mgl-fltk-static
		RUNTIME DESTINATION bin
		ARCHIVE DESTINATION lib
		LIBRARY DESTINATION lib
	)
endif(use_fltk)

if(use_glut)
	add_definitions(-DHAVE_GLUT)
	include_directories(${GLUT_INCLUDE_DIR})
	add_library(mgl-glut SHARED glut.cpp)
	add_library(mgl-glut-static STATIC glut.cpp)
	set_target_properties(mgl-glut-static PROPERTIES OUTPUT_NAME "mgl-glut")
	set_target_properties(mgl-glut PROPERTIES CLEAN_DIRECT_OUTPUT 1)
	set_target_properties(mgl-glut-static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
	target_link_libraries(mgl-glut ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} mgl)
	set_target_properties(mgl-glut PROPERTIES SOVERSION 5.0.0)
	install(
		TARGETS mgl-glut mgl-glut-static
		RUNTIME DESTINATION bin
		ARCHIVE DESTINATION lib
		LIBRARY DESTINATION lib
	)
endif(use_glut)

if(use_wx)
	include(${wxWidgets_USE_FILE})
	add_definitions(-DHAVE_WX)
	add_library(mgl-wx SHARED wx.cpp)
	add_library(mgl-wx-static STATIC wx.cpp)
	set_target_properties(mgl-wx-static PROPERTIES OUTPUT_NAME "mgl-wx")
	set_target_properties(mgl-wx PROPERTIES CLEAN_DIRECT_OUTPUT 1)
	set_target_properties(mgl-wx-static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
	target_link_libraries(mgl-wx ${wxWidgets_LIBRARIES} mgl)
	set_target_properties(mgl-wx PROPERTIES SOVERSION 5.0.0)
	install(
		TARGETS mgl-wx mgl-wx-static
		RUNTIME DESTINATION bin
		ARCHIVE DESTINATION lib
		LIBRARY DESTINATION lib
	)
endif(use_wx)

if(use_qt)
	include(${QT_USE_FILE})
	add_definitions(-DHAVE_QT)
	qt_wrap_cpp(mgl-qt MGL_MOC_FILES ../include/mgl/mgl_qt.h)
	add_library(mgl-qt SHARED qt.cpp ${MGL_MOC_FILES})
	add_library(mgl-qt-static STATIC qt.cpp ${MGL_MOC_FILES})
	set_target_properties(mgl-qt-static PROPERTIES OUTPUT_NAME "mgl-qt")
	set_target_properties(mgl-qt PROPERTIES CLEAN_DIRECT_OUTPUT 1)
	set_target_properties(mgl-qt-static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
	target_link_libraries(mgl-qt ${QT_LIBRARIES} mgl)
	set_target_properties(mgl-qt PROPERTIES SOVERSION 5.0.0)
	install(
		TARGETS mgl-qt mgl-qt-static
		RUNTIME DESTINATION bin
		ARCHIVE DESTINATION lib
		LIBRARY DESTINATION lib
	)
endif(use_qt)

