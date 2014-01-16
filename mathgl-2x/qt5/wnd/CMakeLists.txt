include(GenerateExportHeader)
add_compiler_export_flags()

if(MGL_HAVE_FLTK)
	include_directories(${FLTK_INCLUDE_DIR})

	set(MGL_WND_SRC ../../widgets/qt.cpp ../../include/mgl2/qt.h ../../include/mgl2/qmathgl.h ../../widgets/fltk.cpp ../../include/mgl2/fltk.h)

	add_library(mgl-wnd-qt5 SHARED ${MGL_WND_SRC} ${MGL_MOC_FILES})
	add_library(mgl-wnd-static-qt5 STATIC ${MGL_WND_SRC} ${MGL_MOC_FILES})

	set_target_properties(mgl-wnd-qt5 PROPERTIES SOVERSION ${MathGL_SOVERSION})
	set_target_properties(mgl-wnd-qt5 PROPERTIES CLEAN_DIRECT_OUTPUT 1)
	set_target_properties(mgl-wnd-qt5 PROPERTIES DEFINE_SYMBOL "mgl_EXPORTS")
	set_target_properties(mgl-wnd-static-qt5 PROPERTIES CLEAN_DIRECT_OUTPUT 1)
	set_target_properties(mgl-wnd-static-qt5 PROPERTIES COMPILE_FLAGS -DMGL_STATIC_DEFINE)

	if(enable-mgl2)
		set_target_properties(mgl-wnd-qt5 PROPERTIES OUTPUT_NAME "mgl2-wnd-qt5")
		set_target_properties(mgl-wnd-static-qt5 PROPERTIES OUTPUT_NAME "mgl2-wnd-qt5")
	else(enable-mgl2)
		set_target_properties(mgl-wnd-static-qt5 PROPERTIES OUTPUT_NAME "mgl-wnd-qt5")
	endif(enable-mgl2)

	target_link_libraries(mgl-wnd-qt5 mgl)
	qt5_use_modules(mgl-wnd-qt5 Core Widgets Gui Network WebKit PrintSupport)
	qt5_use_modules(mgl-wnd-static-qt5 Core Widgets Gui Network WebKit PrintSupport)
	target_link_libraries(mgl-wnd-qt5 ${FLTK_LIBRARIES})

	install(
		TARGETS mgl-wnd-qt5 mgl-wnd-static-qt5
		RUNTIME DESTINATION bin
		ARCHIVE DESTINATION ${MGL_LIB_INSTALL_DIR}
		LIBRARY DESTINATION ${MGL_LIB_INSTALL_DIR}
	)
endif(MGL_HAVE_FLTK)
