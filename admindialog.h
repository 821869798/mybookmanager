#ifndef ADMINDIALOG_H
#define ADMINDIALOG_H

#include <QDialog>

namespace Ui {
class AdminDialog;
}

class AdminDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AdminDialog(QWidget *parent = 0);
    ~AdminDialog();

private slots:
    void on_queryBtn1_clicked();

    void on_clearBtn_clicked();

    void on_submitBtn1_clicked();

    void on_updateBtn_clicked();

    void on_addBookBtn_clicked();

    void on_clearBtn_2_clicked();

    void on_clearQueryBtn_clicked();

    void on_bqueryBtn_clicked();

private:
    Ui::AdminDialog *ui;
    void init();
    void initBookTableView();
    void initBorrowTableView();
};

#endif // ADMINDIALOG_H
