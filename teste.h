#ifndef TESTE_H
#define TESTE_H

#include "estado.h"

class teste
{
    public:
        teste(estado *squ,estado *sqv, char c);
        ~teste();
        estado *getQu();
        estado *getQv();
        char getChar();
    protected:

    private:
        estado *qu,*qv;
        char caracter;
};

#endif // TESTE_H
