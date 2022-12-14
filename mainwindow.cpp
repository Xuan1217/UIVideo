#include "mainwindow.h"
#include "ui_mainwindow.h"

//Window Display
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralwidget->setStyleSheet("background-color : #FAF7FB");
    playernumbers = 0;
    playerindex = 0;
    player = new ThePlayer;
    player->setVideoOutput(ui->videowidget);

    // record the play mode
    playmode=0;

    connect(player,&QMediaPlayer::stateChanged, [=](){emit this->sequence_play();});

    // define the properties of slider for the progress of video
    connect(player,&QMediaPlayer::durationChanged,ui->timeslider,&QSlider::setMaximum);
    connect(player,&QMediaPlayer::positionChanged,ui->timeslider,&QSlider::setValue);
    connect(ui->timeslider,&QSlider::sliderMoved,player,&QMediaPlayer::setPosition);

    // define the functions for the slider of the volume
    connect(player,&QMediaPlayer::volumeChanged,ui->volumeslider,&QSlider::setValue);
    connect(ui->volumeslider,&QSlider::sliderMoved,player,&QMediaPlayer::setVolume);
    connect (player,SIGNAL (stateChanged(QMediaPlayer::State)), this, SLOT (voicechange()));

    QTimer *timer = new QTimer(nullptr);
    timer->setInterval(100);
    timer->start();
    connect( timer, SIGNAL (timeout()), SLOT(voicechange()));
    connect (timer, SIGNAL (timeout()),SLOT (timedisplay()));
    connect (player, SIGNAL (stateChanged(QMediaPlayer::State)), this, SLOT (timechange()));

    ui->centralwidget->setMouseTracking(true);
}

std::vector<TheButtonInfo> MainWindow::getInfoIn (std::string loc) {
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
                        qDebug() << "warning: skipping video because I couldn't process thumbnail " << thumb;
            }
            else
                qDebug() << "warning: skipping video because I couldn't find thumbnail " << thumb;
        }
    }
    qDebug()<<out.size();
    playernumbers = out.size();
    return out;
}

void MainWindow::getVideo(const std::string dirName) {
    // let's just check that Qt is operational first
    qDebug() << "Qt version: " << QT_VERSION_STR;

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

void MainWindow::creatbuttonList(){
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
        button->connect (button, SIGNAL(returnindex(int)), this, SLOT (setVideoTitle(int)));
        buttons.push_back(button);
        button->setFlat(true);
        button->setFocusPolicy(Qt::NoFocus);
        button->setIconSize(QSize(245, 140));
        layout->addWidget(button);
        button->init(&videos.at(i));
    }
    player->setContent(&buttons, &videos);
}



