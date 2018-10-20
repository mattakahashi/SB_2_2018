/********************************************************************************
* Arquivo   : modulus.cpp
* Autores   : Lucas De Macedo Pinheiro
			  Matheus Takahashi

* Contato	 : lucasmp10@hotmail.com



* Data      : 2018-10-21
* Objetivo  : Arquivo de definicoes da classe auxiliar de modulo para o ligador
* Notas     : Ligador feito para o trabalho de Software Basico do 2/2018 na UnB
********************************************************************************/

#include "modulus.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

Modulus::Modulus(){}
Modulus::~Modulus(){}

Modulus::Modulus(ifstream &arquivo, string nome) : // Lista de inicializacao
	_tamanho(0), _correcao(0), _nome_mod(nome)
{
	categoriza(arquivo);
}

void Modulus::categoriza(ifstream &arquivo){
	
	arquivo.ignore('\n');
	
	string linha;
	getline(arquivo, linha);
	
	// Tabela de Uso
	string rotulo;
	int valor;
	
	while(!linha.empty()){ // Continua ate linha em branco
		stringstream temp_ss(linha);
		
		if(temp_ss >> rotulo >> valor)
			_tabela_uso[rotulo].push_back(valor);
		getline(arquivo, linha);
	}
	getline(arquivo, linha); // Pula para proxima secao
	getline(arquivo, linha);
	
	// Tabela de Definicoes
	while(!linha.empty()){ // Continua ate linha em branco
		stringstream temp_ss(linha);
		
		if(temp_ss >> rotulo >> valor)
			_tabela_definicoes[rotulo] = valor;
		getline(arquivo, linha);
	}
	getline(arquivo, linha); // Pula para proxima secao
	
	// Secao de enderecos relativos
	int v;
	
	while(arquivo >> v)
		_relativos.push_back(v);
	arquivo.clear(); // Evita problemas com o '\n' remanescente
	getline(arquivo, linha);  // Pula para proxima secao
	
	// Secao Codigo
	while(arquivo >> v)
		_campo_codigo.push_back(v);
	_tamanho = (int)_campo_codigo.size();
}

map<string,vector<int>> Modulus::passa_tabela_uso(){
	return _tabela_uso;
}

map<string,int> Modulus::passa_tabela_definicoes(){
	return _tabela_definicoes;
}

int Modulus::passa_tamanho(){
	return _tamanho;
}

int Modulus::passa_correcao(){
	return _correcao;
}

void Modulus::corrige(int quanto){
	_correcao = quanto;
}

vector<int> Modulus::passa_codigo(){
	return _campo_codigo;
}