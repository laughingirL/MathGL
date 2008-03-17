SUBDIRS = libltdl mgl utils include examples

mgldest=$(DESTDIR)$(datadir)/$(PACKAGE)/fonts

install-data-local:
	mkdir -p $(mgldest)
	cp -r $(top_builddir)/fonts/*.vfm $(mgldest)

uninstall-local:
	rm -rf $(mgldest)

#install-data-local:
#        mkdir -p $(DESTDIR)$(includedir)/mgl
#        cp -r $(top_builddir)/include/mgl/* $(DESTDIR)$(includedir)/mgl
#doc_DATA = $(top_builddir)/help/

#uninstall-local:
#        rm -rf $(DESTDIR)$(includedir)/mgl


#doc_DATA = $(top_builddir)/docs/mathgl-1.3-eng.pdf \
#$(top_builddir)/docs/mathgl-1.3-rus.pdf $(top_builddir)/docs/mgl_scripts-1.0-eng.pdf
