#ifndef TOOL_H
#define TOOL_H
#include<QtSql>
#include<QMessageBox>
#include<QStringList>

class Tool
{
public:
    Tool();
    static Tool * getInstance();
    static void setInstace(Tool *value);

    QSqlDatabase getDb() const;


    QStringList getBookClass();
private:
    QStringList bookClass;
    static Tool *instance;
    QSqlDatabase db;
};

#endif // TOOL_H
