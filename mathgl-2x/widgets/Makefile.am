AM_CXXFLAGS += -I$(top_srcdir)/include -I$(top_builddir)/include

WIDGETS =
if USE_QT
WIDGETS += -DHAVE_QT
endif
if USE_FLTK
WIDGETS += -DHAVE_FLTK
endif

lib_LTLIBRARIES = libmgl-wnd.la
libmgl_wnd_la_LDFLAGS = $(LDFLAGS) -no-undefined -version-info $(MGL_CURRENT):$(MGL_REVISION):$(MGL_AGE)
libmgl_wnd_la_LIBADD = $(top_builddir)/src/libmgl.la
libmgl_wnd_la_CPPFLAGS = $(AM_CXXFLAGS) $(WIDGETS)
libmgl_wnd_la_CXXFLAGS = $(AM_CXXFLAGS) $(WIDGETS)
#libmgl_wnd_la_SOURCES = window.cpp
libmgl_wnd_la_SOURCES = 


if USE_FLTK
libmgl_wnd_la_LIBADD += $(FLTK_LIBS)
libmgl_wnd_la_CXXFLAGS += $(FLTK_FLAGS)
libmgl_wnd_la_SOURCES += fltk.cpp
endif

if USE_QT
include $(top_srcdir)/config/autotroll.mk
ACLOCAL_AMFLAGS = -I $(top_srcdir)/config/

libmgl_wnd_la_LDFLAGS += $(QT_LDFLAGS)
libmgl_wnd_la_LIBADD += $(QT_LIBS)
libmgl_wnd_la_CPPFLAGS += $(QT_CPPFLAGS)
libmgl_wnd_la_CXXFLAGS += $(QT_CXXFLAGS)
libmgl_wnd_la_SOURCES += $(top_builddir)/include/mgl/qt.moc.cpp qt.cpp $(top_srcdir)/include/mgl/qt.h

BUILT_SOURCES = $(top_builddir)/include/mgl/qt.moc.cpp
CLEANFILES = $(BUILT_SOURCES)
endif
