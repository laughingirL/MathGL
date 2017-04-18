include(GenerateExportHeader)

if(MGL_HAVE_FLTK)
	mgl_add_lib(fltk fltk.cpp  image.cpp  image.h ../include/mgl2/fltk.h ../include/mgl2/Fl_MathGL.h)
	target_include_directories(mgl-fltk SYSTEM PUBLIC ${FLTK_INCLUDE_DIR})
	target_include_directories(mgl-fltk-static SYSTEM PUBLIC ${FLTK_INCLUDE_DIR})
	target_link_libraries(mgl-fltk mgl ${FLTK_LIBRARIES})
	target_link_libraries(mgl-fltk-static mgl-static ${FLTK_LIBRARIES})
endif(MGL_HAVE_FLTK)

if(MGL_HAVE_GLUT)
	mgl_add_lib(glut glut.cpp ../include/mgl2/glut.h)
	target_include_directories(mgl-glut SYSTEM PUBLIC ${GLUT_INCLUDE_DIR})
	target_include_directories(mgl-glut-static SYSTEM PUBLIC ${GLUT_INCLUDE_DIR})
	target_link_libraries(mgl-glut mgl ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})
	target_link_libraries(mgl-glut-static mgl-static ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})
endif(MGL_HAVE_GLUT)

if(MGL_HAVE_WX)
	mgl_add_lib(wx wx.cpp ../include/mgl2/wx.h)
	include(${wxWidgets_USE_FILE})
	target_link_libraries(mgl-wx mgl ${wxWidgets_LIBRARIES})
	target_link_libraries(mgl-wx-static mgl-static ${wxWidgets_LIBRARIES})
endif(MGL_HAVE_WX)

add_subdirectory( qt4 )
add_subdirectory( qt5 )

if(USE_GETTEXT)
	set(dir widgets)
	set(src fltk.cpp ../include/mgl2/fltk.h ../include/mgl2/Fl_MathGL.h glut.cpp ../include/mgl2/glut.h wx.cpp ../include/mgl2/wx.h qt.cpp ../include/mgl2/qt.h ../include/mgl2/wnd.h  ../include/mgl2/qmathgl.h )
	add_custom_target(${dir}_po
		COMMAND ${findxgettext} -s --keyword=_mgl -C -c --package-name=MathGL2 --package-version=${MathGL_VERSION} -o ${MathGL2_BINARY_DIR}/${dir}/${dir}.pot ${src}
		COMMAND ${findmsgmerge} -o strings_ru.po strings_ru.po ${MathGL2_BINARY_DIR}/${dir}/${dir}.pot
		WORKING_DIRECTORY ${MathGL2_SOURCE_DIR}/${dir}/
		DEPENDS ${src} strings_ru.po)
	set(po_targets ${po_targets} ${dir}_po PARENT_SCOPE)
	set(po_files ${po_files} ${MathGL2_SOURCE_DIR}/${dir}/strings_ru.po PARENT_SCOPE)
endif(USE_GETTEXT)
