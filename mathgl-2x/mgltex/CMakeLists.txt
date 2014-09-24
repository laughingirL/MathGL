set(extramgl Axis_projection Vectorial)

add_custom_command(OUTPUT scripts
    COMMAND ${CMAKE_COMMAND} -E make_directory ${MathGL_BINARY_DIR}/mgltex/scripts
    COMMAND ${CMAKE_COMMAND} -E copy ${MathGL_SOURCE_DIR}/mgltex/mgltex.tex ${MathGL_BINARY_DIR}/mgltex/
    COMMAND ${CMAKE_COMMAND} -E copy ${MathGL_SOURCE_DIR}/mgltex/mgltex.sty ${MathGL_BINARY_DIR}/mgltex/
    COMMAND ${findpdflatex} -draftmode mgltex.tex
)
foreach(mglscr ${extramgl})
	list(APPEND mglpng scripts/${mglscr}.png)
	add_custom_command(OUTPUT scripts/${mglscr}.png
		COMMAND mglconv -o ${mglscr}.png ${mglscr}.mgl
		DEPENDS mglconv scripts
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/mgltex/scripts/
	)
endforeach(mglscr)

add_custom_command(OUTPUT scripts/mgltex.png
	COMMAND mglconv -o ${mglscr}.png scripts/${mglscr}.mgl
	DEPENDS mglconv scripts
)

add_custom_command(OUTPUT mgltex.pdf
	COMMAND ${findpdflatex} mgltex.tex
	DEPENDS ${mglpng})
add_custom_target(pdf ALL DEPENDS mgltex.pdf)

get_directory_property(mglconv_clean ADDITIONAL_MAKE_CLEAN_FILES)
set(mglconv_clean ${mglconv_clean} scripts mgltex.sty mgltex.tex mgltex.aux mgltex.log)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${mglconv_clean}")

install(FILES mgltex.sty DESTINATION ${CMAKE_INSTALL_PREFIX}/share/texmf/tex/latex/mgltex/)
install(FILES mgltex.tex DESTINATION ${CMAKE_INSTALL_PREFIX}/share/texmf/doc/latex/mgltex/)
install(FILES ${MathGL_BINARY_DIR}/mgltex/mgltex.pdf DESTINATION ${CMAKE_INSTALL_PREFIX}/share/texmf/doc/latex/mgltex/)
# TODO mktexlsr -- see http://en.wikibooks.org/wiki/LaTeX/Installing_Extra_Packages