AM_CXXFLAGS += -I$(top_srcdir)/include -I$(top_builddir)/include

lib_LTLIBRARIES = 
if USE_FLTK
lib_LTLIBRARIES += libmgl-fltk.la
libmgl_fltk_la_LDFLAGS = -no-undefined -version-info $(MGL_CURRENT):$(MGL_REVISION):$(MGL_AGE)
libmgl_fltk_la_LIBADD = $(FLTK_LIBS) $(top_builddir)/src/libmgl.la
libmgl_fltk_la_CXXFLAGS = $(FLTK_FLAGS) $(AM_CXXFLAGS)
libmgl_fltk_la_SOURCES = fltk.cpp window.cpp
endif

if USE_QT
include $(top_srcdir)/config/autotroll.mk
ACLOCAL_AMFLAGS = -I $(top_srcdir)/config/

lib_LTLIBRARIES += libmgl-qt.la
libmgl_qt_la_LDFLAGS = $(QT_LDFLAGS) $(LDFLAGS) -no-undefined -version-info $(MGL_CURRENT):$(MGL_REVISION):$(MGL_AGE)
libmgl_qt_la_LIBADD = $(QT_LIBS) $(top_builddir)/src/libmgl.la
libmgl_qt_la_CPPFLAGS = $(QT_CPPFLAGS) $(AM_CPPFLAGS)
libmgl_qt_la_CXXFLAGS = $(QT_CXXFLAGS) $(AM_CXXFLAGS)
libmgl_qt_la_SOURCES = $(top_builddir)/include/mgl/mgl_qt.moc.cpp qt.cpp $(top_srcdir)/include/mgl/mgl_qt.h window.cpp

BUILT_SOURCES = $(top_builddir)/include/mgl/mgl_qt.moc.cpp
CLEANFILES = $(BUILT_SOURCES)
endif
