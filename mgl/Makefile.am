INCLUDES        = -I$(top_builddir)/include -I$(top_builddir)
INCLUDES        += @INCLTDL@

lib_LTLIBRARIES = libmgl.la
if USE_FLTK
lib_LTLIBRARIES += libmgl-fltk.la
endif
if USE_GLUT
lib_LTLIBRARIES += libmgl-glut.la
endif


mgllib_LTLIBRARIES = 
mgllibdir = $(libdir)/mgl
if USE_JPEG
mgllib_LTLIBRARIES += mgl-jpeg.la
endif
if USE_HDF5
mgllib_LTLIBRARIES += mgl-hdf5.la
endif
if USE_TIFF
mgllib_LTLIBRARIES += mgl-tiff.la
endif

#libmgl_la_LDFLAGS = -release $(MGL_RELEASE) -no-undefined -version-info $(MGL_CURRENT):$(MGL_REVISION):$(MGL_AGE)
libmgl_la_LDFLAGS = -no-undefined -version-info $(MGL_CURRENT):$(MGL_REVISION):$(MGL_AGE)
libmgl_la_LIBADD = @LIBLTDL@ $(PNG_LIBS) $(GSL_LIBS) $(GL_LIBS)

libmgl_la_CFLAGS = -O2 -DMGL_FONT_PATH=\"$(datadir)/$(PACKAGE)/fonts/\" -DMOD_LIB_DIR=\"$(libdir)/mgl/\" $(HDF5_FLAGS) $(JPEG_FLAGS) $(TIFF_FLAGS) $(PNG_FLAGS) $(FLTK_FLAGS) $(ADDON_FLAGS) 
libmgl_la_CPPFLAGS = $(libmgl_la_CFLAGS)

libmgl_la_SOURCES = \
mgl_data_cf.cpp	   mgl_axis.cpp     mgl_data_png.cpp  mgl_zb2.cpp \
mgl_c.cpp          mgl_eps.cpp      mgl_font.cpp      mgl_zb.cpp \
mgl_combi.cpp      mgl_eval.cpp     mgl_gl.cpp        mgl_def_font.cpp\
mgl_1d.cpp         mgl_cont.cpp     mgl_exec.cpp      mgl_tex_table.cpp\
mgl_2d.cpp         mgl_crust.cpp    mgl_export.cpp    mgl_main.cpp  \
mgl_3d.cpp         mgl_data.cpp     mgl_f.cpp         mgl_idtf.cpp \
mgl_ab.cpp         mgl_data_io.cpp  mgl_flow.cpp      mgl_vect.cpp \
mgl_parse.cpp      mgl_fit.cpp

if USE_FLTK
libmgl_fltk_la_LDFLAGS = -no-undefined -version-info $(MGL_CURRENT):$(MGL_REVISION):$(MGL_AGE)
libmgl_fltk_la_LIBADD = $(FLTK_LIBS) libmgl.la
libmgl_fltk_la_CFLAGS = -O2 $(FLTK_FLAGS)
libmgl_fltk_la_CPPFLAGS = $(libmgl_fltk_la_CFLAGS)
libmgl_fltk_la_SOURCES = mgl_fltk.cpp
endif

if USE_GLUT
libmgl_glut_la_LDFLAGS = -no-undefined -version-info $(MGL_CURRENT):$(MGL_REVISION):$(MGL_AGE)
libmgl_glut_la_LIBADD = $(GLUT_LIBS) libmgl.la
libmgl_glut_la_CFLAGS = -O2 $(GLUT_FLAGS)
libmgl_glut_la_CPPFLAGS = $(libmgl_glut_la_CFLAGS)
libmgl_glut_la_SOURCES = mgl_glut.cpp
endif

if USE_JPEG
mgl_jpeg_la_LDFLAGS = -no-undefined -module -avoid-version -shared
mgl_jpeg_la_LIBADD = $(JPEG_LIBS)
mgl_jpeg_la_CFLAGS = -O2 $(JPEG_FLAGS)
mgl_jpeg_la_CPPFLAGS = $(libmgl_jpeg_la_CFLAGS)
mgl_jpeg_la_SOURCES = mgl_zb_jpg.cpp
endif

if USE_HDF5
mgl_hdf5_la_LDFLAGS = -no-undefined -module -avoid-version -shared
mgl_hdf5_la_LIBADD = $(HDF5_LIBS)
mgl_hdf5_la_CFLAGS = -O2 $(HDF5_FLAGS)
mgl_hdf5_la_CPPFLAGS = $(libmgl_hdf5_la_CFLAGS)
mgl_hdf5_la_SOURCES = data_hdf.cpp
endif

if USE_TIFF
mgl_tiff_la_LDFLAGS = -no-undefined -module -avoid-version -shared
mgl_tiff_la_LIBADD = $(TIFF_LIBS)
mgl_tiff_la_CFLAGS = -O2 $(TIFF_FLAGS)
mgl_tiff_la_CPPFLAGS = $(libmgl_tiff_la_CFLAGS)
mgl_tiff_la_SOURCES = mgl_zb_tiff.cpp
endif



#FLAGS = -O3 -march=opteron -mfpmath=sse -DNO_HDF5 -DNO_JPEG -DNO_TIFF
#GLIBS = -L/usr/X11R6/lib -lgsl -lgslcblas -lm -lglut -lpng -lGL -lGLU -lXmu -lX11 -lXi -lXext
