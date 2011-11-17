INCLUDES        = -I$(top_builddir)/src -I$(top_builddir)/

bin_PROGRAMS = udav

udav_SOURCES = \
animate.cpp  editor.cpp    grid.cpp  mathgl.cpp  setup.cpp \
data.cpp     main.cpp  option.cpp  table.cpp help.cpp

udav_LDADD = $(MGL_LIBS) $(FLTK_LIBS)
udav_CPPFLAGS = -O2 -DDOC_DIR=\"$(docdir)/\"
