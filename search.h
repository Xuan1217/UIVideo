#ifndef SEARCH__H
#define SEARCH__H

#include <QtWidgets/QLineEdit>
#include <QString>

#include <vector>

class Thumbnail;

class Searching : public QLineEdit {
    Q_OBJECT
private:
    std::vector<Thumbnail*> *buttons;
public:
    Searching(std::vector<Thumbnail*> *buttons);
private slots:
    void textChanged(const QString& text);
};

#endif // SEARCH__H
