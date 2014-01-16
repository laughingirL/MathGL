if(enable-qt4)
	set(MGL_HAVE_QT4 1)
	FIND_PACKAGE(Qt4 4.8 REQUIRED QtCore QtGui QtNetwork QtWebKit)
	if(NOT QT4_FOUND)
		message(SEND_ERROR "Couldn't find Qt4 library.")
	endif(NOT QT4_FOUND)
	include(${QT_USE_FILE})
	qt4_wrap_cpp(MGL_MOC_FILES ../include/mgl2/qmathgl.h)
	add_library(mgl-qt SHARED ../widgets/qt.cpp ${MGL_MOC_FILES} ../include/mgl2/qt.h ../include/mgl2/qmathgl.h)
	add_library(mgl-qt-static STATIC ../widgets/qt.cpp ${MGL_MOC_FILES} ../include/mgl2/qt.h ../include/mgl2/qmathgl.h)
	set_target_properties(mgl-qt PROPERTIES SOVERSION ${MathGL_SOVERSION})
	set_target_properties(mgl-qt PROPERTIES CLEAN_DIRECT_OUTPUT 1)
	set_target_properties(mgl-qt PROPERTIES DEFINE_SYMBOL "mgl_EXPORTS")
	set_target_properties(mgl-qt-static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
	set_target_properties(mgl-qt-static PROPERTIES COMPILE_FLAGS -DMGL_STATIC_DEFINE)
	target_link_libraries(mgl-qt mgl)
	target_link_libraries(mgl-qt ${QT_LIBRARIES})

	if(enable-mgl2)
		set_target_properties(mgl-qt PROPERTIES OUTPUT_NAME "mgl2-qt")
		set_target_properties(mgl-qt-static PROPERTIES OUTPUT_NAME "mgl2-qt")
	else(enable-mgl2)
		set_target_properties(mgl-qt-static PROPERTIES OUTPUT_NAME "mgl-qt")
	endif(enable-mgl2)

	install(
		TARGETS mgl-qt mgl-qt-static
		RUNTIME DESTINATION bin
		ARCHIVE DESTINATION ${MGL_LIB_INSTALL_DIR}
		LIBRARY DESTINATION ${MGL_LIB_INSTALL_DIR}
	)
	add_subdirectory( wnd )
else(enable-qt4)
	set(MGL_HAVE_QT4 0)
endif(enable-qt4)
