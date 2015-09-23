#ifndef ADDBOOKDIALOG_H
#define ADDBOOKDIALOG_H

#include <QDialog>

namespace Ui {
class AddBookDialog;
}

class AddBookDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddBookDialog(QWidget *parent = 0);
    ~AddBookDialog();

private slots:
    void on_submitBtn_clicked();

private:
    Ui::AddBookDialog *ui;
    void init();
};

#endif // ADDBOOKDIALOG_H
