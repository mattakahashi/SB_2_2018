/*
//////////////////////////////////////////////////////////////// Classe segunda_passagem /////////////////////////////////////////////////////////////////////////////////////////

Classe responsável pelo desenvolvimento das tabelas de uso, tabela de definições e a tradução do código para código objeto. Recebe como argumento o arquivo pré-processado e pós análise
de erros.

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

*/

#include "Montador.h"
 
 
using namespace std;
 
// Método Construtor da Classe
segunda_passagem::segunda_passagem()
{
 	EQU_FLAG = false;
	nlinha= 0;
	linha_equ = 0;
	IF1 =0;
 
}


//Método Destrutor da Classe
segunda_passagem::~segunda_passagem()
{
    
 
}

// Método de Leitura do arquivo .asm, manipulação deste arquivo e desenvolvimento das tabelas de usos, definições e criação do código objeto.
void segunda_passagem::leitura(string ArquivoPre)
{
    ////////////////////////////////////// Declaração de Variáveis /////////////////////////////////////////////////////////
    string EQU("EQU");
    string IF("IF");
    string STOP("STOP");
    string COPY("COPY");
    string SECTION("SECTION");
    string TEXT("TEXT");
    string DATA("DATA");
    string BEGIN("BEGIN");
    string END("END");
    string PUBLIC("PUBLIC");
    string EXTERN("EXTERN");
    string CONST("CONST");
    string SPACE("SPACE");
    string ADD("ADD");
    string SUB("SUB");
    string MULT("MULT");
    string DIV("DIV");
    string JMP("JMP");
    string JMPN("JMPN");
    string JMPP("JMPP");
    string JMPZ("JMPZ");
    string LOAD("LOAD");
    string STORE("STORE");
    string INPUT("INPUT");
    string OUTPUT("OUTPUT");
    string MACRO("MACRO");
    string ENDMACRO("ENDMACRO");

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /////////////////////////Pega os nomes dos arquivos que foram digitados na execução do programa/////////////////////////
    
    ifstream entrada(ArquivoPre.c_str()); 
    size_t Arq = ArquivoPre.find_first_of("."); 
    ArquivoPre = ArquivoPre.substr(0,Arq);
    ArquivoOBJ = ArquivoPre + ".obj";
    ofstream out(ArquivoOBJ.c_str());
    pre_processamento* pre = new pre_processamento();
    

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
    
    if(entrada.is_open())
    {
        while(getline(entrada,frase))
        {
            if(!frase.empty()) // Analisa se a linha está vazia (Se tiver, ignora esta linha)
            {
                if(frase.empty()) // Analisa se a primeira linha está vazia(Se tiver, continua a execução)
                {
                    continue;
                }

                nlinha++;
                frase = pre->Remover_Comentarios(frase); // Retira comentários das linhas do arquivo
                frase = pre->NaoSensivelAoCaso(frase); // Transforma todas strings para Maiuscula
                //buffer_tokens = pre->pegar_tokens(frase); // Pega os tokens de 1 linha

                
                for(unsigned int i=0;i<buffer_tokens.size();i++)
                {
                    /////////////////// Separa os Tokens no caso do COPY//////////////////////////// Ainda não trata casos do COPY estar escrito diferente do que foi especificado
                    if ((buffer_tokens[i].compare(COPY) == 0))
                    {
                        size_t virgula = buffer_tokens[i+1].find(",");
                        if(virgula != string::npos)
                        {
                            string A = buffer_tokens[i+1].substr(0, virgula); //Primeiro token do copy
                            string B = buffer_tokens[i+1].substr(virgula+1,string::npos); //Segundo token do copy

                            buffer_completo.push_back(buffer_tokens[i]);                            
                            buffer_completo.push_back(A);
                            buffer_completo.push_back(B);
                            i++;

                        }
                    }
                    else 
                    {
                    buffer_completo.push_back(buffer_tokens[i]);    
                    }  
                }                
            }
        }

        ///// Criação da tabela de instruções
            
        map<string, int>tabela_instrucoes;
        tabela_instrucoes.insert(make_pair("ADD",1));
        tabela_instrucoes.insert(make_pair("SUB",2));
        tabela_instrucoes.insert(make_pair("MULT",3));
        tabela_instrucoes.insert(make_pair("DIV",4));
        tabela_instrucoes.insert(make_pair("JMP",5));
        tabela_instrucoes.insert(make_pair("JMPN",6));
        tabela_instrucoes.insert(make_pair("JMPP",7));
        tabela_instrucoes.insert(make_pair("JMPZ",8));
        tabela_instrucoes.insert(make_pair("COPY",9));
        tabela_instrucoes.insert(make_pair("LOAD",10));
        tabela_instrucoes.insert(make_pair("STORE",11));
        tabela_instrucoes.insert(make_pair("INPUT",12));
        tabela_instrucoes.insert(make_pair("OUTPUT",13));
        tabela_instrucoes.insert(make_pair("STOP",14));
        
        map<string,int>tabela_definicoes;
        int valor;                                 // variável para verificar se um string possui ":"
        unsigned int cont_posicoes = 0;
             
        for(unsigned int i=0;i<buffer_completo.size();i++)
        {                                // Atualizando os valores do contador de posições
            if(!buffer_completo[i].empty() && i!=0)
            {
                cont_posicoes = cont_posicoes+1;
                if(buffer_completo[i].compare(CONST)==0)
                {
                    cont_posicoes = cont_posicoes-1;
                }
                if(buffer_completo[i].compare(TEXT)==0 || buffer_completo[i].compare(DATA)==0)
                { // Retirando as duas diretivas SECTION TEXT ou SECTION DATA de posições de memória
                    cont_posicoes = cont_posicoes-2;
                }
            }
                        
            // montando a tabela de definições (tabela com os rótulos e seu endereço)
            
            string a = buffer_completo[i];          // salva o buffer_completo[i] com ":", para depois colocar novamente no buffer.
            valor = buffer_completo[i].find(":");
            
            if(valor != -1)
            {        
                buffer_completo[i].erase(valor); // Apaga ":" dos rótulos para colocar na tabela de definições
                tabela_definicoes.insert(make_pair(buffer_completo[i],cont_posicoes));  
                cont_posicoes = cont_posicoes-1;
                buffer_completo[i] = a; // retoma o ":" para inserir novamente no buffer completo
            }
                        
        }

         /////////////////////////////////// Montando o código ///////////////////////////////////////////// 
            
        for(unsigned int i=0;i<buffer_completo.size();i++){             
            if(tabela_instrucoes.count(buffer_completo[i])>0){      // analisa se o string existe na tabela de instruções.
                out  << tabela_instrucoes[buffer_completo[i]]<< " ";
            }                   
            if(tabela_definicoes.count(buffer_completo[i])>0){
                out  << tabela_definicoes[buffer_completo[i]] << " ";
            }
            if(buffer_completo[i].compare(CONST)==0){
                out << buffer_completo[i+1] << " ";
            }
            if(buffer_completo[i].compare(SPACE)==0){
                out << "0" << " ";
            }
        }       
    }     
}

