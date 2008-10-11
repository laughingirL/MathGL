INCLUDES        = -I$(top_builddir)/include -I$(top_builddir)
INCLUDES        += @INCLTDL@

lib_LTLIBRARIES = libmgl.la

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

libmgl_la_LDFLAGS = -no-undefined -version-info $(MGL_CURRENT):$(MGL_REVISION):$(MGL_AGE)
libmgl_la_LIBADD = @LIBLTDL@ $(PNG_LIBS) $(GSL_LIBS) $(GL_LIBS)

libmgl_la_CXXFLAGS = $(WIDGETS) -DMGL_FONT_PATH=\"$(datadir)/$(PACKAGE)/fonts/\" \
    -DMOD_LIB_DIR=\"$(libdir)/mgl/\" $(HDF5_FLAGS) $(JPEG_FLAGS) $(TIFF_FLAGS) \
    $(PNG_FLAGS) $(FLTK_FLAGS) $(GSL_FLAGS) $(ADDON_FLAGS) $(AM_CXXFLAGS)

libmgl_la_SOURCES = \
mgl_data_cf.cpp	   mgl_axis.cpp     mgl_data_png.cpp  mgl_zb2.cpp \
mgl_c.cpp          mgl_eps.cpp      mgl_font.cpp      mgl_zb.cpp \
mgl_combi.cpp      mgl_eval.cpp     mgl_gl.cpp        mgl_def_font.cpp\
mgl_1d.cpp         mgl_cont.cpp     mgl_exec.cpp      mgl_tex_table.cpp\
mgl_2d.cpp         mgl_crust.cpp    mgl_export.cpp    mgl_main.cpp  \
mgl_3d.cpp         mgl_data.cpp     mgl_f.cpp         mgl_idtf.cpp \
mgl_ab.cpp         mgl_data_io.cpp  mgl_flow.cpp      mgl_vect.cpp \
mgl_parse.cpp      mgl_fit.cpp

if USE_JPEG
mgl_jpeg_la_LDFLAGS = -no-undefined -module -avoid-version -shared
mgl_jpeg_la_LIBADD = $(JPEG_LIBS)
mgl_jpeg_la_CXXFLAGS = $(JPEG_FLAGS) $(AM_CXXFLAGS)
mgl_jpeg_la_SOURCES = mgl_zb_jpg.cpp
endif

if USE_HDF5
mgl_hdf5_la_LDFLAGS = -no-undefined -module -avoid-version -shared
mgl_hdf5_la_LIBADD = $(HDF5_LIBS)
mgl_hdf5_la_CXXFLAGS = $(HDF5_FLAGS) $(AM_CXXFLAGS)
mgl_hdf5_la_SOURCES = data_hdf.cpp
endif

if USE_TIFF
mgl_tiff_la_LDFLAGS = -no-undefined -module -avoid-version -shared
mgl_tiff_la_LIBADD = $(TIFF_LIBS)
mgl_tiff_la_CXXFLAGS = $(TIFF_FLAGS) $(AM_CXXFLAGS)
mgl_tiff_la_SOURCES = mgl_zb_tiff.cpp
endif