void MainWindow::set_name(int number){
    QString name1 = player->getButtons()->at(number)->info->videoname;
    ui->name->setText(name1);
//    name =title;
//    qDebug()<<playerindex;
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Button Operations
void MainWindow::on_zoom_clicked(){
    if(playernumbers == 0)
        return;
    ui->videowidget->setFullScreen(true);
}

void MainWindow::on_homebutton_clicked(){
    QMessageBox::question(
        nullptr,
        QString("Sorry"),
        QString("There is no link embeded in the app. "),
        QMessageBox::Yes);
}

void MainWindow::on_folder_clicked(){
    QString array = QFileDialog::getExistingDirectory(this,"need to play file","C:/Users/J/Desktop/MVP/MVP/videos");
    if(array.length() <= 0)
        return;
    std::string dirName = array.toStdString();
    getVideo(dirName);
    creatbuttonList();
    set_name(0);
}

void MainWindow::on_play_clicked(){
    if(playernumbers == 0)
        return;
    switch (player->state())
    {
        case QMediaPlayer::State::PausedState:
            player->play(); // starting playing again...
            ui->play->setIcon(QIcon(":/pic/pause.png"));
            break;
        default:
            player->pause();
            ui->play->setIcon(QIcon(":/pic/play_fill.png"));
            break;
    }
}

void MainWindow::on_voice_clicked(){
    if(playernumbers == 0)
        return;
    switch (player->volume()){
        case 0:
            player->setVolume(20);
            ui->voice->setIcon(QIcon(":/pic/24gf-volumeCross (1).png"));
            break;
        default:
            player->setVolume(0);
            ui->voice->setIcon(QIcon(":/pic/24gf-volumeHigh.png"));
            break;
    }
}

void MainWindow::timedisplay(){
    qint64 time1 = player->position()/1000;
    qint64 time2 = player->duration()/1000;
    QString time1String = QString::number(time1);
    if(time1<10)
        time1String = "00:0" + time1String;
    else
        time1String = "00:" + time1String;
    QString time2String = QString::number(time2);
    if(time2<10)
        time2String = "00:0" + time2String;
    else
        time2String = "00:" + time2String;
    ui->time1->setText(time1String);
    ui->time2->setText(time2String);
}

void MainWindow::timechange (){
    switch (player->state()) {
        case QMediaPlayer::State::PlayingState:
            ui->play->setIcon(QIcon(":/pic/pause.png"));
            break;
        default:
            ui->play->setIcon(QIcon(":/pic/play_fill.png"));
            break;
    }
    switch (player->volume()) {
        case 0:
            ui->voice->setIcon(QIcon(":/pic/24gf-volumeHigh.png"));
            break;
        default:
            ui->voice->setIcon(QIcon(":/pic/24gf-volumeCross (1).png"));
            break;
    }
}

void MainWindow::voicechange (){
    switch (player->volume()) {
        case 0:
            ui->voice->setIcon(QIcon(":/pic/24gf-volumeCross (1).png"));
            break;
        default:
            ui->voice->setIcon(QIcon(":/pic/24gf-volumeHigh.png"));
            break;
    }
}

void MainWindow::on_back_clicked(){
    if(playernumbers == 0){
        return;
    }
    player->setPosition(player->position()-3000);
}

void MainWindow::on_forward_clicked(){
    if(playernumbers == 0){
        return;
    }
//next 3s
    player->setPosition(player->position()+3000);
}

void MainWindow::on_nextone_clicked(){
    if(playernumbers == 0)
        return;
    if(playerindex == playernumbers-1)
        playerindex =  0;
    else
         playerindex= playerindex+1;
    set_name(playerindex);
    TheButtonInfo* button = player->getButtons()->at(playerindex)->info;
    player->jumpTo(button);
    //setVideoTitle(playerindex);
}

void MainWindow::on_lastone_clicked(){
    if(playernumbers == 0)
        return;
    if(playerindex == 0)
        playerindex =  playernumbers-1;
    else
         playerindex= playerindex-1;
    set_name(playerindex);
    TheButtonInfo* button = player->getButtons()->at(playerindex)->info;
    player->jumpTo(button);
    //setVideoTitle(playerindex);
}

void MainWindow::on_large_clicked(){
    static QRect location;
    if(this->isFullScreen()){
        this->setGeometry(location);
        ui->large->setIcon(QIcon(":/Icon/full.png"));
    }else{
        location = this->geometry();
        this->showFullScreen();
        ui->large->setIcon(QIcon(":/Icon/ifull.png"));
    }
}

void MainWindow::on_sequence_clicked()
{
    if(playernumbers == 0)
        return;
    if(playmode==0){
        ui->sequence->setIcon(QIcon(":/pic/liebiao.png"));
        playmode += 1;
    }
    else if(playmode==1){
        ui->sequence->setIcon(QIcon(":/pic/danqu.png"));
        playmode += 1;
    }
    else{
        ui->sequence->setIcon(QIcon(":/pic/xunhuan.png"));
        playmode = 0;
    }
}

void MainWindow::sequence_play()
{
    switch(playmode){
    case 0:{        // play this one endlessly
        if (player->position() == player->duration()){
            player->setPosition(0);
            player->play();
        }

        break;
    }
    case 1:        // play in loop
        if (player->position() == player->duration() && player->state() != QMediaPlayer::State::PlayingState){
            std::cout<<playerindex<<std::endl;
            if(playernumbers == 0)
                return;
            if(playerindex == playernumbers-1)
                playerindex =  0;
            else
                 playerindex= playerindex+1;
            set_name(playerindex);
            TheButtonInfo* button = player->getButtons()->at(playerindex)->info;
            player->jumpTo(button);
            player->play();
        }
        break;

    case 2:        // default stop when finished
        break;
    default:
        playmode = 0;
    }
}


void MainWindow::on_screenshot_clicked(){
    if(playernumbers == 0)
        return;
    QScreen *screen = QGuiApplication::primaryScreen();
    QFileInfo file("./");
    QString s = file.absolutePath();
    QString folderPath = s+"/ScreenShot";
    QString filePathName = "cut-";
    filePathName += QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss-zzz");
    filePathName += ".png";

    QPixmap pixmap = screen->grabWindow(0);
//    if(!pixmap.save(filePathName,"png"))
//    {
//        std::cout<<"cut save png failed"<<std::endl;
//    }
    std::cout<<"Full pixmap width: "<<pixmap.width()<<" height: "<<pixmap.height()<<std::endl;
    QRect geo = this->geometry();
    QRect appGeo = geo; // the program's position in screen
    geo = ui->videowidget->geometry(); // video in screen shot
    QWidget *centerWidget = centralWidget(); // position of main window
    QRect centerRect = centerWidget->geometry();
    QRect copyGeo;
    copyGeo.setX(geo.x() + appGeo.x() + centerRect.x()+15); // add three x
    copyGeo.setY(geo.y() + appGeo.y() + centerRect.y()+15);
    copyGeo.setWidth(geo.width()-15);
    copyGeo.setHeight(geo.height()-15);

    QPixmap pixmapCopy = pixmap.copy(copyGeo); // copy
    filePathName.prepend(folderPath+"/Copy+");
    QDir dir(folderPath);
    if(!dir.exists()){
        bool ismkdir = dir.mkdir(folderPath);
        if(!ismkdir)
            qDebug() << "Create path fail";
        else
            qDebug() << "Create fullpath success";
    }
    if(!pixmapCopy.save(filePathName,"png"))
    {
        std::cout<<"copy cut save png failed"<<std::endl;
    }else{
        QMessageBox::question(
            nullptr,
            QString("Success"),
            QString("Screenshot Successfully! "),
            QMessageBox::Yes);
    }
}


//void MainWindow::on_speed_clicked()
//{
//    if(rate==1)
//        {
//            qDebug()<<player->playbackRate();
//            player->setPlaybackRate(2);
//            rate = 2;
//        }
//        else
//        {
//            qDebug()<<player->playbackRate();
//            player->setPlaybackRate(1);
//            rate = 1;
//        }
//}

//mouse event to zoom in and zoom out

//if the mouse pressed left, obtain the _plast and _curpos
void MainWindow::mousePressEvent(QMouseEvent* event){
    Q_UNUSED(event);
    if (event->button()==Qt::LeftButton){
        this->_isleft = true;
        QPoint temp = event->globalPos();
        _plast = temp;
        _curpos = countFlag(event->pos(), countRow(event->pos()));
    }
}

//Restore the cursor style to the original style
void MainWindow::mouseReleaseEvent(QMouseEvent* event){
    Q_UNUSED(event);
    if(_isleft)
        _isleft = false;
    setCursor(Qt::ArrowCursor);
}

//Restore the cursor style to the original style
void MainWindow::mouseMoveEvent(QMouseEvent* event){
    Q_UNUSED(event);
    if(this->isFullScreen())
        return;
    int poss = countFlag(event->pos(), countRow(event->pos()));
    setCursorType(poss);
    qDebug()<<poss<<Qt::endl;
    if(_isleft){
        QPoint ptemp = event->globalPos();
        ptemp = ptemp - _plast;
        if(_curpos == 22){
            ptemp = ptemp + pos();
            move(ptemp);
        }else{
            QRect arecta = geometry();
            switch (_curpos){
            case 11:
                arecta.setTopLeft(arecta.topLeft() + ptemp);
                break;
            case 13:
                arecta.setTopRight(arecta.topRight() + ptemp);
                break;
            case 31:
                arecta.setBottomLeft(arecta.bottomLeft() + ptemp);
                break;
            case 33:
                arecta.setBottomRight(arecta.bottomRight() + ptemp);
                break;
            case 12:
                arecta.setTop(arecta.top() + ptemp.y());
                break;
            case 21:
                arecta.setLeft(arecta.left() + ptemp.x());
                break;
            case 23:
                arecta.setRight(arecta.right() + ptemp.x());
                break;
            case 32:
                arecta.setBottom(arecta.bottom() + ptemp.y());
                break;
            }
            setGeometry(arecta);
        }
        _plast = event->globalPos();
    }
}

void MainWindow::setCursorType(int check){
    switch (check) {
    case 11:
    case 33:
        setCursor(Qt::SizeFDiagCursor);
        break;
    case 13:
    case 31:
        setCursor(Qt::SizeBDiagCursor);
        break;
    case 21:
    case 23:
        setCursor(Qt::SizeHorCursor);
        break;
    case 12:
    case 32:
        setCursor(Qt::SizeVerCursor);
        break;
    case 22:
        setCursor(Qt::ArrowCursor);
        QApplication::restoreOverrideCursor();
        break;
    }
}

int MainWindow::countRow(QPoint point){
    int row;
    if(point.x()<5)
        row = 1;
    else
        if(point.x()>(this->width()-5))
            row = 3;
        else
            row = 2;
    return row;
}

int MainWindow::countFlag(QPoint point, int row){
 if(point.y()< 5)
     return 10+row;
 else if(point.y()>(this->height()-5))
     return 30+row;
 else
     return 20+row;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
//click space pause
    if (event->key() == Qt::Key_Space) {
        on_play_clicked();
    }
//click left back 3s
    if(event->key() == Qt::Key_Right || event->key() == Qt::Key_D){
        on_forward_clicked();
    }
//click right next 3s
    if(event->key() == Qt::Key_Left || event->key() == Qt::Key_A){
        on_back_clicked();
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
