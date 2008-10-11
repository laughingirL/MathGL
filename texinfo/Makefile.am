SUBDIRS			= png .

MAKEINFOHTML		= texi2html
TEXI2PDF		= texi2pdf

AM_MAKEINFOHTMLFLAGS	= --split=chapter
AM_MAKEINFOFLAGS	= --no-validate

info_TEXINFOS           = mathgl.texi
mathgl_TEXINFOS		= \
cint.texi   example.texi  gpl-3.0.texi  overview.texi  samples.texi \
class.texi  fdl.texi      data.texi \
core.texi   gpl-2.0.texi  mgl.texi      python.texi

mglpng=$(DESTDIR)$(docdir)/png_static

install-data-local: install-html install-pdf
	mkdir -p $(mglpng)
	cp -r $(top_builddir)/texinfo/png_static/*.png $(mglpng)

uninstall-local:
	rm -rf $(mglpng)

all-local: html pdf
