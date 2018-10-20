/********************************************************************************
* Arquivo   : ligadoraux.h
* Autores   : Lucas De Macedo Pinheiro
			  Matheus Takahashi

* Contato	 : lucasmp10@hotmail.com



* Data      : 2018-10-21
* Objetivo  : Arquivo de cabecalho da classe auxiliar do ligador
* Notas     : Ligador feito para o trabalho de Software Basico do 2/2018 na UnB
********************************************************************************/

#include "modulus.h"
#include <vector>
#include <string>
#include <map>
using namespace std;

#ifndef LIGADORAUX_H
#define LIGADORAUX_H

class LigadorAux {
	
private:
	
	int _quantos_arquivos;
	string _arquivo_principal;
	string _str_saida;
	vector<string> _nomes_arquivos;
	map<int,Modulus> _dicio_modulos;
	map<string,int> _tabela_global;
	
public:
	
	LigadorAux();
	~LigadorAux();
	
	// Construtor principal
	LigadorAux(vector<string> arquivos);
	
	// Funcao principal para ligar os arquivos
	int liga();
	
	// Funcao para salvar o arquivo executavel
	int salva();
	
	// Criacao do dicionario de modulos
	int cria_dicio();
	
	// Criacao da tabela global de definicoes
	int cria_tabela();
	
	// Ajuste dos codigos objeto para os enderecos corretos
	int referencias_x();
};

#endif





