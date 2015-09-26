#include "readerdialog.h"
#include "ui_readerdialog.h"
#include "usrinformation.h"
#include "tool.h"
#include "borrowhistory.h"


ReaderDialog::ReaderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReaderDialog)
{
    ui->setupUi(this);
    init();
}

ReaderDialog::~ReaderDialog()
{
    delete ui;
}

void ReaderDialog::initBookTableView()
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

void ReaderDialog::initBorrowTableView()
{
    QSqlTableModel *model = new QSqlTableModel(this,Tool::getInstance()->getDb());
    model->setTable("borrow");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select(); //选取整个表的所有行
    model->setFilter("isreturn=0");
    model->removeColumn(model->columnCount()-1);
    model->removeColumn(model->columnCount()-1);
    model->removeColumn(2);
    model->setHeaderData(0,Qt::Horizontal,"借书编号");
    model->setHeaderData(1,Qt::Horizontal,"图书编号");
    model->setHeaderData(2,Qt::Horizontal,"剩余续借次数");
    model->setHeaderData(3,Qt::Horizontal,"借书日期");
    model->setHeaderData(4,Qt::Horizontal,"应还时间");
    ui->tv2->setModel(model);
    ui->tv2->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void ReaderDialog::initReader()
{
    QSqlQuery query(Tool::getInstance()->getDb());
    query.exec("select * from reader where rid='"+UsrInformation::getInstance()->id+"';");
    query.next();
    ui->ridLabel->setText(query.value(0).toString());
    ui->nameLabel->setText(query.value(2).toString());
    ui->sexLabel->setText(query.value(3).toString());
    if(query.value(4).toInt()==0){
        ui->typeLabel->setText("普通用户");
    }else{
        ui->typeLabel->setText("高级用户");
    }
    ui->bornum->setText(QString::number(query.value(5).toInt()));
    ui->arrearsLabel->setText(QString::number(query.value(6).toDouble()));
}

void ReaderDialog::init()
{
    ui->pwd1->setEchoMode(QLineEdit::Password);
    ui->pwd2->setEchoMode(QLineEdit::Password);
    ui->pwd3->setEchoMode(QLineEdit::Password);
    ui->pwd1->setMaxLength(20);
    ui->pwd2->setMaxLength(20);
    ui->pwd3->setMaxLength(20);
    initBookTableView();
    initBorrowTableView();
    QSqlQuery query(Tool::getInstance()->getDb());
    query.exec("select classnum from book group by classnum;");
    while(query.next()){
        ui->bclass->addItem(query.value(0).toString());
    }
}

int ReaderDialog::getBookNum(QString bid){
    QSqlQuery query(Tool::getInstance()->getDb());
    query.exec("select nownum from book where bid ='"+bid+"';");
    if(query.next())
    {
        return query.value(0).toInt();
    }
    else{
        return 0;
    }
}

void ReaderDialog::on_borrowBtn_clicked()
{

    int row = ui->tv1->currentIndex().row();
    if(row>=0){
        QModelIndex index = ui->tv1->model()->index(row,0);
        QString bid = ui->tv1->model()->data(index).toString();
        if(!getBookNum(bid)){
            QMessageBox::about(NULL,"借书失败","该书数量不足！");
            return;
        }
        QSqlQuery query(Tool::getInstance()->getDb());
        QDate date = QDate::currentDate();
        QDate redate = date.addDays(30);
        QString botime = date.toString("yyyy-MM-dd");
        QString retime = redate.toString("yyyy-MM-dd");
        int type = UsrInformation::getInstance()->type;
        QString rid = UsrInformation::getInstance()->id;
        int overbor;
        if(type==0)
        {
            overbor =1;
        }
        else{
            overbor = 2;
        }
        query.prepare("insert into borrow(bid,rid,overbor,botime,retime) values(:bid,:rid,:overbor,:botime,:retime)");
        query.bindValue(":bid",bid);
        query.bindValue(":rid",rid);
        query.bindValue(":overbor",overbor);
        query.bindValue(":botime",botime);
        query.bindValue(":retime",retime);
        query.exec();
        query.exec("update book set nownum=nownum-1 where bid='"+bid+"';");
        query.exec("update reader set bornum=bornum+1 where rid='"+rid+"';");
        QMessageBox::about(NULL,"提示","借书成功！");
        initBookTableView();
    }
}

void ReaderDialog::on_queryBtn1_clicked()
{
    QString sql = "select * from book where 1=1 and ";
    QString bname = ui->bnameEdit->text();
    QString author = ui->authorEdit->text();
    QString bclass = ui->bclass->currentText();
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

void ReaderDialog::on_tabWidget_currentChanged(int index)
{
    if(index==0){
        initBookTableView();
    }
    else if(index==1){
        initBorrowTableView();
    }
    else{
        initReader();
    }
}

void ReaderDialog::on_renewBtn_clicked()
{
    int row = ui->tv2->currentIndex().row();
    if(row>=0){
        QModelIndex index = ui->tv2->model()->index(row,0);
        QString boid = ui->tv2->model()->data(index).toString();
        QSqlQuery query(Tool::getInstance()->getDb());
        query.exec("select  overbor from borrow where boid='"+boid+"';");
        query.next();
        int overbor = query.value(0).toInt();
        if(overbor>0){
            query.exec("update borrow set overbor=overbor-1 where boid='"+boid+"';");
            query.exec("update borrow set retime=date(retime,'30 day') where boid='"+boid+"';");
            initBorrowTableView();
            QMessageBox::about(NULL,"提示","续借成功！");
        }
        else{
            QMessageBox::about(NULL,"提示","续借失败，续借次数不足！");
        }
    }
}

void ReaderDialog::on_returnBtn_clicked()
{
    int row = ui->tv2->currentIndex().row();
    if(row>=0){
        QModelIndex index = ui->tv2->model()->index(row,0);
        QString boid = ui->tv2->model()->data(index).toString();
        QSqlQuery query(Tool::getInstance()->getDb());
        query.exec("select * from borrow where boid='"+boid+"';");
        query.next();
        QString bid = query.value(1).toString();
        QString rid = query.value(2).toString();
        QDate date = query.value(5).toDate();
        QDate nowDate = QDate::currentDate();
        int day = date.daysTo(nowDate);
        query.exec("update borrow set isreturn=1,retime='"+nowDate.toString("yyyy-MM-dd")+"' where boid='"+boid+"';");
        query.exec("update book set nownum=nownum+1 where bid='"+bid+"';");
        query.exec("update reader set bornum=bornum-1 where rid='"+rid+"';");
        if(day>0){
            QMessageBox::about(NULL,"提示","逾期还书,将增加欠款记录");
            double money = day*0.1;
            QString tp = QString::number(money);
            query.exec("update reader set arrears=arrears+"+tp+" where rid='"+rid+"';");
        }
        else{
            QMessageBox::about(NULL,"提示","还书成功");
        }
        initBorrowTableView();
    }
}

void ReaderDialog::on_historyBtn_clicked()
{
    BorrowHistory * borrow = new BorrowHistory;
    borrow->show();
}

void ReaderDialog::on_clearBtn_clicked()
{
    ui->pwd1->setText("");
    ui->pwd2->setText("");
    ui->pwd3->setText("");
}

void ReaderDialog::on_submitBtn_clicked()
{
    if(ui->pwd1->text()==""||ui->pwd2->text()==""){
        QMessageBox::about(NULL,"提示","密码不能为空");
        return;
    }
    if(ui->pwd2->text()!=ui->pwd3->text()){
        QMessageBox::about(NULL,"提示","两次密码输入不一样 ");
        return;
    }
    QSqlQuery query(Tool::getInstance()->getDb());
    query.exec("select * from reader where rid='"+UsrInformation::getInstance()->id+"' and pwd='"+ui->pwd1->text()+"';");
    if(query.next()){
        query.exec("update reader set pwd='"+ui->pwd2->text()+"' where rid='"+UsrInformation::getInstance()->id+"';");
        QMessageBox::about(NULL,"提示","修改密码成功");
    }
    else{
        QMessageBox::about(NULL,"提示","密码不正确");
    }
}
