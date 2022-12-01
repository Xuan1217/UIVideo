#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget) {
    ui->setupUi(this);
    playernumbers = 0;
    playerindex = 0;
    player = new ThePlayer;
    player->setVideoOutput(ui->videoWidget);
    playertitle= "Video";
    //Set video progress bar
    connect(player,&QMediaPlayer::durationChanged,ui->playerslider,&QSlider::setMaximum);
    connect(player,&QMediaPlayer::positionChanged,ui->playerslider,&QSlider::setValue);
    connect(ui->playerslider,&QSlider::sliderMoved,player,&QMediaPlayer::setPosition);



    //Set volume progress bar

    //Detect video status and change button icon
    QTimer *mTimer = new QTimer(nullptr);
    mTimer->setInterval(100);
    mTimer->start();
    connect( mTimer, SIGNAL (timeout()), SLOT(volumeStateChanged()));
    connect (mTimer, SIGNAL (timeout()),SLOT (updateduration()));
    connect (player, SIGNAL (stateChanged(QMediaPlayer::State)), this, SLOT (playerStateChanged()));

}

Widget::~Widget(){
    delete ui;
}

//slots
void Widget::on_open_clicked(){
//open the file
    QString array = QFileDialog::getExistingDirectory(this,"need to play file","\\Mac\Home\Desktop\myuicw\videos");
    if(array.length() <= 0)
        return;
    std::string dirName = array.toStdString();
    getVideo(dirName);
    creatbuttonList();
    playerindex = 0;
    setVideoTitle(playerindex);
}


void Widget::on_pause_clicked(){
    if(playernumbers == 0){
        return;
    }
    switch (player->state())
    {
        case QMediaPlayer::State::PausedState:
            player->play(); // starting playing again...
            ui->pause->setIcon(QIcon(":/icon/pause.png"));
            break;
        default:
            player->pause();
            ui->pause->setIcon(QIcon(":/icon/start.png"));
            break;
    }
}


void Widget::on_fullscreen_clicked(){
    if (isFullScreen()) {
        setWindowFlags (Qt::SubWindow);
        showNormal();
    } else {
        setWindowFlags (Qt::Window);
        showFullScreen();
    }
}


void Widget::on_mute_clicked(){
//click the mute to zero-music
    switch (player->volume())
    {
        case 0:
            player->setVolume(20);
            ui->mute->setIcon(QIcon(":/icon/volumeHigh.png"));
            break;
        default:
            player->setVolume(0);
            ui->mute->setIcon(QIcon(":/icon/volumeDisable.png"));
            break;
    }
}


void Widget::on_backward_clicked(){
    player->setPosition(player->position()-3000);
}


void Widget::on_forward_clicked(){
//next 3s
    player->setPosition(player->position()+3000);
}


void Widget::on_speed_clicked(){
//speed
    int rate = player->playbackRate()/1;
    switch(rate)
    {
    case 1:player->setPlaybackRate(2);
        ui->speed->setIcon(QIcon(":/icon/speedx2.png"));
        break;
    case 2: player->setPlaybackRate(1);
        ui->speed->setIcon(QIcon(":/icon/speed.png"));
        break;
    default: player->setPlaybackRate(1);
        ui->speed->setIcon(QIcon(":/icon/speed.png"));
        break;
    }

}


void Widget::playerStateChanged (){
    switch (player->state()) {
        case QMediaPlayer::State::PausedState:
            ui->pause->setIcon(QIcon(":/icon/start.png"));
            break;
        default:
            ui->pause->setIcon(QIcon(":/icon/pause.png"));
            break;
    }
    switch (player->volume()) {
        case 0:
            ui->mute->setIcon(QIcon(":/icon/volumeDisable.png"));
            break;
        default:
            ui->mute->setIcon(QIcon(":/icon/volumeHigh.png"));
            break;
    }
}


void Widget::volumeStateChanged (){
    switch (player->volume()) {
        case 0:
            ui->mute->setIcon(QIcon(":/icon/volumeDisable.png"));
            break;
        default:
            ui->mute->setIcon(QIcon(":/icon/volumeHigh.png"));
            break;
    }
}


void Widget::on_tag_clicked() {
//Used to tag the video to classify
}


void Widget::on_user_clicked() {
//used for user registration/login
}


void Widget::on_search_clicked() {
//Click the search box and enter text to search for the video
}


void Widget::on_share_clicked() {
//Click to share the current video
}


