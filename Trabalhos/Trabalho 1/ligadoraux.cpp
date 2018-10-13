/********************************************************************************
* Arquivo   : ligadoraux.h
* Autores   : Lucas De Macedo Pinheiro
               M

* Contato    : lucasmp10@hotmail.com



* Data      : 2018-10-12
* Objetivo  : Arquivo de definicoes da classe auxiliar do ligador
* Notas     : Ligador feito para o trabalho de Software Basico do 2/2018 na UnB
********************************************************************************/

#include "ligadoraux.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

LigadorAux::LigadorAux() {}

// Construtor principal
LigadorAux::LigadorAux(vector<string> arquivos) :
// Lista de inicializacao
	_quantos_arquivos((int)arquivos.size()), _nomes_arquivos(arquivos)
{}

LigadorAux::~LigadorAux() {}



// Funcao principal para ligar os arquivos
int LigadorAux::liga() {
	
	if(_nomes_arquivos.empty()){
		cout << "Nao foi possivel ler os arquivos passados.";
		return 1;
	}
	
	for(int c=0; c<_quantos_arquivos; c++){
		ifstream temp(_nomes_arquivos[c]);
		
	}
	
	return 1;
}

// Funcao para salvar o arquivo executavel
int LigadorAux::salva() {

	ofstream executavel(_nomes_arquivos[0] + ".e");

	//ofile << ;
	
	return 1;
}






