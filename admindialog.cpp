#include "admindialog.h"
#include "ui_admindialog.h"
#include "tool.h"
#include "usrinformation.h"
#include "addbookdialog.h"
#include "tempquerydialog.h"
#include "addreaderdialog.h"
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

void AdminDialog::initReaderTableView()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("select rid as 读者编号,name as 姓名,sex as 性别,case type when 0 then '普通用户' else '高级用户' end as 类型,bornum as 借阅数量,arrears as 欠款金额 from reader;");
    ui->tv2->setModel(model);
}

void AdminDialog::initBorrowTableView()
{
    QSqlTableModel *model = new QSqlTableModel(this,Tool::getInstance()->getDb());
    model->setTable("borrow");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select(); //选取整个表的所有行
    model->setFilter("isreturn=0");
    model->removeColumn(model->columnCount()-1);
    model->removeColumn(model->columnCount()-1);
    model->setHeaderData(0,Qt::Horizontal,"借阅编号");
    model->setHeaderData(1,Qt::Horizontal,"图书编号");
    model->setHeaderData(2,Qt::Horizontal,"读者编号");
    model->setHeaderData(3,Qt::Horizontal,"剩余续借次数");
    model->setHeaderData(4,Qt::Horizontal,"借阅时间");
    model->setHeaderData(5,Qt::Horizontal,"应还时间");
    ui->tv3->setModel(model);
    ui->tv3->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void AdminDialog::init()
{
    if(UsrInformation::getInstance()->type==0)
    {
        ui->deleteBookBtn->setDisabled(true);
        ui->readerUpgrade->setDisabled(true);
        ui->borrowAddBtn->setDisabled(true);
    }
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
    initReaderTableView();
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
    initBookTableView();
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
    model->removeColumn(model->columnCount()-1);
    model->removeColumn(model->columnCount()-1);
    model->setHeaderData(0,Qt::Horizontal,"借阅编号");
    model->setHeaderData(1,Qt::Horizontal,"图书编号");
    model->setHeaderData(2,Qt::Horizontal,"读者编号");
    model->setHeaderData(3,Qt::Horizontal,"剩余续借次数");
    model->setHeaderData(4,Qt::Horizontal,"借阅时间");
    model->setHeaderData(5,Qt::Horizontal,"应还时间");
    ui->tv3->setModel(model);
}

void AdminDialog::on_dateQueryBtn_clicked()
{
    QString sql = "select * from borrow where isreturn=0 and ";
    QString boid = ui->boidEdit->text();
    QString bid = ui->bobidEdit->text();
    QString rid = ui->boridEdit->text();
    QString date1 = ui->botimeEdit1->date().toString("yyyy-MM-dd");
    QString date2 = ui->botimeEdit2->date().toString("yyyy-MM-dd");
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
    sql += "botime>'"+date1+"' and botime<'"+date2+"' and ";
    sql =  sql.left(sql.size()-5);
    QSqlQueryModel*model = new QSqlQueryModel;
    model->setQuery(sql);
    model->removeColumn(model->columnCount()-1);
    model->removeColumn(model->columnCount()-1);
    model->setHeaderData(0,Qt::Horizontal,"借阅编号");
    model->setHeaderData(1,Qt::Horizontal,"图书编号");
    model->setHeaderData(2,Qt::Horizontal,"读者编号");
    model->setHeaderData(3,Qt::Horizontal,"剩余续借次数");
    model->setHeaderData(4,Qt::Horizontal,"借阅时间");
    model->setHeaderData(5,Qt::Horizontal,"应还时间");
    ui->tv3->setModel(model);
}

void AdminDialog::on_pushButton_2_clicked()
{
    QString date = QDate::currentDate().toString("yyyy-MM-dd");
    QSqlQueryModel *model = new QSqlQueryModel;
    QString sql = "select boid,bid,rid,overbor,botime,retime from borrow where isreturn=0 and retime<'"+date+"';";
    model->setQuery(sql);
    model->setHeaderData(0,Qt::Horizontal,"借阅编号");
    model->setHeaderData(1,Qt::Horizontal,"图书编号");
    model->setHeaderData(2,Qt::Horizontal,"读者编号");
    model->setHeaderData(3,Qt::Horizontal,"剩余续借次数");
    model->setHeaderData(4,Qt::Horizontal,"借阅时间");
    model->setHeaderData(5,Qt::Horizontal,"应还时间");
    ui->tv3->setModel(model);
}

void AdminDialog::on_loseBookBtn_clicked()
{
    int row = ui->tv3->currentIndex().row();
    if(row>=0){
        QModelIndex index = ui->tv3->model()->index(row,1);
        QModelIndex index1 = ui->tv3->model()->index(row,2);
        QModelIndex index2 = ui->tv3->model()->index(row,0);
        QString boid = ui->tv3->model()->data(index2).toString();
        QString bid = ui->tv3->model()->data(index).toString();
        QString rid = ui->tv3->model()->data(index1).toString();
        QSqlQuery query(Tool::getInstance()->getDb());
        query.exec("select price from book where bid='"+bid+"';");
        query.next();
        QString price = QString::number(query.value(0).toDouble()*2);
        query.exec("update book set allnum=allnum-1 where bid='"+bid+"';");
        query.exec("update borrow set isreturn=1,islose=1 where boid="+boid);
        query.exec("update reader set arrears=arrears+"+price+" where rid='"+rid+"';");
        QMessageBox::about(NULL,"提示","添加图书遗失成功");
        initBorrowTableView();
    }
}

void AdminDialog::on_bookReaderBtn_clicked()
{
    int row = ui->tv1->currentIndex().row();
    if(row>=0){
        QModelIndex index = ui->tv1->model()->index(row,0);
        QString bid = ui->tv1->model()->data(index).toString();
        QString sql = "select reader.rid as 读者编号,reader.name as 读者姓名,reader.sex as性别,reader.bornum as  借书数量 from reader,borrow,book where book.bid=borrow.bid and reader.rid=borrow.rid and book.bid='"+bid+"';";
        TempQueryDialog *tempquery = new TempQueryDialog(sql);
        tempquery->show();
    }

}

void AdminDialog::on_loseQueryBtn_clicked()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("select borrow.boid as 借阅编号,book.bid as 图书编号,book.bname as 书名,reader.rid as 读者编号,reader.name as 读者姓名 from book,borrow,reader where book.bid=borrow.bid and reader.rid=borrow.rid and islose=1");
    ui->tv3->setModel(model);
}

