#include "estado.h"

estado::estado(int novoNome)
{
    this->nome=novoNome;
    this->Final=false;
    this->Inicial=false;
}

void estado::setNome(int novoNome)
{
    this->nome = novoNome;
}
int estado::getNome()
{
    return this->nome;
}
void estado::SetAsFinal()
{
    this->Final=true;
}
bool estado::IsFinal()
{
    return this->Final;
}

void estado::SetAsInicial()
{
    this->Inicial=true;
}
bool estado::IsInicial()
{
    return this->Inicial;
}


void estado::AddTransicao(char caracter,estado *ProxEstado)
{
    this->transicoes[caracter]=ProxEstado;
}

void estado::DelTransicao(char caracter)
{
    this->transicoes.erase(caracter);
}

bool estado::EhMorto()
{
    int qtdT = transicoes.size();
    int cont=0;
    if(!qtdT)
        return true;
    else
    {
        for (map<char, estado*>::iterator k =transicoes.begin() ; k != transicoes.end() ; k ++ )
        {
            estado *state = k->second;
            if(state==this)
                cont++;
        }
        if(cont==qtdT)
            return true;
    }
    return false;
}

estado * estado::ProximoEstado(char caracter)
{
    if(transicoes.find(caracter)==transicoes.end())
        return nullptr;
    else
        return this->transicoes[caracter];
}


