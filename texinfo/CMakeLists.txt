if(use_doc)
	add_custom_command(OUTPUT ${CMAKE_SOURCE_DIR}/texinfo/png/all.png
		COMMAND LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/mgl ${CMAKE_BINARY_DIR}/examples/mgl_example
		COMMAND touch all.png
		MAIN_DEPENDENCY ${CMAKE_BINARY_DIR}/examples/mgl_example
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/texinfo/png
	)
	file(GLOB_RECURSE doc_file_list *.texi *.png)
	add_custom_command(OUTPUT mathgl.info
		COMMAND ${findmi} --no-validate mathgl.texi
		MAIN_DEPENDENCY mathgl.texi
		DEPENDS ${doc_file_file_list} ${CMAKE_SOURCE_DIR}/texinfo/png/all.png
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/texinfo
	)
	add_custom_command(OUTPUT mathgl.pdf
		COMMAND ${findtp} mathgl.texi
		COMMAND ${findtp} mathgl.texi
		COMMAND ${findtp} mathgl.texi
		MAIN_DEPENDENCY mathgl.texi
		DEPENDS ${doc_file_file_list} ${CMAKE_SOURCE_DIR}/texinfo/png/all.png
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/texinfo
	)
	add_custom_command(OUTPUT mathgl
		COMMAND ${findth} --split=chapter mathgl.texi
		MAIN_DEPENDENCY mathgl.texi
		DEPENDS ${doc_file_file_list} ${CMAKE_SOURCE_DIR}/texinfo/png/all.png
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/texinfo
	)
	add_custom_target(documentation ALL
		DEPENDS mathgl.pdf mathgl.info mathgl
	)
	add_custom_target(clean
		COMMAND rm -f mathgl.[^t]* mathgl.t[^e]* mathgl.te[^x]* mathgl.tex[^i] ${CMAKE_SOURCE_DIR}/texinfo/png/*.png mathgl.info*
	)
	install(DIRECTORY ./ DESTINATION ${MGL_DOC_PATH}
			FILES_MATCHING PATTERN ".svn" EXCLUDE
			PATTERN "CMakeFiles" EXCLUDE
			PATTERN "*.png"
			PATTERN "*.html"
			PATTERN "./mathgl.info*"
			PATTERN "./mathgl.pdf")
endif(use_doc)
