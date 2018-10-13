all:
	g++ -std=c++11 -Wall -c -o Pre_Processador.o Pre_Processador.cpp
	
	g++ -std=c++11 -Wall -c -o primeira_passagem.o primeira_passagem.cpp

	g++ -std=c++11 -Wall -c -o segunda_passagem.o segunda_passagem.cpp

	g++ -std=c++11 -Wall -c -o montador.o montador.cpp

	g++ -std=c++11 -Wall -o montador Pre_Processador.o primeira_passagem.o segunda_passagem.o montador.o

	g++ -std=c++11 -Wall -c -o ligadoraux.o ligadoraux.cpp

	g++ -std=c++11 -Wall -c -o ligador.o ligador.cpp

	g++ -std=c++11 -Wall -o ligador ligadoraux.o ligador.o