install(DIRECTORY bigforth/ DESTINATION "${MGL_DATA_PATH}/bigforth"
                        PATTERN ".svn" EXCLUDE
                        PATTERN "*.fs")

set(MGL_INCLUDE_PATH "${CMAKE_INSTALL_PREFIX}/include/mgl")
install(DIRECTORY mgl/ DESTINATION ${MGL_INCLUDE_PATH}
                        PATTERN ".svn" EXCLUDE
                        PATTERN "mgl_fltk.h" EXCLUDE
                        PATTERN "mgl_glut.h" EXCLUDE
                        PATTERN "mgl_wx.h" EXCLUDE
                        PATTERN "mgl_qt.h" EXCLUDE
                        PATTERN "*.h")

if(use_fltk)
	install(FILES mgl/mgl_fltk.h DESTINATION ${MGL_INCLUDE_PATH})
endif(use_fltk)
if(use_glut)
	install(FILES mgl/mgl_glut.h DESTINATION ${MGL_INCLUDE_PATH})
endif(use_glut)
if(use_wx)
	install(FILES mgl/mgl_wx.h DESTINATION ${MGL_INCLUDE_PATH})
endif(use_wx)
if(use_qt)
	install(FILES mgl/mgl_qt.h DESTINATION ${MGL_INCLUDE_PATH})
endif(use_qt)
