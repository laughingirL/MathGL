INCLUDES        = -I$(top_builddir)/include -I$(top_builddir)

lib_LTLIBRARIES = 
if USE_FLTK
lib_LTLIBRARIES += libmgl-fltk.la
libmgl_fltk_la_LDFLAGS = -no-undefined -version-info $(MGL_CURRENT):$(MGL_REVISION):$(MGL_AGE)
libmgl_fltk_la_LIBADD = $(FLTK_LIBS) $(top_builddir)/mgl/libmgl.la
libmgl_fltk_la_CXXFLAGS = $(FLTK_FLAGS) $(AM_CXXFLAGS)
libmgl_fltk_la_SOURCES = mgl_fltk.cpp
endif

if USE_GLUT
lib_LTLIBRARIES += libmgl-glut.la
libmgl_glut_la_LDFLAGS = -no-undefined -version-info $(MGL_CURRENT):$(MGL_REVISION):$(MGL_AGE)
libmgl_glut_la_LIBADD = $(GLUT_LIBS) $(GL_LIBS) $(top_builddir)/mgl/libmgl.la
libmgl_glut_la_CXXFLAGS = $(GLUT_FLAGS) $(AM_CXXFLAGS)
libmgl_glut_la_SOURCES = mgl_glut.cpp
endif

if USE_WX
lib_LTLIBRARIES += libmgl-wx.la
libmgl_wx_la_LDFLAGS = -no-undefined -version-info $(MGL_CURRENT):$(MGL_REVISION):$(MGL_AGE)
libmgl_wx_la_LIBADD = $(WX_LIBS) $(top_builddir)/mgl/libmgl.la
libmgl_wx_la_CXXFLAGS = $(WX_FLAGS) $(AM_CXXFLAGS)
libmgl_wx_la_SOURCES = mgl_wx.cpp
endif

if USE_QT
include $(top_srcdir)/config/autotroll.mk
ACLOCAL_AMFLAGS = -I $(top_srcdir)/config/

lib_LTLIBRARIES += libmgl-qt.la
libmgl_qt_la_LDFLAGS = $(QT_LDFLAGS) $(LDFLAGS) -no-undefined -version-info $(MGL_CURRENT):$(MGL_REVISION):$(MGL_AGE)
libmgl_qt_la_LIBADD = $(QT_LIBS) $(top_builddir)/mgl/libmgl.la
libmgl_qt_la_CPPFLAGS = $(QT_CPPFLAGS) $(AM_CPPFLAGS)
libmgl_qt_la_CXXFLAGS = $(QT_CXXFLAGS) $(AM_CXXFLAGS)
libmgl_qt_la_SOURCES = $(top_builddir)/include/mgl/mgl_qt.moc.cpp mgl_qt.cpp $(top_builddir)/include/mgl/mgl_qt.h

BUILT_SOURCES = $(top_builddir)/include/mgl/mgl_qt.moc.cpp
CLEANFILES = $(BUILT_SOURCES)
endif
