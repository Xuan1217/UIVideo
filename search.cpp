#include "search.h"
#include "thumbnail.h"

#include <QString>

#include <iostream>
#include <vector>

Searching::Searching(std::vector<Thumbnail*> *buttons) {
    this->buttons = buttons;

    QSize size = QSize(210, 30);
    this->setMinimumSize(size);
    this->setMaximumSize(size);
    this->setPlaceholderText("Type here to filter videos");
    this->setTextMargins(5, 5, 5, 5);

    // handle text changed (user typed something)
    connect(
        this, SIGNAL( textChanged(const QString&) ),
        this, SLOT( textChanged(const QString&) ));
}

void Searching::textChanged(const QString& text) {
    string filter = text.toStdString();

    // iterate through each button and look at the title
    std::vector<Thumbnail*>::iterator ptr;
    for (ptr = this->buttons->begin(); ptr < this->buttons->end(); ptr++) {
        Thumbnail *button = *ptr;

        QString *title = button->info->title;
        string str     = title->toStdString();

        // if the title contains our filter we want to make it visible
        // otherwise hide it
        int i = str.find(filter);
        // i.e. show video if the title contains our filter text
        button->setVisible(i != std::string::npos);
    }
}
