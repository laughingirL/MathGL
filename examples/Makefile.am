INCLUDES        = -I$(top_builddir)/include -I$(top_builddir)
INCLUDES        += @INCLTDL@

bin_PROGRAMS = mgl_example
check_PROGRAMS = test

if USE_FLTK
bin_PROGRAMS += mgl_fltk_example
endif
if USE_GLUT
bin_PROGRAMS += mgl_glut_example
endif
mgl_example_SOURCES = full_test.cpp
mgl_example_LDADD = $(top_builddir)/mgl/libmgl.la
mgl_example_CFLAGS = 
mgl_example_CPPFLAGS = $(mgl_example_CFLAGS)

if USE_FLTK
mgl_fltk_example_SOURCES = fltk_example.cpp
mgl_fltk_example_LDADD = $(top_builddir)/mgl/libmgl-fltk.la
mgl_fltk_example_CFLAGS = 
mgl_fltk_example_CPPFLAGS = $(mgl_fltk_example_CFLAGS)
endif

if USE_GLUT
mgl_glut_example_SOURCES = glut_example.cpp
mgl_glut_example_LDADD = $(top_builddir)/mgl/libmgl-glut.la
mgl_glut_example_CFLAGS = 
mgl_glut_example_CPPFLAGS = $(mgl_glut_example_CFLAGS)
endif

test_SOURCES = main.cpp
test_LDADD = $(top_builddir)/mgl/libmgl-fltk.la
test_CFLAGS = 
test_CPPFLAGS = $(test_CFLAGS)
