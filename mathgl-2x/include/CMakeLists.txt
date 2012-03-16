configure_file(${MathGL_SOURCE_DIR}/include/config.h.in ${MathGL_BINARY_DIR}/include/mgl/config.h)

set(MGL_INCLUDE_PATH "${CMAKE_INSTALL_PREFIX}/include/mgl")
install(DIRECTORY mgl/ DESTINATION ${MGL_INCLUDE_PATH}
                        PATTERN ".svn" EXCLUDE
                        PATTERN "fltk.h" EXCLUDE
                        PATTERN "glut.h" EXCLUDE
                        PATTERN "wx.h" EXCLUDE
                        PATTERN "qt.h" EXCLUDE
                        PATTERN "*~" EXCLUDE
                        PATTERN "*.fs"
                        PATTERN "*.pas"
                        PATTERN "*.h")

install(FILES ${MathGL_BINARY_DIR}/include/mgl/config.h DESTINATION ${MGL_INCLUDE_PATH})
if(MGL_HAVE_FLTK)
	install(FILES mgl/fltk.h DESTINATION ${MGL_INCLUDE_PATH})
endif(MGL_HAVE_FLTK)
if(MGL_HAVE_GLUT)
	install(FILES mgl/glut.h DESTINATION ${MGL_INCLUDE_PATH})
endif(MGL_HAVE_GLUT)
if(MGL_HAVE_WX)
	install(FILES mgl/wx.h DESTINATION ${MGL_INCLUDE_PATH})
endif(MGL_HAVE_WX)
if(MGL_HAVE_QT)
	install(FILES mgl/qt.h DESTINATION ${MGL_INCLUDE_PATH})
endif(MGL_HAVE_QT)
