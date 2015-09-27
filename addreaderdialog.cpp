#include "addreaderdialog.h"
#include "ui_addreaderdialog.h"
#include "tool.h"

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

void AddReaderDialog::on_submitBtn_clicked()
{
    QString rid = ui->ridEdit->text();
    QString rname = ui->rnameEdit->text();
    QString sex;
    if(ui->sex_man->isChecked())
    {
        sex = "男";
    }
    else
    {
        sex = "女";
    }
    int type = ui->typeEdit->currentIndex();
    QSqlQuery query(Tool::getInstance()->getDb());
    query.prepare("insert into reader(rid,name,sex,type) values(:rid,:rname,:sex,:type)");
    query.bindValue(":rid",rid);
    query.bindValue(":rname",rname);
    query.bindValue(":sex",sex);
    query.bindValue(":type",type);
    bool flag = query.exec();
    if(flag){
        QMessageBox::about(NULL,"提示","添加成功");
    }
    else{
        QMessageBox::about(NULL,"提示","添加失败");
    }
}
