ACLOCAL_AMFLAGS = -I config
SUBDIRS = libltdl mgl lang widgets utils examples include

mgldest=$(DESTDIR)$(datadir)/$(PACKAGE)/fonts

install-data-local:
	mkdir -p $(mgldest)
	cp -r $(top_builddir)/fonts/*.vfm $(mgldest)

uninstall-local:
	rm -rf $(mgldest)
