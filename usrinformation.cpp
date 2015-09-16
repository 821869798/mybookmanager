#include "usrinformation.h"

UsrInformation * UsrInformation::instance = NULL;

UsrInformation *UsrInformation::getInstance()
{
    if(instance==NULL){
        instance = new UsrInformation();
    }
    return instance;
}
