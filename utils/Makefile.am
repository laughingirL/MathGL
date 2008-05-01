INCLUDES        = -I$(top_builddir)/include

bin_PROGRAMS = mgl2png mgl2eps mgl2svg mgl2cpp
if USE_FLTK
bin_PROGRAMS += mglview
endif

mgl2png_SOURCES = mgl2png.cpp
mgl2png_LDADD = $(top_builddir)/mgl/libmgl.la
#mgl2png_CFLAGS = -O3 -mfpmath=sse $(HDF5_FLAGS) $(JPEG_FLAGS) $(TIFFFLAGS) $(PNGFLAGS)
mgl2png_CPPFLAGS = $(libmgl_la_CFLAGS)

mgl2cpp_SOURCES = mgl2cpp.cpp
mgl2cpp_LDADD = $(top_builddir)/mgl/libmgl.la
#mgl2cpp_CFLAGS = -O3 -mfpmath=sse $(HDF5_FLAGS) $(JPEG_FLAGS) $(TIFFFLAGS) $(PN
mgl2cpp_CPPFLAGS = $(libmgl_la_CFLAGS)

mgl2eps_SOURCES = mgl2eps.cpp
mgl2eps_LDADD = $(top_builddir)/mgl/libmgl.la
#mgl2eps_CFLAGS = -O3 -mfpmath=sse $(HDF5FLAGS) $(JPEGFLAGS) $(TIFF_FLAGS) $(PNG_FLAGS)
mgl2eps_CPPFLAGS = $(libmgl_la_CFLAGS)

mgl2svg_SOURCES = mgl2svg.cpp
mgl2svg_LDADD = $(top_builddir)/mgl/libmgl.la
#mgl2svg_CFLAGS = -O3 -mfpmath=sse $(HDF5FLAGS) $(JPEGFLAGS) $(TIFFFLAGS) $(PNGFLAGS)
mgl2svg_CPPFLAGS = $(libmgl_la_CFLAGS)

if USE_FLTK
mglview_SOURCES = mglview.cpp
mglview_LDADD = $(top_builddir)/mgl/libmgl-fltk.la
mglview_CFLAGS = -O2 $(FLTK_FLAGS)
mglview_CPPFLAGS = $(mglview_CFLAGS)
endif

#FLAGS = -O3 -march=opteron -mfpmath=sse -DNO_HDF5 -DNO_JPEG -DNO_TIFF
#GLIBS = -L/usr/X11R6/lib -lgsl -lgslcblas -lm -lglut -lpng -lGL -lGLU -lXmu -lX11 -lXi -lXext
