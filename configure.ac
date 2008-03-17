AC_INIT(mgl/mgl_main.cpp)
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_SRCDIR(mgl)
AC_CONFIG_SUBDIRS(libltdl)

#MGL_RELEASE=1.4
#AC_SUBST(MGL_RELEASE)

# LT Version numbers, remember to change them just *before* a release.
#   (Interfaces removed:    CURRENT++, AGE=0, REVISION=0)
#   (Interfaces added:      CURRENT++, AGE++, REVISION=0)
#   (No interfaces changed:                   REVISION++)
MGL_CURRENT=4
MGL_REVISION=0
MGL_AGE=0
AC_SUBST(MGL_CURRENT)
AC_SUBST(MGL_REVISION)
AC_SUBST(MGL_AGE)

AM_INIT_AUTOMAKE(mathgl,MGL_RELEASE)

AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_LIBTOOL_DLOPEN
AC_LIBLTDL_CONVENIENCE
AC_PROG_LIBTOOL
AC_SUBST(INCLTDL)
AC_SUBST(LIBLTDL)

AC_CHECK_HEADERS([math.h string.h sys/types.h stdarg.h time.h float.h stdio.h \
stdlib.h ctype.h dlfcn.h],,exit)

AC_CHECK_HEADERS([gsl/gsl_fft_complex.h gsl/gsl_fft_real.h gsl/gsl_sf.h],,\
[echo "Please install gsl headers"; exit])
AC_CHECK_LIB([gsl], [main], [GSL_LIBS="-lgsl -lgslcblas" AC_SUBST(GSL_LIBS)], 
	[echo "Please install gsl library (libgsl, libgslcblas)"; exit],\
	[-lgslcblas])

case "${host}" in
  *darwin*)
       GL_LIBS="-framework OpenGL"
       GLUT_LIBS="-framework GLUT"
       ;;

       *)
	AC_CHECK_HEADERS([GL/gl.h],,[echo "Please install OpenGL headers (GL/gl.h)"; exit])
	AC_CHECK_LIB([GL], [main], [GL_LIBS=-lGL AC_SUBST(GL_LIBS)], 
	    [echo "Please install OpenGL library (libGL.so)"; exit])
       ;;

esac

AC_ARG_ENABLE(all,
[  --enable-all    Turn on all features],
[case "${enableval}" in
  yes) all=true ;;
  no)  all=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-all) ;;
esac],[all=false])

AC_ARG_ENABLE(glut,
[  --enable-glut    Turn on glut],
[case "${enableval}" in
  yes) glut=true ;;
  no)  glut=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-glut) ;;
esac],[glut=false])
case "${host}" in
  *darwin*)
       GLUT_LIBS="-framework GLUT"
       ;;

       *)
	AC_CHECK_HEADERS([GL/glut.h],[(test x$all = xtrue || test x$glut = xtrue) && GLUT_FLAGS=-DHAVE_GLUT AC_SUBST(GLUT_FLAGS)],
	    [(test x$all = xtrue || test x$glut = xtrue) && echo "Please install FreeGLUT headers (GL/glut.h)"; exit])
	AC_CHECK_LIB([glut], [main], [(test x$all = xtrue || test x$glut = xtrue) && GLUT_LIBS=-lglut AC_SUBST(GLUT_LIBS)],
	    [(test x$all = xtrue || test x$glut = xtrue) && echo "Please install FreeGLUT library (libglut.so)"; exit])
       ;;

esac
AM_CONDITIONAL(USE_GLUT, (test x$all = xtrue || test x$glut = xtrue))

case "${host}" in
  *linux*)
       ADDON_FLAGS=-DNO_COLOR_ARRAY
       ;;

       *)
       ;;
esac

AC_ARG_ENABLE(hdf5,
[  --enable-hdf5    Turn on hdf5],
[case "${enableval}" in
  yes) hdf5=true ;;
  no)  hdf5=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-hdf5) ;;
