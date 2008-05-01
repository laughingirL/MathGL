if USE_FLTK
FLTK_INC = mgl/mgl_fltk.h
endif
if USE_GLUT
GLUT_INC = mgl/mgl_glut.h
endif

nobase_include_HEADERS = $(FLTK_INC) $(GLUT_INC) \
mgl/mgl_ab.h  mgl/mgl_data.h  mgl/mgl_eval.h  mgl/mgl_zb.h    mgl/mgl_font.h\
mgl/mgl_c.h   mgl/mgl_eps.h   mgl/mgl_f.h     mgl/mgl_gl.h    mgl/mgl.h \
mgl/mgl_parse.h

mgldest=$(DESTDIR)$(datadir)/$(PACKAGE)/bigforth

install-data-local:
	mkdir -p $(mgldest)
	cp -r $(top_builddir)/include/bigforth/*.fs $(mgldest)

uninstall-local:
	rm -rf $(mgldest)
