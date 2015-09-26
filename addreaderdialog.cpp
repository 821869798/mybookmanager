#include "addreaderdialog.h"
#include "ui_addreaderdialog.h"

AddReaderDialog::AddReaderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddReaderDialog)
{
    ui->setupUi(this);
}

AddReaderDialog::~AddReaderDialog()
{
    delete ui;
}
