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

    void on_tabWidget_currentChanged(int index);

    void on_renewBtn_clicked();

    void on_returnBtn_clicked();

    void on_historyBtn_clicked();

    void on_clearBtn_clicked();

    void on_submitBtn_clicked();

private:
    Ui::ReaderDialog *ui;
    void init();
    int getBookNum(QString bid);
    void initBookTableView();
    void initBorrowTableView();
    void initReader();
};

#endif // READERDIALOG_H
