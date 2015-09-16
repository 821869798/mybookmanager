#include "readerdialog.h"
#include "ui_readerdialog.h"
#include "usrinformation.h"
#include "tool.h"


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

void ReaderDialog::init()
{
    QSqlTableModel *model = new QSqlTableModel(this,Tool::getInstance()->getDb());
    model->setTable("book");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select(); //选取整个表的所有行
    ui->tv1->setModel(model);
    ui->tv1->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QSqlQuery query(Tool::getInstance()->getDb());
    query.exec("select classnum from book group by classnum;");
    while(query.next()){
        ui->bclass->addItem(query.value(0).toString());
    }
}

void ReaderDialog::on_borrowBtn_clicked()
{
    int row = ui->tv1->currentIndex().row();
    if(row>=0){
        QModelIndex index = ui->tv1->model()->index(row,0);
        QString bid = ui->tv1->model()->data(index).toString();
        QSqlQuery query(Tool::getInstance()->getDb());
        QDateTime time = QDateTime::currentDateTime();
        QString botime = time.toString("yyyy-MM-dd");
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
        query.prepare("insert into borrow(bid,rid,overbor,botime) values(:bid,:rid,:overbor,:botime)");
        query.bindValue(":bid",bid);
        query.bindValue(":rid",rid);
        query.bindValue(":overbor",overbor);
        query.bindValue(":botime",botime);
        query.exec();    }
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
