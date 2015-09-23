#include "tool.h"
Tool *Tool::instance =  NULL;

Tool::Tool()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("book");
    db.open();
    bookClass.append("IT技术");
    bookClass.append("科学");
    bookClass.append("文学");
    bookClass.append("儿童");
    bookClass.append("杂志");
    bookClass.append("哲学");
    bookClass.append("法律政治");
    bookClass.append("医药");
    bookClass.append("经济");
    bookClass.append("教育");
}

Tool *Tool::getInstance()
{
    if(instance==NULL)
    {
        instance = new Tool();
    }
    return instance;
}
QSqlDatabase Tool::getDb() const
{
    return db;
}

QStringList Tool::getBookClass()
{
    return this->bookClass;
}

