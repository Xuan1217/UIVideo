#include "videowidget.h"
#include <QKeyEvent>
#include <QMouseEvent>

VideoWidget::VideoWidget(QWidget *parent)
    : QVideoWidget(parent) {
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    setAttribute(Qt::WA_OpaquePaintEvent);
}

void VideoWidget::keyPressEvent(QKeyEvent *event) {
//click the esc close fullscreen
    if (event->key() == Qt::Key_Escape) {
          setWindowFlags (Qt::SubWindow);
          showNormal();
    }
}

void VideoWidget::mouseDoubleClickEvent(QMouseEvent *event) {
//double click the video to open or close fullscreen
    if (isFullScreen()) {
        setWindowFlags (Qt::SubWindow);
        showNormal();
    } else {
        setWindowFlags (Qt::Window);
        showFullScreen();
    }
}
