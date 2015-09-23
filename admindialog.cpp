#include "admindialog.h"
#include "ui_admindialog.h"
#include "tool.h"
#include "usrinformation.h"
#include "addbookdialog.h"
#include <QtSql>

AdminDialog::AdminDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdminDialog)
{
    ui->setupUi(this);
    init();
}

void AdminDialog::initBookTableView()
{
    QSqlTableModel *model = new QSqlTableModel(this,Tool::getInstance()->getDb());
    model->setTable("book");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select(); //选取整个表的所有行
    model->setHeaderData(0,Qt::Horizontal,"图书编号");
    model->setHeaderData(1,Qt::Horizontal,"书名");
    model->setHeaderData(2,Qt::Horizontal,"作者");
    model->setHeaderData(3,Qt::Horizontal,"出版社");
    model->setHeaderData(4,Qt::Horizontal,"出版日期");
    model->setHeaderData(5,Qt::Horizontal,"价格");
    model->setHeaderData(6,Qt::Horizontal,"类型");
    model->setHeaderData(7,Qt::Horizontal,"总数量");
    model->setHeaderData(8,Qt::Horizontal,"在馆数");
    ui->tv1->setModel(model);
    ui->tv1->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void AdminDialog::initBorrowTableView()
{
    QSqlTableModel *model = new QSqlTableModel(this,Tool::getInstance()->getDb());
    model->setTable("borrow");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select(); //选取整个表的所有行
    model->removeColumn(model->columnCount()-1);
    model->setHeaderData(0,Qt::Horizontal,"借阅编号");
    model->setHeaderData(1,Qt::Horizontal,"图书编号");
    model->setHeaderData(2,Qt::Horizontal,"读者编号");
    model->setHeaderData(3,Qt::Horizontal,"剩余续借次数");
    model->setHeaderData(4,Qt::Horizontal,"借阅时间");
    model->setHeaderData(5,Qt::Horizontal,"应还时间");
    model->setFilter("isreturn=0");
    ui->tv3->setModel(model);
    ui->tv3->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void AdminDialog::init()
{
    ui->bidAdd->setDisabled(true);
    ui->bnumAdd->setValidator(new QIntValidator(0,1000,this));
    ui->priceAdd->setValidator(new QDoubleValidator(0,1000 ,2,this));
    ui->bclassAdd->addItems(Tool::getInstance()->getBookClass());
    QSqlQuery query(Tool::getInstance()->getDb());
    query.exec("select classnum from book group by classnum;");
    while(query.next()){
        ui->bclass->addItem(query.value(0).toString());
    }
    initBookTableView();
    initBorrowTableView();
}

AdminDialog::~AdminDialog()
{
    delete ui;
}

void AdminDialog::on_queryBtn1_clicked()
{
    QString sql = "select * from book where 1=1 and ";
    QString bname = ui->bnameEdit->text();
    QString author = ui->authorEdit->text();
    QString bclass = ui->bclass->currentText();
    QString bid = ui->bidEdit->text();
    if(bid!="")
    {
        sql += "bid like '"+bid+"%' and ";
    }
    if(bname!="")
    {
        sql += "bname like '%"+bname+"%' and ";
    }
    if(author!="")
    {
        sql += "author like '%"+author+"%' and ";
    }
    if(bclass!="无")
    {
        sql += "classnum like '%"+bclass+"%' and ";
    }
   sql =  sql.left(sql.size()-5);
    QSqlQueryModel*model = new QSqlQueryModel;
    model->setQuery(sql);
    ui->tv1->setModel(model);
}

void AdminDialog::on_clearBtn_clicked()
{
    ui->bidEdit->setText("");
    ui->bnameEdit->setText("");
    ui->authorEdit->setText("");
    ui->bclass->setCurrentIndex(0);
}

void AdminDialog::on_submitBtn1_clicked()
{
    QString bid = ui->bidAdd->text();
    if(bid==""){
        QMessageBox::about(NULL,"警告","你未选中图书");
        return;
    }
    QString name = ui->bnameAdd->text();
    QString author = ui->bauthorAdd->text();
    QString press = ui->bpressAdd->text();
    QString pub = ui->bpubAdd->date().toString("yyyy-MM-dd");
    double price = ui->priceAdd->text().toDouble(0);
    QString bclass = ui->bclassAdd->currentText();
    int addNum = ui->bnumAdd->text().toInt();
   QSqlQuery query(Tool::getInstance()->getDb());
   QString sql = "update book set bname='"+name+
           "',author='"+author+
           "',press='"+press+
           "',pub='"+pub+
           "',price="+QString::number(price)+
           ",classnum='"+bclass+
           "',allnum=allnum+"+QString::number(addNum)+
           ",nownum=nownum+"+QString::number(addNum)+
           " where bid='"+bid+"';";
   query.exec(sql);
   initBookTableView();
   QMessageBox::about(NULL,"提示","修改成功");
}

void AdminDialog::on_updateBtn_clicked()
{
    int row = ui->tv1->currentIndex().row();
    if(row>=0){
        QModelIndex index = ui->tv1->model()->index(row,0);
        QString bid = ui->tv1->model()->data(index).toString();
        QSqlQuery query(Tool::getInstance()->getDb());
        query.exec("select * from book where bid='"+bid+"';");
        query.next();
        ui->bidAdd->setText(bid);
        ui->bnameAdd->setText(query.value(1).toString());
        ui->bauthorAdd->setText(query.value(2).toString());
        ui->bpressAdd->setText(query.value(3).toString());
        ui->bpubAdd->setDate(query.value(4).toDate());
        ui->priceAdd->setText(QString::number(query.value(5).toDouble()));
        int bclass = ui->bclassAdd->findText(query.value(6).toString());
        ui->bclassAdd->setCurrentIndex(bclass);
        ui->bnumAdd->setText("0");
    }
    else
    {
        QMessageBox::about(NULL,"提示","没有选中图书");
    }
}

void AdminDialog::on_addBookBtn_clicked()
{
    AddBookDialog *addbook = new AddBookDialog;
    addbook->show();
}

void AdminDialog::on_clearBtn_2_clicked()
{
    ui->boidEdit->setText("");
    ui->bobidEdit->setText("");
    ui->boridEdit->setText("");
}

void AdminDialog::on_clearQueryBtn_clicked()
{
    initBorrowTableView();
}

void AdminDialog::on_bqueryBtn_clicked()
{
    QString sql = "select * from borrow where isreturn=0 and ";
    QString boid = ui->boidEdit->text();
    QString bid = ui->bobidEdit->text();
    QString rid = ui->boridEdit->text();
    if(boid!="")
    {
        sql += "boid like '%"+boid+"%' and ";
    }
    if(bid!="")
    {
        sql += "bid like '%"+bid+"%' and ";
    }
    if(rid!="")
    {
        sql += "rid like '%"+rid+"%' and ";
    }
    sql =  sql.left(sql.size()-5);
    QSqlQueryModel*model = new QSqlQueryModel;
    model->setQuery(sql);
    ui->tv3->setModel(model);
}
