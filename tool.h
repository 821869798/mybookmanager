#ifndef TOOL_H
#define TOOL_H
#include<QtSql>
#include<QMessageBox>

class Tool
{
public:
    Tool();
    static Tool * getInstance();
    static void setInstace(Tool *value);

    QSqlDatabase getDb() const;

private:
    static Tool *instance;
    QSqlDatabase db;
};

#endif // TOOL_H
