#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <locale>
#include <stack>
#include <algorithm>

#include "estado.h"
#include "teste.h"

using namespace std;

bool checkChar(char c,vector<char> alf);

bool checkWord(string palavra, estado* state);

void LimparMortos(map<int,estado*> &maquina,vector<char> alf);

int main()
{
    setlocale(LC_ALL,"Portuguese");
    ofstream fOut;
    ifstream fIn;
    int qtdEstados=0, qtdAlfabeto=0, qtdTransicoes=0,qtdFinais=0,qtdPalavras=0;
    int eqCount=0;
    map<int,estado*> maquina;
    map<int,estado*> novaMaquina;
    stack<teste*> testes;
    stack<int> toRemove;
    vector<char> alfabeto;
    vector<string> Testes;
    char caracter='_';
    int state=0,endstate=0;
    string temp,fileName;
    std::stringstream num;
    estado *qu,*qv,*pu,*pv;
    bool tabela[11][11];
    for (int i=0; i<10 ; i++ )
        for (int j=0; j<10 ; j++ )
            tabela[i][j]=false;
    cout << "Insira o nome do arquivo de texto de entrada (não é necessário digitar a extensão \".txt\")"<<endl;
    cin >> fileName;
    fileName.append(".txt");
    fIn.open(fileName);

    if(fIn)
    {
        //cout << "Insira a quantidade de estados (máximo: 10): ";
        //Lê quantidade de estados
        //cin >> qtdEstados;
        fIn >> qtdEstados;
        if(qtdEstados>10||qtdEstados<0)
        {
            cout << "Limite de estados excedido ou inválido."<< endl;
            return 0;
        }
        //Cria estados
        for (int i=0; i<=qtdEstados ; i++ )
            maquina.insert(make_pair(i,new estado(i)));
        maquina[0]->SetAsInicial();

        //cout << "Insira a quantidade de caracteres do alfabeto (maximo: 10), e os mesmos separados por espaços..."<<endl;
        //Lê quantidade de letras no alfabeto
        fIn >> qtdAlfabeto;
        //Confere qtd de letras
        if(qtdAlfabeto>10||qtdAlfabeto<0)
        {
            cout << "Limite do alfabeto excedido ou invalido."<< endl;
            return 0;
        }
        alfabeto.reserve(qtdAlfabeto);

        //Lê letras
        for (int i=0; i<qtdAlfabeto ; i++ )
        {
            fIn >> caracter;
            //cin>>caracter;
            alfabeto.push_back(caracter);
        }
        //cout << "Insira a quantidade de estados finais (maximo: " << qtdEstados << "): ";
        //Lê qnd de estados finais
        //cin >> qtdFinais;
        fIn >> qtdFinais;

        //Confere qtd de estados finais
        if(qtdFinais > qtdEstados)
        {
            cout << "Entrada invalida. Quantidade entrada excede a quantidade de estados."<<endl;
            return 0;
        }
        for (int i=0; i<qtdFinais ; i++ )
        {
            //cin >> state;
            fIn >> state;
            //Confere se estado entrado é valido
            if(state >= qtdEstados || state <0)
            {
                cout << "Entrada invalida." << endl;
                return 0;
            }
            //Declara aquele estado ("state") como final
            maquina[state]->SetAsFinal();
        }

        //cout << "Insira a quantidade de transições (maximo: 50): ";
        //Lê quantidade de transicoes
        //cin >> qtdTransicoes;
        fIn >> qtdTransicoes;
        //getchar();
        //Confere qtd de transicoes
        if(qtdTransicoes>50||qtdTransicoes<0)
        {
            cout << "Limite de transicoes excedido ou invalido."<< endl;
            return 0;
        }
        //cout << "Insira a transicao no seguinte formato: \"[numero] [caracter] [numero]\"" << endl;
        //cout << "Exemplo: 0 a 1"<< endl;
        //Lê transicoes
        for (int i=0; i<qtdTransicoes ; i++ )
        {
            //state = estado de saida
            //endstate = estado de chegada
            //caracter = caracter para a transição
            //scanf("%d %c %d",&state,&caracter,&endstate);
            fIn >> state >> caracter >> endstate;
            //getchar();
            //Confere se palavra se encontra no alfabeto
            if(!checkChar(caracter,alfabeto))
            {
                cout << "Character não presente no alfabeto" << endl;
                return 0;
            }
            //Confere se estados são validos
            if(state >=qtdEstados||state<0||endstate>=qtdEstados||endstate<0)
            {
                cout << "Estado entrado invalido" << endl;
                return 0;
            }
            //Adiciona ao estado "state" a transição para o proximo estado ("endstate")
            maquina[state]->AddTransicao(caracter,maquina[endstate]);
        }

        //Conferir por estados sem transiçoes para alguma(s) letras, e adiciona para um estado morto

        for (unsigned int i=0; i<alfabeto.size() ; i++ )
        {

            maquina[qtdEstados]->AddTransicao(alfabeto.at(i),maquina[qtdEstados]);//faz com o que o estado morto possua todos os caminhos para ele mesmo
            for (int j=0; j<qtdEstados ; j++ )
                if(!maquina[j]->ProximoEstado(alfabeto.at(i)))
                    maquina[j]->AddTransicao(alfabeto.at(i),maquina[qtdEstados]);
        }


        //cout << "Insira a quantidade de palavras a serem testadas (maximo: 10): ";
        //Lê quantidade de palavras
        //cin >> qtdPalavras;
        fIn >> qtdPalavras;
        //Testa limite
        if(qtdPalavras>10||qtdPalavras<0)
        {
            cout << "Limite de palavras atingido ou invalido" << endl;
            return 0;
        }
        Testes.reserve(qtdPalavras);
        //cout << "Insira as palavras, teclando Enter a cada palavra escrita"<<endl;
        //Lê palavras
        for (int i=0; i<qtdPalavras ; i++ )
        {
            //cin >> temp;
            fIn >> temp;
            //Confere limite de tamanho
            if(temp.size()>15)
            {
                cout << "Tamanho maximo da palavra atingido" << endl;
                return 0;
            }
            //Adiciona num vetor de testes
            Testes.push_back(temp);
        }

        //Testa as palavras
        ///VALIDAÇÃO

        fOut.open ("saida.txt");
        for (unsigned int i=0; i<Testes.size() ; i++ )
        {
            cout << Testes.at(i) << " - ";
            //fOut << Testes.at(i) << " - ";
            if(checkWord(Testes.at(i),maquina[0]))
            {
                fOut << "Aceita.\n";
                cout << "Aceita" << endl;
            }
            else
            {
                fOut << "Rejeitada.\n";
                cout << "Rejeitada" << endl;
            }

        }


        ///MINIMIZAÇÃO
        //b) marcação de estados não trivialmente equivalentes
        for (int i=0; i<qtdEstados-1 ; i++ )
            for(int j=0; j<=i; j++)
            {
                //cout << i+1{v} << " - " << j{h}<<endl;
                if((maquina[i+1]->IsFinal() && !maquina[j]->IsFinal())||(!maquina[i+1]->IsFinal() && maquina[j]->IsFinal()))
                    tabela[i][j]=true;
            }

        //c) marcação dos estados nao equivalentes
        for(unsigned int k =0; k<alfabeto.size(); k++)
            for (int i=0; i<qtdEstados ; i++ )
                for(int j=0; j<=i; j++)
                    if(!tabela[i][j])
                    {
                        qu=maquina[i+1];
                        qv=maquina[j];

                        //cout << "qu: " << qu->getNome() << " - qv: " << qv->getNome() << endl;
                        pu = qu->ProximoEstado(alfabeto.at(k));
                        pv = qv->ProximoEstado(alfabeto.at(k));

                        if((pu != pv)&&(!tabela[pu->getNome()-1][pv->getNome()]))//2a opção
                        {
                            testes.push(new teste(qu,qv,alfabeto.at(k)));
                        }
                        if((pu != pv)&&(tabela[pu->getNome()-1][pv->getNome()]))//3a opção
                            tabela[i][j]=true;

                    }

        //refaz os testes empilhados
        while(testes.size())
        {
            teste *testeAtual = testes.top();
            qu = testeAtual->getQu();
            qv = testeAtual->getQv();
            pu = qu->ProximoEstado(testeAtual->getChar());
            pv = qv->ProximoEstado(testeAtual->getChar());
            if(tabela[pu->getNome()-1][pv->getNome()])
            {
                tabela[qu->getNome()-1][qv->getNome()]=true;
            }
            testes.pop();
        }

        //Elimina transiçoes pro estado extra
        for (unsigned int i=0; i<maquina.size()-1; i++ )
            for (unsigned int j=0; j<alfabeto.size() ; j++ )
                if(maquina[i]->ProximoEstado(alfabeto.at(j))->getNome()==qtdEstados)
                    maquina[i]->DelTransicao(alfabeto.at(j));


        //Conta a qtd de estados equivalentes
        for (int i=0; i <qtdEstados-1 ; i++ )
            for (int j=0 ; j<=i ; j++ )
                if(!tabela[i][j])
                    eqCount++;

        //apaga o estado extra adicionado
        maquina.erase(qtdEstados);

        //se ha estados equivalentes
        if(eqCount)
        {
            cout << endl << "->Automato minimizavel"<< endl << endl;
            fOut << endl <<"->Automato minimizavel"<< endl << endl;
            //Automato minimizavel. Eliminar estados mortos
            LimparMortos(maquina,alfabeto);

            //Junção de estados equivalentes
            for (int i=0; i <qtdEstados-1 ; i++ )
                for (int j=0 ; j<=i ; j++ )
                {
                    qu=maquina[i+1];//Estado assimilador
                    qv=maquina[j];//Estado assimilado
                    if(!tabela[i][j]&&qu&&qv)
                    {
                        for (unsigned int k=0; k<alfabeto.size() ; k++ )
                        {
                            ///Casos internos
                            //Se qv aponta pra ele mesmo, qu apontara pra ele mesmo
                            if(qv->ProximoEstado(alfabeto.at(k))==qv)
                                qu->AddTransicao(alfabeto.at(k),qu);
                            //Se qu aponta pra qv, qu apontara para ele mesmo
                            if(qu->ProximoEstado(alfabeto.at(k))==qv)
                                qu->AddTransicao(alfabeto.at(k),qu);
                            //se qv aponta pra qu, qu apontara para ele mesmo
                            if(qv->ProximoEstado(alfabeto.at(k))==qu)
                                qu->AddTransicao(alfabeto.at(k),qu);

                            //se qu nao é inicial, mas qv é inicial
                            if(!qu->IsInicial()&&qv->IsInicial())
                                qu->SetAsInicial();

                            ///Casos externos
                            //Todos que apontavam pra qv agora apontam para qu
                            for (int l=0; l<qtdEstados ; l++ )
                                if(maquina[l])
                                    if((maquina[l]!=qu)&&(maquina[l]!=qv))
                                        if(maquina[l]->ProximoEstado(alfabeto.at(k))==qv)
                                            maquina[l]->AddTransicao(alfabeto.at(k),qu);

                            //Copia todos os caminhos externos pra qu
                            if((qv->ProximoEstado(alfabeto.at(k))!=qu)&&(qv->ProximoEstado(alfabeto.at(k))!=qv))
                                qu->AddTransicao(alfabeto.at(k),qv->ProximoEstado(alfabeto.at(k)));
                        }
                        //adiciona os estados assimilados a uma pilha de remoção
                        toRemove.push(qv->getNome());
                    }
                }

            //Remove os estados assimilados (empilhados) da maquina
            while(toRemove.size())
            {
                maquina.erase(toRemove.top());
                toRemove.pop();
            }

            qtdEstados=0;

            for (const auto & atual : maquina)
            {
                qu = atual.second;
                if(qu)
                    qtdEstados++;
            }
            ///Exportação de automato
            //alfabeto
            fOut << "Alfabeto = (";
            cout << "Alfabeto = (";
            for (unsigned int i=0; i<alfabeto.size() ; i++ )
            {
                fOut << alfabeto.at(i);
                cout << alfabeto.at(i);
                if(i!=alfabeto.size()-1)
                {
                    cout << ", ";
                    fOut << ", ";
                }
            }
            fOut << ")"<< endl;

            //estados e suas transições
            fOut << "Quantidade de estados: " << qtdEstados<<endl;
            cout << ")"<< endl;
            cout << "Quantidade de estados: " << qtdEstados << endl;
            for (map<int, estado*>::iterator atual =maquina.begin() ; atual != maquina.end() ; atual ++ )
            {
                qu = atual->second;
                if(qu)
                {
                    fOut << "Estado :"<<qu->getNome();
                    cout << "Estado :"<<qu->getNome();
                    if(qu->IsFinal())
                    {
                        fOut << " - final"<<endl;
                        cout << " - final"<<endl;
                    }
                    else if(qu->IsInicial())
                    {
                        fOut << " - inicial"<<endl;
                        cout << " - inicial"<<endl;
                    }
                    else
                    {
                        fOut << endl;
                        cout << endl;
                    }

                    for (unsigned int i=0; i<alfabeto.size() ; i++ )
                    {
                        pu = qu->ProximoEstado(alfabeto.at(i));
                        if(pu)
                        {
                            cout << "T("<< qu->getNome()<<","<<alfabeto.at(i)<<")->"<< pu->getNome() << endl;
                            fOut << "T("<< qu->getNome()<<","<<alfabeto.at(i)<<")->"<< pu->getNome() << endl;
                        }
                    }
                }

            }


        }
        else //caso ja esteja no minimo, avisar ao usuario
        {
            fOut << "Seu autômato já está no estado minimo. =D\n";
            cout << "Seu autômato já está no estado minimo. =D\n";
        }

    }else{
        cout << "Arquivo não encontrado =P" << endl;
    }

    fOut.close();
    system("pause");

    return 0;
}

