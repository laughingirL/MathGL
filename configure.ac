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
MGL_REVISION=1
MGL_AGE=2
AC_SUBST(MGL_CURRENT)
AC_SUBST(MGL_REVISION)
AC_SUBST(MGL_AGE)

AM_INIT_AUTOMAKE(mathgl,MGL_RELEASE)

AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL

AC_LIBTOOL_WIN32_DLL
AC_LIBTOOL_DLOPEN
AC_LIBLTDL_INSTALLABLE
#AC_LIBLTDL_CONVENIENCE
AC_PROG_LIBTOOL

AC_SUBST(INCLTDL)
AC_SUBST(LIBLTDL)

AM_CXXFLAGS="-Wall"
AC_SUBST(AM_CXXFLAGS)

GSL_PROG=gsl-config
AC_CHECK_PROG(GSL_FLAGS,$GSL_PROG,`$GSL_PROG --cflags`)
AC_CHECK_PROG(GSL_LIBS,$GSL_PROG,`$GSL_PROG --libs`)
if !(test "$GSL_FLAGS" && test "$GSL_LIBS") ;then
    echo "Please install gsl headers and libraries and make sure that \
	path to $GSL_PROG exist in your PATH"
    exit
fi

AC_ARG_ENABLE(all,
[  --enable-all    Turn on all features],
[case "${enableval}" in
  yes) all=true ;;
  no)  all=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-all) ;;
esac],[all=false])

AC_ARG_ENABLE(langall,
[  --enable-langall    Turn on all language interfaces],
[case "${enableval}" in
  yes) langall=true ;;
  no)  langall=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-langall) ;;
esac],[langall=false])

AC_ARG_ENABLE(glut,
[  --enable-glut    Turn on glut],
[case "${enableval}" in
  yes) glut=true ;;
  no)  glut=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-glut) ;;
esac],[glut=false])

case "${host}" in
  *darwin*)
	GL_LIBS="-framework OpenGL"
	AC_SUBST(GL_LIBS)
	if (test x$all = xtrue || test x$glut = xtrue) ;then
	    GLUT_LIBS="-framework GLUT -framework OpenGL"
	    AC_SUBST(GLUT_LIBS)
	fi
       ;;

  *w32*)
	GL_LIBS=opengl32
	AC_CHECK_HEADER([GL/gl.h],,[echo "Please install OpenGL headers (GL/gl.h)"; exit])
	AC_CHECK_LIB([${GL_LIBS}], [main], [GL_LIBS=-l${GL_LIBS} AC_SUBST(GL_LIBS)], 
	    [echo "Please install OpenGL library (lib${GL_LIBS}.a)"; exit])
	GLUT_LIBS=glut32
	AC_CHECK_HEADER([GL/glut.h],[(test x$all = xtrue || test x$glut = xtrue) && GLUT_FLAGS=-DHAVE_GLUT AC_SUBST(GLUT_FLAGS)],
	    [(test x$all = xtrue || test x$glut = xtrue) && (echo "Please install FreeGLUT headers (GL/glut.h)"; exit)])
	AC_CHECK_LIB([${GLUT_LIBS}], [main], [(test x$all = xtrue || test x$glut = xtrue) && GLUT_LIBS=-l${GLUT_LIBS} AC_SUBST(GLUT_LIBS)],
	    [(test x$all = xtrue || test x$glut = xtrue) && echo "Please install FreeGLUT library (lib${GLUT_LIBS}.a)"; exit])
       ;;

       *)
	GL_LIBS=GL
	AC_CHECK_HEADER(GL/gl.h,,[echo "Please install OpenGL headers (GL/gl.h)"; exit])
	AC_CHECK_LIB([${GL_LIBS}], [main], [GL_LIBS=-l${GL_LIBS} AC_SUBST(GL_LIBS)], 
	    [echo "Please install OpenGL library (lib${GL_LIBS}.a)"; exit])
	GLUT_LIBS=glut
	AC_CHECK_HEADER(GL/glut.h,[(test x$all = xtrue || test x$glut = xtrue) && GLUT_FLAGS=-DHAVE_GLUT AC_SUBST(GLUT_FLAGS)],
	    [(test x$all = xtrue || test x$glut = xtrue) && (echo "Please install FreeGLUT headers (GL/glut.h)"; exit)])
	AC_CHECK_LIB([${GLUT_LIBS}], [main], [(test x$all = xtrue || test x$glut = xtrue) && GLUT_LIBS=-l${GLUT_LIBS} AC_SUBST(GLUT_LIBS)],
	    [(test x$all = xtrue || test x$glut = xtrue) && echo "Please install FreeGLUT library (lib${GLUT_LIBS}.a)"; exit])
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
FLTK_PROG=fltk-config
AC_CHECK_PROG(FLTK_FLAGS,$FLTK_PROG,`$FLTK_PROG --cxxflags`)
AC_CHECK_PROG(FLTK_LIBS,$FLTK_PROG,`$FLTK_PROG --ldflags`)
if (test x$all = xtrue || test x$fltk = xtrue) ;then
    if test "$FLTK_FLAGS" && test "$FLTK_LIBS" ;then
	test_fltk=true
    else
	echo "Please install FLTK headers and libraries and make sure that \
	            path to $FLTK_PROG exist in your PATH"
	exit
    fi 
