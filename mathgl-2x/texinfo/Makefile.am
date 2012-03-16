SUBDIRS				= png .

CLEANFILES = mathgl_en.info mathgl_en.info-1 mathgl_en.info-2 mathgl_en.info-3 mgl_en.info \
mathgl_ru.info mathgl_ru.info-1 mathgl_ru.info-2 mathgl_ru.info-3 mathgl_ru.info-4 mgl_ru.info

MAKEINFOHTML		= texi2html
TEXI2PDF			= texi2pdf

#AM_MAKEINFOHTMLFLAGS	= --split=chapter
AM_MAKEINFOFLAGS	= --no-validate

info_TEXINFOS		= mathgl_en.texi
mathgl_en_TEXINFOS	= \
overview_en.texi example_en.texi core_en.texi class_en.texi widget_en.texi \
data_en.texi other_en.texi mgl_cmd_en.texi samples_en.texi appendix_en.texi fdl.texi


info_TEXINFOS		+= mathgl_ru.texi
mathgl_ru_TEXINFOS	= \
overview_ru.texi example_ru.texi core_ru.texi class_ru.texi widget_ru.texi \
data_ru.texi other_ru.texi mgl_cmd_ru.texi samples_ru.texi appendix_ru.texi fdl.texi

info_TEXINFOS		+= mgl_en.texi
mgl_en_TEXINFOS	= \
mgl_cmd_en.texi samples_en.texi ex_mgl_en.texi concept_en.texi fdl.texi

info_TEXINFOS		+= mgl_ru.texi
mgl_ru_TEXINFOS	= \
mgl_cmd_ru.texi samples_ru.texi ex_mgl_ru.texi concept_ru.texi fdl.texi

#mglpng=$(DESTDIR)$(docdir)/png_static

#install-data-local: install-html install-pdf
install-data-local: 
#	mkdir -p $(mglpng)
#	cp -r $(top_srcdir)/texinfo/png_static/*.png $(mglpng)
uninstall-local:
#	rm -rf $(mglpng)

#all-local: html pdf
all-local: html
	./filter.py
