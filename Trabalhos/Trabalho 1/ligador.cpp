/********************************************************************************
* Arquivo   : ligador.cpp
* Autores   : Lucas De Macedo Pinheiro
			   M

* Contato	 : lucasmp10@hotmail.com



* Data      : 2018-10-12
* Objetivo  : Arquivo de execucao do ligador
* Notas     : Ligador feito para o trabalho de Software Basico do 2/2018 na UnB
********************************************************************************/

#include "ligadoraux.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

int main(int argc, char* argv[])
{
	// Checa se os argumentos foram passados 
	if(argc < 2){
    	cerr << "Faltando argumentos no comando."
    	" Entrada: $ ./ligador arquivo1" << endl;
    	return 1;
  	}
	
  	// Vetor com nome de todos arquivos passados pelo ponteiro argv
  	vector<string> arquivos(argv+1, argv+argc);

	cout << "Arquivos para ligar: ";
 	for (vector<string>::iterator it=arquivos.begin(); it!=arquivos.end(); it++)
    	cout << (*it) << " ";
    cout << endl;

	LigadorAux L(arquivos);
	
	if (!L.liga()) { // Liga os arquivos e verifica erros
		if(!L.salva()) // Exporta o arquivo executavel e verifica erros
			return 0;
		else
			return 1;
	}
	else
		return 1;
	
	return 0;
}












