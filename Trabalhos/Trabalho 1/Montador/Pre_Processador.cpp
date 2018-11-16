/*
//////////////////////////////////////////////////////////////// Classe pre_processamento /////////////////////////////////////////////////////////////////////////////////////////

Classe responsável pelo pre-processamento do arquivo .asm. Recebe como argumento o nome deste arquivo, o abre e faz a leitura de linha por linha. Retira os comentários do arquivo,
deixa todos caracteres em letra maiúscula (Montador não sensível ao caso), separa os caracteres em tokens, detecta se há alguma diretiva EQU e IF e as trata,
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

*/

#include "Montador.h"
 
 
using namespace std;
 
// Método Construtor da Classe
pre_processamento::pre_processamento()
{

    ERRO_FLAG = false;
    ERRO_FLAG_EQU = false;
    ERRO_FLAG_1 = false;
    ERRO_FLAG_IF = false;
	nlinha= 0;
 
}


//Método Destrutor da Classe
pre_processamento::~pre_processamento()
{
    
 
}

// Método de Leitura do arquivo .asm, manipulação deste arquivo e tratamento das diretivas IF e EQU
void pre_processamento::leitura(string ArquivoEntrada)
{
    ////////////////////////////////////// Declaração de Variáveis /////////////////////////////////////////////////////////
    string EQU("EQU");
    string IF("IF");
    string STOP("STOP");
    string COPY("COPY");
    string SECTION("SECTION");
    string TEXT("TEXT");
    string DATA("DATA");
    string BSS("BSS");
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
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /////////////////////////Pega os nomes dos arquivos que foram digitados na execução do programa/////////////////////////
    
    ArquivoEntrada = ArquivoEntrada + ".asm";
    ifstream entrada(ArquivoEntrada.c_str()); 
    size_t Arq = ArquivoEntrada.find_first_of("."); 
    ArquivoEntrada = ArquivoEntrada.substr(0,Arq);
    ArquivoPre = ArquivoEntrada + ".pre";
    ofstream saida_pre(ArquivoPre.c_str());
   
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
                frase = Remover_Comentarios(frase); // Retira comentários das linhas do arquivo
                frase = NaoSensivelAoCaso(frase); // Transforma todas strings para Maiuscula
                buffer_tokens = pegar_tokens(frase); // Pega os tokens de 1 linha
                
                size_t tira_espaco = frase.find_first_of(":");
                    
                if(tira_espaco!=string::npos)
                {
                    string verifica_espaco = frase.substr(tira_espaco,string::npos);
                }
                

                size_t equ_true = frase.find("EQU");
                if(equ_true!= string::npos)
                {
                    unsigned int tamanho = count(frase.begin(),frase.end(),' ');
                    if(tamanho>2 && frase.back()!= ' ')                    
                    {
                        ERRO_FLAG = true;
                        ERRO_FLAG_EQU = true;
                        linha_equ.push_back(Transforma_para_String(nlinha));
                    }

                    size_t rotulo_equ = frase.find_first_of(":");
                    
                    if(rotulo_equ==string::npos)
                    {
                        ERRO_FLAG =true;
                        ERRO_FLAG_1 = true;
                        linha_equ1.push_back(Transforma_para_String(nlinha));
                    }


                    
                    for(unsigned int i = 0;i<buffer_tokens.size();i++)
                    {
                        size_t dois_pontos = buffer_tokens[i].find_first_of(":"); 
                        if(dois_pontos!=string::npos)
                        {
                                buffer_tokens[i] = buffer_tokens[i].substr(0,dois_pontos);
                                Nome_Rot_EQU.push_back(buffer_tokens[i]);
                                Valor_EQU.push_back(buffer_tokens[i+2]);
                        }                                                                     
                    } 
                    
                    frase = frase.replace(frase.begin(),frase.end(),"");

                }

                for(unsigned int i = 0; i<Nome_Rot_EQU.size();i++)
                {
                    for(unsigned int j = 0;j<Valor_EQU.size();j++)
                    {
                        size_t substitui_equ = frase.find(Nome_Rot_EQU[i]);
                        if(substitui_equ!=string::npos)
                        {
                            frase = frase.replace(substitui_equ,Nome_Rot_EQU[i].size(),Valor_EQU[j]);
                        }
                    }
                     
                }
                
                size_t verifica_if = frase.find("IF");
                if(verifica_if != string::npos)
                {
                    unsigned int tamanho = count(frase.begin(),frase.end(),' ');
                    if(tamanho>1 && frase.back()!= ' ')                    
                    {
                        
                        ERRO_FLAG = true;
                        ERRO_FLAG_IF = true;
                        linha_if.push_back(Transforma_para_String(nlinha));
                    }

                    size_t if_0 = frase.find("0");
                    if(if_0 != string::npos)
                    {
                        frase = frase.replace(frase.begin(),frase.end(),"");
                        getline(entrada,frase);
                        frase = frase.replace(frase.begin(),frase.end(),"");
                    }
                    else
                    {
                        frase = frase.replace(frase.begin(),frase.end(),"");
                    }
                }
                
                if(ERRO_FLAG == false)
                {

                    if(frase.empty()) // Analisa se a primeira linha está vazia(Se tiver, continua a execução)
                    {
                        continue;
                    }
                    else
                    {   
                        saida_pre<<frase<<endl;
                    }
                } 
                else
                {
                    remove(ArquivoPre.c_str());
                }
            }
        }

        if(ERRO_FLAG_EQU == true)
        {
            for(unsigned int i = 0; i<linha_equ.size();i++)
            {
                cout<<"Erro semântico na linha "<<Transforma_para_int(linha_equ[i])<<". Diretiva EQU com argumentos inválidos."<<endl;
            }
        }
        if(ERRO_FLAG_1 ==true)
        {
            for(unsigned int i = 0; i<linha_equ1.size();i++)
            {
                cout<<"Erro sintático na linha "<<Transforma_para_int(linha_equ1[i])<<". Diretiva EQU sem label."<<endl;
            }
        }
        if(ERRO_FLAG_IF == true)
        {
            for(unsigned int i = 0; i<linha_if.size();i++)
            {
                cout<<"Erro semântico na linha "<<Transforma_para_int(linha_if[i])<<". Diretiva IF com argumentos inválidos."<<endl;
            }
        }

        primeira_passagem* primeira = new primeira_passagem();
            
        primeira->leitura(ArquivoEntrada);


        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////// Imprime na tela para debug////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
      
        /*for(unsigned int i=0;i<buffer_completo.size();++i)
        {
            //size_t tamanho_token = buffer_completo[i].length();
            //csaida_pre<<tamanho_token<<endl;
            cout<<buffer_completo[i]<<endl; //Imprime na tela 
        }*/
        /*for(unsigned int i=0;i<buffer_tokens.size();++i)
        {
            cout<<buffer_tokens[i]<<endl; //Imprime na tela 
        }*/
    }
}

