//
// Created by twak on 11/11/2019.
//

#ifndef CW2_THE_PLAYER_H
#define CW2_THE_PLAYER_H

#include <QApplication>
#include <QMediaPlayer>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLayout>

#include <vector>
#include <QTimer>

class VideoInfo;
class Thumbnail;

using namespace std;

class Player : public QMediaPlayer {

Q_OBJECT

private:
    vector<VideoInfo>* infos;
    vector<Thumbnail*>* buttons;
    QLayout *buttonLayout;
    QTimer* mTimer;
    long updateCount = 0;

    VideoInfo *currentInfo;
    QLabel *label;
public:

    Player(QLayout *buttonLayout);

    void setLabel(QLabel *label);

    // all buttons have been setup, store pointers here
    void setContent(vector<Thumbnail*>* b, vector<VideoInfo>* i);
private slots:

    // change the image and video for one button every one second
    // void shuffle();

    void playStateChanged (QMediaPlayer::State ms);

public slots:

    // start playing this ButtonInfo
    void jumpTo (VideoInfo* info);
    void addVideo (VideoInfo* info);
};

#endif //CW2_THE_PLAYER_H
