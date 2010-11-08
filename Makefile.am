ACLOCAL_AMFLAGS = -I config
SUBDIRS = mgl lang widgets utils examples include

doctargets =

if USE_DOCS
SUBDIRS += texinfo
#doctargets += install-html install-pdf
doctargets += install-html
endif

mgldest=$(DESTDIR)$(datadir)/$(PACKAGE)/fonts

install-data-local: $(doctargets)
	mkdir -p $(mgldest)
	cp -r $(top_srcdir)/fonts/*.vfm $(mgldest)

uninstall-local:
	rm -rf $(mgldest)

clean-local:
	find . -name '*~' -print0 | xargs -0 rm -f

#all-local: html pdf
all-local: html
