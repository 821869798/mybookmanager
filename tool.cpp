#include "tool.h"
Tool *Tool::instance =  NULL;

Tool::Tool()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("book");
    db.open();
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



