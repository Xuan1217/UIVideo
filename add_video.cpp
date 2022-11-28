#include "add_video.h"

#include <QtWidgets/QFileDialog>
#include <QUrl> 
#include <QtDebug>

#include <string>
#include <fstream>
#include <iostream>

#include "util.h"

void AddVideo::clicked() {
    QString result = QFileDialog::getOpenFileName(
            this, "Choose Video", "", "Video Files (*.mp4, *.mov, *.wmv)");
    string filepath = result.toStdString();
    
    // new_path should be the path to a new file
    // in our videos directory with the same
    // filename as the file the user chose
    string new_path = getFilename(filepath);
    new_path.insert(0, this->videosDir);

    std::ifstream src(filepath, std::ios::binary);
    std::ofstream dst(new_path, std::ios::binary);
    
    // copy the source file to the destination path
    dst << src.rdbuf();

    QString qpath = QString::fromStdString(new_path);

    QUrl *url = new QUrl(QUrl::fromLocalFile(qpath));
    
    // tell the player to add the video
    emit addVideo(new VideoInfo(qpath, url, nullptr));
}
