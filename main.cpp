#include "logindialog.h"
#include "readerdialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginDialog login;
    login.show();
//    ReaderDialog *rd = new ReaderDialog();
//    rd->show();
    return a.exec();
}
