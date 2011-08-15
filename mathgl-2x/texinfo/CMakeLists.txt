
# Warning!!!!!
# new files and destination for html docs.

if(use_doc)
	add_custom_command(OUTPUT ${CMAKE_SOURCE_DIR}/texinfo/png/all.png
		COMMAND LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/mgl ${CMAKE_BINARY_DIR}/examples/mgl_example
		COMMAND echo "" > ${CMAKE_SOURCE_DIR}/texinfo/png/all.png
		MAIN_DEPENDENCY ${CMAKE_BINARY_DIR}/examples/mgl_example
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/texinfo/png
	)
	set_source_files_properties(${CMAKE_BINARY_DIR}/examples/mgl_example PROPERTIES GENERATED 1)
	ADD_CUSTOM_TARGET(gen_all_png DEPENDS ${CMAKE_SOURCE_DIR}/texinfo/png/all.png)
	ADD_DEPENDENCIES(gen_all_png mgl_example)
	set(list_texi_files_en mathgl_en.texi appendix_en.texi core_en.texi example_en.texi mgl_en.texi samples_en.texi data_en.texi overview_en.texi)
	set(list_texi_files_ru mathgl_ru.texi appendix_ru.texi core_ru.texi example_ru.texi mgl_ru.texi samples_ru.texi data_ru.texi overview_ru.texi)
	add_custom_command(OUTPUT ${CMAKE_SOURCE_DIR}/texinfo/mathgl_en.info ${CMAKE_SOURCE_DIR}/texinfo/mathgl_ru.info
		COMMAND ${findmi} --no-validate mathgl_en.texi
		COMMAND ${findmi} --no-validate mathgl_ru.texi
#		DEPENDS ${CMAKE_SOURCE_DIR}/texinfo/png/all.png
		DEPENDS ${list_texi_files_en}
		DEPENDS ${list_texi_files_ru}
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/texinfo
	)
	add_custom_command(OUTPUT ${CMAKE_SOURCE_DIR}/texinfo/mathgl_en.pdf
		COMMAND ${findtp} mathgl_en.texi
		COMMAND ${findtp} mathgl_en.texi
		COMMAND ${findtp} mathgl_en.texi
#		DEPENDS ${CMAKE_SOURCE_DIR}/texinfo/png/all.png
		DEPENDS ${list_texi_files_en}
		DEPENDS ${list_texi_files_ru}
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/texinfo
	)
	add_custom_command(OUTPUT ${CMAKE_SOURCE_DIR}/texinfo/mathgl_en
		COMMAND ${findth} --split=chapter mathgl_en.texi -o mathgl_en
#		DEPENDS ${CMAKE_SOURCE_DIR}/texinfo/png/all.png
		DEPENDS ${list_texi_files_en}
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/texinfo
	)
	add_custom_command(OUTPUT ${CMAKE_SOURCE_DIR}/texinfo/mathgl_ru
		COMMAND ${findth} --split=chapter mathgl_ru.texi -o mathgl_ru
#		DEPENDS ${CMAKE_SOURCE_DIR}/texinfo/png/all.png
		DEPENDS ${list_texi_files_ru}
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/texinfo
	)
	add_custom_command(OUTPUT ${CMAKE_SOURCE_DIR}/texinfo/mgl_en
		COMMAND ${findth} --split=chapter mgl_en.texi -o mgl_en
#		DEPENDS ${CMAKE_SOURCE_DIR}/texinfo/png/all.png
		DEPENDS ${list_texi_files_en}
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/texinfo
	)
	add_custom_command(OUTPUT ${CMAKE_SOURCE_DIR}/texinfo/mgl_ru
		COMMAND ${findth} --split=chapter mgl_ru.texi -o mgl_ru
#		DEPENDS ${CMAKE_SOURCE_DIR}/texinfo/png/all.png
		DEPENDS ${list_texi_files_ru}
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/texinfo
	)
	add_custom_target(documentation ALL
#		DEPENDS ${CMAKE_SOURCE_DIR}/texinfo/mathgl_en.pdf
#		DEPENDS ${CMAKE_SOURCE_DIR}/texinfo/mathgl_en.info
		DEPENDS ${CMAKE_SOURCE_DIR}/texinfo/mathgl_en
		DEPENDS ${CMAKE_SOURCE_DIR}/texinfo/mgl_en
#		DEPENDS ${CMAKE_SOURCE_DIR}/texinfo/mathgl_ru.pdf
#		DEPENDS ${CMAKE_SOURCE_DIR}/texinfo/mathgl_ru.info
		DEPENDS ${CMAKE_SOURCE_DIR}/texinfo/mathgl_ru
		DEPENDS ${CMAKE_SOURCE_DIR}/texinfo/mgl_ru
	)
	add_custom_target(clean
		COMMAND rm -f mathgl*.[hali]* mathgl*.?? mathgl_*/* *~ png/*.png
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/texinfo
	)
	install(DIRECTORY ./ DESTINATION ${MGL_DOC_PATH}
			FILES_MATCHING PATTERN ".svn" EXCLUDE
			PATTERN "CMakeFiles" EXCLUDE
			PATTERN "*.png"
			PATTERN "*/*.html"
			PATTERN "./mathgl*.info*"
			PATTERN "./mathgl*.pdf")
endif(use_doc)
