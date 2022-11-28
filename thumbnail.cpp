//
// Created by twak on 11/11/2019.
//

#include <QImageReader>

#include "thumbnail.h"

#include "util.h"

void Thumbnail::initialize(VideoInfo* i) {
    this->label->setText(*i->title);
    if (i->icon != nullptr) {
        this->btn->setIcon(*i->icon);
    } else {
        QIcon *icon = createIcon(":/images/no-thumbnail.jpg");
        if (icon != nullptr)
            this->btn->setIcon(*icon);
    }
    this->info = i;
}

void Thumbnail::clicked() {
    emit jumpTo(info);
}
