if(use_fltk)
	add_definitions(-DHAVE_FLTK)
	include_directories(${FLTK_INCLUDE_DIR})
	set(mgl_lab_src animate.cpp editor.cpp help.cpp mathgl.cpp setup.cpp write.cpp data.cpp grid.cpp main.cpp option.cpp table.cpp)
	add_executable(mgllab ${mgl_lab_src})
	target_link_libraries(mgllab mgl-wnd mgl)
	install(
		TARGETS mgllab
		RUNTIME DESTINATION bin	)
endif(use_fltk)

