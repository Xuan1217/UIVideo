#include "editwindow.h"
#include "ui_editwindow.h"

EditWindow::EditWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditWindow)
{
    ui->setupUi(this);
    playernumbers = 0;
    //playerindex = 0;
    player = new ThePlayer;
    player->setVideoOutput(ui->videowidget);
}

EditWindow::~EditWindow()
{
    delete ui;
}

void EditWindow::on_folder_clicked(){
    QString array = QFileDialog::getExistingDirectory(this,"need to play file","C:/Users/J/Desktop/MVP/MVP/videos");
    if(array.length() <= 0)
        return;
    std::string dirName = array.toStdString();
    getVideo(dirName);
    creatbuttonList();
    //set_name(0);
}

void EditWindow::getVideo(const std::string dirName) {
    // let's just check that Qt is operational first
    qDebug() << "Qt version: " << QT_VERSION_STR;
    this->loc = dirName;
    // collect all the videos in the folder
    videos.clear();
    videos = getInfoIn(dirName);
    if (videos.size() == 0) {
        const int result = QMessageBox::question(
                    nullptr,
                    QString("Tomeo"),
                    QString("no videos found! download, unzip, and add command line argument to \"quoted\" file location."),
                    QMessageBox::Yes );
        switch( result ) {

        default:
            break;
        }
        return;
    }
}

std::vector<TheButtonInfo> EditWindow::getInfoIn (std::string loc) {
    std::vector<TheButtonInfo> out =  std::vector<TheButtonInfo>();
    QDir dir(QString::fromStdString(loc) );
    QDirIterator it(dir);
    //get dir name
    QString dirname=QString::fromStdString(loc);
    qDebug()<<dirname;
    int dirlength = dirname.length();
    qDebug()<<dirlength;

    while (it.hasNext()) {
        QString f = it.next();
            if (f.contains("."))

#if defined(_WIN32)
            if (f.contains(".wmv")) { // windows
#else
            if (f.contains(".mp4") || f.contains("MOV"))  { // mac/linux
#endif
            QString thumb = f.left(f.length() - 4) +".png";
            QString videoname = f.right(f.length() - dirlength - 1);
            if (QFile(thumb).exists()) { // if a png thumbnail exists
                QImageReader *imageReader = new QImageReader(thumb);
                    QImage sprite = imageReader->read(); // read the thumbnail
                    if (!sprite.isNull()) {
                        QIcon* ico = new QIcon(QPixmap::fromImage(sprite)); // voodoo to create an icon for the button
                        QUrl* url = new QUrl(QUrl::fromLocalFile(f)); // convert the file location to a generic url
                        out.push_back(TheButtonInfo(url, ico, videoname));
                    } else{
                        qDebug() << "warning: skipping video because I couldn't process thumbnail " << thumb;
                    }
            } else {
                qDebug() << "warning: skipping video because I couldn't find thumbnail " << thumb;
            }
        }
    }
    qDebug()<<"out size:"<<out.size();
    playernumbers = out.size();
    return out;
}


void EditWindow::creatbuttonList(){
    // the QMediaPlayer which controls the playback
    QList<QPushButton*> btns =  ui->scrollwidgets->findChildren<QPushButton*>();
    foreach(QPushButton* btn, btns){
        delete btn;
    }
    delete ui->scrollwidgets->layout();
    QVBoxLayout *layout = new QVBoxLayout();
    ui->scrollwidgets->setLayout(layout);
    buttons.clear();
    for ( int i = 0; i < playernumbers; i++ ) {
        TheButton *button = new TheButton(ui->scrollwidgets);
        button->index = i;
        button->connect(button, SIGNAL(jumpTo(TheButtonInfo* )), player, SLOT (jumpTo(TheButtonInfo*))); // when clicked, tell the player to play.
        button->connect(button, SIGNAL(returnindex(int)), this, SLOT (getbuttonindex(int)));
        button->connect (button, SIGNAL(returnindex(int)), this, SLOT (set_name(int)));
        buttons.push_back(button);
        button->setFlat(true);
        button->setFocusPolicy(Qt::NoFocus);
        button->setIconSize(QSize(245, 140));
        layout->addWidget(button);
        button->init(&videos.at(i));
        button->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(button, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(ShowTaskBoxContextMenu(QPoint)));
    }
    player->setContent(&buttons, &videos);
}


