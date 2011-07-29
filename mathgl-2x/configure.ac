AC_INIT([mathgl], [2.0], [mathgl.abalakin@gmail.com])

MGL_RELEASE=2.0
AC_SUBST(MGL_RELEASE)

AC_CONFIG_MACRO_DIR([config])
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_SRCDIR(src)
AC_CONFIG_HEADER(include/mgl/config.h)
AC_LANG([C++])

#MGL_RELEASE=1.4
#AC_SUBST(MGL_RELEASE)

# LT Version numbers, remember to change them just *before* a release.
#   (Interfaces removed:    CURRENT++, AGE=0, REVISION=0)
#   (Interfaces added:      CURRENT++, AGE++, REVISION=0)
#   (No interfaces changed:                   REVISION++)
MGL_CURRENT=6
MGL_REVISION=0
MGL_AGE=0
AC_SUBST(MGL_CURRENT)
AC_SUBST(MGL_REVISION)
AC_SUBST(MGL_AGE)

AM_INIT_AUTOMAKE

AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL

AC_LIBTOOL_WIN32_DLL
AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL

AC_ARG_ENABLE(double,
[  --enable-double    Turn on double precision in MathGL library],
[case "${enableval}" in
  yes) double=true ;;
  no)  double=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-double) ;;
esac],[double=false])

if (test x$double = xtrue) ;then
    AC_DEFINE([MGL_USE_DOUBLE],1,[This define enables double precision in MathGL])
else
    AC_DEFINE([MGL_USE_DOUBLE],0,[This define enables double precision in MathGL])
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

AC_ARG_ENABLE(pthread,
[  --enable-pthread    Turn on pthread support in MathGL library],
[case "${enableval}" in
  yes) pthread=true ;;
  no)  pthread=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-pthread) ;;
esac],[pthread=true])
AC_CHECK_HEADER(pthread.h,[(test x$all = xtrue || test x$pthread = xtrue) && PTHREAD_FLAGS=-DHAVE_PTHREAD AC_SUBST(PTHREAD_FLAGS)],
    [(test x$all = xtrue || test x$pthread = xtrue) && echo "Please install posix threads headers" && exit])

AC_CHECK_LIB([pthread], [main], [(test x$all = xtrue || test x$pthread = xtrue) && PTHREAD_LIBS=-lpthread AC_SUBST(PTHREAD_LIBS)], 
    [(test x$all = xtrue || test x$pthread = xtrue) && echo "Please install posix threads library" && exit])
AM_CONDITIONAL(USE_PTHREAD, test x$pthread = xtrue)

AC_ARG_ENABLE(gsl,
[  --enable-gsl    Turn on gsl functions],
[case "${enableval}" in
  yes) gsl=true ;;
  no)  gsl=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-gsl) ;;
esac],[gsl=true])

if (test x$all = xtrue || test x$gsl = xtrue) ;then
    GSL_PROG=gsl-config
    AC_CHECK_PROG(GSL_FLAGS,$GSL_PROG,`$GSL_PROG --cflags`)
    AC_CHECK_PROG(GSL_LIBS,$GSL_PROG,`$GSL_PROG --libs`)
    if test "$GSL_LIBS" ;then
	test_gsl=true
    else
	echo "Please install gsl headers and libraries and make sure that \
	path to $GSL_PROG exist in your PATH"
	exit
    fi
else
    GSL_FLAGS=-DNO_GSL
    AC_SUBST(GSL_FLAGS)
fi
AM_CONDITIONAL(USE_GSL, test x$test_gsl = xtrue)

case "${host}" in
  *darwin*)
	GL_LIBS="-framework OpenGL"
	AC_SUBST(GL_LIBS)
       ;;

  *w32*)
	GL_LIBS=opengl32
	AC_CHECK_HEADER([GL/gl.h],,[echo "Please install OpenGL headers (GL/gl.h)"; exit])
	AC_CHECK_LIB([${GL_LIBS}], [main], [GL_LIBS=-l${GL_LIBS} AC_SUBST(GL_LIBS)], 
	    [echo "Please install OpenGL library (lib${GL_LIBS}.a)"; exit])
       ;;

       *)
	GL_LIBS=GL
	AC_CHECK_HEADER(GL/gl.h,,[echo "Please install OpenGL headers (GL/gl.h)"; exit])
	AC_CHECK_LIB([${GL_LIBS}], [main], [GL_LIBS=-l${GL_LIBS} AC_SUBST(GL_LIBS)], 
	    [echo "Please install OpenGL library (lib${GL_LIBS}.a)"; exit])
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
AC_ARG_ENABLE(hdf5_18,
[  --enable-hdf5_18    Turn on hdf5 version 1.8],
[case "${enableval}" in
  yes) hdf5_18=true ;;
  no)  hdf5_18=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-hdf5_18) ;;
esac],[hdf5_18=false])
#AC_CHECK_HEADER(hdf5.h,[(test x$all = xtrue || test x$hdf5 = xtrue || test x$hdf5_18 = xtrue) && HDF5_FLAGS=-DHAVE_HDF5 AC_SUBST(HDF5_FLAGS)],
AC_CHECK_HEADER(hdf5.h,[(test x$all = xtrue || test x$hdf5 = xtrue || test x$hdf5_18 = xtrue) && HDF5_FLAGS=-DHAVE_HDF5 ],
    [(test x$all = xtrue || test x$hdf5 = xtrue || test x$hdf5_18 = xtrue) && echo "Please install hdf5 headers" && exit])
if ((test x$all = xtrue || test x$hdf5 = xtrue) && test x$hdf5_18 = xfalse) ;then
    HDF5_FLAGS="$HDF5_FLAGS -DH5_USE_16_API"
    AC_SUBST(HDF5_FLAGS)