void Widget::on_menu_clicked() {
//Enter the menu interface.
}



void Widget::on_next_clicked() {
//next vedio
}

void Widget::getbuttonindex(int index){
    QString qString;
    playerindex=index;
    qDebug()<<qString.number(playerindex);

}

void Widget::setVideoTitle(int index){
//set video tittle

}

void Widget::updateduration(){
    qint64 time1 = player->position()/1000;
    qint64 time2 = player->duration()/1000;
    QString Str1 = QString::number(time1);
    Str1 = Str1 + "/";
    Str1 = "00:" + Str1;
    QString Str2 = QString::number(time2);
    Str2 = "00:" + Str2;
    Str1.append(Str2);
    ui->time->setText(Str1);
}

void Widget::on_upload_clicked()
{
//Click to upload the video, drag the file into the box to upload it.

}

//end slots


//function
std::vector<TheButtonInfo> Widget::getInfoIn (std::string loc) {
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

            QString thumb = f.left( f .length() - 4) +".png";
            QString videoname = f.right(f .length() - dirlength - 1);
            qDebug()<<videoname;
            if (QFile(thumb).exists()) { // if a png thumbnail exists
                QImageReader *imageReader = new QImageReader(thumb);
                    QImage sprite = imageReader->read(); // read the thumbnail
                    if (!sprite.isNull()) {
                        QIcon* ico = new QIcon(QPixmap::fromImage(sprite)); // voodoo to create an icon for the button
                        QUrl* url = new QUrl(QUrl::fromLocalFile( f )); // convert the file location to a generic url
                        out . push_back(TheButtonInfo( url , ico , videoname));
                    }
                    else
                        qDebug() << "warning: skipping video because I couldn't process thumbnail " << thumb << endl;
            }
            else
                qDebug() << "warning: skipping video because I couldn't find thumbnail " << thumb << endl;
        }
    }
    qDebug()<<out.size();
    playernumbers = out.size();
    return out;
}

void Widget::getVideo(const std::string dirName) {
    // let's just check that Qt is operational first
    qDebug() << "Qt version: " << QT_VERSION_STR << endl;

    // collect all the videos in the folder
    videoList.clear();
    videoList = getInfoIn(dirName);

    if (videoList.size() == 0) {
        const int result = QMessageBox::question(
                    nullptr,
                    QString("Tomeo"),
                    QString("no videos found! download, unzip, and add command line argument to \"quoted\" file location."),
                    QMessageBox::Yes );

        return;
    }
}

void Widget::creatbuttonList() {
    // the QMediaPlayer which controls the playback
    QList<QPushButton*> btns =  ui->buttonWidget->findChildren<QPushButton*>();
    foreach(QPushButton* btn, btns){
        delete btn;
    }
    delete ui->buttonWidget->layout();
    QVBoxLayout *layout = new QVBoxLayout();
    ui->buttonWidget->setLayout(layout);
	buttonList.clear();
    // create the six buttons
    for ( int i = 0; i < playernumbers; i++ ) {
        TheButton *button = new TheButton(ui->buttonWidget);
        button->index = i;
        button->connect(button, SIGNAL(jumpTo(TheButtonInfo* )), player, SLOT (jumpTo(TheButtonInfo*))); // when clicked, tell the player to play.
        button->connect(button, SIGNAL(returnindex(int)), this, SLOT (getbuttonindex(int)));
        button->connect (button, SIGNAL(returnindex(int)), this, SLOT (setVideoTitle(int)));
        buttonList.push_back(button);
        button->setFlat(true);
        button->setFocusPolicy(Qt::NoFocus);
        layout->addWidget(button);
        button->init(&videoList.at(i));
    }
    player->setContent(&buttonList, &videoList);
}

void Widget::keyPressEvent(QKeyEvent *event) {
//click space pause
    if (event->key() == Qt::Key_Space) {
        on_pause_clicked();
    }
//click left back 3s
    if(event->key() == Qt::Key_Right || event->key() == Qt::Key_D){
        on_forward_clicked();
    }
//click right next 3s
    if(event->key() == Qt::Key_Left || event->key() == Qt::Key_A){
        on_backward_clicked();
    }
//click up volume up
    if(event->key() == Qt::Key_Up || event->key() == Qt::Key_W){
        player->setVolume(player->volume()+5);
    }
//click down volume down
    if(event->key() == Qt::Key_Down || event->key() == Qt::Key_S){
        player->setVolume(player->volume()-5);
    }
}



//end function


