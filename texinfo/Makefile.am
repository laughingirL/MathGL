SUBDIRS			= png .

CLEANFILES = mathgl_en.info mathgl_en.info-1 mathgl_en.info-2 mathgl_en.info-3 \
mathgl_ru.info mathgl_ru.info-1 mathgl_ru.info-2 mathgl_ru.info-3

MAKEINFOHTML		= texi2html
TEXI2PDF		= texi2pdf

AM_MAKEINFOHTMLFLAGS	= --split=chapter
AM_MAKEINFOFLAGS	= --no-validate

info_TEXINFOS           = mathgl_en.texi
mathgl_en_TEXINFOS		= \
example_en.texi  gpl-3.0.texi  overview_en.texi  samples_en.texi \
class_en.texi  fdl.texi      data_en.texi \
core_en.texi   gpl-2.0.texi  mgl_en.texi

info_TEXINFOS           += mathgl_ru.texi
mathgl_ru_TEXINFOS	= \
samples_ru.texi class_ru.texi overview_ru.texi appendix_ru.texi \
data_ru.texi mgl_ru.texi core_ru.texi example_ru.texi fdl.texi \
gpl-2.0.texi gpl-3.0.texi

mglpng=$(DESTDIR)$(docdir)/png_static

#install-data-local: install-html install-pdf
install-data-local: install-html
	mkdir -p $(mglpng)
	cp -r $(top_builddir)/texinfo/png_static/*.png $(mglpng)

uninstall-local:
	rm -rf $(mglpng)

#all-local: html pdf
all-local: html
