#include "util.h"

#include <QIcon>
#include <QImageReader>
#include <QImage>
#include <QPixmap>

#include <string>

// helper to create the icon for thumbnails
// if path begins with ':' it will search
// the .qrc files
// e.g. :/images/play.png refers to the play
// image in the images directory in this
// project
QIcon* createIcon(char *path) {
    QImageReader *imageReader = new QImageReader(path);
    QImage sprite = imageReader->read(); // read the thumbnail
    if (sprite.isNull())
        return nullptr;
    return new QIcon(QPixmap::fromImage(sprite));
}

// get the filename of a std string
// done by taking the content after the last slash
// e.g. path/to/file => file
std::string getFilename(std::string path) {
    std::string filename = path;
    size_t last_slash = filename.find_last_of("\\/");
    if (std::string::npos != last_slash)
        filename.erase(0, last_slash+1);
    return filename;
}
