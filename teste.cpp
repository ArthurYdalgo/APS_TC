#include "teste.h"

teste::teste(estado *squ,estado *sqv, char c)
{
    qu=squ;
    qv=sqv;
    caracter=c;
}

teste::~teste()
{
    //dtor
}

estado *teste::getQu()
{
    return qu;
}

estado *teste::getQv()
{
    return qv;
}

char teste::getChar()
{
    return caracter;
}