bool checkWord(string palavra, estado* state)
{
    if(palavra.size()!=0)
    {
        char c = palavra.at(0);
        palavra = palavra.substr(1);
        if(state->ProximoEstado(c))
            return checkWord(palavra,state->ProximoEstado(c));
        else
        {
            return false;
        }

    }
    else if(state->IsFinal())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool checkChar(char c,vector<char> alf)
{
    for (unsigned int i=0; i<alf.size() ; i++ )
    {
        if(alf.at(i)==c)
            return true;
    }
    return false;
}

void LimparMortos(map<int,estado*> &maquina,vector<char> alf)
{
    //pilha de estados a serem deletados
    stack<int>Deletar;
    estado *state;

    for (unsigned int i=0; i<maquina.size() ; i++ )
    {
        //se estado nao for final e for morto (realiza teste (olhar na classe) de não possuir saida ou se todas as saidas forem para ele mesmo)
        if(!maquina[i]->IsFinal()&&maquina[i]->EhMorto())
        {
            //adiciona na pilha
            Deletar.push(i);
            //passar por todos os estados, quem possuia transição para estado morto tem a transicao removida
            for (map<int, estado*>::iterator k =maquina.begin() ; k != maquina.end() ; k ++ )
            {
                state = k->second ;
                if(state!=maquina[i])
                    for (unsigned int j=0 ; j<alf.size() ; j++ )
                        if(state->ProximoEstado(alf.at(j))==maquina[i])
                            state->DelTransicao(alf.at(j));
            }
        }
    }

    while(Deletar.size())
    {
        maquina.erase(Deletar.top());
        Deletar.pop();
        LimparMortos(maquina,alf);
    }


}
