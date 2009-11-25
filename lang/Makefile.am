INCLUDES        = -I$(top_builddir)/include -I$(top_builddir)

CLEANFILES =
BUILT_SOURCES =

if USE_PYTHON
python_PYTHON = mathgl.py
pyexec_LTLIBRARIES = _mathgl.la
_mathgl_la_SOURCES = mgl_python.cpp
_mathgl_la_CXXFLAGS = $(PYTHON_HEADERS) $(AM_CXXFLAGS)
_mathgl_la_LIBADD = $(top_builddir)/mgl/libmgl.la
#_mathgl_la_LIBADD = $(top_builddir)/widgets/libmgl-qt.la
_mathgl_la_LDFLAGS = -avoid-version -module

CLEANFILES += mgl_python.cpp mathgl.py
BUILT_SOURCES += mathgl.py mgl_python.cpp

mgl_python.cpp: mgl.i $(top_builddir)/include/mgl/mgl_c.h $(top_builddir)/include/mgl/mgl_data.h
	swig -python -c++ -I$(top_builddir)/include -o mgl_python.cpp mgl.i
mathgl.py: mgl.i $(top_builddir)/include/mgl/mgl_c.h $(top_builddir)/include/mgl/mgl_data.h
	swig -python -c++ -I$(top_builddir)/include -o mgl_python.cpp mgl.i
endif


if USE_OCTAVE

CLEANFILES += mgl_octave.cpp mathgl.oct mathgl.tar.gz

PKG_NAME = mathgl
PKG_OCT_FILES = mathgl.oct
PKG_OTHER_FILES = $(top_builddir)/COPYING $(srcdir)/DESCRIPTION $(srcdir)/INDEX $(srcdir)/PKG_ADD_template
octavedir=$(pkgdatadir)/octave
dist_octave_DATA = mathgl.tar.gz
mathgl.tar.gz: $(PKG_OCT_FILES) $(PKG_OTHER_FILES)
	mkdir -p $(PKG_NAME)
	mkdir -p $(PKG_NAME)/inst/$(OCTAVE_ARCH)
	cp -rf $(PKG_OTHER_FILES) $(PKG_NAME)
	mv $(PKG_NAME)/PKG_ADD_template $(PKG_NAME)/PKG_ADD
	cp -rf $(PKG_OCT_FILES) $(PKG_NAME)/inst/$(OCTAVE_ARCH)
	tar cpzvf $@ $(PKG_NAME)
	rm -rf $(PKG_NAME)

MY_MKOCTFILE=$(CXX) -O0 -g -shared $(OCTAVE_INCFLAGS)

mgl_octave.cpp: mgl.i $(top_builddir)/include/mgl/mgl_c.h $(top_builddir)/include/mgl/mgl_data.h
	swig -octave -c++ -I$(top_builddir)/include -o mgl_octave.cpp mgl.i
mathgl.oct: mgl_octave.cpp
	$(MY_MKOCTFILE) -o $@ $^ -fPIC -I../include -L../mgl/.libs/ -lmgl
endif

