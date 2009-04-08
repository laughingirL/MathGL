Name: mathgl
Version: 1.8.1
Release: 1%{?dist}
Summary: MathGL is a library for making high-quality scientific graphics
Summary(ru): MathGL - это библиотека для осуществления высококачественной визуализации данных
License: GPL
Group: System Environment/Libraries
Url: http://mathgl.sourceforge.net
Source: http://downloads.sourceforge.net/%{name}/%{name}-%{version}.tar.gz
# mandatory packages
BuildRequires: libpng-devel
Requires: libpng
# optional packages
BuildRequires: freeglut-devel hdf5-devel libjpeg-devel libtiff-devel
Requires: freeglut hdf5 libjpeg libtiff
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

%description
* a library for making high-quality scientific graphics under Linux and Windows;
* a library for the fast data plotting and handling of large data arrays;
* a library for working in window and console modes and for easy embedding into other programs;
* a library with large and growing set of graphics.

%package devel
Summary: Static libraries and header files for mathgl library
License: GPL
Group: System Environment/Libraries
Requires: %{name} = %{version}-%{release}

%description devel
The %{name}-devel package contains libraries and header files for
developing applications that use %{name}.

%description devel -l ru
Пакет %{name}-devel содержит библиотеки и файлы заголовков необходимые 
для разработки приложений с использованием %{name}.

%prep
%setup -q

%build
#%configure --enable-all
CPPFLAGS=-I/usr/lib/python2.5/site-packages/numpy/core/include ./configure --disable-gsl --enable-glut --disable-gif --disable-wx --disable-qt --without-qt --enable-python --prefix=/usr

#make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT%{_datadir}/%{name}/fonts
cp fonts/*.vfm $RPM_BUILD_ROOT%{_datadir}/%{name}/fonts

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%doc AUTHORS ChangeLog* NEWS README
%{_libdir}/*.so.*
%{_libdir}/python2.5/site-packages/*.so
%{_libdir}/python2.5/site-packages/*.pyo
%{_bindir}/mgl*
#%{_datadir}/%{name}/bigforth/*.fs
%{_datadir}/%{name}/fonts/*.vfm

%files devel
%defattr(-,root,root)
%{_includedir}/mgl/*
%{_libdir}/*.so
%{_libdir}/*.la
%{_libdir}/*.a
%{_libdir}/python2.5/site-packages/*.a
%{_libdir}/python2.5/site-packages/*.la
%{_libdir}/python2.5/site-packages/mathgl.pyc
%{_libdir}/python2.5/site-packages/mathgl.py

%post
/sbin/ldconfig

%changelog
* Wed Jun 4 2008 Nik <niktr@mail.ru> 
- disabled hdf5 support according to developer request

* Tue Jun 3 2008 Nik <niktr@mail.ru> 
- updated to version 1.7.1

* Sun Jun 2 2008 Nik <niktr@mail.ru> 
- updated to version 1.7

* Sat Apr 5 2008 Nik <niktr@mail.ru> 
- updated to version 1.6.2

* Thu Mar 13 2008 Nik <niktr@mail.ru>
- updated to version 1.6
- added fltk(-devel) to requres list

* Tue Mar 11 2008 Nik <niktr@mail.ru>
- tiff bug fixed
- enable-all option added

* Sat Mar 1 2008 Nik <niktr@mail.ru>
- spec cleanup
- --enable(jpeg, tiff, hdf5) features added
- enable-tiff leads to make error, submitted to author. Feature disabled.

* Fri Feb 29 2008 Nik <niktr@mail.ru>
- initial build of version 1.5
