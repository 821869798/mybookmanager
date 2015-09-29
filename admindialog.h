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

    void on_dateQueryBtn_clicked();

    void on_pushButton_2_clicked();

    void on_loseBookBtn_clicked();

    void on_bookReaderBtn_clicked();

    void on_loseQueryBtn_clicked();

    void on_borrowAddBtn_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_readerQueryBtn_clicked();

    void on_arrearsQueryBtn_clicked();

    void on_clearReaderQuery_clicked();

    void on_arrearsQueryBtn_2_clicked();

    void on_alreadyBorrowQuery_clicked();

    void on_readerUpgrade_clicked();

    void on_clearReaderEdit_clicked();

    void on_addReaderBtn_clicked();

    void on_losepasswdBtn_clicked();

    void on_deleteBookBtn_clicked();

    void on_informationBtn_clicked();

private:
    Ui::AdminDialog *ui;
    void init();
    void initBookTableView();
    void initBorrowTableView();
    void initReaderTableView();
};

#endif // ADMINDIALOG_H
