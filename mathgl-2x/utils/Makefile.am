AM_CXXFLAGS += -I$(top_srcdir)/include  -I$(top_builddir)/include

bin_PROGRAMS = mglconv
if USE_FLTK
bin_PROGRAMS += mglview
endif

mglconv_SOURCES = mglconv.cpp
mglconv_LDADD = $(top_builddir)/src/libmgl.la

if USE_FLTK
mglview_SOURCES = mglview.cpp
mglview_LDADD = $(top_builddir)/widgets/libmgl-wnd.la $(top_builddir)/src/libmgl.la
mglview_CXXFLAGS = $(FLTK_FLAGS) $(AM_CXXFLAGS)
endif

