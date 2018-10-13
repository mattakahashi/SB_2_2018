/********************************************************************************
* Arquivo   : ligadoraux.h
* Autores   : Lucas De Macedo Pinheiro
			   M

* Contato	 : lucasmp10@hotmail.com



* Data      : 2018-10-12
* Objetivo  : Arquivo de cabecalho da classe auxiliar do ligador
* Notas     : Ligador feito para o trabalho de Software Basico do 2/2018 na UnB
********************************************************************************/

#include <vector>
#include <string>
using namespace std;

#ifndef LIGADORAUX_H
#define LIGADORAUX_H

class LigadorAux {
	
private:
	
	int _quantos_arquivos;
	string _arquivo_principal;
	vector<string> _nomes_arquivos;
	
public:
	
	LigadorAux();
	LigadorAux(vector<string> arquivos);
	~LigadorAux();
	
	int liga();
	int salva();
};

#endif