void AdminDialog::on_borrowAddBtn_clicked()
{
    QAbstractItemModel *model = ui->tv3->model();
    for(int i=0;i<model->rowCount();i++)
    {
        QModelIndex index = ui->tv3->model()->index(i,0);
        QString boid = ui->tv3->model()->data(index).toString();
        QSqlQuery query(Tool::getInstance()->getDb());
        query.exec("update borrow set overbor=overbor+1 where boid="+boid);
    }
    if(model->rowCount()>0){
        QMessageBox::about(NULL,"提示","增加续借次数成功！");
        initBorrowTableView();
    }
}

void AdminDialog::on_tabWidget_currentChanged(int index)
{
    if(index==0){
        initBookTableView();
    }
    else if(index==1){
        initReaderTableView();
    }
    else{
        initBorrowTableView();
    }
}

void AdminDialog::on_readerQueryBtn_clicked()
{
    QString sql = "select rid as 读者编号,name as 姓名,sex as 性别,case type when 0 then '普通用户' else '高级用户' end as 类型,bornum as 借阅数量,arrears as 欠款金额 from reader where 1=1 and ";
    QString rid = ui->ridEdit->text();
    QString rname = ui->rnameEdit->text();
    QString rtype = ui->rtypeEdit->currentText();
    if(rid!="")
    {
        sql += "rid='"+rid+"' and ";
    }
    if(rname!="")
    {
        sql += "name like '%"+rname+"%' and ";
    }
    if(rtype!="无")
    {
        if(rtype=="普通用户")
        {
            sql += "type=0 and ";
        }
        else
        {
            sql += "type>0 and ";
        }
    }
    sql =  sql.left(sql.size()-5);
    QSqlQueryModel*model = new QSqlQueryModel;
    model->setQuery(sql);
    ui->tv2->setModel(model);
}

void AdminDialog::on_arrearsQueryBtn_clicked()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("select rid as 读者编号,name as 姓名,sex as 性别,case type when 0 then '普通用户' else '高级用户' end as 类型,bornum as 借阅数量,arrears as 欠款金额 from reader where arrears>0;");
    ui->tv2->setModel(model);
}

