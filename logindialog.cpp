#include "logindialog.h"
#include "ui_logindialog.h"
#include "tool.h"
#include "usrinformation.h"
#include "readerdialog.h"
#include "admindialog.h"
#include <QDebug>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    init();
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::init()
{
    this->setFixedSize(this->width(),this->height());
    ui->le2->setEchoMode(QLineEdit::Password);
    ui->le2->setMaxLength(20);
    ui->le1->setMaxLength(10);
    ui->le2_2->setEchoMode(QLineEdit::Password);
    ui->le2_2->setMaxLength(20);
    ui->le1_2->setMaxLength(10);
    connect(ui->exit_2,&QPushButton::clicked,this,&LoginDialog::on_exit_clicked);
    logoStr1 = QStringLiteral("欢迎广大读者使用本管理系统");
    QTimer *timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&LoginDialog::scrollCaption);
    timer->start(500);
}

void LoginDialog::on_login_clicked()
{
    QString id = ui->le1->text().trimmed();
    QString pwd = ui->le2->text().trimmed();
    if(id=="" || pwd=="")
    {
        QMessageBox::about(NULL,"警告","账号或密码不能为空");
        return;
    }
    QSqlDatabase db = Tool::getInstance()->getDb();
    QSqlQuery query(db);
    QString sql = "select * from reader where rid=:id and pwd=:pwd;";
    query.prepare(sql);
    query.bindValue(":id",id);
    query.bindValue(":pwd",pwd);
    query.exec();
    if(query.next()){
        UsrInformation::getInstance()->id = query.value(0).toString();
        UsrInformation::getInstance()->type = query.value(4).toInt(0);
        UsrInformation::getInstance()->borrowNum = query.value(5).toInt(0);
        ReaderDialog *rd = new ReaderDialog;
        rd->show();
        this->close();
    }
    else{
        QMessageBox::about(NULL,"提示","账号或密码错误");
        ui->le2->setText("");
    }
}

void LoginDialog::on_exit_clicked()
{
    this->close();
}

void LoginDialog::scrollCaption()
{
        static int nPos = 0;
        if (nPos >logoStr1.length() )
        {
            nPos = 0;
        }
        ui->logo1->setText(logoStr1.mid(nPos));
        nPos++;
}

void LoginDialog::on_login_2_clicked()
{
    QString id = ui->le1_2->text().trimmed();
    QString pwd = ui->le2_2->text().trimmed();
    if(id=="" || pwd=="")
    {
        QMessageBox::about(NULL,"警告","账号或密码不能为空");
        return;
    }
    QSqlDatabase db = Tool::getInstance()->getDb();
    QSqlQuery query(db);
    QString sql = "select * from root where rootid=:id and rootpwd=:pwd;";
    query.prepare(sql);
    query.bindValue(":id",id);
    query.bindValue(":pwd",pwd);
    query.exec();
    if(query.next()){
        AdminDialog *adminDialog = new AdminDialog;
        adminDialog->show();
        this->close();
    }
    else{
        QMessageBox::about(NULL,"提示","账号或密码错误");
        ui->le2_2->setText("");
    }
}
