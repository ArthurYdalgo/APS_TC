#ifndef ESTADO_H
#define ESTADO_H
#include <map>

using namespace std;

class estado
{
    public:
        estado(int novoNome);
        void setNome(int novoNome);
        int getNome();
        void SetAsFinal();
        bool IsFinal();
        void SetAsInicial();
        bool IsInicial();
        void AddTransicao(char caracter,estado *ProxEstado);
        void DelTransicao(char caracter);
        bool EhMorto();
        int qtdSaidas()
        {
            return transicoes.size();
        }
        estado * ProximoEstado(char caracter);
    private:
        int nome;
        map<char,estado*> transicoes;
        bool Final;
        bool Inicial;

};

#endif // ESTADO_H
