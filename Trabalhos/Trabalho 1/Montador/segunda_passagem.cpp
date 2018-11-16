
/*
//////////////////////////////////////////////////////////////// Classe segunda_passagem /////////////////////////////////////////////////////////////////////////////////////////

Classe responsável pela montagem do código. Analisa se as strings são instruções, diretivas ou argumentos e cria as tabelas de símbolos e definições. Além de montar o código, 
da a informação de realocação. 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

*/

#include "Montador.h"
 
 
using namespace std;
 
// Método Construtor da Classe
segunda_passagem::segunda_passagem()
{
 flag_com_modulo =false;
}


//Método Destrutor da Classe
segunda_passagem::~segunda_passagem()
{
    
 
}

// Método de Leitura do arquivo .asm, manipulação deste arquivo e tratamento das diretivas IF e EQU
void segunda_passagem::montar(string ArquivoEntrada)
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
    string MACRO("MACRO");
    string ENDMACRO("ENDMACRO");

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /////////////////////////Pega os nomes dos arquivos que foram digitados na execução do programa/////////////////////////
    
    ArquivoPre = ArquivoEntrada + ".pre";
    ifstream entrada(ArquivoPre.c_str()); 
    size_t Arq = ArquivoPre.find_first_of("."); 
    ArquivoPre = ArquivoPre.substr(0,Arq);
    ArquivoOBJ = ArquivoPre + ".obj";
    ofstream out(ArquivoOBJ.c_str());
    pre_processamento* pre = new pre_processamento();

