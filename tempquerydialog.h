#ifndef TEMPQUERYDIALOG_H
#define TEMPQUERYDIALOG_H

#include <QDialog>

namespace Ui {
class TempQueryDialog;
}

class TempQueryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TempQueryDialog(QString sql,QWidget *parent = 0);
    ~TempQueryDialog();

private:
    Ui::TempQueryDialog *ui;
    void init(QString);
};

#endif // TEMPQUERYDIALOG_H
