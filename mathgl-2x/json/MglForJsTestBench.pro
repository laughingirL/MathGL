#-------------------------------------------------
#
# Project created by QtCreator 2012-08-23T14:34:42
#
#-------------------------------------------------

QT       += core gui webkit network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MglForJsTestBench
TEMPLATE = app

INCLUDEPATH=../mathgl/mathgl-2x/include/ ../mathgl/mathgl-2x-build/include/

SOURCES += main.cpp\
           MainWindow.cpp \
           Backend.cpp

HEADERS += MainWindow.hpp \
           Backend.hpp

FORMS   += MainWindow.ui

LIBS += -L../mathgl/mathgl-2x-build/src/ -lmgl

OTHER_FILES += main.js \
               mathgl.Backend.js \
               mathgl.Graph.js \
               mathgl.js \
               mathgl.View.js \
               mathgl.WebkitBackend.js \
               sylvester.js \
               index.html

COPY_TO_DEST_FILES = $${OTHER_FILES}

linux-g++ {
  for(FILE,COPY_TO_DEST_FILES){
    QMAKE_POST_LINK += $$quote(cp $${IN_PWD}/$${FILE} $${OUT_PWD} $$escape_expand(\\n\\t))
    POST_TARGETDEPS += $${IN_PWD}/$${FILE}
  }
}

win32 {
  DESTDIR_WIN = $${OUT_PWD}
  DESTDIR_WIN ~= s,/,\\,g
  for(FILE,COPY_TO_DEST_FILES){
    FILE_WIN=$${IN_PWD}/${FILE}
    FILE_WIN ~= s,/,\\,g
    QMAKE_POST_LINK +=$$quote(cmd /c copy /y $${FILE_WIN} $${DESTDIR_WIN} $$escape_expand(\\n\\t))
    POST_TARGETDEPS += $${IN_PWD}/$${FILE}
  }
}
