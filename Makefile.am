ACLOCAL_AMFLAGS = -I config
SUBDIRS = libltdl mgl lang widgets utils examples include

if USE_DOCS
SUBDIRS += texinfo
endif

mgldest=$(DESTDIR)$(datadir)/$(PACKAGE)/fonts

install-data-local: install-html install-pdf
	mkdir -p $(mgldest)
	cp -r $(top_builddir)/fonts/*.vfm $(mgldest)

uninstall-local:
	rm -rf $(mgldest)

clean-local:
	find . -name '*~' -print0 | xargs -0 rm -f

all-local: html pdf
