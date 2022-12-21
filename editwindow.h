#ifndef EDITWINDOW_H
#define EDITWINDOW_H

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

namespace Ui {
class EditWindow;
}

class EditWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditWindow(QWidget *parent = nullptr);
    ~EditWindow();

private slots:
    void on_folder_clicked();


private:
    ThePlayer *player;
    void getVideo(const std::string dirName);
    std::vector<TheButtonInfo> getInfoIn (std::string loc);
    void creatbuttonList();
    Ui::EditWindow *ui;
    std::string loc;
    std::vector<TheButtonInfo> videos;
    std::vector<TheButton*> buttons;
    int playernumbers;
};

#endif // EDITWINDOW_H
