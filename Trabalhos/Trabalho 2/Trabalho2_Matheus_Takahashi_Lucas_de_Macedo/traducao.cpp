#include "traducao.h"
 
 
using namespace std;
 
// Método Construtor da Classe
traducao::traducao()
{
 	EQU_FLAG = false;
    SPACE_FLAG = false; 
	nlinha= 0;
	linha_equ = 0;
	IF1 =0;

    INPUT_FLAG = false;
    OUTPUT_FLAG = false;    
    C_INPUT_FLAG = false;
    C_OUTPUT_FLAG = false;
    S_INPUT_FLAG = false;
    S_OUTPUT_FLAG = false;
    PRIMEIRO_OPERANDO_FLAG = false;
    SEGUNDO_OPERANDO_FLAG = false;
 
}


//Método Destrutor da Classe
traducao::~traducao()
{
    
 
}

// Método de Leitura das linhas do Assembly inventado e tratamentos dos erros 
void traducao::leitura(string ArquivoEntrada)
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
    string C_INPUT("C_INPUT");
    string C_OUTPUT("C_OUTPUT");
    string S_INPUT("S_INPUT");
    string S_OUTPUT("S_OUTPUT");    

 	unsigned int k=0,j=0;
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 	////////////////////Pega os nomes dos arquivos que foram digitados na execução do programa//////////////////////////////
    ifstream entrada(ArquivoEntrada.c_str());
    size_t Arq = ArquivoEntrada.find_first_of(".");
    ArquivoEntrada = ArquivoEntrada.substr(0,Arq);
    ArquivoSaida = ArquivoEntrada + ".s";
    ofstream saida(ArquivoSaida.c_str());
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

	            /////////// Separa os Tokens no caso do COPY, S_INPUT, S_OUTPUT////////////////////////////
		        for(unsigned int i=0;i<buffer_tokens.size();i++)
		        {
                    if ((buffer_tokens[i].compare(COPY) == 0)||(buffer_tokens[i].compare(S_INPUT)==0)||(buffer_tokens[i].compare(S_OUTPUT)==0))
                    {
                        size_t virgula = buffer_tokens[i+1].find(",");
                        if(virgula != string::npos)
                        {
                            string A = buffer_tokens[i+1].substr(0, virgula); //Primeiro token do copy
                            string B = buffer_tokens[i+1].substr(virgula+1); //Segundo token do copy

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
           


                /////////////////////////////////// Analisa SPACE//////////////////////////////////////////////////////////
                ///////////////////////////////////////////////////////////////////////////////////////////////////////////	
                for(unsigned int i = 0; i< buffer_completo.size();i++)
                {
                    /////// Retira os dois pontos dos rótulos antes do SPACE e salva o nome do rótulo e seus valores/////
                    if(buffer_completo[i].compare(SPACE) == 0)
                    {
                        size_t dois_pontos = buffer_completo[i-1].find_first_of(":"); 
                        if(dois_pontos!=string::npos)
                        {
                            buffer_completo[i-1] = buffer_completo[i-1].substr(0,dois_pontos);
                            buffer_bss.push_back(buffer_completo[i-1]);
                            if(buffer_tokens.size()==2)
                            {
                                buffer_bss_valor.push_back("1");
                                continue;
                            }
                            size_t space_arg = buffer_completo[i+1].find_first_of("0123456789");

                            if(space_arg!=string::npos)
                            {
                                buffer_bss_valor.push_back(buffer_completo[i+1]);
                            }
                            else
                            {
                                buffer_bss_valor.push_back("1");
                            }
                        }

                        SPACE_FLAG = true;                      
                        linha_equ = nlinha;                         
                    }
                    ///////////////////////////////////////////////////////////////////////////////////////////////////////
                }

                /////////////////////// Troca os Rótulos pelos valores correspondentes/////////////////////////////////////
                for(unsigned int i = 0; i< buffer_completo.size();i++)
                {
                    for(unsigned int j=0; j< buffer_bss.size();j++)
                    {
                        if((buffer_completo[i]== buffer_bss[j] && SPACE_FLAG == true))
                        {                          
                            //buffer_completo[i] = buffer_bss_valor[j];
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
                //////////////////////////////////////////////////////////////////////////////////////////////////////////
	        }
	    }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////// Colocando nos buffers o .BSS e .DATA///////////////////////////////////////////    
        for(unsigned int j=0;j<buffer_completo.size();j++)
        {


            ///////////////////Pegar os INPUT's e colocar eles em .BSS////////////////

            if(buffer_completo[j].compare(DATA)==0)
            {
                                
                for(unsigned int i=j;i<buffer_completo.size();i++)
                {                


                ////////////////////////////////// Separando os rotulos //////////////////////////////////////////////////////

                    size_t rot = buffer_completo[i].find_first_of(":");

                    if ( rot != string::npos )
                    {
                        string rotulo = buffer_completo[i].substr( 0, rot);

                        if (buffer_completo[i+1].compare(CONST) == 0){

                            buffer_data.push_back(rotulo);
                            buffer_data_valor.push_back(buffer_completo[i+2]);
                            //buffer_bss_valor.push_back(buffer_completo[i+2]);
                        }
                    
                    }
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////
                }

               
                /////////////////////////IMPRIMINDO NA SAIDA O DATA E O BSS/////////////////////
                saida<<"section .data"<<endl;
                saida<<"incorreto\tdb\t'Valor de entrada incorreto!', 0ah"<<endl;
                saida<<"newline\tdb\t0Ah"<<endl;
                saida<<"NWLINESIZE\tEQU $-newline"<<endl<<endl;

                for(unsigned int i=0;i<buffer_data.size();i++)
                {
                    saida<<buffer_data[i]<<":\tdd\t"<<buffer_data_valor[i]<<endl;
                }

                saida<<endl<<"section .bss"<<endl;

                for(unsigned int i=0;i<buffer_bss.size();i++)
                {
                     saida<<buffer_bss[i]<<"\tresd\t"<<buffer_bss_valor[i]<<endl;
                }
                ////////////////////////////////////////////////////////////////////////////////
            }
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////     
        //////////////////////////////// Pegando o que está em SECTION TEXT/////////////////////////////////////////////////    

        for(unsigned int j=0;j<buffer_completo.size();j++)
        {
            
            if(buffer_completo[j].compare(TEXT)==0)
            {
                saida<<endl<<"section .text"<<endl<<"global _start"<<endl<<"_start: "<<endl;
                
                for(unsigned int i=j;i<buffer_completo.size();i++)
                {
                    ////////////////////////////////////////////////////////////////////////////////////////////////////////
                    if(buffer_completo[i].compare(SECTION)==0) break; //Para quando chegar no SECTION DATA
                    ////////////////////////////////////////////////////////////////////////////////////////////////////////
                    /////////////////////////////////////Achar rótulos para o JUMP/////////////////////////////////////////
                    size_t pula = buffer_completo[i].find_first_of(":");
                    if(pula!=string::npos)
                    {
                        string jump = buffer_completo[i].substr(0,pula);
                        buffer_rotulo.push_back(jump);
                        saida<<jump<<": ";
                        
                    }
                    ////////////////////////////////////////////////////////////////////////////////////////////////////////

                    /////////////////////////////////////Achar + para tratar vetor /////////////////////////////////////////
                    size_t mais = buffer_completo[i+1].find_first_of("+");
                    size_t more = buffer_completo[i+2].find_first_of("+");
                    
                    ////////////////////////////////////////////////////////////////////////////////////////////////////////
 
                    /////////////////////////////////////////// JMP////////////////////////////////////////////////////////
                    
                    if(buffer_completo[i].compare(JMP)==0)
                    {
                        for(unsigned int k=0; k<buffer_rotulo.size(); k++)
                        {
                            if (buffer_completo[i+1].compare(buffer_rotulo[k]) == 0)
                            {
                                saida<<"\tjmp "<<buffer_rotulo[k]<<endl;
                                break;
                            } 
                        }
                    }
                    /////////////////////////////////////////////////////////////////////////////////////////////////////

                    ///////////////////////////////////////////JMPP////////////////////////////////////////////////////////
                    
                    if(buffer_completo[i].compare(JMPP)==0)
                    {
                        for(unsigned int k=0; k<buffer_rotulo.size(); k++)
                        {
                            if (buffer_completo[i+1].compare(buffer_rotulo[k]) == 0)
                            {   
                                saida<<"\tcmp ebx,0"<<endl;
                                saida<<"\tjg "<<buffer_rotulo[k]<<endl;
                                break;
                            } 
                        }
                    }
                    /////////////////////////////////////////////////////////////////////////////////////////////////////

                    ///////////////////////////////////////////JMPN////////////////////////////////////////////////////////
                    
                    if(buffer_completo[i].compare(JMPN)==0)
                    {
                        for(unsigned int k=0; k<buffer_rotulo.size(); k++)
                        {
                            if (buffer_completo[i+1].compare(buffer_rotulo[k]) == 0)
                            {   
                                saida<<"\tcmp ebx,0"<<endl;
                                saida<<"\tjl "<<buffer_rotulo[k]<<endl;
                                break;
                            } 
                        }
                    }
                    /////////////////////////////////////////////////////////////////////////////////////////////////////

                    ///////////////////////////////////////////JMPZ////////////////////////////////////////////////////////
                    
                    if(buffer_completo[i].compare(JMPZ)==0)
                    {
                        
                        for(unsigned int k=0; k<buffer_rotulo.size(); k++)
                        {
                            if (buffer_completo[i+1].compare(buffer_rotulo[k]) == 0)
                            {   
                                saida<<"\tcmp ebx,0"<<endl;
                                saida<<"\tje "<<buffer_rotulo[k]<<endl;
                                break;
                            } 
                        }
                    }
                    /////////////////////////////////////////////////////////////////////////////////////////////////////

                    //////////////////////////////MULT/////////////////////////////////////
                    if(buffer_completo[i].compare(MULT)==0)
                    {                           
                        if(mais!=string::npos)
                        {
                            string vetor = buffer_completo[i+1].substr(mais,string::npos);
                            string label =  buffer_completo[i+1].substr(0,mais);
                            buffer_completo[i+1]= label;
                            int indice = Transforma_para_int(vetor);
                            indice = 4*indice;
                            string vetor_assembly = Transforma_para_String(indice);
                            saida<<"\tmov eax,ebx"<<endl;
                            saida<<"\timul dword ["<<buffer_completo[i+1]<<"+"<<vetor_assembly<<"]"<<endl;
                            saida<<"\tmov ebx,eax"<<endl;                                                   
                        }
                        else
                        {
                            saida<<"\tmov eax,ebx"<<endl;
                            saida<<"\timul dword ["<<buffer_completo[i+1]<<"]"<<endl;
                            saida<<"\tmov ebx,eax"<<endl;
                        }
                    }
                    //////////////////////////////////////////////////////////////////////

                    //////////////////////////////DIV/////////////////////////////////////
                    if(buffer_completo[i].compare(DIV)==0)
                    {   
                        if(mais!=string::npos)
                        {
                            string vetor = buffer_completo[i+1].substr(mais,string::npos);
                            string label =  buffer_completo[i+1].substr(0,mais);
                            buffer_completo[i+1]= label;
                            int indice = Transforma_para_int(vetor);
                            indice = 4*indice;
                            string vetor_assembly = Transforma_para_String(indice);
                            saida<<"\tmov eax,ebx"<<endl;
                            saida<<"\tidiv dword ["<<buffer_completo[i+1]<<"+"<<vetor_assembly<<"]"<<endl;
                            saida<<"\tmov ebx,eax"<<endl;                                                   
                        }
                        else
                        {
                            saida<<"\tmov eax,ebx"<<endl;
                            saida<<"\tidiv dword ["<<buffer_completo[i+1]<<"]"<<endl;
                            saida<<"\tmov ebx,eax"<<endl;
                        }                        
                        
                    }
                    //////////////////////////////////////////////////////////////////////                    

                    //////////////////////////////LOAD/////////////////////////////////////
                    if(buffer_completo[i].compare(LOAD)==0)
                    {   
                        if(mais!=string::npos)
                        {
                            string vetor = buffer_completo[i+1].substr(mais,string::npos);
                            string label =  buffer_completo[i+1].substr(0,mais);
                            buffer_completo[i+1]= label;
                            int indice = Transforma_para_int(vetor);
                            indice = 4*indice;
                            string vetor_assembly = Transforma_para_String(indice);
                            saida<<"\tmov ebx,["<<buffer_completo[i+1]<<"+"<<vetor_assembly<<"]"<<endl;                                                  
                        } 
                        else
                        {                       
                            saida<<"\tmov ebx,["<<buffer_completo[i+1]<<"]"<<endl;                    
                        }
                    }
                    //////////////////////////////////////////////////////////////////////
                    
                    //////////////////////////////COPY/////////////////////////////////////
                    if(buffer_completo[i].compare(COPY)==0)
                    {   
                        if(mais!=string::npos)
                        {
                            PRIMEIRO_OPERANDO_FLAG = true; 
                        }
                        if(more!=string::npos)
                        {
                            SEGUNDO_OPERANDO_FLAG = true;
                        }

                        if(PRIMEIRO_OPERANDO_FLAG & SEGUNDO_OPERANDO_FLAG)
                        {
                            
                            string vet = buffer_completo[i+2].substr(more,string::npos);
                            string lab = buffer_completo[i+2].substr(0,more);
                            buffer_completo[i+2]=lab;
                            int ind = Transforma_para_int(vet);
                            ind = 4*ind;
                            string vet_assembly = Transforma_para_String(ind);
                            string vetor = buffer_completo[i+1].substr(mais,string::npos);
                            string label =  buffer_completo[i+1].substr(0,mais);
                            buffer_completo[i+1]= label;
                            int indice = Transforma_para_int(vetor);
                            indice = 4*indice;
                            string vetor_assembly = Transforma_para_String(indice); 
                            saida<<"\tmov edx,0\n\tmov edx,["<<buffer_completo[i+1]<<"+"<<vetor_assembly<<"]"<<endl;
                            saida<<"\tmov ["<<buffer_completo[i+2]<<"+"<<vet_assembly<<"],edx"<<endl;    
                                                                              
                        }
                        else if(PRIMEIRO_OPERANDO_FLAG)
                        {
                            
                            string vetor = buffer_completo[i+1].substr(mais,string::npos);
                            string label =  buffer_completo[i+1].substr(0,mais);
                            buffer_completo[i+1]= label;
                            int indice = Transforma_para_int(vetor);
                            indice = 4*indice;
                            string vetor_assembly = Transforma_para_String(indice);
                            saida<<"\tmov ecx,0\n\tmov ecx,["<<buffer_completo[i+1]<<"+"<<vetor_assembly<<"]"<<endl;
                            saida<<"\tmov ["<<buffer_completo[i+2]<<"],ecx"<<endl;
                        }
                        else if(SEGUNDO_OPERANDO_FLAG)
                        {   
                            
                            string vetor = buffer_completo[i+2].substr(more,string::npos);
                            string label =  buffer_completo[i+2].substr(0,more);
                            buffer_completo[i+2]= label;
                            int indice = Transforma_para_int(vetor);
                            indice = 4*indice;
                            string vetor_assembly = Transforma_para_String(indice);
                            saida<<"\tmov ecx,0\n\tmov ecx,["<<buffer_completo[i+1]<<"]"<<endl;
                            saida<<"\tmov ["<<buffer_completo[i+2]<<"+"<<vetor_assembly<<"],ecx"<<endl;   
                        }
                        else
                        {
                            
                            saida<<"\tmov ecx,0\n\tmov ecx,["<<buffer_completo[i+1]<<"]"<<endl;
                            saida<<"\tmov ["<<buffer_completo[i+2]<<"],ecx\n\tpop ecx"<<endl;
                        }
                        
                    }
                    //////////////////////////////////////////////////////////////////////

                    //////////////////////////////STORE/////////////////////////////////////
                    if(buffer_completo[i].compare(STORE)==0)
                    {    
                        if(mais!=string::npos)
                        {
                            string vetor = buffer_completo[i+1].substr(mais,string::npos);
                            string label =  buffer_completo[i+1].substr(0,mais);
                            buffer_completo[i+1]= label;
                            int indice = Transforma_para_int(vetor);
                            indice = 4*indice;
                            string vetor_assembly = Transforma_para_String(indice);
                            saida<<"\tmov ["<<buffer_completo[i+1]<<"+"<<vetor_assembly<<"],ebx"<<endl;                                                  
                        }
                        else
                        {
                            saida<<"\tmov ["<<buffer_completo[i+1]<<"],ebx"<<endl;
                        }                        
                                            
                    }
                    //////////////////////////////////////////////////////////////////////

                    //////////////////////////////ADD/////////////////////////////////////
                    if(buffer_completo[i].compare(ADD)==0)
                    {   
                        if(mais!=string::npos)
                        {
                            string vetor = buffer_completo[i+1].substr(mais,string::npos);
                            string label =  buffer_completo[i+1].substr(0,mais);
                            buffer_completo[i+1]= label;
                            int indice = Transforma_para_int(vetor);
                            indice = 4*indice;
                            string vetor_assembly = Transforma_para_String(indice);
                            saida<<"\tadd ebx,["<<buffer_completo[i+1]<<"+"<<vetor_assembly<<"]"<<endl;                                                  
                        }
                        else
                        {
                            saida<<"\tadd ebx,["<<buffer_completo[i+1]<<"]"<<endl;
                        }
                    }
                    //////////////////////////////////////////////////////////////////////

                    //////////////////////////////SUB/////////////////////////////////////
                    if(buffer_completo[i].compare(SUB)==0)
                    {   
                        if(mais!=string::npos)
                        {
                            string vetor = buffer_completo[i+1].substr(mais,string::npos);
                            string label =  buffer_completo[i+1].substr(0,mais);
                            buffer_completo[i+1]= label;
                            int indice = Transforma_para_int(vetor);
                            indice = 4*indice;
                            string vetor_assembly = Transforma_para_String(indice);
                            saida<<"\tsub ebx,["<<buffer_completo[i+1]<<"+"<<vetor_assembly<<"]"<<endl;                                                  
                        }
                        else
                        {
                            saida<<"\tsub ebx,["<<buffer_completo[i+1]<<"]"<<endl;   
                        }
                            
                    }
                    //////////////////////////////////////////////////////////////////////

                    ///////////////////////////////INPUT///////////////////////////
                    if(buffer_completo[i].compare(INPUT)==0)
                    {
                        if(mais!=string::npos)
                        {
                            INPUT_FLAG = true;
                            string vetor = buffer_completo[i+1].substr(mais,string::npos);
                            string label =  buffer_completo[i+1].substr(0,mais);
                            buffer_completo[i+1]= label;
                            int indice = Transforma_para_int(vetor);
                            indice = 4*indice;
                            string vetor_assembly = Transforma_para_String(indice);
                            saida<<"\tmov ecx,0\n\tmov ecx,"<<buffer_completo[i+1]<<"\n\tadd ecx,"<<vetor_assembly<<endl;
                            saida<<"\tpush ecx"<<endl;
                            saida<<"\tcall LerInteiro"<<endl;                                                    
                        }
                        else
                        {  
                            INPUT_FLAG = true;
                            saida<<"\tpush "<<buffer_completo[i+1]<<endl;
                            saida<<"\tcall LerInteiro"<<endl;
                        }
                    }
                    ////////////////////////////////////////////////////////////////

                    ///////////////////////////////OUTPUT///////////////////////////
                    if(buffer_completo[i].compare(OUTPUT)==0)
                    {

                         if(mais!=string::npos)
                        {
                            OUTPUT_FLAG = true;
                            string vetor = buffer_completo[i+1].substr(mais,string::npos);
                            string label =  buffer_completo[i+1].substr(0,mais);
                            buffer_completo[i+1]= label;
                            int indice = Transforma_para_int(vetor);
                            indice = 4*indice;
                            string vetor_assembly = Transforma_para_String(indice);
                            saida<<"\tmov ecx,0\n\tmov ecx,"<<buffer_completo[i+1]<<"\n\tadd ecx,"<<vetor_assembly<<endl;
                            saida<<"\tpush ecx"<<endl;
                            saida<<"\tcall EscreverInteiro"<<endl;                                                    
                        }
                        else
                        {  
                            OUTPUT_FLAG = true;
                            saida<<"\tpush "<<buffer_completo[i+1]<<endl;
                            saida<<"\tcall EscreverInteiro"<<endl;
                        }

                    }
                    ////////////////////////////////////////////////////////////////

                    ///////////////////////////////C_INPUT///////////////////////////
                    if(buffer_completo[i].compare(C_INPUT)==0)
                    {
                        
                        if(mais!=string::npos)
                        {
                            C_INPUT_FLAG = true;
                            string vetor = buffer_completo[i+1].substr(mais,string::npos);
                            string label =  buffer_completo[i+1].substr(0,mais);
                            buffer_completo[i+1]= label;
                            int indice = Transforma_para_int(vetor);
                            indice = 4*indice;
                            string vetor_assembly = Transforma_para_String(indice);
                            saida<<"\tmov ecx,0\n\tmov ecx,"<<buffer_completo[i+1]<<"\n\tadd ecx,"<<vetor_assembly<<endl;
                            saida<<"\tpush ecx"<<endl;
                            saida<<"\tcall LerChar"<<endl;                                                    
                        }
                        else
                        {  
                            C_INPUT_FLAG = true;
                            saida<<"\tpush "<<buffer_completo[i+1]<<endl;
                            saida<<"\tcall LerChar"<<endl;
                        }
               
                    }
                    ////////////////////////////////////////////////////////////////

                    ///////////////////////////////C_OUTPUT///////////////////////////
                    if(buffer_completo[i].compare(C_OUTPUT)==0)
                    {
                        
                        if(mais!=string::npos)
                        {
                            C_OUTPUT_FLAG = true;
                            string vetor = buffer_completo[i+1].substr(mais,string::npos);
                            string label =  buffer_completo[i+1].substr(0,mais);
                            buffer_completo[i+1]= label;
                            int indice = Transforma_para_int(vetor);
                            indice = 4*indice;
                            string vetor_assembly = Transforma_para_String(indice);
                            saida<<"\tmov ecx,0\n\tmov ecx,"<<buffer_completo[i+1]<<"\n\tadd ecx,"<<vetor_assembly<<endl;
                            saida<<"\tpush ecx"<<endl;
                            saida<<"\tcall EscreverChar"<<endl;                                                    
                        }
                        else
                        {  
                            C_OUTPUT_FLAG = true;
                            saida<<"\tpush "<<buffer_completo[i+1]<<endl;
                            saida<<"\tcall EscreverChar"<<endl;
                        }
               
                    }
                    ////////////////////////////////////////////////////////////////

                    ///////////////////////////////S_INPUT///////////////////////////
                    if(buffer_completo[i].compare(S_INPUT)==0)
                    {
                        
                        if(mais!=string::npos)
                        {
                            PRIMEIRO_OPERANDO_FLAG = true; 
                        }
                        if(more!=string::npos)
                        {
                            SEGUNDO_OPERANDO_FLAG = true;
                        }

                        if(PRIMEIRO_OPERANDO_FLAG & SEGUNDO_OPERANDO_FLAG)
                        {
                            
                            S_INPUT_FLAG = true;
                            string vet = buffer_completo[i+2].substr(more,string::npos);
                            string lab = buffer_completo[i+2].substr(0,more);
                            buffer_completo[i+2]=lab;
                            int ind = Transforma_para_int(vet);
                            ind = 4*ind;
                            string vet_assembly = Transforma_para_String(ind);
                            saida<<"\tmov edx,0\n\tmov edx,"<<buffer_completo[i+2]<<"\n\tadd edx,"<<vet_assembly<<endl;
                            saida<<"\tpush edx"<<endl;    
                            string vetor = buffer_completo[i+1].substr(mais,string::npos);
                            string label =  buffer_completo[i+1].substr(0,mais);
                            buffer_completo[i+1]= label;
                            int indice = Transforma_para_int(vetor);
                            indice = 4*indice;
                            string vetor_assembly = Transforma_para_String(indice);
                            saida<<"\tmov ecx,0\n\tmov ecx,"<<buffer_completo[i+1]<<"\n\tadd ecx,"<<vetor_assembly<<endl;
                            saida<<"\tpush ecx"<<endl;
                            saida<<"\tcall LerString"<<endl;                                                    
                        }
                        else if(PRIMEIRO_OPERANDO_FLAG)
                        {
                            
                            S_INPUT_FLAG = true;
                            string vetor = buffer_completo[i+1].substr(mais,string::npos);
                            string label =  buffer_completo[i+1].substr(0,mais);
                            buffer_completo[i+1]= label;
                            int indice = Transforma_para_int(vetor);
                            indice = 4*indice;
                            string vetor_assembly = Transforma_para_String(indice);
                            saida<<"push "<<buffer_completo[i+2]<<endl;
                            saida<<"\tmov ecx,0\n\tmov ecx,"<<buffer_completo[i+1]<<"\n\tadd ecx,"<<vetor_assembly<<endl;
                            saida<<"\tpush ecx"<<endl;
                            saida<<"\tcall LerString"<<endl;
                        }
                        else if(SEGUNDO_OPERANDO_FLAG)
                        {   
                            
                            S_INPUT_FLAG = true;
                            string vetor = buffer_completo[i+2].substr(more,string::npos);
                            string label =  buffer_completo[i+2].substr(0,more);
                            buffer_completo[i+2]= label;
                            int indice = Transforma_para_int(vetor);
                            indice = 4*indice;
                            string vetor_assembly = Transforma_para_String(indice);
                            saida<<"\tmov ecx,0\n\tmov ecx,"<<buffer_completo[i+2]<<"\n\tadd ecx,"<<vetor_assembly<<endl;
                            saida<<"\tpush ecx"<<endl;
                            saida<<"push "<<buffer_completo[i+1]<<endl;
                            saida<<"\tcall LerString"<<endl;   
                        }
                        else
                        {
                            
                            S_INPUT_FLAG = true;
                            saida<<"\tpush "<<buffer_completo[i+2]<<endl;
                            saida<<"\tpush "<<buffer_completo[i+1]<<endl;
                            saida<<"\tcall LerString"<<endl;
                        }
               
                    }
                    ////////////////////////////////////////////////////////////////

                    ///////////////////////////////S_OUTPUT///////////////////////////
                    if(buffer_completo[i].compare(S_OUTPUT)==0)
                    {
                        
                        if(mais!=string::npos)
                        {
                            PRIMEIRO_OPERANDO_FLAG = true; 
                        }
                        if(more!=string::npos)
                        {
                            SEGUNDO_OPERANDO_FLAG = true;
                        }

                        if(PRIMEIRO_OPERANDO_FLAG & SEGUNDO_OPERANDO_FLAG)
                        {
                            
                            S_OUTPUT_FLAG = true;
                            string vet = buffer_completo[i+2].substr(more,string::npos);
                            string lab = buffer_completo[i+2].substr(0,more);
                            buffer_completo[i+2]=lab;
                            int ind = Transforma_para_int(vet);
                            ind = 4*ind;
                            string vet_assembly = Transforma_para_String(ind);
                            saida<<"\tmov edx,0\n\tmov edx,"<<buffer_completo[i+2]<<"\n\tadd edx,"<<vet_assembly<<endl;
                            saida<<"\tpush edx"<<endl;    
                            string vetor = buffer_completo[i+1].substr(mais,string::npos);
                            string label =  buffer_completo[i+1].substr(0,mais);
                            buffer_completo[i+1]= label;
                            int indice = Transforma_para_int(vetor);
                            indice = 4*indice;
                            string vetor_assembly = Transforma_para_String(indice);
                            saida<<"\tmov ecx,0\n\tmov ecx,"<<buffer_completo[i+1]<<"\n\tadd ecx,"<<vetor_assembly<<endl;
                            saida<<"\tpush ecx"<<endl;
                            saida<<"\tcall EscreverString"<<endl;                                                    
                        }
                        else if(PRIMEIRO_OPERANDO_FLAG)
                        {
                            
                            S_OUTPUT_FLAG = true;
                            string vetor = buffer_completo[i+1].substr(mais,string::npos);
                            string label =  buffer_completo[i+1].substr(0,mais);
                            buffer_completo[i+1]= label;
                            int indice = Transforma_para_int(vetor);
                            indice = 4*indice;
                            string vetor_assembly = Transforma_para_String(indice);
                            saida<<"push "<<buffer_completo[i+2]<<endl;
                            saida<<"\tmov ecx,0\n\tmov ecx,"<<buffer_completo[i+1]<<"\n\tadd ecx,"<<vetor_assembly<<endl;
                            saida<<"\tpush ecx"<<endl;
                            saida<<"\tcall EscreverString"<<endl;
                        }
                        else if(SEGUNDO_OPERANDO_FLAG)
                        {   
                            
                            S_OUTPUT_FLAG = true;
                            string vetor = buffer_completo[i+2].substr(more,string::npos);
                            string label =  buffer_completo[i+2].substr(0,more);
                            buffer_completo[i+2]= label;
                            int indice = Transforma_para_int(vetor);
                            indice = 4*indice;
                            string vetor_assembly = Transforma_para_String(indice);
                            saida<<"\tmov ecx,0\n\tmov ecx,"<<buffer_completo[i+2]<<"\n\tadd ecx,"<<vetor_assembly<<endl;
                            saida<<"\tpush ecx"<<endl;
                            saida<<"push "<<buffer_completo[i+1]<<endl;
                            saida<<"\tcall EscreverString"<<endl;   
                        }
                        else
                        {
                            
                            S_OUTPUT_FLAG = true;
                            saida<<"\tpush "<<buffer_completo[i+2]<<endl;
                            saida<<"\tpush "<<buffer_completo[i+1]<<endl;
                            saida<<"\tcall EscreverString"<<endl;
                        }
                    }
                    ////////////////////////////////////////////////////////////////

                    //////////////////////////STOP/////////////////////////////////
                    if(buffer_completo[i].compare(STOP)==0) 
                    {
                        saida<<"\tmov eax,1\n\tmov ebx,0\n\tint 80h"<<endl<<endl;
                    }

                    ///////////////////////////////////////////////////////////////
                PRIMEIRO_OPERANDO_FLAG = false;
                SEGUNDO_OPERANDO_FLAG = false;
                }break;
            }
        }       
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        ////////////////////IMPRIMINDO BUFFER////////////////////////
        /*for(unsigned int i =0; i< buffer_completo.size();i++)
        {
            cout<<"buffer_completo["<<i<<"]: "<<buffer_completo[i]<<endl;
            
        }
        for(unsigned int i=0;i<buffer_rotulo.size();i++)
        {
            cout<<"buffer_rotulo["<<i<<"]: "<<buffer_rotulo[i]<<endl;
        }*/
        ////////////////////////////////////////////////////////////


        //////////////////////////////////////////////////IMPRIMINDO FUNÇÕES DO CALL////////////////////////////////////////////////////////////////////////////////////////////


        //////////////////////LER INTEIRO///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if(INPUT_FLAG){

            saida<<"LerInteiro:"<<endl;
            saida<<"\tenter 24,0\n\txor eax,eax\n\tpush ebx\n\tpush ecx\n\tpush edx\n\tpush edi\n\tpush esi"<<endl;
            saida<<"\tmov eax,3\n\tmov ebx,0\n\tmov edx,12\n\tmov ecx,ebp\n\tsub ecx,18\n\tint 80h"<<endl;
            saida<<"\tmov [ebp-24],eax\n\tmov ecx,eax\n\tsub ecx,1 \n\tmov edi,0\n\tmov esi,10\n\tmov eax,0\n\tcmp byte [ebp-18],0x2D\n\tje negativo\n\tmov word [ebp-20], 0"<<endl;
            saida<<"retorna:"<<endl;
            saida<<"\tcmp byte [ebp-18+edi],0x39\n\tjg erro\n\tcmp byte [ebp-18+edi],0x30\n\tjl erro\n\tmov bl,[ebp-18+edi]\n\tcmp  edi,0\n\tje soma\n\tcmp word [ebp-20],1\n\tje if_neg"<<endl;
            saida<<"retorna2:"<<endl;                        
            saida<<"\tmul esi"<<endl;
            saida<<"soma:"<<endl;
            saida<<"\tadd eax,ebx\n\tsub eax,0x30\n\tsub ecx,1\n\tcmp ecx,0\n\tje conf_neg\n\tinc edi\n\tjmp retorna"<<endl;
            saida<<"if_neg:"<<endl;
            saida<<"\tcmp edi,1\n\tje soma\n\tjmp retorna2"<<endl;
            saida<<"negativo:"<<endl;
            saida<<"\tmov word [ebp-20], 1\n\tinc edi\n\tdec ecx\n\tjmp retorna"<<endl;
            saida<<"conf_neg:"<<endl;
            saida<<"\tcmp word [ebp-20],0\n\tje guarda\n\tneg eax"<<endl;
            saida<<"guarda:"<<endl;
            saida<<"\tmov ebx,[EBP+8]\n\tmov dword [ebx],eax\n\tjmp final"<<endl;
            saida<<"erro:"<<endl;
            saida<<"\tmov eax,4\n\tmov ebx,1\n\tmov ecx,incorreto\n\tmov edx,28\n\tint 80h\n\tmov eax,1\n\tmov ebx,0\n\tint 80h"<<endl;
            saida<<"final:"<<endl;
            saida<<"\tmov eax,[ebp-24]\n\tpop esi\n\tpop edi\n\tpop edx\n\tpop ecx\n\tpop ebx\n\tleave\n\tret 4"<<endl<<endl;

        }
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        //////////////////////ESCREVE INTEIRO///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if(OUTPUT_FLAG){

            saida<<"EscreverInteiro:"<<endl;
            saida<<"\tenter 16,0\n\txor eax,eax\n\tpush ebx\n\tpush ecx\n\tpush edx\n\tpush edi\n\tpush esi"<<endl;
            saida<<"\tmov edi,[ebp+8]\n\tmov eax,[edi]\n\tmov edi,0\n\tmov ecx,0\n\tmov ebx,10\n\tcmp eax,0\n\tje zero\n\tcmp eax,0\n\tjg positivo_EI"<<endl;
            saida<<"negativo_EI:"<<endl;
            saida<<"\tmov ecx,1\n\tneg eax"<<endl;
            saida<<"positivo_EI:"<<endl;
            saida<<"\tcdq\n\tcmp eax,0\n\tje conf_neg_EI\n\tdiv ebx\n\tadd edx,0x30\n\tmov [ebp-12+edi],edx\n\tinc edi\n\tmov [ebp-16],edi\n\tjmp positivo_EI"<<endl;
            saida<<"conf_neg_EI:"<<endl;
            saida<<"\tcmp ecx,0\n\tje imprime_EI\n\tmov byte [ebp-12+edi],0x2D\n\tinc edi\n\tjmp inicio_imprime"<<endl;
            saida<<"zero:"<<endl;
            saida<<"\tadd eax,0x30\n\tmov [ebp-12],eax\n\tinc edi\n\tmov [ebp-16],edi\n\tjmp inicio_imprime"<<endl;
            saida<<"inicio_imprime:"<<endl;
            saida<<"\tmov esi,edi"<<endl;
            saida<<"imprime_EI:"<<endl;
            saida<<"\tmov eax,4\n\tmov ebx,1\n\tmov ecx,ebp\n\tsub ecx,12\n\tadd ecx,edi\n\tmov edx,1\n\tint 80h\n\tcmp edi,0\n\tje fim_imprime\n\tdec edi\n\tjmp imprime_EI"<<endl;
            saida<<"fim_imprime:"<<endl;
            saida<<"\tcmp ecx,1\n\tje final_EI\n\tmov eax,4\n\tmov ebx,1\n\tmov ecx,newline\n\tmov edx,NWLINESIZE\n\tint 80h"<<endl;
            saida<<"final_EI:"<<endl;
            saida<<"\tmov eax,[ebp-16]\n\tinc eax\n\tpop esi\n\tpop edi\n\tpop edx\n\tpop ecx\n\tpop ebx\n\tleave\n\tret 4"<<endl<<endl;

        }
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        //////////////////////LER CHAR/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if(C_INPUT_FLAG){

            saida<<"LerChar:"<<endl;
            saida<<"\tenter 10,0\n\txor eax,eax\n\tpush ebx\n\tpush ecx\n\tpush edx\n\tpush edi\n\tpush esi"<<endl;
            saida<<"\tmov eax,3\n\tmov ebx,0\n\tmov ecx,ebp\n\tsub ecx,8\n\tmov edx,10\n\tint 80h\n\tmov eax,[ebp-8]\n\tmov ebx,[EBP+8]\n\tmov dword [ebx],eax"<<endl;
            saida<<"\tmov eax,1\n\tpop esi\n\tpop edi\n\tpop edx\n\tpop ecx\n\tpop ebx\n\tleave\n\tret 4"<<endl<<endl;


        }
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        //////////////////////ESCREVE CHAR///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if(C_OUTPUT_FLAG){

            saida<<"EscreverChar:"<<endl;
            saida<<"\tenter 10,0\n\txor eax,eax\n\tpush ebx\n\tpush ecx\n\tpush edx\n\tpush edi\n\tpush esi"<<endl;
            saida<<"\tmov edi,[ebp+8]\n\tmov eax,4\n\tmov ebx,1\n\tmov ecx,edi\n\tmov edx,1\n\tint 80h\n\tmov eax,4\n\tmov ebx,1\n\tmov ecx,newline\n\tmov edx,NWLINESIZE\n\tint 80h"<<endl;
            saida<<"mov eax,2\n\tpop esi\n\tpop edi\n\tpop edx\n\tpop ecx\n\tpop ebx\n\tleave\n\tret 4"<<endl<<endl;

        }
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        //////////////////////LER STRING///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if(S_INPUT_FLAG){

            saida<<"LerString:"<<endl;
            saida<<"\tenter 10,0\n\txor eax,eax\n\tpush ebx\n\tpush ecx\n\tpush edx\n\tpush edi\n\tpush esi\n\tmov edi,[ebp+8]\n\tmov esi,[ebp+12]\n\tmov byte [ebp-10],0"<<endl;
            saida<<"ler_LS:"<<endl;
            saida<<"\tmov eax,3\n\tmov ebx,0\n\tmov edx,1\n\tmov ecx,edi\n\tint 80h\n\tcmp byte [edi],0ah\n\tje final_LS\n\tinc edi\n\tinc byte [ebp-4]\n\tcmp [ebp-4],esi\n\tje final_LS\n\tjmp ler_LS"<<endl;
            saida<<"final_LS:"<<endl;
            saida<<"\tmov eax,[ebp-4]\n\tpop esi\n\tpop edi\n\tpop edx\n\tpop ecx\n\tpop ebx\n\tleave\n\tret 4"<<endl<<endl;
        }
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        //////////////////////ESCREVE STRING//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if(S_OUTPUT_FLAG){

            saida<<"EscreverString:"<<endl;
            saida<<"\tenter 10,0\n\txor eax,eax\n\tpush ebx\n\tpush ecx\n\tpush edx\n\tpush edi\n\tpush esi\n\tmov edi,[ebp+8]\n\tmov esi,[ebp+12]\n\tmov byte [ebp-10],0"<<endl;
            saida<<"escreve_ES:"<<endl;
            saida<<"\tmov eax,4\n\tmov ebx,1\n\tmov edx,1\n\tmov ecx,edi\n\tint 80h\n\tcmp byte [edi],0ah\n\tje final_LS\n\tinc edi\n\tinc byte [ebp-4]\n\tcmp [ebp-4],esi\n\tje final_ES\n\tjmp escreve_ES"<<endl;
            saida<<"final_ES:"<<endl;
            saida<<"\tmov eax,[ebp-4]\n\tpop esi\n\tpop edi\n\tpop edx\n\tpop ecx\n\tpop ebx\n\tleave\n\tret 4"<<endl<<endl;
            

        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
        
       

    }     
}

// Método para Pegar os tokens de cada linha
vector<string> traducao::pegar_tokens(string linha)
{
    string frase;
    stringstream ss(linha);
    vector<string> buffer_tokens;
 
    while (ss >> frase)
        buffer_tokens.push_back(frase);
 
    return(buffer_tokens);
}

// Método para Remover comentários
string traducao::Remover_Comentarios(string frase)
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
string traducao::NaoSensivelAoCaso(string frase)
{
 
    for(unsigned int i=0; i < frase.size(); i++)
    {
 
       frase[i]=toupper(frase[i]);
    }
    return(frase);
}

//Método para transformar int para string
string traducao::Transforma_para_String(int numero)
{
     ostringstream ss;
     ss << numero;

     return ss.str();
}
// Método para transformar string para int
int traducao::Transforma_para_int(string vetor)
{
    return(atoi(vetor.c_str()));
}
