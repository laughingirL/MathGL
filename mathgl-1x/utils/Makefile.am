AM_CXXFLAGS += -I$(top_srcdir)/include  -I$(top_builddir)/include

bin_PROGRAMS = mgl2png mgl2eps mgl2svg mgl2cpp
if USE_FLTK
bin_PROGRAMS += mglview
endif

if USE_GIF
bin_PROGRAMS += mgl2gif
endif

mgl2png_SOURCES = mgl2png.cpp
mgl2png_LDADD = $(top_builddir)/mgl/libmgl.la

mgl2cpp_SOURCES = mgl2cpp.cpp
mgl2cpp_LDADD = $(top_builddir)/mgl/libmgl.la

mgl2eps_SOURCES = mgl2eps.cpp
mgl2eps_LDADD = $(top_builddir)/mgl/libmgl.la

mgl2svg_SOURCES = mgl2svg.cpp
mgl2svg_LDADD = $(top_builddir)/mgl/libmgl.la

if USE_FLTK
mglview_SOURCES = mglview.cpp
mglview_LDADD = $(top_builddir)/widgets/libmgl-fltk.la $(top_builddir)/mgl/libmgl.la 
mglview_CXXFLAGS = $(FLTK_FLAGS) $(AM_CXXFLAGS)
endif

if USE_GIF
mgl2gif_SOURCES = mgl2gif.cpp
mgl2gif_LDADD = $(top_builddir)/mgl/libmgl.la
mgl2gif_CXXFLAGS = $(GIF_FLAGS) $(AM_CXXFLAGS)
endif
