#ifndef USRINFORMATION_H
#define USRINFORMATION_H
#include <QString>

class UsrInformation
{
public:
    static UsrInformation * getInstance();
    QString id;
    int type;
    int borrowNum;
private:
    static UsrInformation *instance;
};

#endif // USRINFORMATION_H