esac],[hdf5=false])
AC_CHECK_HEADER(hdf5.h,[(test x$all = xtrue || test x$hdf5 = xtrue) && HDF5_FLAGS=-DHAVE_HDF5 AC_SUBST(HDF5_FLAGS)],
    [(test x$all = xtrue || test x$hdf5 = xtrue) && echo "Please install hdf5 headers" && exit])

AC_CHECK_LIB([hdf5], [main], [(test x$all = xtrue || test x$hdf5 = xtrue) && HDF5_LIBS=-lhdf5 AC_SUBST(HDF5_LIBS)], 
    [(test x$all = xtrue || test x$hdf5 = xtrue) && echo "Please install hdf5 library" && exit])
AM_CONDITIONAL(USE_HDF5, (test x$all = xtrue || test x$hdf5 = xtrue))

AC_CHECK_HEADER(png.h,,[echo "Please install png headers" && exit])
AC_CHECK_LIB([png], [main], [PNG_LIBS=-lpng AC_SUBST(PNG_LIBS)],
    [echo "Please install png library" && exit])

AC_ARG_ENABLE(jpeg,
[  --enable-jpeg    Turn on jpeg],
[case "${enableval}" in
  yes) jpeg=true ;;
  no)  jpeg=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-jpeg) ;;
esac],[jpeg=false])
AC_CHECK_HEADER(jpeglib.h,[(test x$all = xtrue || test x$jpeg = xtrue) && JPEG_FLAGS=-DHAVE_JPEG AC_SUBST(JPEG_FLAGS)],
    [(test x$all = xtrue || test x$jpeg = xtrue) && echo "Please install jpeg headers" && exit])

AC_CHECK_LIB([jpeg], [main], [(test x$all = xtrue || test x$jpeg = xtrue) && JPEG_LIBS=-ljpeg AC_SUBST(JPEG_LIBS)], 
    [(test x$all = xtrue || test x$jpeg = xtrue) && echo "Please install jpeg library" && exit])
AM_CONDITIONAL(USE_JPEG, (test x$all = xtrue || test x$jpeg = xtrue))

AC_ARG_ENABLE(tiff,
[  --enable-tiff    Turn on tiff],
[case "${enableval}" in
  yes) tiff=true ;;
  no)  tiff=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-tiff) ;;
esac],[tiff=false])
AC_CHECK_HEADER(tiffio.h,[(test x$all = xtrue || test x$tiff = xtrue) && TIFF_FLAGS=-DHAVE_TIFF AC_SUBST(TIFF_FLAGS)],
    [(test x$all = xtrue || test x$tiff = xtrue) && echo "Please install tiff headers" && exit])

AC_CHECK_LIB([tiff], [main], [(test x$all = xtrue || test x$tiff = xtrue) && TIFF_LIBS=-ltiff AC_SUBST(TIFF_LIBS)], 
    [(test x$all = xtrue || test x$tiff = xtrue) && echo "Please install tiff library" && exit])
AM_CONDITIONAL(USE_TIFF, (test x$all = xtrue || test x$tiff = xtrue))

AC_ARG_ENABLE(fltk,
[  --enable-fltk    Turn on fltk],
[case "${enableval}" in
  yes) fltk=true ;;
  no)  fltk=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-fltk) ;;
esac],[fltk=false])
AC_CHECK_HEADER(FL/Fl_Export.H,[(test x$all = xtrue || test x$fltk = xtrue) && FLTK_FLAGS=-DHAVE_FLTK AC_SUBST(FLTK_FLAGS)],
    [(test x$all = xtrue || test x$fltk = xtrue) && echo "Please install fltk headers" && exit])

AC_CHECK_LIB([fltk], [main], [(test x$all = xtrue || test x$fltk = xtrue) && FLTK_LIBS="-lfltk -lfltk_images" AC_SUBST(FLTK_LIBS)],
    [(test x$all = xtrue || test x$fltk = xtrue) && echo "Please install fltk library" && exit])
AM_CONDITIONAL(USE_FLTK, (test x$all = xtrue || test x$fltk = xtrue))


AC_OUTPUT(Makefile mgl/Makefile utils/Makefile include/Makefile examples/Makefile)