void AdminDialog::on_clearReaderQuery_clicked()
{
    initReaderTableView();
}

void AdminDialog::on_arrearsQueryBtn_2_clicked()
{
    int row = ui->tv2->currentIndex().row();
    if(row>=0){
        QModelIndex index = ui->tv2->model()->index(row,0);
        QString rid = ui->tv2->model()->data(index).toString();
        QSqlQuery query(Tool::getInstance()->getDb());
        query.exec("update reader set arrears=0 where rid='"+rid+"';");
        QMessageBox::about(NULL,"提示","清除成功");
        initReaderTableView();
    }
}

void AdminDialog::on_alreadyBorrowQuery_clicked()
{
    int row = ui->tv2->currentIndex().row();
    if(row>=0){
        QModelIndex index = ui->tv2->model()->index(row,0);
        QString rid = ui->tv2->model()->data(index).toString();
        QString sql = "select borrow.boid as 借阅编号,book.bid as 图书编号,book.bname as 书名,book.author as 作者,book.classnum as 分类,borrow.botime as 借阅时间 from reader,borrow,book where book.bid=borrow.bid and reader.rid=borrow.rid and borrow.isreturn=0 and reader.rid='"+rid+"';";
        TempQueryDialog *tempquery = new TempQueryDialog(sql);
        tempquery->show();
    }
}

void AdminDialog::on_readerUpgrade_clicked()
{
    int row = ui->tv2->currentIndex().row();
    if(row>=0){
        QModelIndex index = ui->tv2->model()->index(row,0);
        QString rid = ui->tv2->model()->data(index).toString();
        QSqlQuery query(Tool::getInstance()->getDb());
        query.exec("select type from reader where rid='"+rid);
        query.next();
        int type = query.value(0).toInt();
        if(type==0)
        {
            query.exec("update reader set type=1 where rid='"+rid+"';");
            qDebug()<<query.lastQuery();
            QMessageBox::about(NULL,"提示","用户升级成功");
            initReaderTableView();
        }
        else{
            QMessageBox::about(NULL,"提示","已是高级用户");
        }
    }
}

void AdminDialog::on_clearReaderEdit_clicked()
{
    ui->ridEdit->setText("");
    ui->rnameEdit->setText("");
    ui->rtypeEdit->setCurrentIndex(0);
}


void AdminDialog::on_addReaderBtn_clicked()
{
    AddReaderDialog *addreader = new AddReaderDialog;
    addreader->show();
}

void AdminDialog::on_losepasswdBtn_clicked()
{
    int row = ui->tv2->currentIndex().row();
    if(row>=0){
        QModelIndex index = ui->tv2->model()->index(row,0);
        QString rid = ui->tv2->model()->data(index).toString();
        QSqlQuery query(Tool::getInstance()->getDb());
        query.exec("update reader set pwd='123456' where rid='"+rid+"';");
        QMessageBox::about(NULL,"提示","密码初始化成功");
    }
}

void AdminDialog::on_deleteBookBtn_clicked()
{
    int row = ui->tv1->currentIndex().row();
    if(row>=0){
        QMessageBox::StandardButton rb = QMessageBox::information(NULL, "警告", "是否删除该书,该操作不可还原", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if(rb == QMessageBox::Yes)
        {
            QModelIndex index = ui->tv1->model()->index(row,0);
            QString bid = ui->tv1->model()->data(index).toString();
            QSqlQuery query(Tool::getInstance()->getDb());
            QSqlQuery query1(Tool::getInstance()->getDb());
            query.exec("PRAGMA foreign_keys = ON");
            query.exec("select reader.rid,count(*) from borrow,book,reader where borrow.bid=book.bid and borrow.rid=reader.rid and borrow.isreturn=0 and borrow.bid='"+bid+"' group by borrow.bid,borrow.rid;");
            while(query.next()){
                QString rid = query.value(0).toString();
                int num = query.value(1).toInt();
                query1.exec("update reader set bornum=bornum-"+QString::number(num)+" where rid='"+rid+"';");
            }
            query.exec("delete from book where bid='"+bid+"';");
            QMessageBox::about(NULL,"提示","删除该书成功");
            initBookTableView();
        }
    }
}
