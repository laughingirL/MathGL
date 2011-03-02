pngdir = $(docdir)/png

images = ${shell $(top_builddir)/examples/mgl_example -list}
png_images = ${images:=.png}
png_DATA = $(png_images)
CLEANFILES = $(png_images)

$(png_images): hotdogs.pts # $(top_builddir)/examples/mgl_example
	$(top_builddir)/examples/mgl_example -kind=${@:.png=}
	cp ${@} ../../png/
	touch $(srcdir)/../mathgl.texi
