AM_CXXFLAGS += -I$(top_srcdir)/include -I$(top_builddir)/include

lib_LTLIBRARIES = libmgl.la


libmgl_la_LIBADD = $(PNG_LIBS) $(GL_LIBS)
libmgl_la_LDFLAGS = -no-undefined -version-info $(MGL_CURRENT):$(MGL_REVISION):$(MGL_AGE)

libmgl_la_CXXFLAGS = \
    -DMOD_LIB_DIR=\"$(libdir)/mgl/\" $(HDF5_FLAGS) $(HDF4_FLAGS) $(JPEG_FLAGS) \
    $(PNG_FLAGS) $(FLTK_FLAGS) $(ADDON_FLAGS) $(AM_CXXFLAGS) $(PTHREAD_FLAGS)

if USE_PTHREAD
libmgl_la_LIBADD += $(PTHREAD_LIBS)
endif

if USE_GSL
libmgl_la_LIBADD += $(GSL_LIBS)
endif

if USE_HDF4
libmgl_la_LIBADD += $(HDF4_LIBS)
endif

if USE_HDF5
libmgl_la_LIBADD += $(HDF5_LIBS)
endif

if USE_JPEG
libmgl_la_LIBADD += $(JPEG_LIBS)
endif

if USE_U3D
libmgl_la_LIBADD += $(U3D_LIBS)
endif

libmgl_la_SOURCES = \
addon.cpp axis.cpp base_cf.cpp base.cpp canvas_cf.cpp canvas.cpp cont.cpp crust.cpp \
data.cpp data_io.cpp data_new.cpp data_op.cpp data_png.cpp def_font.cpp export_2d.cpp \
export_3d.cpp eval.cpp evalp.cpp exec.cpp export.cpp fit.cpp font.cpp other.cpp evalc.cpp \
parser.cpp pde.cpp pixel.cpp plot.cpp prim.cpp surf.cpp tex_table.cpp vect.cpp volume.cpp \
opengl.cpp

#if USE_OPENGL
#libmgl_la_SOURCES += opengl.cpp
#libmgl_la_LIBADD +=
#endif