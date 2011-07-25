if(use_gsl)
#	target_link_libraries(mgl ${GSL_LIB} ${GSL_CBLAS_LIB} )
	add_definitions(-DHAVE_GSL)
	include_directories(${GSL_INCLUDE_DIR})
else(use_gsl)
	add_definitions(-DNO_GSL)
endif(use_gsl)

add_executable(mglconv mglconv.cpp)
target_link_libraries(mglconv mgl)

install(
	TARGETS mglconv
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

