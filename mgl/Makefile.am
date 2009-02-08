INCLUDES        = -I$(top_builddir)/include -I$(top_builddir)

lib_LTLIBRARIES = libmgl.la

WIDGETS =
if USE_QT
WIDGETS += -DWITH_QT
endif
if USE_WX
WIDGETS += -DWITH_WX
endif
if USE_FLTK
WIDGETS += -DWITH_FLTK
endif
if USE_GLUT
WIDGETS += -DWITH_GLUT
endif

libmgl_la_LIBADD = $(PNG_LIBS) $(GL_LIBS) -lgomp
libmgl_la_LDFLAGS = -no-undefined -version-info $(MGL_CURRENT):$(MGL_REVISION):$(MGL_AGE)

libmgl_la_CXXFLAGS = $(WIDGETS) -DMGL_FONT_PATH=\"$(datadir)/$(PACKAGE)/fonts/\" \
    -DMOD_LIB_DIR=\"$(libdir)/mgl/\" $(HDF5_FLAGS) $(GIF_FLAGS) $(JPEG_FLAGS) $(TIFF_FLAGS) \
    $(PNG_FLAGS) $(FLTK_FLAGS) $(ADDON_FLAGS) $(AM_CXXFLAGS) -fopenmp

if USE_GSL
libmgl_la_LIBADD += $(GSL_LIBS)
endif

if USE_HDF5
libmgl_la_LIBADD += $(HDF5_LIBS)
endif

if USE_GIF
libmgl_la_LIBADD += $(GIF_LIBS)
endif

if USE_JPEG
libmgl_la_LIBADD += $(JPEG_LIBS)
endif

if USE_LTDL
INCLUDES        += @INCLTDL@
libmgl_la_LIBADD += @LIBLTDL@
libmgl_la_CXXFLAGS += -DWITH_LTDL
endif

libmgl_la_SOURCES = \
mgl_data_cf.cpp	   mgl_axis.cpp     mgl_data_png.cpp  mgl_zb2.cpp \
mgl_c.cpp          mgl_eps.cpp      mgl_font.cpp      mgl_zb.cpp \
mgl_combi.cpp      mgl_eval.cpp     mgl_gl.cpp        mgl_def_font.cpp\
mgl_1d.cpp         mgl_cont.cpp     mgl_exec.cpp      mgl_tex_table.cpp\
mgl_2d.cpp         mgl_crust.cpp    mgl_export.cpp    mgl_main.cpp  \
mgl_3d.cpp         mgl_data.cpp     mgl_f.cpp         mgl_idtf.cpp \
mgl_ab.cpp         mgl_data_io.cpp  mgl_flow.cpp      mgl_vect.cpp \
mgl_parse.cpp      mgl_fit.cpp      mgl_pde.cpp       mgl_evalc.cpp \
mgl_addon.cpp

if USE_TESTIO
bin_PROGRAMS = test-io
test_io_SOURCES = test_io.cpp mgl_io.cpp
if USE_LTDL
test_io_CXXFLAGS = -DWITH_LTDL $(AM_CXXFLAGS)
test_io_LDADD = @LIBLTDL@
mgllib_LTLIBRARIES = test-mod.la
mgllibdir = $(libdir)/mgl
test_mod_la_LDFLAGS = -no-undefined -module -avoid-version -shared
test_mod_la_SOURCES = test_mod.cpp
endif
endif