fi
AM_CONDITIONAL(USE_FLTK, test x$test_fltk = xtrue)

AC_ARG_ENABLE(wx,
[  --enable-wx    Turn on wxWidget],
[case "${enableval}" in
  yes) wx=true ;;
  no)  wx=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-wx) ;;
esac],[wx=false])
WX_PROG=wx-config
AC_CHECK_PROG(WX_FLAGS,$WX_PROG,`$WX_PROG --cflags`)
AC_CHECK_PROG(WX_LIBS,$WX_PROG,`$WX_PROG --libs`)
if (test x$all = xtrue || test x$wx = xtrue) ;then
    if test "$WX_FLAGS" && test "$WX_LIBS" ;then
	test_wx=true
    else
	echo "Please install wxwidget headers and libraries and make sure that \
	            path to $WX_PROG exist in your PATH"
	exit
    fi 
fi
AM_CONDITIONAL(USE_WX, test x$test_wx = xtrue)

AC_ARG_ENABLE(qt,
[  --enable-qt    Turn on Qt],
[case "${enableval}" in
  yes) qt=true ;;
  no)  qt=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-qt) ;;
esac],[qt=false])
if (test x$all = xtrue || test x$qt = xtrue) ;then
    AT_WITH_QT
    AT_REQUIRE_QT_VERSION(4.3)
    test_qt=true
fi
AM_CONDITIONAL(USE_QT, test x$test_qt = xtrue)

AC_ARG_ENABLE(python,
[  --enable-python    Turn on interface to python],
[case "${enableval}" in
  yes) python=true ;;
  no)  python=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-python) ;;
esac],[python=false])
AC_CHECK_PROG(PYTHON_HEADERS,python-config,`python-config --cflags`)
AC_CHECK_PROG(HAVE_SWIG,swig$EXEEXT,true)
if (test x$langall = xtrue || test x$python = xtrue) ;then
    if (test "$PYTHON_HEADERS" && test "$HAVE_SWIG") ;then
	test_python=true
	AM_PATH_PYTHON()
    else
	echo Please install python-dev and swig packages
	exit
    fi 
fi
AM_CONDITIONAL(USE_PYTHON, test x$test_python = xtrue )

AC_ARG_ENABLE(octave,
[  --enable-octave    Turn on interface to octave],
[case "${enableval}" in
  yes) octave=true ;;
  no)  octave=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-octave) ;;
esac],[octave=false])
if (test x$langall = xtrue || test x$octave = xtrue) ;then
# find Octave arch
    OCTAVE=octave
    AC_MSG_CHECKING([for Octave arch])
    OCTAVE_ARCH=`$OCTAVE -q --eval 'strcat(octave_config_info("canonical_host_type"), "-", octave_config_info("api_version"))'|sed -e 's/ans = //'`
    AC_MSG_RESULT([$OCTAVE_ARCH])
    AC_SUBST(OCTAVE_ARCH,[$OCTAVE_ARCH])
    test_octave=true
fi
AM_CONDITIONAL(USE_OCTAVE, test x$test_octave = xtrue )

AC_ARG_ENABLE(testio,
[  --enable-testio    Turn on testio],
[case "${enableval}" in
  yes) testio=true ;;
  no)  testio=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-testio) ;;
esac],[testio=false])
AM_CONDITIONAL(USE_TESTIO, test x$testio = xtrue )

AC_ARG_ENABLE(docs,
[  --enable-docs    Turn on documentation building],
[case "${enableval}" in
  yes) docs=true ;;
  no)  docs=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-docs) ;;
esac],[docs=false])
if (test x$docs = xtrue || test x$all = xtrue) ;then
	AC_CONFIG_FILES([texinfo/Makefile texinfo/png/Makefile])
fi
AM_CONDITIONAL(USE_DOCS, (test x$docs = xtrue || test x$all = xtrue) )

AM_CONDITIONAL(USE_LTDL, (test x$all = xtrue || test x$testio = xtrue || test x$hdf5 = xtrue || test x$jpeg = xtrue || test x$tiff = xtrue))

AC_CONFIG_FILES([
Makefile
mgl/Makefile
examples/Makefile
include/Makefile
lang/Makefile
utils/Makefile
widgets/Makefile
])
AC_OUTPUT
