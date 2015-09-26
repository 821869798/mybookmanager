#include "addbookdialog.h"
#include "ui_addbookdialog.h"
#include "tool.h"
#include <QtSql>
#include <QMessageBox>

AddBookDialog::AddBookDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddBookDialog)
{
    ui->setupUi(this);
    init();
}

void AddBookDialog::init()
{
    ui->bnum->setValidator(new QIntValidator(0,1000,this));
    ui->bprice->setValidator(new QDoubleValidator(0,1000 ,2,this));
    QSqlQuery query(Tool::getInstance()->getDb());
    query.exec("select classnum from book group by classnum;");
    while(query.next()){
        ui->bclass->addItem(query.value(0).toString());
    }
}

AddBookDialog::~AddBookDialog()
{
    delete ui;
}

void AddBookDialog::on_submitBtn_clicked()
{
    QString bid = ui->bid->text();
    QString bname = ui->bname->text();
    QString bauthor = ui->bauthor->text();
    QString bpress = ui->bpress->text();
    QString pub = ui->pub->date().toString("yyyy-MM-dd");
    QString bprice = QString::number(ui->bprice->text().toDouble());
    QString bclass = ui->bclass->currentText();
    int num = ui->bnum->text().toInt();
    if(bid==""||bname==""||bauthor  ==""||bpress=="")
    {
        QMessageBox::about(NULL,"提示","不可为空");
        return ;
    }
    if(num<=0)
    {
        QMessageBox::about(NULL,"提示","图书数量错误！");
    }
    QSqlQuery query(Tool::getInstance()->getDb());
    query.prepare("insert into book values(:bid,:bname,:author,:press,:pub,:price,:bclass,:allnum,:nownum);");
    query.bindValue(":bid",bid);
    query.bindValue(":bname",bname);
    query.bindValue(":author",bauthor);
    query.bindValue(":press",bpress);
    query.bindValue(":pub",pub);
    query.bindValue(":price",bprice);
    query.bindValue(":bclass",bclass);
    query.bindValue(":allnum",num);
    query.bindValue(":nownum",num);
    bool flag = query.exec();
    if(flag){
        QMessageBox::about(NULL,"提示","添加成功");
    }
    else{
        QMessageBox::about(NULL,"提示"," 添加失败");
    }
}
