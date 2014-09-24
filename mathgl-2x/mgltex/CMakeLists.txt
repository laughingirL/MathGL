set(listmgl Axis_projection Test Vectorial)

add_custom_command(OUTPUT scripts
    COMMAND ${CMAKE_COMMAND} -E make_directory ${MathGL_BINARY_DIR}/mgltex/scripts
    COMMAND ${CMAKE_COMMAND} -E copy ${MathGL_SOURCE_DIR}/mgltex/Test.tex ${MathGL_BINARY_DIR}/mgltex/
    COMMAND ${CMAKE_COMMAND} -E copy ${MathGL_SOURCE_DIR}/mgltex/mgltex.sty ${MathGL_BINARY_DIR}/mgltex/
    COMMAND ${findpdflatex} -draftmode Test.tex
)
foreach(mglscr ${listmgl})
	list(APPEND mglpng scripts/${mglscr}.png)
	add_custom_command(OUTPUT scripts/${mglscr}.png
		COMMAND mglconv -o ${mglscr}.png ${mglscr}.mgl
		DEPENDS mglconv scripts
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/mgltex/scripts
	)
endforeach(mglscr)

add_custom_command(OUTPUT Test.pdf
	COMMAND ${findpdflatex} Test.tex
	DEPENDS ${mglpng})
add_custom_target(pdf ALL DEPENDS Test.pdf)

get_directory_property(mglconv_clean ADDITIONAL_MAKE_CLEAN_FILES)
set(mglconv_clean ${mglconv_clean} scripts mgltex.sty Test.tex Test.aux Test.log)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${mglconv_clean}")
