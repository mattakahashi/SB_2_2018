/********************************************************************************
* Arquivo   : ligadoraux.cpp
* Autores   : Lucas De Macedo Pinheiro
			  Matheus Takahashi

* Contato    : lucasmp10@hotmail.com



* Data      : 2018-10-21
* Objetivo  : Arquivo de definicoes da classe auxiliar do ligador
* Notas     : Ligador feito para o trabalho de Software Basico do 2/2018 na UnB
********************************************************************************/

#include "ligadoraux.h"
#include "modulus.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
using namespace std;

LigadorAux::LigadorAux() {}
LigadorAux::~LigadorAux() {}

LigadorAux::LigadorAux(vector<string> arquivos) : // Lista de inicializacao
	_quantos_arquivos((int)arquivos.size()),_str_saida(""),_nomes_arquivos(arquivos)
{}

int LigadorAux::liga() {
	
	// Nome para exportar o arquivo de saida
	_arquivo_principal = _nomes_arquivos[0];
	
	if(_quantos_arquivos == 1){ // Um so arquivo. Sem modulos
		ifstream temp(_arquivo_principal);
		getline(temp, _str_saida); // Simplesmente uma copia do arquivo
		return 1;
	}
	// Dicionario de objetos Modulus
	if(!cria_dicio())	return 0;
	
	// Tabela global de definicoes
	cria_tabela();
	
	referencias_x();
	
	return 1;
}

int LigadorAux::salva() {

	ofstream executavel(_arquivo_principal + ".e");
	executavel << _str_saida;
	
	return 1;
}

int LigadorAux::cria_dicio(){
	
	for(int c=0; c<_quantos_arquivos; c++){
		ifstream temp(_nomes_arquivos[c]);
		
		if(temp.fail()){
			cerr << "Erro ao ler o arquivo: " << _nomes_arquivos[c] << endl;
			return 0;
		}
		else	_dicio_modulos[c] = Modulus(temp,_nomes_arquivos[c]);
	}
	return 1;
}

int LigadorAux::cria_tabela(){
	
	for(map<int,Modulus>::iterator it = _dicio_modulos.begin();
		it != _dicio_modulos.end(); ++it){
		
		map<string,int> tabela_temp = (*it).second.passa_tabela_definicoes();
		
		// Salva o fator de correcao de acordo com modulo anterior
		int fc = 0;
		if(it != _dicio_modulos.begin()){ // Se nao for o primeiro modulo
			fc = (*prev(it)).second.passa_tamanho() + (*prev(it)).second.passa_correcao();
			(*it).second.corrige(fc);
		}
		for(map<string,int>::iterator it2 = tabela_temp.begin();
			it2 != tabela_temp.end(); ++it2){
			(*it2).second += fc;
			_tabela_global.insert(*it2);
		}
	}
	return 1;
}

int LigadorAux::referencias_x(){
	
	for(map<int,Modulus>::iterator it = _dicio_modulos.begin();
		it != _dicio_modulos.end(); ++it){
		
		vector<int> codigo_temp = (*it).second.passa_codigo();
		map<string,vector<int>> tabela_temp = (*it).second.passa_tabela_uso();
		
		// Iteracao na tabela de uso do respectivo modulo
		for(map<string,vector<int>>::iterator it2 = tabela_temp.begin();
			it2 != tabela_temp.end(); ++it2){
			// Valor de ajuste do rotulo na tabela global
			int ajuste = (*_tabela_global.find((*it2).first)).second;
			
			for(unsigned int h = 0; h < (*it2).second.size(); h++)
				codigo_temp[(*it2).second[h]] += ajuste;
		}
		// Conversao do codigo objeto em vetor para string de saida
		for (vector<int>::iterator it3 = codigo_temp.begin();
			it3 != codigo_temp.end(); it3++ ) {
			stringstream ss;
			ss << (*it3) << " ";
			_str_saida += ss.str();
		}
	}
	return 1;
}






							 
							 
							 
							 
							 
							 

