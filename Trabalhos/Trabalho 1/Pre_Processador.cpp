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
pre_processamento::pre_processamento()
{
 	EQU_FLAG = false;
    ERRO_FLAG = false;
    COPY_SEM_ESPACO = false;
	nlinha= 0;
	linha_equ = 0;
	IF1 =0;
 
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
  

    unsigned int k=0,j=0;
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /////////////////////////Pega os nomes dos arquivos que foram digitados na execução do programa/////////////////////////
    
    ArquivoEntrada = ArquivoEntrada + ".asm";
    ifstream entrada(ArquivoEntrada.c_str()); 
    size_t Arq = ArquivoEntrada.find_first_of("."); 
    ArquivoEntrada = ArquivoEntrada.substr(0,Arq);
   
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
                tabela_tokens = pegaLinha(frase,nlinha);


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
                                COPY_SEM_ESPACO =true;                                
                            }

                            i++;

                        }
                    }
                    else
                    {
                        buffer_completo.push_back(buffer_tokens[i]);    
                    }  
                }        
                           
                ///////////////////////////////////// Analisa EQU //////////////////////////////////////////////////
                ///////////////////////////////////////////////////////////////////////////////////////////////////////////
                for(unsigned int i = 0; i< buffer_completo.size();i++)
                {

                    /////// Retira os dois pontos dos rótulos antes do EQU e salva o nome do rótulo e seus valores/////
                    if(buffer_completo[i].compare(EQU) == 0)
                    {
                        size_t dois_pontos = buffer_completo[i-1].find_first_of(":"); 
                        if(dois_pontos!=string::npos)
                        {
                            buffer_completo[i-1] = buffer_completo[i-1].substr(0,dois_pontos);
                            Nome_Rot_EQU.push_back(buffer_completo[i-1]);
                            Valor_EQU.push_back(buffer_completo[i+1]);
                        }

                        EQU_FLAG = true;                      
                        linha_equ = nlinha;                         
                    }
                    ///////////////////////////////////////////////////////////////////////////////////////////////////////
                }
                     
                /////////////////////// Troca os Rótulos pelos valores correspondentes///////////////////////////////////// 
                for(unsigned int i = 0; i< buffer_completo.size();i++)
                {
                    for(unsigned int j=0; j< Nome_Rot_EQU.size();j++)
                    {
                        if((buffer_completo[i]== Nome_Rot_EQU[j] && EQU_FLAG == true))
                        {                          
                            buffer_completo[i] = Valor_EQU[j];
                        }
                    }
                }
           

            ///////////////////////////////////////Analisa IF//////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////


                for (unsigned int i = 0; i < buffer_completo.size(); ++i)
                {
                    if(buffer_completo[i].compare(IF)==0)
                    {
                        IF1 = 0;
                    }
                }
                
                for(k=j ; k < buffer_completo.size();k++)
                {
                    if(k!= 0)
                    {
                        if (buffer_completo[k-1].compare(IF) == 0 && IF1==0)
                        {
                            if(buffer_completo[k] == "0" || buffer_completo[k]=="0 ")
                            {
                                getline(entrada,linha);
                                nlinha++;
                            }

                            j=k;

                            IF1 = 1;
                            buffer_completo.pop_back();
                            buffer_completo.pop_back();

                        }
                    }
                }
                IF1 =0; 

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
        
        if(ERRO_FLAG == false)
        {
            ArquivoPre = ArquivoEntrada + ".pre";
            ofstream saida_pre(ArquivoPre.c_str());

            ////////////////////////////// Imprime Arquivo .pre////////////////////////////////////////////////////////////////
            for(unsigned int i=0;i<buffer_completo.size();++i)
            { 
                for(unsigned int j=0;j<buffer_rotulo.size();++j)
                {
                    size_t pula = buffer_completo[i].find_first_of(":");
                    size_t space_argumento = buffer_completo[i+2].find_first_of(":");
                    if(pula!=string::npos)
                    {
                        string tira_2 = buffer_completo[i].substr(0,pula);
                        string rot_space_comp = buffer_completo[i+2].substr(0,space_argumento);
                        
                        if(tira_2 == buffer_rotulo[j])
                        {
                            if((buffer_completo[i+1].compare(ADD) ==0)||(buffer_completo[i+1].compare(SUB) ==0)||(buffer_completo[i+1].compare(MULT) ==0)||(buffer_completo[i+1].compare(DIV) ==0)||(buffer_completo[i+1].compare(JMP) ==0)||(buffer_completo[i+1].compare(JMPN) ==0)||(buffer_completo[i+1].compare(JMPZ) ==0)||(buffer_completo[i+1].compare(JMPP) ==0)||(buffer_completo[i+1].compare(LOAD) ==0)||(buffer_completo[i+1].compare(STORE) ==0)||(buffer_completo[i+1].compare(INPUT) ==0)||(buffer_completo[i+1].compare(OUTPUT) ==0))
                            {
                                saida_pre<<buffer_completo[i]<<"\t"<<buffer_completo[i+1]<<"\t"<<buffer_completo[i+2]<<endl; 
                                i=i+2;
                            }
                            
                            else if((buffer_completo[i+1].compare(SPACE) ==0))
                            {
                                if(rot_space_comp!= (buffer_rotulo[j+1]))
                                {
                                    saida_pre<<buffer_completo[i]<<"\t"<<buffer_completo[i+1]<<"\t"<<buffer_completo[i+2]<<endl;
                                    i = i+2;
                                }

                                else
                                {
                                    saida_pre<<buffer_completo[i]<<"\t"<<buffer_completo[i+1]<<endl;
                                    i=i+1;
                                }
                            }

                            else if ((buffer_completo[i+1].compare(CONST) ==0))
                            {
                                saida_pre<<buffer_completo[i]<<"\t"<<buffer_completo[i+1]<<"\t"<<buffer_completo[i+2]<<endl;
                                i=i+2;
                            }

                            else if(buffer_completo[i+1].compare(COPY) == 0)
                            {
                                if(COPY_SEM_ESPACO==true)
                                {
                                    saida_pre<<buffer_completo[i]<<"\t"<<buffer_completo[i+1]<<"\t"<<buffer_completo[i+2]<<","<<buffer_completo[i+3]<<endl;
                                    i = i+3;
                                    COPY_SEM_ESPACO = false;
                                }
                                else
                                {
                                    saida_pre<<buffer_completo[i]<<"\t"<<buffer_completo[i+1]<<"\t"<<buffer_completo[i+2]<<", "<<buffer_completo[i+4]<<endl;
                                    i = i+3;
                                }
                                
                            }
                        }
                    }
                }
                
                if(buffer_completo[i].compare(COPY) == 0)
                {
                    
                    if(COPY_SEM_ESPACO==true)
                    {
                        saida_pre<<buffer_completo[i]<<"\t"<<buffer_completo[i+1]<<","<<buffer_completo[i+2]<<endl;
                        i = i+2;
                        COPY_SEM_ESPACO = false;
                    }
                    else
                    {
                        saida_pre<<buffer_completo[i]<<"\t"<<buffer_completo[i+1]<<", "<<buffer_completo[i+3]<<endl;
                        i = i+2;
                    }

                }

                else if((buffer_completo[i].compare(SECTION) ==0)||(buffer_completo[i].compare(INPUT) ==0)||(buffer_completo[i].compare(ADD) ==0)||(buffer_completo[i].compare(SUB) ==0)||(buffer_completo[i].compare(MULT) ==0)||(buffer_completo[i].compare(DIV) ==0)||(buffer_completo[i].compare(JMP) ==0)||(buffer_completo[i].compare(JMPN) ==0)||(buffer_completo[i].compare(JMPZ) ==0)||(buffer_completo[i].compare(JMPP) ==0)||(buffer_completo[i].compare(LOAD) ==0)||(buffer_completo[i].compare(STORE) ==0)||(buffer_completo[i].compare(OUTPUT) ==0))
                {
                    saida_pre<<buffer_completo[i]<<"\t"<<buffer_completo[i+1]<<endl;
                    i = i+1;
                }

                else if ((buffer_completo[i].compare(STOP) ==0))
                {
                    saida_pre<<buffer_completo[i]<<endl;
                }
            }

            primeira_passagem* primeira = new primeira_passagem();
            
            primeira->leitura(ArquivoEntrada);
        }
  
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////// Imprime na tela para debug//////////////////////////////////////////////////////////////////////////
            
       
    if(tabela_tokens.count("SECTION")>0)
    {
        cout<<tabela_tokens["SECTION"]<<endl;
    }
      
        /*for(unsigned int i=0;i<buffer_completo.size();++i)
        {
            //size_t tamanho_token = buffer_completo[i].length();
            //csaida_pre<<tamanho_token<<endl;
            cout<<buffer_completo[i]<<endl; //Imprime na tela 
        }*/
        /*for(unsigned int i=0;i<buffer_tokens.size();++i)
        {
            csaida_pre<<buffer_tokens[i]<<endl; //Imprime na tela 
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