#include <iostream>
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
	void leitura(string ArquivoEntrada);
	vector<string> pegar_tokens(string linha, int numero_linha);
	string Remover_Comentarios(string comment); // Chama Método Remover_Comentarios
	string NaoSensivelAoCaso(string frase);
	string Transforma_para_String(int numero);
	int Transforma_para_int(string vetor);
	int Pega_linha(int teste);
	

	
	
private:
	vector<string>  buffer_tokens, buffer_rotulo, Nome_Rot_EQU, Valor_EQU, buffer_completo;
	string linha;
	string frase, ArquivoEntrada, ArquivoPre, ArquivoOBJ;
	int nlinha,IF1,linha_equ;
	bool EQU_FLAG, ERRO_FLAG;
};

class primeira_passagem
{
public:

	primeira_passagem(); // Chama Construtor da Classe
	~primeira_passagem(); // Destrutor da Classe
	void leitura(string ArquivoEntrada);
	vector<string> pegar_tokens(string linha, int numero_linha);	

	
	
private:
	vector<string>  buffer_tokens, buffer_rotulo, Nome_Rot_EQU, Valor_EQU, buffer_completo;
	string linha;
	string frase, ArquivoPre;
	int nlinha,linha_text, linha_data,linha_bss,linha_cop;
	bool EQU_FLAG, ERRO_FLAG, Erro_Flag_TEXT, ERRO_NO_TEXT, ERRO_NO_DATA, ERRO_COPY;
};

//Define a Classe Pre_Processamento
class segunda_passagem
{
public:

	segunda_passagem(); // Chama Construtor da Classe
	~segunda_passagem(); // Destrutor da Classe
	void leitura(string ArquivoPre);

	
	
private:
	vector<string>  buffer_tokens, buffer_rotulo, Nome_Rot_EQU, Valor_EQU, buffer_completo;
	string linha;
	string frase, ArquivoEntrada, ArquivoPre, ArquivoOBJ;
	int nlinha,IF1,linha_equ;
	bool EQU_FLAG, ERRO_FLAG;
};