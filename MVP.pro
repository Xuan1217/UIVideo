QT += core gui widgets multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    buttons.cpp \
    editwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    player.cpp \
    videowidget.cpp

HEADERS += \
    buttons.h \
    editwindow.h \
    mainwindow.h \
    player.h \
    videowidget.h

FORMS += \
    editwindow.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES += \
    pic/24gf-backward.png \
    pic/24gf-camera3.png \
    pic/24gf-forward.png \
    pic/24gf-fullScreenEnter3.png \
    pic/24gf-fullScreenExit.png \
    pic/24gf-tags.png \
    pic/24gf-videoCamera.png \
    pic/24gf-volumeCross (1).png \
    pic/24gf-volumeCross.png \
    pic/24gf-volumeHigh.png \
    pic/24gf-volumeZero (1).png \
    pic/24gf-volumeZero.png \
    pic/Maximize-2.png \
    pic/Maximize-3.png \
    pic/Minimize-1.png \
    pic/Minimize-2.png \
    pic/Shut down-1.png \
    pic/Shut down-2.png \
    pic/Videotape-1.png \
    pic/add.png \
    pic/backward.png \
    pic/cut.png \
    pic/danqu.png \
    pic/daotui.png \
    pic/file-add.png \
    pic/forward.png \
    pic/kuaijin.png \
    pic/liebiao.png \
    pic/lvjing.png \
    pic/more.png \
    pic/pause.png \
    pic/play_fill.png \
    pic/speed.png \
    pic/texiao.png \
    pic/tiaojie.png \
    pic/user.png \
    pic/video.png \
    pic/wenben.png \
    pic/xunhuan.png \
    pic/yinyue.png
