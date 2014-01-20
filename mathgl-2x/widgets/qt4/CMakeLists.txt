include(GenerateExportHeader)
add_compiler_export_flags()

if(enable-qt4)
	include(../../cmake-qt4.txt)
	qt4_wrap_cpp(MGL_MOC_FILES ${MathGL_SOURCE_DIR}/include/mgl2/qmathgl.h)
	set(MGL_QT_FILES ../qt.cpp ${MGL_MOC_FILES} ${MathGL_SOURCE_DIR}/include/mgl2/qt.h ${MathGL_SOURCE_DIR}/include/mgl2/qmathgl.h)
	add_library(mgl-qt4 SHARED ${MGL_QT_FILES})
	add_library(mgl-qt4-static STATIC ${MGL_QT_FILES})
	set_target_properties(mgl-qt4 PROPERTIES SOVERSION ${MathGL_SOVERSION})
	set_target_properties(mgl-qt4 PROPERTIES CLEAN_DIRECT_OUTPUT 1)
	set_target_properties(mgl-qt4 PROPERTIES DEFINE_SYMBOL "mgl_EXPORTS")
	set_target_properties(mgl-qt4-static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
	set_target_properties(mgl-qt4-static PROPERTIES COMPILE_FLAGS -DMGL_STATIC_DEFINE)
	target_link_libraries(mgl-qt4 mgl)
	target_link_libraries(mgl-qt4 ${QT_LIBRARIES})

	if(enable-mgl2)
		set_target_properties(mgl-qt4 PROPERTIES OUTPUT_NAME "mgl2-qt4")
		set_target_properties(mgl-qt4-static PROPERTIES OUTPUT_NAME "mgl2-qt4")
	else(enable-mgl2)
		set_target_properties(mgl-qt4-static PROPERTIES OUTPUT_NAME "mgl-qt4")
	endif(enable-mgl2)

	install(
		TARGETS mgl-qt4 mgl-qt4-static
		RUNTIME DESTINATION bin
		ARCHIVE DESTINATION ${MGL_LIB_INSTALL_DIR}
		LIBRARY DESTINATION ${MGL_LIB_INSTALL_DIR}
	)

	if(NOT enable-qt5asqt)
	add_library(mgl-qt SHARED ${MGL_QT_FILES})
	add_library(mgl-qt-static STATIC ${MGL_QT_FILES})
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
	endif(NOT enable-qt5asqt)

	if(MGL_HAVE_FLTK AND NOT enable-qt5)
		include_directories(${FLTK_INCLUDE_DIR})

		set(MGL_WND_SRC ${MGL_QT_FILES} ../fltk.cpp ${MathGL_SOURCE_DIR}/include/mgl2/fltk.h)

		add_library(mgl-wnd SHARED ${MGL_WND_SRC})
		add_library(mgl-wnd-static STATIC ${MGL_WND_SRC})

		set_target_properties(mgl-wnd PROPERTIES SOVERSION ${MathGL_SOVERSION})
		set_target_properties(mgl-wnd PROPERTIES CLEAN_DIRECT_OUTPUT 1)
		set_target_properties(mgl-wnd PROPERTIES DEFINE_SYMBOL "mgl_EXPORTS")
		set_target_properties(mgl-wnd-static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
		set_target_properties(mgl-wnd-static PROPERTIES COMPILE_FLAGS -DMGL_STATIC_DEFINE)

		if(enable-mgl2)
			set_target_properties(mgl-wnd PROPERTIES OUTPUT_NAME "mgl2-wnd")
			set_target_properties(mgl-wnd-static PROPERTIES OUTPUT_NAME "mgl2-wnd")
		else(enable-mgl2)
			set_target_properties(mgl-wnd-static PROPERTIES OUTPUT_NAME "mgl-wnd")
		endif(enable-mgl2)

		target_link_libraries(mgl-wnd mgl)
		target_link_libraries(mgl-wnd ${QT_LIBRARIES})
		target_link_libraries(mgl-wnd ${FLTK_LIBRARIES})

		install(
			TARGETS mgl-wnd mgl-wnd-static
			RUNTIME DESTINATION bin
			ARCHIVE DESTINATION ${MGL_LIB_INSTALL_DIR}
			LIBRARY DESTINATION ${MGL_LIB_INSTALL_DIR}
		)
	endif(MGL_HAVE_FLTK AND  NOT enable-qt5)
else(enable-qt4)
	set(MGL_HAVE_QT4 0)
endif(enable-qt4)
