include(GenerateExportHeader)
add_compiler_export_flags()

if(enable-qt5)
	include(../../cmake-qt5.txt)
	qt5_wrap_cpp(MGL_MOC_FILES ${MathGL_SOURCE_DIR}/include/mgl2/qmathgl.h)
	set(MGL_QT_FILES ../qt.cpp ${MGL_MOC_FILES} ${MathGL_SOURCE_DIR}/include/mgl2/qt.h ${MathGL_SOURCE_DIR}/include/mgl2/qmathgl.h)
	add_library(mgl-qt5 SHARED ${MGL_QT_FILES})
	add_library(mgl-qt5-static STATIC ${MGL_QT_FILES})
	set_target_properties(mgl-qt5 PROPERTIES SOVERSION ${MathGL_SOVERSION})
	set_target_properties(mgl-qt5 PROPERTIES CLEAN_DIRECT_OUTPUT 1)
	set_target_properties(mgl-qt5 PROPERTIES DEFINE_SYMBOL "mgl_EXPORTS")
	set_target_properties(mgl-qt5-static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
	set_target_properties(mgl-qt5-static PROPERTIES COMPILE_FLAGS -DMGL_STATIC_DEFINE)
	target_link_libraries(mgl-qt5 mgl)
	qt5_use_modules(mgl-qt5 Core Widgets Gui Network WebKit PrintSupport)
	qt5_use_modules(mgl-qt5-static Core Widgets Gui Network WebKit PrintSupport)
	if(enable-mgl2)
		set_target_properties(mgl-qt5 PROPERTIES OUTPUT_NAME "mgl2-qt5")
		set_target_properties(mgl-qt5-static PROPERTIES OUTPUT_NAME "mgl2-qt5")
	else(enable-mgl2)
		set_target_properties(mgl-qt5-static PROPERTIES OUTPUT_NAME "mgl-qt5")
	endif(enable-mgl2)

	install(
		TARGETS mgl-qt5 mgl-qt5-static
		RUNTIME DESTINATION bin
		ARCHIVE DESTINATION ${MGL_LIB_INSTALL_DIR}
		LIBRARY DESTINATION ${MGL_LIB_INSTALL_DIR}
	)
	if(enable-qt5asqt)
	add_library(mgl-qt SHARED ${MGL_QT_FILES})
	add_library(mgl-qt-static STATIC ${MGL_QT_FILES})
	set_target_properties(mgl-qt PROPERTIES SOVERSION ${MathGL_SOVERSION})
	set_target_properties(mgl-qt PROPERTIES CLEAN_DIRECT_OUTPUT 1)
	set_target_properties(mgl-qt PROPERTIES DEFINE_SYMBOL "mgl_EXPORTS")
	set_target_properties(mgl-qt-static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
	set_target_properties(mgl-qt-static PROPERTIES COMPILE_FLAGS -DMGL_STATIC_DEFINE)
	target_link_libraries(mgl-qt mgl)
	qt5_use_modules(mgl-qt Core Widgets Gui Network WebKit PrintSupport)
	qt5_use_modules(mgl-qt-static Core Widgets Gui Network WebKit PrintSupport)
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
	endif(enable-qt5asqt)

	if(MGL_HAVE_FLTK)
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
		qt5_use_modules(mgl-wnd Core Widgets Gui Network WebKit PrintSupport)
		qt5_use_modules(mgl-wnd-static Core Widgets Gui Network WebKit PrintSupport)
		target_link_libraries(mgl-wnd ${FLTK_LIBRARIES})

		install(
			TARGETS mgl-wnd mgl-wnd-static
			RUNTIME DESTINATION bin
			ARCHIVE DESTINATION ${MGL_LIB_INSTALL_DIR}
			LIBRARY DESTINATION ${MGL_LIB_INSTALL_DIR}
		)
	endif(MGL_HAVE_FLTK)
else(enable-qt5)
	set(MGL_HAVE_QT5 0)
endif(enable-qt5)
