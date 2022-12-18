#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "QtWidgets/qmenu.h"
#include "buttons.h"
#include "player.h"
#include <QMainWindow>
#include <QWidget>
#include <iostream>
#include <QApplication>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QMediaPlaylist>
#include <string>
#include <vector>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtCore/QFileInfo>
#include <QtWidgets/QFileIconProvider>
#include <QDesktopServices>
#include <QImageReader>
#include <QMessageBox>
#include <QFileDialog>
#include <QtCore>
#include <QSize>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include <QScreen>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_zoom_clicked();
    void on_homebutton_clicked();
    void on_folder_clicked();
    void onTaskBoxContextMenuEvent();
    void on_play_clicked();
    void on_voice_clicked();
    void timechange();
    void voicechange();
    void timedisplay();
    void on_back_clicked();
    void on_forward_clicked();
    void on_nextone_clicked();
    void on_lastone_clicked();
    void on_large_clicked();
    void on_sequence_clicked();
    void sequence_play();
    void on_screenshot_clicked();
    void set_name(int number);
    void keyPressEvent(QKeyEvent *event);
    void ShowTaskBoxContextMenu(QPoint);
//    void on_speed_clicked(); s

    void on_edit_clicked();

private:
    Ui::MainWindow *ui;
    std::vector<TheButtonInfo> videos;
    std::vector<TheButton*> buttons;
    ThePlayer *player;
    int playerindex;
    int playernumbers;
    int playmode;
    bool _isleft = false;
    int _curpos = 0;
    QPoint _plast;
    QString name;
    void creatbuttonList();
    void playMovie();
    std::vector<TheButtonInfo> getInfoIn (std::string loc);
    void getVideo(const std::string dirName);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    int countRow(QPoint point);
    int countFlag(QPoint point, int row);
    void setCursorType (int check);
    QMenu *m_rightClieckMenu;
};
#endif // MAINWINDOW_H
