#include "tempquerydialog.h"
#include "ui_tempquerydialog.h"
#include <QtSql>

TempQueryDialog::TempQueryDialog(QString sql,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TempQueryDialog)
{
    ui->setupUi(this);
    init(sql);
}

TempQueryDialog::~TempQueryDialog()
{
    delete ui;
}

void TempQueryDialog::init(QString sql)
{
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery(sql);
    ui->tv->setModel(model);
}
