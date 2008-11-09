add_executable(mgl2png mgl2png.cpp)
target_link_libraries(mgl2png mgl)

add_executable(mgl2eps mgl2eps.cpp)
target_link_libraries(mgl2eps mgl)

add_executable(mgl2svg mgl2svg.cpp)
target_link_libraries(mgl2svg mgl)

add_executable(mgl2cpp mgl2cpp.cpp)
target_link_libraries(mgl2cpp mgl)

install(
	TARGETS mgl2png mgl2eps mgl2svg mgl2cpp
	RUNTIME DESTINATION bin
)

if(use_fltk)
add_executable(mglview mglview.cpp)
target_link_libraries(mglview mgl-fltk mgl)
install(
	TARGETS mglview
	RUNTIME DESTINATION bin
)
endif(use_fltk)

