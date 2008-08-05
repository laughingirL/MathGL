INCLUDES        = -I$(top_builddir)/include -I$(top_builddir)
INCLUDES        += @INCLTDL@

CLEANFILES =
BUILT_SOURCES =

if USE_PYTHON
python_PYTHON = mathgl.py
pyexec_LTLIBRARIES = _mathgl.la
_mathgl_la_SOURCES = mgl_python.cpp
_mathgl_la_CXXFLAGS = $(PYTHON_HEADERS) $(AM_CXXFLAGS)
_mathgl_la_LIBADD = $(top_builddir)/mgl/libmgl.la
_mathgl_la_LDFLAGS = -avoid-version -module

CLEANFILES += mgl_python.cpp mathgl.py
BUILT_SOURCES += mathgl.py mgl_python.cpp

mgl_python.cpp: mgl.i mgl_base.i  mgl_data.i  mgl_graph.i $(top_builddir)/include/mgl/mgl_c.h
	swig -python -classic -o mgl_python.cpp mgl.i 
mathgl.py: mgl.i mgl_base.i  mgl_data.i  mgl_graph.i $(top_builddir)/include/mgl/mgl_c.h
	swig -python -classic -o mgl_python.cpp mgl.i
endif

