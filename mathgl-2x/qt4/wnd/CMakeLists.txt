include(GenerateExportHeader)
add_compiler_export_flags()

if(MGL_HAVE_FLTK)
	include_directories(${FLTK_INCLUDE_DIR})

	set(MGL_WND_SRC ../../widgets/qt.cpp ../../include/mgl2/qt.h ../../include/mgl2/qmathgl.h ../../widgets/fltk.cpp ../../include/mgl2/fltk.h)

	add_library(mgl-wnd SHARED ${MGL_WND_SRC} ${MGL_MOC_FILES})
	add_library(mgl-wnd-static STATIC ${MGL_WND_SRC} ${MGL_MOC_FILES})

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
endif(MGL_HAVE_FLTK)
