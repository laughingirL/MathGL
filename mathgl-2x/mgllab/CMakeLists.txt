if(use_qt)


#set(mgl_wnd_src window.cpp)
set(lab_src main_wnd.cpp show_hint.cpp)

set(lab_moc_hdr main_wnd.h)

set(lab_rc mgllab.qrc)

if(use_gsl)
#	target_link_libraries(mgl ${GSL_LIB} ${GSL_CBLAS_LIB} )
	add_definitions(-DHAVE_GSL)
	include_directories(${GSL_INCLUDE_DIR})
else(use_gsl)
	add_definitions(-DNO_GSL)
endif(use_gsl)


include(${QT_USE_FILE})
add_definitions(-DHAVE_QT)
qt4_add_resources(lab_rc_src ${udav_rc} )
#qt_wrap_cpp(udav udav_moc_src ${udav_moc_hdr} )
qt4_wrap_cpp(lab_moc_src ${lab_moc_hdr} )
add_executable(mgllab ${lab_src} ${lab_moc_src} ${lab_rc_src})
target_link_libraries(mgllab mgl-wnd)
target_link_libraries(mgllab ${QT_LIBRARIES})

if(use_pthread)
	target_link_libraries(mgllab ${CMAKE_THREAD_LIBS_INIT})
	add_definitions(-DHAVE_PTHREAD)
endif(use_pthread)

install(
	TARGETS mgllab
	RUNTIME DESTINATION bin
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
)

endif(use_qt)
