#set(mgl_wnd_src window.cpp)
set(udav_src anim_dlg.cpp find_dlg.cpp mem_pnl.cpp prop_dlg.cpp textedit.cpp args_dlg.cpp help_pnl.cpp newcmd_dlg.cpp qmglcanvas.cpp text_pnl.cpp calc_dlg.cpp hint_dlg.cpp open_dlg.cpp qmglsyntax.cpp udav_wnd.cpp dat_pnl.cpp info_dlg.cpp opt_dlg.cpp setup_dlg.cpp files_dlg.cpp main.cpp plot_pnl.cpp style_dlg.cpp)

set(udav_moc_hdr anim_dlg.h files_dlg.h info_dlg.h opt_dlg.h qmglsyntax.h text_pnl.h args_dlg.h find_dlg.h mem_pnl.h plot_pnl.h setup_dlg.h udav_wnd.h calc_dlg.h help_pnl.h newcmd_dlg.h prop_dlg.h style_dlg.h dat_pnl.h hint_dlg.h open_dlg.h qmglcanvas.h textedit.h)

set(udav_rc udav.qrc)

if(use_gsl)
#	target_link_libraries(mgl ${GSL_LIB} ${GSL_CBLAS_LIB} )
	add_definitions(-DHAVE_GSL)
	include_directories(${GSL_INCLUDE_DIR})
else(use_gsl)
	add_definitions(-DNO_GSL)
endif(use_gsl)


include(${QT_USE_FILE})
add_definitions(-DHAVE_QT)
qt4_add_resources(udav_rc_src ${udav_rc} )
#qt_wrap_cpp(udav udav_moc_src ${udav_moc_hdr} )
qt4_wrap_cpp(udav_moc_src ${udav_moc_hdr} )
add_executable(udav ${udav_src} ${udav_moc_src} ${udav_rc_src})
target_link_libraries(udav mgl)
target_link_libraries(udav ${QT_LIBRARIES})

if(use_pthread)
	target_link_libraries(udav ${CMAKE_THREAD_LIBS_INIT})
	add_definitions(-DHAVE_PTHREAD)
endif(use_pthread)

install(
	TARGETS udav
	RUNTIME DESTINATION bin
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
)
