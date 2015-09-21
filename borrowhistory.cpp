#include "borrowhistory.h"
#include "ui_borrowhistory.h"
#include <QtSql>
#include "tool.h"
#include "usrinformation.h"

BorrowHistory::BorrowHistory(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BorrowHistory)
{
    ui->setupUi(this);
    init();
}

void BorrowHistory::init()
{
        QSqlTableModel *model = new QSqlTableModel(this,Tool::getInstance()->getDb());
        model->setTable("borrow");
        model->setEditStrategy(QSqlTableModel::OnManualSubmit);
        model->select(); //选取整个表的所有行
        model->setFilter("isreturn=1 and rid='"+UsrInformation::getInstance()->id+"';");
        model->removeColumn(model->columnCount()-1);
        model->removeColumn(2);
        model->setHeaderData(0,Qt::Horizontal,"借书编号");
        model->setHeaderData(1,Qt::Horizontal,"图书编号");
        model->setHeaderData(2,Qt::Horizontal,"剩余续借次数");
        model->setHeaderData(3,Qt::Horizontal,"借书日期");
        model->setHeaderData(4,Qt::Horizontal,"归还时间");
        ui->tv1->setModel(model);
        ui->tv1->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

BorrowHistory::~BorrowHistory()
{
    delete ui;
}
