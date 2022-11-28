#ifndef VIDEO_CONTROL__H
#define VIDEO_CONTROL__H

#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QLabel>

class Player;

class VideoControl : public QWidget {
    Q_OBJECT
private:
    Player *player;

    // icons to swap out play/pause button
    QIcon *playIcon;
    QIcon *pauseIcon;

    // Qt components
    QPushButton *playBtn;
    QProgressBar *bar;
    QLabel *time;
public:
    VideoControl(Player *player);
private slots:
    void updateDuration(qint64 duration);
    void updatePosition(qint64 position);
    void togglePlayback();
};

#endif // VIDEO_CONTROL__H
