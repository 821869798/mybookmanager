#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

private slots:
    void init();

    void on_login_clicked();

    void on_exit_clicked();
    void scrollCaption();

    void on_login_2_clicked();

private:
    Ui::LoginDialog *ui;
    QString logoStr1;
};

#endif // LOGINDIALOG_H
