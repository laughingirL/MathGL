#DEFINES += MGL_DOC_DIR=\\\"$(MGLDOCDIR)\\\"
TEMPLATE = app
CONFIG += release \
    warn_on \
    thread \
    qt
TARGET = ../bin/udav
RESOURCES += udav.qrc
SOURCES += calc_dlg.cpp \
    hint_dlg.cpp \
    find_dlg.cpp \
    textedit.cpp \
    style_dlg.cpp \
    setup_dlg.cpp \
    qmglsyntax.cpp \
    qmglcanvas.cpp \
    prop_dlg.cpp \
    opt_dlg.cpp \
    open_dlg.cpp \
    newcmd_dlg.cpp \
    main.cpp \
    info_dlg.cpp \
    files_dlg.cpp \
    args_dlg.cpp \
    anim_dlg.cpp \
    mem_pnl.cpp \
    dat_pnl.cpp \
    plot_pnl.cpp \
    help_pnl.cpp \
    text_pnl.cpp \
    udav_wnd.cpp
HEADERS += calc_dlg.h \
    hint_dlg.h \
    find_dlg.h \
    textedit.h \
    style_dlg.h \
    setup_dlg.h \
    qmglsyntax.h \
    qmglcanvas.h \
    prop_dlg.h \
    opt_dlg.h \
    open_dlg.h \
    newcmd_dlg.h \
    info_dlg.h \
    files_dlg.h \
    args_dlg.h \
    anim_dlg.h \
    mem_pnl.h \
    dat_pnl.h \
    plot_pnl.h \
    help_pnl.h \
    text_pnl.h \
    udav_wnd.h
DISTFILES += ../TODO \
    ../ChangeLog.txt \
    ../INSTALL
RC_FILE = udav.rc
INSTALLS += target data icon desc
TRANSLATIONS = udav_ru.ts
LIBS += -lmgl -lhdf5
desc.files += ../UDAV.desktop
icon.files += udav.png
data.files += udav_ru.qm
target.path = /usr/local/bin/
data.path = /usr/local/share/udav/
icon.path = /usr/local/share/pixmaps/
desc.path = /usr/local/share/applications/
OTHER_FILES += ReadMe
