AM_CXXFLAGS += -I$(top_srcdir)/include -I$(top_builddir)/include

check_PROGRAMS =
bin_PROGRAMS = mgl_example
mgl_example_SOURCES = wnd_samples.cpp full_test.cpp
mgl_example_LDADD = $(top_builddir)/mgl/libmgl.la

if USE_FLTK
bin_PROGRAMS += mgl_fltk_example
mgl_fltk_example_SOURCES = wnd_samples.cpp fltk_example.cpp
mgl_fltk_example_LDADD = $(FLTK_LIBS) $(top_builddir)/widgets/libmgl-fltk.la $(top_builddir)/mgl/libmgl.la
mgl_fltk_example_CXXFLAGS = $(FLTK_FLAGS) $(AM_CXXFLAGS)

check_PROGRAMS += test
test_SOURCES = wnd_samples.cpp main.cpp
test_LDADD = $(FLTK_LIBS) $(top_builddir)/widgets/libmgl-fltk.la $(top_builddir)/mgl/libmgl.la
test_CXXFLAGS = $(FLTK_FLAGS) $(AM_CXXFLAGS)
endif

if USE_GLUT
bin_PROGRAMS += mgl_glut_example
mgl_glut_example_SOURCES = wnd_samples.cpp glut_example.cpp
mgl_glut_example_LDADD = $(top_builddir)/widgets/libmgl-glut.la $(top_builddir)/mgl/libmgl.la
endif

if USE_WX
bin_PROGRAMS += mgl_wx_example
mgl_wx_example_LDADD = $(WX_LIBS) $(top_builddir)/widgets/libmgl-wx.la $(top_builddir)/mgl/libmgl.la
mgl_wx_example_CXXFLAGS = $(WX_FLAGS) $(AM_CXXFLAGS)
mgl_wx_example_SOURCES = wnd_samples.cpp wx_example.cpp
endif

if USE_QT
include $(top_srcdir)/config/autotroll.mk
ACLOCAL_AMFLAGS = -I $(top_srcdir)/config/

bin_PROGRAMS += mgl_qt_example
mgl_qt_example_SOURCES = wnd_samples.cpp $(top_builddir)/include/mgl/mgl_qt.moc.cpp $(top_srcdir)/include/mgl/mgl_qt.h qt_example.cpp
mgl_qt_example_LDADD = $(QT_LIBS) $(top_builddir)/widgets/libmgl-qt.la $(top_builddir)/mgl/libmgl.la
mgl_qt_example_LDFLAGS = $(QT_LDFLAGS)
mgl_qt_example_CPPFLAGS = $(QT_CPPFLAGS) $(AM_CPPFLAGS)
mgl_qt_example_CXXFLAGS = $(QT_CXXFLAGS) $(AM_CXXFLAGS)

BUILT_SOURCES = $(top_builddir)/include/mgl/mgl_qt.moc.cpp
CLEANFILES = $(BUILT_SOURCES)
endif