fi
AC_CHECK_LIB([hdf5], [main], [(test x$all = xtrue || test x$hdf5 = xtrue || test x$hdf5_18 = xtrue) && HDF5_LIBS=-lhdf5 AC_SUBST(HDF5_LIBS)], 
    [(test x$all = xtrue || test x$hdf5 = xtrue || test x$hdf5_18 = xtrue) && echo "Please install hdf5 library" && exit])
AM_CONDITIONAL(USE_HDF5, (test x$all = xtrue || test x$hdf5 = xtrue || test x$hdf5_18 = xtrue))


AC_ARG_ENABLE(hdf4,
[  --enable-hdf4    Turn on hdf4],
[case "${enableval}" in
  yes) hdf4=true ;;
  no)  hdf4=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-hdf4) ;;
esac],[hdf4=false])
AC_CHECK_HEADER(hdf/mfhdf.h,[(test x$all = xtrue || test x$hdf4 = xtrue) && HDF4_FLAGS=-DHAVE_HDF4 ],
    [(test x$all = xtrue || test x$hdf4 = xtrue) && echo "Please install hdf4 headers" && exit])
    AC_SUBST(HDF4_FLAGS)
AC_CHECK_LIB([df], [main], [(test x$all = xtrue || test x$hdf4 = xtrue) && HDF4_LIBS="-lmfhdf -ldf" AC_SUBST(HDF4_LIBS)], 
    [(test x$all = xtrue || test x$hdf4 = xtrue) && echo "Please install hdf4 library" && exit])
AM_CONDITIONAL(USE_HDF4, (test x$all = xtrue || test x$hdf4 = xtrue))

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

AC_ARG_ENABLE(u3d,
[  --enable-u3d    Turn on u3d functions],
[case "${enableval}" in
  yes) u3d=true ;;
  no)  u3d=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-u3d) ;;
esac],[u3d=false])
AC_ARG_ENABLE(pdf,
[  --enable-pdf    Turn on u3d and pdf functions],
[case "${enableval}" in
  yes) pdf=true; u3d=true ;;
  no)  pdf=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-pdf) ;;
esac],[pdf=false])
AS_IF([test "x$pdf" == xtrue],
  [AC_CHECK_LIB([hpdf], [main],
   [],
   [AC_MSG_FAILURE(
   [--enable-pdf was given, but test for libharu failed])]
  )])
AS_IF([test "x$pdf" == xtrue],
  [AC_CHECK_HEADERS([hpdf.h], [],
  [AC_MSG_FAILURE(
  [--enable-pdf was given, but test for headers failed])]
  )])
AM_CONDITIONAL(USE_PDF, test x$pdf = xtrue)

AS_IF([test "x$u3d" == xtrue],
  [AC_CHECK_LIB([IDTF], [main],
  [AC_SUBST([U3D_LIBS], ["-lIDTF -lm -ldl"])
  AC_DEFINE([HAVE_U3D], [1],
  [Define if you have libIDTF])
  ],
  [AC_MSG_FAILURE(
  [--enable-u3d was given, but test for libIDTF failed])],
  [-lm -ldl])])
AS_IF([test "x$u3d" == xtrue],
  [AC_CHECK_HEADERS([u3d/SceneConverterLib.h], [],
  [AC_MSG_FAILURE(
  [--enable-u3d was given, but test for headers failed])]
  )])
AM_CONDITIONAL(USE_U3D, test x$u3d = xtrue)

AC_ARG_ENABLE(fltk,
[  --enable-fltk    Turn on fltk],
[case "${enableval}" in
  yes) fltk=true ;;
  no)  fltk=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-fltk) ;;
esac],[fltk=false])
if (test x$all = xtrue || test x$fltk = xtrue) ;then
    FLTK_PROG=fltk-config
    AC_CHECK_PROG(FLTK_FLAGS,$FLTK_PROG,`$FLTK_PROG --cxxflags`)
    AC_CHECK_PROG(FLTK_LIBS,$FLTK_PROG,`$FLTK_PROG --ldflags`)
    if test "$FLTK_FLAGS" && test "$FLTK_LIBS" ;then
	test_fltk=true
    else
	echo "Please install FLTK headers and libraries and make sure that \
	            path to $FLTK_PROG exist in your PATH"
	exit
    fi 
fi
AM_CONDITIONAL(USE_FLTK, test x$test_fltk = xtrue)

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
if (test x$langall = xtrue || test x$python = xtrue) ;then
    AC_CHECK_PROG(PYTHON_HEADERS,python-config,`python-config --cflags`)
    AC_CHECK_PROG(HAVE_SWIG,swig$EXEEXT,true)
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
    AC_CHECK_PROG([OCTAVE],[octave-config],[octave-config])
# find Octave arch
    AC_MSG_CHECKING([for Octave arch])
    OCTAVE_ARCH=`$OCTAVE -p CANONICAL_HOST_TYPE`-`$OCTAVE -p API_VERSION`
    OCTAVE_INCFLAGS="-I`$OCTAVE -p OCTINCLUDEDIR`"
    AC_MSG_RESULT([$OCTAVE_ARCH])
    AC_SUBST(OCTAVE_ARCH,[$OCTAVE_ARCH])
    AC_SUBST(OCTAVE_INCFLAGS,[$OCTAVE_INCFLAGS])
    test_octave=true
fi
AM_CONDITIONAL(USE_OCTAVE, test x$test_octave = xtrue )

AM_CXXFLAGS="$AM_CXXFLAGS -Wall $GSL_FLAGS"
AC_SUBST(AM_CXXFLAGS)

AC_CONFIG_FILES([
Makefile
src/Makefile
include/Makefile
widgets/Makefile
lang/Makefile
utils/Makefile
examples/Makefile
])
AC_OUTPUT

