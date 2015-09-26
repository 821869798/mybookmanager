#ifndef ADDREADERDIALOG_H
#define ADDREADERDIALOG_H

#include <QDialog>

namespace Ui {
class AddReaderDialog;
}

class AddReaderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddReaderDialog(QWidget *parent = 0);
    ~AddReaderDialog();

private:
    Ui::AddReaderDialog *ui;
};

#endif // ADDREADERDIALOG_H
