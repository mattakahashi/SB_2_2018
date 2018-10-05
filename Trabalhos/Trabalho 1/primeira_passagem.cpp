/*
//////////////////////////////////////////////////////////////// Classe pre_processamento /////////////////////////////////////////////////////////////////////////////////////////

Classe responsável pelo pre-processamento do arquivo .asm. Recebe como argumento o nome deste arquivo, o abre e faz a leitura de linha por linha. Retira os comentários do arquivo,
deixa todos caracteres em letra maiúscula (Montador não sensível ao caso), separa os caracteres em tokens, detecta se há alguma diretiva EQU e IF e as trata, detecta a diretiva
MACRO e substitui seu código ao há chamada dessa diretiva.

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

*/

#include "Montador.h"
 
 
using namespace std;
 
// Método Construtor da Classe
primeira_passagem::primeira_passagem()
{
    EQU_FLAG = false;
    ERRO_FLAG = false;
    Erro_Flag_TEXT = false;
    ERRO_NO_TEXT = true;
    ERRO_NO_DATA = true;
    ERRO_COPY =false;
    nlinha= 0; 
    linha_text = 0;
    linha_data = 0;
    linha_bss = 0;
}


//Método Destrutor da Classe
primeira_passagem::~primeira_passagem()
{
    
 
}

// Método de Leitura do arquivo .asm, manipulação deste arquivo e tratamento das diretivas IF e EQU
void primeira_passagem::leitura(string ArquivoEntrada)
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
                buffer_tokens = pegar_tokens(frase, nlinha);
             
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

                            size_t teste_copy = B.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXZ,._");
                            if(teste_copy != string::npos)
                            {                                
                                ERRO_COPY =true;                                
                            }

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

        
        
        ///////////////////////////////////// Analisa Sections //////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////////////
        for(unsigned int i = 0; i< buffer_completo.size();i++)
        {

            /////// Retira os dois pontos dos rótulos antes do EQU e salva o nome do rótulo e seus valores/////
            if(buffer_completo[i].compare(TEXT) == 0)
            {
                ERRO_NO_TEXT = false;                            
            }  
            if(buffer_completo[i].compare(DATA) == 0)
            {
                ERRO_NO_DATA = false;                            
            } 
        } 
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////// Coloca os rotulos no buffer_rotulo /////////////////////////////////////////////////    
        for(unsigned int i=0;i<buffer_completo.size();++i)
        {
            size_t pula = buffer_completo[i].find_first_of(":");
            if(pula!=string::npos)
            {
                string jump = buffer_completo[i].substr(0,pula);
                buffer_rotulo.push_back(jump); 

            }
        }
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////// Imprime na tela para debug//////////////////////////////////////////////////////////////////////////
        cout<<linha_text<<endl;
        cout<<linha_data<<endl;
        //cout<<linha_bss<<endl;
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

    if(ERRO_NO_TEXT == true)
    {
        cout<<"Erro semântico. Código sem SECTION TEXT."<<endl;
        ERRO_FLAG =true;
    }

    if(ERRO_NO_DATA == true)
    {
        cout<<"Erro semântico. Código sem SECTION DATA."<<endl;
        ERRO_FLAG =true;
        
    }
    if(ERRO_NO_DATA == false && ERRO_NO_TEXT == false)
    {
        if(((linha_text>linha_data) && linha_data >=1) || ((linha_text>linha_bss) && linha_bss >=1) )
        {
            cout<<"Erro semântico na linhas "<<linha_text<<" ,"<<linha_data<<" e "<<linha_bss<<". SECTION TEXT deve sempre precedir a SECTION DATA e a SECTION BSS."<<endl;
            ERRO_FLAG =true;
        }
    }

    if(ERRO_COPY == true)
    {
        cout<<"Erro semântico na linha "<<linha_cop<<". Erro na chamada a instrução COPY."<<endl;
        ERRO_FLAG =true;
        
    }
    
    if(ERRO_FLAG == false)
    {
        segunda_passagem* segunda = new segunda_passagem();
        
        segunda->leitura(ArquivoPre);
    }
}

// Método para Pegar os tokens de cada linha
vector<string>primeira_passagem::pegar_tokens(string linha, int numero_linha)
{
    string frase;
    string TEXT("TEXT");
    string COPY("COPY");
    string DATA("DATA");
    string BSS("BSS");
    stringstream ss(linha);
    vector<string> buffer_tokens;
 
    while (ss >> frase)
    {
        buffer_tokens.push_back(frase);
        if(frase.compare(TEXT)==0)
        {
            linha_text = numero_linha;
        }

        if(frase.compare(DATA)==0)
        {
            linha_data = numero_linha;
        }

        if(frase.compare(BSS)==0)
        {
            linha_bss = numero_linha;
        }

        if(frase.compare(COPY)==0)
        {
            linha_cop = numero_linha;
        }

    }
 
    return(buffer_tokens);
}