///// Criação da tabela de instruções
    if(entrada.is_open())
    {
        buffer_completo.clear();
        while(getline(entrada,frase))
        {
            
            if(!frase.empty()) // Analisa se a linha está vazia (Se tiver, ignora esta linha)
            {
                if(frase.empty()) // Analisa se a primeira linha estÃ¡ vazia(Se tiver, continua a execução)
                {
                    continue;
                }

                nlinha++;
                frase = pre->Remover_Comentarios(frase); // Retira comentários das linhas do arquivo
                frase = pre->NaoSensivelAoCaso(frase); // Transforma todas strings para Maiuscula
                
                size_t consta = frase.find("CONST");
                if(consta!=string::npos)
                {
                    size_t hexa = frase.find("0X");
                    if(hexa!=string::npos)
                    {
                        string aux = frase.substr(hexa,string::npos);
                        frase = frase.replace(hexa,aux.size(),pre->Transforma_para_String(converte_hexa(aux)));
                        cout<<frase<<endl;
                    }
                }

                buffer_tokens = pre->pegar_tokens(frase); // Pega os tokens de 1 linha
                
                
                for(unsigned int i=0;i<buffer_tokens.size();i++)
                {
        
                    buffer_completo.push_back(buffer_tokens[i]);    
                }                
            }
        }    
    }         
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
    
    map<string, int>tabela_diretivas;
    tabela_instrucoes.insert(make_pair("BEGIN",0));
    tabela_instrucoes.insert(make_pair("END",0));
    tabela_instrucoes.insert(make_pair("EXTERN",0));
    tabela_instrucoes.insert(make_pair("PUBLIC",0));
    

    
    
    map<string,int>tabela_simbolos;
    int valor;                                 // variável para verificar se um string possui ":"
    unsigned int cont_posicoes = 0;
         
    for(unsigned int i=0;i<buffer_completo.size();i++)
    {                                // Atualizando os valores do contador de posições
        if(buffer_completo[i].compare(BEGIN)==0)
        {
            flag_com_modulo =true;
        }

        if(!buffer_completo[i].empty() && i!=0)
        {
            cont_posicoes = cont_posicoes+1;
            if(buffer_completo[i].compare(SPACE)==0)
            {
                size_t space_com_arg = buffer_completo[i+1].find_first_of("0123456789");
                if(space_com_arg!=string::npos)
                {
                    cont_posicoes = cont_posicoes-1;
                }
            }
            if(buffer_completo[i].compare(CONST)==0 ||buffer_completo[i].compare(BEGIN)==0|| buffer_completo[i].compare(EXTERN)==0|| buffer_completo[i].compare(END)==0)
            {
                cont_posicoes = cont_posicoes-1;
            }
            if(buffer_completo[i].compare(TEXT)==0 || buffer_completo[i].compare(DATA)==0|| buffer_completo[i].compare(BSS)==0|| buffer_completo[i].compare(PUBLIC)==0)
            { // Retirando as duas diretivas SECTION TEXT ou SECTION DATA de posiÃ§Ãµes de memória
                cont_posicoes = cont_posicoes-2;
            }
        }
                    
        // montando a tabela de definiçõess (tabela com os rótulos e seu endereço)
        
        string a = buffer_completo[i];          // salva o buffer_completo[i] com ":", para depois colocar novamente no buffer.
        valor = buffer_completo[i].find(":");
        if(valor != -1)
        {    
                buffer_completo[i].erase(valor); // Apaga ":" dos rótulos para colocar na tabela de definições
                tabela_simbolos.insert(make_pair(buffer_completo[i],cont_posicoes));  
                cont_posicoes = cont_posicoes-1;
                buffer_completo[i] = a; // retoma o ":" para inserir novamente no buffer completo
           
        }
                   
    }
    /////////////// TABELA DE DEFINICOES /////////////////////
    
    
    if(flag_com_modulo==true)
    {    
        out<<"TABLE DEFINITION"<<endl;   
        map<string,int>::iterator it = tabela_simbolos.begin();                         
        for(unsigned int i=0;i<buffer_completo.size();i++)
        {
            if(buffer_completo[i].compare(PUBLIC)==0)
            {
                it = tabela_simbolos.find(buffer_completo[i+1]);
                out << buffer_completo[i+1] << " " << it->second << endl;
                it++;
            }
        }
        out << "\n";
    }

    /////////// TABELA DE USO ///////////////////////////////        
    if(flag_com_modulo==true)
    {
        out << "TABLE USE"<<endl;
        string auxiliar, auxiliar2;
        int value;  
        int contador=0;
        for(unsigned int i=0;i<buffer_completo.size();i++)
        {
            if(buffer_completo[i+1].compare(EXTERN)==0)
            {
                auxiliar2 = buffer_completo[i];
                value = buffer_completo[i].find(":");               // retirando o ":" dos rótulos.
                if(value != -1)
                {      
                    buffer_auxiliar.push_back(buffer_completo[i].erase(value));
                    buffer_completo[i] = auxiliar2;
                }
            } 
        }  
        
        for(unsigned int i=0;i<buffer_completo.size();i++)
        {
            size_t dois_pontos = buffer_completo[i].find(":");
            size_t mais = buffer_completo[i].find_first_of("+");
            if(dois_pontos != string::npos)
            {
                contador = contador-1;
            }

            if (buffer_completo[i].compare(BEGIN)==0|| buffer_completo[i].compare(EXTERN)==0)
            {
                contador = contador-1;
            }
            if(buffer_completo[i].compare(TEXT)==0 || buffer_completo[i].compare(DATA)==0|| buffer_completo[i].compare(BSS)==0|| buffer_completo[i].compare(PUBLIC)==0)
            { // Retirando as duas diretivas SECTION TEXT ou SECTION DATA de posiçõees de memória
                contador = contador-2;
            }
            for(unsigned int j=0;j<buffer_auxiliar.size();j++)
            {
                if(mais!=string::npos)
                {
                    
                    string A = buffer_completo[i].substr(0,mais);
                    string B = buffer_completo[i].substr(mais+1,string::npos);

                    if(A.compare(buffer_auxiliar[j])==0)
                    {                    
                        out  <<A<< " "<< contador <<endl;
                    }
                    
                }  

                else
                {

                    if(buffer_completo[i].compare(buffer_auxiliar[j])==0)
                    {        
                        out << buffer_completo[i]<< " "<< contador <<endl;
                    
                    }
                } 

                
            }
            
            contador++;
        }
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////// Relativo //////////////////////////////////////////////////////
    if(flag_com_modulo == true)
    {
        int contador=0;
        out<<"\n";
        out<<"RELATIVE"<<endl;

        for(unsigned int i=0;i<buffer_completo.size();i++)
        {
            size_t dois_pontos = buffer_completo[i].find(":");
            size_t mais = buffer_completo[i].find_first_of("+");
            if(dois_pontos != string::npos)
            {
                contador = contador-1;
            }

            if (buffer_completo[i].compare(BEGIN)==0|| buffer_completo[i].compare(EXTERN)==0)
            {
                contador = contador-1;
            }
            if(buffer_completo[i].compare(TEXT)==0 || buffer_completo[i].compare(DATA)==0|| buffer_completo[i].compare(BSS)==0|| buffer_completo[i].compare(PUBLIC)==0)
            { // Retirando as duas diretivas SECTION TEXT ou SECTION DATA de posiçõees de memória
                contador = contador-2;
            }
            for(unsigned int j=0;j<buffer_auxiliar.size();j++)
            {
                if(mais!=string::npos)
                {
                    
                    string A = buffer_completo[i].substr(0,mais);
                    string B = buffer_completo[i].substr(mais+1,string::npos);

                    if(A.compare(buffer_auxiliar[j])==0)
                    {                    
                        out  <<A<< " "<< contador <<endl;
                    }
                    
                }  

                else
                {

                    if(buffer_completo[i].compare(buffer_auxiliar[j])==0)
                    {        
                        out << contador<< " ";
                    
                    }
                } 

                
            }
            
            contador++;
        }
        out << "\n";

    }

     /////////////////////////////////// Montando o Código ///////////////////////////////////////////// 
    
    if(flag_com_modulo==true)
    {
        out << "\n";
    }
    for(unsigned int i=0;i<buffer_completo.size();i++)
    { 

        if(i==0 && flag_com_modulo==true)
        {
            out << "CODE"<<endl;
        }  

        if(tabela_instrucoes.count(buffer_completo[i])>0 && buffer_completo[i].compare(END)!=0 && buffer_completo[i].compare(EXTERN)!=0&&buffer_completo[i].compare(PUBLIC)!=0 && buffer_completo[i].compare(BEGIN)!=0)
        {      // analisa se o string existe na tabela de instruções.
            out  << tabela_instrucoes[buffer_completo[i]]<< " ";
        }     
        
        
        if(tabela_diretivas.count(buffer_completo[i])>0)
        {      // analisa se o string existe na tabela de instruções.
            
            if(buffer_completo[i].compare(BEGIN)==0|| buffer_completo[i].compare(EXTERN)==0|| buffer_completo[i].compare(END)==0)
            {
                out  << tabela_diretivas[buffer_completo[i]]<< " ";
                cont_posicoes = cont_posicoes-1;
            }
            else
            {
                out  << tabela_diretivas[buffer_completo[i]]<< " ";
                i=i+2;
                cont_posicoes = cont_posicoes-2;
            }
            
        }                   
        

        else if(buffer_completo[i].compare(CONST)==0)
        {
            out << buffer_completo[i+1] << " ";
        }

        else if(buffer_completo[i].compare(SPACE)==0)
        {
            out << "0" << " ";
        }

        size_t mais = buffer_completo[i].find_first_of("+");
        if(mais!=string::npos)
        {
            
            string A = buffer_completo[i].substr(0,mais);
            string B = buffer_completo[i].substr(mais+1,string::npos);

            if(tabela_simbolos.count(A)>0 &&  buffer_completo[i+1].compare(EXTERN)!=0 && buffer_completo[i-1].compare(PUBLIC)!=0)
            {
                
                out  << tabela_simbolos[buffer_completo[i].substr(0,mais)] + pre->Transforma_para_int(B) << " ";
            }
            
        }  

        else
        {
            if(tabela_simbolos.count(buffer_completo[i])>0 &&  buffer_completo[i+1].compare(EXTERN)!=0 && buffer_completo[i-1].compare(PUBLIC)!=0)
            {
             
                out  << tabela_simbolos[buffer_completo[i]] << " ";
            }
        }      
    } 
    
    
    
    /*for (const auto &p : tabela_simbolos) {
    std::cout << "m[" << p.first << "] = " << p.second << '\n';
    }*/
    
   /*for(unsigned int i=0; i<buffer_completo.size();i++){
        cout << buffer_completo[i] << endl;
    }*/
    /*for (const auto &p : tabela_instrucoes) {
    std::cout << "m[" << p.first << "] = " << p.second << '\n';
    }*/
}
int segunda_passagem::converte_hexa(string hexa)
{
    int aux;
    stringstream ss;
    ss<<hexa;
    ss>>std::hex>>aux;
    return(aux);
}
