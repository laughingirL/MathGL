if(MGL_HAVE_QT)

set(json_src Backend.cpp main.cpp MainWindow.cpp)
set(json_moc_hdr Backend.hpp MainWindow.hpp)

include(${QT_USE_FILE})

qt4_wrap_cpp(json_moc_src ${json_moc_hdr} )
add_executable(MglForJsTestBench ${json_src} ${json_moc_src})
target_link_libraries(MglForJsTestBench mgl-qt ${QT_LIBRARIES})

endif(MGL_HAVE_QT)
