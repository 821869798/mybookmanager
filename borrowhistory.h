#ifndef BORROWHISTORY_H
#define BORROWHISTORY_H

#include <QDialog>

namespace Ui {
class BorrowHistory;
}

class BorrowHistory : public QDialog
{
    Q_OBJECT

public:
    explicit BorrowHistory(QWidget *parent = 0);
    ~BorrowHistory();

private:
    Ui::BorrowHistory *ui;
    void init();
};

#endif // BORROWHISTORY_H
