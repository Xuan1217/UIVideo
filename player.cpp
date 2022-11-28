//
// Created by twak on 11/11/2019.
//

#include <QtWidgets/QLabel>

#include "player.h"

#include "thumbnail.h"

using namespace std;

Player::Player(QLayout *buttonLayout) : QMediaPlayer(NULL) {
    this->buttonLayout = buttonLayout;

    setVolume(0); // be slightly less annoying
    connect(
        this, SIGNAL( stateChanged(QMediaPlayer::State) ),
        this, SLOT( playStateChanged(QMediaPlayer::State) ));
}

void Player::setLabel(QLabel *label) {
    this->label = label;
}

// handle when 'add video' button tells us to add
// a video info 
void Player::addVideo(VideoInfo *info) {
    Thumbnail *thumbnail = new Thumbnail(nullptr, this);
    thumbnail->initialize(info);
    this->infos->push_back(*info);
    this->buttons->push_back(thumbnail);
    this->buttonLayout->addWidget(thumbnail);
    this->buttonLayout->setAlignment(thumbnail, Qt::AlignHCenter);
}

// all buttons have been setup, store pointers here
void Player::setContent(std::vector<Thumbnail*>* b, std::vector<VideoInfo>* i) {
    buttons = b;
    infos = i;
    jumpTo(buttons->at(0)->info);
}

void Player::playStateChanged (QMediaPlayer::State ms) {
    // update video label to current video
    if (this->currentInfo != nullptr) {
        this->label->setText( *this->currentInfo->title );
    }
    switch (ms) {
        case QMediaPlayer::State::StoppedState:
            play(); // starting playing again...
            break;
    default:
        break;
    }
}

void Player::jumpTo (VideoInfo* button) {
    this->currentInfo = button;
    setMedia( * button -> url);
    play();
}
