if(enable-json-sample)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(json_src Backend.cpp MainWindow.cpp)
set(json_moc_hdr Backend.hpp MainWindow.hpp)

add_executable(MglForJsTestBench ${json_src} ${json_moc_hdr})
if(enable-qt5)
	target_compile_definitions(MglForJsTestBench PUBLIC MGL_USE_QT5)
	target_link_libraries(MglForJsTestBench mgl-qt5 ${MGL_QT5_LIBS} Qt5::Network Qt5::WebKit Qt5::WebKitWidgets)
else(enable-qt5)
	target_link_libraries(MglForJsTestBench mgl-qt4 ${MGL_QT4_LIBS} Qt4::QtNetwork Qt4::QtWebKit)
endif(enable-qt5)

endif(enable-json-sample)
