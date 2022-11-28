
#include "video_control.h"

#include <QtGlobal>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QLabel>
#include <QMediaPlayer>

#include <iostream>

#include "player.h"
#include "util.h"

VideoControl::VideoControl(Player *player) {
    this->player = player;

    QHBoxLayout *layout = new QHBoxLayout();

    // get icons stored using .qrc file
    this->playIcon  = createIcon(":/images/play.png");
    this->pauseIcon = createIcon(":/images/pause.png");

    // create inner Qt components
    this->playBtn = new QPushButton();
    playBtn->setIcon(*this->pauseIcon);

    this->time = new QLabel();
    this->time->setText("00:00 / 00:00");

    this->bar = new QProgressBar();
    this->bar->setTextVisible(false);

    layout->addWidget(this->playBtn);
    layout->addWidget(this->bar);
    layout->addWidget(this->time);

    this->setLayout(layout);

    // handle duration change (video change)
    connect(
        player, SIGNAL( durationChanged(qint64) ),
        this, SLOT( updateDuration(qint64) ));
    // handle position change (e.g. second passed while playing)
    connect(
        player, SIGNAL( positionChanged(qint64) ),
        this, SLOT( updatePosition(qint64) ));
    // handle playing toggle (clicked play/pause button)
    connect(
        this->playBtn, SIGNAL( pressed() ),
        this, SLOT( togglePlayback() ));
}

void VideoControl::updateDuration(qint64 millis) {
    this->bar->setMaximum(millis / 1000); // length in seconds
    this->bar->setValue(0);
}

void VideoControl::updatePosition(qint64 millis) {
    this->bar->setValue(millis / 1000);

    // calculate elapsed & total time in the video
    // in minutes & seconds for display
    int elapsed_m = millis / (1000 * 60);
    int elapsed_s = millis / 1000 - elapsed_m * 60;

    int duration = this->bar->maximum();
    int total_m = duration / 60;
    int total_s = duration - total_m * 60;

    char buf[42];
    // create formatted string in the following format:
    // xx:xx / xx:xx 
    snprintf(buf, sizeof(buf), "%02d:%02d / %02d:%02d", elapsed_m, elapsed_s, total_m, total_s);
    this->time->setText(buf);
}

void VideoControl::togglePlayback() {
    // just toggles play/pause, updating the icon appropriately
    // uses current player state so other sources can change
    // if the video is playing (e.g. clickeding on another
    // video calls `play`) without this messing up
    if (this->player->state() == QMediaPlayer::PlayingState) {
        this->playBtn->setIcon(*this->playIcon);
        this->player->pause();
    } else {
        this->playBtn->setIcon(*this->pauseIcon);
        this->player->play();
    }
}
