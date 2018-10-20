/********************************************************************************
* Arquivo   : modulus.h
* Autores   : Lucas De Macedo Pinheiro
			   M

* Contato	 : lucasmp10@hotmail.com



* Data      : 2018-10-12
* Objetivo  : Arquivo de cabecalho da classe auxiliar de modulo para o ligador
* Notas     : Ligador feito para o trabalho de Software Basico do 2/2018 na UnB
********************************************************************************/

#include <map>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

#ifndef MODULUS_H
#define MODULUS_H

class Modulus {
	
private:
	int _tamanho, _correcao;
	string _nome_mod;
	vector<int> _relativos;
	vector<int> _campo_codigo;
	map<string,vector<int>> _tabela_uso;
	map<string,int> _tabela_definicoes;
	
public:
	
	Modulus();
	~Modulus();
	
	// Construtor principal
	Modulus(ifstream &arquivo, string nome);
	
	// Leitura do arquivo para criacao dos campos (eg. tabelas de uso/definicoes)
	// Supoe formatacao padrao:
	// ------------------------- //
	// TABLE USE
	// ROT1 11
	// ROT1 15
	// ROT2 18
	//
	// TABLE DEFINITION
	// ROT3 4
	//
	// RELATIVE
	// 1 3 4
	//
	// CODE
	// 14 12 12 15 04 14 12 5
	// ------------------------- //
	void categoriza(ifstream &arquivo);
	
	// Retorna a tabela de uso do respectivo modulo
	map<string,vector<int>> passa_tabela_uso();
	
	// Retorna a tabela de definicoes do respectivo modulo
	map<string,int> passa_tabela_definicoes();
	
	// Retorna o tamanho do modulo
	int passa_tamanho();
	
	// Retorna o valor do fator de correcao dos enderecos
	int passa_correcao();
	
	// Salva o fator de correcao dos enderecos
	void corrige(int quanto);
	
	// Salva o fator de correcao dos enderecos
	vector<int> passa_codigo();
};

#endif





