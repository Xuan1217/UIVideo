//
// Created by twak on 11/11/2019.
//

#ifndef CW2_THE_BUTTON_H
#define CW2_THE_BUTTON_H

#include <QPushButton>
#include <QUrl>

#include "player.h"

class VideoInfo {

public:
    QString* title; 
    QUrl* url; // video file to play
    QIcon* icon; // icon to display

    VideoInfo ( QString &title, QUrl* url, QIcon* icon) : url (url), icon (icon) {
        this->title = new QString();
        this->title->append(title);
    }
};

class Thumbnail : public QWidget {
    Q_OBJECT

public:
    QPushButton *btn;
    QLabel *label;
    VideoInfo* info;

    Thumbnail(QWidget *parent, Player *player) : QWidget(parent) {
        QVBoxLayout *layout = new QVBoxLayout();

        this->label = new QLabel("<loading>");
        this->btn = new QPushButton();

        QSize size = QSize(200, 110);
        btn->setMinimumSize(size);
        btn->setMaximumSize(size);
        btn->setIconSize(size);
        // handle clicked signal 
        btn->connect(
                btn, SIGNAL( clicked() ),
                this, SLOT ( clicked() ));
        // forward jumpTo to player
        btn->connect(
                this, SIGNAL( jumpTo(VideoInfo*) ),
                player, SLOT( jumpTo(VideoInfo*) ));

        layout->addWidget(label);
        layout->addWidget(btn);
        this->setLayout(layout);
    }

    void initialize(VideoInfo* i);

private slots:
    void clicked();
signals:
    void jumpTo(VideoInfo*);
};

#endif //CW2_THE_BUTTON_H
