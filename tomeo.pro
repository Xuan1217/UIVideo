QT += core gui widgets multimedia multimediawidgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        thumbnail.cpp \
        player.cpp \
        video_control.cpp \
        add_video.cpp \
        search.cpp \
        util.cpp \
        tomeo.cpp 

HEADERS += \
    thumbnail.h \
    player.h \
    video_control.h \
    add_video.h \
    search.h \
    util.h

RESOURCES += \
    resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

