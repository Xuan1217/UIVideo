#ifndef USERWINDOW_H
#define USERWINDOW_H

#include <QWidget>

namespace Ui {
class userwindow;
}

class userwindow : public QWidget
{
    Q_OBJECT

public:
    explicit userwindow(QWidget *parent = nullptr);
    ~userwindow();

private:
    Ui::userwindow *ui;
};

#endif // USERWINDOW_H
