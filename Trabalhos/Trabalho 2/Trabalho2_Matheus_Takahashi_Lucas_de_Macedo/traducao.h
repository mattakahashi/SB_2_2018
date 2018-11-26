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

using namespace std;

//Define a Classe Pre_Processamento
class traducao
{
public:

	traducao(); // Chama Construtor da Classe
	~traducao(); // Destrutor da Classe
	void leitura(string ArquivoEntrada);
	vector<string> pegar_tokens(string linha);
	string Remover_Comentarios(string comment); // Chama Método Remover_Comentarios
	string NaoSensivelAoCaso(string frase);
	string Transforma_para_String(int numero);
	int Transforma_para_int(string vetor);


	
	
private:
	vector<string>  buffer_tokens, buffer_rotulo, Nome_Rot_EQU, Valor_EQU, buffer_completo, buffer_data, buffer_bss, buffer_bss_valor,buffer_data_valor;
	string linha;
	string frase, ArquivoEntrada, ArquivoSaida;
	int nlinha,IF1,linha_equ;
	bool EQU_FLAG, SPACE_FLAG;
	bool INPUT_FLAG, OUTPUT_FLAG, C_INPUT_FLAG, C_OUTPUT_FLAG, S_INPUT_FLAG, S_OUTPUT_FLAG, H_INPUT_FLAG, H_OUTPUT_FLAG, PRIMEIRO_OPERANDO_FLAG, SEGUNDO_OPERANDO_FLAG;
};