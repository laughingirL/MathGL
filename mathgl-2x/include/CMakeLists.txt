if(use_double)
	set(MGL_USE_DOUBLE 1)
else(use_double)
	set(MGL_USE_DOUBLE 0)
endif(use_double)

configure_file(${CMAKE_SOURCE_DIR}/include/config.h.in ${CMAKE_SOURCE_DIR}/include/mgl/config.h)

set(MGL_INCLUDE_PATH "${CMAKE_INSTALL_PREFIX}/include/mgl")
install(DIRECTORY mgl/ DESTINATION ${MGL_INCLUDE_PATH}
                        PATTERN ".svn" EXCLUDE
                        PATTERN "mgl_fltk.h" EXCLUDE
                        PATTERN "mgl_glut.h" EXCLUDE
                        PATTERN "mgl_wx.h" EXCLUDE
                        PATTERN "mgl_qt.h" EXCLUDE
                        PATTERN "*.fs"
                        PATTERN "*.pas"
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
