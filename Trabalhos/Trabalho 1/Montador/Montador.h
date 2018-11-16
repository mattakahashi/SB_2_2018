#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
#include <stack>
#include <stdbool.h>
#include <cstddef> 
#include <sstream>
#include <stdlib.h> 
#include <iomanip>
#include <typeinfo>
#include <map>
#include <iterator>
#include <cstring>

using namespace std;

//Define a Classe Pre_Processamento
class pre_processamento
{
public:

	pre_processamento(); // Chama Construtor da Classe
	~pre_processamento(); // Destrutor da Classe
	void leitura(string ArquivoEntrada);// Método responsável pela leitura do arquivo .asm
	vector<string> pegar_tokens(string linha);// Método responsável pela separação dos tokens a partir de cada linha do arquivo
	map<string,int> pegaLinha(string linha, int numero_linha);// Método que retorna a linha em que cada token foi referenciado
	string Remover_Comentarios(string comment); // Chama Método Remover_Comentarios
	string NaoSensivelAoCaso(string frase);// Método que transforma todos caracteres do arquivo para somente um tipo de case
	string Transforma_para_String(int numero);// Método que transforma um int para string
	int Transforma_para_int(string vetor);// Método que transforma uma string para int
	
	
	
private:
	map<string,int>tabela_tokens;
	vector<string>  buffer_tokens, buffer_rotulo, Nome_Rot_EQU, Valor_EQU, buffer_completo,linha_equ,linha_equ1,linha_if;
	string linha;
	string frase, ArquivoEntrada, ArquivoPre, ArquivoOBJ;
	int nlinha,IF1;
	bool ERRO_FLAG_1, ERRO_FLAG,ERRO_FLAG_IF,ERRO_FLAG_EQU;
};

class primeira_passagem
{
public:

	primeira_passagem(); // Chama Construtor da Classe
	~primeira_passagem(); // Destrutor da Classe
	void leitura(string ArquivoEntrada); // Método responsável pela leitura do arquivo .asm e tratamento de erros no processo de montagem
	
	//string Transforma_para_String(int numero);// Método que transforma um int para string
	//int Transforma_para_int(string vetor);// Método que transforma uma string para int
	


	
	
private:
	map<string,int>tabela_tokens;
	vector<string>  buffer_tokens, buffer_rotulo, Nome_Rot_EQU, Valor_EQU, buffer_completo,linha_cop, linha_invalid_section;
	string linha;
	string frase, ArquivoPre;
	int nlinha,linha_text, linha_data,linha_bss;
	bool EQU_FLAG, ERRO_FLAG, Erro_Flag_TEXT, ERRO_NO_TEXT, ERRO_NO_DATA, ERRO_COPY,ERRO_INVALID_SECTION;
};

//Define a Classe Pre_Processamento
class segunda_passagem
{
public:

	segunda_passagem(); // Chama Construtor da Classe
	~segunda_passagem(); // Destrutor da Classe
	void montar(string ArquivoEntrada);// Método responsável pela leitura do arquivo .pre e construção das tabelas de uso, definições e montagem do código objeto
	int converte_hexa(string hexa); // Método que transforma a CONST escrita em hexadecimal para decimal 

	
	
private:
	vector<string>  buffer_tokens, buffer_rotulo, Nome_Rot_EQU, Valor_EQU, buffer_completo,buffer_auxiliar;
	string linha;
	string frase, ArquivoEntrada, ArquivoPre, ArquivoOBJ;
	int nlinha,IF1,linha_equ;
	bool EQU_FLAG, ERRO_FLAG,flag_com_modulo;
};