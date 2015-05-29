if(enable-json-sample)


set(json_src Backend.cpp MainWindow.cpp)
set(json_moc_hdr Backend.hpp MainWindow.hpp)

if(enable-qt5)
	include(../cmake-qt5.txt)
	set(CMAKE_AUTOMOC ON)
	set(CMAKE_INCLUDE_CURRENT_DIR ON)
	qt5_wrap_ui(json_ui MainWindow.ui)
	add_executable(MglForJsTestBench ${json_src} ${json_moc_hdr} ${json_ui})
	target_compile_definitions(MglForJsTestBench PUBLIC MGL_USE_QT5)
	target_link_libraries(MglForJsTestBench mgl-qt5 ${MGL_QT5_LIBS} Qt5::Network Qt5::WebKit Qt5::WebKitWidgets)
else(enable-qt5)
	include(../cmake-qt4.txt)
	set(CMAKE_AUTOMOC ON)
	set(CMAKE_INCLUDE_CURRENT_DIR ON)
	qt4_wrap_ui(json_ui MainWindow.ui)
	add_executable(MglForJsTestBench ${json_src} ${json_moc_hdr}  ${json_ui})
	target_link_libraries(MglForJsTestBench mgl-qt4 ${MGL_QT4_LIBS} Qt4::QtNetwork Qt4::QtWebKit)
endif(enable-qt5)

endif(enable-json-sample)
