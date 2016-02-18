add_library(getopt SHARED getopt.c getopt.h)
set_target_properties(getopt PROPERTIES DEFINE_SYMBOL "EXPORTS_GETOPT")

install(
       TARGETS getopt
       RUNTIME DESTINATION bin
       ARCHIVE DESTINATION ${MGL_LIB_INSTALL_DIR}
       LIBRARY DESTINATION ${MGL_LIB_INSTALL_DIR}
)

