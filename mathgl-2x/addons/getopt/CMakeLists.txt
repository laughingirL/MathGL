add_library(getopt SHARED getopt.c getopt.h)
target_compile_definitions(getopt PUBLIC EXPORTS_GETOPT)
add_library(getopt-static STATIC getopt.c getopt.h)
target_compile_definitions(getopt-static PUBLIC STATIC_GETOPT)

install(
       TARGETS getopt getopt-static
       RUNTIME DESTINATION bin
       ARCHIVE DESTINATION ${MGL_LIB_INSTALL_DIR}
       LIBRARY DESTINATION ${MGL_LIB_INSTALL_DIR}
)

