#include <iostream>
#include <string>
#include <stdlib.h>


#include "traducao.h"

using namespace std;

int main(int argc, char *argv[]){

	
	string ArquivoEntrada, ArquivoSaida;

	if(argc == 2)
	{
		ArquivoEntrada = argv[1];

		//Instanciando a classe traducao
		traducao* traduz = new traducao();
          
        traduz->leitura(ArquivoEntrada);// Chama método leitura da classe traducao		

	}

	else
	{
		cout<<"Entrada Errada. Argumentos Inválidos.\n\n";
		
	}
}