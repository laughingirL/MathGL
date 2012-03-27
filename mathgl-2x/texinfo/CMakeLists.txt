
# Warning!!!!!
# new files and destination for html docs.

if(MGL_HAVE_DOC)
	add_custom_command(OUTPUT ${CMAKE_SOURCE_DIR}/texinfo/png/all.png
#		COMMAND LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/mgl ${CMAKE_BINARY_DIR}/examples/mgl_example
		COMMAND ${CMAKE_BINARY_DIR}/examples/mgl_example
		COMMAND touch ${CMAKE_SOURCE_DIR}/texinfo/png/all.png
#		MAIN_DEPENDENCY ${CMAKE_BINARY_DIR}/examples/mgl_example
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/texinfo/png
	)
  add_custom_command(OUTPUT ${CMAKE_SOURCE_DIR}/texinfo/small/all.png
    COMMAND ${CMAKE_BINARY_DIR}/examples/mgl_example -mini
    COMMAND touch ${CMAKE_SOURCE_DIR}/texinfo/small/all.png
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/texinfo/small
  )
#	set_source_files_properties(${CMAKE_BINARY_DIR}/examples/mgl_example PROPERTIES GENERATED 1)
#	ADD_CUSTOM_TARGET(gen_all_png DEPENDS ${CMAKE_SOURCE_DIR}/texinfo/png/all.png)
#	ADD_DEPENDENCIES(gen_all_png mgl_example)
	set(list_texi_files_en mathgl_en.texi mgl_en.texi overview_en.texi example_en.texi ex_mgl_en.texi parse_en.texi core_en.texi concept_en.texi widget_en.texi data_en.texi other_en.texi appendix_en.texi fdl.texi)
	set(list_texi_files_ru mathgl_ru.texi mgl_ru.texi overview_ru.texi example_ru.texi ex_mgl_ru.texi parse_ru.texi core_ru.texi concept_ru.texi widget_ru.texi data_ru.texi other_ru.texi appendix_ru.texi fdl.texi)

  add_custom_command(OUTPUT ${CMAKE_SOURCE_DIR}/texinfo/web_en.pdf
    COMMAND ${findtp} -I ${CMAKE_SOURCE_DIR}/texinfo/png web_en.texi
    COMMAND ${findth} -I=${CMAKE_SOURCE_DIR}/texinfo/png --split=section web_en.texi -o web_en
    DEPENDS ${CMAKE_SOURCE_DIR}/texinfo/small/all.png web_en.texi
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/texinfo
  )

	add_custom_command(OUTPUT ${CMAKE_SOURCE_DIR}/texinfo/mathgl_en.info
		COMMAND ${findth} -I=${CMAKE_SOURCE_DIR}/texinfo/png --split=section mathgl_en.texi -o mathgl_en
		COMMAND ${findth} -I=${CMAKE_SOURCE_DIR}/texinfo/png --split=node --frames -o mgl_web_en mathgl_en.texi
		COMMAND ${findth} -I=${CMAKE_SOURCE_DIR}/texinfo/png --split=no mgl_en.texi -o mgl_en.html
		COMMAND ${findmi} --no-validate mathgl_en.texi
		DEPENDS ${CMAKE_SOURCE_DIR}/texinfo/png/all.png
		DEPENDS ${list_texi_files_en}
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/texinfo
	)
	add_custom_command(OUTPUT ${CMAKE_SOURCE_DIR}/texinfo/mathgl_ru.info
		COMMAND ${findth} -I=${CMAKE_SOURCE_DIR}/texinfo/png --split=section mathgl_ru.texi -o mathgl_ru
		COMMAND ${findth} -I=${CMAKE_SOURCE_DIR}/texinfo/png --split=no mgl_ru.texi -o mgl_ru.html
		COMMAND ${findmi} --no-validate mathgl_ru.texi
		DEPENDS ${CMAKE_SOURCE_DIR}/texinfo/png/all.png
		DEPENDS ${list_texi_files_ru}
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/texinfo
	)
	add_custom_command(OUTPUT ${CMAKE_SOURCE_DIR}/texinfo/mathgl_en.pdf
		COMMAND ${findtp} -I ${CMAKE_SOURCE_DIR}/texinfo/png mathgl_en.texi
		COMMAND ${findtp} -I ${CMAKE_SOURCE_DIR}/texinfo/png mathgl_en.texi
		COMMAND ${findtp} -I ${CMAKE_SOURCE_DIR}/texinfo/png mathgl_en.texi
		DEPENDS ${CMAKE_SOURCE_DIR}/texinfo/png/all.png
		DEPENDS ${list_texi_files_en}
#		DEPENDS ${list_texi_files_ru}
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/texinfo
	)
	add_custom_target(documentation ALL
		DEPENDS ${CMAKE_SOURCE_DIR}/texinfo/mathgl_en.info
		DEPENDS ${CMAKE_SOURCE_DIR}/texinfo/mathgl_en.pdf
    DEPENDS ${CMAKE_SOURCE_DIR}/texinfo/web_en.pdf
#		DEPENDS ${CMAKE_SOURCE_DIR}/texinfo/mathgl_ru.info
#		DEPENDS ${CMAKE_SOURCE_DIR}/texinfo/mathgl_ru.pdf
	)
	add_custom_target(clean
		COMMAND rm -f mathgl*.[hali]* mathgl*.?? mathgl_*/* *~ #png/*.png
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/texinfo
	)
	install(DIRECTORY ./ DESTINATION ${MGL_DOC_PATH}
			FILES_MATCHING PATTERN ".svn" EXCLUDE
			PATTERN "CMakeFiles" EXCLUDE
			PATTERN "*.png"
			PATTERN "*.html"
			PATTERN "*/*.html"
			PATTERN "./mathgl*.info*"
			PATTERN "./mathgl*.pdf")
	install(DIRECTORY ./ DESTINATION ${MGL_MAN_PATH}/man1
			FILES_MATCHING PATTERN ".svn" EXCLUDE
			PATTERN "CMakeFiles" EXCLUDE
			PATTERN "*_en" EXCLUDE
			PATTERN "png" EXCLUDE
			PATTERN "small" EXCLUDE
			PATTERN "*.1")
	install(DIRECTORY ./ DESTINATION ${MGL_MAN_PATH}/man5
			FILES_MATCHING PATTERN ".svn" EXCLUDE
			PATTERN "CMakeFiles" EXCLUDE
			PATTERN "*_en" EXCLUDE
			PATTERN "png" EXCLUDE
			PATTERN "small" EXCLUDE
			PATTERN "*.5")
endif(MGL_HAVE_DOC)
