/*
///////////////////////////////////////////////////////////////////////// Classe Principal////////////////////////////////////////////////////////////////////////////////////////////////////////////

Recebe 3 Argumentos na Linha de Comando do Linux e chama as outras Classes de Acordo com cada um dos Argumentos
Por exemplo: .\Macro_Assembler teste.asm -p (Roda o executável Macro_assembler, abre o arquivo teste.asm e monta ele de acordo com "-p", descrito no roteiro do projeto)
Se entrada for algo diferente que a estrutura citada acima, imprime uma mensagem de erro. Além disso, se o usuário digitar algum tipo de operação que não foi definido, tb da gera uma mensagem de erro

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/
#include <iostream>
#include <string>
#include <stdlib.h>


#include "Montador.h"

using namespace std;

int main(int argc, char *argv[]){

	
	string ArquivoEntrada;

	if(argc == 2)
	{
		ArquivoEntrada = argv[1]; // Pega o segundo argumento (Nome do arquivo .asm) 

		pre_processamento* pre = new pre_processamento(); // Instancia a classe pre_processamento e cria o objeto pre, para manipularmos elementos dela 	          
	    pre->leitura(ArquivoEntrada); // Chama o método leitura e passa o nome do arquivo que vai ser lido no pre-processamento

	}

	else
	{
		cout<<"Entrada Errada. Argumentos Inválidos.\n\n";
		
	}
}