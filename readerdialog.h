#ifndef READERDIALOG_H
#define READERDIALOG_H

#include <QDialog>

namespace Ui {
class ReaderDialog;
}

class ReaderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReaderDialog(QWidget *parent = 0);
    ~ReaderDialog();

private slots:
    void on_borrowBtn_clicked();

    void on_queryBtn1_clicked();

private:
    Ui::ReaderDialog *ui;
    void init();
};

#endif // READERDIALOG_H