// Método para Pegar os tokens de cada linha
vector<string> pre_processamento::pegar_tokens(string linha)
{
    string frase;
    stringstream ss(linha);
    vector<string> buffer_tokens;
 
    while (ss >> frase)
    {
        buffer_tokens.push_back(frase);
        
    }
 
    return(buffer_tokens);
}

map<string, int> pre_processamento::pegaLinha(string linha, int numero_linha)
{
    string frase;
    stringstream ss(linha);
    map<string, int>tabela_tokens; 
 
    while (ss >> frase)
    { 
        tabela_tokens.insert(make_pair(frase,numero_linha));       
    }

    return tabela_tokens;
}
// Método para Remover comentários
string pre_processamento::Remover_Comentarios(string frase)
{
    size_t achou = frase.find_first_of(";");               
 
    if (achou != string::npos)
    {
       frase = frase.substr(0, achou);
       return(frase);
    }
    else
       return(frase);    
}

//Método para passar todos strings para maiuscula e eliminar a sensibilidade ao caso 
string pre_processamento::NaoSensivelAoCaso(string frase)
{
 
    for(unsigned int i=0; i < frase.size(); i++)
    {
 
       frase[i]=toupper(frase[i]);
    }
    return(frase);
}
//Método para transformar int para string
string pre_processamento::Transforma_para_String(int numero)
{
     ostringstream ss;
     ss << numero;

     return ss.str();
}
// Método para transformar string para int
int pre_processamento::Transforma_para_int(string vetor)
{
    return(atoi(vetor.c_str()));
}

